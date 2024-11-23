#include "Raytracer.h"
#include "BlinnPhongShader.h"
#include <iostream>
#include <algorithm>
#include <omp.h>
#include <chrono>
#include <cmath>

Raytracer::Raytracer(int width, int height, int samplesPerPixel)
    : image(width, height), samplesPerPixel(samplesPerPixel),
      rng(std::random_device{}()), distribution(0.0f, 1.0f)
{
    // Initialization if necessary
}

void Raytracer::render(const Scene &scene, const std::string &outputFilename)
{
    const Camera &camera = scene.camera;
    int width = camera.width;
    int height = camera.height;
    image = Image(width, height);

    bool isBinaryMode = scene.renderMode == "binary";
    bool isPathTracerMode = scene.renderMode == "pathtracer";

    // Initialize the shader once
    BlinnPhongShader shader(scene, scene.camera.position);

    // Start timing
    auto startTime = std::chrono::steady_clock::now();

    // Retrieve exposure from camera and calculate exposure scale with baseEV offset
    float exposure = camera.exposure;
    float baseEV = 1.2f;                                     // Exposure offset to ensure sufficient brightness
    float exposureScale = std::pow(2.0f, exposure + baseEV); // Adjusted scaling

    // Debug: Print samplesPerPixel
    std::cout << "Samples per pixel: " << samplesPerPixel << std::endl;

    // Iterate over each pixel with OpenMP parallelization
#pragma omp parallel for schedule(dynamic) shared(image)
    for (int y = 0; y < height; ++y)
    {
        // Initialize a local random number generator for each thread
        std::mt19937 local_rng(rng());
        std::uniform_real_distribution<float> local_distribution(0.0f, 1.0f);

        for (int x = 0; x < width; ++x)
        {
            Color pixelColor(0.0f, 0.0f, 0.0f);

            for (int s = 0; s < samplesPerPixel; ++s)
            {
                // Generate random offsets within the pixel
                float u = (static_cast<float>(x) + local_distribution(local_rng)) / width;
                float v = (static_cast<float>(y) + local_distribution(local_rng)) / height;

                // Generate random numbers for lens sampling
                float r1 = local_distribution(local_rng);
                float r2 = local_distribution(local_rng);

                // Generate the ray with jittered coordinates and lens sampling
                Ray ray = camera.generateRay(u * width, v * height, r1, r2);

                // Trace the ray and compute the color
                Color color;
                if (isBinaryMode)
                {
                    Intersection hit;
                    bool hasHit = scene.intersect(ray, hit);
                    if (hasHit)
                    {
                        color = Color(1.0f, 0.0f, 0.0f);
                    }
                    else
                    {
                        color = scene.backgroundColor;
                    }
                }
                else if (isPathTracerMode)
                {
                    color = pathTrace(ray, scene, 0);
                }
                else
                {
                    color = trace(ray, scene, shader, 0);
                }

                pixelColor += color;
            }

            // Average the color by the number of samples
            pixelColor /= static_cast<float>(samplesPerPixel);

            if (isBinaryMode)
            {
                // In binary mode, trace already returns red or background
                // So no need to modify color here
            }
            else
            {
                // Apply exposure adjustment using the adjusted exposure scale
                pixelColor *= exposureScale;

                // Apply simple linear tone mapping: clamp to [0, 1]
                pixelColor = pixelColor.clamp(0.0f, 1.0f);

                // Optionally, apply gamma correction for display purposes
                // Uncomment the following line if your display expects gamma-corrected colors
                // pixelColor = pixelColor.gammaCorrect(2.2f);
            }

            // Set the pixel color (assumes thread-safe)
            image.setPixel(x, y, pixelColor);
        }

        // Calculate and display progress every 100 lines or on the last line
#pragma omp critical
        {
            if (y % 100 == 0 || y == height - 1)
            {
                auto currentTime = std::chrono::steady_clock::now();
                std::chrono::duration<double> elapsed = currentTime - startTime;
                double progress = (static_cast<double>(y) / height) * 100.0;
                std::cout << "\rRendering Progress: " << progress << "% | Elapsed Time: " << elapsed.count() << "s" << std::flush;
            }
        }
    }

    std::cout << std::endl; // Move to the next line after progress output

    // Save the rendered image to the specified file
    if (image.saveAsPPM(outputFilename))
    {
        std::cout << "Rendering completed. Image saved to " << outputFilename << std::endl;
    }
    else
    {
        std::cerr << "Failed to save image to " << outputFilename << std::endl;
    }
}

Color Raytracer::trace(const Ray &ray, const Scene &scene, const BlinnPhongShader &shader, int depth) const
{
    // Existing trace logic for non-binary modes
    if (depth >= scene.nbounces)
        return Color(0, 0, 0); // Base case: no further contribution

    Intersection closestHit;
    bool hit = scene.intersect(ray, closestHit);

    if (hit)
    {
        // Compute the color at the intersection point using the shader
        Color shadedColor = shader.shade(closestHit);

        // Initialize accumulated color with the shaded color
        Color accumulatedColor = shadedColor;

        // Handle reflections if the material is reflective
        if (closestHit.material.isReflective && closestHit.material.reflectivity > 0.0f)
        {
            Vector3 reflectedDir = ray.direction.reflect(closestHit.normal).normalise();
            Ray reflectedRay(closestHit.point + closestHit.normal * 1e-4f, reflectedDir);
            Color reflectedColor = trace(reflectedRay, scene, shader, depth + 1);

            // Blend the reflected color with the current accumulated color
            float reflectivity = closestHit.material.reflectivity;
            accumulatedColor = accumulatedColor * (1.0f - reflectivity) + reflectedColor * reflectivity;
        }

        // Handle refractions if the material is refractive
        if (closestHit.material.isRefractive)
        {
            float transmission = 1.0f - closestHit.material.reflectivity;
            if (transmission > 0.0f)
            {
                float eta = closestHit.material.refractiveIndex;
                Vector3 normal = closestHit.normal;
                float cosi = normal.dot(ray.direction);
                float etai = 1.0f, etat = eta;

                if (cosi > 0)
                {
                    // Ray is inside the object, swap indices and invert normal
                    std::swap(etai, etat);
                    normal = -normal;
                }
                float etaRatio = etai / etat;

                // Calculate the refracted direction
                float k = 1 - etaRatio * etaRatio * (1 - cosi * cosi);
                Vector3 refractedDir;
                if (k < 0)
                {
                    // Total internal reflection
                    refractedDir = ray.direction.reflect(normal);
                }
                else
                {
                    refractedDir = ray.direction * etaRatio + normal * (etaRatio * cosi - sqrtf(k));
                }

                refractedDir = refractedDir.normalise();
                Ray refractedRay(closestHit.point - normal * 1e-4f, refractedDir);
                Color refractedColor = trace(refractedRay, scene, shader, depth + 1);

                // Blend the refracted color with the accumulated color
                accumulatedColor = accumulatedColor * (1.0f - transmission) + refractedColor * transmission;
            }
        }

        // Return the accumulated color without tone mapping
        return accumulatedColor;
    }
    else
    {
        // No intersection; return the scene's background color without tone mapping
        return scene.backgroundColor;
    }
}

Color Raytracer::pathTrace(const Ray &ray, const Scene &scene, int depth) const
{
    if (depth >= scene.nbounces)
        return Color(0, 0, 0); // Terminate recursion

    Intersection hit;
    if (scene.intersect(ray, hit))
    {
        // Use texture color if available
        Color materialColor = hit.material.diffuseColor;
        if (hit.material.texture)
        {
            materialColor = hit.material.texture->getColor(hit.u, hit.v);
        }

        Vector3 normal = hit.normal.normalise();

        // **Direct Lighting (Next Event Estimation)**
        Color directLighting(0, 0, 0);
        for (const auto &light : scene.lights)
        {
            Vector3 lightDir = (light.getPosition() - hit.point).normalise();
            float lightDistance = (light.getPosition() - hit.point).length();

            // **Check for shadows**
            Ray shadowRay(hit.point + normal * 1e-4f, lightDir);
            Intersection shadowHit;
            if (scene.intersect(shadowRay, shadowHit) && shadowHit.distance < lightDistance)
            {
                continue; // In shadow, skip this light
            }

            // **Lambertian Reflectance**
            float NdotL = std::max(normal.dot(lightDir), 0.0f);

            // **Add Light Contribution**
            Color lightIntensity = light.getIntensity() / (lightDistance * lightDistance);
            directLighting += materialColor * hit.material.kd * NdotL * lightIntensity;
        }

        // Russian Roulette for path termination
        float probability = materialColor.maxComponent();
        probability = std::clamp(probability, 0.1f, 0.95f);

        float randVal = distribution(rng);
        if (randVal > probability)
        {
            return directLighting;
        }

        // **Sample Reflection or Refraction Based on Material**
        Vector3 newDirection;
        Color brdf;
        float pdf;

        if (hit.material.isReflective && hit.material.reflectivity > 0.0f)
        {
            // **Specular Reflection**
            newDirection = ray.direction.reflect(normal).normalise();
            brdf = hit.material.specularColor * hit.material.reflectivity;
            pdf = 1.0f;
        }
        else if (hit.material.isRefractive)
        {
            // **Refraction**
            float eta = hit.material.refractiveIndex;
            Vector3 n = normal;
            float cosi = std::clamp(ray.direction.dot(normal), -1.0f, 1.0f);
            float etai = 1.0f, etat = eta;
            if (cosi > 0)
            {
                std::swap(etai, etat);
                n = -normal;
            }
            float etaRatio = etai / etat;
            float k = 1 - etaRatio * etaRatio * (1 - cosi * cosi);
            if (k < 0)
            {
                // Total internal reflection
                newDirection = ray.direction.reflect(n).normalise();
                brdf = hit.material.specularColor; // For total internal reflection
            }
            else
            {
                newDirection = ray.direction * etaRatio + n * (etaRatio * cosi - sqrtf(k));
                newDirection = newDirection.normalise();
                brdf = Color(1.0f, 1.0f, 1.0f); // Assume white color for refracted ray
            }
            pdf = 1.0f;
        }
        else
        {
            // **Diffuse Reflection**
            newDirection = sampleHemisphere(normal);
            float cosine = std::max(newDirection.dot(normal), 0.0f);
            brdf = materialColor * hit.material.kd * (1.0f / M_PI);
            pdf = cosine / M_PI;
        }

        // Create new ray from hit point
        Ray newRay(hit.point + newDirection * 1e-4f, newDirection);

        // Recursive path tracing
        Color incomingRadiance = pathTrace(newRay, scene, depth + 1);

        // **Correct Radiance Accumulation**
        Color indirectLighting = brdf * incomingRadiance * std::abs(newDirection.dot(normal)) / (pdf * probability);

        // **Combine Direct and Indirect Lighting**
        Color radiance = directLighting + indirectLighting;

        return radiance;
    }
    else
    {
        // No intersection; return background color
        return scene.backgroundColor;
    }
}

// Helper function to sample a cosine-weighted direction in hemisphere
Vector3 Raytracer::sampleHemisphere(const Vector3 &normal) const
{
    float u1 = distribution(rng);
    float u2 = distribution(rng);

    float r = sqrt(u1);
    float theta = 2 * M_PI * u2;

    float x = r * cos(theta);
    float y = r * sin(theta);
    float z = sqrt(1.0f - u1);

    // Create a coordinate system (normal, tangent, bitangent)
    Vector3 w = normal;
    Vector3 u = ((fabs(w.x) > 0.1f ? Vector3(0.0f, 1.0f, 0.0f) : Vector3(1.0f, 0.0f, 0.0f)).cross(w)).normalise();
    Vector3 v = w.cross(u);

    // Convert sample to world space
    Vector3 sampleDirection = u * x + v * y + w * z;

    return sampleDirection.normalise();
}

Color Raytracer::getPixelColor(int x, int y) const
{
    return image.getPixel(x, y);
}

// Raytracer.cpp
#include "Raytracer.h"
#include "BlinnPhongShader.h"
#include "MicrofacetBRDF.h" // Include MicrofacetBRDF
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

    // Initialize the shader once (can be omitted if using BRDF-based shading)
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
                    // **Changed Part: Use the trace method for Phong rendering**
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

        // Initialize the accumulated radiance
        Color L(0.0f, 0.0f, 0.0f);

        // **Direct Lighting (Next Event Estimation)**
        for (const auto &light : scene.lights)
        {
            Vector3 lightDir;
            float lightDistance;
            Color lightIntensity;

            if (light.getType() == LightType::Point)
            {
                lightDir = (light.getPosition() - hit.point).normalise();
                lightDistance = (light.getPosition() - hit.point).length();
                lightIntensity = light.getIntensity() / (lightDistance * lightDistance);
            }
            else if (light.getType() == LightType::Area)
            {
                // Sample a point on the area light
                lightDir = (light.samplePosition(rng, distribution) - hit.point).normalise();
                lightDistance = (light.samplePosition(rng, distribution) - hit.point).length();
                lightIntensity = light.getIntensity() / (lightDistance * lightDistance);
            }
            else
            {
                continue; // Unknown light type
            }

            // **Shadow Ray**
            Ray shadowRay(hit.point + normal * 1e-4f, lightDir);
            Intersection shadowHit;
            if (scene.intersect(shadowRay, shadowHit))
            {
                if (shadowHit.distance < lightDistance)
                {
                    continue; // In shadow
                }
            }

            float NdotL = std::max(normal.dot(lightDir), 0.0f);

            // Add light contribution
            L += materialColor * hit.material.kd * NdotL * lightIntensity;
        }

        // **Indirect Lighting**

        // Russian Roulette for path termination
        float probability = materialColor.maxComponent();
        probability = std::clamp(probability, 0.1f, 0.95f);

        float randVal = distribution(rng);
        if (randVal > probability)
        {
            return L;
        }

        // **BRDF Sampling**
        if (hit.material.brdf)
        {
            float pdf_val;
            Vector3 wi = hit.material.brdf->sample(ray.direction * -1.0f, normal, pdf_val);

            if (wi.dot(normal) <= 0.0f)
                return L;

            // Compute BRDF value
            Color brdf = hit.material.brdf->evaluate(wi, ray.direction * -1.0f, normal);

            // Create new ray
            Ray newRay(hit.point + wi * 1e-4f, wi.normalise());

            // Recursive path tracing
            Color incomingRadiance = pathTrace(newRay, scene, depth + 1);

            // **Radiance Contribution**
            float cosine = std::max(wi.dot(normal), 0.0f);
            L += brdf * incomingRadiance * cosine / (pdf_val * probability);
        }
        else
        {
            // Fallback to Lambertian reflection if BRDF is not defined
            // Sample a random direction in the hemisphere
            Vector3 wi = sampleHemisphere(normal);
            float pdf_val = wi.dot(normal) / static_cast<float>(M_PI);

            // Compute BRDF value (Lambertian)
            Color brdf = materialColor * hit.material.kd / static_cast<float>(M_PI);

            // Create new ray
            Ray newRay(hit.point + wi * 1e-4f, wi.normalise());

            // Recursive path tracing
            Color incomingRadiance = pathTrace(newRay, scene, depth + 1);

            // **Radiance Contribution**
            float cosine = std::max(wi.dot(normal), 0.0f);
            L += brdf * incomingRadiance * cosine / (pdf_val * 1.0f); // probability = 1.0f for Lambertian
        }

        return L;
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

    float r = std::sqrt(u1);
    float theta = 2 * M_PI * u2;

    float x = r * std::cos(theta);
    float y = r * std::sin(theta);
    float z = std::sqrt(1.0f - u1);

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
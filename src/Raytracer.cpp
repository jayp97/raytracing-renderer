#include "Raytracer.h"
#include "BlinnPhongShader.h"
#include <iostream>
#include <algorithm>
#include <omp.h>
#include <chrono>
#include <cmath> // Include for std::pow

Raytracer::Raytracer(int width, int height) : image(width, height)
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

    // Initialize the shader once
    BlinnPhongShader shader(scene, scene.camera.position);

    // Start timing
    auto startTime = std::chrono::steady_clock::now();

    // Retrieve exposure from camera and calculate exposure scale
    float exposure = camera.exposure;
    float exposureScale = std::pow(2.0f, exposure); // Using EV scale

    // Iterate over each pixel with OpenMP parallelization
#pragma omp parallel for schedule(dynamic) shared(image)
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            Ray ray = camera.generateRay(static_cast<float>(x), static_cast<float>(y));

            // Trace the ray and compute the color
            Color color = trace(ray, scene, shader, 0);

            if (isBinaryMode)
            {
                // In binary mode, trace already returns red or background
                // So no need to modify color here
            }
            else
            {
                // Apply linear exposure adjustment
                color *= exposureScale;

                // Apply linear tone mapping: simple scaling and clamping
                // Prevents colors from exceeding the displayable range
                color = color.clamp(0.0f, 1.0f);

                // Optionally, apply gamma correction for display purposes
                // Uncomment the following line if your display expects gamma-corrected colors
                // color = color.gammaCorrect(2.2f);
            }

            // Set the pixel color (assumes thread-safe)
            image.setPixel(x, y, color);
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
    // Check if the render mode is "binary"
    bool isBinaryMode = scene.renderMode == "binary";

    if (isBinaryMode)
    {
        Intersection hit;
        bool hasHit = scene.intersect(ray, hit);
        if (hasHit)
        {
            // Return fixed color (e.g., red) for any intersection
            return Color(1.0f, 0.0f, 0.0f);
        }
        else
        {
            // Return background color if no intersection
            return scene.backgroundColor;
        }
    }

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

Color Raytracer::getPixelColor(int x, int y) const
{
    return image.getPixel(x, y);
}

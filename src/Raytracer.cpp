#include "Raytracer.h"
#include "BlinnPhongShader.h"
#include <iostream>
#include <algorithm>

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

    // Iterate over each pixel
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            Ray ray = camera.generateRay(static_cast<float>(x), static_cast<float>(y));

            // Trace the ray and compute the color
            Color color = trace(ray, scene, shader, 0);

            // Apply tone mapping
            color = color.toneMap();

            if (isBinaryMode)
            {
                // If binary mode, set color to white on intersection, else to background color
                color = (color != scene.backgroundColor) ? Color(1, 1, 1) : scene.backgroundColor;
            }

            image.setPixel(x, y, color);
        }

        // Optional: Print progress every 100 lines
        if (y % 100 == 0)
        {
            std::cout << "Rendered " << y << " / " << height << " lines." << std::endl;
        }
    }

    // Save the rendered image to the specified file
    image.saveAsPPM(outputFilename);

    std::cout << "Rendering completed. Image saved to " << outputFilename << std::endl;
}

Color Raytracer::trace(const Ray &ray, const Scene &scene, const BlinnPhongShader &shader, int depth) const
{
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
                float etai = 1.0f;
                float etat = eta;

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

        // Return the tone-mapped color
        return accumulatedColor.toneMap();
    }
    else
    {
        // No intersection; return the scene's background color (tone-mapped)
        return scene.backgroundColor.toneMap();
    }
}

Color Raytracer::getPixelColor(int x, int y) const
{
    return image.getPixel(x, y);
}
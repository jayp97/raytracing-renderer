// Raytracer.cpp
#include "Raytracer.h"
#include "BlinnPhongShader.h" // Shader class for shading computations
#include <iostream>

Raytracer::Raytracer()
{
    // Initialization if necessary
}

// Render the scene and save the output image
void Raytracer::render(const Scene &scene, const std::string &outputFilename)
{
    const Camera &camera = scene.camera;
    int width = camera.width;     // Correct member name
    int height = camera.height;   // Correct member name
    image = Image(width, height); // Ensure Image has a constructor Image(int, int)

    bool isBinaryMode = scene.renderMode == "binary";
    int maxBounces = scene.nbounces;

    // Initialize the shader (e.g., Blinn-Phong)
    BlinnPhongShader shader(scene, scene.camera.position);

    // Iterate over each pixel
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            Ray ray = camera.generateRay(static_cast<float>(x), static_cast<float>(y));

            // Trace the ray and compute the color
            Color color = trace(ray, scene, 0);

            if (isBinaryMode)
            {
                // If binary mode, set color to white on intersection, else black
                if (color != Color(0, 0, 0))
                {
                    color = Color(1, 1, 1);
                }
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

// Trace a ray into the scene and compute the color, supporting recursion for reflections/refractions
Color Raytracer::trace(const Ray &ray, const Scene &scene, int depth) const
{
    if (depth > scene.nbounces)
        return Color(0, 0, 0); // Base case: no contribution

    Intersection closestHit;
    bool hit = scene.intersect(ray, closestHit);

    if (hit)
    {
        // Compute the color at the intersection point using the shader
        BlinnPhongShader shader(scene, scene.camera.position); // Corrected access to camera
        Color shadedColor = shader.shade(closestHit);

        // Handle reflections if the material is reflective
        if (closestHit.material.isReflective && depth < scene.nbounces)
        {
            Vector3 reflectedDir = ray.direction.reflect(closestHit.normal).normalise();
            Ray reflectedRay(closestHit.point + closestHit.normal * 1e-4f, reflectedDir);
            Color reflectedColor = trace(reflectedRay, scene, depth + 1);
            shadedColor = shadedColor * (1.0f - closestHit.material.reflectivity) + reflectedColor * closestHit.material.reflectivity;
        }

        // Handle refractions if the material is refractive
        if (closestHit.material.isRefractive && depth < scene.nbounces)
        {
            Vector3 refractedDir = ray.direction.refract(closestHit.normal, closestHit.material.refractiveIndex).normalise();
            Ray refractedRay(closestHit.point - closestHit.normal * 1e-4f, refractedDir);
            Color refractedColor = trace(refractedRay, scene, depth + 1);
            shadedColor = shadedColor * (1.0f - closestHit.material.reflectivity) + refractedColor * closestHit.material.reflectivity;
        }

        return shadedColor;
    }
    else
    {
        // No intersection; return the scene's background color
        return scene.backgroundColor;
    }
}

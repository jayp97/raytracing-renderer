#include "Raytracer.h"
#include <iostream>

Raytracer::Raytracer(int width, int height) : width(width), height(height), image(width, height) {}

void Raytracer::render(const SceneLoader &sceneLoader, const Camera &camera)
{
    bool isBinaryMode = sceneLoader.getRenderMode() == "binary";

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            Ray ray = camera.generateRay(x, y);

            Color color(0, 0, 0); // Default to black for no intersection

            if (trace(ray, sceneLoader, color))
            {
                if (isBinaryMode)
                {
                    // If binary mode, set color to white on intersection
                    color = Color(1, 1, 1);
                }
                // Else, keep the color set by the intersected object's type
            }
            image.setPixel(x, y, color);
        }
    }

    // Save the rendered image to a PPM file
    image.saveAsPPM("output.ppm");
}

bool Raytracer::trace(const Ray &ray, const SceneLoader &sceneLoader, Color &color)
{
    float closestT = std::numeric_limits<float>::max();
    bool hit = false;

    for (const auto &sphere : sceneLoader.getSpheres())
    {
        float t;
        if (sphere.intersect(ray, t) && t < closestT)
        {
            closestT = t;
            color = Color(1, 0, 0); // Red for sphere (only used in non-binary mode)
            hit = true;
        }
    }

    for (const auto &cylinder : sceneLoader.getCylinders())
    {
        float t = closestT;
        if (cylinder.intersect(ray, t) && t < closestT)
        {
            closestT = t;
            color = Color(0, 0, 1); // Blue for cylinder (only used in non-binary mode)
            hit = true;
        }
    }

    for (const auto &triangle : sceneLoader.getTriangles())
    {
        float t, u, v;
        if (triangle.intersect(ray, t, u, v) && t < closestT)
        {
            closestT = t;
            color = Color(0, 1, 0); // Green for triangle (only used in non-binary mode)
            hit = true;
        }
    }

    return hit;
}

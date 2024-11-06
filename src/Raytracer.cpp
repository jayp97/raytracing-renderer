#include "Raytracer.h"
#include <iostream>

Raytracer::Raytracer(int width, int height) : width(width), height(height), image(width, height) {}

void Raytracer::render(const SceneLoader &sceneLoader, const Camera &camera)
{
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            Ray ray = camera.generateRay(x, y, width, height);

            Color color(0, 0, 0); // Default color (black) for no intersection
            if (trace(ray, sceneLoader, color))
            {
                // color will be modified in the trace function if an intersection is found
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

    // Test each sphere for intersection
    for (const auto &sphere : sceneLoader.getSpheres())
    {
        float t;
        if (sphere.intersect(ray, t) && t < closestT)
        {
            closestT = t;
            color = Color(1, 0, 0); // Set color for spheres (red)
            hit = true;
        }
    }

    // Test each cylinder for intersection
    for (const auto &cylinder : sceneLoader.getCylinders())
    {
        float t = closestT; // Pass closestT to limit the check within current closest intersection distance
        if (cylinder.intersect(ray, t) && t < closestT)
        {
            closestT = t;
            color = Color(0, 0, 1); // Set color for cylinders (blue)
            hit = true;
        }
    }

    // Test each triangle for intersection
    for (const auto &triangle : sceneLoader.getTriangles())
    {
        float t, u, v; // Triangle intersection requires t, u, v parameters
        if (triangle.intersect(ray, t, u, v) && t < closestT)
        {
            closestT = t;
            color = Color(0, 1, 0); // Set color for triangles (green)
            hit = true;
        }
    }

    return hit;
}
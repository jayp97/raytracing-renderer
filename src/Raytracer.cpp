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
    // Placeholder for intersection testing; currently returns no intersection
    // In future steps, this will contain logic to check intersections with objects
    return false;
}

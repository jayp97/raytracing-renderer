// Raytracer.h
#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "Image.h"
#include "Scene.h" // Updated to include Scene instead of SceneLoader
#include "Ray.h"
#include "Vector3.h"
#include "Intersection.h"
#include "Color.h" // Assuming a Color class similar to Vector3
#include <limits>
#include <memory>
#include <string>

class Raytracer
{
public:
    // Default constructor
    Raytracer();

    // Render the scene and save the output image
    void render(const Scene &scene, const std::string &outputFilename);

    // Trace a ray into the scene and compute the color, supporting recursion for reflections/refractions
    Color trace(const Ray &ray, const Scene &scene, int depth = 0) const;

    // Access the rendered image (for testing purposes)
    Image getImage() const { return image; }

private:
    Image image;
};

#endif // RAYTRACER_H
#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "Image.h"
#include "Scene.h"
#include "Ray.h"
#include "Vector3.h"
#include "Intersection.h"
#include "Color.h"
#include <string>

// Forward declaration to avoid circular dependency
class BlinnPhongShader;

class Raytracer
{
public:
    // Constructor with width and height
    Raytracer(int width, int height);

    // Render the scene and save the output image
    void render(const Scene &scene, const std::string &outputFilename);

    // Trace a ray into the scene and compute the color, supporting recursion for reflections/refractions
    Color trace(const Ray &ray, const Scene &scene, const BlinnPhongShader &shader, int depth = 0) const;

    // Access the rendered image (for testing purposes)
    Image getImage() const { return image; }

    Color getPixelColor(int x, int y) const;

private:
    Image image;
};

#endif // RAYTRACER_H

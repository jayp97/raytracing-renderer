#ifndef RAYTRACER_H
#define RAYTRACER_H

#ifdef __APPLE__
#include <libkern/OSByteOrder.h>
#define htobe16(x) OSSwapHostToBigInt16(x)
#define be16toh(x) OSSwapBigToHostInt16(x)
#define htobe32(x) OSSwapHostToBigInt32(x)
#define be32toh(x) OSSwapBigToHostInt32(x)
#define htobe64(x) OSSwapHostToBigInt64(x)
#define be64toh(x) OSSwapBigToHostInt64(x)
#else
#include <endian.h>
#endif

#include "Vector3.h"
#include "Ray.h"
#include "Intersection.h"
#include "BlinnPhongShader.h"
#include "Scene.h"
#include "Image.h"
#include <string>
#include <random>

// Forward declaration to avoid circular dependency
class BlinnPhongShader;

class Raytracer
{
public:
    // Constructor with width and height
    Raytracer(int width, int height, int samplesPerPixel = 1);

    // Render the scene and save the output image
    void render(const Scene &scene, const std::string &outputFilename);

    // Trace a ray into the scene and compute the color, supporting recursion for reflections/refractions
    Color trace(const Ray &ray, const Scene &scene, const BlinnPhongShader &shader, int depth = 0) const;

    // Path tracing function for multi-bounce global illumination
    Color pathTrace(const Ray &ray, const Scene &scene, int depth = 0) const;

    // Access the rendered image (for testing purposes)
    Image getImage() const { return image; }

    Color getPixelColor(int x, int y) const;

private:
    Image image;
    int samplesPerPixel;                                        // Number of samples per pixel for antialiasing
    mutable std::mt19937 rng;                                   // Random number generator
    mutable std::uniform_real_distribution<float> distribution; // Uniform distribution [0, 1)

    // Helper function to sample a cosine-weighted direction in hemisphere
    Vector3 sampleHemisphere(const Vector3 &normal) const;
};

#endif // RAYTRACER_H

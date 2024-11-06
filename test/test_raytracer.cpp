#include "catch_amalgamated.hpp"
#include "../include/Raytracer.h"
#include "../include/Camera.h"
#include "../include/SceneLoader.h"
#include "../include/Vector3.h"
#include "../include/Ray.h"

const float tolerance = 1e-5;

bool areColorsEqual(const Color &c1, const Color &c2, float tolerance)
{
    return std::fabs(c1.r - c2.r) < tolerance &&
           std::fabs(c1.g - c2.g) < tolerance &&
           std::fabs(c1.b - c2.b) < tolerance;
}

TEST_CASE("Raytracer runs and produces output", "[RAYTRACER]")
{
    int width = 3;
    int height = 3;

    // Setup a basic scene
    SceneLoader sceneLoader;
    REQUIRE(sceneLoader.loadScene("scenes/binary_primitives.json") == true);

    // Setup camera (values should match the scene file)
    Camera camera(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(0, 1, 0), 45.0f, width, height, 0.1f);

    // Initialize Raytracer
    Raytracer raytracer(width, height);

    // Run render (should produce an output image without errors)
    raytracer.render(sceneLoader, camera);

    // Test a few pixels in the image
    // For now, without intersections, the color will be black (0, 0, 0)
    Image expectedImage(width, height);
    Color black(0, 0, 0);

    // Check if each pixel in the rendered image is black
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            Color pixelColor = raytracer.getImage().getPixel(x, y); // Assumes getImage() and getPixel() methods
            REQUIRE(areColorsEqual(pixelColor, black, tolerance));
        }
    }
}

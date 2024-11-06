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

TEST_CASE("Raytracer produces expected colored output for intersections", "[RAYTRACER]")
{
    int width = 3;
    int height = 3;

    // Load the scene
    SceneLoader sceneLoader;
    REQUIRE(sceneLoader.loadScene("scenes/binary_primitives.json") == true);

    // Setup camera
    Camera camera(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(0, 1, 0), 45.0f, width, height, 0.1f);

    // Initialize Raytracer
    Raytracer raytracer(width, height);
    raytracer.render(sceneLoader, camera);

    // Define expected colors for shapes and background
    Color black(0, 0, 0);
    Color red(1, 0, 0);   // Sphere
    Color green(0, 1, 0); // Triangle
    Color blue(0, 0, 1);  // Cylinder

    // Track if we find each color
    bool foundRed = false, foundGreen = false, foundBlue = false, foundBlack = false;

    // Check each pixel for expected colors
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            Color pixelColor = raytracer.getImage().getPixel(x, y);

            if (areColorsEqual(pixelColor, red, tolerance))
                foundRed = true;
            else if (areColorsEqual(pixelColor, green, tolerance))
                foundGreen = true;
            else if (areColorsEqual(pixelColor, blue, tolerance))
                foundBlue = true;
            else if (areColorsEqual(pixelColor, black, tolerance))
                foundBlack = true;
        }
    }

    // Verify that each expected color was found
    REQUIRE(foundRed);
    REQUIRE(foundGreen);
    REQUIRE(foundBlue);
    REQUIRE(foundBlack);
}

// test_raytracer.cpp

#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"

#include "Raytracer.h"
#include "Camera.h"
#include "SceneLoader.h"
#include "Vector3.h"
#include "Color.h"
#include "Scene.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Triangle.h"
#include <cmath>
#include <memory>
#include <iostream>

const float tolerance = 0.05f;

// Function to compare two colors within a specified tolerance
bool areColorsEqual(const Color &c1, const Color &c2, float tolerance)
{
    return std::fabs(c1.r - c2.r) < tolerance &&
           std::fabs(c1.g - c2.g) < tolerance &&
           std::fabs(c1.b - c2.b) < tolerance;
}

// Functions to check for dominant color components
bool isSphereColor(const Color &c)
{
    return c.r > c.g + tolerance && c.r > c.b + tolerance;
}

bool isCylinderColor(const Color &c)
{
    return c.b > c.r + tolerance && c.b > c.g + tolerance;
}

bool isTriangleColor(const Color &c)
{
    return c.g > c.r + tolerance && c.g > c.b + tolerance;
}

TEST_CASE("Raytracer binary render mode produces expected output", "[RAYTRACER][binary]")
{
    // Load the binary scene
    SceneLoader sceneLoader;
    Scene scene;
    REQUIRE(sceneLoader.loadScene("scenes/binary_primitives.json", scene) == true); // Using binary_primitives.json

    // Retrieve camera dimensions from the loaded scene
    int width = scene.camera.width;
    int height = scene.camera.height;

    // Initialize Raytracer with camera's width and height
    Raytracer raytracer(width, height);

    // Render the scene
    raytracer.render(scene, "binary_test_output.ppm");

    // Define expected colors for binary mode
    Color backgroundColor(0.25f, 0.25f, 0.25f); // From binary_primitives.json
    Color white(1.0f, 1.0f, 1.0f);              // Binary mode: white for any intersection

    // Track if we find white and background color
    bool foundWhite = false, foundBackground = false;

    // Check each pixel for expected colors
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            Color pixelColor = raytracer.getPixelColor(x, y);

            // Print the pixel colors for debugging
            // std::cout << "Binary Pixel (" << x << ", " << y << "): ("
            //           << pixelColor.r << ", " << pixelColor.g << ", " << pixelColor.b << ")\n";

            if (areColorsEqual(pixelColor, backgroundColor, tolerance))
                foundBackground = true;
            else if (areColorsEqual(pixelColor, white, tolerance))
                foundWhite = true;
        }
    }

    // Verify that at least one white and one background color pixel are found
    REQUIRE(foundWhite);
    REQUIRE(foundBackground);
}

TEST_CASE("Raytracer produces expected colors for intersections in Phong mode", "[RAYTRACER][phong]")
{
    // Load the Phong scene
    SceneLoader sceneLoader;
    Scene scene;
    REQUIRE(sceneLoader.loadScene("scenes/simple_phong.json", scene) == true); // Using simple_phong.json

    // Retrieve camera dimensions from the loaded scene
    int width = scene.camera.width;
    int height = scene.camera.height;

    // Initialize Raytracer with camera's width and height
    Raytracer raytracer(width, height);

    // Build the BVH for the scene
    scene.buildBVH();

    // Render the scene
    raytracer.render(scene, "phong_test_output.ppm");

    // Define expected colors based on material properties and lighting calculations
    Color backgroundColor(0.25f, 0.25f, 0.25f); // From simple_phong.json

    // Track if we find each color
    bool foundBackground = false, foundSphere = false, foundCylinder = false, foundTriangle = false;

    // Check each pixel for expected colors
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            Color pixelColor = raytracer.getPixelColor(x, y);

            if (areColorsEqual(pixelColor, backgroundColor, tolerance))
                foundBackground = true;
            else if (isSphereColor(pixelColor))
                foundSphere = true;
            else if (isCylinderColor(pixelColor))
                foundCylinder = true;
            else if (isTriangleColor(pixelColor))
                foundTriangle = true;
        }
    }

    // Verify that each expected color was found
    REQUIRE(foundBackground);
    REQUIRE(foundSphere);
    REQUIRE(foundCylinder);
    REQUIRE(foundTriangle);
}

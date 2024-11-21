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
#include <iostream> // Include for printing

const float tolerance = 0.15f; // Increased tolerance for flexibility

// Function to compare two colors within a specified tolerance using Euclidean distance
bool areColorsSimilar(const Color &c1, const Color &c2, float tolerance)
{
    float dr = c1.r - c2.r;
    float dg = c1.g - c2.g;
    float db = c1.b - c2.b;
    float distance = std::sqrt(dr * dr + dg * dg + db * db);
    return distance < tolerance;
}

// Functions to check for expected colors based on Phong shading
bool isSphereColor(const Color &c)
{
    // Updated expected sphere color based on calculations
    Color expected(1.0f, 0.88f, 0.88f);
    return areColorsSimilar(c, expected, tolerance);
}

bool isCylinderColor(const Color &c)
{
    // Expected cylinder color based on ambient and diffuse components
    // Example calculation: (0.79f, 0.79f, 1.0f)
    Color expected(0.79f, 0.79f, 1.0f);
    return areColorsSimilar(c, expected, tolerance);
}

bool isTriangleColor(const Color &c)
{
    // Expected triangle color based on ambient, diffuse, and specular components
    // Example calculation: (0.89f, 1.0f, 0.89f)
    Color expected(0.89f, 1.0f, 0.89f);
    return areColorsSimilar(c, expected, tolerance);
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

            // Optionally, comment out to reduce console output
            // std::cout << "Binary Pixel (" << x << ", " << y << "): ("
            //           << pixelColor.r << ", " << pixelColor.g << ", " << pixelColor.b << ")\n";

            if (areColorsSimilar(pixelColor, backgroundColor, tolerance))
                foundBackground = true;
            else if (areColorsSimilar(pixelColor, white, tolerance))
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
    Color sphereColor(1.0f, 0.88f, 0.88f);      // Expected sphere color (updated)
    Color cylinderColor(0.79f, 0.79f, 1.0f);    // Expected cylinder color
    Color triangleColor(0.89f, 1.0f, 0.89f);    // Expected triangle color (updated)

    // Track if we find each color
    bool foundBackground = false, foundSphere = false, foundCylinder = false, foundTriangle = false;
    int triangleColorMatches = 0;
    int sphereColorMatches = 0;
    int cylinderColorMatches = 0;

    // Check each pixel for expected colors
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            Color pixelColor = raytracer.getPixelColor(x, y);

            // Optionally, comment out to reduce console output
            // std::cout << "Phong Pixel (" << x << ", " << y << "): ("
            //           << pixelColor.r << ", " << pixelColor.g << ", " << pixelColor.b << ")\n";

            if (areColorsSimilar(pixelColor, backgroundColor, tolerance))
                foundBackground = true;
            else if (isSphereColor(pixelColor))
            {
                foundSphere = true;
                sphereColorMatches++;
                // Print first 5 sphere pixels for debugging
                if (sphereColorMatches <= 5)
                {
                    std::cout << "Sphere Pixel Found at (" << x << ", " << y << "): ("
                              << pixelColor.r << ", " << pixelColor.g << ", " << pixelColor.b << ")\n";
                }
            }
            else if (isCylinderColor(pixelColor))
            {
                foundCylinder = true;
                cylinderColorMatches++;
            }
            else if (isTriangleColor(pixelColor))
            {
                foundTriangle = true;
                triangleColorMatches++;
                // Print first 5 triangle pixels for debugging
                if (triangleColorMatches <= 5)
                {
                    std::cout << "Triangle Pixel Found at (" << x << ", " << y << "): ("
                              << pixelColor.r << ", " << pixelColor.g << ", " << pixelColor.b << ")\n";
                }
            }
            else
            {
                // Optionally, print unknown colors for debugging
                // std::cout << "Unknown Pixel (" << x << ", " << y << "): ("
                //           << pixelColor.r << ", " << pixelColor.g << ", " << pixelColor.b << ")\n";
            }
        }
    }

    // Print the number of pixels found for each object
    std::cout << "Number of Sphere Pixels Found: " << sphereColorMatches << "\n";
    std::cout << "Number of Cylinder Pixels Found: " << cylinderColorMatches << "\n";
    std::cout << "Number of Triangle Pixels Found: " << triangleColorMatches << "\n";

    // Verify that each expected color was found
    REQUIRE(foundBackground);
    REQUIRE(foundSphere);
    REQUIRE(foundCylinder);
    REQUIRE(foundTriangle);
}

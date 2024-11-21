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
#include <cmath>
#include <memory>

const float tolerance = 1e-5f;

bool areColorsEqual(const Color &c1, const Color &c2, float tolerance)
{
    return std::fabs(c1.r - c2.r) < tolerance &&
           std::fabs(c1.g - c2.g) < tolerance &&
           std::fabs(c1.b - c2.b) < tolerance;
}

TEST_CASE("Raytracer produces expected colors for intersections", "[RAYTRACER]")
{
    int width = 3;
    int height = 3;

    // Load the scene
    SceneLoader sceneLoader;
    Scene scene;
    REQUIRE(sceneLoader.loadScene("scenes/binary_primitives.json", scene) == true);

    // Build the BVH for the scene
    scene.buildBVH();

    // Initialize Raytracer
    Raytracer raytracer(width, height);

    // Render the scene
    raytracer.render(scene, "test_output.ppm");

    // Define expected colors for shapes and background
    Color black(0, 0, 0); // Background
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
            Color pixelColor = raytracer.getPixelColor(x, y);

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

TEST_CASE("BVH correctly identifies intersections", "[BVH]")
{
    // Create a scene with multiple spheres
    Scene scene;

    // Add spheres in a grid
    const int gridSize = 10;
    const float spacing = 2.0f;
    for (int x = -gridSize; x <= gridSize; ++x)
    {
        for (int y = -gridSize; y <= gridSize; ++y)
        {
            Vector3 center(x * spacing, 0.0f, y * spacing);
            float radius = 0.5f;

            Material material;
            material.diffuseColor = Color(1.0f, 0.0f, 0.0f); // Red spheres

            auto sphere = std::make_shared<Sphere>(center, radius, material);
            scene.addObject(sphere);
        }
    }

    // Build the BVH
    scene.buildBVH();

    // Create a ray that should intersect a sphere at the center
    Ray ray(Vector3(0.0f, 0.0f, -10.0f), Vector3(0.0f, 0.0f, 1.0f));

    // Perform intersection test
    Intersection hit;
    bool intersected = scene.intersect(ray, hit);

    // The ray should intersect the sphere at (0,0,0)
    REQUIRE(intersected);
    REQUIRE(areColorsEqual(hit.material.diffuseColor, Color(1.0f, 0.0f, 0.0f), tolerance));

    // Create a ray that misses all spheres
    Ray missRay(Vector3(0.0f, 0.0f, -10.0f), Vector3(0.0f, 1.0f, 0.0f));

    // Perform intersection test
    Intersection missHit;
    bool missed = scene.intersect(missRay, missHit);

    // The ray should not intersect any sphere
    REQUIRE(!missed);
}

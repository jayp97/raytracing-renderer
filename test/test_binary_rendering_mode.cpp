#include "catch_amalgamated.hpp"
#include "../include/Raytracer.h"
#include "../include/Camera.h"
#include "../include/SceneLoader.h"
#include "../include/Vector3.h"
#include "../include/Ray.h"
#include <iostream>
#include <chrono> // For timing

const float tolerance = 1e-5;

// Utility function to compare colors with a tolerance
bool areColorsEqual(const Color &c1, const Color &c2, float tolerance)
{
    return std::fabs(c1.r - c2.r) < tolerance &&
           std::fabs(c1.g - c2.g) < tolerance &&
           std::fabs(c1.b - c2.b) < tolerance;
}

TEST_CASE("Raytracer produces binary output for intersections", "[RAYTRACER_BINARY]")
{
    // Load the binary scene
    SceneLoader sceneLoader;
    REQUIRE(sceneLoader.loadScene("scenes/binary_primitives.json") == true);

    Camera camera = sceneLoader.getCamera();

    // Initialize Raytracer with binary rendering mode
    Raytracer raytracer(camera.width, camera.height);

    // Start rendering and timing
    auto start = std::chrono::high_resolution_clock::now();
    raytracer.render(sceneLoader, camera);

    // Save the rendered image to a PPM file
    raytracer.getImage().saveAsPPM("output_binary.ppm");

    // Define expected binary colors
    Color black(0, 0, 0); // No intersection
    Color white(1, 1, 1); // Intersection

    // Track if we find each color
    bool foundWhite = false;
    bool foundBlack = false;

    // Total pixels for progress calculation
    int totalPixels = camera.width * camera.height;
    int samplePixels = 100; // Sample size for timing
    int processedPixels = 0;

    // Measure the time for the first 10 pixels
    auto sampleStart = std::chrono::high_resolution_clock::now();
    for (int y = 0; y < camera.height && processedPixels < samplePixels; ++y)
    {
        for (int x = 0; x < camera.width && processedPixels < samplePixels; ++x)
        {
            Color pixelColor = raytracer.getImage().getPixel(x, y);

            if (areColorsEqual(pixelColor, white, tolerance))
                foundWhite = true;
            else if (areColorsEqual(pixelColor, black, tolerance))
                foundBlack = true;
            else
            {
                FAIL("Unexpected color found in binary mode output.");
            }

            processedPixels++;
        }
    }
    auto sampleEnd = std::chrono::high_resolution_clock::now();

    // Calculate time per pixel and estimated total time
    double timePerPixel = std::chrono::duration<double>(sampleEnd - sampleStart).count() / samplePixels;
    double estimatedTotalTime = timePerPixel * totalPixels;

    std::cout << "Estimated total render time: " << estimatedTotalTime << " seconds.\n";

    // Continue processing remaining pixels with periodic updates
    processedPixels = 0; // Reset to include all pixels now
    for (int y = 0; y < camera.height; ++y)
    {
        for (int x = 0; x < camera.width; ++x)
        {
            Color pixelColor = raytracer.getImage().getPixel(x, y);

            if (areColorsEqual(pixelColor, white, tolerance))
                foundWhite = true;
            else if (areColorsEqual(pixelColor, black, tolerance))
                foundBlack = true;
            else
            {
                FAIL("Unexpected color found in binary mode output.");
            }

            // Update progress every 5% of total pixels
            processedPixels++;
            if (processedPixels % (totalPixels / 20) == 0) // 5% step
            {
                double elapsedTime = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start).count();
                double remainingTime = estimatedTotalTime - elapsedTime;
                std::cout << "Progress: " << (processedPixels * 100 / totalPixels) << "%, "
                          << "Estimated remaining time: " << remainingTime << " seconds\r";
                std::cout.flush();
            }
        }
    }

    std::cout << std::endl; // Newline after progress completion

    // Verify that both intersection and non-intersection pixels are present
    REQUIRE(foundWhite);
    REQUIRE(foundBlack);
}

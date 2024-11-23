#include "SceneLoader.h"
#include "Raytracer.h"
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <sstream>

// Function to render a single scene
void renderScene(const std::string &inputJson, const std::string &outputPpm, int samplesPerPixel)
{
    SceneLoader loader;
    Scene scene;

    std::cout << "Loading scene: " << inputJson << std::endl;

    // Load the scene from the JSON file
    if (!loader.loadScene(inputJson, scene))
    {
        std::cerr << "Failed to load scene from " << inputJson << std::endl;
        return;
    }

    std::cout << "Successfully loaded " << inputJson << std::endl;

    // Build the Bounding Volume Hierarchy (BVH) for the scene
    scene.buildBVH();

    // Initialize Raytracer with the camera's resolution and samples per pixel
    Raytracer raytracer(scene.camera.width, scene.camera.height, samplesPerPixel);

    // Render the scene and save the output PPM file
    raytracer.render(scene, outputPpm);
}

int main(int argc, char *argv[])
{
    // Minimum 3 arguments: program, input, output
    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " [--spp <samples_per_pixel>] <input_json1> <output_ppm1> [<input_json2> <output_ppm2> ...]" << std::endl;
        return 1;
    }

    int samplesPerPixel = 1; // Default value
    std::vector<std::pair<std::string, std::string>> scenes;

    // Parse command-line arguments
    for (int i = 1; i < argc;)
    {
        std::string arg = argv[i];
        if (arg == "--spp")
        {
            if (i + 1 >= argc)
            {
                std::cerr << "Error: --spp requires a numerical value." << std::endl;
                return 1;
            }
            try
            {
                samplesPerPixel = std::stoi(argv[i + 1]);
                if (samplesPerPixel < 1)
                {
                    std::cerr << "Samples per pixel must be at least 1. Using default value of 1." << std::endl;
                    samplesPerPixel = 1;
                }
            }
            catch (const std::invalid_argument &)
            {
                std::cerr << "Invalid value for samples per pixel. Using default value of 1." << std::endl;
                samplesPerPixel = 1;
            }
            i += 2;
        }
        else
        {
            if (i + 1 >= argc)
            {
                std::cerr << "Error: Missing output file for input " << arg << std::endl;
                return 1;
            }
            std::string inputJson = argv[i];
            std::string outputPpm = argv[i + 1];
            scenes.emplace_back(inputJson, outputPpm);
            i += 2;
        }
    }

    // Render all scenes
    for (const auto &scenePair : scenes)
    {
        renderScene(scenePair.first, scenePair.second, samplesPerPixel);
    }

    std::cout << "All scenes have been processed." << std::endl;

    return 0;
}

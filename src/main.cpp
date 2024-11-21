#include "SceneLoader.h"
#include "Raytracer.h"
#include <iostream>
#include <vector>
#include <string>
#include <chrono>

// Function to render a single scene
void renderScene(const std::string &inputJson, const std::string &outputPpm)
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

    // Initialize Raytracer with the camera's resolution
    Raytracer raytracer(scene.camera.width, scene.camera.height);

    // Render the scene and save the output PPM file
    raytracer.render(scene, outputPpm);
}

int main(int argc, char *argv[])
{
    // Check for at least one scene file
    if (argc < 3 || argc % 2 != 1)
    {
        std::cerr << "Usage: " << argv[0] << " <input_json1> <output_ppm1> [<input_json2> <output_ppm2> ...]" << std::endl;
        return 1;
    }

    // Process each pair of input JSON and output PPM
    for (int i = 1; i < argc; i += 2)
    {
        std::string inputJson = argv[i];
        std::string outputPpm = argv[i + 1];
        renderScene(inputJson, outputPpm);
    }

    std::cout << "All scenes have been processed." << std::endl;

    return 0;
}

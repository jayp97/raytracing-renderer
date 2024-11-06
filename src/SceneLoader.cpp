#include "SceneLoader.h"
#include <fstream>
#include <iostream>

// Loads the entire scene from a JSON file
bool SceneLoader::loadScene(const std::string &filePath)
{
    std::ifstream sceneFile(filePath);
    if (!sceneFile.is_open())
    {
        std::cerr << "Could not open file: " << filePath << std::endl;
        return false;
    }

    json sceneData;
    sceneFile >> sceneData;

    // Load Camera
    if (sceneData.contains("camera"))
    {
        camera = loadCamera(sceneData["camera"]);
    }
    else
    {
        std::cerr << "Camera data missing in JSON file." << std::endl;
        return false;
    }

    // Load Shapes
    if (sceneData.contains("scene") && sceneData["scene"].contains("shapes"))
    {
        auto shapesData = sceneData["scene"]["shapes"];
        spheres = loadSpheres(shapesData);
        cylinders = loadCylinders(shapesData);
        triangles = loadTriangles(shapesData);
    }
    else
    {
        std::cerr << "Shapes data missing in JSON file." << std::endl;
        return false;
    }

    return true;
}

// Loads camera data from JSON
Camera SceneLoader::loadCamera(const json &cameraData)
{
    Vector3 position(cameraData["position"][0], cameraData["position"][1], cameraData["position"][2]);
    Vector3 lookAt(cameraData["lookAt"][0], cameraData["lookAt"][1], cameraData["lookAt"][2]);
    Vector3 upVector(cameraData["upVector"][0], cameraData["upVector"][1], cameraData["upVector"][2]);
    float fov = cameraData["fov"];
    int width = cameraData["width"];
    int height = cameraData["height"];
    float exposure = cameraData["exposure"];

    // Pass width and height to match the Camera constructor
    return Camera(position, lookAt, upVector, fov, width, height, exposure);
}
// Loads sphere objects from JSON
std::vector<Sphere> SceneLoader::loadSpheres(const json &shapesData)
{
    std::vector<Sphere> spheres;
    for (const auto &shape : shapesData)
    {
        if (shape["type"] == "sphere")
        {
            Vector3 center(shape["center"][0], shape["center"][1], shape["center"][2]);
            float radius = shape["radius"];
            spheres.emplace_back(center, radius);
        }
    }
    return spheres;
}

// Loads cylinder objects from JSON
std::vector<Cylinder> SceneLoader::loadCylinders(const json &shapesData)
{
    std::vector<Cylinder> cylinders;
    for (const auto &shape : shapesData)
    {
        if (shape["type"] == "cylinder")
        {
            Vector3 center(shape["center"][0], shape["center"][1], shape["center"][2]);
            Vector3 axis(shape["axis"][0], shape["axis"][1], shape["axis"][2]);
            float radius = shape["radius"];
            float height = shape["height"];
            cylinders.emplace_back(center, axis, radius, height);
        }
    }
    return cylinders;
}

// Loads triangle objects from JSON
std::vector<Triangle> SceneLoader::loadTriangles(const json &shapesData)
{
    std::vector<Triangle> triangles;
    for (const auto &shape : shapesData)
    {
        if (shape["type"] == "triangle")
        {
            Vector3 v0(shape["v0"][0], shape["v0"][1], shape["v0"][2]);
            Vector3 v1(shape["v1"][0], shape["v1"][1], shape["v1"][2]);
            Vector3 v2(shape["v2"][0], shape["v2"][1], shape["v2"][2]);
            triangles.emplace_back(v0, v1, v2);
        }
    }
    return triangles;
}

#include "SceneLoader.h"
#include "Material.h"
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

    // Load Render Mode
    rendermode = sceneData.value("rendermode", "binary");

    // Load nbounces if available
    nbounces = sceneData.value("nbounces", 1);

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

    // Load Background Color
    if (sceneData.contains("scene") && sceneData["scene"].contains("backgroundcolor"))
    {
        backgroundColor = Vector3(sceneData["scene"]["backgroundcolor"][0], sceneData["scene"]["backgroundcolor"][1], sceneData["scene"]["backgroundcolor"][2]);
    }

    // Load Lights
    if (sceneData["scene"].contains("lightsources"))
    {
        lights = loadLights(sceneData["scene"]["lightsources"]);
    }

    // Load Shapes
    if (sceneData["scene"].contains("shapes"))
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

// Load Camera
Camera SceneLoader::loadCamera(const json &cameraData)
{
    Vector3 position(cameraData["position"][0], cameraData["position"][1], cameraData["position"][2]);
    Vector3 lookAt(cameraData["lookAt"][0], cameraData["lookAt"][1], cameraData["lookAt"][2]);
    Vector3 upVector(cameraData["upVector"][0], cameraData["upVector"][1], cameraData["upVector"][2]);
    float fov = cameraData["fov"];
    int width = cameraData["width"];
    int height = cameraData["height"];
    float exposure = cameraData["exposure"];

    return Camera(position, lookAt, upVector, fov, width, height, exposure);
}

// Loads light sources
std::vector<Light> SceneLoader::loadLights(const json &lightsData)
{
    std::vector<Light> lights;
    for (const auto &light : lightsData)
    {
        Vector3 position(light["position"][0], light["position"][1], light["position"][2]);
        Vector3 intensity(light["intensity"][0], light["intensity"][1], light["intensity"][2]);
        lights.emplace_back(position, intensity); // Assuming a Light constructor that takes position and intensity
    }
    return lights;
}

// Helper to load material properties
Material loadMaterial(const json &materialData)
{
    Material material;
    material.ks = materialData["ks"];
    material.kd = materialData["kd"];
    material.specularExponent = materialData["specularexponent"];
    material.diffuseColor = Vector3(materialData["diffusecolor"][0], materialData["diffusecolor"][1], materialData["diffusecolor"][2]);
    material.specularColor = Vector3(materialData["specularcolor"][0], materialData["specularcolor"][1], materialData["specularcolor"][2]);
    material.isReflective = materialData["isreflective"];
    material.reflectivity = materialData["reflectivity"];
    material.isRefractive = materialData["isrefractive"];
    material.refractiveIndex = materialData["refractiveindex"];
    return material;
}

// Load Spheres
std::vector<Sphere> SceneLoader::loadSpheres(const json &shapesData)
{
    std::vector<Sphere> spheres;
    for (const auto &shape : shapesData)
    {
        if (shape["type"] == "sphere")
        {
            Vector3 center(shape["center"][0], shape["center"][1], shape["center"][2]);
            float radius = shape["radius"];
            Material material = shape.contains("material") ? loadMaterial(shape["material"]) : Material();
            spheres.emplace_back(center, radius, material); // Sphere constructor with material
        }
    }
    return spheres;
}

// Load Cylinders
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
            Material material = shape.contains("material") ? loadMaterial(shape["material"]) : Material();
            cylinders.emplace_back(center, axis, radius, height, material); // Cylinder constructor with material
        }
    }
    return cylinders;
}

// Load Triangles
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
            Material material = shape.contains("material") ? loadMaterial(shape["material"]) : Material();
            triangles.emplace_back(v0, v1, v2, material); // Triangle constructor
        }
    }
    return triangles;
}
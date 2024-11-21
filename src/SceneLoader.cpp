// SceneLoader.cpp

#include "SceneLoader.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Triangle.h"
#include "Material.h"
#include "Color.h"
#include "CheckerboardTexture.h"
#include <fstream>
#include <iostream>

// Loads the entire scene from a JSON file and populates the Scene object
bool SceneLoader::loadScene(const std::string &filePath, Scene &scene)
{
    std::ifstream sceneFile(filePath);
    if (!sceneFile.is_open())
    {
        std::cerr << "Could not open file: " << filePath << std::endl;
        return false;
    }

    json sceneData;
    try
    {
        sceneFile >> sceneData;
    }
    catch (const json::parse_error &e)
    {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
        return false;
    }

    // Load Render Mode
    scene.renderMode = sceneData.value("rendermode", "binary");

    // Load nbounces if available
    scene.nbounces = sceneData.value("nbounces", 1);

    // Load Camera
    if (sceneData.contains("camera"))
    {
        scene.camera = loadCamera(sceneData["camera"]);
    }
    else
    {
        std::cerr << "Camera data missing in JSON file." << std::endl;
        return false;
    }

    // Load Background Color
    if (sceneData.contains("scene") && sceneData["scene"].contains("backgroundcolor"))
    {
        auto bg = sceneData["scene"]["backgroundcolor"];
        if (bg.is_array() && bg.size() == 3)
        {
            scene.backgroundColor = Color(bg[0], bg[1], bg[2]);
        }
        else
        {
            std::cerr << "Invalid backgroundColor format. Using default." << std::endl;
        }
    }

    // Load Lights
    if (sceneData["scene"].contains("lightsources"))
    {
        scene.lights = loadLights(sceneData["scene"]["lightsources"]);
    }

    // Load Objects
    if (sceneData["scene"].contains("shapes"))
    {
        auto shapesData = sceneData["scene"]["shapes"];
        for (const auto &shape : shapesData)
        {
            auto obj = loadObject(shape);
            if (obj)
            {
                scene.addObject(obj);
            }
        }
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
    float exposure = cameraData.value("exposure", 1.0f); // Default exposure if not specified

    return Camera(position, lookAt, upVector, fov, width, height, exposure);
}

// Loads light sources
std::vector<Light> SceneLoader::loadLights(const json &lightsData)
{
    std::vector<Light> lights;
    for (const auto &light : lightsData)
    {
        if (light.contains("position") && light.contains("intensity"))
        {
            Vector3 position(light["position"][0], light["position"][1], light["position"][2]);
            Color intensity(light["intensity"][0], light["intensity"][1], light["intensity"][2]);
            lights.emplace_back(position, intensity); // Assuming a Light constructor that takes position and intensity
        }
        else
        {
            std::cerr << "Invalid light source format. Skipping." << std::endl;
        }
    }
    return lights;
}

// Helper to load material properties
Material SceneLoader::loadMaterial(const json &materialData)
{
    Material material;
    // Map JSON fields to Material properties
    // Scene.json uses:
    // "ks": specular coefficient (float)
    // "kd": diffuse coefficient (float)
    // "specularexponent": specular exponent (float)
    // "diffusecolor": [r, g, b]
    // "specularcolor": [r, g, b]
    // "isreflective": bool
    // "reflectivity": float
    // "isrefractive": bool
    // "refractiveindex": float

    // Set diffuse color
    if (materialData.contains("diffusecolor") && materialData["diffusecolor"].is_array() && materialData["diffusecolor"].size() == 3)
    {
        material.diffuseColor = Color(materialData["diffusecolor"][0], materialData["diffusecolor"][1], materialData["diffusecolor"][2]);
    }
    else
    {
        // Default diffuse color
        material.diffuseColor = Color(0.5f, 0.5f, 0.5f);
    }

    // Set specular color
    if (materialData.contains("specularcolor") && materialData["specularcolor"].is_array() && materialData["specularcolor"].size() == 3)
    {
        material.specularColor = Color(materialData["specularcolor"][0], materialData["specularcolor"][1], materialData["specularcolor"][2]);
    }
    else
    {
        // Default specular color
        material.specularColor = Color(1.0f, 1.0f, 1.0f);
    }

    // Set shininess
    if (materialData.contains("specularexponent"))
    {
        material.specularExponent = materialData["specularexponent"];
    }
    else
    {
        // Default shininess
        material.specularExponent = 32.0f;
    }

    // Load texture if present
    if (materialData.contains("texture"))
    {
        auto textureData = materialData["texture"];
        if (textureData.contains("type"))
        {
            std::string textureType = textureData["type"];
            if (textureType == "checkerboard")
            {
                Color color1(1.0f, 1.0f, 1.0f);
                Color color2(0.0f, 0.0f, 0.0f);
                float scale = 1.0f;
                if (textureData.contains("color1") && textureData["color1"].is_array() && textureData["color1"].size() == 3)
                {
                    color1 = Color(textureData["color1"][0], textureData["color1"][1], textureData["color1"][2]);
                }
                if (textureData.contains("color2") && textureData["color2"].is_array() && textureData["color2"].size() == 3)
                {
                    color2 = Color(textureData["color2"][0], textureData["color2"][1], textureData["color2"][2]);
                }
                if (textureData.contains("scale"))
                {
                    scale = textureData["scale"];
                }
                material.texture = std::make_shared<CheckerboardTexture>(color1, color2, scale);
            }
            else
            {
                std::cerr << "Unknown texture type: " << textureType << ". Skipping texture." << std::endl;
            }
        }
    }

    // Set diffuse coefficient
    material.kd = materialData.value("kd", 0.9f);

    // Set specular coefficient
    material.ks = materialData.value("ks", 0.1f);

    // Derive ambient color as diffuseColor * kd
    material.ambient = material.diffuseColor * material.kd;

    // Set reflectivity
    material.isReflective = materialData.value("isreflective", false);
    material.reflectivity = materialData.value("reflectivity", 0.0f);

    // Set refractive properties
    material.isRefractive = materialData.value("isrefractive", false);
    material.refractiveIndex = materialData.value("refractiveindex", 1.0f);

    return material;
}

// Loads a single object (Sphere, Cylinder, Triangle)
std::shared_ptr<Object> SceneLoader::loadObject(const json &shapeData)
{
    if (!shapeData.contains("type"))
    {
        std::cerr << "Shape type missing. Skipping object." << std::endl;
        return nullptr;
    }

    std::string type = shapeData["type"];
    Material material = Material(); // Default material
    if (shapeData.contains("material"))
    {
        material = loadMaterial(shapeData["material"]);
    }

    if (type == "sphere")
    {
        if (shapeData.contains("center") && shapeData.contains("radius"))
        {
            Vector3 center(shapeData["center"][0], shapeData["center"][1], shapeData["center"][2]);
            float radius = shapeData["radius"];
            return std::make_shared<Sphere>(center, radius, material);
        }
        else
        {
            std::cerr << "Invalid sphere format. Skipping object." << std::endl;
            return nullptr;
        }
    }
    else if (type == "cylinder")
    {
        if (shapeData.contains("center") && shapeData.contains("axis") && shapeData.contains("radius") && shapeData.contains("height"))
        {
            Vector3 center(shapeData["center"][0], shapeData["center"][1], shapeData["center"][2]);
            Vector3 axis(shapeData["axis"][0], shapeData["axis"][1], shapeData["axis"][2]);
            float radius = shapeData["radius"];
            float height = shapeData["height"];
            return std::make_shared<Cylinder>(center, axis, radius, height, material);
        }
        else
        {
            std::cerr << "Invalid cylinder format. Skipping object." << std::endl;
            return nullptr;
        }
    }
    else if (type == "triangle")
    {
        if (shapeData.contains("v0") && shapeData.contains("v1") && shapeData.contains("v2"))
        {
            Vector3 v0(shapeData["v0"][0], shapeData["v0"][1], shapeData["v0"][2]);
            Vector3 v1(shapeData["v1"][0], shapeData["v1"][1], shapeData["v1"][2]);
            Vector3 v2(shapeData["v2"][0], shapeData["v2"][1], shapeData["v2"][2]);

            // Load UV coordinates if available
            Vector3 uv0(0.0f, 0.0f, 0.0f);
            Vector3 uv1(1.0f, 0.0f, 0.0f);
            Vector3 uv2(0.0f, 1.0f, 0.0f);
            if (shapeData.contains("uv0") && shapeData["uv0"].is_array() && shapeData["uv0"].size() >= 2)
            {
                uv0 = Vector3(shapeData["uv0"][0], shapeData["uv0"][1], 0.0f);
            }
            if (shapeData.contains("uv1") && shapeData["uv1"].is_array() && shapeData["uv1"].size() >= 2)
            {
                uv1 = Vector3(shapeData["uv1"][0], shapeData["uv1"][1], 0.0f);
            }
            if (shapeData.contains("uv2") && shapeData["uv2"].is_array() && shapeData["uv2"].size() >= 2)
            {
                uv2 = Vector3(shapeData["uv2"][0], shapeData["uv2"][1], 0.0f);
            }

            return std::make_shared<Triangle>(v0, v1, v2, material, uv0, uv1, uv2);
        }
        else
        {
            std::cerr << "Invalid triangle format. Skipping object." << std::endl;
            return nullptr;
        }
    }
    else
    {
        std::cerr << "Unknown shape type: " << type << ". Skipping object." << std::endl;
        return nullptr;
    }
}

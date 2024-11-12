// SceneLoader.h
#ifndef SCENELOADER_H
#define SCENELOADER_H

#include <vector>
#include <string>
#include <memory>
#include "nlohmann/json.hpp"
#include "Scene.h"   // Updated to include Scene
#include "Light.h"   // Light class
#include "Vector3.h" // Vector3 class

using json = nlohmann::json;

class SceneLoader
{
public:
    // Default constructor
    SceneLoader() {}

    // Loads the scene from a JSON file and populates the Scene object
    bool loadScene(const std::string &filePath, Scene &scene);

private:
    // Helper methods for loading different components
    Camera loadCamera(const json &cameraData);
    std::shared_ptr<Object> loadObject(const json &shapeData);
    std::vector<Light> loadLights(const json &lightsData); // Loads light sources
    Material loadMaterial(const json &materialData);       // Loads material properties
};

#endif // SCENELOADER_H

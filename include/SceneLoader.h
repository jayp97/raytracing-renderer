// SceneLoader.h
#ifndef SCENELOADER_H
#define SCENELOADER_H

#include <vector>
#include <string>
#include <memory>
#include "nlohmann/json.hpp"
#include "Scene.h"
#include "Light.h"
#include "Color.h"
#include "Vector3.h"

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
    std::vector<Light> loadLights(const json &lightsData);
    Material loadMaterial(const json &materialData);
};

#endif // SCENELOADER_H

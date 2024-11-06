#ifndef SCENELOADER_H
#define SCENELOADER_H

#include <vector>
#include <string>
#include "nlohmann/json.hpp"
#include "Camera.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Triangle.h"
#include "Vector3.h"

using json = nlohmann::json;

class SceneLoader
{
public:
    // Default constructor
    SceneLoader() : camera(), spheres(), cylinders(), triangles() {}

    // Loads the scene from a JSON file
    bool loadScene(const std::string &filePath);

    // Getters for loaded objects
    Camera getCamera() const { return camera; }
    const std::vector<Sphere> &getSpheres() const { return spheres; }
    const std::vector<Cylinder> &getCylinders() const { return cylinders; }
    const std::vector<Triangle> &getTriangles() const { return triangles; }

private:
    // Helper methods for loading different components
    Camera loadCamera(const json &cameraData);
    std::vector<Sphere> loadSpheres(const json &shapes);
    std::vector<Cylinder> loadCylinders(const json &shapes);
    std::vector<Triangle> loadTriangles(const json &shapes);

    // Scene components
    Camera camera;
    std::vector<Sphere> spheres;
    std::vector<Cylinder> cylinders;
    std::vector<Triangle> triangles;
};

#endif // SCENELOADER_H

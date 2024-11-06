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
#include "Light.h" // Assuming a Light class is defined to represent lights in the scene

using json = nlohmann::json;

class SceneLoader
{
public:
    // Default constructor
    SceneLoader() : camera(), spheres(), cylinders(), triangles(), rendermode("binary"), nbounces(1), backgroundColor(0.25f, 0.25f, 0.25f) {}

    // Loads the scene from a JSON file
    bool loadScene(const std::string &filePath);

    // Getters for loaded objects
    Camera getCamera() const { return camera; }
    const std::vector<Sphere> &getSpheres() const { return spheres; }
    const std::vector<Cylinder> &getCylinders() const { return cylinders; }
    const std::vector<Triangle> &getTriangles() const { return triangles; }
    const std::vector<Light> &getLights() const { return lights; }
    std::string getRenderMode() const { return rendermode; }
    int getNBounces() const { return nbounces; }
    Vector3 getBackgroundColor() const { return backgroundColor; }

private:
    // Helper methods for loading different components
    Camera loadCamera(const json &cameraData);
    std::vector<Sphere> loadSpheres(const json &shapes);
    std::vector<Cylinder> loadCylinders(const json &shapes);
    std::vector<Triangle> loadTriangles(const json &shapes);
    std::vector<Light> loadLights(const json &lightsData); // Loads light sources

    // Scene components
    Camera camera;
    std::vector<Sphere> spheres;
    std::vector<Cylinder> cylinders;
    std::vector<Triangle> triangles;
    std::vector<Light> lights; // New member to store lights
    std::string rendermode;    // Rendering mode, e.g., binary, phong, etc.
    int nbounces;              // Number of bounces for path tracing
    Vector3 backgroundColor;   // Background color
};

#endif // SCENELOADER_H

// Scene.h
#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>
#include "Object.h"  // Base class for Sphere, Cylinder, Triangle
#include "Light.h"   // Light class
#include "Camera.h"  // Camera class
#include "Vector3.h" // Vector3 class for colors and positions
#include "BVHNode.h" // BVHNode class for acceleration structure

class Scene
{
public:
    // Constructor
    Scene() : backgroundColor(0.25f, 0.25f, 0.25f), renderMode("binary"), nbounces(1), bvhRoot(nullptr) {}

    // Scene components
    Camera camera;                                // Camera
    std::vector<std::shared_ptr<Object>> objects; // Collection of objects (polymorphic)
    std::vector<Light> lights;                    // Collection of lights
    Color backgroundColor;                        // Background color
    std::string renderMode;                       // Rendering mode (e.g., binary, phong, pathtracer)
    int nbounces;                                 // Number of bounces for reflections/refractions

    // Method to add an object
    void addObject(const std::shared_ptr<Object> &obj)
    {
        objects.push_back(obj);
    }

    // Method to add a light
    void addLight(const Light &light)
    {
        lights.push_back(light);
    }

    // Method to build the BVH tree
    void buildBVH();

    // Intersection method declaration
    bool intersect(const Ray &ray, Intersection &closestHit) const;

private:
    // Private members or helper methods if needed
    std::shared_ptr<BVHNode> bvhRoot; // Root of the BVH tree
};

#endif // SCENE_H

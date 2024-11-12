// Scene.h
#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>
#include "Object.h"  // Base class for Sphere, Cylinder, Triangle
#include "Light.h"   // Light class
#include "Camera.h"  // Camera class
#include "Vector3.h" // Vector3 class for colors and positions

class Scene
{
public:
    // Constructor
    Scene() : backgroundColor(0.25f, 0.25f, 0.25f), renderMode("binary"), nbounces(1) {}

    // Scene components
    Camera camera;                                // Camera
    std::vector<std::shared_ptr<Object>> objects; // Collection of objects (polymorphic)
    std::vector<Light> lights;                    // Collection of lights
    Vector3 backgroundColor;                      // Background color
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

    // Acceleration structure or method can be added here
    // For example, a BVH tree or a grid

    // Intersection method to find the closest hit
    bool intersect(const Ray &ray, Intersection &closestHit) const
    {
        bool hitAnything = false;
        float closestDistance = std::numeric_limits<float>::max();

        for (const auto &object : objects)
        {
            Intersection hit;
            if (object->intersect(ray, hit))
            {
                if (hit.distance < closestDistance)
                {
                    closestDistance = hit.distance;
                    closestHit = hit;
                    hitAnything = true;
                }
            }
        }

        return hitAnything;
    }

private:
    // Private members or helper methods if needed
};

#endif // SCENE_H

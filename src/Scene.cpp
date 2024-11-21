// Scene.cpp
#include "Scene.h"
#include "BVHNode.h"
#include <limits>

// Build the BVH tree
void Scene::buildBVH()
{
    if (!objects.empty())
    {
        bvhRoot = std::make_shared<BVHNode>(objects, 0, objects.size());
    }
    else
    {
        bvhRoot = nullptr;
    }
}

// Intersection method using BVH traversal
bool Scene::intersect(const Ray &ray, Intersection &closestHit) const
{
    closestHit.distance = std::numeric_limits<float>::max();

    if (bvhRoot)
    {
        return bvhRoot->intersect(ray, closestHit);
    }
    else
    {
        // Fallback to brute-force if BVH is not built
        bool hitAnything = false;
        for (const auto &object : objects)
        {
            Intersection tempHit;
            if (object->intersect(ray, tempHit))
            {
                if (tempHit.distance < closestHit.distance)
                {
                    closestHit = tempHit;
                    hitAnything = true;
                }
            }
        }
        return hitAnything;
    }
}

// Scene.cpp
#include "Scene.h"
#include "BVHNode.h"
#include <limits>
#include <iostream>

void Scene::buildBVH()
{
    if (objects.empty())
    {
        bvhRoot = nullptr;
        return;
    }

    bvhRoot = std::make_shared<BVHNode>(objects, 0, objects.size());
}

bool Scene::intersect(const Ray &ray, Intersection &closestHit) const
{
    if (!bvhRoot)
        return false;

    // Initialize closestHit.distance to infinity
    closestHit.distance = std::numeric_limits<float>::infinity();

    bool hit = bvhRoot->intersect(ray, closestHit);

    return hit;
}

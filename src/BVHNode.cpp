// BVHNode.cpp
#include "BVHNode.h"
#include <algorithm>
#include <limits>
#include <cstdlib> // For rand()
#include <iostream>

// Default constructor implementation
BVHNode::BVHNode() : left(nullptr), right(nullptr), object(nullptr) {}

// Helper function to select the axis with the largest extent
int BVHNode::selectAxis(const Vector3 &extent) const
{
    if (extent.x >= extent.y && extent.x >= extent.z)
        return 0; // X-axis
    else if (extent.y >= extent.x && extent.y >= extent.z)
        return 1; // Y-axis
    else
        return 2; // Z-axis
}

// BVHNode constructor that builds the BVH tree
BVHNode::BVHNode(std::vector<std::shared_ptr<Object>> &objects, size_t start, size_t end)
{
    // Compute the bounding box for all objects in this node
    box = objects[start]->getBoundingBox();
    for (size_t i = start + 1; i < end; ++i)
    {
        box.expand(objects[i]->getBoundingBox());
    }

    size_t objectSpan = end - start;

    if (objectSpan == 1)
    {
        // Leaf node
        object = objects[start];
        left = right = nullptr;
    }
    else
    {
        // Determine the axis with the largest extent
        Vector3 extent = box.getMax() - box.getMin();
        int axis = selectAxis(extent);

        // Sort objects based on the chosen axis
        std::sort(objects.begin() + start, objects.begin() + end, [axis](const std::shared_ptr<Object> &a, const std::shared_ptr<Object> &b)
                  { return a->getBoundingBox().getMin()[axis] < b->getBoundingBox().getMin()[axis]; });

        // Find the midpoint to split the objects
        size_t mid = start + objectSpan / 2;

        // Recursively build the left and right child nodes
        left = std::make_shared<BVHNode>(objects, start, mid);
        right = std::make_shared<BVHNode>(objects, mid, end);
    }
}

// Intersection method implementation
bool BVHNode::intersect(const Ray &ray, Intersection &hit) const
{
    if (!box.intersect(ray, 0.001f, hit.distance))
        return false;

    bool hitSomething = false;

    if (object)
    {
        // Leaf node: check intersection with the single object
        Intersection tempHit;
        if (object->intersect(ray, tempHit))
        {
            if (tempHit.distance < hit.distance)
            {
                hit = tempHit;
                hitSomething = true;
            }
        }
    }
    else
    {
        // Internal node: check intersection with both children
        if (left && left->intersect(ray, hit))
            hitSomething = true;
        if (right && right->intersect(ray, hit))
            hitSomething = true;
    }

    return hitSomething;
}

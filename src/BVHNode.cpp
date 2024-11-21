// BVHNode.cpp
#include "BVHNode.h"
#include <algorithm>
#include <limits>
#include <cstdlib> // For rand()

BVHNode::BVHNode() : left(nullptr), right(nullptr), object(nullptr) {}

BVHNode::BVHNode(const std::vector<std::shared_ptr<Object>> &objects, size_t start, size_t end)
{
    // Compute the bounding box for all objects in this node
    std::vector<std::shared_ptr<Object>> sortedObjects = objects; // Copy objects

    // Choose an axis to sort by (we'll cycle through axes)
    int axis = rand() % 3;
    auto comparator = [axis](const std::shared_ptr<Object> &a, const std::shared_ptr<Object> &b)
    {
        return a->getBoundingBox().getMin()[axis] < b->getBoundingBox().getMin()[axis];
    };

    size_t objectSpan = end - start;
    if (objectSpan == 1)
    {
        // Leaf node
        object = objects[start];
        box = object->getBoundingBox();
        left = right = nullptr;
    }
    else if (objectSpan == 2)
    {
        // Create two leaf nodes
        if (comparator(objects[start], objects[start + 1]))
        {
            left = std::make_shared<BVHNode>(objects, start, start + 1);
            right = std::make_shared<BVHNode>(objects, start + 1, start + 2);
        }
        else
        {
            left = std::make_shared<BVHNode>(objects, start + 1, start + 2);
            right = std::make_shared<BVHNode>(objects, start, start + 1);
        }
        box = left->box;
        box.expand(right->box);
    }
    else
    {
        // Sort objects and split
        std::sort(sortedObjects.begin() + start, sortedObjects.begin() + end, comparator);
        size_t mid = start + objectSpan / 2;
        left = std::make_shared<BVHNode>(sortedObjects, start, mid);
        right = std::make_shared<BVHNode>(sortedObjects, mid, end);

        box = left->box;
        box.expand(right->box);
    }
}

bool BVHNode::intersect(const Ray &ray, Intersection &hit) const
{
    if (!box.intersect(ray, 0.001f, hit.distance))
        return false;

    if (object)
    {
        // Leaf node
        return object->intersect(ray, hit);
    }

    bool hitLeft = left && left->intersect(ray, hit);
    bool hitRight = right && right->intersect(ray, hit);

    return hitLeft || hitRight;
}

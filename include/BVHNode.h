// BVHNode.h
#ifndef BVH_NODE_H
#define BVH_NODE_H

#include "BoundingBox.h"
#include "Object.h"
#include <vector>
#include <memory>

class BVHNode
{
public:
    BVHNode();

    // Build the BVH node from a list of objects
    BVHNode(const std::vector<std::shared_ptr<Object>> &objects, size_t start, size_t end);

    // Check for intersection with the BVH node
    bool intersect(const Ray &ray, Intersection &hit) const;

private:
    BoundingBox box;
    std::shared_ptr<BVHNode> left;
    std::shared_ptr<BVHNode> right;
    std::shared_ptr<Object> object; // For leaf nodes
};

#endif // BVH_NODE_H

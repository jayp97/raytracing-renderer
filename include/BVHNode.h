// BVHNode.h
#ifndef BVH_NODE_H
#define BVH_NODE_H

#include "BoundingBox.h"
#include "Object.h"
#include "Ray.h"
#include "Intersection.h"
#include <vector>
#include <memory>

class BVHNode
{
public:
    // Default constructor
    BVHNode();

    // Constructor that builds the BVH node from a list of objects
    // Accepts a non-const reference to allow sorting
    BVHNode(std::vector<std::shared_ptr<Object>> &objects, size_t start, size_t end);

    // Check for intersection with the BVH node
    bool intersect(const Ray &ray, Intersection &hit) const;

private:
    BoundingBox box;                // Bounding box for this node
    std::shared_ptr<BVHNode> left;  // Left child
    std::shared_ptr<BVHNode> right; // Right child
    std::shared_ptr<Object> object; // For leaf nodes

    // Helper function to select the axis with the largest extent
    int selectAxis(const Vector3 &extent) const;
};

#endif // BVH_NODE_H

// Triangle.cpp
#include "Triangle.h"
#include <cmath>     // For fabs and other math functions
#include <algorithm> // For std::min and std::max

bool Triangle::intersect(const Ray &ray, Intersection &hit) const
{
    const float EPSILON = 1e-8f;
    Vector3 edge1 = v1 - v0;
    Vector3 edge2 = v2 - v0;

    Vector3 h = ray.direction.cross(edge2);
    float a = edge1.dot(h);

    if (std::fabs(a) < EPSILON)
        return false; // Ray is parallel to the triangle

    float f = 1.0f / a;
    Vector3 s = ray.origin - v0;
    float u = f * s.dot(h);

    if (u < 0.0f || u > 1.0f)
        return false;

    Vector3 q = s.cross(edge1);
    float v = f * ray.direction.dot(q);

    if (v < 0.0f || (u + v) > 1.0f)
        return false;

    float t_val = f * edge2.dot(q);

    if (t_val > EPSILON)
    { // Ray intersection
        hit.distance = t_val;
        hit.point = ray.origin + ray.direction * t_val;

        // Compute the normal and ensure it's facing the ray
        Vector3 normal = edge1.cross(edge2).normalise();
        if (ray.direction.dot(normal) > 0)
            normal = -normal; // Flip normal if it's facing away

        hit.normal = normal;
        hit.material = material;

        // Store barycentric coordinates for potential texture mapping
        hit.u = u;
        hit.v = v;

        return true;
    }
    else
    { // Line intersection but not a ray intersection
        return false;
    }
}

BoundingBox Triangle::getBoundingBox() const
{
    Vector3 minPoint = Vector3(
        std::min({v0.x, v1.x, v2.x}),
        std::min({v0.y, v1.y, v2.y}),
        std::min({v0.z, v1.z, v2.z}));

    Vector3 maxPoint = Vector3(
        std::max({v0.x, v1.x, v2.x}),
        std::max({v0.y, v1.y, v2.y}),
        std::max({v0.z, v1.z, v2.z}));

    // Add a small epsilon to ensure the bounding box has non-zero thickness
    const float epsilon = 1e-4f;
    minPoint = minPoint - Vector3(epsilon, epsilon, epsilon);
    maxPoint = maxPoint + Vector3(epsilon, epsilon, epsilon);

    return BoundingBox(minPoint, maxPoint);
}

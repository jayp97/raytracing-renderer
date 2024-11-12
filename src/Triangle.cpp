// Triangle.cpp
#include "Triangle.h"
#include <cmath> // For fabs and other math functions

bool Triangle::intersect(const Ray &ray, Intersection &hit) const
{
    const float EPSILON = 1e-8f;
    Vector3 edge1 = v1 - v0;
    Vector3 edge2 = v2 - v0;

    Vector3 h = ray.direction.cross(edge2);
    float a = edge1.dot(h);

    // If a is near zero, the ray is parallel to the triangle
    if (std::fabs(a) < EPSILON)
        return false;

    float f = 1.0f / a;
    Vector3 s = ray.origin - v0;
    float u = f * s.dot(h);

    // Check if the intersection is outside the triangle
    if (u < 0.0f || u > 1.0f)
        return false;

    Vector3 q = s.cross(edge1);
    float v = f * ray.direction.dot(q);

    // Check if the intersection is outside the triangle
    if (v < 0.0f || u + v > 1.0f)
        return false;

    // Calculate t to find out where the intersection point is on the ray
    float t_val = f * edge2.dot(q);

    if (t_val > EPSILON)
    { // Ray intersection
        // Populate the Intersection structure
        hit.distance = t_val;
        hit.point = ray.origin + ray.direction * t_val;
        hit.normal = edge1.cross(edge2).normalise(); // Ensure the normal is normalized
        hit.material = material;

        return true;
    }
    else
    { // This means that there is a line intersection but not a ray intersection
        return false;
    }
}

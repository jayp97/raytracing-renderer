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

    if (std::fabs(a) < EPSILON)
        return false;

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
        hit.normal = edge1.cross(edge2).normalise();
        hit.material = material;

        // Compute the third barycentric coordinate
        float w = 1.0f - u - v;

        // Interpolate UV coordinates using barycentric coordinates
        float texU = w * uv0.x + u * uv1.x + v * uv2.x;
        float texV = w * uv0.y + u * uv1.y + v * uv2.y;

        // Store UV coordinates in the Intersection
        hit.u = u;
        hit.v = v;

        return true;
    }
    else
    { // Line intersection but not a ray intersection
        return false;
    }
}

// Triangle.cpp
#include "Triangle.h"
#include <cmath>

bool Triangle::intersect(const Ray &ray, float &t, float &u, float &v) const
{
    const float EPSILON = 1e-8;
    Vector3 edge1 = v1 - v0;
    Vector3 edge2 = v2 - v0;

    Vector3 h = ray.direction.cross(edge2);
    float a = edge1.dot(h);

    if (std::fabs(a) < EPSILON)
    {
        return false; // Ray is parallel to the triangle
    }

    float f = 1.0f / a;
    Vector3 s = ray.origin - v0;
    u = f * s.dot(h);

    if (u < 0.0 || u > 1.0)
    {
        return false; // Intersection point is outside the triangle
    }

    Vector3 q = s.cross(edge1);
    v = f * ray.direction.dot(q);

    if (v < 0.0 || u + v > 1.0)
    {
        return false; // Intersection point is outside the triangle
    }

    t = f * edge2.dot(q);

    if (t > EPSILON)
    {
        return true; // Intersection point is valid
    }
    else
    {
        return false; // Intersection is behind the ray origin
    }
}

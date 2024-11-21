// Sphere.cpp
#include "Sphere.h"
#include <cmath> // For sqrt and atan2

bool Sphere::intersect(const Ray &ray, Intersection &hit) const
{
    Vector3 oc = ray.origin - center;
    float a = ray.direction.dot(ray.direction);
    float b = 2.0f * oc.dot(ray.direction);
    float c = oc.dot(oc) - radius * radius;
    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0.0f)
        return false;

    float sqrtDisc = sqrt(discriminant);
    float t0 = (-b - sqrtDisc) / (2.0f * a);
    float t1 = (-b + sqrtDisc) / (2.0f * a);

    if (t0 > t1)
        std::swap(t0, t1);

    float t;
    if (t0 > 1e-4f)
    {
        t = t0;
    }
    else if (t1 > 1e-4f)
    {
        t = t1;
    }
    else
    {
        return false;
    }

    hit.distance = t;
    hit.point = ray.origin + ray.direction * t;
    hit.normal = (hit.point - center).normalise();
    hit.material = material;

    // Compute UV coordinates for texture mapping
    Vector3 p = (hit.point - center).normalise(); // Point on unit sphere
    float u = 0.5f + (std::atan2(p.z, p.x) / (2.0f * M_PI));
    float v = 0.5f - (std::asin(p.y) / M_PI);

    hit.u = u;
    hit.v = v;

    return true;
}

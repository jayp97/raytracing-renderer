// Sphere.cpp
#include "Sphere.h"
#include <cmath> // For sqrt

// Sphere.h
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

    // Swap if necessary to ensure t0 is the smaller value
    if (t0 > t1)
        std::swap(t0, t1);

    // Find the nearest positive t
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
        // Both t0 and t1 are negative or too close to zero
        return false;
    }

    hit.distance = t;
    hit.point = ray.origin + ray.direction * t;
    hit.normal = (hit.point - center).normalise();
    hit.material = material;

    return true;
}

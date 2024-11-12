// Sphere.cpp
#include "Sphere.h"
#include <cmath> // For sqrt

bool Sphere::intersect(const Ray &ray, Intersection &hit) const
{
    Vector3 oc = ray.origin - center;
    float a = ray.direction.dot(ray.direction);
    float b = 2.0f * oc.dot(ray.direction);
    float c = oc.dot(oc) - radius * radius;

    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0)
    {
        return false; // No intersection
    }
    else
    {
        float sqrtDiscriminant = std::sqrt(discriminant);
        float t1 = (-b - sqrtDiscriminant) / (2.0f * a);
        float t2 = (-b + sqrtDiscriminant) / (2.0f * a);

        float t = 0.0f;

        // Find the nearest positive t
        if (t1 > 0.001f)
        {
            t = t1;
        }
        else if (t2 > 0.001f)
        {
            t = t2;
        }
        else
        {
            return false; // Both intersections are behind the ray origin
        }

        // Populate the Intersection structure
        hit.distance = t;
        hit.point = ray.origin + ray.direction * t;
        hit.normal = (hit.point - center).normalise();
        hit.material = material;

        return true;
    }
}

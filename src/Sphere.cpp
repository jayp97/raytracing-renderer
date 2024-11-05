#include "Sphere.h"
#include <cmath> // Correcting the inclusion of cmath

bool Sphere::intersect(const Ray &ray, float &t) const
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
        float sqrtDiscriminant = sqrt(discriminant);
        float t1 = (-b - sqrtDiscriminant) / (2.0f * a);
        float t2 = (-b + sqrtDiscriminant) / (2.0f * a);

        if (t1 > 0.001f)
        {
            t = t1;
            return true;
        }
        else if (t2 > 0.001f)
        {
            t = t2;
            return true;
        }
        return false; // Intersection is behind the ray origin
    }
}

#include "Plane.h"
#include "Ray.h"
#include <cmath>

bool Plane::intersect(const Ray &ray, Intersection &hit) const
{
    float denom = normal.dot(ray.direction);
    if (std::fabs(denom) > 1e-6)
    {
        float t = (point - ray.origin).dot(normal) / denom;
        if (t > 0)
        {
            hit.point = ray.origin + ray.direction * t;
            hit.normal = normal;
            hit.material = material;
            hit.distance = t;
            return true;
        }
    }
    return false;
}

#include "Cylinder.h"
#include <cmath>
#include <limits> // For std::numeric_limits

// Updated constructor with Material parameter
Cylinder::Cylinder(const Vector3 &c, const Vector3 &a, float r, float h, const Material &m)
    : center(c), axis(a.normalise()), radius(r), height(h), material(m) {}

// Method to check ray-cylinder intersection
bool Cylinder::intersect(const Ray &ray, float &t) const
{
    Vector3 oc = ray.origin - center;

    // Components perpendicular to the cylinder axis
    Vector3 rayDirPerp = ray.direction - (ray.direction.dot(axis)) * axis;
    Vector3 ocPerp = oc - (oc.dot(axis)) * axis;

    float a = rayDirPerp.dot(rayDirPerp);
    float b = 2.0f * rayDirPerp.dot(ocPerp);
    float c = ocPerp.dot(ocPerp) - radius * radius;

    float discriminant = b * b - 4 * a * c;
    bool hit = false;

    if (discriminant >= 0)
    {
        float sqrtDisc = std::sqrt(discriminant);
        float t1 = (-b - sqrtDisc) / (2.0f * a);
        float t2 = (-b + sqrtDisc) / (2.0f * a);

        // Check for valid intersection along the side surface
        float tSide = std::numeric_limits<float>::max();
        for (float tempT : {t1, t2})
        {
            if (tempT > 0 && tempT < t)
            {
                Vector3 point = ray.origin + tempT * ray.direction;
                float heightCheck = (point - center).dot(axis);
                if (heightCheck >= 0 && heightCheck <= height)
                {
                    if (tempT < tSide)
                    {
                        tSide = tempT;
                        t = tempT;
                        hit = true;
                    }
                }
            }
        }
    }

    // Check for cap intersections only if the ray is not parallel to the caps
    float axisDotRayDir = ray.direction.dot(axis);
    const float epsilon = 1e-6f;

    if (std::fabs(axisDotRayDir) > epsilon)
    {
        // Top cap
        float tTop = ((center + axis * height) - ray.origin).dot(axis) / axisDotRayDir;
        if (tTop > 0 && tTop < t)
        {
            Vector3 point = ray.origin + tTop * ray.direction;
            Vector3 v = point - (center + axis * height);
            if (v.dot(v) - std::pow(v.dot(axis), 2) <= radius * radius)
            {
                t = tTop;
                hit = true;
            }
        }

        // Bottom cap
        float tBottom = (center - ray.origin).dot(axis) / axisDotRayDir;
        if (tBottom > 0 && tBottom < t)
        {
            Vector3 point = ray.origin + tBottom * ray.direction;
            Vector3 v = point - center;
            if (v.dot(v) - std::pow(v.dot(axis), 2) <= radius * radius)
            {
                t = tBottom;
                hit = true;
            }
        }
    }

    return hit;
}

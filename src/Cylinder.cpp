// Cylinder.cpp
#include "Cylinder.h"
#include <cmath>
#include <limits>

// Constructor with Material parameter
Cylinder::Cylinder(const Vector3 &c, const Vector3 &a, float r, float h, const Material &m)
    : center(c), axis(a.normalise()), radius(r), height(h), material(m) {}

// Method to check ray-cylinder intersection and populate Intersection
bool Cylinder::intersect(const Ray &ray, Intersection &hit) const
{
    Vector3 oc = ray.origin - center;

    // Components perpendicular to the cylinder axis
    Vector3 rayDirPerp = ray.direction - axis * ray.direction.dot(axis);
    Vector3 ocPerp = oc - axis * oc.dot(axis);

    float a = rayDirPerp.dot(rayDirPerp);
    float b = 2.0f * rayDirPerp.dot(ocPerp);
    float c = ocPerp.dot(ocPerp) - radius * radius;

    float discriminant = b * b - 4 * a * c;
    bool hitCylinderSide = false;
    float tSide = std::numeric_limits<float>::max();

    // Check for intersection with the side surface of the cylinder
    if (discriminant >= 0)
    {
        float sqrtDisc = std::sqrt(discriminant);
        float t1 = (-b - sqrtDisc) / (2.0f * a);
        float t2 = (-b + sqrtDisc) / (2.0f * a);

        for (float tTemp : {t1, t2})
        {
            if (tTemp > 1e-4f) // Avoid self-intersection
            {
                Vector3 point = ray.origin + tTemp * ray.direction;
                float heightCheck = (point - center).dot(axis);
                if (heightCheck >= 0 && heightCheck <= height)
                {
                    if (tTemp < tSide)
                    {
                        tSide = tTemp;
                        hitCylinderSide = true;
                    }
                }
            }
        }
    }

    // Initialize to no intersection
    bool hitAnything = false;
    float tFinal = std::numeric_limits<float>::max();

    // If intersection with side surface is found
    if (hitCylinderSide && tSide < tFinal)
    {
        tFinal = tSide;
        hit.point = ray.origin + tFinal * ray.direction;
        hit.normal = getNormal(hit.point);
        hit.material = material;
        hit.distance = tFinal;
        hitAnything = true;
    }

    // Check for intersection with the top and bottom caps
    float tCap;
    Vector3 capCenter;
    Vector3 capNormal;

    // Top cap
    capCenter = center + axis * height;
    capNormal = axis;
    float denom = ray.direction.dot(capNormal);
    if (std::fabs(denom) > 1e-6f)
    {
        tCap = (capCenter - ray.origin).dot(capNormal) / denom;
        if (tCap > 1e-4f && tCap < tFinal)
        {
            Vector3 pCap = ray.origin + tCap * ray.direction;
            Vector3 v = pCap - capCenter;
            if (v.dot(v) <= radius * radius)
            {
                tFinal = tCap;
                hit.point = pCap;
                hit.normal = capNormal;
                hit.material = material;
                hit.distance = tFinal;
                hitAnything = true;
            }
        }
    }

    // Bottom cap
    capCenter = center;
    capNormal = -axis;
    denom = ray.direction.dot(capNormal);
    if (std::fabs(denom) > 1e-6f)
    {
        tCap = (capCenter - ray.origin).dot(capNormal) / denom;
        if (tCap > 1e-4f && tCap < tFinal)
        {
            Vector3 pCap = ray.origin + tCap * ray.direction;
            Vector3 v = pCap - capCenter;
            if (v.dot(v) <= radius * radius)
            {
                tFinal = tCap;
                hit.point = pCap;
                hit.normal = capNormal;
                hit.material = material;
                hit.distance = tFinal;
                hitAnything = true;
            }
        }
    }

    return hitAnything;
}

// Method to get the normal at a point on the cylinder
Vector3 Cylinder::getNormal(const Vector3 &point) const
{
    Vector3 v = point - center;
    float projection = v.dot(axis);
    Vector3 projectionPoint = center + axis * projection;
    Vector3 normal = (point - projectionPoint).normalise();
    return normal;
}

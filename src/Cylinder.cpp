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

        // Compute UV coordinates for the cylinder side
        Vector3 v = hit.point - center;
        float projection = v.dot(axis);
        Vector3 circleCenter = center + axis * projection;
        Vector3 d = (hit.point - circleCenter);

        float theta = std::atan2(d.z, d.x);
        if (theta < 0.0f)
            theta += 2.0f * M_PI;

        float u = theta / (2.0f * M_PI);     // Range [0,1]
        float v_coord = projection / height; // Range [0,1] along the height

        hit.u = u;
        hit.v = v_coord;

        hitAnything = true;
    }

    // Prepare orthogonal vectors for cap UV mapping
    Vector3 uDir, vDir;
    if (std::fabs(axis.x) > std::fabs(axis.y))
        uDir = Vector3(-axis.z, 0, axis.x).normalise();
    else
        uDir = Vector3(0, axis.z, -axis.y).normalise();
    vDir = axis.cross(uDir).normalise();

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

                // Compute UV coordinates for the top cap
                Vector3 localPoint = pCap - capCenter;
                float u = (localPoint.dot(uDir) / radius) * 0.5f + 0.5f;
                float v_coord = (localPoint.dot(vDir) / radius) * 0.5f + 0.5f;

                hit.u = u;
                hit.v = v_coord;

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

                // Compute UV coordinates for the bottom cap
                Vector3 localPoint = pCap - capCenter;
                float u = (localPoint.dot(uDir) / radius) * 0.5f + 0.5f;
                float v_coord = (localPoint.dot(vDir) / radius) * 0.5f + 0.5f;

                hit.u = u;
                hit.v = v_coord;

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

    // Check if the point is on the side or caps
    if (projection >= 0 && projection <= height)
    {
        // Side surface
        Vector3 normal = (point - projectionPoint).normalise();
        return normal;
    }
    else if (std::fabs(projection) < 1e-4f)
    {
        // Bottom cap
        return -axis;
    }
    else
    {
        // Top cap
        return axis;
    }
}

BoundingBox Cylinder::getBoundingBox() const
{
    // Approximate the bounding box by considering the cylinder's axis-aligned dimensions
    Vector3 radiusVec(radius, radius, radius);

    // Compute base and top centers
    Vector3 baseCenter = center;
    Vector3 topCenter = center + axis * height;

    // Initialize min and max points
    Vector3 minPoint = baseCenter - radiusVec;
    Vector3 maxPoint = baseCenter + radiusVec;

    // Expand to include the top
    minPoint = Vector3::min(minPoint, topCenter - radiusVec);
    maxPoint = Vector3::max(maxPoint, topCenter + radiusVec);

    return BoundingBox(minPoint, maxPoint);
}
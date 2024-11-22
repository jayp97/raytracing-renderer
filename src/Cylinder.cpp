// Cylinder.cpp
#include "Cylinder.h"
#include <cmath>     // For sqrt, fabs, etc.
#include <limits>    // For numeric_limits
#include <algorithm> // For std::min and std::max
#include <iostream>  // For debugging

// Constructor implementation
Cylinder::Cylinder(const Vector3 &c, const Vector3 &a, float r, float h, const Material &m)
    : center(c), axis(a.normalise()), radius(r), height(h), material(m) {}

// Method to check ray-cylinder intersection and populate Intersection
bool Cylinder::intersect(const Ray &ray, Intersection &hit) const
{
    Vector3 oc = ray.origin - center;

    // Since the cylinder is aligned along the Y-axis, we can simplify calculations
    // Project the ray and oc onto the XZ-plane
    Vector3 rayDirPerp(ray.direction.x, 0.0f, ray.direction.z);
    Vector3 ocPerp(oc.x, 0.0f, oc.z);

    float a = rayDirPerp.dot(rayDirPerp);
    float b = 2.0f * ocPerp.dot(rayDirPerp);
    float c = ocPerp.dot(ocPerp) - radius * radius;

    float discriminant = b * b - 4.0f * a * c;
    bool hitCylinderSide = false;
    float tSide = std::numeric_limits<float>::max();

    // Check for intersection with the side surface of the cylinder
    if (discriminant >= 0.0f)
    {
        float sqrtDisc = sqrt(discriminant);
        float t0 = (-b - sqrtDisc) / (2.0f * a);
        float t1 = (-b + sqrtDisc) / (2.0f * a);

        // Check both possible intersection points
        for (float tTemp : {t0, t1})
        {
            if (tTemp > 1e-4f) // Avoid self-intersection
            {
                Vector3 point = ray.origin + ray.direction * tTemp;
                // Treat 'height' as half-height to double the actual height
                float halfHeight = height;
                if (point.y >= center.y - halfHeight && point.y <= center.y + halfHeight)
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
    float tFinal = std::numeric_limits<float>::infinity();

    // If intersection with side surface is found
    if (hitCylinderSide && tSide < tFinal)
    {
        tFinal = tSide;
        hit.point = ray.origin + ray.direction * tFinal;
        hit.normal = Vector3(hit.point.x - center.x, 0.0f, hit.point.z - center.z).normalise();
        hit.material = material;
        hit.distance = tFinal;

        // Compute UV coordinates for the cylinder side
        float theta = std::atan2(hit.point.z - center.z, hit.point.x - center.x);
        if (theta < 0.0f)
            theta += 2.0f * M_PI;

        float u = theta / (2.0f * M_PI);                                       // Range [0,1]
        float v_coord = (hit.point.y - (center.y - height)) / (2.0f * height); // Adjusted for doubled height

        hit.u = u;
        hit.v = v_coord;

        hitAnything = true;

        // Debugging statement
        // std::cout << "Cylinder side hit at distance: " << tSide << "\n";
    }

    // Check for intersection with the top and bottom caps
    float halfHeight = height; // Treat 'height' as half-height to double the actual height

    // Top cap
    float tTop = (center.y + halfHeight - ray.origin.y) / ray.direction.y;
    if (tTop > 1e-4f && tTop < tFinal)
    {
        Vector3 pTop = ray.origin + ray.direction * tTop;
        if ((pTop.x - center.x) * (pTop.x - center.x) + (pTop.z - center.z) * (pTop.z - center.z) <= radius * radius)
        {
            tFinal = tTop;
            hit.point = pTop;
            hit.normal = Vector3(0.0f, 1.0f, 0.0f); // Top cap normal
            hit.material = material;
            hit.distance = tFinal;

            // Compute UV coordinates for the top cap
            float u = (pTop.x - center.x) / radius * 0.5f + 0.5f;
            float v_coord = (pTop.z - center.z) / radius * 0.5f + 0.5f;

            hit.u = u;
            hit.v = v_coord;

            hitAnything = true;

            // Debugging statement
            // std::cout << "Cylinder top cap hit at distance: " << tTop << "\n";
        }
    }

    // Bottom cap
    float tBottom = (center.y - halfHeight - ray.origin.y) / ray.direction.y;
    if (tBottom > 1e-4f && tBottom < tFinal)
    {
        Vector3 pBottom = ray.origin + ray.direction * tBottom;
        if ((pBottom.x - center.x) * (pBottom.x - center.x) + (pBottom.z - center.z) * (pBottom.z - center.z) <= radius * radius)
        {
            tFinal = tBottom;
            hit.point = pBottom;
            hit.normal = Vector3(0.0f, -1.0f, 0.0f); // Bottom cap normal
            hit.material = material;
            hit.distance = tFinal;

            // Compute UV coordinates for the bottom cap
            float u = (pBottom.x - center.x) / radius * 0.5f + 0.5f;
            float v_coord = (pBottom.z - center.z) / radius * 0.5f + 0.5f;

            hit.u = u;
            hit.v = v_coord;

            hitAnything = true;

            // Debugging statement
            // std::cout << "Cylinder bottom cap hit at distance: " << tBottom << "\n";
        }
    }

    return hitAnything;
}

// Method to get the normal at a point on the cylinder
Vector3 Cylinder::getNormal(const Vector3 &point) const
{
    // Since cylinder is aligned along Y-axis
    Vector3 normal(point.x - center.x, 0.0f, point.z - center.z);
    return normal.normalise();
}

// Get the bounding box of the cylinder
BoundingBox Cylinder::getBoundingBox() const
{
    float halfHeight = height; // Treat 'height' as half-height to double the actual height
    Vector3 minPoint(center.x - radius, center.y - halfHeight, center.z - radius);
    Vector3 maxPoint(center.x + radius, center.y + halfHeight, center.z + radius);
    return BoundingBox(minPoint, maxPoint);
}

// Cylinder.cpp
#include "Cylinder.h"
#include <cmath>     // For sqrt, fabs, etc.
#include <limits>    // For numeric_limits
#include <algorithm> // For std::min and std::max
#include <iostream>  // For debugging

// Constructor implementation
Cylinder::Cylinder(const Vector3 &c, const Vector3 &a, float r, float h, const Material &m)
    : center(c), axis(a.normalise()), radius(r), height(h * 2.0f), material(m) // Double the height here
{
    if (h <= 0.0f)
    {
        std::cerr << "Warning: Cylinder height must be positive. Defaulting to height=1.0f.\n";
        height = 2.0f; // Default full height
    }
}

// Method to check ray-cylinder intersection and populate Intersection
bool Cylinder::intersect(const Ray &ray, Intersection &hit) const
{
    // Vector from cylinder center to ray origin
    Vector3 oc = ray.origin - center;

    // Project the ray direction and oc onto the plane perpendicular to the cylinder's axis
    float a = ray.direction.dot(ray.direction) - pow(ray.direction.dot(axis), 2);
    float b = 2.0f * (ray.direction.dot(oc) - ray.direction.dot(axis) * oc.dot(axis));
    float c = oc.dot(oc) - pow(oc.dot(axis), 2) - radius * radius;

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
                // Project the point onto the cylinder axis
                float projection = (point - center).dot(axis);
                // Check if the point is within the cylinder's height bounds
                if (projection >= -height * 0.5f && projection <= height * 0.5f)
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

        // Compute the normal at the hit point
        Vector3 projection = axis * (hit.point - center).dot(axis);
        hit.normal = (hit.point - center - projection).normalise();

        hit.material = material;
        hit.distance = tFinal;

        // Compute UV coordinates for the cylinder side
        // Compute theta based on a reference axis (e.g., Vector3(1,0,0))
        Vector3 refAxis(1.0f, 0.0f, 0.0f);
        Vector3 orthogonal = refAxis.cross(axis);
        if (orthogonal.lengthSquared() < 1e-6f) // If axis is parallel to refAxis
            orthogonal = Vector3(0.0f, 1.0f, 0.0f).cross(axis);
        orthogonal = orthogonal.normalise();
        Vector3 tangent = axis.cross(orthogonal).normalise();

        float theta = std::atan2(hit.normal.dot(tangent), hit.normal.dot(orthogonal));
        if (theta < 0.0f)
            theta += 2.0f * M_PI;

        float u = theta / (2.0f * M_PI); // Range [0,1]

        // v_coord based on projection along axis
        float v_coord = (hit.point - center).dot(axis) / height + 0.5f; // Range [0,1]

        hit.u = u;
        hit.v = v_coord;

        hitAnything = true;

        // Debugging Output
        // std::cout << "Cylinder Side Hit: t=" << tFinal
        //           << ", Point=(" << hit.point.x << ", " << hit.point.y << ", " << hit.point.z << ")"
        //           << ", Normal=(" << hit.normal.x << ", " << hit.normal.y << ", " << hit.normal.z << ")\n";
    }

    // Check for intersection with the caps only if the cylinder has finite height
    if (height > 0.0f)
    {
        // Define cap normals
        Vector3 topCenter = center + axis * (height * 0.5f);
        Vector3 bottomCenter = center - axis * (height * 0.5f);
        Vector3 topNormal = axis;
        Vector3 bottomNormal = -axis;

        // Top cap intersection
        float denomTop = axis.dot(ray.direction);
        if (std::fabs(denomTop) > 1e-6f)
        {
            float tTop = (topCenter - ray.origin).dot(axis) / denomTop;
            if (tTop > 1e-4f && tTop < tFinal)
            {
                Vector3 pTop = ray.origin + ray.direction * tTop;
                Vector3 v = pTop - topCenter;
                if (v.dot(v) <= radius * radius)
                {
                    tFinal = tTop;
                    hit.point = pTop;
                    hit.normal = topNormal;
                    hit.material = material;
                    hit.distance = tFinal;

                    // Compute UV coordinates for the top cap
                    // Project v onto orthogonal and tangent vectors
                    Vector3 orthogonal_ref = Vector3(1.0f, 0.0f, 0.0f).cross(axis);
                    if (orthogonal_ref.lengthSquared() < 1e-6f) // If axis is parallel to refAxis
                        orthogonal_ref = Vector3(0.0f, 1.0f, 0.0f).cross(axis);
                    orthogonal_ref = orthogonal_ref.normalise();
                    Vector3 tangent_ref = axis.cross(orthogonal_ref).normalise();

                    float u_cap = (v.dot(orthogonal_ref) / radius) * 0.5f + 0.5f;
                    float v_cap = (v.dot(tangent_ref) / radius) * 0.5f + 0.5f;

                    hit.u = u_cap;
                    hit.v = v_cap;

                    hitAnything = true;

                    // Debugging Output
                    // std::cout << "Cylinder Top Cap Hit: t=" << tFinal
                    //           << ", Point=(" << hit.point.x << ", " << hit.point.y << ", " << hit.point.z << ")"
                    //           << ", Normal=(" << hit.normal.x << ", " << hit.normal.y << ", " << hit.normal.z << ")\n";
                }
            }
        }

        // Bottom cap intersection
        float denomBottom = axis.dot(ray.direction);
        if (std::fabs(denomBottom) > 1e-6f)
        {
            float tBottom = (bottomCenter - ray.origin).dot(axis) / denomBottom;
            if (tBottom > 1e-4f && tBottom < tFinal)
            {
                Vector3 pBottom = ray.origin + ray.direction * tBottom;
                Vector3 v = pBottom - bottomCenter;
                if (v.dot(v) <= radius * radius)
                {
                    tFinal = tBottom;
                    hit.point = pBottom;
                    hit.normal = bottomNormal;
                    hit.material = material;
                    hit.distance = tFinal;

                    // Compute UV coordinates for the bottom cap
                    // Project v onto orthogonal and tangent vectors
                    Vector3 orthogonal_ref = Vector3(1.0f, 0.0f, 0.0f).cross(axis);
                    if (orthogonal_ref.lengthSquared() < 1e-6f) // If axis is parallel to refAxis
                        orthogonal_ref = Vector3(0.0f, 1.0f, 0.0f).cross(axis);
                    orthogonal_ref = orthogonal_ref.normalise();
                    Vector3 tangent_ref = axis.cross(orthogonal_ref).normalise();

                    float u_cap = (v.dot(orthogonal_ref) / radius) * 0.5f + 0.5f;
                    float v_cap = (v.dot(tangent_ref) / radius) * 0.5f + 0.5f;

                    hit.u = u_cap;
                    hit.v = v_cap;

                    hitAnything = true;

                    // Debugging Output
                    // std::cout << "Cylinder Bottom Cap Hit: t=" << tFinal
                    //           << ", Point=(" << hit.point.x << ", " << hit.point.y << ", " << hit.point.z << ")"
                    //           << ", Normal=(" << hit.normal.x << ", " << hit.normal.y << ", " << hit.normal.z << ")\n";
                }
            }
        }
    }

    return hitAnything;
}

// Method to get the normal at a point on the cylinder
Vector3 Cylinder::getNormal(const Vector3 &point) const
{
    // Compute the normal based on the cylinder's axis
    Vector3 projection = axis * (point - center).dot(axis);
    Vector3 normal = (point - center - projection).normalise();
    return normal;
}

// Get the bounding box of the cylinder
BoundingBox Cylinder::getBoundingBox() const
{
    // Compute the bounding box based on the cylinder's axis and height
    float halfHeight = height * 0.5f;
    Vector3 topCenter = center + axis * halfHeight;
    Vector3 bottomCenter = center - axis * halfHeight;

    // Determine the orthogonal vectors to the axis for bounding calculations
    Vector3 refAxis(1.0f, 0.0f, 0.0f);
    Vector3 orthogonal = refAxis.cross(axis);
    if (orthogonal.lengthSquared() < 1e-6f) // If axis is parallel to refAxis
        orthogonal = Vector3(0.0f, 1.0f, 0.0f).cross(axis);
    orthogonal = orthogonal.normalise();
    Vector3 tangent = axis.cross(orthogonal).normalise();

    // Points on the top and bottom circles
    Vector3 topPoints[4] = {
        topCenter + orthogonal * radius,
        topCenter - orthogonal * radius,
        topCenter + tangent * radius,
        topCenter - tangent * radius};

    Vector3 bottomPoints[4] = {
        bottomCenter + orthogonal * radius,
        bottomCenter - orthogonal * radius,
        bottomCenter + tangent * radius,
        bottomCenter - tangent * radius};

    // Initialize min and max points
    Vector3 minPt = topPoints[0];
    Vector3 maxPt = topPoints[0];

    for (int i = 0; i < 4; ++i)
    {
        minPt = Vector3::min(minPt, topPoints[i]);
        maxPt = Vector3::max(maxPt, topPoints[i]);
        minPt = Vector3::min(minPt, bottomPoints[i]);
        maxPt = Vector3::max(maxPt, bottomPoints[i]);
    }

    BoundingBox box(minPt, maxPt);

    return box;
}

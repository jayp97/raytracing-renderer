// Intersection.h
#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "Vector3.h"
#include "Material.h" // Include Material

struct Intersection
{
    Vector3 point;     // Intersection point
    Vector3 normal;    // Normal at the intersection point
    Material material; // Material of the intersected object
    float distance;    // Distance from the ray origin to the intersection point

    float u; // U coordinate for texture mapping
    float v; // V coordinate for texture mapping
};

#endif // INTERSECTION_H

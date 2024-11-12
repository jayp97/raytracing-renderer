#ifndef Intersection_H
#define Intersection_H

#include "Vector3.h"
#include "Material.h" // Include Material

struct Intersection
{
    Vector3 point;     // Intersection point
    Vector3 normal;    // Normal at the intersection point
    Material material; // Material of the intersected object
    float distance;    // Distance from the ray origin to the intersection point
};

#endif // Intersection_H
#ifndef OBJECT_H
#define OBJECT_H

#include "Ray.h"
#include "Intersection.h"

class Object
{
public:
    virtual ~Object() {}

    // Pure virtual intersect method to be implemented by derived classes
    virtual bool intersect(const Ray &ray, Intersection &hit) const = 0;
};

#endif // OBJECT_H
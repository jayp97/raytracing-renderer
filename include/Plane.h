#ifndef PLANE_H
#define PLANE_H

#include "Object.h"
#include "Vector3.h"
#include "Material.h"

class Plane : public Object
{
public:
    Plane(const Vector3 &point, const Vector3 &normal, const Material &material)
        : point(point), normal(normal.normalise()), material(material) {}

    bool intersect(const Ray &ray, Intersection &hit) const override;

private:
    Vector3 point;  // A point on the plane
    Vector3 normal; // Normal to the plane
    Material material;
};

#endif // PLANE_H

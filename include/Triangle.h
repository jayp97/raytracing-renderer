// Triangle.h
#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Vector3.h"
#include "Ray.h"
#include "Material.h"
#include "Intersection.h"
#include "Object.h"
#include "BoundingBox.h"

class Triangle : public Object
{
public:
    // Constructor with material and optional UV coordinates
    Triangle(const Vector3 &vertex0, const Vector3 &vertex1, const Vector3 &vertex2, const Material &m,
             const Vector3 &uv0 = Vector3(0, 0, 0), const Vector3 &uv1 = Vector3(1, 0, 0), const Vector3 &uv2 = Vector3(0, 1, 0))
        : v0(vertex0), v1(vertex1), v2(vertex2), uv0(uv0), uv1(uv1), uv2(uv2), material(m) {}

    bool intersect(const Ray &ray, Intersection &hit) const override;

    // Triangle vertices
    Vector3 v0, v1, v2;

    // UV coordinates at each vertex
    Vector3 uv0, uv1, uv2;

    Material material;

    // Get the bounding box of the triangle
    BoundingBox getBoundingBox() const override;
};

#endif // TRIANGLE_H

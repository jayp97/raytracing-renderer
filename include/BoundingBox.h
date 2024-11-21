// BoundingBox.h
#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include "Vector3.h"
#include "Ray.h"

class BoundingBox
{
public:
    BoundingBox();
    BoundingBox(const Vector3 &min, const Vector3 &max);

    // Check if the ray intersects the bounding box
    bool intersect(const Ray &ray, float t_min, float t_max) const;

    // Expand the bounding box to include another bounding box
    void expand(const BoundingBox &box);

    // Expand the bounding box to include a point
    void expand(const Vector3 &point);

    // Getters
    Vector3 getMin() const { return minPoint; }
    Vector3 getMax() const { return maxPoint; }

private:
    Vector3 minPoint;
    Vector3 maxPoint;
};

#endif // BOUNDING_BOX_H

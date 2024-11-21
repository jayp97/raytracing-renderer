// BoundingBox.cpp
#include "BoundingBox.h"
#include <limits>
#include <algorithm>

BoundingBox::BoundingBox()
    : minPoint(Vector3(std::numeric_limits<float>::max(),
                       std::numeric_limits<float>::max(),
                       std::numeric_limits<float>::max())),
      maxPoint(Vector3(std::numeric_limits<float>::lowest(),
                       std::numeric_limits<float>::lowest(),
                       std::numeric_limits<float>::lowest())) {}

BoundingBox::BoundingBox(const Vector3 &min, const Vector3 &max)
    : minPoint(min), maxPoint(max) {}

bool BoundingBox::intersect(const Ray &ray, float t_min, float t_max) const
{
    for (int i = 0; i < 3; ++i)
    {
        float invD = 1.0f / ray.direction[i];
        float t0 = (minPoint[i] - ray.origin[i]) * invD;
        float t1 = (maxPoint[i] - ray.origin[i]) * invD;
        if (invD < 0.0f)
            std::swap(t0, t1);
        t_min = t0 > t_min ? t0 : t_min;
        t_max = t1 < t_max ? t1 : t_max;
        if (t_max <= t_min)
            return false;
    }
    return true;
}

void BoundingBox::expand(const BoundingBox &box)
{
    minPoint = Vector3::min(minPoint, box.minPoint);
    maxPoint = Vector3::max(maxPoint, box.maxPoint);
}

void BoundingBox::expand(const Vector3 &point)
{
    minPoint = Vector3::min(minPoint, point);
    maxPoint = Vector3::max(maxPoint, point);
}

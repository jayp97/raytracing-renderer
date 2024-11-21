#ifndef BLINN_PHONG_SHADER_H
#define BLINN_PHONG_SHADER_H

#include "Scene.h"
#include "Intersection.h"
#include "Color.h"

class BlinnPhongShader
{
public:
    BlinnPhongShader(const Scene &scene, const Vector3 &cameraPos);

    Color shade(const Intersection &hit) const;

private:
    const Scene &scene;
    Vector3 cameraPosition;

    // Check if a point is in shadow
    bool isInShadow(const Vector3 &point, const Vector3 &lightDir, float lightDistance) const;
};

#endif // BLINN_PHONG_SHADER_H

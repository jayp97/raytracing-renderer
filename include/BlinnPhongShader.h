// BlinnPhongShader.h
#ifndef BLINNPHONGSHADER_H
#define BLINNPHONGSHADER_H

#include "Scene.h"
#include "Intersection.h"
#include "Vector3.h"
#include "Color.h"

class BlinnPhongShader
{
public:
    // Constructor takes the scene and camera position for shading computations
    BlinnPhongShader(const Scene &scene, const Vector3 &cameraPos);

    // Shade method computes the color at the intersection point
    Color shade(const Intersection &hit) const;

private:
    const Scene &scene;
    Vector3 cameraPosition;
};

#endif // BLINNPHONGSHADER_H

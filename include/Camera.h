#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3.h"
#include "Ray.h"

class Camera
{
public:
    Vector3 position;
    Vector3 forward;
    Vector3 up;
    Vector3 right;
    float fov;
    float aspectRatio;
    float exposure;

    // Constructor
    Camera(const Vector3 &position, const Vector3 &lookAt, const Vector3 &up, float fov, int width, int height, float exposure);

    // Method to generate a ray for a given pixel
    Ray generateRay(float pixelX, float pixelY, int imageWidth, int imageHeight) const;
};

#endif

// Camera.h
#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3.h"
#include "Ray.h"

class Camera
{
public:
    // Default constructor
    Camera()
        : position(0.0f, 0.0f, 0.0f),
          lookAt(0.0f, 0.0f, -1.0f),
          forward(0.0f, 0.0f, -1.0f),
          up(0.0f, 1.0f, 0.0f),
          right(1.0f, 0.0f, 0.0f),
          fov(45.0f),
          aspectRatio(1.0f),
          exposure(1.0f),
          width(800),
          height(600),
          aperture(0.0f),
          focalDistance(1.0f)
    {
    }

    // Constructor with parameters
    Camera(const Vector3 &position, const Vector3 &lookAt, const Vector3 &up, float fov,
           int width, int height, float exposure, float aperture = 0.0f, float focalDistance = 1.0f);

    // Generate a ray from the camera through the viewport at the specified pixel coordinates
    Ray generateRay(float pixelX, float pixelY, float r1 = 0.0f, float r2 = 0.0f) const;

    // Method to update resolution and recompute directional vectors
    void setResolution(int newWidth, int newHeight)
    {
        width = newWidth;
        height = newHeight;
        aspectRatio = static_cast<float>(width) / height;
        right = forward.cross(up).normalise();
        this->up = right.cross(forward).normalise();
    }

    // Camera attributes
    Vector3 position;
    Vector3 lookAt; // Added lookAt member
    Vector3 forward, up, right;
    float fov;
    float aspectRatio;
    float exposure;
    int width;  // Image width
    int height; // Image height

    // New attributes for depth of field
    float aperture;      // Aperture size (diameter)
    float focalDistance; // Distance from the camera to the focus plane

private:
    // Helper function for concentric disk sampling
    static void concentricSampleDisk(float u1, float u2, float &dx, float &dy);
};

#endif // CAMERA_H

// Camera.cpp
#include "Camera.h"
#include <cmath>
#include <iostream>

Camera::Camera(const Vector3 &position, const Vector3 &lookAt, const Vector3 &up, float fov,
               int width, int height, float exposure, float aperture, float focalDistance)
    : position(position), lookAt(lookAt), exposure(exposure), width(width), height(height),
      aperture(aperture), focalDistance(focalDistance)
{
    // Calculate aspect ratio from width and height
    aspectRatio = static_cast<float>(width) / height;

    // Calculate the forward, right, and up directions based on lookAt and up
    forward = (lookAt - position).normalise();
    right = forward.cross(up).normalise();
    this->up = right.cross(forward).normalise();

    // Convert the field of view to radians
    this->fov = (fov * M_PI) / 180.0f;
}

// Generate a ray from the camera through the viewport at the specified pixel coordinates
Ray Camera::generateRay(float pixelX, float pixelY, float r1, float r2) const
{
    // Calculate the viewport scaling based on the FOV
    float scale = std::tan(fov / 2.0f);

    // Normalize pixel coordinates to the range [-1, 1] for viewport space
    float x = (-2 * (pixelX + 0.5f) / width + 1) * aspectRatio * scale;
    float y = (1 - 2 * (pixelY + 0.5f) / height) * scale;

    // Calculate the direction of the ray through the pixel
    Vector3 direction = (forward + right * x + up * y).normalise();

    if (aperture > 0.0f)
    {
        // Compute the focus point at the focal distance along the ray direction
        Vector3 focusPoint = position + direction * focalDistance;

        // Sample a point on the lens (disk) centered at the camera position
        float lensRadius = aperture / 2.0f;

        // Map random numbers to disk using concentric disk sampling
        float lensU, lensV;
        concentricSampleDisk(r1, r2, lensU, lensV);
        lensU *= lensRadius;
        lensV *= lensRadius;

        // Compute the offset lens point
        Vector3 lensPoint = position + right * lensU + up * lensV;

        // Generate new ray direction from lens point to focus point
        Vector3 newDirection = (focusPoint - lensPoint).normalise();

        return Ray(lensPoint, newDirection);
    }
    else
    {
        // Generate ray as before
        return Ray(position, direction);
    }
}

// Helper function for concentric disk sampling
void Camera::concentricSampleDisk(float u1, float u2, float &dx, float &dy)
{
    float r, theta;
    float sx = 2 * u1 - 1;
    float sy = 2 * u2 - 1;

    if (sx == 0.0f && sy == 0.0f)
    {
        dx = 0.0f;
        dy = 0.0f;
        return;
    }

    if (std::abs(sx) > std::abs(sy))
    {
        r = sx;
        theta = (M_PI / 4.0f) * (sy / sx);
    }
    else
    {
        r = sy;
        theta = (M_PI / 2.0f) - (M_PI / 4.0f) * (sx / sy);
    }

    dx = r * std::cos(theta);
    dy = r * std::sin(theta);
}

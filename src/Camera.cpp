#include "Camera.h"
#include <cmath>

Camera::Camera(const Vector3 &position, const Vector3 &lookAt, const Vector3 &up, float fov, float aspectRatio)
    : position(position), aspectRatio(aspectRatio)
{
    // Calculate the forward, right, and up directions based on lookAt and up
    forward = (lookAt - position).normalise();
    right = forward.cross(up).normalise();
    this->up = right.cross(forward).normalise();

    // Convert the field of view to radians and store it
    this->fov = (fov * M_PI) / 180.0f;
}

// Generate a ray from the camera through the viewport at the specified pixel coordinates
Ray Camera::generateRay(float pixelX, float pixelY, int imageWidth, int imageHeight) const
{
    // Calculate the viewport scaling based on the FOV
    float scale = std::tan(fov / 2.0f);

    // Normalize pixel coordinates to the range [-1, 1] for viewport space
    float x = (2 * (pixelX + 0.5f) / imageWidth - 1) * aspectRatio * scale;
    float y = (1 - 2 * (pixelY + 0.5f) / imageHeight) * scale;

    // Calculate the direction of the ray through the pixel
    Vector3 direction = (forward + right * x + up * y).normalise();

    return Ray(position, direction);
}

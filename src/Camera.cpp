#include "Camera.h"
#include <cmath>
#include <iostream>

Camera::Camera(const Vector3 &position, const Vector3 &lookAt, const Vector3 &up, float fov, int width, int height, float exposure)
    : position(position), lookAt(lookAt), exposure(exposure), width(width), height(height)
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
Ray Camera::generateRay(float pixelX, float pixelY) const
{
    // Calculate the viewport scaling based on the FOV
    float scale = std::tan(fov / 2.0f);

    // Normalize pixel coordinates to the range [-1, 1] for viewport space
    float x = (-2 * (pixelX + 0.5f) / width + 1) * aspectRatio * scale;
    float y = (1 - 2 * (pixelY + 0.5f) / height) * scale;

    // Calculate the direction of the ray through the pixel
    Vector3 direction = (forward + right * x + up * y).normalise();

    Ray ray(position, direction);
    // std::cout << "Generated Ray Origin: (" << ray.origin.x << ", " << ray.origin.y << ", " << ray.origin.z << ") "
    //           << "Direction: (" << ray.direction.x << ", " << ray.direction.y << ", " << ray.direction.z << ")\n";
    return ray;
}

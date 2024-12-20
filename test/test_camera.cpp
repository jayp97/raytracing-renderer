// test_camera.cpp
#include "catch_amalgamated.hpp"
#include "../include/Camera.h"
#include "../include/Ray.h"
#include "../include/Vector3.h"

const float tolerance = 1e-5;

// Helper function to compare two vectors with a tolerance
bool areVectorsEqual(const Vector3 &v1, const Vector3 &v2, float tolerance)
{
    return std::fabs(v1.x - v2.x) < tolerance &&
           std::fabs(v1.y - v2.y) < tolerance &&
           std::fabs(v1.z - v2.z) < tolerance;
}

TEST_CASE("Camera Initialization", "[CAMERA]")
{
    Vector3 position(0, 0, 0);
    Vector3 lookAt(0, 0, -1);
    Vector3 up(0, 1, 0);
    float fov = 90.0f;
    int width = 1200;
    int height = 800;
    float exposure = 0.1f;

    Camera camera(position, lookAt, up, fov, width, height, exposure);

    REQUIRE(camera.position == position);
    REQUIRE(areVectorsEqual(camera.forward, (lookAt - position).normalise(), tolerance));
    REQUIRE(areVectorsEqual(camera.right, camera.forward.cross(up).normalise(), tolerance));
    REQUIRE(areVectorsEqual(camera.up, camera.right.cross(camera.forward).normalise(), tolerance));
    REQUIRE(camera.exposure == exposure);
}

TEST_CASE("Camera generateRay() Center Pixel", "[CAMERA]")
{
    Vector3 position(0, 0, 0);
    Vector3 lookAt(0, 0, -1);
    Vector3 up(0, 1, 0);
    float fov = 90.0f;
    int width = 3;
    int height = 3;
    float exposure = 0.1f;

    Camera camera(position, lookAt, up, fov, width, height, exposure);

    // Center pixel in an odd-sized image
    int centerX = width / 2;  // For width=3, centerX=1
    int centerY = height / 2; // For height=3, centerY=1

    // Correct generateRay call with 2 parameters
    Ray ray = camera.generateRay(static_cast<float>(centerX), static_cast<float>(centerY));

    REQUIRE(ray.origin == position);
    REQUIRE(areVectorsEqual(ray.direction, camera.forward, tolerance));
}

TEST_CASE("Camera generateRay() Corner Pixel", "[CAMERA]")
{
    Vector3 position(0, 0, 0);
    Vector3 lookAt(0, 0, -1);
    Vector3 up(0, 1, 0);
    float fov = 90.0f;
    int width = 4;
    int height = 4;
    float exposure = 0.1f;

    Camera camera(position, lookAt, up, fov, width, height, exposure);

    // Generate ray through the top-left corner pixel (0, 0)
    int pixelX = 0;
    int pixelY = 0;

    // Correct generateRay call with 2 parameters
    Ray ray = camera.generateRay(static_cast<float>(pixelX), static_cast<float>(pixelY));

    // Calculate expected x and y using the same formula as generateRay()
    float fovRadians = (fov * M_PI) / 180.0f;  // Convert FOV to radians
    float scale = std::tan(fovRadians / 2.0f); // Calculate the scaling factor
    float aspectRatio = static_cast<float>(width) / height;

    float x = (2 * (pixelX + 0.5f) / width - 1) * aspectRatio * scale;
    float y = (1 - 2 * (pixelY + 0.5f) / height) * scale;

    // Expected direction based on calculated x and y
    Vector3 expectedDirection = (camera.forward + camera.right * x + camera.up * y).normalise();

    REQUIRE(ray.origin == position);
    REQUIRE(areVectorsEqual(ray.direction, expectedDirection, tolerance));
}

#include "catch_amalgamated.hpp"
#include "../include/SceneLoader.h"
#include "../include/Camera.h"
#include "../include/Vector3.h"
#include "../include/Sphere.h"
#include "../include/Cylinder.h"
#include "../include/Triangle.h"

const float tolerance = 1e-5;

bool areVectorsEqual(const Vector3 &v1, const Vector3 &v2, float tolerance)
{
    return std::fabs(v1.x - v2.x) < tolerance &&
           std::fabs(v1.y - v2.y) < tolerance &&
           std::fabs(v1.z - v2.z) < tolerance;
}

bool areApproximatelyEqual(float a, float b, float tolerance)
{
    return std::fabs(a - b) < tolerance;
}

TEST_CASE("SceneLoader loads Camera correctly from binary_primitives.json", "[SCENELOADER][CAMERA]")
{
    SceneLoader loader;
    REQUIRE(loader.loadScene("scenes/binary_primitives.json") == true);

    Camera camera = loader.getCamera();
    Vector3 expectedPosition(0.0f, 0.0f, 0.0f);
    Vector3 expectedLookAt(0.0f, 0.0f, 1.0f);
    Vector3 expectedUp(0.0f, 1.0f, 0.0f);
    float expectedFov = 45.0f;
    int expectedWidth = 1200;
    int expectedHeight = 800;
    float expectedExposure = 0.1f;

    REQUIRE(camera.position == expectedPosition);
    REQUIRE(areVectorsEqual(camera.forward, (expectedLookAt - expectedPosition).normalise(), tolerance));
    REQUIRE(areVectorsEqual(camera.right, camera.forward.cross(expectedUp).normalise(), tolerance));
    REQUIRE(areVectorsEqual(camera.up, camera.right.cross(camera.forward).normalise(), tolerance));
    REQUIRE(camera.exposure == expectedExposure);

    REQUIRE(areApproximatelyEqual(camera.aspectRatio, static_cast<float>(expectedWidth) / expectedHeight, tolerance));
}

TEST_CASE("SceneLoader loads Sphere correctly from binary_primitives.json", "[SCENELOADER][SPHERE]")
{
    SceneLoader loader;
    REQUIRE(loader.loadScene("scenes/binary_primitives.json") == true);

    const auto &spheres = loader.getSpheres();
    REQUIRE(spheres.size() == 1);

    Sphere sphere = spheres[0];
    Vector3 expectedCenter(-0.3f, 0.19f, 1.0f);
    float expectedRadius = 0.2f;

    REQUIRE(areVectorsEqual(sphere.center, expectedCenter, tolerance));
    REQUIRE(sphere.radius == expectedRadius);
}

TEST_CASE("SceneLoader loads Cylinder correctly from binary_primitives.json", "[SCENELOADER][CYLINDER]")
{
    SceneLoader loader;
    REQUIRE(loader.loadScene("scenes/binary_primitives.json") == true);

    const auto &cylinders = loader.getCylinders();
    REQUIRE(cylinders.size() == 1);

    Cylinder cylinder = cylinders[0];
    Vector3 expectedCenter(-0.3f, -0.2f, 1.0f);
    Vector3 expectedAxis(1.0f, 0.0f, 0.0f);
    float expectedRadius = 0.15f;
    float expectedHeight = 0.2f;

    REQUIRE(areVectorsEqual(cylinder.center, expectedCenter, tolerance));
    REQUIRE(areVectorsEqual(cylinder.axis, expectedAxis, tolerance));
    REQUIRE(cylinder.radius == expectedRadius);
    REQUIRE(cylinder.height == expectedHeight);
}

TEST_CASE("SceneLoader loads Triangle correctly from binary_primitives.json", "[SCENELOADER][TRIANGLE]")
{
    SceneLoader loader;
    REQUIRE(loader.loadScene("scenes/binary_primitives.json") == true);

    const auto &triangles = loader.getTriangles();
    REQUIRE(triangles.size() == 1);

    Triangle triangle = triangles[0];
    Vector3 expectedV0(0.0f, 0.0f, 1.0f);
    Vector3 expectedV1(0.5f, 0.0f, 1.0f);
    Vector3 expectedV2(0.25f, 0.25f, 1.0f);

    REQUIRE(areVectorsEqual(triangle.v0, expectedV0, tolerance));
    REQUIRE(areVectorsEqual(triangle.v1, expectedV1, tolerance));
    REQUIRE(areVectorsEqual(triangle.v2, expectedV2, tolerance));
}

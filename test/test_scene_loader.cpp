#include "catch_amalgamated.hpp"
#include "../include/SceneLoader.h"
#include "../include/Scene.h"
#include "../include/Sphere.h"
#include "../include/Cylinder.h"
#include "../include/Triangle.h"
#include "../include/Vector3.h"
#include <memory>

const float tolerance = 1e-5;

bool areVectorsEqual(const Vector3 &v1, const Vector3 &v2, float tolerance)
{
    return std::fabs(v1.x - v2.x) < tolerance &&
           std::fabs(v1.y - v2.y) < tolerance &&
           std::fabs(v1.z - v2.z) < tolerance;
}

TEST_CASE("SceneLoader loads Camera correctly from binary_primitives.json", "[SCENELOADER][CAMERA]")
{
    Scene scene;
    SceneLoader loader;
    REQUIRE(loader.loadScene("scenes/binary_primitives.json", scene) == true);

    Camera camera = scene.camera;
    Vector3 expectedPosition(0.0f, 0.0f, 0.0f);
    Vector3 expectedLookAt(0.0f, 0.0f, 1.0f);
    Vector3 expectedUp(0.0f, 1.0f, 0.0f);
    float expectedFov = 45.0f * M_PI / 180.0f; // Convert to radians
    float expectedAspectRatio = 1200.0f / 800.0f;

    REQUIRE(camera.position == expectedPosition);
    REQUIRE(areVectorsEqual(camera.forward, (expectedLookAt - expectedPosition).normalise(), tolerance));
    REQUIRE(areVectorsEqual(camera.up, camera.right.cross(camera.forward).normalise(), tolerance));
    REQUIRE(camera.fov == Catch::Approx(expectedFov).epsilon(tolerance));
    REQUIRE(camera.aspectRatio == Catch::Approx(expectedAspectRatio).epsilon(tolerance));
}

TEST_CASE("SceneLoader loads Sphere correctly from binary_primitives.json", "[SCENELOADER][SPHERE]")
{
    Scene scene;
    SceneLoader loader;
    REQUIRE(loader.loadScene("scenes/binary_primitives.json", scene) == true);

    bool foundSphere = false;

    for (const auto &object : scene.objects)
    {
        auto sphere = std::dynamic_pointer_cast<Sphere>(object);
        if (sphere)
        {
            foundSphere = true;

            Vector3 expectedCenter(-0.3f, 0.19f, 1.0f);
            float expectedRadius = 0.2f;

            REQUIRE(areVectorsEqual(sphere->center, expectedCenter, tolerance));
            REQUIRE(sphere->radius == Catch::Approx(expectedRadius).epsilon(tolerance));
        }
    }

    REQUIRE(foundSphere == true);
}

TEST_CASE("SceneLoader loads Cylinder correctly from binary_primitives.json", "[SCENELOADER][CYLINDER]")
{
    Scene scene;
    SceneLoader loader;
    REQUIRE(loader.loadScene("scenes/binary_primitives.json", scene) == true);

    bool foundCylinder = false;

    for (const auto &object : scene.objects)
    {
        auto cylinder = std::dynamic_pointer_cast<Cylinder>(object);
        if (cylinder)
        {
            foundCylinder = true;

            Vector3 expectedCenter(-0.3f, -0.2f, 1.0f);
            Vector3 expectedAxis(1.0f, 0.0f, 0.0f);
            float expectedRadius = 0.15f;
            float expectedHeight = 0.2f;

            REQUIRE(areVectorsEqual(cylinder->getCenter(), expectedCenter, tolerance));
            REQUIRE(areVectorsEqual(cylinder->getAxis(), expectedAxis, tolerance));
            REQUIRE(cylinder->getRadius() == Catch::Approx(expectedRadius).epsilon(tolerance));
            REQUIRE(cylinder->getHeight() == Catch::Approx(expectedHeight).epsilon(tolerance));
        }
    }

    REQUIRE(foundCylinder == true);
}

TEST_CASE("SceneLoader loads Triangle correctly from binary_primitives.json", "[SCENELOADER][TRIANGLE]")
{
    Scene scene;
    SceneLoader loader;
    REQUIRE(loader.loadScene("scenes/binary_primitives.json", scene) == true);

    bool foundTriangle = false;

    for (const auto &object : scene.objects)
    {
        auto triangle = std::dynamic_pointer_cast<Triangle>(object);
        if (triangle)
        {
            foundTriangle = true;

            Vector3 expectedV0(0.0f, 0.0f, 1.0f);
            Vector3 expectedV1(0.5f, 0.0f, 1.0f);
            Vector3 expectedV2(0.25f, 0.25f, 1.0f);

            REQUIRE(areVectorsEqual(triangle->v0, expectedV0, tolerance));
            REQUIRE(areVectorsEqual(triangle->v1, expectedV1, tolerance));
            REQUIRE(areVectorsEqual(triangle->v2, expectedV2, tolerance));
        }
    }

    REQUIRE(foundTriangle == true);
}
#include "catch_amalgamated.hpp"
#include "../include/Scene.h"
#include "../include/Sphere.h"
#include "../include/Plane.h"
#include "../include/Light.h"
#include "../include/Raytracer.h"
#include "../include/BlinnPhongShader.h"
#include "../include/Intersection.h"

TEST_CASE("Shadows are correctly computed", "[SHADOW]")
{
    // Create the scene
    Scene scene;

    // Set up camera
    scene.camera = Camera(
        Vector3(0, 5, 10),
        Vector3(0, 0, 0),
        Vector3(0, 1, 0),
        45.0f,
        800,
        600,
        1.0f);

    // Add light
    Light light(Vector3(5, 10, 5), Color(1, 1, 1));
    scene.addLight(light);

    // Add a sphere to cast a shadow
    Material sphereMaterial;
    sphereMaterial.diffuseColor = Color(1, 0, 0);
    sphereMaterial.kd = 0.8f;
    sphereMaterial.ks = 0.2f;
    sphereMaterial.specularExponent = 50;
    auto sphere = std::make_shared<Sphere>(Vector3(0, 0, 0), 2.0f, sphereMaterial);
    scene.addObject(sphere);

    // Add a plane to receive the shadow
    Material planeMaterial;
    planeMaterial.diffuseColor = Color(0.5f, 0.5f, 0.5f);
    planeMaterial.kd = 0.8f;
    auto plane = std::make_shared<Plane>(Vector3(0, -2, 0), Vector3(0, 1, 0), planeMaterial);
    scene.addObject(plane);

    // Render the scene
    Raytracer raytracer;
    raytracer.render(scene, "test_shadow.ppm");

    // Verify shading on plane
    Intersection hit;
    Ray testRay(Vector3(0, -1.9, 0), Vector3(0, 1, 0)); // Ray toward the plane under the sphere

    REQUIRE(scene.intersect(testRay, hit) == true);

    BlinnPhongShader shader(scene, scene.camera.position);
    Color shadedColor = shader.shade(hit);

    std::cout << "Shaded color: " << shadedColor << "\n";

    // Verify shadow contributions
    REQUIRE(shadedColor.r < 0.5f);
    REQUIRE(shadedColor.g < 0.5f);
    REQUIRE(shadedColor.b < 0.5f);
}

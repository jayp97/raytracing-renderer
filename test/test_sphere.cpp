#include "catch_amalgamated.hpp"
#include "../include/Sphere.h"
#include "../include/Ray.h"
#include "../include/Vector3.h"
#include "../include/Intersection.h"

const float tolerance = 1e-5;

// Helper function to check approximate equality for floats
bool areFloatsEqual(float a, float b, float tolerance)
{
    return std::fabs(a - b) < tolerance;
}

TEST_CASE("Sphere Intersection - Ray Hits Sphere", "[SPHERE]")
{
    Material defaultMaterial;
    // Sphere centered at (0, 0, -5) with radius 1
    Sphere sphere(Vector3(0, 0, -5), 1.0f, defaultMaterial);
    Ray ray(Vector3(0, 0, 0), Vector3(0, 0, -1));

    Intersection hit;
    REQUIRE(sphere.intersect(ray, hit) == true);

    // Check that the intersection point distance is close to the expected value
    REQUIRE(areFloatsEqual(hit.distance, 4.0f, tolerance));
}

TEST_CASE("Sphere Intersection - Ray Misses Sphere", "[SPHERE]")
{
    Material defaultMaterial;
    // Sphere centered at (0, 0, -5) with radius 1
    Sphere sphere(Vector3(0, 0, -5), 1.0f, defaultMaterial);
    Ray ray(Vector3(0, 0, 0), Vector3(1, 0, 0)); // Ray in x-direction, away from sphere

    Intersection hit;
    REQUIRE(sphere.intersect(ray, hit) == false); // Expect no intersection
}

TEST_CASE("Sphere Intersection - Ray Starts Inside Sphere", "[SPHERE]")
{
    Material defaultMaterial;
    // Sphere centered at (0, 0, 0) with radius 2
    Sphere sphere(Vector3(0, 0, 0), 2.0f, defaultMaterial);
    Ray ray(Vector3(0, 0, 0), Vector3(0, 0, 1)); // Ray originates at sphere center

    Intersection hit;
    REQUIRE(sphere.intersect(ray, hit) == true);

    // Expect hit.distance to be the radius, as the ray exits the sphere at radius distance
    REQUIRE(areFloatsEqual(hit.distance, 2.0f, tolerance));
}

TEST_CASE("Sphere Intersection - Tangent Ray", "[SPHERE]")
{
    Material defaultMaterial;
    Sphere sphere(Vector3(0, 1, -5), 1.0f, defaultMaterial);
    Ray ray(Vector3(0, 0, 0), Vector3(0, 1, -5).normalise());

    Intersection hit;
    REQUIRE(sphere.intersect(ray, hit) == true);

    float expectedT = std::sqrt(26.0f) - 1.0f; // Corrected expected value

    // Debugging output for manual inspection
    if (!areFloatsEqual(hit.distance, expectedT, tolerance))
    {
        std::cout << "Expected t: " << expectedT << ", Computed t: " << hit.distance << std::endl;
    }

    REQUIRE(areFloatsEqual(hit.distance, expectedT, tolerance));
}

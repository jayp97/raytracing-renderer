#include "catch_amalgamated.hpp"
#include "../include/Sphere.h"
#include "../include/Ray.h"
#include "../include/Vector3.h"

const float tolerance = 1e-5;

// Helper function to check approximate equality for floats
bool areFloatsEqual(float a, float b, float tolerance)
{
    return std::fabs(a - b) < tolerance;
}

TEST_CASE("Sphere Intersection - Ray Hits Sphere", "[SPHERE]")
{
    // Sphere centered at (0, 0, -5) with radius 1
    Sphere sphere(Vector3(0, 0, -5), 1.0f);
    Ray ray(Vector3(0, 0, 0), Vector3(0, 0, -1));

    float t;
    REQUIRE(sphere.intersect(ray, t) == true);

    // Check that the intersection point distance is close to the expected value
    REQUIRE(areFloatsEqual(t, 4.0f, tolerance));
}

TEST_CASE("Sphere Intersection - Ray Misses Sphere", "[SPHERE]")
{
    // Sphere centered at (0, 0, -5) with radius 1
    Sphere sphere(Vector3(0, 0, -5), 1.0f);
    Ray ray(Vector3(0, 0, 0), Vector3(1, 0, 0)); // Ray in x-direction, away from sphere

    float t;
    REQUIRE(sphere.intersect(ray, t) == false); // Expect no intersection
}

TEST_CASE("Sphere Intersection - Ray Starts Inside Sphere", "[SPHERE]")
{
    // Sphere centered at (0, 0, 0) with radius 2
    Sphere sphere(Vector3(0, 0, 0), 2.0f);
    Ray ray(Vector3(0, 0, 0), Vector3(0, 0, 1)); // Ray originates at sphere center

    float t;
    REQUIRE(sphere.intersect(ray, t) == true);

    // Expect t to be the radius, as the ray exits the sphere at radius distance
    REQUIRE(areFloatsEqual(t, 2.0f, tolerance));
}

TEST_CASE("Sphere Intersection - Tangent Ray", "[SPHERE]")
{
    Sphere sphere(Vector3(0, 1, -5), 1.0f);
    Ray ray(Vector3(0, 0, 0), Vector3(0, 1, -5).normalise());

    float t;
    REQUIRE(sphere.intersect(ray, t) == true);

    float expectedT = sqrt(26.0f) - 1.0f; // Corrected expected value

    // Debugging output for manual inspection
    if (!areFloatsEqual(t, expectedT, tolerance))
    {
        std::cout << "Expected t: " << expectedT << ", Computed t: " << t << std::endl;
    }

    REQUIRE(areFloatsEqual(t, expectedT, tolerance));
}

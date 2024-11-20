#include "catch_amalgamated.hpp"
#include "../include/Triangle.h"
#include "../include/Ray.h"
#include "../include/Vector3.h"
#include "../include/Intersection.h"

const float tolerance = 1e-5;

TEST_CASE("Triangle Intersection - Ray Hits Triangle", "[TRIANGLE]")
{
    Material defaultMaterial;
    // Define a triangle in the X-Y plane
    Triangle triangle(Vector3(0, 0, 0), Vector3(1, 0, 0), Vector3(0, 1, 0), defaultMaterial);
    Ray ray(Vector3(0.25f, 0.25f, -1), Vector3(0, 0, 1)); // Ray pointing towards the triangle

    Intersection hit;
    REQUIRE(triangle.intersect(ray, hit) == true);
    REQUIRE(hit.distance > 0); // Ensure the intersection distance is valid
}

TEST_CASE("Triangle Intersection - Ray Misses Triangle", "[TRIANGLE]")
{
    Material defaultMaterial;
    // Define a triangle in the X-Y plane
    Triangle triangle(Vector3(0, 0, 0), Vector3(1, 0, 0), Vector3(0, 1, 0), defaultMaterial);
    Ray ray(Vector3(-1, -1, -1), Vector3(0, 0, 1)); // Ray pointing away from the triangle

    Intersection hit;
    REQUIRE(triangle.intersect(ray, hit) == false); // Expect no intersection
}

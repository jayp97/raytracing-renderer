#include "catch_amalgamated.hpp"
#include "../include/Ray.h"
#include "../include/Vector3.h"

TEST_CASE("Ray Initialisation", "[RAY]")
{

    Vector3 origin(0, 0, 0);
    Vector3 direction(1, 2, 3);
    Ray ray(origin, direction);

    REQUIRE(ray.origin == origin);
    REQUIRE(ray.direction == direction.normalise());
}

TEST_CASE("Ray at() Method", "[RAY]")
{
    {
        Vector3 origin(0, 0, 0);
        Vector3 direction(1, 2, 3);
        Ray ray(origin, direction);

        float t = 2.0f;
        Vector3 expected_point = origin + direction.normalise() * t;

        // Test if the at function correctly computes points along the ray
        REQUIRE(ray.at(t) == expected_point);
    }
}

TEST_CASE("Ray Direction Normalisation", "[RAY]")
{
    Vector3 origin(0, 0, 0);
    Vector3 unnormalised_direction(10, 0, 0);
    Ray ray(origin, unnormalised_direction);

    // Since the direction was (10, 0, 0), the normalised direction should be (1, 0, 0)
    Vector3 normalised_direction(1, 0, 0);
    REQUIRE(ray.direction == normalised_direction);
}

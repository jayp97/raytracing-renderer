// test_triangle.cpp
#include "catch_amalgamated.hpp"
#include "../include/Triangle.h"
#include "../include/Ray.h"
#include "../include/Vector3.h"

const float tolerance = 1e-5;

TEST_CASE("Triangle Intersection - Ray Hits Triangle", "[TRIANGLE]")
{
    Material defaultMaterial;
    Triangle triangle(Vector3(0, 0, 0), Vector3(1, 0, 0), Vector3(0, 1, 0), defaultMaterial);
    Ray ray(Vector3(0.25f, 0.25f, -1), Vector3(0, 0, 1));

    float t, u, v;
    REQUIRE(triangle.intersect(ray, t, u, v) == true);
    REQUIRE(t > 0);
}

TEST_CASE("Triangle Intersection - Ray Misses Triangle", "[TRIANGLE]")
{
    Material defaultMaterial;
    Triangle triangle(Vector3(0, 0, 0), Vector3(1, 0, 0), Vector3(0, 1, 0), defaultMaterial);
    Ray ray(Vector3(-1, -1, -1), Vector3(0, 0, 1));

    float t, u, v;
    REQUIRE(triangle.intersect(ray, t, u, v) == false);
}

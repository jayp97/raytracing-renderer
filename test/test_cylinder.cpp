#include "catch_amalgamated.hpp"
#include "../include/Cylinder.h"
#include "../include/Ray.h"
#include "../include/Vector3.h"
#include "../include/Intersection.h"
#include <limits>

TEST_CASE("Cylinder Intersection - Ray Hits Cylinder", "[CYLINDER]")
{
    Material defaultMaterial;
    Cylinder cylinder(Vector3(0, 0, 0), Vector3(0, 1, 0), 0.5f, 1.0f, defaultMaterial);
    Ray ray(Vector3(0.25f, 0.5f, -1), Vector3(0, 0, 1));

    Intersection hit;
    REQUIRE(cylinder.intersect(ray, hit) == true);
    REQUIRE(hit.distance > 0); // Ensure the intersection distance is valid
}

TEST_CASE("Cylinder Intersection - Ray Misses Cylinder", "[CYLINDER]")
{
    Material defaultMaterial;
    Cylinder cylinder(Vector3(0, 0, 0), Vector3(0, 1, 0), 0.5f, 1.0f, defaultMaterial);
    Ray ray(Vector3(2, 2, -1), Vector3(0, 0, 1));

    Intersection hit;
    REQUIRE(cylinder.intersect(ray, hit) == false);
}

TEST_CASE("Cylinder Intersection - Ray Hits Top Cap", "[CYLINDER]")
{
    Material defaultMaterial;
    Cylinder cylinder(Vector3(0, 0, 0), Vector3(0, 1, 0), 0.5f, 1.0f, defaultMaterial);
    Ray ray(Vector3(0.25f, 1.0f, -1), Vector3(0, 0, 1));

    Intersection hit;
    REQUIRE(cylinder.intersect(ray, hit) == true);
    REQUIRE(hit.distance > 0); // Ensure the intersection distance is valid
}

TEST_CASE("Cylinder Intersection - Ray Hits Bottom Cap", "[CYLINDER]")
{
    Material defaultMaterial;
    Cylinder cylinder(Vector3(0, 0, 0), Vector3(0, 1, 0), 0.5f, 1.0f, defaultMaterial);
    Ray ray(Vector3(0.25f, 0.0f, -1), Vector3(0, 0, 1));

    Intersection hit;
    REQUIRE(cylinder.intersect(ray, hit) == true);
    REQUIRE(hit.distance > 0); // Ensure the intersection distance is valid
}

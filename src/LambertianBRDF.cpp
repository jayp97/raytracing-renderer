// LambertianBRDF.cpp
#include "LambertianBRDF.h"
#include <cmath>
#include <random>

// Constructor
LambertianBRDF::LambertianBRDF(const Color &albedo)
    : albedo(albedo)
{
}

// Evaluate the BRDF value (Lambertian)
Color LambertianBRDF::evaluate(const Vector3 &wi, const Vector3 &wo, const Vector3 &normal) const
{
    // Suppress unused parameter warnings
    (void)wi;
    (void)wo;
    (void)normal;

    // For Lambertian, the BRDF is albedo / pi
    return albedo / static_cast<float>(M_PI);
}

// Sample an incoming direction based on cosine-weighted hemisphere sampling
Vector3 LambertianBRDF::sample(const Vector3 &wo, const Vector3 &normal, float &pdf_val) const
{
    // Suppress unused parameter warnings
    (void)wo;
    (void)normal;

    // Cosine-weighted sampling
    float u1 = static_cast<float>(rand()) / RAND_MAX;
    float u2 = static_cast<float>(rand()) / RAND_MAX;

    float r = std::sqrt(u1);
    float theta = 2.0f * static_cast<float>(M_PI) * u2;

    float x = r * std::cos(theta);
    float y = r * std::sin(theta);
    float z = std::sqrt(1.0f - u1);

    // Construct orthonormal basis
    Vector3 w = normal;
    Vector3 u = ((std::abs(w.x) > 0.1f) ? Vector3(0.0f, 1.0f, 0.0f) : Vector3(1.0f, 0.0f, 0.0f)).cross(w).normalise();
    Vector3 v = w.cross(u);

    Vector3 sampleDir = (u * x + v * y + w * z).normalise();

    pdf_val = sampleDir.dot(normal) / static_cast<float>(M_PI);

    return sampleDir;
}

// Compute the PDF for a given incoming direction
float LambertianBRDF::pdf(const Vector3 &wi, const Vector3 &wo, const Vector3 &normal) const
{
    // Suppress unused parameter warnings
    (void)wo;

    return std::max(wi.dot(normal), 0.0f) / static_cast<float>(M_PI);
}

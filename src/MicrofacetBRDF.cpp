// MicrofacetBRDF.cpp
#include "MicrofacetBRDF.h"
#include <cmath>
#include <algorithm>

// Constructor
MicrofacetBRDF::MicrofacetBRDF(const Color &baseColor, float roughness, float F0)
    : baseColor(baseColor), roughness(std::max(0.05f, std::min(roughness, 1.0f))), F0(F0),
      rng(std::random_device{}()), distribution(0.0f, 1.0f)
{
}

// Normal Distribution Function (D) - GGX
float MicrofacetBRDF::D(const Vector3 &m, const Vector3 &normal) const
{
    float NdotM = std::max(normal.dot(m), 0.0f);
    float alpha = roughness * roughness;
    float alpha2 = alpha * alpha;
    float NdotM2 = NdotM * NdotM;

    float denom = NdotM2 * (alpha2 - 1.0f) + 1.0f;
    denom = static_cast<float>(M_PI) * denom * denom;

    return alpha2 / denom;
}

// Geometry Function (G) - Smith's G1 for GGX
float MicrofacetBRDF::G(const Vector3 &wi, const Vector3 &wo, const Vector3 &m, const Vector3 &normal) const
{
    // Suppress unused parameter warning
    (void)normal;

    auto lambda = [&](const Vector3 &v) -> float
    {
        float VdotM = std::max(v.dot(m), 0.0f);
        float tanTheta = std::sqrt(1.0f - VdotM * VdotM) / VdotM;
        float a = 1.0f / (roughness * tanTheta);
        if (a < 1.6f)
        {
            return (3.535f * a + 2.181f * a * a) / (1.0f + 2.276f * a + 2.577f * a * a);
        }
        else
        {
            return 1.0f;
        }
    };

    return lambda(wi) + lambda(wo);
}

// Fresnel Function (F) - Schlick's Approximation
Color MicrofacetBRDF::F(const Vector3 &wi, const Vector3 &m) const
{
    float cosTheta = std::max(wi.dot(m), 0.0f);
    return baseColor + (Color(1.0f, 1.0f, 1.0f) - baseColor) * std::pow(1.0f - cosTheta, 5.0f);
}

// Evaluate the BRDF value (Cook-Torrance)
Color MicrofacetBRDF::evaluate(const Vector3 &wi, const Vector3 &wo, const Vector3 &normal) const
{
    Vector3 m = (wi + wo).normalise();
    if (m.length() == 0.0f)
        return Color(0.0f, 0.0f, 0.0f);

    // Removed the explicit declaration of NdotM to eliminate unused variable warning
    float NdotWi = std::max(normal.dot(wi), 0.0f);
    float NdotWo = std::max(normal.dot(wo), 0.0f);

    if (NdotWi <= 0.0f || NdotWo <= 0.0f)
        return Color(0.0f, 0.0f, 0.0f);

    float D_val = D(m, normal);
    float G_val = G(wi, wo, m, normal);
    Color F_val = F(wi, m);

    float denominator = 4.0f * NdotWi * NdotWo + 1e-7f; // Prevent division by zero

    return (D_val * G_val * F_val) / denominator;
}

// Sample an incoming direction based on GGX distribution
Vector3 MicrofacetBRDF::sample(const Vector3 &wo, const Vector3 &normal, float &pdf_val) const
{
    // Importance sampling the GGX distribution to sample the microfacet normal m
    float u1 = distribution(rng);
    float u2 = distribution(rng);

    float alpha2 = roughness * roughness;

    float phi = 2.0f * static_cast<float>(M_PI) * u1;
    float cosTheta = std::sqrt((1.0f - u2) / (1.0f + (alpha2 - 1.0f) * u2));
    float sinTheta = std::sqrt(1.0f - cosTheta * cosTheta);

    Vector3 m;
    m.x = sinTheta * std::cos(phi);
    m.y = sinTheta * std::sin(phi);
    m.z = cosTheta;

    // Transform m to align with the surface normal
    Vector3 up = (std::abs(normal.z) < 0.999f) ? Vector3(0.0f, 0.0f, 1.0f) : Vector3(1.0f, 0.0f, 0.0f);
    Vector3 tangent = up.cross(normal).normalise();
    Vector3 bitangent = normal.cross(tangent);

    m = (tangent * m.x + bitangent * m.y + normal * m.z).normalise();

    Vector3 wi = (2.0f * m.dot(wo) * m - wo).normalise();

    // Compute PDF
    float D_val = D(m, normal);
    float pdf_m = D_val * m.dot(normal) / std::max(0.0f, wo.dot(m));

    pdf_val = pdf_m;

    // **Removed the incorrect flipping of wi below**

    return wi.normalise();
}

// Compute the PDF for a given incoming direction
float MicrofacetBRDF::pdf(const Vector3 &wi, const Vector3 &wo, const Vector3 &normal) const
{
    Vector3 m = (wi + wo).normalise();
    if (m.length() == 0.0f)
        return 0.0f;

    float D_val = D(m, normal);
    float pdf_m = D_val * m.dot(normal) / std::max(0.0f, wo.dot(m));

    return pdf_m;
}

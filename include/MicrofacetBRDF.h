// MicrofacetBRDF.h
#ifndef MICRFACET_BRDF_H
#define MICRFACET_BRDF_H

#include "BRDF.h"
#include "Color.h"
#include "Vector3.h"
#include <random>

class MicrofacetBRDF : public BRDF
{
public:
    MicrofacetBRDF(const Color &baseColor, float roughness, float F0);

    virtual Color evaluate(const Vector3 &wi, const Vector3 &wo, const Vector3 &normal) const override;
    virtual Vector3 sample(const Vector3 &wo, const Vector3 &normal, float &pdf_val) const override;
    virtual float pdf(const Vector3 &wi, const Vector3 &wo, const Vector3 &normal) const override;

private:
    Color baseColor;
    float roughness;
    float F0; // Fresnel reflectance at normal incidence

    // Random number generator for sampling
    mutable std::mt19937 rng;
    mutable std::uniform_real_distribution<float> distribution;

    // Helper functions
    float D(const Vector3 &m, const Vector3 &normal) const;
    float G(const Vector3 &wi, const Vector3 &wo, const Vector3 &m, const Vector3 &normal) const;
    Color F(const Vector3 &wi, const Vector3 &m) const;
};

#endif // MICRFACET_BRDF_H

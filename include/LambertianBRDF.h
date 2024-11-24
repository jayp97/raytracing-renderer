// LambertianBRDF.h
#ifndef LAMBERTIAN_BRDF_H
#define LAMBERTIAN_BRDF_H

#include "BRDF.h"

class LambertianBRDF : public BRDF
{
public:
    LambertianBRDF(const Color &albedo);

    virtual Color evaluate(const Vector3 &wi, const Vector3 &wo, const Vector3 &normal) const override;
    virtual Vector3 sample(const Vector3 &wo, const Vector3 &normal, float &pdf) const override;
    virtual float pdf(const Vector3 &wi, const Vector3 &wo, const Vector3 &normal) const override;

private:
    Color albedo;
};

#endif // LAMBERTIAN_BRDF_H

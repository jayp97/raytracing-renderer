// BRDF.h
#ifndef BRDF_H
#define BRDF_H

#include "Vector3.h"
#include "Color.h"

class BRDF
{
public:
    virtual ~BRDF() {}

    // Evaluate the BRDF value given incoming and outgoing directions and the normal
    virtual Color evaluate(const Vector3 &wi, const Vector3 &wo, const Vector3 &normal) const = 0;

    // Sample an incoming direction based on the BRDF and return the PDF
    virtual Vector3 sample(const Vector3 &wo, const Vector3 &normal, float &pdf) const = 0;

    // Compute the PDF for a given incoming direction
    virtual float pdf(const Vector3 &wi, const Vector3 &wo, const Vector3 &normal) const = 0;
};

#endif // BRDF_H

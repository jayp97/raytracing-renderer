// Material.h
#ifndef MATERIAL_H
#define MATERIAL_H

#include "Vector3.h"

class Material
{
public:
    // Default constructor with typical default values
    Material()
        : ks(0.0f), kd(1.0f), specularExponent(1.0f),
          ambient(0.1f, 0.1f, 0.1f),
          diffuseColor(1.0f, 1.0f, 1.0f), specularColor(1.0f, 1.0f, 1.0f),
          isReflective(false), reflectivity(0.0f),
          isRefractive(false), refractiveIndex(1.0f) {}

    // Material properties
    float ks;               // Specular coefficient
    float kd;               // Diffuse coefficient
    float specularExponent; // Specular exponent for shininess
    Vector3 ambient;        // Ambient color
    Vector3 diffuseColor;   // Diffuse color
    Vector3 specularColor;  // Specular color
    bool isReflective;      // Reflective flag
    float reflectivity;     // Reflectivity factor
    bool isRefractive;      // Refractive flag
    float refractiveIndex;  // Refractive index
};

#endif // MATERIAL_H

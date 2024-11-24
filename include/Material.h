// Material.h
#ifndef MATERIAL_H
#define MATERIAL_H

#include "Color.h"
#include <memory>
#include "Texture.h"
#include "BRDF.h" // Include BRDF base class

class Material
{
public:
    // Default constructor with typical default values
    Material()
        : ks(0.0f), kd(1.0f), specularExponent(1.0f),
          ambient(0.1f, 0.1f, 0.1f),
          diffuseColor(1.0f, 1.0f, 1.0f), specularColor(1.0f, 1.0f, 1.0f),
          isReflective(false), reflectivity(0.0f),
          isRefractive(false), refractiveIndex(1.0f),
          texture(nullptr),
          brdf(nullptr) {} // Initialize BRDF to nullptr

    // Material properties
    float ks;               // Specular coefficient
    float kd;               // Diffuse coefficient
    float specularExponent; // Specular exponent for shininess
    Color ambient;          // Ambient color
    Color diffuseColor;     // Diffuse color
    Color specularColor;    // Specular color
    bool isReflective;      // Reflective flag
    float reflectivity;     // Reflectivity factor
    bool isRefractive;      // Refractive flag
    float refractiveIndex;  // Refractive index

    // Texture
    std::shared_ptr<Texture> texture;

    // BRDF
    std::shared_ptr<BRDF> brdf; // Pointer to a BRDF instance
};

#endif // MATERIAL_H

// BlinnPhongShader.cpp
#include "BlinnPhongShader.h"
#include "Ray.h"
#include <algorithm>
#include <cmath>

BlinnPhongShader::BlinnPhongShader(const Scene &scene, const Vector3 &cameraPos)
    : scene(scene), cameraPosition(cameraPos) {}

Color BlinnPhongShader::shade(const Intersection &hit) const
{
    Color finalColor(0, 0, 0); // Initialize to black

    // Use texture color if available
    Color diffuseColor = hit.material.diffuseColor;
    if (hit.material.texture)
    {
        diffuseColor = hit.material.texture->getColor(hit.u, hit.v);
    }

    Vector3 viewDir = (cameraPosition - hit.point).normalise();
    Vector3 normal = hit.normal.normalise();

    // Compute ambient component once
    Color ambient = diffuseColor * hit.material.kd; // Or use hit.material.ambient if set

    finalColor += ambient; // Add ambient component to final color

    for (const auto &light : scene.lights)
    {
        Vector3 lightDir = (light.getPosition() - hit.point).normalise();
        float lightDistance = (light.getPosition() - hit.point).length();

        if (isInShadow(hit.point, lightDir, lightDistance))
        {
            // Skip diffuse and specular contributions from this light
            continue;
        }

        Vector3 halfDir = (lightDir + viewDir).normalise();

        float diff = std::max(normal.dot(lightDir), 0.0f);
        Color diffuse = diffuseColor * hit.material.kd * diff * light.getIntensity();

        float specAngle = std::max(normal.dot(halfDir), 0.0f);
        float spec = std::pow(specAngle, hit.material.specularExponent);
        Color specular = hit.material.specularColor * hit.material.ks * spec * light.getIntensity();

        finalColor += diffuse + specular;
    }

    return finalColor.clamp(0.0f, 1.0f);
}

bool BlinnPhongShader::isInShadow(const Vector3 &point, const Vector3 &lightDir, float lightDistance) const
{
    const float shadowBias = 1e-3f;
    Ray shadowRay(point + lightDir * shadowBias, lightDir);

    for (const auto &object : scene.objects)
    {
        Intersection shadowHit;
        if (object->intersect(shadowRay, shadowHit))
        {
            if (shadowHit.distance < lightDistance)
            {
                return true; // Point is in shadow
            }
        }
    }

    return false; // Point is illuminated
}

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

    // Ambient component with a fixed intensity
    Color ambient = diffuseColor * hit.material.kd * 0.1f; // Ambient intensity is 0.1

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

        // Diffuse shading
        float diff = std::max(normal.dot(lightDir), 0.0f);
        Color diffuse = diffuseColor * hit.material.kd * diff;

        // Specular shading using Blinn-Phong model
        Vector3 halfDir = (lightDir + viewDir).normalise();
        float specAngle = std::max(normal.dot(halfDir), 0.0f);
        float spec = std::pow(specAngle, hit.material.specularExponent);
        Color specular = hit.material.specularColor * hit.material.ks * spec;

        // Linear attenuation to maintain realistic lighting
        // Using a simple inverse square law with a slight bias to prevent extreme attenuation
        float attenuation = 1.0f / (lightDistance * lightDistance + 1.0f);

        // Multiply by light intensity
        Color lightIntensity = light.getIntensity();

        // Add contributions from this light
        finalColor += (diffuse + specular) * lightIntensity * attenuation;
    }

    // Do not clamp here; allow high dynamic range values for accurate linear processing
    return finalColor;
}

bool BlinnPhongShader::isInShadow(const Vector3 &point, const Vector3 &lightDir, float lightDistance) const
{
    const float shadowBias = 1e-4f; // Small bias to prevent self-shadowing
    Ray shadowRay(point + lightDir * shadowBias, lightDir);

    // Check for intersection with BVH
    Intersection shadowHit;
    if (scene.intersect(shadowRay, shadowHit))
    {
        if (shadowHit.distance < lightDistance)
        {
            return true; // Point is in shadow
        }
    }

    return false; // Point is illuminated
}

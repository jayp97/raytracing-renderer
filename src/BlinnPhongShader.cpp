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

    // Number of samples per area light for soft shadows
    const int samplesPerAreaLight = 16;

    for (const auto &light : scene.lights)
    {
        if (light.getType() == LightType::Point)
        {
            // --- Point Light Shading ---
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

            // Adjusted attenuation to balance brightness
            float attenuation = 1.0f / (lightDistance * lightDistance + 1.0f);

            // Multiply by light intensity
            Color lightIntensity = light.getIntensity();

            // Add contributions from this light
            finalColor += (diffuse + specular) * lightIntensity * attenuation;
        }
        else if (light.getType() == LightType::Area)
        {
            // --- Area Light Shading with Soft Shadows ---
            Color accumulatedLight(0, 0, 0);
            float areaSampleWeight = 1.0f / static_cast<float>(samplesPerAreaLight);

            // Initialize random number generators for sampling
            std::mt19937 rng(std::random_device{}());
            std::uniform_real_distribution<float> dist(0.0f, 1.0f);

            for (int i = 0; i < samplesPerAreaLight; ++i)
            {
                // Sample a random position on the area light
                Vector3 sampledLightPos = light.samplePosition(rng, dist);
                Vector3 lightDir = (sampledLightPos - hit.point).normalise();
                float lightDistance = (sampledLightPos - hit.point).length();

                // Check if the point is in shadow relative to this sample
                if (isInShadow(hit.point, lightDir, lightDistance))
                {
                    continue; // This sample is occluded; no contribution
                }

                // Diffuse shading
                float diff = std::max(normal.dot(lightDir), 0.0f);
                Color diffuse = diffuseColor * hit.material.kd * diff;

                // Specular shading using Blinn-Phong model
                Vector3 halfDir = (lightDir + viewDir).normalise();
                float specAngle = std::max(normal.dot(halfDir), 0.0f);
                float spec = std::pow(specAngle, hit.material.specularExponent);
                Color specular = hit.material.specularColor * hit.material.ks * spec;

                // Adjusted attenuation to balance brightness
                float attenuation = 1.0f / (lightDistance * lightDistance + 1.0f);

                // Multiply by light intensity
                Color lightIntensity = light.getIntensity();

                // Accumulate the light contributions
                accumulatedLight += (diffuse + specular) * lightIntensity * attenuation;
            }

            // Average the contributions from all samples
            finalColor += accumulatedLight * areaSampleWeight;
        }
    }

    // Do not clamp here; allow high dynamic range values for accurate linear processing
    return finalColor;
}

bool BlinnPhongShader::isInShadow(const Vector3 &point, const Vector3 &lightDir, float lightDistance) const
{
    const float shadowBias = 1e-4f; // Small bias to prevent self-shadowing
    Ray shadowRay(point + lightDir * shadowBias, lightDir);

    // Check for intersection with the scene
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

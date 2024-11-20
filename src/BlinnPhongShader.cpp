// BlinnPhongShader.cpp
#include "BlinnPhongShader.h"
#include <algorithm>
#include <cmath>

BlinnPhongShader::BlinnPhongShader(const Scene &scene, const Vector3 &cameraPos)
    : scene(scene), cameraPosition(cameraPos)
{
    // Initialization if needed
}

Color BlinnPhongShader::shade(const Intersection &hit) const
{
    // Initialize the final color with the ambient component
    Color finalColor = hit.material.ambient;

    // Normalize the normal at the hit point
    Vector3 normal = hit.normal.normalise();

    // Compute the view direction (from hit point to camera)
    Vector3 viewDir = (cameraPosition - hit.point).normalise();

    // Iterate over each light source
    for (const auto &light : scene.lights)
    {
        // Compute the direction from the hit point to the light
        Vector3 lightDir = (light.getPosition() - hit.point).normalise();

        // Compute the halfway vector between light direction and view direction
        Vector3 halfDir = (lightDir + viewDir).normalise();

        // Diffuse component
        float diff = std::max(normal.dot(lightDir), 0.0f);
        Color diffuse = hit.material.diffuseColor * hit.material.kd * diff;
        diffuse = diffuse * light.getIntensity(); // Element-wise multiplication

        // Specular component
        float specAngle = std::max(normal.dot(halfDir), 0.0f);
        float spec = std::pow(specAngle, hit.material.specularExponent);
        Color specular = hit.material.specularColor * hit.material.ks * spec;
        specular = specular * light.getIntensity(); // Element-wise multiplication

        // Shadow check
        Ray shadowRay(hit.point + normal * 1e-4f, lightDir);
        bool inShadow = false;

        for (const auto &object : scene.objects)
        {
            Intersection shadowHit;
            if (object->intersect(shadowRay, shadowHit))
            {
                float distanceToLight = (light.getPosition() - hit.point).length();
                if (shadowHit.distance < distanceToLight)
                {
                    inShadow = true;
                    break;
                }
            }
        }

        if (!inShadow)
        {
            // Accumulate diffuse and specular contributions
            finalColor += diffuse + specular;
        }
    }

    // Clamp the final color to [0, 1] range
    finalColor = finalColor.clamp(0.0f, 1.0f);

    return finalColor;
}

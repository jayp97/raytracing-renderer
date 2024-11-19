// BlinnPhongShader.cpp
#include "BlinnPhongShader.h"
#include <algorithm> // For std::max
#include <cmath>     // For std::pow

BlinnPhongShader::BlinnPhongShader(const Scene &scene, const Vector3 &cameraPos)
    : scene(scene), cameraPosition(cameraPos)
{
    // Initialization if needed
}

Color BlinnPhongShader::shade(const Intersection &hit) const
{
    Color finalColor(0, 0, 0); // Initialize to black

    // Iterate over each light source
    for (const auto &light : scene.lights)
    {
        // Use the getter method to access the light's position
        Vector3 lightDir = (light.getPosition() - hit.point).normalise();
        Vector3 viewDir = (cameraPosition - hit.point).normalise();
        Vector3 halfDir = (lightDir + viewDir).normalise();
        Vector3 normal = hit.normal.normalise();

        // Ambient component
        Color ambient = hit.material.ambient;

        // Diffuse component
        float diff = std::max(normal.dot(lightDir), 0.0f);
        Color diffuse = hit.material.diffuseColor * diff;

        // Specular component
        float specAngle = std::max(normal.dot(halfDir), 0.0f);
        float spec = std::pow(specAngle, hit.material.specularExponent);
        Color specular = hit.material.specularColor * spec;

        // Shadow check
        // Directly construct the Ray with origin and direction
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
            // Accumulate color contributions
            finalColor += ambient + diffuse + specular;
        }
        else
        {
            // Only ambient component is added if in shadow
            finalColor += ambient;
        }
    }

    // Clamp the final color to [0, 1] range
    finalColor = finalColor.clamp(0.0f, 1.0f);

    return finalColor;
}

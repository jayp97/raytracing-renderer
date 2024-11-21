#include "BlinnPhongShader.h"
#include "Ray.h"

BlinnPhongShader::BlinnPhongShader(const Scene &scene, const Vector3 &cameraPos)
    : scene(scene), cameraPosition(cameraPos) {}

Color BlinnPhongShader::shade(const Intersection &hit) const
{
    Color finalColor(0, 0, 0); // Initialize to black

    for (const auto &light : scene.lights)
    {
        Vector3 lightDir = (light.getPosition() - hit.point).normalise();
        float lightDistance = (light.getPosition() - hit.point).length();

        if (isInShadow(hit.point, lightDir, lightDistance))
        {
            finalColor += hit.material.ambient;
            std::cout << "In shadow. Adding ambient: " << hit.material.ambient << "\n";
            continue;
        }

        Vector3 viewDir = (cameraPosition - hit.point).normalise();
        Vector3 halfDir = (lightDir + viewDir).normalise();
        Vector3 normal = hit.normal.normalise();

        Color ambient = hit.material.ambient;
        float diff = std::max(normal.dot(lightDir), 0.0f);
        Color diffuse = hit.material.diffuseColor * hit.material.kd * diff * light.getIntensity();
        float specAngle = std::max(normal.dot(halfDir), 0.0f);
        float spec = std::pow(specAngle, hit.material.specularExponent);
        Color specular = hit.material.specularColor * hit.material.ks * spec * light.getIntensity();

        finalColor += ambient + diffuse + specular;

        // std::cout << "Ambient: " << ambient << ", Diffuse: " << diffuse << ", Specular: " << specular << "\n";
    }

    return finalColor.clamp(0.0f, 1.0f);
}

bool BlinnPhongShader::isInShadow(const Vector3 &point, const Vector3 &lightDir, float lightDistance) const
{
    // Increase the offset to ensure the shadow ray starts outside any geometry
    const float shadowBias = 1e-3f;
    Ray shadowRay(point + lightDir * shadowBias, lightDir);

    for (const auto &object : scene.objects)
    {
        Intersection shadowHit;
        if (object->intersect(shadowRay, shadowHit))
        {
            // Check if the object is between the point and the light
            if (shadowHit.distance < lightDistance)
            {
                // Optionally, you can add a small bias to the lightDistance comparison
                std::cout << "Shadow detected at distance: " << shadowHit.distance << "\n";
                return true; // Point is in shadow
            }
        }
    }

    return false; // Point is illuminated
}

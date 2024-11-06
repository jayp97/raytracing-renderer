#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "Image.h"
#include "Camera.h"
#include "SceneLoader.h"
#include "Ray.h"
#include "Vector3.h"

class Raytracer
{
public:
    Raytracer(int width, int height);
    void render(const SceneLoader &sceneLoader, const Camera &camera);
    bool trace(const Ray &ray, const SceneLoader &sceneLoader, Color &color);

    Image getImage() const { return image; } // Access image for testing

private:
    int width, height;
    Image image;
};

#endif // RAYTRACER_H

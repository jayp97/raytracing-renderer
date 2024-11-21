// Texture.h
#ifndef TEXTURE_H
#define TEXTURE_H

#include "Color.h"

class Texture
{
public:
    virtual ~Texture() {}

    // Given UV coordinates, return the color
    virtual Color getColor(float u, float v) const = 0;
};

#endif // TEXTURE_H

// CheckerboardTexture.h
#ifndef CHECKERBOARD_TEXTURE_H
#define CHECKERBOARD_TEXTURE_H

#include "Texture.h"
#include <memory>

class CheckerboardTexture : public Texture
{
public:
    CheckerboardTexture(const Color &color1, const Color &color2, float scale = 1.0f)
        : color1(color1), color2(color2), scale(scale) {}

    Color getColor(float u, float v) const override;

private:
    Color color1;
    Color color2;
    float scale;
};

#endif // CHECKERBOARD_TEXTURE_H

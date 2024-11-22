// CheckerboardTexture.cpp
#include "CheckerboardTexture.h"
#include <cmath>

Color CheckerboardTexture::getColor(float u, float v) const
{
    // Ensure u and v wrap around [0,1]
    u = u - std::floor(u);
    v = v - std::floor(v);

    int checkU = static_cast<int>(std::floor(u * scale)) % 2;
    int checkV = static_cast<int>(std::floor(v * scale)) % 2;
    if ((checkU + checkV) % 2 == 0)
    {
        return color1;
    }
    else
    {
        return color2;
    }
}

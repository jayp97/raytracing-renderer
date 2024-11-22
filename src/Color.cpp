#include "Color.h"
#include <cmath>

Color Color::toneMap(float exposure) const
{
    // Apply exposure
    Color exposed = (*this) * exposure;

    // Apply Reinhard tone mapping
    return Color(
        exposed.r / (exposed.r + 1.0f),
        exposed.g / (exposed.g + 1.0f),
        exposed.b / (exposed.b + 1.0f));
}

#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include <string>

struct Color
{
    float r, g, b;
    Color() : r(0), g(0), b(0) {}
    Color(float r, float g, float b) : r(r), g(g), b(b) {}
};

class Image
{
public:
    Image(int width, int height);

    void setPixel(int x, int y, const Color &color);
    bool saveAsPPM(const std::string &filename) const;

private:
    int width, height;
    std::vector<Color> pixels;
};

#endif // IMAGE_H

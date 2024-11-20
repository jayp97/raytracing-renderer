// include/Image.h
#ifndef IMAGE_H
#define IMAGE_H

#include "Color.h"
#include <vector>
#include <string>

class Image
{
public:
    Image();
    Image(int width, int height);

    void setPixel(int x, int y, const Color &color);
    Color getPixel(int x, int y) const;
    bool saveAsPPM(const std::string &filename) const;

private:
    int width;
    int height;
    std::vector<Color> pixels;
};

#endif // IMAGE_H

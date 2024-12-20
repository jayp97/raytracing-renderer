#include "Image.h"
#include <fstream>
#include <mutex>

// Mutex for thread safety
std::mutex imageMutex;

Image::Image() : width(0), height(0), pixels()
{
    // Initialize to an empty image or set default values if necessary
}

Image::Image(int width, int height) : width(width), height(height), pixels(width * height) {}

void Image::setPixel(int x, int y, const Color &color)
{
    if (x >= 0 && x < width && y >= 0 && y < height)
    {
        // Lock the mutex for thread-safe access
        std::lock_guard<std::mutex> lock(imageMutex);
        pixels[y * width + x] = color;
    }
}

Color Image::getPixel(int x, int y) const
{
    if (x >= 0 && x < width && y >= 0 && y < height)
    {
        return pixels[y * width + x];
    }
    return Color(); // Return default black color if out of bounds
}

bool Image::saveAsPPM(const std::string &filename) const
{
    std::ofstream file(filename);
    if (!file.is_open())
        return false;

    file << "P3\n"
         << width << " " << height << "\n255\n";
    for (const auto &color : pixels)
    {
        file << static_cast<int>(255.99 * color.r) << " "
             << static_cast<int>(255.99 * color.g) << " "
             << static_cast<int>(255.99 * color.b) << "\n";
    }
    file.close();
    return true;
}

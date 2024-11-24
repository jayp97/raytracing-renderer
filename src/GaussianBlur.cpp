// GaussianBlur.cpp
#include "GaussianBlur.h"
#include <cmath>

GaussianBlur::GaussianBlur(int kernelSize, float sigma)
    : kernelSize(kernelSize), sigma(sigma)
{
    generateKernel();
}

void GaussianBlur::generateKernel()
{
    kernel.resize(kernelSize * kernelSize);
    int halfSize = kernelSize / 2;
    float sum = 0.0f;
    for (int y = -halfSize; y <= halfSize; ++y)
    {
        for (int x = -halfSize; x <= halfSize; ++x)
        {
            float exponent = -(x * x + y * y) / (2.0f * sigma * sigma);
            float value = std::exp(exponent);
            kernel[(y + halfSize) * kernelSize + (x + halfSize)] = value;
            sum += value;
        }
    }
    // Normalize the kernel
    for (auto &val : kernel)
    {
        val /= sum;
    }
}

Image GaussianBlur::apply(const Image &input) const
{
    int width = input.getWidth();
    int height = input.getHeight();
    Image output(width, height);

    int halfSize = kernelSize / 2;

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            Color accumulated(0.0f, 0.0f, 0.0f);
            for (int ky = -halfSize; ky <= halfSize; ++ky)
            {
                for (int kx = -halfSize; kx <= halfSize; ++kx)
                {
                    int sampleX = std::clamp(x + kx, 0, width - 1);
                    int sampleY = std::clamp(y + ky, 0, height - 1);
                    Color sampleColor = input.getPixel(sampleX, sampleY);
                    float weight = kernel[(ky + halfSize) * kernelSize + (kx + halfSize)];
                    accumulated += sampleColor * weight;
                }
            }
            output.setPixel(x, y, accumulated);
        }
    }

    return output;
}

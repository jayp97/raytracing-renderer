// GaussianBlur.h
#ifndef GAUSSIAN_BLUR_H
#define GAUSSIAN_BLUR_H

#include "Image.h"

class GaussianBlur
{
public:
    GaussianBlur(int kernelSize, float sigma);
    Image apply(const Image &input) const;

private:
    int kernelSize;
    float sigma;
    std::vector<float> kernel;

    void generateKernel();
};

#endif // GAUSSIAN_BLUR_H

#pragma once

#include "CircularBuffer.h"

class Compressor
{
public:
    Compressor();
    float compressSample(float data,
                         float thresh,
                         float ratio,
                         float attack,
                         float release,
                         float knee);

private:
    static float interpolatePoints(const float* xPoints, const float* yPoints, float detectedValue);

    CircularBuffer buffer_;
    float tav_, rms_, gain_;
};
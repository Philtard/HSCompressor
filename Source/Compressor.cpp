#include "Compressor.h"

Compressor::Compressor()
{
    buffer_ = CircularBuffer(150, 20);
    tav_ = 0.01f;
    rms_ = 0;
    gain_ = 1;
}

float Compressor::compressSample(
    float data, float thresh, float ratio, float attack, float release, float knee)
{
    rms_ = (1 - tav_) * rms_ + tav_ * std::pow(data, 2.0f);
    float dbRMS = 10 * std::log10(rms_);

    float slope = 1 - (1 / ratio);

    if (knee > 0 && dbRMS > (thresh - knee / 2.0) && dbRMS < (thresh + knee / 2.0))
    {
        float kneeBottom = thresh - knee / 2.0f;
        float kneeTop = thresh + knee / 2.0f;
        float xPoints[2], yPoints[2];
        xPoints[0] = kneeBottom;
        xPoints[1] = std::fmin(0.0f, kneeTop);
        yPoints[0] = 0.0f;
        yPoints[1] = slope;
        slope = interpolatePoints(&xPoints[0], &yPoints[0], thresh);
        thresh = kneeBottom;
    }

    float dbGain = std::min(0.0f, (slope * (thresh - dbRMS)));
    auto newGain = static_cast<float>(std::pow(10, dbGain / 20));

    float coeff;
    if (newGain < gain_)
        coeff = attack;
    else
        coeff = release;
    gain_ = (1 - coeff) * gain_ + coeff * newGain;

    float compressedSample = gain_ * buffer_.getData();
    buffer_.setData(data);
    buffer_.nextSample();

    return compressedSample;
}

float Compressor::interpolatePoints(const float* xPoints,
                                    const float* yPoints,
                                    float detectedValue)
{
    float result = 0.0f;
    int n = 2;

    for (int i = 0; i < n; i++)
    {
        float term = 1.0f;
        for (int j = 0; j < n; j++)
        {
            if (j != i)
            {
                term *= (detectedValue - xPoints[j]) / (xPoints[i] - xPoints[j]);
            }
        }
        result += term * yPoints[i];
    }

    return result;
}
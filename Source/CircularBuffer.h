#pragma once
#include "juce_audio_basics/juce_audio_basics.h"

class CircularBuffer {
public:
    CircularBuffer();
    CircularBuffer(int buffer_size, int delay_length);
    float getData();
    void setData(float data);
    void nextSample();

private:
    juce::AudioSampleBuffer buffer_;
    int write_index_;
    int read_index_;
    int delay_length_;
};
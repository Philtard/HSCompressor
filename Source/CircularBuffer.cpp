#include "CircularBuffer.h"

CircularBuffer::CircularBuffer()
{
    buffer_ = juce::AudioSampleBuffer();
    write_index_ = read_index_ = delay_length_ = 0;
}

CircularBuffer::CircularBuffer(int buffer_size, int delay_length)
{
    buffer_ = juce::AudioSampleBuffer(1, buffer_size);
    buffer_.clear();
    write_index_ = delay_length;
    read_index_ = 0;
    delay_length_ = delay_length;
}

float CircularBuffer::getData()
{
    return buffer_.getSample(0, read_index_);
}

void CircularBuffer::setData(float data)
{
    buffer_.setSample(0, write_index_, data);
}

void CircularBuffer::nextSample()
{
    int bufferLength = buffer_.getNumSamples();
    read_index_ = ((bufferLength + write_index_) - delay_length_) % bufferLength;
    write_index_ = (write_index_ + 1) % bufferLength;
}
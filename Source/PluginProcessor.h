#pragma once

#include "juce_audio_processors/juce_audio_processors.h"
#include "Compressor.h"

class HSCompressorAudioProcessor
    : public juce::AudioProcessor
{
public:
    HSCompressorAudioProcessor();

    void prepareToPlay(double sampleRate, int blockSize) override;
    void releaseResources() override {}

    bool isBusesLayoutSupported(const BusesLayout&) const override { return true; }

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return JucePlugin_Name; }

    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return juce::String(); }
    void changeProgramName(int, const juce::String& /*newName*/) override {}

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

private:
    juce::Array<Compressor> all_compressors_;

    juce::AudioProcessorValueTreeState apvts_;

    std::atomic<float>* thresh_;
    std::atomic<float>* slope_;
    std::atomic<float>* knee_;
    std::atomic<float>* attack_;
    std::atomic<float>* release_;

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameters();
};

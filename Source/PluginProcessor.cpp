#include "PluginProcessor.h"
#include "PluginEditor.h"

HSCompressorAudioProcessor::HSCompressorAudioProcessor()
    : apvts_(*this, nullptr, "PARAMETERS", createParameters())
{
    thresh_ = apvts_.getRawParameterValue("thresh");
    slope_ = apvts_.getRawParameterValue("ratio");
    knee_ = apvts_.getRawParameterValue("knee");
    attack_ = apvts_.getRawParameterValue("attack");
    release_ = apvts_.getRawParameterValue("release");
}

void HSCompressorAudioProcessor::prepareToPlay(double /*sampleRate*/,
                                               int /*blockSize*/)
{
    for (int channel = 0; channel < getTotalNumOutputChannels(); channel++)
    {
        all_compressors_.add(Compressor());
    }
}

void HSCompressorAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages)

{
    midiMessages.clear();
    for (int i = 0; i < buffer.getNumSamples(); i++)
    {
        float at =
            1
            - std::pow(juce::MathConstants<float>::euler,
                       ((1 / getSampleRate()) * -2.2f) / (*attack_ / 1000.0f));
        float rt =
            1
            - std::pow(juce::MathConstants<float>::euler,
                       ((1 / getSampleRate()) * -2.2f) / (*release_ / 1000.0f));

        for (int channel = 0; channel < getTotalNumOutputChannels(); channel++)
        {
            auto* data = buffer.getWritePointer(channel);
            Compressor* compressor = &all_compressors_.getReference(channel);
            data[i] = compressor->compressSample(
                data[i], *thresh_, *slope_, at, rt, *knee_);
        }
    }
}

juce::AudioProcessorEditor* HSCompressorAudioProcessor::createEditor()
{
    return new HSCompressorAudioEditor(*this, apvts_);
}

void HSCompressorAudioProcessor::getStateInformation(juce::MemoryBlock& dest_data)
{
    const auto state = apvts_.copyState();
    const auto xml = state.createXml();
    copyXmlToBinary(*xml, dest_data);
}

void HSCompressorAudioProcessor::setStateInformation(const void* data,
                                                     int sizeInBytes)
{
    const auto xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState != nullptr && xmlState->hasTagName(apvts_.state.getType()))
    {
        apvts_.replaceState(juce::ValueTree::fromXml(*xmlState));
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HSCompressorAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout
    HSCompressorAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "thresh",
        "Threshold",
        juce::NormalisableRange<float>(-60.0f, 20.0f, 0.01f),
        10.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "ratio", "Ratio", juce::NormalisableRange<float>(1.0f, 20.0f, 0.01f), 2.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "knee",
        "KneeWidth",
        juce::NormalisableRange<float>(0.0f, 24.0f, 0.01f),
        0.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "attack",
        "Attack",
        juce::NormalisableRange<float>(0.0f, 500.0f, 0.01f),
        100.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "release",
        "Release",
        juce::NormalisableRange<float>(0.01f, 2000.0f, 0.01f),
        500.0f));

    return {params.begin(), params.end()};
}

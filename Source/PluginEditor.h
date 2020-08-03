#pragma once

#include "PluginProcessor.h"

typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

class HSCompressorAudioEditor
    : public juce::AudioProcessorEditor
    , juce::AudioProcessorValueTreeState::Listener
{
public:
    explicit HSCompressorAudioEditor(HSCompressorAudioProcessor&,
                                     juce::AudioProcessorValueTreeState&);

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    void addSlider(juce::String name,
                   juce::String label_text,
                   juce::Slider& slider,
                   juce::Label& label,
                   std::unique_ptr<SliderAttachment>& attachment);
    void parameterChanged(const juce::String& parameter_id,
                          float new_value) override;

    HSCompressorAudioProcessor& processor_;
    juce::AudioProcessorValueTreeState& apvts_;

    juce::Slider thresh_slider_;
    juce::Slider slope_slider_;
    juce::Slider knee_slider_;
    juce::Slider attack_slider_;
    juce::Slider release_slider_;

    juce::Label thresh_label_;
    juce::Label slope_label_;
    juce::Label knee_label_;
    juce::Label attack_label_;
    juce::Label release_label_;

    std::unique_ptr<SliderAttachment> thresh_attachment_;
    std::unique_ptr<SliderAttachment> slope_attachment_;
    std::unique_ptr<SliderAttachment> knee_attachment_;
    std::unique_ptr<SliderAttachment> attack_attachment_;
    std::unique_ptr<SliderAttachment> release_attachment_;
};

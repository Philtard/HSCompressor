#include "PluginProcessor.h"
#include "PluginEditor.h"

HSCompressorAudioEditor::HSCompressorAudioEditor(
    HSCompressorAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor(&p)
    , processor_(p)
    , apvts_(vts)
{
    addSlider(
        "thresh", "Threshold", thresh_slider_, thresh_label_, thresh_attachment_);
    addSlider("ratio", "Ratio", slope_slider_, slope_label_, slope_attachment_);
    addSlider("knee", "Knee", knee_slider_, knee_label_, knee_attachment_);
    addSlider("attack", "Attack", attack_slider_, attack_label_, attack_attachment_);
    addSlider(
        "release", "Release", release_slider_, release_label_, release_attachment_);
    setSize(400, 300);
}

void HSCompressorAudioEditor::paint(juce::Graphics& g)
{
    g.fillAll(
        getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void HSCompressorAudioEditor::resized()
{
    thresh_slider_.setBounds(100, 0, 200, 50);
    slope_slider_.setBounds(100, 50, 200, 50);
    knee_slider_.setBounds(100, 100, 200, 50);
    attack_slider_.setBounds(100, 150, 200, 50);
    release_slider_.setBounds(100, 200, 200, 50);
}

void HSCompressorAudioEditor::parameterChanged(const juce::String& parameter_id,
                                               float new_value)
{
    DBG("parameterChanged Event: parameter_id: " << parameter_id
                                                 << " new_value: " << new_value);
}

void HSCompressorAudioEditor::addSlider(
    juce::String name,
    juce::String label_text,
    juce::Slider& slider,
    juce::Label& label,
    std::unique_ptr<SliderAttachment>& attachment)
{
    addAndMakeVisible(slider);
    attachment = std::make_unique<SliderAttachment>(apvts_, name, slider);
    label.setText(label_text, juce::dontSendNotification);
    label.attachToComponent(&slider, true);
    addAndMakeVisible(label);
}

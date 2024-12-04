#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class DFM1FilterEditor : public juce::AudioProcessorEditor
{
public:
    DFM1FilterEditor(DFM1FilterProcessor&);
    ~DFM1FilterEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    // References to the processor
    DFM1FilterProcessor& processor;

    // Sliders for parameters
    juce::Slider cutoffSlider;
    juce::Slider resonanceSlider;
    juce::Slider driveSlider;
    juce::Slider mixSlider;

    // Attachments to connect sliders to parameters
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> cutoffAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> resonanceAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DFM1FilterEditor)
};


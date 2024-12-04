#include "PluginEditor.h"
#include "PluginProcessor.h"

DFM1FilterEditor::DFM1FilterEditor(DFM1FilterProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    // Set the editor's size
    setSize(400, 300);

    // Configure sliders and attach to parameters
    auto setupSlider = [this](juce::Slider& slider, const juce::String& paramID,
                              std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& attachment)
    {
        addAndMakeVisible(slider);
        slider.setSliderStyle(juce::Slider::Rotary);
        slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
        attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            processor.getAPVTS(), paramID, slider);
    };

    setupSlider(cutoffSlider, "cutoff", cutoffAttachment);
    setupSlider(resonanceSlider, "resonance", resonanceAttachment);
    setupSlider(driveSlider, "drive", driveAttachment);
    setupSlider(mixSlider, "mix", mixAttachment);
}

DFM1FilterEditor::~DFM1FilterEditor() {}

void DFM1FilterEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.drawFittedText("DFM1 Filter", getLocalBounds(), juce::Justification::centredTop, 1);
}

void DFM1FilterEditor::resized()
{
    const auto sliderWidth = 100;
    const auto sliderHeight = 100;
    const auto margin = 20;

    cutoffSlider.setBounds(margin, margin, sliderWidth, sliderHeight);
    resonanceSlider.setBounds(margin + sliderWidth + margin, margin, sliderWidth, sliderHeight);
    driveSlider.setBounds(margin, margin + sliderHeight + margin, sliderWidth, sliderHeight);
    mixSlider.setBounds(margin + sliderWidth + margin, margin + sliderHeight + margin, sliderWidth, sliderHeight);
}


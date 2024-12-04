#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <dlfcn.h>

// Julia function typedef
typedef double (*ProcessSampleFunction)(double, double, double, double);

DFM1FilterPluginAudioProcessor::DFM1FilterPluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true))
#endif
{
    // Load Julia library
    juliaLib = dlopen("libs/libdfm1.dylib", RTLD_NOW);
    if (!juliaLib)
        throw std::runtime_error("Failed to load Julia library!");

    processSample = (ProcessSampleFunction)dlsym(juliaLib, "process_sample");
    if (!processSample)
        throw std::runtime_error("Failed to load process_sample function!");
}

DFM1FilterPluginAudioProcessor::~DFM1FilterPluginAudioProcessor()
{
    if (juliaLib)
        dlclose(juliaLib);
}

// Audio processing
void DFM1FilterPluginAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    auto* channelData = buffer.getWritePointer(0);
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        channelData[sample] = processSample(channelData[sample], cutoff, resonance, drive);
    }
}

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "PluginProcessor.h"

#include <dlfcn.h>

// Julia function typedef
typedef double (*ProcessSampleFunction)(double, double, double, double);

//==============================================================================
DFM1FilterProcessor::DFM1FilterProcessor()
        : AudioProcessor(BusesProperties()
                                 .withInput("Input", juce::AudioChannelSet::stereo(), true)
                                 .withOutput("Output", juce::AudioChannelSet::stereo(), true))
), apvts(*this, nullptr, "Parameters", createParameterLayout())
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


juce::AudioProcessorValueTreeState::ParameterLayout DFM1FilterProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "cutoff", "Cutoff", juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 0.25f), 1000.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "resonance", "Resonance", juce::NormalisableRange<float>(0.1f, 10.0f, 0.01f, 0.5f), 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "drive", "Drive", juce::NormalisableRange<float>(0.0f, 10.0f, 0.1f, 1.0f), 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "mix", "Mix", juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f, 1.0f), 1.0f));

    return { params.begin(), params.end() };
}

//==============================================================================
void DFM1FilterProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Initialization code here
}

void DFM1FilterProcessor::releaseResources()
{
    // Cleanup code here
}

bool DFM1FilterProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    // Support mono/stereo layout only
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    return true;
}

void DFM1FilterProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    // DSP logic here
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    auto* channelData = buffer.getWritePointer(0);

    // Clear unused output channels
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        channelData[sample] = processSample(channelData[sample], cutoff, resonance, drive);
    }

    // Example: Apply gain to all channels (replace with your DSP)
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            channelData[sample] *= apvts.getRawParameterValue("mix")->load();
        }
    }
}

//==============================================================================
juce::AudioProcessorEditor* DFM1FilterProcessor::createEditor()
{
    return new DFM1FilterEditor(*this);
}

bool DFM1FilterProcessor::hasEditor() const
{
    return true;
}

//==============================================================================
const juce::String DFM1FilterProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DFM1FilterProcessor::acceptsMidi() const
{
    return false;
}

bool DFM1FilterProcessor::producesMidi() const
{
    return false;
}

bool DFM1FilterProcessor::isMidiEffect() const
{
    return false;
}

double DFM1FilterProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

//==============================================================================
int DFM1FilterProcessor::getNumPrograms()
{
    return 1; // One program for simplicity
}

int DFM1FilterProcessor::getCurrentProgram()
{
    return 0;
}

void DFM1FilterProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String DFM1FilterProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void DFM1FilterProcessor::changeProgramName(int index, const juce::String& newName)
{
    juce::ignoreUnused(index, newName);
}

//==============================================================================
void DFM1FilterProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream stream(destData, true);
    apvts.state.writeToStream(stream);
}

void DFM1FilterProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    auto tree = juce::ValueTree::readFromData(data, size_t(sizeInBytes));
    if (tree.isValid())
    {
        apvts.replaceState(tree);
    }
}

//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DFM1FilterProcessor();
}

juce::AudioProcessorValueTreeState& DFM1FilterProcessor::getAPVTS()
{
    return apvts;
}


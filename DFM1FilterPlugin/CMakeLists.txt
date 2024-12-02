cmake_minimum_required(VERSION 3.15)
project(DFM1FilterPlugin VERSION 1.0.0)

# Add JUCE
find_package(JUCE CONFIG REQUIRED)

# Define the JUCE plugin
juce_add_plugin(DFM1FilterPlugin
    COMPANY_NAME "MyCompany"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE Juce
    PLUGIN_CODE DFM1
    FORMATS AU VST3 Standalone
)

# Include the Julia shared library
set(JULIA_LIB_DIR ${CMAKE_SOURCE_DIR}/libs)
set(JULIA_INCLUDE_DIR /usr/local/Cellar/julia/1.11.1/include/julia)

target_include_directories(DFM1FilterPlugin PRIVATE ${JULIA_INCLUDE_DIR})
target_link_libraries(DFM1FilterPlugin PRIVATE ${JULIA_LIB_DIR}/libdfm1.dylib)

# Add source files
target_sources(DFM1FilterPlugin PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
)

# Add JUCE modules
target_link_libraries(DFM1FilterPlugin PRIVATE
    juce::juce_audio_utils
    juce::juce_audio_processors
    juce::juce_gui_basics
    juce::juce_core
)

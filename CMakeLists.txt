cmake_minimum_required(VERSION 3.15)
project(dfmJulioos VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)

set(JUCE_DIR "tracktion_engine/modules/JUCE")

add_subdirectory(tracktion_engine)

# Add JUCE
find_package(JUCE CONFIG REQUIRED)

# Define the JUCE plugin
juce_add_plugin(dfmJulioos
    COMPANY_NAME "seclorum.wien"
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT TRUE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE sclr
    PLUGIN_CODE DFM1
    FORMATS AU VST3 Standalone
)

# Include the Julia shared library
set(JULIA_LIB_DIR ${CMAKE_SOURCE_DIR}/libs)
set(JULIA_INCLUDE_DIR /usr/local/Cellar/julia/1.11.1/include/julia)

target_include_directories(dfmJulioos PRIVATE ${JULIA_INCLUDE_DIR})
target_link_libraries(dfmJulioos PRIVATE ${JULIA_LIB_DIR}/libdfm1.dylib)

# Add source files
target_sources(dfmJulioos PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
)

# Add JUCE modules
target_link_libraries(dfmJulioos PRIVATE
    juce::juce_audio_utils
    juce::juce_audio_processors
    juce::juce_gui_basics
    juce::juce_core
)

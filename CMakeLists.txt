cmake_minimum_required(VERSION 3.15)
project(dfmJulioos VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)

# Disable unnecessary targets from tracktion_engine
set(BUILD_EXAMPLES OFF CACHE BOOL "Disable building examples")
set(BUILD_DEMOS OFF CACHE BOOL "Disable building demos")
set(BUILD_TESTS OFF CACHE BOOL "Disable building tests")

# Add tracktion_engine as a module
add_subdirectory(tracktion_engine)

# JUCE setup
set(JUCE_DIR "${CMAKE_SOURCE_DIR}/tracktion_engine/modules/JUCE")

# Define the JUCE plugin
juce_add_plugin(dfmJulioos
        COMPANY_NAME "seclorum.wien"
        IS_SYNTH TRUE
        NEEDS_MIDI_INPUT TRUE
        NEEDS_MIDI_OUTPUT TRUE
        IS_MIDI_EFFECT FALSE
        EDITOR_WANTS_KEYBOARD_FOCUS TRUE
        COPY_PLUGIN_AFTER_BUILD TRUE
        PLUGIN_MANUFACTURER_CODE JWVS
        PLUGIN_CODE DFM1
        FORMATS AU VST3 Standalone # Removed LV2 to avoid LV2URI warnings
)

# Julia setup
set(JULIA_EXECUTABLE /usr/local/bin/julia)
set(JULIA_SOURCE ${CMAKE_SOURCE_DIR}/Source/dfm1.jl)
set(JULIA_LIB_DIR ${CMAKE_BINARY_DIR}/libs)
set(JULIA_LIB_NAME libdfm1.${CMAKE_SHARED_LIBRARY_SUFFIX})
set(JULIA_OUTPUT_PATH ${JULIA_LIB_DIR}/${JULIA_LIB_NAME})
set(JULIA_INCLUDE_DIR /usr/local/Cellar/julia/1.11.1/include/julia)

# Ensure the output directory for the Julia library exists
file(MAKE_DIRECTORY ${JULIA_LIB_DIR})

# Custom command to build the Julia shared library
add_custom_command(
        OUTPUT ${JULIA_OUTPUT_PATH}
        COMMAND ${JULIA_EXECUTABLE} --project -e "
        using PackageCompiler;
        create_library(\"${JULIA_SOURCE}\",
                       outputs=[\"${JULIA_OUTPUT_PATH}\"],
                       name=\"dfm1\")"
        DEPENDS ${JULIA_SOURCE}
        COMMENT "Compiling Julia code to a shared library"
        VERBATIM
)

# Custom target to build Julia code
add_custom_target(BuildJuliaCode ALL DEPENDS ${JULIA_OUTPUT_PATH})

# Ensure the plugin depends on the Julia library
add_dependencies(dfmJulioos BuildJuliaCode)

# Link the Julia library to the plugin
target_link_libraries(dfmJulioos PRIVATE ${JULIA_LIB_DIR}/${JULIA_LIB_NAME})

# Include Julia headers
target_include_directories(dfmJulioos PRIVATE ${JULIA_INCLUDE_DIR})

# Add source files
target_sources(dfmJulioos PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginEditor.cpp
)

# Add JUCE modules and tracktion_engine
target_link_libraries(dfmJulioos PRIVATE
        tracktion_engine
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_gui_basics
        juce::juce_core
)

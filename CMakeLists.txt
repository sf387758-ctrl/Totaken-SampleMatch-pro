cmake_minimum_required(VERSION 3.22)
project(TotakanSampleMatchPRO VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 17)
add_subdirectory(JUCE)
juce_add_plugin(TotakanSampleMatchPRO
    COMPANY_NAME "Totakan Audio"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
    PLUGIN_MANUFACTURER_CODE Tota
    PLUGIN_CODE SmpM
    FORMATS VST3
    PRODUCT_NAME "Totakan SampleMatch PRO")
juce_generate_juce_header(TotakanSampleMatchPRO)
target_sources(TotakanSampleMatchPRO PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp)
target_compile_definitions(TotakanSampleMatchPRO PUBLIC JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0 JUCE_VST3_CAN_REPLACE_VST2=0)
target_link_libraries(TotakanSampleMatchPRO PRIVATE juce::juce_audio_utils juce::juce_dsp juce::juce_recommended_config_flags juce::juce_recommended_lto_flags juce::juce_recommended_warning_flags)

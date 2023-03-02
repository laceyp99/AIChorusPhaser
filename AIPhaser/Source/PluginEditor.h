/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

struct CustomRotarySlider : juce::Slider
{
    CustomRotarySlider() : juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::TextBoxBelow)
    {

    }
};

//==============================================================================
/**
*/
class AIPhaserAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    AIPhaserAudioProcessorEditor (AIPhaserAudioProcessor&);
    ~AIPhaserAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AIPhaserAudioProcessor& audioProcessor;

    CustomRotarySlider rateSlider, depthSlider, feedbackSlider, freqSlider, mixSlider;

    juce::AudioProcessorValueTreeState::SliderAttachment rateAttachment, depthAttachment, feedbackAttachment, freqAttachment, mixAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AIPhaserAudioProcessorEditor)
};

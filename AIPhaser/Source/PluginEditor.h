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
    void setChorusGuiVisible(bool isVisible);
    void setPhaserGuiVisible(bool isVisible);

private:
 
    AIPhaserAudioProcessor& audioProcessor;


    juce::ComboBox effectbox;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> effectAttachment;

    CustomRotarySlider chorusrateSlider, phaserrateSlider, chorusdepthSlider, phaserdepthSlider, chorusfeedbackSlider, phaserfeedbackSlider, 
        chorusdelaySlider, phaserfreqSlider, mixSlider;

    juce::AudioProcessorValueTreeState::SliderAttachment chorusrateAttachment, phaserrateAttachment, chorusdepthAttachment,
        phaserdepthAttachment, chorusfeedbackAttachment, phaserfeedbackAttachment, chorusdelayAttachment, phaserfreqAttachment, mixAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AIPhaserAudioProcessorEditor)
};

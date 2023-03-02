/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AIPhaserAudioProcessorEditor::AIPhaserAudioProcessorEditor (AIPhaserAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), rateAttachment(audioProcessor.treestate, "rate", rateSlider),
    depthAttachment(audioProcessor.treestate, "depth", depthSlider), freqAttachment(audioProcessor.treestate, "frequency", freqSlider),
    feedbackAttachment(audioProcessor.treestate, "feedback", feedbackSlider), mixAttachment(audioProcessor.treestate, "mix", mixSlider)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    addAndMakeVisible(rateSlider);
    rateSlider.setTextValueSuffix(" Hz");
    rateSlider.setTextBoxIsEditable(true);

    addAndMakeVisible(depthSlider);
    depthSlider.setTextBoxIsEditable(true);

    addAndMakeVisible(feedbackSlider);
    feedbackSlider.setTextBoxIsEditable(true);

    addAndMakeVisible(freqSlider);
    freqSlider.setTextValueSuffix(" Hz");
    freqSlider.setTextBoxIsEditable(true);

    addAndMakeVisible(mixSlider);
    mixSlider.setTextValueSuffix(" %");
    mixSlider.setTextBoxIsEditable(true);


    setResizable(true, true);
    setResizeLimits(300, 225, 600, 400);
    getConstrainer()->setFixedAspectRatio(2.0);
    setSize(400, 300);
}

AIPhaserAudioProcessorEditor::~AIPhaserAudioProcessorEditor()
{
}

//==============================================================================
void AIPhaserAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    //g.setColour (juce::Colours::white);
    //g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void AIPhaserAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    auto x = 0.0;
    auto y = 0.0;
    auto bounds = getLocalBounds();
    auto width = bounds.getWidth();
    auto height = bounds.getHeight();
    auto w = width / 5;

    rateSlider.setBounds(x, y, w, height);
    depthSlider.setBounds(w, y, w, height);
    freqSlider.setBounds(2 * w, y, w, height);
    feedbackSlider.setBounds(3 * w, y, w, height);
    mixSlider.setBounds(4 * w, y, w, height);


}

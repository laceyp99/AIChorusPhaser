/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"



//==============================================================================
AIPhaserAudioProcessorEditor::AIPhaserAudioProcessorEditor (AIPhaserAudioProcessor& p)
    : AudioProcessorEditor (&p), 
    audioProcessor (p), chorusrateAttachment(audioProcessor.treestate, "chorus rate", chorusrateSlider),
    chorusdepthAttachment(audioProcessor.treestate, "chorus depth", chorusdepthSlider),
    chorusdelayAttachment(audioProcessor.treestate, "chorus delay", chorusdelaySlider),
    chorusfeedbackAttachment(audioProcessor.treestate, "chorus feedback", chorusfeedbackSlider),
    phaserrateAttachment(audioProcessor.treestate, "phaser rate", phaserrateSlider),
    phaserdepthAttachment(audioProcessor.treestate, "phaser depth", phaserdepthSlider),
    phaserfreqAttachment(audioProcessor.treestate, "phaser frequency", phaserfreqSlider),
    phaserfeedbackAttachment(audioProcessor.treestate, "phaser feedback", phaserfeedbackSlider),
    mixAttachment(audioProcessor.treestate, "mix", mixSlider)
{
    // chorus components
    addAndMakeVisible(chorusrateSlider);
    chorusrateSlider.setTextValueSuffix(" Hz");
    chorusrateSlider.setTextBoxIsEditable(true); 
    addAndMakeVisible(chorusdepthSlider);
    chorusdepthSlider.setTextBoxIsEditable(true);
    addAndMakeVisible(chorusfeedbackSlider);
    chorusfeedbackSlider.setTextBoxIsEditable(true);
    addAndMakeVisible(chorusdelaySlider);
    chorusdelaySlider.setTextValueSuffix(" ms");
    chorusdelaySlider.setTextBoxIsEditable(true);
    addAndMakeVisible(chorusdelayLabel);
    chorusdelayLabel.setText("Delay", juce::dontSendNotification);
    chorusdelayLabel.attachToComponent(&chorusdelaySlider, false);
    chorusdelayLabel.setJustificationType(juce::Justification::centredTop);

    // phaser components
    addAndMakeVisible(phaserrateSlider);
    phaserrateSlider.setTextValueSuffix(" Hz");
    phaserrateSlider.setTextBoxIsEditable(true);
    addAndMakeVisible(phaserdepthSlider);
    phaserdepthSlider.setTextBoxIsEditable(true);
    addAndMakeVisible(phaserfeedbackSlider);
    phaserfeedbackSlider.setTextBoxIsEditable(true);
    addAndMakeVisible(phaserfreqSlider);
    phaserfreqSlider.setTextValueSuffix(" Hz");
    phaserfreqSlider.setTextBoxIsEditable(true);
    addAndMakeVisible(phaserfreqLabel);
    phaserfreqLabel.setText("Frequency", juce::dontSendNotification);
    phaserfreqLabel.attachToComponent(&phaserfreqSlider, false);
    phaserfreqLabel.setJustificationType(juce::Justification::centredTop);

    // combo box effecting what components are visible
    addAndMakeVisible(effectbox);
    effectbox.addItem("Chorus", 1);
    effectbox.addItem("Phaser", 2);
    effectbox.onChange = [this]
    {
        if (effectbox.getSelectedId() == 1)
        {
            chorusrateSlider.setVisible(true);
            phaserrateSlider.setVisible(false);
            rateLabel.setVisible(true);
            chorusdepthSlider.setVisible(true);
            phaserdepthSlider.setVisible(false);
            depthLabel.setVisible(true);
            chorusdelaySlider.setVisible(true);
            chorusdelayLabel.setVisible(true);
            phaserfreqSlider.setVisible(false);
            phaserfreqLabel.setVisible(false);
            chorusfeedbackSlider.setVisible(true);
            phaserfeedbackSlider.setVisible(false);
            feedbackLabel.setVisible(true);

        }
        if (effectbox.getSelectedId() == 2)
        {
            chorusrateSlider.setVisible(false);
            phaserrateSlider.setVisible(true);
            rateLabel.setVisible(true);
            chorusdepthSlider.setVisible(false);
            phaserdepthSlider.setVisible(true);
            depthLabel.setVisible(true);
            chorusdelaySlider.setVisible(false);
            chorusdelayLabel.setVisible(false);
            phaserfreqSlider.setVisible(true);
            phaserfreqLabel.setVisible(true);
            chorusfeedbackSlider.setVisible(false);
            phaserfeedbackSlider.setVisible(true);
            feedbackLabel.setVisible(true);
        }
    };
    effectbox.setSelectedId(1);
    effectAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.treestate, "effect", effectbox);

    // mix slider for both the phaser and the chorus
    addAndMakeVisible(mixSlider);
    mixSlider.setTextValueSuffix(" %");
    mixSlider.setTextBoxIsEditable(true);
    addAndMakeVisible(mixLabel);
    mixLabel.setText("Mix", juce::dontSendNotification);
    mixLabel.attachToComponent(&mixSlider, false);
    mixLabel.setJustificationType(juce::Justification::centredTop);
    
    // labels
    addAndMakeVisible(rateLabel);
    rateLabel.setText("Rate", juce::dontSendNotification);
    rateLabel.attachToComponent(&chorusrateSlider, false);
    rateLabel.attachToComponent(&phaserrateSlider, false);
    rateLabel.setJustificationType(juce::Justification::centredTop);
    addAndMakeVisible(depthLabel);
    depthLabel.setText("Depth", juce::dontSendNotification);
    depthLabel.attachToComponent(&chorusdepthSlider, false);
    depthLabel.attachToComponent(&phaserdepthSlider, false);
    depthLabel.setJustificationType(juce::Justification::centredTop);
    addAndMakeVisible(feedbackLabel);
    feedbackLabel.setText("Feedback", juce::dontSendNotification);
    feedbackLabel.attachToComponent(&chorusfeedbackSlider, false);
    feedbackLabel.attachToComponent(&phaserfeedbackSlider, false);
    feedbackLabel.setJustificationType(juce::Justification::centredTop);

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
}

void AIPhaserAudioProcessorEditor::resized()
{
    auto x = 0.0;
    auto y = 0.0;
    auto bounds = getLocalBounds();
    auto width = bounds.getWidth();
    auto height = bounds.getHeight();
    auto w = width / 5;
    auto h1 = height * 0.1;
    auto h9 = height * 0.9;

    effectbox.setBounds(width * 0.25, y, width * 0.5, h1);
    
    chorusrateSlider.setBounds(x, h1 * 3, w, h1 * 6);
    chorusdepthSlider.setBounds(w, h1 * 3, w, h1 * 6);
    chorusdelaySlider.setBounds(2 * w, h1 * 3, w, h1 * 6);
    chorusfeedbackSlider.setBounds(3 * w, h1 * 3, w, h1 * 6);
    phaserrateSlider.setBounds(x, h1 * 3, w, h1 * 6);
    phaserdepthSlider.setBounds(w, h1 * 3, w, h1 * 6);
    phaserfreqSlider.setBounds(2 * w, h1 * 3, w, h1 * 6);
    phaserfeedbackSlider.setBounds(3 * w, h1 * 3, w, h1 * 6);
    mixSlider.setBounds(4 * w, h1 * 3, w, h1 * 6);
}


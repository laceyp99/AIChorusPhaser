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

    addAndMakeVisible(effectbox);
    effectbox.addItem("Chorus", 1);
    effectbox.addItem("Phaser", 2);
    
    effectbox.onChange = [this]
    {
        if (effectbox.getSelectedId() == 1)
        {
            chorusrateSlider.setVisible(true);
            phaserrateSlider.setVisible(false);
            chorusdepthSlider.setVisible(true);
            phaserdepthSlider.setVisible(false);
            chorusdelaySlider.setVisible(true);
            phaserfreqSlider.setVisible(false);
            chorusfeedbackSlider.setVisible(true);
            phaserfeedbackSlider.setVisible(false);
            

        }
        if (effectbox.getSelectedId() == 2)
        {
            chorusrateSlider.setVisible(false);
            phaserrateSlider.setVisible(true);
            chorusdepthSlider.setVisible(false);
            phaserdepthSlider.setVisible(true);
            chorusdelaySlider.setVisible(false);
            phaserfreqSlider.setVisible(true);
            chorusfeedbackSlider.setVisible(false);
            phaserfeedbackSlider.setVisible(true);

        }
    };
    
    effectbox.setSelectedId(1);
    effectAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.treestate, "effect", effectbox);

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
    
    chorusrateSlider.setBounds(x, h1, w, h9);
    chorusdepthSlider.setBounds(w, h1, w, h9);
    chorusdelaySlider.setBounds(2 * w, h1, w, h9);
    chorusfeedbackSlider.setBounds(3 * w, h1, w, h9);
    phaserrateSlider.setBounds(x, h1, w, h9);
    phaserdepthSlider.setBounds(w, h1, w, h9);
    phaserfreqSlider.setBounds(2 * w, h1, w, h9);
    phaserfeedbackSlider.setBounds(3 * w, h1, w, h9);
    mixSlider.setBounds(4 * w, h1, w, h9);

}


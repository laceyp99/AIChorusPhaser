/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AIPhaserAudioProcessor::AIPhaserAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), treestate(*this, nullptr, "PARMETERS", createParameterLayout())
#endif
{
    treestate.addParameterListener("effect", this);
    treestate.addParameterListener("chorus rate", this);
    treestate.addParameterListener("phaser rate", this);
    treestate.addParameterListener("chorus depth", this);
    treestate.addParameterListener("phaser depth", this);
    treestate.addParameterListener("chorus delay", this);
    treestate.addParameterListener("phaser frequency", this);
    treestate.addParameterListener("chorus feedback", this);
    treestate.addParameterListener("phaser feedback", this);
    treestate.addParameterListener("mix", this);
}

AIPhaserAudioProcessor::~AIPhaserAudioProcessor()
{
    treestate.removeParameterListener("effect", this);
    treestate.removeParameterListener("chorus rate", this);
    treestate.removeParameterListener("phaser rate", this);
    treestate.removeParameterListener("chorus depth", this);
    treestate.removeParameterListener("phaser depth", this);
    treestate.removeParameterListener("chorus delay", this);
    treestate.removeParameterListener("phaser frequency", this);
    treestate.removeParameterListener("chorus feedback", this);
    treestate.removeParameterListener("phaser feedback", this);
    treestate.removeParameterListener("mix", this);
}

juce::AudioProcessorValueTreeState::ParameterLayout AIPhaserAudioProcessor::createParameterLayout()
{
    std::vector < std::unique_ptr<juce::RangedAudioParameter >> params;

    juce::StringArray choices = { "Chorus", "Phaser" };

    auto pChoice = std::make_unique<juce::AudioParameterChoice>("effect", "Effect", choices, 0);

    auto pPRate = std::make_unique<juce::AudioParameterFloat>("phaser rate", "Phaser Rate", 0.1, 10.0, 0.5);
    auto pCRate = std::make_unique<juce::AudioParameterFloat>("chorus rate", "Chorus Rate", 0.1, 10.0, 1.0);

    auto pPDepth = std::make_unique<juce::AudioParameterFloat>("phaser depth", "Phaser Depth", 0.1, 0.9, 0.2);
    auto pCDepth = std::make_unique<juce::AudioParameterFloat>("chorus depth", "Chorus Depth", 0.1, 1.0, 0.2);

    auto pDelay = std::make_unique<juce::AudioParameterFloat>("chorus delay", "Chorus Delay", 10.0, 50.0, 20.0);
    auto pFreq = std::make_unique<juce::AudioParameterFloat>("phaser frequency", "Phaser Frequency", 100.0, 2000.0, 1000.0);

    auto pPFeedback = std::make_unique<juce::AudioParameterFloat>("phaser feedback", "Phaser Feedback", 0.1, 0.9, 0.9);
    auto pCFeedback = std::make_unique<juce::AudioParameterFloat>("chorus feedback", "Chorus Feedback", 0.0, 0.9, 0.1);

    auto pMix = std::make_unique<juce::AudioParameterFloat>("mix", "Mix", 0.0, 100.0, 100.0);

    params.push_back(std::move(pChoice));
    params.push_back(std::move(pPRate));
    params.push_back(std::move(pCRate));
    params.push_back(std::move(pPDepth));
    params.push_back(std::move(pCDepth));
    params.push_back(std::move(pDelay));
    params.push_back(std::move(pFreq));
    params.push_back(std::move(pCFeedback));
    params.push_back(std::move(pPFeedback));
    params.push_back(std::move(pMix));

    return { params.begin(), params.end() };

}

void AIPhaserAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    if (parameterID == "effect")
    {
        cpp.setEffect(newValue);
    }
    if (parameterID == "chorus rate")
    {
        cpp.setChorusRate(newValue);
    }
    if (parameterID == "chorus depth")
    {
        cpp.setChorusDepth(newValue);
    }
    if (parameterID == "chorus delay")
    {
        cpp.setChorusDelay(newValue);
    }
    if (parameterID == "chorus feedback")
    {
        cpp.setChorusFeedback(newValue);
    }
    if (parameterID == "phaser rate")
    {
        cpp.setPhaserRate(newValue);
    }
    if (parameterID == "phaser depth")
    {
        cpp.setPhaserDepth(newValue);
    }
    if (parameterID == "phaser frequency")
    {
        cpp.setPhaserFreq(newValue);
    }
    if (parameterID == "phaser feedback")
    {
        cpp.setPhaserFeedback(newValue);
    }
    if (parameterID == "mix")
    {
        cpp.setMix(newValue / 100.0);
    }
}

//==============================================================================
const juce::String AIPhaserAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AIPhaserAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AIPhaserAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AIPhaserAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AIPhaserAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AIPhaserAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AIPhaserAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AIPhaserAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String AIPhaserAudioProcessor::getProgramName (int index)
{
    return {};
}

void AIPhaserAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void AIPhaserAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    spec.sampleRate = sampleRate;

    cpp.prepareToPlay(spec);
    
    // linking treestate parameters to ChatGPT generated Phaser/Chorus parameters

    float fxchoice = *treestate.getRawParameterValue("effect");
    float prate = (*treestate.getRawParameterValue("phaser rate"));
    float pdepth = (*treestate.getRawParameterValue("phaser depth"));
    float pfreq = (*treestate.getRawParameterValue("phaser frequency"));
    float pfeedback = (*treestate.getRawParameterValue("phaser feedback"));
    float mix = (*treestate.getRawParameterValue("mix") / 100.0);

    float crate = *treestate.getRawParameterValue("chorus rate");
    float cdepth = *treestate.getRawParameterValue("chorus depth");
    float cdelay = *treestate.getRawParameterValue("chorus delay");
    float cfeedback = *treestate.getRawParameterValue("chorus feedback");

    if (fxchoice == 0.0f)
    {
        cpp.setChorusRate(crate);
        cpp.setChorusDepth(cdepth);
        cpp.setChorusDelay(cdelay);
        cpp.setChorusFeedback(cfeedback);
        cpp.setMix(mix);
    }
    if (fxchoice == 1.0f)
    {
        cpp.setPhaserRate(prate);
        cpp.setPhaserDepth(pdepth);
        cpp.setPhaserFreq(pfreq);
        cpp.setPhaserFeedback(pfeedback);
        cpp.setMix(mix); 
    }
}

void AIPhaserAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AIPhaserAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void AIPhaserAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // id normally make an audio block and pass the context within process functions, but ChatGPT's function calls for the AudioBuffer
    // which makes this processBlock code super simple
    cpp.processAudio(buffer);
    
}

//==============================================================================
bool AIPhaserAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AIPhaserAudioProcessor::createEditor()
{
    return new AIPhaserAudioProcessorEditor (*this);
    //return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void AIPhaserAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream stream(destData, false);
    treestate.state.writeToStream(stream);
}

void AIPhaserAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto tree = juce::ValueTree::readFromData(data, size_t(sizeInBytes));

    if (tree.isValid())
    {
        treestate.state = tree;
        cpp.setEffect(*treestate.getRawParameterValue("effect"));
        cpp.setPhaserRate(*treestate.getRawParameterValue("phaser rate"));
        cpp.setChorusRate(*treestate.getRawParameterValue("chorus rate"));
        cpp.setPhaserDepth(*treestate.getRawParameterValue("phaser depth"));
        cpp.setChorusDepth(*treestate.getRawParameterValue("chorus depth"));
        cpp.setPhaserFreq(*treestate.getRawParameterValue("phaser frequency"));
        cpp.setChorusDelay(*treestate.getRawParameterValue("chorus delay"));
        cpp.setChorusFeedback(*treestate.getRawParameterValue("chorus feedback"));
        cpp.setPhaserFeedback(*treestate.getRawParameterValue("phaser feedback"));
        cpp.setMix(*treestate.getRawParameterValue("mix") / 100.0);
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AIPhaserAudioProcessor();
}

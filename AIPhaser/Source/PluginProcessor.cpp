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
    treestate.addParameterListener("rate", this);
    treestate.addParameterListener("depth", this);
    treestate.addParameterListener("frequency", this);
    treestate.addParameterListener("feedback", this);
    treestate.addParameterListener("mix", this);
}

AIPhaserAudioProcessor::~AIPhaserAudioProcessor()
{
    treestate.removeParameterListener("rate", this);
    treestate.removeParameterListener("depth", this);
    treestate.removeParameterListener("frequency", this);
    treestate.removeParameterListener("feedback", this);
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

    if (parameterID == "chorus rate")
    {
        cpp.setRate(newValue);
    }
    if (parameterID == "chorus depth")
    {
        cpp.setDepth(newValue);
    }
    if (parameterID == "chorus delay")
    {
        cpp.setCentre(newValue);
    }
    if (parameterID == "chorus feedback")
    {
        cpp.setFeedback(newValue);
    }

    if (parameterID == "phaser rate")
    {
        cpp.setRate(newValue);
    }
    if (parameterID == "phaser depth")
    {
        cpp.setDepth(newValue);
    }
    if (parameterID == "phaser frequency")
    {
        cpp.setCentre(newValue);
    }
    if (parameterID == "phaser feedback")
    {
        cpp.setFeedback(newValue);
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
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    spec.sampleRate = sampleRate;

    //phaser.prepare(spec);

    /*
    cpp.chain.get<0>().setRate(chorus.getRate());
    cpp.get<0>().setDepth(chorus.getDepth());
    chain.get<0>().setCentreDelay(chorus.getCentreDelay());
    chain.get<0>().setFeedback(chorus.getFeedback());
    chain.get<1>().setRate(phaser.getRate());
    chain.get<1>().setDepth(phaser.getDepth());
    chain.get<1>().setCentreFrequency(phaser.getCentreFrequency());
    chain.get<1>().setFeedback(phaser.getFeedback());
    */
    //cpp.setChorus();

    float prate = (*treestate.getRawParameterValue("phaser rate"));
    float pdepth = (*treestate.getRawParameterValue("phaser depth"));
    float pfreq = (*treestate.getRawParameterValue("phaser frequency"));
    float pfeedback = (*treestate.getRawParameterValue("phaser feedback"));
    float mix = (*treestate.getRawParameterValue("mix") / 100.0);

    float crate = *treestate.getRawParameterValue("chorus rate");
    float cdepth = *treestate.getRawParameterValue("chorus depth");
    float cdelay = *treestate.getRawParameterValue("chorus delay");
    float cfeedback = *treestate.getRawParameterValue("chorus feedback");

    if (*treestate.getRawParameterValue("effect") == 0.0f)
    {
        cpp.setRate(crate);
        cpp.setDepth(cdepth);
        cpp.setCentre(cdelay);
        cpp.setFeedback(cfeedback);
        cpp.setMix(mix);
    }
    if (*treestate.getRawParameterValue("effect") == 1.0f)
    {
        cpp.setRate(prate);
        cpp.setDepth(pdepth);
        cpp.setCentre(pfreq);
        cpp.setFeedback(pfeedback);
        cpp.setMix(mix); 
    }

    //cpp.setPhaser(prate, pdepth, pfreq, pfeedback, pmix);

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

    //phaser.processAudio(buffer);
    cpp.processAudio(buffer);
    
}

//==============================================================================
bool AIPhaserAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AIPhaserAudioProcessor::createEditor()
{
    //return new AIPhaserAudioProcessorEditor (*this);
    return new juce::GenericAudioProcessorEditor(*this);
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

        cpp.setRate(*treestate.getRawParameterValue("phaser rate"));
        cpp.setRate(*treestate.getRawParameterValue("chorus rate"));
        cpp.setDepth(*treestate.getRawParameterValue("phaser depth"));
        cpp.setDepth(*treestate.getRawParameterValue("chorus depth"));
        cpp.setCentre(*treestate.getRawParameterValue("phaser frequency"));
        cpp.setCentre(*treestate.getRawParameterValue("chorus delay"));
        cpp.setFeedback(*treestate.getRawParameterValue("chorus feedback"));
        cpp.setFeedback(*treestate.getRawParameterValue("phaser feedback"));
        cpp.setMix(*treestate.getRawParameterValue("mix") / 100.0);
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AIPhaserAudioProcessor();
}

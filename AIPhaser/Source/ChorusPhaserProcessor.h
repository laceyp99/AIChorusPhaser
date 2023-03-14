#pragma once
#include <JuceHeader.h>

class ChorusPhaserProcessor
{
public:
    ChorusPhaserProcessor() : currentEffectIndex(0)
    {
        chain.get<0>().setRate(1.0f);
        chain.get<0>().setDepth(0.2f);
        chain.get<0>().setCentreDelay(20.0f);
        chain.get<0>().setFeedback(0.1f);
        chain.get<0>().setMix(1.0f);
    }

    void prepareToPlay(const juce::dsp::ProcessSpec& spec)
    {
        chain.prepare(spec);
        chain.reset();
    }

   
    void processAudio(juce::AudioBuffer<float>& buffer)
    {
        juce::dsp::AudioBlock<float> block(buffer);
        juce::dsp::ProcessContextReplacing<float> context(block);


        // Process the audio with the current effect
        if (currentEffectIndex == 0)
        {
            chain.get<0>().process(context);
        }
        else
        {
            chain.get<1>().process(context);
        }
    }
    
    // all these functions were added myself to pass parameters into this class from the treestate
    void setEffect(int effectIndex)
    {
        currentEffectIndex = effectIndex;
    }

    void setChorusRate(float rate)
    {
        chain.get<0>().setRate(rate);
    }

    void setPhaserRate(float rate)
    {
        chain.get<1>().setRate(rate);
    }

    void setChorusDepth(float depth)
    {
        chain.get<0>().setDepth(depth);
    }

    void setPhaserDepth(float depth)
    {
        chain.get<1>().setDepth(depth);
    }

    void setChorusDelay(float centre)
    {
        chain.get<0>().setCentreDelay(centre);
    }

    void setPhaserFreq(float centre)
    {
        chain.get<1>().setCentreFrequency(centre);
    }

    void setChorusFeedback(float feedback)
    {
        chain.get<0>().setFeedback(feedback);
    }

    void setPhaserFeedback(float feedback)
    {
        chain.get<1>().setFeedback(feedback);
    }

    // since i had Chat GPT decide all my initial values and ranges for the parameters, the mix was the only one with
    // the same numbers for both effects (100 and 0-100) so this was the only function that uses the currentEffectIndex to 
    // choose which effect the mix is for
    void setMix(float mix)
    {
        if (currentEffectIndex == 0)
        {
            chain.get<0>().setMix(mix);
        }
        if (currentEffectIndex == 1)
        {
            chain.get<1>().setMix(mix);
        }
    }

private:
    
    int currentEffectIndex;

    juce::dsp::ProcessorChain<juce::dsp::Chorus<float>, juce::dsp::Phaser<float>> chain;
};

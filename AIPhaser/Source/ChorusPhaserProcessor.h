#pragma once
#include <JuceHeader.h>

class ChorusPhaserProcessor
{
public:
    ChorusPhaserProcessor() : currentEffectIndex(0)
    {
        rightchain.get<0>().setRate(1.0f);
        rightchain.get<0>().setDepth(0.2f);
        rightchain.get<0>().setCentreDelay(20.0f);
        rightchain.get<0>().setFeedback(0.1f);
        rightchain.get<0>().setMix(1.0f);

        leftchain.get<1>().setRate(0.5f);
        leftchain.get<1>().setDepth(0.2f);
        leftchain.get<1>().setCentreFrequency(1000.0f);
        leftchain.get<1>().setFeedback(0.9f);
        leftchain.get<1>().setMix(1.0f);
    }

    void prepareToPlay(const juce::dsp::ProcessSpec& spec)
    {
        leftchain.prepare(spec);
        rightchain.prepare(spec);
        leftchain.reset();
        rightchain.reset();
    }

   
    void processAudio(juce::AudioBuffer<float>& buffer)
    {
        juce::dsp::AudioBlock<float> block(buffer);
        juce::dsp::ProcessContextReplacing<float> context(block);


        // Process the audio with the current effect
        if (currentEffectIndex == 0)
        {
            leftchain.get<0>().process(context);
            rightchain.get<0>().process(context);
        }
        else
        {
            leftchain.get<1>().process(context);
            rightchain.get<1>().process(context);
        }
    }

    void setEffect(int effectIndex)
    {
        currentEffectIndex = effectIndex;
    }

    void setChorusRate(float rate)
    {
        leftchain.get<0>().setRate(rate);
        rightchain.get<0>().setRate(rate);
    }

    void setPhaserRate(float rate)
    {
        leftchain.get<1>().setRate(rate);
        rightchain.get<1>().setRate(rate);
    }

    void setChorusDepth(float depth)
    {
        leftchain.get<0>().setDepth(depth);
        rightchain.get<0>().setDepth(depth);
    }

    void setPhaserDepth(float depth)
    {
        leftchain.get<1>().setDepth(depth);
        rightchain.get<1>().setDepth(depth);
    }

    void setChorusDelay(float centre)
    {
        leftchain.get<0>().setCentreDelay(centre);
        rightchain.get<0>().setCentreDelay(centre);
    }

    void setPhaserFreq(float centre)
    {
        leftchain.get<1>().setCentreFrequency(centre);
        rightchain.get<1>().setCentreFrequency(centre);
    }

    void setChorusFeedback(float feedback)
    {
        leftchain.get<0>().setFeedback(feedback);
        rightchain.get<0>().setFeedback(feedback);
    }

    void setPhaserFeedback(float feedback)
    {
        leftchain.get<1>().setFeedback(feedback);
        rightchain.get<1>().setFeedback(feedback);
    }

    void setMix(float mix)
    {
        if (currentEffectIndex == 0)
        {
            leftchain.get<0>().setMix(mix);
            rightchain.get<0>().setMix(mix);
        }
        if (currentEffectIndex == 1)
        {
            leftchain.get<1>().setMix(mix);
            rightchain.get<1>().setMix(mix);
        }
    }

private:
    
    int currentEffectIndex;

    using Monochain = juce::dsp::ProcessorChain<juce::dsp::Chorus<float>, juce::dsp::Phaser<float>>;
    Monochain leftchain, rightchain;
};

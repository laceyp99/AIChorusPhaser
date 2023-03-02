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
        rightchain.get<0>().setMix(100.0f);

        leftchain.get<1>().setRate(0.5f);
        leftchain.get<1>().setDepth(0.2f);
        leftchain.get<1>().setCentreFrequency(1000.0f);
        leftchain.get<1>().setFeedback(0.9f);
        leftchain.get<1>().setMix(100.0f);
    }

    void prepareToPlay(double sampleRate, int samplesPerBlock)
    {
        juce::dsp::ProcessSpec spec;
        spec.sampleRate = sampleRate;
        spec.maximumBlockSize = samplesPerBlock;
        spec.numChannels = 1;

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

    void setRate(float rate)
    {
        if (currentEffectIndex == 0)
        {
            leftchain.get<0>().setRate(rate);
            rightchain.get<0>().setRate(rate);
        }
        else
        {
            leftchain.get<1>().setRate(rate);
            rightchain.get<1>().setRate(rate);
        }
    }

    void setDepth(float depth)
    {
        if (currentEffectIndex == 0)
        {
            leftchain.get<0>().setDepth(depth);
            rightchain.get<0>().setDepth(depth);
        }
        else
        {
            leftchain.get<1>().setDepth(depth);
            rightchain.get<1>().setDepth(depth);
        }
    }

    void setCentre(float centre)
    {
        if (currentEffectIndex == 0)
        {
            leftchain.get<0>().setCentreDelay(centre);
            rightchain.get<0>().setCentreDelay(centre);
        }
        else
        {
            leftchain.get<1>().setCentreFrequency(centre);
            rightchain.get<1>().setCentreFrequency(centre);
        }
    }

    void setFeedback(float feedback)
    {
        if (currentEffectIndex == 0)
        {
            leftchain.get<0>().setFeedback(feedback);
            rightchain.get<0>().setFeedback(feedback);
        }
        else
        {
            leftchain.get<1>().setFeedback(feedback);
            rightchain.get<1>().setFeedback(feedback);
        }
    }

    void setMix(float mix)
    {
        if (currentEffectIndex == 0)
        {
            leftchain.get<0>().setMix(mix);
            rightchain.get<0>().setMix(mix);
        }
        else
        {
            leftchain.get<1>().setMix(mix);
            rightchain.get<1>().setMix(mix);
        }
    }


    /*
    void setChorus(float rate, float depth, float delay, float feedback, float mix)
    {
        leftchain.get<0>().setRate(rate);
        leftchain.get<0>().setDepth(depth);
        leftchain.get<0>().setCentreDelay(delay);
        leftchain.get<0>().setFeedback(feedback);
        leftchain.get<0>().setMix(mix);

        rightchain.get<0>().setRate(rate);
        rightchain.get<0>().setDepth(depth);
        rightchain.get<0>().setCentreDelay(delay);
        rightchain.get<0>().setFeedback(feedback);
        rightchain.get<0>().setMix(mix);
    }

    void setPhaser(float rate, float depth, float freq, float feedback, float mix)
    {
        leftchain.get<1>().setRate(rate);
        leftchain.get<1>().setDepth(depth);
        leftchain.get<1>().setCentreFrequency(freq);
        leftchain.get<1>().setFeedback(feedback);
        leftchain.get<1>().setMix(mix);

        rightchain.get<1>().setRate(rate);
        rightchain.get<1>().setDepth(depth);
        rightchain.get<1>().setCentreFrequency(freq);
        rightchain.get<1>().setFeedback(feedback);
        rightchain.get<1>().setMix(mix);
    }
    */
private:
    
    int currentEffectIndex;

    using Monochain = juce::dsp::ProcessorChain<juce::dsp::Chorus<float>, juce::dsp::Phaser<float>>;
    Monochain leftchain, rightchain;
};

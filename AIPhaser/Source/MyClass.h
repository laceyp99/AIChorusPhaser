#pragma once

#include <JuceHeader.h>

// Declare a `juce::dsp::Phaser` object as a member variable in your class
class MyClass
{
public:
    MyClass()
    {
        // Initialize the phaser with the desired parameters
        phaser.setRate(0.5);
        phaser.setDepth(0.2);
        phaser.setCentreFrequency(1000.0);
        phaser.setFeedback(0.9);
    }

    void prepare(const juce::dsp::ProcessSpec& spec)
    {
        phaser.reset();
        phaser.prepare(spec);
    }

    // Process audio using the phaser
    void processAudio(juce::AudioBuffer<float>& buffer)
    {
        juce::dsp::AudioBlock<float> block(buffer);
        juce::dsp::ProcessContextReplacing<float> context(block);
        phaser.process(context);
    }
    /*
    insted of making separate calls pass a vector of params to the header file, but it might be more complex to implement
    void setParameters(std::vector<float> phaserparams)
    {
        phaser.setRate(phaserparams[0]);
        phaser.setDepth(phaserparams[1]);
        phaser.setCentreFrequency(phaserparams[2]);
        phaser.setFeedback(phaserparams[3]);
        phaser.setMix(phaserparams[4]);

    }
    */
    void setRate(float rate) { phaser.setRate(rate); }

    void setDepth(float depth) { phaser.setDepth(depth); }

    void setFreq(float freq) { phaser.setCentreFrequency(freq); }

    void setFeedback(float feedback) { phaser.setFeedback(feedback); }

    void setMix(float mix) { phaser.setMix(mix); }

private:
    juce::dsp::Phaser<float> phaser;
};

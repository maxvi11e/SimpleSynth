/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_core/juce_core.h>
#include <juce_audio_basics/juce_audio_basics.h>


//==============================================================================
/**
*/

// Add a basic sine wave synthesiser voice
struct SineWaveVoice : public juce::SynthesiserVoice
{
SineWaveVoice() : phase(0.0), level(0.0), phaseDelta(0.0) {}

bool canPlaySound(juce::SynthesiserSound* sound) override
{
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound*, int) override
{
    level = velocity * 0.15;
    phaseDelta = 2.0 * juce::MathConstants<double>::pi * juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) / getSampleRate();
    adsr.noteOn();
}

void stopNote(float, bool allowTailOff) override
{
    if (allowTailOff)
        {
            adsr.noteOff();
        }
    else
        {
            adsr.noteOff();
            adsr.reset();
            clearCurrentNote();
        }
}

void pitchWheelMoved(int) override {}
void controllerMoved(int, int) override {}

void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
{
    while (--numSamples >= 0)
    {
        phase += phaseDelta;
        // float currentSample = (float)(std::sin(phase) * level);
        float currentSample = (float)(std::sin(phase) * level * adsr.getNextSample());

        for (int i = 0; i < outputBuffer.getNumChannels(); ++i)
        {
            outputBuffer.addSample(i, startSample, currentSample);
        }

        ++startSample;
    }
}

public:
double phase;
double level;
double phaseDelta;

juce::ADSR adsr;
juce::ADSR::Parameters adsrParams;
};


class SimpleSynthAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    SimpleSynthAudioProcessor();
    ~SimpleSynthAudioProcessor() override;

    //==============================================================================
    
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;



    juce::Synthesiser synth;
    juce::ADSR::Parameters adsrParams;




private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleSynthAudioProcessor)
};

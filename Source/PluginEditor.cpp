/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleSynthAudioProcessorEditor::SimpleSynthAudioProcessorEditor (SimpleSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    attackSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    attackSlider.setRange(0.0, 5.0, 0.01);
    attackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    attackSlider.onValueChange = [this] { sliderValueChanged(&attackSlider); };
    addAndMakeVisible(attackSlider);

    decaySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    decaySlider.setRange(0.0, 10.0, 0.01);
    decaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    decaySlider.onValueChange = [this] { sliderValueChanged(&decaySlider); };
    addAndMakeVisible(decaySlider);

    sustainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    sustainSlider.setRange(0.0, 5.0, 0.01);
    sustainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    sustainSlider.onValueChange = [this] { sliderValueChanged(&sustainSlider); };
    addAndMakeVisible(sustainSlider);

    releaseSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    releaseSlider.setRange(0.0, 5.0, 0.01);
    releaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    releaseSlider.onValueChange = [this] { sliderValueChanged(&releaseSlider); };
    addAndMakeVisible(releaseSlider);
    
    releaseLabel.setText("Release", juce::dontSendNotification);
    releaseLabel.attachToComponent(&releaseSlider, false);
    addAndMakeVisible(releaseLabel);
    
}

SimpleSynthAudioProcessorEditor::~SimpleSynthAudioProcessorEditor()
{
}

//==============================================================================
void SimpleSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    // g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void SimpleSynthAudioProcessorEditor::resized()
{
    attackSlider.setBounds(10, 10, 60, getHeight() - 60);
    decaySlider.setBounds(80, 10, 60, getHeight() - 60);
    sustainSlider.setBounds(150, 10, 60, getHeight() - 60);
    releaseSlider.setBounds(220, 10, 60, getHeight() - 60);
}

void SimpleSynthAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &attackSlider)
        audioProcessor.adsrParams.attack = attackSlider.getValue();
    else if (slider == &decaySlider)
        audioProcessor.adsrParams.decay = decaySlider.getValue();
    else if (slider == &sustainSlider)
        audioProcessor.adsrParams.sustain = sustainSlider.getValue();
    else if (slider == &releaseSlider)
        audioProcessor.adsrParams.release = releaseSlider.getValue();

    // Update the synthesiser voice(s) with the new ADSR parameters
    for (int i = 0; i < audioProcessor.synth.getNumVoices(); ++i)
    {
        if (auto* voice = dynamic_cast<SineWaveVoice*>(audioProcessor.synth.getVoice(i)))
        {
            voice->adsr.setParameters(audioProcessor.adsrParams);
        }
    }
}



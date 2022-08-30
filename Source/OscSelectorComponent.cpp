/*
  ==============================================================================

    OscSelectorComboBox.cpp
    Created: 16 Jun 2022 1:31:14pm
    Author:  Kimberly Young

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OscSelectorComponent.h"

//==============================================================================
OscSelectorComboBox::OscSelectorComboBox(juce::AudioProcessorValueTreeState& apvts)
{

    //making the oscillator selector attatched between the editor and the OSC value tree in the processor
    addAndMakeVisible(oscSelector);
    oscSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, "OSC", oscSelector);
}

OscSelectorComboBox::~OscSelectorComboBox()
{
}

void OscSelectorComboBox::paint (juce::Graphics& g)
{

    g.fillAll (juce::Colours::transparentBlack);
}

void OscSelectorComboBox::resized()
{
 
    oscSelector.setBoundsRelative(0.0, 0.0, 1.0, 1.0);

}



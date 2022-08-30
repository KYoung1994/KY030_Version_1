/*
  ==============================================================================

    OscSelectorComboBox.cpp
    Created: 16 Jun 2022 1:31:14pm
    Author:  Kimberly Young

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DivSelector.h"

//==============================================================================

//An object that is a combo box to control the division of the note values
DivSelectorComboBox::DivSelectorComboBox(juce::AudioProcessorValueTreeState& apvts)
{

    juce::StringArray choices {"Four", "Eight", "Sixteen"};
    divSelector.addItemList(choices, 1);
    
    addAndMakeVisible(divSelector);
    
    divSelector.setColour(0x1000b00, juce::Colours::silver);
    
    divSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, "BARDIV", divSelector);
}

DivSelectorComboBox::~DivSelectorComboBox()
{
}

void DivSelectorComboBox::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);
}

void DivSelectorComboBox::resized()
{

    divSelector.setBoundsRelative(0.0, 0.0, 1.0, 1.0);

}

void DivSelectorComboBox::setUpComboBox(){
    
}

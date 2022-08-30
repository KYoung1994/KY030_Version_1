/*
  ==============================================================================

    NoteSelectorComboBox.cpp
    Created: 26 Jun 2022 9:28:26pm
    Author:  Kimberly Young

  ==============================================================================
*/

#include <JuceHeader.h>
#include "NoteSelectorComponent.h"

//Note selector cpp file. Constructor requires 3 attachmentIDs to be able to attach to the appropiate value tree in the processor class

//==============================================================================
NoteSelectorComboBox::NoteSelectorComboBox(juce::String attatchmentID, juce::String accentAttachmentID, juce::String glideAttachmentID, juce::AudioProcessorValueTreeState& apvts)
{
    // Constructing the attachments, the visual component combo boxes, and the film strip buttons for the UI. Each object of this instance will be one note, one accent button below on the left, and one glide button below on the right
    noteSelector.addItemList(choices, 1);
    noteSelector.setTextWhenNothingSelected("NONE");
    noteSelector.setColour(0x1000b00, juce::Colours::silver);
    noteSelector.setColour(0x1000f00, juce::Colours::silver);

    accentImage = ImageCache::getFromMemory(BinaryData::accentpng_png, BinaryData::accentpng_pngSize);
    glideImage = ImageCache::getFromMemory(BinaryData::glidepng_png, BinaryData::glidepng_pngSize);
    
    addAndMakeVisible(noteSelector);
    addAndMakeVisible(onOffButton);
    onOffButton.setToggleState(false, juce::dontSendNotification);
    onOffButton.setButtonText("Accent");
    onOffButton.setColour(0x1006501, juce::Colours::black);
    onOffButton.setColour(0x1006502, juce::Colours::black);
    onOffButton.setColour(0x1006503, juce::Colours::black);
    
    addAndMakeVisible(glideButton);
    glideButton.setToggleState(false, juce::dontSendNotification);
    glideButton.setButtonText("Glide");
    
    noteSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, attatchmentID, noteSelector);
    
    accentAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, accentAttachmentID, onOffButton);
    
    glideAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, glideAttachmentID, glideButton);
    
    
}

NoteSelectorComboBox::~NoteSelectorComboBox()
{
}

void NoteSelectorComboBox::paint (juce::Graphics& g)
{
    //Lables for each of the buttons
    g.fillAll (juce::Colours::transparentBlack);
    g.drawImage(accentImage, Rectangle<float>(2, 46, 30, 10));
    g.drawImage(glideImage, Rectangle<float>(47, 46, 24, 9));

}



void NoteSelectorComboBox::resized()
{
    
    noteSelector.setBoundsRelative(0.0, 0.0, 1.0, 0.3);
    onOffButton.setBoundsRelative(0.105, 0.35, 0.2, 0.9);
    glideButton.setBoundsRelative(0.64, 0.35, 0.2, 0.9);


}

void NoteSelectorComboBox::changeValue(int newId){
    
    noteSelector.setSelectedId(newId);
    
}

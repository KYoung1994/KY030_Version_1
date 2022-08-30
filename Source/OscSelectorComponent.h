/*
  ==============================================================================

    OscSelectorComboBox.h
    Created: 16 Jun 2022 1:31:14pm
    Author:  Kimberly Young

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "FilmStripTog.h"

//==============================================================================
/*
*/

//header file for the oscillator selector that appears in the UI, the appearance of the oscillator will come from the FilmStripTog object, and the Attachment will attach it to the OSC value tree in the processor
class OscSelectorComboBox  : public juce::Component
{
public:
    OscSelectorComboBox(juce::AudioProcessorValueTreeState& apvts);
    ~OscSelectorComboBox() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    typedef std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> ComboBoxAttachment;
    
    typedef std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> ButtonAttachment;
private:
    
    FilmStripTog oscSelector;
    
    ButtonAttachment oscSelectorAttachment;
    

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscSelectorComboBox)
};

/*
  ==============================================================================

    OscSelectorComboBox.h
    Created: 16 Jun 2022 1:31:14pm
    Author:  Kimberly Young

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
//An object that is a combo box to control the division of the note values
class DivSelectorComboBox  : public juce::Component
{
public:
    DivSelectorComboBox(juce::AudioProcessorValueTreeState& apvts);
    ~DivSelectorComboBox() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    typedef std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> ComboBoxAttachment;

private:
    
    juce::ComboBox divSelector;
    
    ComboBoxAttachment divSelectorAttachment;
    
    void setUpComboBox();
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DivSelectorComboBox)
};

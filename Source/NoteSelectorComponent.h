/*
  ==============================================================================

    NoteSelectorComboBox.h
    Created: 26 Jun 2022 9:28:26pm
    Author:  Kimberly Young

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "FilmStripButton.h"

//==============================================================================
/*
*/
//Header file for the note selector combo box. Each object of this instance will be one note, one accent button below on the left, and one glide button below on the right
class NoteSelectorComboBox  : public juce::Component
{
public:
    NoteSelectorComboBox(juce::String attatchmentID, juce::String accentAttatchmentID, juce::String glideAttachmentID, juce::AudioProcessorValueTreeState& apvts);
    ~NoteSelectorComboBox() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void changeValue(int newValue);

    typedef std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> ComboBoxAttachment;
    typedef std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> ButtonAttachment;
    
    juce::ComboBox noteSelector;
    
    FilmStripButton onOffButton, glideButton;
 
    juce::Image accentImage, glideImage;

    ComboBoxAttachment noteSelectorAttachment;
    ButtonAttachment accentAttachment, glideAttachment;
   
    
    //Notes available for selection
    juce::StringArray choices{"NONE", "B0", "C0", "C#0", "D0", "D#0", "E0", "F0", "F#0", "G0", "G#0", "A1", "A#1", "B1", "C1", "C#1", "D1", "D#1", "E1", "F1", "F#1", "G1", "G#1", "A2", "A#2", "B2", "C2", "C#2", "D2", "D#2", "E2", "F2", "F#2", "G2", "G#2", "A3", "A#3", "B3", "C3", "C#3", "D3", "D#3", "E3", "F3", "F#3", "G3", "G#3", "A4", "A#4", "B4", "C4"};
    
    
    
    
private:

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NoteSelectorComboBox)
};

/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once


#include "PluginProcessor.h"
#include <JuceHeader.h>
#include "adsrComponent.h"
#include "OscSelectorComponent.h"
#include "DivSelector.h"
#include "NoteSelectorComponent.h"
#include "Filmstrip.h"
#include "PresetPanel.h"

//==============================================================================
/**
*/
class KY030AudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    KY030AudioProcessorEditor (KY030AudioProcessor&);
    ~KY030AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
     typedef std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachment;
        
        
    void setUpSlider(juce::Slider& slider, std::string labelName, juce::Label& label);
    void loadParams();

private:

    KY030AudioProcessor& audioProcessor;
        
    Gui::PresetPanel presetPanel;
    
    juce::Image squareDrawing, sawDrawing, ky303title, accentFontOne;

    OscSelectorComboBox oscComboBox;
    
    juce::TextButton randomiser{"Random"}, tunefulRandomiser{"Randomiser"}, clearNotes{"Clear"};
    
    DivSelectorComboBox divisionComboBox;
    NoteSelectorComboBox noteComboBoxOne, noteComboBoxTwo,noteComboBoxThree, noteComboBoxFour, noteComboBoxFive, noteComboBoxSix, noteComboBoxSeven, noteComboBoxEight, noteComboBoxNine, noteComboBoxTen, noteComboBoxEleven, noteComboBoxTwelve, noteComboBoxThirteen, noteComboBoxFourteen, noteComboBoxFifteen, noteComboBoxSixteen ;

    
    FilmStripKnob cutoffSlider, resonanceSlider, gainSlider, tuneSlider, EnvModSlider, EnvDecaySlider, AccentSlider ;
     juce::Label cutoffLabel, resonanceLabel, gainLabel, tuneLabel, EnvModLabel, EnvDecayLabel,AccentLabel  ;

     
     sliderAttachment cuttoffAttachment,resonanceAttachment, gainAttachment, tuneAttachment, EnvModAttachment, EnvDecayAttachment, AccentAttachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KY030AudioProcessorEditor)
};

/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
KY030AudioProcessorEditor::KY030AudioProcessorEditor (KY030AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), presetPanel(p.getPresetManager()), oscComboBox(audioProcessor.apvts), divisionComboBox(audioProcessor.apvts), noteComboBoxOne("NOTEONE", "ACSIXTEEN", "GLIDESIXTEEN", audioProcessor.apvts), noteComboBoxTwo("NOTETWO", "ACONE","GLIDEONE", audioProcessor.apvts), noteComboBoxThree("NOTETHREE", "ACTWO", "GLIDETWO",audioProcessor.apvts), noteComboBoxFour("NOTEFOUR", "ACTHREE","GLIDETHREE", audioProcessor.apvts), noteComboBoxFive("NOTEFIVE", "ACFOUR","GLIDEFOUR", audioProcessor.apvts), noteComboBoxSix("NOTESIX", "ACFIVE", "GLIDEFIVE",audioProcessor.apvts), noteComboBoxSeven("NOTESEVEN", "ACSIX" ,"GLIDESIX", audioProcessor.apvts), noteComboBoxEight("NOTEEIGHT","ACSEVEN", "GLIDESEVEN",audioProcessor.apvts), noteComboBoxNine("NOTENINE","ACEIGHT","GLIDEEIGHT",audioProcessor.apvts), noteComboBoxTen("NOTETEN", "ACNINE", "GLIDENINE",audioProcessor.apvts), noteComboBoxEleven("NOTEELEVEN", "ACTEN","GLIDETEN",audioProcessor.apvts), noteComboBoxTwelve("NOTETWELVE", "ACELEVEN", "GLIDEELEVEN",audioProcessor.apvts), noteComboBoxThirteen("NOTETHIRTEEN","ACTWELVE" , "GLIDETWELVE",audioProcessor.apvts), noteComboBoxFourteen("NOTEFOURTEEN", "ACTHIRTEEN" ,"GLIDETHIRTEEN", audioProcessor.apvts), noteComboBoxFifteen("NOTEFIFTEEN", "ACFOURTEEN","GLIDEFOURTEEN", audioProcessor.apvts), noteComboBoxSixteen("NOTESIXTEEN", "ACFIFTEEN","GLIDEFIFTEEN", audioProcessor.apvts)

{
    //Adding images from binary data
    squareDrawing = ImageCache::getFromMemory(BinaryData::squareDrawing_png, BinaryData::squareDrawing_pngSize);
    sawDrawing = ImageCache::getFromMemory(BinaryData::sawDrawing_png, BinaryData::sawDrawing_pngSize);
    ky303title = ImageCache::getFromMemory(BinaryData::KY030_2_png, BinaryData::KY030_2_pngSize);
    accentFontOne = ImageCache::getFromMemory(BinaryData::accentpng_png, BinaryData::accentpng_pngSize);
    
    
    //adding preset panel component
    addAndMakeVisible(presetPanel);
    
    //adding ramdomiser button component and setting name, clours and behaviour on click
    addAndMakeVisible(tunefulRandomiser);
    tunefulRandomiser.setName("Random");
    tunefulRandomiser.setColour(0x1000100, juce::Colours::silver);
    tunefulRandomiser.setColour(0x1000101, juce::Colours::grey);
    tunefulRandomiser.onClick = [this]{
        audioProcessor.tunefulRandomise();
       
    };
  
   
    //adding clear button component and setting name, clours and behaviour on click
    addAndMakeVisible(clearNotes);
    clearNotes.setName("Clear");
    clearNotes.setColour(0x1000100, juce::Colours::silver);
    clearNotes.onClick = [this]{
        audioProcessor.clearNotes();
    };
    
    
    
    //Setting up slider components and attachments
     setUpSlider(cutoffSlider, "Cutoff", cutoffLabel);
        cuttoffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "CUTOFF", cutoffSlider);
        
        setUpSlider(resonanceSlider, "Resonance", resonanceLabel);
        resonanceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "RESONANCE", resonanceSlider);
      
    
    setUpSlider(tuneSlider, "Tune", tuneLabel);
    tuneAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "TUNING", tuneSlider);
    

    
    setUpSlider(EnvModSlider, "Env", EnvModLabel);
    EnvModAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "ENV", EnvModSlider);
    
    setUpSlider(EnvDecaySlider, "Env Decay", EnvDecayLabel);
    EnvDecayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "ENVDECAY", EnvDecaySlider);
    
    setUpSlider(AccentSlider, "Accent", AccentLabel);
    AccentAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "ACCENT", AccentSlider);
    
    
    setUpSlider(gainSlider, "Gain", gainLabel);
       gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "GAIN", gainSlider);
    
 
    
   // making the additional components visible
    addAndMakeVisible(oscComboBox);
    addAndMakeVisible(divisionComboBox);
    
    addAndMakeVisible(noteComboBoxOne);
    addAndMakeVisible(noteComboBoxTwo);
    addAndMakeVisible(noteComboBoxThree);
    addAndMakeVisible(noteComboBoxFour);
    addAndMakeVisible(noteComboBoxFive);
    addAndMakeVisible(noteComboBoxSix);
    addAndMakeVisible(noteComboBoxSeven);
    addAndMakeVisible(noteComboBoxEight);
    addAndMakeVisible(noteComboBoxNine);
    addAndMakeVisible(noteComboBoxTen);
    addAndMakeVisible(noteComboBoxEleven);
    addAndMakeVisible(noteComboBoxTwelve);
    addAndMakeVisible(noteComboBoxThirteen);
    addAndMakeVisible(noteComboBoxFourteen);
    addAndMakeVisible(noteComboBoxFifteen);
    addAndMakeVisible(noteComboBoxSixteen);
    
    
    //Colour for GUI
    getLookAndFeel().setColour(juce::PopupMenu::backgroundColourId, juce::Colours::silver);
    getLookAndFeel().setColour(juce::PopupMenu::highlightedBackgroundColourId, juce::Colours::grey);
    
    
    setSize (800, 350);
}

KY030AudioProcessorEditor::~KY030AudioProcessorEditor()
{
}

//==============================================================================
void KY030AudioProcessorEditor::setUpSlider(juce::Slider& slider, std::string labelName, juce::Label& label){
    // a function that contains the information for the settings of the slider
    addAndMakeVisible(slider);
    slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setColour(0x1001400, juce::Colours::black);
    slider.setColour(0x1001700, juce::Colours::transparentBlack);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    addAndMakeVisible(label);
    slider.setNumDecimalPlacesToDisplay(2);
    label.attachToComponent(&slider, false);
    label.setJustificationType(juce::Justification::centredBottom);
    label.setText(labelName, juce::dontSendNotification);

}


void KY030AudioProcessorEditor::paint (juce::Graphics& g)
{
    // setting the colours and shapes for the GUI
     g.fillAll (juce::Colours::lightgrey);;

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
   
    g.drawImage(squareDrawing, Rectangle<float>(33, 30, 15, 15));
    
    g.drawImage(sawDrawing, Rectangle<float>(33, 95, 15, 15));
    g.drawImage(ky303title, Rectangle<float>(700, 0, 85, 135));
    
    g.setColour (juce::Colours::darkgrey);
    g.drawLine(55, 10, 55, 145, 2);
    g.drawLine(695, 10, 695, 145, 2);
    g.drawLine(55, 10, 695, 10, 2);
    g.drawLine(55, 145, 695, 145, 2);
    
    g.drawLine(10, 10, 55, 10, 2);
    g.drawLine(10, 10, 10, 145, 2);
    g.drawLine(10, 145, 55, 145, 2);
    
    g.drawLine(0, 155, 800, 155, 2);
    
    g.drawLine(0, 200, 800, 200, 2);
    g.drawLine(0, 270, 660, 270, 2);


    g.drawLine(0, 340, 660, 340, 2);
    
  
   
}

void KY030AudioProcessorEditor::resized()
{

    oscComboBox.setBoundsRelative(0.0, 0.1, 0.1, 0.2);
    divisionComboBox.setBoundsRelative(0.6, 0.45, 0.2, 0.095);
   
    tunefulRandomiser.setBoundsRelative(0.8, 0.45, 0.2, 0.095);
    clearNotes.setBoundsRelative(0.5, 0.45, 0.1, 0.095);
    presetPanel.setBoundsRelative(0.0, 0.45, 0.55, 0.1);
   
    noteComboBoxTwo.setBoundsRelative(0.0, 0.6, 0.1, 0.16);
    noteComboBoxThree.setBoundsRelative(0.1, 0.6, 0.1, 0.16);
    noteComboBoxFour.setBoundsRelative(0.2, 0.6, 0.1, 0.16);
    noteComboBoxFive.setBoundsRelative(0.3, 0.6, 0.1, 0.16);
    noteComboBoxSix.setBoundsRelative(0.4, 0.6, 0.1, 0.16);
    noteComboBoxSeven.setBoundsRelative(0.5, 0.6, 0.1, 0.16);
    noteComboBoxEight.setBoundsRelative(0.6, 0.6, 0.1, 0.16);
    noteComboBoxNine.setBoundsRelative(0.7, 0.6, 0.1, 0.16);
    noteComboBoxTen.setBoundsRelative(0.0, 0.8, 0.1, 0.16);
    noteComboBoxEleven.setBoundsRelative(0.1, 0.8, 0.1, 0.16);
    noteComboBoxTwelve.setBoundsRelative(0.2, 0.8, 0.1, 0.16);
    noteComboBoxThirteen.setBoundsRelative(0.3, 0.8, 0.1, 0.16);
    noteComboBoxFourteen.setBoundsRelative(0.4, 0.8, 0.1, 0.16);
    noteComboBoxFifteen.setBoundsRelative(0.5, 0.8, 0.1, 0.16);
    noteComboBoxSixteen.setBoundsRelative(0.6, 0.8, 0.1, 0.16);
    noteComboBoxOne.setBoundsRelative(0.7, 0.8, 0.1, 0.16);
    
    
    tuneSlider.setBoundsRelative(0.1, 0.12, 0.1, 0.3);
    cutoffSlider.setBoundsRelative(0.225, 0.12, 0.1, 0.3);
    
    resonanceSlider.setBoundsRelative(0.35, 0.12, 0.1, 0.3);
   
    EnvModSlider.setBoundsRelative(0.475, 0.12, 0.1, 0.3);
    EnvDecaySlider.setBoundsRelative(0.6, 0.12, 0.1, 0.3);
    AccentSlider.setBoundsRelative(0.725, 0.12, 0.1, 0.3);
    gainSlider.setBoundsRelative(0.85, 0.65, 0.1, 0.3);
}

void KY030AudioProcessorEditor::loadParams(){
    
    //Called on button click to update value trees to what is shown on GUI
    
    noteComboBoxSixteen.changeValue(audioProcessor.noteOneParameter->load());
    noteComboBoxOne.changeValue(audioProcessor.noteTwoParameter->load());
    noteComboBoxTwo.changeValue(audioProcessor.noteThreeParameter->load());
    noteComboBoxThree.changeValue(audioProcessor.noteFourParameter->load());
    noteComboBoxFour.changeValue(audioProcessor.noteFiveParameter->load());
    noteComboBoxFive.changeValue(audioProcessor.noteSixParameter->load());
    noteComboBoxSix.changeValue(audioProcessor.noteSevenParameter->load());
    noteComboBoxSeven.changeValue(audioProcessor.noteEightParameter->load());
    noteComboBoxEight.changeValue(audioProcessor.noteNineParameter->load());
    noteComboBoxNine.changeValue(audioProcessor.noteTenParameter->load());
    noteComboBoxTen.changeValue(audioProcessor.noteElevenParameter->load());
    noteComboBoxEleven.changeValue(audioProcessor.noteTwelveParameter->load());
    noteComboBoxTwelve.changeValue(audioProcessor.noteThirteenParameter->load());
    noteComboBoxThirteen.changeValue(audioProcessor.noteFourteenParameter->load());
    noteComboBoxFourteen.changeValue(audioProcessor.noteFifteenParameter->load());
    noteComboBoxFifteen.changeValue(audioProcessor.noteSixteenParameter->load());
    
}

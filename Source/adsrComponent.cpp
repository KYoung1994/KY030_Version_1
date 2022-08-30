/*
  ==============================================================================

    adsrComponent.cpp
    Created: 14 Jun 2022 8:59:13pm
    Author:  Kimberly Young

  ==============================================================================
*/

#include <JuceHeader.h>
#include "adsrComponent.h"

//==============================================================================
adsrComponent::adsrComponent(juce::AudioProcessorValueTreeState& apvts)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
   // setUpSlider(attackSlider, "Attack", attackLabel);
    
 //   attackSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "ATTACK", attackSlider);
    
    
  //  setUpSlider(decaySlider, "Decay", decayLabel);
    
 //   decaySliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "DECAY", decaySlider);
    
  //  setUpSlider(sustainSlider, "Sustain", sustainLabel);
    
    //sustainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "SUSTAIN", sustainSlider);
    
  //  setUpSlider(releaseSlider, "Release", releaseLabel);
    
 //std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "RELEASE", releaseSlider);

    
}

adsrComponent::~adsrComponent()
{
}

void adsrComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (juce::Colours::black);
}

void adsrComponent::resized()
{
 //   attackSlider.setBoundsRelative(0.0, 0.1, 0.2, 0.9);
  //  decaySlider.setBoundsRelative(0.2, 0.1, 0.2, 0.9);
  //  sustainSlider.setBoundsRelative(0.4, 0.1, 0.2, 0.9);
  //  releaseSlider.setBoundsRelative(0.6, 0.1, 0.2, 0.9);
}


void adsrComponent::setUpSlider(juce::Slider& slider, std::string labelName, juce::Label& label){
    
    addAndMakeVisible(slider);
    slider.setSliderStyle(juce::Slider::LinearVertical);
    slider.setColour(0x1001400, juce::Colours::black);
    slider.setColour(0x1001700, juce::Colours::transparentBlack);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 15);
    addAndMakeVisible(label);
    slider.setNumDecimalPlacesToDisplay(2);
    label.attachToComponent(&slider, false);
    label.setJustificationType(juce::Justification::centredBottom);
    
    label.setText(labelName, juce::dontSendNotification);

}

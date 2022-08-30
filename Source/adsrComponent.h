/*
  ==============================================================================

    adsrComponent.h
    Created: 14 Jun 2022 8:59:13pm
    Author:  Kimberly Young

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class adsrComponent  : public juce::Component
{
public:
    adsrComponent(juce::AudioProcessorValueTreeState& apvts);
    ~adsrComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    typedef std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachment;

private:
    
  //  juce::Slider attackSlider;
   // juce::Slider decaySlider;
   // juce::Slider sustainSlider;
   // juce::Slider releaseSlider;
    
    
 //   juce::Label attackLabel;
  //  juce::Label decayLabel;
  //  juce::Label sustainLabel;
  //  juce::Label releaseLabel;
    
    sliderAttachment attackSliderAttachment;
    sliderAttachment decaySliderAttachment;
    sliderAttachment sustainSliderAttachment;
    sliderAttachment releaseSliderAttachment;
    
    void setUpSlider(juce::Slider& slider, std::string labelName, juce::Label& label);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (adsrComponent)
};

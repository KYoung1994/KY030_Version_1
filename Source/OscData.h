/*
  ==============================================================================

    OscData.h
    Created: 16 Jun 2022 10:29:26am
    Author:  Kimberly Young

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class OscData : public juce::dsp::Oscillator<float> {
public:
    void setOscFrequency (int midiNoteNumber);
    void setWaveType(const int choice);
    void prepareToPlay (juce::dsp::ProcessSpec& spec);
    void processNextAudioBlock(juce::dsp::AudioBlock<float>& audioBlock);
    void setOscGlideFrequency(int midiNoteNumber);
    
private:
    
    bool isGlideOn;
    
    juce::SmoothedValue<float> smoothednoteValue;
    int newNoteValue = 0;
    
};

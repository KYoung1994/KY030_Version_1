/*
  ==============================================================================

    adsrData.h
    Created: 14 Jun 2022 9:00:24pm
    Author:  Kimberly Young

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

// Updates the ADSR envelope

class AdsrData : public juce::ADSR {
    
public:
    
    void UpdateADSR(const float attack, const float decay, const float sustain, const float release);
private:
    
    juce::ADSR::Parameters adsrParams;
};

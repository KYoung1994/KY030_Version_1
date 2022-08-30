/*
  ==============================================================================

    adsrData.cpp
    Created: 14 Jun 2022 9:00:24pm
    Author:  Kimberly Young

  ==============================================================================
*/

#include "adsrData.h"

// Updates the ADSR envelope
void AdsrData::UpdateADSR(const float attack, const float decay, const float sustain, const float release){
    
    adsrParams.attack = attack;
    adsrParams.decay = decay;
    adsrParams.sustain = sustain;
    adsrParams.release = release;
    
    setParameters(adsrParams);
}


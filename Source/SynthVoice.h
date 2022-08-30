/*
  ==============================================================================

    SynthVoice.h
    Created: 1 Jun 2022 5:46:36pm
    Author:  Kimberly Young

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include "adsrData.h"
#include "OscData.h"

class SynthVoice : public juce::SynthesiserVoice{
    
public:
    
    bool canPlaySound (juce::SynthesiserSound* sound) override;
    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition) override;
    void stopNote (float velocity, bool allowTailOff) override;
    void controllerMoved (int controllerNumber, int newControllerValue) override;
    void pitchWheelMoved (int newPitchWheelValue) override;
    void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);
    void UpdateADSR(const float attack, const float decay, const float sustain, const float release, float tune);
    
    void renderNextBlock (juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples) override;
    
    OscData& getOscillator(){return osc;};
    
    float tuning = 0;
    void updateGlide(bool glideOnOff);
    
    
private:
    AdsrData adsr;
    juce::AudioBuffer<float> synthBuffer;
    
   
    OscData osc;
    juce::dsp::Gain<float> gain;
    bool isPrepared {false};
    bool isGlide {false};
    
 
   
    
    
};

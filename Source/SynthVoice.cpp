/*
  ==============================================================================

    SynthVoice.cpp
    Created: 1 Jun 2022 5:46:36pm
    Author:  Kimberly Young

  ==============================================================================
*/

#include "SynthVoice.h"

//These methods are inhereted from the JUCE synthesiser class and are overidden to be used with this projects adsr, and oscillator projects

bool SynthVoice::canPlaySound (juce::SynthesiserSound* sound) {
    
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr ;
}
void SynthVoice::startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition) {
    
    adsr.noteOn();
    if(isGlide ==true){
    osc.setOscGlideFrequency(midiNoteNumber + tuning);
    }
    if(isGlide==false){
        osc.setOscFrequency(midiNoteNumber + tuning);
    }
}
 void SynthVoice::stopNote (float velocity, bool allowTailOff) {
    
    adsr.noteOff();
    
    if ( ! allowTailOff || ! adsr.isActive()){
        clearCurrentNote();
    }
    
}
void SynthVoice::controllerMoved (int controllerNumber, int newControllerValue) {
}
void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels){
    
    adsr.setSampleRate(sampleRate);
    
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;
    
    osc.prepareToPlay(spec);
    gain.prepare (spec);
    
    
    gain.setGainLinear (0.5f);
    
  
    isPrepared = true;
}

void SynthVoice::UpdateADSR(const float attack, const float decay, const float sustain, const float release, float tune){
    
    tuning = tune;
    adsr.UpdateADSR(attack, decay, sustain, release);
}


void SynthVoice::pitchWheelMoved (int newPitchWheelValue) {

}

void SynthVoice::renderNextBlock (juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples) {
    
    jassert(isPrepared);
    
    if ( ! isVoiceActive()){
        return;
    }
    
    synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true );
    synthBuffer.clear();
    
    juce::dsp::AudioBlock<float> audioBlock { synthBuffer };
    osc.processNextAudioBlock  (audioBlock);
    gain.process (juce::dsp::ProcessContextReplacing<float> (audioBlock));
    
    adsr.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples());
    
    for(int channel = 0; channel < outputBuffer.getNumChannels(); ++channel ){
        outputBuffer.addFrom(channel, startSample, synthBuffer, channel, 0, numSamples);
        
        if( ! adsr.isActive()){
            clearCurrentNote();
        }
    }
}

void SynthVoice::updateGlide(bool glideOnOff){
    
    isGlide = glideOnOff;
}

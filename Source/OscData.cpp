/*
  ==============================================================================

    OscData.cpp
    Created: 16 Jun 2022 10:29:26am
    Author:  Kimberly Young

  ==============================================================================
*/

#include "OscData.h"

//This contains methods to prepare the oscillator shape and set the frequency of each note

void OscData::setOscFrequency(int midiNoteNumber){
    isGlideOn=false;
    setFrequency (juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
    
}

void OscData::setOscGlideFrequency(int midiNoteNumber){
    isGlideOn=true;
    newNoteValue=midiNoteNumber;
    smoothednoteValue.setTargetValue(juce::MidiMessage::getMidiNoteInHertz(newNoteValue));
}

void OscData::prepareToPlay(juce::dsp::ProcessSpec& spec){
    prepare (spec);
    
    smoothednoteValue.reset(spec.sampleRate, 0.3);
}



void OscData::setWaveType(const int choice){
        
    switch (choice) {
        case 0:
            
            //saw wave
            initialise([](float x) {return x / juce::MathConstants<float>::pi;} );
            
            break;
            
        case 1:

            //square wave
     
            initialise([](float x) {return (((x < 0.0f ? -1.0f : std::sin(x)) + std::sin(x))) ;} );
            break;
            
            
        default:
            jassertfalse;
            break;
    }
}


void OscData::processNextAudioBlock(juce::dsp::AudioBlock<float>& audioBlock){
    
    if(isGlideOn==true){
    for (int sample = 0; sample < audioBlock.getNumSamples(); sample++) {
        setFrequency(smoothednoteValue.getNextValue());
        juce::dsp::AudioBlock<float> subBlock = audioBlock.getSubBlock(sample, 1);
        process (juce::dsp::ProcessContextReplacing<float>(subBlock));
        }
    }
    if(isGlideOn==false){
        process (juce::dsp::ProcessContextReplacing<float> (audioBlock));
    }
}

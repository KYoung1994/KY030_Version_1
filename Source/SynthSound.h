/*
  ==============================================================================

    SynthSound.h
    Created: 1 Jun 2022 5:46:46pm
    Author:  Kimberly Young

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>


class SynthSound : public juce::SynthesiserSound{
public:
    bool appliesToNote (int midiNoteNumber) override { return true; }
    bool appliesToChannel (int midiChannel) override { return true; }
    
    
};

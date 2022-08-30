/*
  ==============================================================================

    EnvFol.h
    Created: 23 Mar 2022 10:10:44am
    Author:  Kimberly Young

  ==============================================================================
*/



#pragma once

#include <JuceHeader.h>

class EnvelopeFollower {
public:
    EnvelopeFollower()
    {
        
    }
    
    ~EnvelopeFollower()
    {
        
    }
    
    void prepare(double newSampleRate, int numChannels)
    {
        sampleRate = newSampleRate;
        calculateAttackCoefficient(attack);
        calculateReleaseCoefficient(release);
        
        lastOutput.resize(numChannels);
        for (int i = 0; i < lastOutput.size(); i++)
        {
            lastOutput[i] = 0.0;
        }
    }
    
    void setAttackAndRelease(float thisAttack, float thisRelease)
    {
        //because pow is an expensive operation and is used in both of these functions, calculating the attack and release will only happen when there is a change in them from the initial parameters
        if (thisAttack != attack)
        {
            calculateAttackCoefficient(thisAttack);
        }
        if (thisRelease != release)
        {
            calculateReleaseCoefficient(thisRelease);
        }
    }
    
    float calculateEnvelope(float inputValue, int channel)
    {
        //checking if the function needs to use the attack or release function based on the input
        thisCoefficient = (inputValue > lastOutput[channel]) ? attackCoefficient : releaseCoefficient;
        
        //Calculates the RMS of the input and returns the envelope
      //  lastOutput[channel] = sqrtf(thisCoefficient * powf(lastOutput[channel], 2.0) + (1.0 - thisCoefficient) * powf(inputValue, 2.0));
        
        //Calculates the Peak of the input and returns the envelope
        lastOutput[channel] = inputValue + thisCoefficient * (lastOutput[channel] - inputValue);
        return lastOutput[channel];
    }
    
private:
    void calculateAttackCoefficient(float time)
    {
        //checks that the pow operation is not being used on 0
        attack = time;
        if (time == 0.0) {
            attackCoefficient = 0.0;
        } else
        {
            attackCoefficient = pow(M_E, -1.0 / (time * sampleRate));
        }
    }
    
    void calculateReleaseCoefficient(float time)
    {
        //checks that the pow operation is not being used on 0
        release = time;
        if (time == 0.0) {
            releaseCoefficient = 0.0;
        } else
        {
            releaseCoefficient = pow(M_E, -1.0 / (time * sampleRate));
        }
    }
    
    std::vector<float> lastOutput;
    float attackCoefficient, releaseCoefficient, thisCoefficient, attack = 0.1, release = 0.1;
    double sampleRate = 44100.0;
};

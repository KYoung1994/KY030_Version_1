/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <JuceHeader.h>
#include "SynthVoice.h"
#include "SynthSound.h"
#include "EnvFol.h"
#include "PresetManager.h"


//==============================================================================
/**
*/
class KY030AudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    KY030AudioProcessor();
    ~KY030AudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

          juce::AudioProcessorValueTreeState apvts;
    
    juce::AudioPlayHead::PositionInfo posInfo;
    
    void updateCurrentTimeInfoFromHost();
    
    void tunefulRandomise();
    bool notePlaying();
    bool nowPlaying;
    
    std::atomic<float>* noteOneParameter = nullptr;
    std::atomic<float>* noteTwoParameter = nullptr;
    std::atomic<float>* noteThreeParameter = nullptr;
    std::atomic<float>* noteFourParameter = nullptr;
    std::atomic<float>* noteFiveParameter = nullptr;
    std::atomic<float>* noteSixParameter = nullptr;
    std::atomic<float>* noteSevenParameter = nullptr;
    std::atomic<float>* noteEightParameter = nullptr;
    std::atomic<float>* noteNineParameter = nullptr;
    std::atomic<float>* noteTenParameter = nullptr;
    std::atomic<float>* noteElevenParameter = nullptr;
    std::atomic<float>* noteTwelveParameter = nullptr;
    std::atomic<float>* noteThirteenParameter = nullptr;
    std::atomic<float>* noteFourteenParameter = nullptr;
    std::atomic<float>* noteFifteenParameter = nullptr;
    std::atomic<float>* noteSixteenParameter = nullptr;
    
    Service::PresetManager& getPresetManager() {return *presetManager;}
    void getRawParamValues();
    void applyEnvelopeAndAccentAndGain(juce::AudioBuffer<float>& buffer, float accent, float pbGain);
    
    void clearNotes();
    void synthVoiceandADSR ();
    void stepSequence(int step, int stepNumber, int pbNoteStop, bool pbAccent, bool pbGlide, int pbNoteStart);
 
    std::atomic<int> notePos;
    
    juce::AudioBuffer<float> noisebuffer;
    
    
private:

    std::unique_ptr<Service::PresetManager> presetManager;
    
    float resonanceDecay= 0.1;
    int resFac = 1;

    juce::dsp::LadderFilter<float> ladderFilter;

    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();
    
    bool applyAccent, applyGlide;
  
    void setLadderParams(float accent);
    
    int getBarDivision(int selection);
    int getMidiNoteNum(int note);
    
    juce::Synthesiser synth;

    EnvelopeFollower envelopeFollower;
    
    int ppq, stepDivision, step = 0;

    juce::MidiBuffer midiBuff;
    
    juce::AudioPlayHead* playhead;
    
    juce::AudioPlayHead::CurrentPositionInfo hostInfo;
    
    std::atomic<float>* cuttoffParam = nullptr;
    std::atomic<float>* resonanceParam = nullptr;
    std::atomic<float>* driveParam = nullptr;
    std::atomic<float>* gainParam = nullptr;
    std::atomic<float>* followerEffectParameter = nullptr;
    std::atomic<float>* envDecayParameter = nullptr;
    std::atomic<float>* accentParameter = nullptr;
    std::atomic<float>* barDivParameter = nullptr;
    
    std::atomic<float>* accentOneParameter = nullptr;
    std::atomic<float>* accentTwoParameter = nullptr;
    std::atomic<float>* accentThreeParameter = nullptr;
    std::atomic<float>* accentFourParameter = nullptr;
    std::atomic<float>* accentFiveParameter = nullptr;
    std::atomic<float>* accentSixParameter = nullptr;
    std::atomic<float>* accentSevenParameter = nullptr;
    std::atomic<float>* accentEightParameter = nullptr;
    std::atomic<float>* accentNineParameter = nullptr;
    std::atomic<float>* accentTenParameter = nullptr;
    std::atomic<float>* accentElevenParameter = nullptr;
    std::atomic<float>* accentTwelveParameter = nullptr;
    std::atomic<float>* accentThirteenParameter = nullptr;
    std::atomic<float>* accentFourteenParameter = nullptr;
    std::atomic<float>* accentFifteenParameter = nullptr;
    std::atomic<float>* accentSixteenParameter = nullptr;
    
    std::atomic<float>* glideOneParameter = nullptr;
    std::atomic<float>* glideTwoParameter = nullptr;
    std::atomic<float>* glideThreeParameter = nullptr;
    std::atomic<float>* glideFourParameter = nullptr;
    std::atomic<float>* glideFiveParameter = nullptr;
    std::atomic<float>* glideSixParameter = nullptr;
    std::atomic<float>* glideSevenParameter = nullptr;
    std::atomic<float>* glideEightParameter = nullptr;
    std::atomic<float>* glideNineParameter = nullptr;
    std::atomic<float>* glideTenParameter = nullptr;
    std::atomic<float>* glideElevenParameter = nullptr;
    std::atomic<float>* glideTwelveParameter = nullptr;
    std::atomic<float>* glideThirteenParameter = nullptr;
    std::atomic<float>* glideFourteenParameter = nullptr;
    std::atomic<float>* glideFifteenParameter = nullptr;
    std::atomic<float>* glideSixteenParameter = nullptr;
    
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KY030AudioProcessor)
};

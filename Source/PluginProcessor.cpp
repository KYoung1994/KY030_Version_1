/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
KY030AudioProcessor::KY030AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
 ,apvts(*this, nullptr, "Parameters", createParameters())
#endif
{
    //initialising the voice and sound of the synthesiser
    synth.addSound(new SynthSound());
    synth.addVoice(new SynthVoice());
    
    //Setting property for the value tree state version, allowing the plugin to be used as AU and VST3
    apvts.state.setProperty(Service::PresetManager::presetNameProperty, "", nullptr);
    apvts.state.setProperty("Version", ProjectInfo::versionString, nullptr);
    
    // adding a pointer to the value tree state to the preset manager
    presetManager = std::make_unique<Service::PresetManager>(apvts);
    
    //calling a function to initialise all atomic pointers to value trees
    getRawParamValues();
    
    apvts.state = ValueTree("SavedParams");
}

KY030AudioProcessor::~KY030AudioProcessor()
{
}

//==============================================================================
const juce::String KY030AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool KY030AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool KY030AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool KY030AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double KY030AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int KY030AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int KY030AudioProcessor::getCurrentProgram()
{
    return 0;
}

void KY030AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String KY030AudioProcessor::getProgramName (int index)
{
    return {};
}

void KY030AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void KY030AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    //Initialising all objects with essential information they need to function corrently
    
    synth.setCurrentPlaybackSampleRate(sampleRate);
    
    for(int i = 0; i < synth.getNumVoices(); i++){
        if( auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i))){
            
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        }
    }
        
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();
    spec.maximumBlockSize = samplesPerBlock;
    
    ladderFilter.prepare(spec);
    ladderFilter.reset();
    ladderFilter.setMode(juce::dsp::LadderFilterMode::LPF24);
    
    envelopeFollower.prepare(sampleRate, getTotalNumOutputChannels());
    envelopeFollower.setAttackAndRelease(5.0 / 1000.0, 100.0 / 1000.0);
    
 
}

void KY030AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool KY030AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void KY030AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    // getting values from value trees
    auto pbBarDiv = barDivParameter->load();
    auto pbNoteOne = getMidiNoteNum(noteOneParameter->load() + 21);
    auto pbNoteTwo = getMidiNoteNum(noteTwoParameter->load() + 21);
    auto pbNoteThree = getMidiNoteNum(noteThreeParameter->load() + 21);
    auto pbNoteFour = getMidiNoteNum(noteFourParameter->load() + 21);
    auto pbNoteFive = getMidiNoteNum(noteFiveParameter->load()+ 21);
    auto pbNoteSix = getMidiNoteNum(noteSixParameter->load()+ 21);
    auto pbNoteSeven = getMidiNoteNum(noteSevenParameter->load()+ 21);
    auto pbNoteEight = getMidiNoteNum(noteEightParameter->load()+ 21);
    auto pbNoteNine = getMidiNoteNum(noteNineParameter->load()+ 21);
    auto pbNoteTen = getMidiNoteNum(noteTenParameter->load()+ 21);
    auto pbNoteEleven = getMidiNoteNum(noteElevenParameter->load()+ 21);
    auto pbNoteTwelve = getMidiNoteNum(noteTwelveParameter->load()+ 21);
    auto pbNoteThirteen = getMidiNoteNum(noteThirteenParameter->load()+ 21);
    auto pbNoteFourteen = getMidiNoteNum(noteFourteenParameter->load()+ 21);
    auto pbNoteFifteen = getMidiNoteNum(noteFifteenParameter->load()+ 21);
    auto pbNoteSixteen = getMidiNoteNum(noteSixteenParameter->load()+ 21);
    
    auto pbAcOne = accentOneParameter->load();
    auto pbAcTwo = accentTwoParameter->load();
    auto pbAcThree = accentThreeParameter->load();
    auto pbAcFour = accentFourParameter->load();
    auto pbAcFive = accentFiveParameter->load();
    auto pbAcSix = accentSixParameter->load();
    auto pbAcSeven = accentSevenParameter->load();
    auto pbAcEight = accentEightParameter->load();
    auto pbAcNine = accentNineParameter->load();
    auto pbAcTen = accentTenParameter->load();
    auto pbAcEleven = accentElevenParameter->load();
    auto pbAcTwelve = accentTwelveParameter->load();
    auto pbAcThirteen = accentThirteenParameter->load();
    auto pbAcFourteen = accentFourteenParameter->load();
    auto pbAcFifteen = accentFifteenParameter->load();
    auto pbAcSixteen = accentSixteenParameter->load();
    
    auto pbGlideOne  = glideOneParameter->load();
    auto pbGlideTwo  = glideTwoParameter->load();
    auto pbGlideThree  = glideThreeParameter->load();
    auto pbGlideFour = glideFourParameter->load();
    auto pbGlideFive  = glideFiveParameter->load();
    auto pbGlideSix  = glideSixParameter->load();
    auto pbGlideSeven  = glideSevenParameter->load();
    auto pbGlideEight  = glideEightParameter->load();
    auto pbGlideNine  = glideNineParameter->load();
    auto pbGlideTen  = glideTenParameter->load();
    auto pbGlideEleven  = glideElevenParameter->load();
    auto pbGlideTwelve  = glideTwelveParameter->load();
    auto pbGlideThirteen  = glideThirteenParameter->load();
    auto pbGlideFourteen  = glideFourteenParameter->load();
    auto pbGlideFifteen  = glideFifteenParameter->load();
    auto pbGlideSixteen = glideSixteenParameter->load();
    
    auto accent = accentParameter->load();
    auto pbGain = gainParam->load();
    
    
    
 
    //Step sequencer process, taking information from the audio playhead from the host
       playhead = this->getPlayHead();
        if(playhead != nullptr){
            playhead->getCurrentPosition(hostInfo);
            stepDivision = hostInfo.ppqPosition * getBarDivision(pbBarDiv) ;
         
             
            if (playhead->getPosition()->getIsPlaying()) {
                if(ppq > stepDivision){
                    step = 0;
                }
                if (ppq != stepDivision){
                    
                    step++;
                    
                    notePos.store(step);
                    if (step == 16) {step = 0;}
                    //inputting information in the step sequencer function according to the current step
                    stepSequence(step, 0, pbNoteSixteen, pbAcSixteen, pbGlideSixteen, pbNoteOne);
                    stepSequence(step, 1, pbNoteOne, pbAcOne, pbGlideOne, pbNoteTwo);
                    stepSequence(step, 2, pbNoteTwo, pbAcTwo, pbGlideTwo, pbNoteThree);
                    stepSequence(step, 3, pbNoteThree, pbAcThree, pbGlideThree, pbNoteFour);
                    stepSequence(step, 4, pbNoteFour, pbAcFour, pbGlideFour, pbNoteFive);
                    stepSequence(step, 5, pbNoteFive, pbAcFive, pbGlideFive, pbNoteSix);
                    stepSequence(step, 6, pbNoteSix, pbAcSix, pbGlideSix, pbNoteSeven);
                    stepSequence(step, 7, pbNoteSeven, pbAcSeven, pbGlideSeven, pbNoteEight);
                    stepSequence(step, 8, pbNoteEight, pbAcEight, pbGlideEight, pbNoteNine);
                    stepSequence(step, 9, pbNoteNine, pbAcNine, pbGlideNine, pbNoteTen);
                    stepSequence(step, 10, pbNoteTen, pbAcTen, pbGlideTen, pbNoteEleven);
                    stepSequence(step, 11, pbNoteEleven, pbAcEleven, pbGlideEleven, pbNoteTwelve);
                    stepSequence(step, 12, pbNoteTwelve, pbAcTwelve, pbGlideTwelve, pbNoteThirteen);
                    stepSequence(step, 13, pbNoteThirteen, pbAcThirteen, pbGlideThirteen, pbNoteFourteen);
                    stepSequence(step, 14, pbNoteFourteen, pbAcFourteen, pbGlideFourteen, pbNoteFifteen);
                    stepSequence(step, 15, pbNoteFifteen, pbAcFifteen, pbGlideFifteen, pbNoteSixteen);
                    stepSequence(step, 16, pbNoteSixteen, pbAcSixteen, pbGlideSixteen, pbNoteOne);
                                  
                ppq = stepDivision;
                }
            } else
            {
                //preventing stuck notes
                synth.allNotesOff(1, true);
            }
          
        }
        else{
            DBG(" no playhead");
        }

    
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    //set and update ladder parameters
    setLadderParams( accent);
    
    
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    
    //filling the buffer with the synthesiser sound
    synthVoiceandADSR();
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    applyEnvelopeAndAccentAndGain(buffer, accent, pbGain); // Calculating cutoff
    
}

//==============================================================================
bool KY030AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* KY030AudioProcessor::createEditor()
{
    return new KY030AudioProcessorEditor (*this);
}

//==============================================================================
void KY030AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    //The following code in gettStateInformation was added post deadline
    std::unique_ptr<juce::XmlElement> xml (apvts.state.createXml());
    copyXmlToBinary(*xml, destData);
    //std::unique_ptr
}

void KY030AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
    
    
    //The following code in setStateInformation was added post deadline
    std::unique_ptr<juce::XmlElement> theParams (getXmlFromBinary(data, sizeInBytes));
    
    if(theParams != nullptr){
        if(theParams -> hasTagName(apvts.state.getType())){
            apvts.state = ValueTree::fromXml(*theParams);
        }
    }
    
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new KY030AudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout KY030AudioProcessor::createParameters(){
   
      // this function is creating and storing all of the parameter functions into a vector. This funtion is called in the audio Processor constructor
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> vParams;
    

    //Value tree initialisation
    vParams.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID("OSC", 1), "Oscillaro", juce::StringArray {"Saw", "Sqare"}, 0));
    
      vParams.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID("BARDIV",1), "BarDiv", juce::StringArray {"Four", "Eight", "Sixteen"}, 2));
    
    vParams.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID("NOTEONE",1), "NoteOne", juce::StringArray {"NONE", "B0", "C0", "C#0", "D0", "D#0", "E0", "F0", "F#0", "G0", "G#0", "A1", "A#1", "B1", "C1", "C#1", "D1", "D#1", "E1", "F1", "F#1", "G1", "G#1", "A2", "A#2", "B2", "C2", "C#2", "D2", "D#2", "E2", "F2", "F#2", "G2", "G#2", "A3", "A#3", "B3", "C3", "C#3", "D3", "D#3", "E3", "F3", "F#3", "G3", "G#3", "A4", "A#4", "B4", "C4"}, 0));
    vParams.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID ("ACONE",1), "AccentOne", false));
    vParams.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID ("GLIDEONE",1), "GlideOne", false));
    
    vParams.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID ("NOTETWO",1), "NoteTwo", juce::StringArray {"NONE", "B0", "C0", "C#0", "D0", "D#0", "E0", "F0", "F#0", "G0", "G#0", "A1", "A#1", "B1", "C1", "C#1", "D1", "D#1", "E1", "F1", "F#1", "G1", "G#1", "A2", "A#2", "B2", "C2", "C#2", "D2", "D#2", "E2", "F2", "F#2", "G2", "G#2", "A3", "A#3", "B3", "C3", "C#3", "D3", "D#3", "E3", "F3", "F#3", "G3", "G#3", "A4", "A#4", "B4", "C4"}, 0));
    vParams.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID ("ACTWO", 1), "AccentTwo", false));
    vParams.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("GLIDETWO", 1), "GlideTwo", false));
    
    vParams.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID("NOTETHREE",1), "NoteThree", juce::StringArray {"NONE", "B0", "C0", "C#0", "D0", "D#0", "E0", "F0", "F#0", "G0", "G#0", "A1", "A#1", "B1", "C1", "C#1", "D1", "D#1", "E1", "F1", "F#1", "G1", "G#1", "A2", "A#2", "B2", "C2", "C#2", "D2", "D#2", "E2", "F2", "F#2", "G2", "G#2", "A3", "A#3", "B3", "C3", "C#3", "D3", "D#3", "E3", "F3", "F#3", "G3", "G#3", "A4", "A#4", "B4", "C4"}, 0));
    
    vParams.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("ACTHREE",1), "AccentThree", false));
    vParams.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("GLIDETHREE", 1), "GlideThree", false));
    
    vParams.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID ("NOTEFOUR",1), "NoteFour", juce::StringArray {"NONE", "B0", "C0", "C#0", "D0", "D#0", "E0", "F0", "F#0", "G0", "G#0", "A1", "A#1", "B1", "C1", "C#1", "D1", "D#1", "E1", "F1", "F#1", "G1", "G#1", "A2", "A#2", "B2", "C2", "C#2", "D2", "D#2", "E2", "F2", "F#2", "G2", "G#2", "A3", "A#3", "B3", "C3", "C#3", "D3", "D#3", "E3", "F3", "F#3", "G3", "G#3", "A4", "A#4", "B4", "C4"}, 0));
    
    vParams.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("ACFOUR",1), "AccentFour", false));
    vParams.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID ("GLIDEFOUR",1), "GlideFour", false));
    
    vParams.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID("NOTEFIVE",1), "NoteFive", juce::StringArray {"NONE", "B0", "C0", "C#0", "D0", "D#0", "E0", "F0", "F#0", "G0", "G#0", "A1", "A#1", "B1", "C1", "C#1", "D1", "D#1", "E1", "F1", "F#1", "G1", "G#1", "A2", "A#2", "B2", "C2", "C#2", "D2", "D#2", "E2", "F2", "F#2", "G2", "G#2", "A3", "A#3", "B3", "C3", "C#3", "D3", "D#3", "E3", "F3", "F#3", "G3", "G#3", "A4", "A#4", "B4", "C4"}, 0));
    
    vParams.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID ("ACFIVE",1), "AccentFive", false));
    vParams.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("GLIDEFIVE",1), "GlideFive", false));
    
    vParams.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID("NOTESIX",1), "NoteSix", juce::StringArray {"NONE", "B0", "C0", "C#0", "D0", "D#0", "E0", "F0", "F#0", "G0", "G#0", "A1", "A#1", "B1", "C1", "C#1", "D1", "D#1", "E1", "F1", "F#1", "G1", "G#1", "A2", "A#2", "B2", "C2", "C#2", "D2", "D#2", "E2", "F2", "F#2", "G2", "G#2", "A3", "A#3", "B3", "C3", "C#3", "D3", "D#3", "E3", "F3", "F#3", "G3", "G#3", "A4", "A#4", "B4", "C4"}, 0));
    
    vParams.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("ACSIX",1), "AccentSix", false));
    vParams.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID ("GLIDESIX",1), "GlideSic", false));
    
    vParams.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID ("NOTESEVEN",1), "NoteSeven", juce::StringArray {"NONE", "B0", "C0", "C#0", "D0", "D#0", "E0", "F0", "F#0", "G0", "G#0", "A1", "A#1", "B1", "C1", "C#1", "D1", "D#1", "E1", "F1", "F#1", "G1", "G#1", "A2", "A#2", "B2", "C2", "C#2", "D2", "D#2", "E2", "F2", "F#2", "G2", "G#2", "A3", "A#3", "B3", "C3", "C#3", "D3", "D#3", "E3", "F3", "F#3", "G3", "G#3", "A4", "A#4", "B4", "C4"}, 0));
    
    vParams.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("ACSEVEN",1), "AccentSeven", false));
    vParams.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("GLIDESEVEN",1), "GlideSeven", false));
    
    vParams.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID("NOTEEIGHT",1), "NoteEight", juce::StringArray{"NONE", "B0", "C0", "C#0", "D0", "D#0", "E0", "F0", "F#0", "G0", "G#0", "A1", "A#1", "B1", "C1", "C#1", "D1", "D#1", "E1", "F1", "F#1", "G1", "G#1", "A2", "A#2", "B2", "C2", "C#2", "D2", "D#2", "E2", "F2", "F#2", "G2", "G#2", "A3", "A#3", "B3", "C3", "C#3", "D3", "D#3", "E3", "F3", "F#3", "G3", "G#3", "A4", "A#4", "B4", "C4"}, 0));
    
    vParams.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("ACEIGHT",1), "AccentEight", false));
    vParams.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("GLIDEEIGHT",1), "GlideEight", false));
    
    vParams.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID("NOTENINE",1), "NoteNine", juce::StringArray {"NONE", "B0", "C0", "C#0", "D0", "D#0", "E0", "F0", "F#0", "G0", "G#0", "A1", "A#1", "B1", "C1", "C#1", "D1", "D#1", "E1", "F1", "F#1", "G1", "G#1", "A2", "A#2", "B2", "C2", "C#2", "D2", "D#2", "E2", "F2", "F#2", "G2", "G#2", "A3", "A#3", "B3", "C3", "C#3", "D3", "D#3", "E3", "F3", "F#3", "G3", "G#3", "A4", "A#4", "B4", "C4"}, 0));
    
    vParams.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("ACNINE",1), "AccentNine", false));
    vParams.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("GLIDENINE",1), "GlideNine", false));
    
    vParams.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID("NOTETEN",1), "NoteTen", juce::StringArray {"NONE", "B0", "C0", "C#0", "D0", "D#0", "E0", "F0", "F#0", "G0", "G#0", "A1", "A#1", "B1", "C1", "C#1", "D1", "D#1", "E1", "F1", "F#1", "G1", "G#1", "A2", "A#2", "B2", "C2", "C#2", "D2", "D#2", "E2", "F2", "F#2", "G2", "G#2", "A3", "A#3", "B3", "C3", "C#3", "D3", "D#3", "E3", "F3", "F#3", "G3", "G#3", "A4", "A#4", "B4", "C4"}, 0));
    vParams.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("ACTEN",1), "AccentTen", false));
    vParams.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("GLIDETEN",1), "GlideTen", false));
    
    vParams.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID("NOTEELEVEN",1), "NoteEleven", juce::StringArray {"NONE", "B0", "C0", "C#0", "D0", "D#0", "E0", "F0", "F#0", "G0", "G#0", "A1", "A#1", "B1", "C1", "C#1", "D1", "D#1", "E1", "F1", "F#1", "G1", "G#1", "A2", "A#2", "B2", "C2", "C#2", "D2", "D#2", "E2", "F2", "F#2", "G2", "G#2", "A3", "A#3", "B3", "C3", "C#3", "D3", "D#3", "E3", "F3", "F#3", "G3", "G#3", "A4", "A#4", "B4", "C4"}, 0));
    vParams.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("ACELEVEN",1), "AccentEleven", false));
    vParams.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("GLIDEELEVEN",1), "GlideEleven", false));
    
    vParams.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID ("NOTETWELVE",1), "NoteTwelve", juce::StringArray {"NONE", "B0", "C0", "C#0", "D0", "D#0", "E0", "F0", "F#0", "G0", "G#0", "A1", "A#1", "B1", "C1", "C#1", "D1", "D#1", "E1", "F1", "F#1", "G1", "G#1", "A2", "A#2", "B2", "C2", "C#2", "D2", "D#2", "E2", "F2", "F#2", "G2", "G#2", "A3", "A#3", "B3", "C3", "C#3", "D3", "D#3", "E3", "F3", "F#3", "G3", "G#3", "A4", "A#4", "B4", "C4"}, 0));
    vParams.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("ACTWELVE",1), "AccentTwelve", false));
    vParams.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("GLIDETWELVE",1), "GlideTwelve", false));
    
    vParams.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID("NOTETHIRTEEN",1), "NoteThirteen", juce::StringArray {"NONE", "B0", "C0", "C#0", "D0", "D#0", "E0", "F0", "F#0", "G0", "G#0", "A1", "A#1", "B1", "C1", "C#1", "D1", "D#1", "E1", "F1", "F#1", "G1", "G#1", "A2", "A#2", "B2", "C2", "C#2", "D2", "D#2", "E2", "F2", "F#2", "G2", "G#2", "A3", "A#3", "B3", "C3", "C#3", "D3", "D#3", "E3", "F3", "F#3", "G3", "G#3", "A4", "A#4", "B4", "C4"}, 0));
    vParams.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("ACTHIRTEEN",1), "AccentThirteen", false));
    vParams.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("GLIDETHIRTEEN",1), "GlideThirteen", false));
    
    vParams.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID("NOTEFOURTEEN",1), "NoteFourteen", juce::StringArray {"NONE", "B0", "C0", "C#0", "D0", "D#0", "E0", "F0", "F#0", "G0", "G#0", "A1", "A#1", "B1", "C1", "C#1", "D1", "D#1", "E1", "F1", "F#1", "G1", "G#1", "A2", "A#2", "B2", "C2", "C#2", "D2", "D#2", "E2", "F2", "F#2", "G2", "G#2", "A3", "A#3", "B3", "C3", "C#3", "D3", "D#3", "E3", "F3", "F#3", "G3", "G#3", "A4", "A#4", "B4", "C4"}, 0));
    vParams.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("ACFOURTEEN",1), "AccentFourteen", false));
    vParams.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("GLIDEFOURTEEN",1), "GlideFourteen", false));
    
    vParams.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID("NOTEFIFTEEN",1), "NoteFifteen", juce::StringArray {"NONE", "B0", "C0", "C#0", "D0", "D#0", "E0", "F0", "F#0", "G0", "G#0", "A1", "A#1", "B1", "C1", "C#1", "D1", "D#1", "E1", "F1", "F#1", "G1", "G#1", "A2", "A#2", "B2", "C2", "C#2", "D2", "D#2", "E2", "F2", "F#2", "G2", "G#2", "A3", "A#3", "B3", "C3", "C#3", "D3", "D#3", "E3", "F3", "F#3", "G3", "G#3", "A4", "A#4", "B4", "C4"}, 0));
    vParams.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("ACFIFTEEN",1), "AccentFifteen", false));
    vParams.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("GLIDEFIFTEEN",1), "GlideFifteen", false));
    
    vParams.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID("NOTESIXTEEN",1), "NoteSixteen", juce::StringArray {"NONE", "B0", "C0", "C#0", "D0", "D#0", "E0", "F0", "F#0", "G0", "G#0", "A1", "A#1", "B1", "C1", "C#1", "D1", "D#1", "E1", "F1", "F#1", "G1", "G#1", "A2", "A#2", "B2", "C2", "C#2", "D2", "D#2", "E2", "F2", "F#2", "G2", "G#2", "A3", "A#3", "B3", "C3", "C#3", "D3", "D#3", "E3", "F3", "F#3", "G3", "G#3", "A4", "A#4", "B4", "C4"}, 0));
    vParams.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("ACSIXTEEN",1), "AccentSixteen", false));
    vParams.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("GLIDESIXTEEN",1), "GlideSixteen", false));
    //ADSR
    
    vParams.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("ATTACK",1), "Attack", juce::NormalisableRange<float> {0.00001f, 0.1f}, 0.00001f));
    
    vParams.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("DECAY",1), "Decay", juce::NormalisableRange<float> {0.001f, 0.1f}, 0.005f));
    
    vParams.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("SUSTAIN",1), "Sustain", juce::NormalisableRange<float> {0.1f, 1.0f}, 0.0f));
    
    vParams.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("RELEASE",1), "Release", juce::NormalisableRange<float> {0.001f, 3.0f}, 0.20f));
    
    vParams.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("CUTOFF",1), "cut off Hz", 500.1, 34000.0, 5000.0f));
        
    vParams.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("RESONANCE",1), "Resonance", 0.0, 0.8f , 0.5f));
    
    vParams.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("ACCENT",1), "Accent", 0.0, 0.40f , 0.00f));
 
    vParams.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("TUNING",1), "Tuning", -12.0, 12.0f , 0.0f));
    
    vParams.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("ENV",1), "Env", 0.0, 2.1f , 0.9f));
    
    vParams.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("GAIN",1), "Gain", 0.1, 1.0f , 0.9f));
    
    vParams.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("ENVDECAY",1), "EnvDecay", 15.0f, 100.0f , 30.0f));
    
    return{vParams.begin(), vParams.end()};
}



void KY030AudioProcessor::setLadderParams(float accent){
 
    //updating ladder parameters depending on accent settings
    ladderFilter.setEnabled(true);
  
    if (applyAccent == true){
    ladderFilter.setResonance(resonanceParam->load()+ (accent/5));
    }
    else
        ladderFilter.setResonance(resonanceParam->load());

}

int KY030AudioProcessor::getBarDivision(int selection){
    
    //converts selection to useable numbers
    if(selection == 0)
        return 1;
    if(selection == 1)
        return 2;
    if(selection == 2)
        return 4;
    else return 4;
    
}

int KY030AudioProcessor::getMidiNoteNum(int note){
    
    //ofsetting the note for an accurate output
    
    if(note == 0){
        return 0;
    }
    else return note + 13;
}

bool KY030AudioProcessor::notePlaying(){
    
    return nowPlaying;
}

void KY030AudioProcessor::tunefulRandomise(){
    
    //Randomises in key
    
    std::vector<int> vtRandom;
    vtRandom.clear();
   
    auto rootNote = juce::Random::getSystemRandom().nextInt (juce::Range<int> (0, 20));
    
    
    while(vtRandom.size() <= 16){
        
        //Making "no note" a more likley option
        auto note = juce::Random::getSystemRandom().nextInt (juce::Range<int> (0, 20));
        if (note == rootNote + 1 || note == rootNote + 3 || note == rootNote + 6){
            vtRandom.push_back(0);
        }
           
        //ensuring the note is in key
        if (note == rootNote ||note == rootNote + 2||note == rootNote + 4 ||note == rootNote + 5 ||note == rootNote + 7 ||note == rootNote + 9 ||note == rootNote + 11 ||note == rootNote - 1 ||note == rootNote - 3 || note == rootNote - 5 ||note == rootNote - 7 || note == rootNote - 8 || note ==  1){
            
            vtRandom.push_back(note );
        }
    }
    
    
    apvts.getParameter("NOTEONE")->setValueNotifyingHost(rootNote / 41.0);
    apvts.getParameter("NOTETWO")->setValueNotifyingHost(vtRandom.at(3) / 41.0);
    apvts.getParameter("NOTETHREE")->setValueNotifyingHost(vtRandom.at(4) / 41.0);
    apvts.getParameter("NOTEFOUR")->setValueNotifyingHost(vtRandom.at(5) / 41.0);
    apvts.getParameter("NOTEFIVE")->setValueNotifyingHost(vtRandom.at(6) / 41.0);
    apvts.getParameter("NOTESIX")->setValueNotifyingHost(vtRandom.at(7) / 41.0);
    apvts.getParameter("NOTESEVEN")->setValueNotifyingHost(vtRandom.at(8) / 41.0);
    apvts.getParameter("NOTEEIGHT")->setValueNotifyingHost(vtRandom.at(9) / 41.0);
    apvts.getParameter("NOTENINE")->setValueNotifyingHost(vtRandom.at(10) / 41.0);
    apvts.getParameter("NOTETEN")->setValueNotifyingHost(vtRandom.at(11) / 41.0);
    apvts.getParameter("NOTEELEVEN")->setValueNotifyingHost(vtRandom.at(12) / 41.0);
    apvts.getParameter("NOTETWELVE")->setValueNotifyingHost(vtRandom.at(13) / 41.0);
    apvts.getParameter("NOTETHIRTEEN")->setValueNotifyingHost(vtRandom.at(14) / 41.0);
    apvts.getParameter("NOTEFOURTEEN")->setValueNotifyingHost(vtRandom.at(15) / 41.0);
    apvts.getParameter("NOTEFIFTEEN")->setValueNotifyingHost(vtRandom.at(0) / 41.0);
    apvts.getParameter("NOTESIXTEEN")->setValueNotifyingHost(vtRandom.at(1) / 41.0);

}

void KY030AudioProcessor::getRawParamValues(){
    
    cuttoffParam = apvts.getRawParameterValue("CUTOFF");
    gainParam = apvts.getRawParameterValue("GAIN");
    resonanceParam = apvts.getRawParameterValue("RESONANCE");
    followerEffectParameter = apvts.getRawParameterValue("ENV");
    envDecayParameter = apvts.getRawParameterValue("ENVDECAY");
    accentParameter = apvts.getRawParameterValue("ACCENT");
    barDivParameter = apvts.getRawParameterValue("BARDIV");
    noteOneParameter = apvts.getRawParameterValue("NOTEONE");
    noteTwoParameter = apvts.getRawParameterValue("NOTETWO");
    noteThreeParameter = apvts.getRawParameterValue("NOTETHREE");
    noteFourParameter = apvts.getRawParameterValue("NOTEFOUR");
    noteFiveParameter = apvts.getRawParameterValue("NOTEFIVE");
    noteSixParameter = apvts.getRawParameterValue("NOTESIX");
    noteSevenParameter = apvts.getRawParameterValue("NOTESEVEN");
    noteEightParameter = apvts.getRawParameterValue("NOTEEIGHT");
    noteNineParameter = apvts.getRawParameterValue("NOTENINE");
    noteTenParameter = apvts.getRawParameterValue("NOTETEN");
    noteElevenParameter = apvts.getRawParameterValue("NOTEELEVEN");
    noteTwelveParameter = apvts.getRawParameterValue("NOTETWELVE");
    noteThirteenParameter = apvts.getRawParameterValue("NOTETHIRTEEN");
    noteFourteenParameter = apvts.getRawParameterValue("NOTEFOURTEEN");
    noteFifteenParameter = apvts.getRawParameterValue("NOTEFIFTEEN");
    noteSixteenParameter = apvts.getRawParameterValue("NOTESIXTEEN");
    
    accentOneParameter = apvts.getRawParameterValue("ACONE");
    accentTwoParameter = apvts.getRawParameterValue("ACTWO");
    accentThreeParameter = apvts.getRawParameterValue("ACTHREE");
    accentFourParameter = apvts.getRawParameterValue("ACFOUR");
    accentFiveParameter = apvts.getRawParameterValue("ACFIVE");
    accentSixParameter = apvts.getRawParameterValue("ACSIX");
    accentSevenParameter = apvts.getRawParameterValue("ACSEVEN");
    accentEightParameter = apvts.getRawParameterValue("ACEIGHT");
    accentNineParameter = apvts.getRawParameterValue("ACNINE");
    accentTenParameter = apvts.getRawParameterValue("ACTEN");
    accentElevenParameter = apvts.getRawParameterValue("ACELEVEN");
    accentTwelveParameter = apvts.getRawParameterValue("ACTWELVE");
    accentThirteenParameter = apvts.getRawParameterValue("ACTHIRTEEN");
    accentFourteenParameter = apvts.getRawParameterValue("ACFOURTEEN");
    accentFifteenParameter = apvts.getRawParameterValue("ACFIFTEEN");
    accentSixteenParameter = apvts.getRawParameterValue("ACSIXTEEN");
    
    glideOneParameter = apvts.getRawParameterValue("GLIDEONE");
    glideTwoParameter = apvts.getRawParameterValue("GLIDETWO");
    glideThreeParameter = apvts.getRawParameterValue("GLIDETHREE");
    glideFourParameter = apvts.getRawParameterValue("GLIDEFOUR");
    glideFiveParameter = apvts.getRawParameterValue("GLIDEFIVE");
    glideSixParameter = apvts.getRawParameterValue("GLIDESIX");
    glideSevenParameter = apvts.getRawParameterValue("GLIDESEVEN");
    glideEightParameter = apvts.getRawParameterValue("GLIDEEIGHT");
    glideNineParameter = apvts.getRawParameterValue("GLIDENINE");
    glideTenParameter = apvts.getRawParameterValue("GLIDETEN");
    glideElevenParameter = apvts.getRawParameterValue("GLIDEELEVEN");
    glideTwelveParameter = apvts.getRawParameterValue("GLIDETWELVE");
    glideThirteenParameter = apvts.getRawParameterValue("GLIDETHIRTEEN");
    glideFourteenParameter = apvts.getRawParameterValue("GLIDEFOURTEEN");
    glideFifteenParameter = apvts.getRawParameterValue("GLIDEFIFTEEN");
    glideSixteenParameter = apvts.getRawParameterValue("GLIDESIXTEEN");
    
}

void KY030AudioProcessor::applyEnvelopeAndAccentAndGain(juce::AudioBuffer<float>& buffer, float accent, float pbGain){
    
    //applies envelope behaviour depending on accent settings
    
    auto followerEffect = followerEffectParameter->load();
    auto envDecay = envDecayParameter->load();
    
    juce::dsp::AudioBlock<float> sampleBlock (buffer);
    

    for (int sample = 0; sample < buffer.getNumSamples(); sample++) {
        float thisCutoff = 0;
        for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {

            //Calculating and applying the envelope to the filter per sample
            
            envelopeFollower.setAttackAndRelease(0, envDecay / 1000.0 );
            if (applyAccent==true)
            {
                    
                    thisCutoff += cuttoffParam->load() *  envelopeFollower.calculateEnvelope(buffer.getSample(channel, sample) * followerEffect + accent, channel);
               
            
            }
            if (applyAccent==false)
            {
                    thisCutoff += cuttoffParam->load() *  envelopeFollower.calculateEnvelope(buffer.getSample(channel, sample) * followerEffect, channel);
            }
                
            buffer.setSample(channel, sample, buffer.getSample(channel, sample) * pbGain);
        }
        
        //Change settings of filter if necessary
        ladderFilter.setCutoffFrequencyHz(juce::jlimit(0.1, (float) getSampleRate() / 2.0, thisCutoff / (double) buffer.getNumChannels()));
        juce::dsp::AudioBlock<float> subBlock = sampleBlock.getSubBlock(sample, 1);
        ladderFilter.process(juce::dsp::ProcessContextReplacing<float>(subBlock)); // Applying filter
    }
        
}
void KY030AudioProcessor::clearNotes(){
    
    //setting parameters to 0
    
    apvts.getParameter("NOTEONE")->setValueNotifyingHost(0);
    apvts.getParameter("NOTETWO")->setValueNotifyingHost(0);
    apvts.getParameter("NOTETHREE")->setValueNotifyingHost(0);
    apvts.getParameter("NOTEFOUR")->setValueNotifyingHost(0);
    apvts.getParameter("NOTEFIVE")->setValueNotifyingHost(0);
    apvts.getParameter("NOTESIX")->setValueNotifyingHost(0);
    apvts.getParameter("NOTESEVEN")->setValueNotifyingHost(0);
    apvts.getParameter("NOTEEIGHT")->setValueNotifyingHost(0);
    apvts.getParameter("NOTENINE")->setValueNotifyingHost(0);
    apvts.getParameter("NOTETEN")->setValueNotifyingHost(0);
    apvts.getParameter("NOTEELEVEN")->setValueNotifyingHost(0);
    apvts.getParameter("NOTETWELVE")->setValueNotifyingHost(0);
    apvts.getParameter("NOTETHIRTEEN")->setValueNotifyingHost(0);
    apvts.getParameter("NOTEFOURTEEN")->setValueNotifyingHost(0);
    apvts.getParameter("NOTEFIFTEEN")->setValueNotifyingHost(0);
    apvts.getParameter("NOTESIXTEEN")->setValueNotifyingHost(0);
    
    apvts.getParameter("ACONE")->setValueNotifyingHost(0);
    apvts.getParameter("ACTWO")->setValueNotifyingHost(0);
    apvts.getParameter("ACTHREE")->setValueNotifyingHost(0);
    apvts.getParameter("ACFOUR")->setValueNotifyingHost(0);
    apvts.getParameter("ACFIVE")->setValueNotifyingHost(0);
    apvts.getParameter("ACSIX")->setValueNotifyingHost(0);
    apvts.getParameter("ACSEVEN")->setValueNotifyingHost(0);
    apvts.getParameter("ACEIGHT")->setValueNotifyingHost(0);
    apvts.getParameter("ACNINE")->setValueNotifyingHost(0);
    apvts.getParameter("ACTEN")->setValueNotifyingHost(0);
    apvts.getParameter("ACELEVEN")->setValueNotifyingHost(0);
    apvts.getParameter("ACTWELVE")->setValueNotifyingHost(0);
    apvts.getParameter("ACTHIRTEEN")->setValueNotifyingHost(0);
    apvts.getParameter("ACFOURTEEN")->setValueNotifyingHost(0);
    apvts.getParameter("ACFIFTEEN")->setValueNotifyingHost(0);
    apvts.getParameter("ACSIXTEEN")->setValueNotifyingHost(0);
    
    apvts.getParameter("GLIDEONE")->setValueNotifyingHost(0);
    apvts.getParameter("GLIDETWO")->setValueNotifyingHost(0);
    apvts.getParameter("GLIDETHREE")->setValueNotifyingHost(0);
    apvts.getParameter("GLIDEFOUR")->setValueNotifyingHost(0);
    apvts.getParameter("GLIDEFIVE")->setValueNotifyingHost(0);
    apvts.getParameter("GLIDESIX")->setValueNotifyingHost(0);
    apvts.getParameter("GLIDESEVEN")->setValueNotifyingHost(0);
    apvts.getParameter("GLIDEEIGHT")->setValueNotifyingHost(0);
    apvts.getParameter("GLIDENINE")->setValueNotifyingHost(0);
    apvts.getParameter("GLIDETEN")->setValueNotifyingHost(0);
    apvts.getParameter("GLIDEELEVEN")->setValueNotifyingHost(0);
    apvts.getParameter("GLIDETWELVE")->setValueNotifyingHost(0);
    apvts.getParameter("GLIDETHIRTEEN")->setValueNotifyingHost(0);
    apvts.getParameter("GLIDEFOURTEEN")->setValueNotifyingHost(0);
    apvts.getParameter("GLIDEFIFTEEN")->setValueNotifyingHost(0);
    apvts.getParameter("GLIDESIXTEEN")->setValueNotifyingHost(0);
    
    synth.allNotesOff(1, true);
}


void KY030AudioProcessor::synthVoiceandADSR(){
    
    //Not visable in GUI. Sets ADSR of each note playing
    for (int i = 0; i < synth.getNumVoices(); ++i){
        if ( auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i))){
            
            
            auto& attack = *apvts.getRawParameterValue("ATTACK");
            auto& decay = *apvts.getRawParameterValue("DECAY");
            auto& sustain = *apvts.getRawParameterValue("SUSTAIN");
            auto& release = *apvts.getRawParameterValue("RELEASE");
            auto& tuning = *apvts.getRawParameterValue("TUNING");
            
            auto& oscWaveChoice = *apvts.getRawParameterValue("OSC");
            
            
            voice->updateGlide(applyGlide);
            voice->UpdateADSR(attack.load(), decay.load(), sustain.load(), release.load(), tuning.load());
           
            voice->getOscillator().setWaveType (oscWaveChoice.load());
        }
    }
    
    
}

void KY030AudioProcessor::stepSequence(int step, int stepNumber, int pbNoteStop, bool pbAccent, bool pbGlide, int pbNoteStart){
    
    //called each time a note or a empty note is played
    
    if(step == stepNumber){
        
        if(applyGlide == false){
        synth.noteOff(1, pbNoteStop, 1, true);
            apvts.getParameter("DECAY")->setValueNotifyingHost(0.5f);
        }
        if(applyGlide == true){
            apvts.getParameter("DECAY")->setValueNotifyingHost(1.0f);
        }
     
        applyAccent = false;
        if(pbNoteStart != 34  ){
            applyAccent = pbAccent;
            applyGlide = pbGlide;
        synth.noteOn(1, pbNoteStart, 1);
            
        }
        
    }
    
    
}

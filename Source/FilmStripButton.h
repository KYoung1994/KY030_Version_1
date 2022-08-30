/*
  ==============================================================================

    ToggleStrip.h
    Created: 28 Jun 2022 11:25:45pm
    Author:  Kimberly Young

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

using namespace juce;

//=============================================================================
//inheriting from the ToggleButton class to draw the filmstrip to create an animated  button

class FilmStripButton : public ToggleButton
{
public:
    FilmStripButton()
        : ToggleButton(juce::String(0)),
    filmStrip(juce::ImageFileFormat::loadFrom(BinaryData::buttonSwitch_png, BinaryData::buttonSwitch_pngSize)),
            numFrames_(2),
            isHorizontal_(false)
    {
     
        frameHeight = filmStrip.getHeight() / numFrames_;
        frameWidth = filmStrip.getWidth();

        getProperties().set(Identifier("index"), &index);
    }

    void paint(Graphics& g) override
    {
        
        g.fillAll(juce::Colours::transparentBlack);
        
            float value = getToggleState();
            if(isHorizontal_)
            {
                g.drawImage(filmStrip, 0, 0, getWidth(), getHeight(),
                    value/2 * frameWidth, 0, frameWidth, frameHeight / 2);
                
            }
            else
            {
                g.drawImage(filmStrip, 0, 0, getWidth(), getHeight()/2,
                    0, value / 2 * frameHeight, frameWidth, frameHeight / 2);
            }
    }

private:
    Image filmStrip;
    const int numFrames_;
    const bool isHorizontal_;
    int frameWidth, frameHeight;
    int value;
}
;

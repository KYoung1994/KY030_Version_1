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

//inheriting from the ToggleButton class to draw the filmstrip to create an animated toggle button
class FilmStripTog : public ToggleButton
{
public:
    FilmStripTog()
        : ToggleButton(juce::String(0)),
    filmStrip(juce::ImageFileFormat::loadFrom(BinaryData::switch_slide_2_png, BinaryData::switch_slide_2_pngSize)),
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
        
            int value = getToggleState();
            if(isHorizontal_)
            {
                g.drawImage(filmStrip, 0, 0, getWidth(), getHeight(),
                    value * frameWidth, 0, frameWidth, frameHeight);
                
            }
            else
            {
                g.drawImage(filmStrip, 0, 0, getWidth(), getHeight(),
                    0, value * frameHeight, frameWidth, frameHeight);
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

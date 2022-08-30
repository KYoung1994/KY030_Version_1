/*
  ==============================================================================

    PresetPanel.h
    Created: 3 Jul 2022 4:42:36pm
    Author:  Kimberly Young

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace Gui
{
class PresetPanel : public Component, Button::Listener, ComboBox::Listener
{
public:
    PresetPanel(Service::PresetManager& pm): presetManager(pm){
        
        //UI Preset panel configuration
        configureButton(saveButton, "Save");
        configureButton(deleteButton, "Delete");
        configureButton(previousPresetButton, "<-");
        configureButton(nextPresetButton, "->");
        presetList.setColour(0x1000b00, juce::Colours::silver);
        presetList.setTextWhenNothingSelected("Nothing Selected");
        presetList.setMouseCursor(MouseCursor::PointingHandCursor);
        addAndMakeVisible(presetList);
        presetList.addListener (this);
        
        loadPresetList();

    }
    ~PresetPanel(){
        //removing listeners to avoid memory leak
        saveButton.removeListener(this);
        deleteButton.removeListener(this);
        previousPresetButton.removeListener(this);
        nextPresetButton.removeListener(this);
        presetList.removeListener(this);
    }
    
    void resized()override{
        
       //size and position of preset panel object
        saveButton.setBoundsRelative(0.0, 0.01, 0.2, 0.95);
        deleteButton.setBoundsRelative(0.2, 0.01, 0.2, 0.95);
        previousPresetButton.setBoundsRelative(0.4, 0.01, 0.1, 0.95);
        presetList.setBoundsRelative(0.5, 0.01, 0.3, 0.95);
        nextPresetButton.setBoundsRelative(0.8, 0.01, 0.1, 0.95);
    }
private:
    void buttonClicked(Button* button) override{
        
        //perform appropiate functions when putton is clicked
        if( button == &saveButton){
            fileChooser = std::make_unique<FileChooser>(
                                                       "Please enter the name of the preset to save", Service::PresetManager::defaultDirectory, "*." + Service::PresetManager::extension);
            fileChooser->launchAsync(FileBrowserComponent::saveMode, [&] (const FileChooser& chooser)
            {
                const auto resultFile = chooser.getResult();
                presetManager.savePreset(resultFile.getFileNameWithoutExtension());
                loadPresetList();
            });
        }
        if( button == &previousPresetButton){
            const auto index = presetManager.loadPreviousPreset();
            presetList.setSelectedItemIndex(index, juce::dontSendNotification);
        }
        if(button == &nextPresetButton){
            const auto index = presetManager.loadNextPreset();
            presetList.setSelectedItemIndex(index, juce::dontSendNotification);
           }
        if(button == &deleteButton){
            presetManager.deletePreset(presetManager.getCurrentPreset());
            loadPresetList();
        }
    }
    
    
    void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override {
        //update combo box when UI changes
        if(comboBoxThatHasChanged == &presetList){
            presetManager.loadPreset(presetList.getItemText(presetList.getSelectedItemIndex()));
        }
    }
    
    void configureButton(Button& button, const String& buttonText){
        //making buttings for the UI, called in the constructor
        button.setButtonText(buttonText);
        button.setMouseCursor(MouseCursor::PointingHandCursor);
        addAndMakeVisible(button);
        button.addListener(this);
        button.setColour(0x1000100, juce::Colours::silver);
        
    }
    
    void loadPresetList(){
        //loading the preset list, this will be visible in the combobox
        presetList.clear(juce::dontSendNotification);
        const auto allPresets = presetManager.getAllPresets();
        const auto currentPreset = presetManager.getCurrentPreset();
        presetList.addItemList(allPresets, 1);
        presetList.setSelectedItemIndex(allPresets.indexOf(currentPreset), juce::dontSendNotification);
    }
    
    TextButton saveButton, deleteButton, previousPresetButton, nextPresetButton;
    ComboBox presetList;
    Service::PresetManager& presetManager;
    
    std::unique_ptr<FileChooser> fileChooser;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PresetPanel)
};
}

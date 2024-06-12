#pragma once

#include <JuceHeader.h>
#include "../../EZ_DSP/EZ_DSP.h"
//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent,
                        public juce::Slider::Listener,
                        public juce::ComboBox::Listener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;
    void sliderValueChanged(juce::Slider* slider)override
    {
        if (slider == &param_1_slider){
            param_1 = param_1_slider.getValue();
        }else if (slider == &param_2_slider){
            param_2 = param_2_slider.getValue();
        }else if (slider == &param_3_slider){
            param_3 = param_3_slider.getValue();
        }else if (slider == &osc_freq_slider){
            osc_freq = osc_freq_slider.getValue();
        }else if (slider == &osc_amp_slider){
            osc_amp = osc_amp_slider.getValue();
        }else if (slider == &param_5_slider){
            param_5 = param_5_slider.getValue();
        }
    }
    void comboBoxChanged(juce::ComboBox* box) override {
        if (box == &osc_type_box){
            osc_type = osc_type_box.getSelectedIdAsValue().getValue();
        }else if (box == &param_4_box){
            param_4_value = param_4_box.getSelectedIdAsValue().getValue();
        }
    }
    //bool keyPressed(const juce::KeyPress &key) override;
    bool keyStateChanged(bool isKeyDown) override;
    

private:
    //==============================================================================
    // Your private member variables go here...
    EZ_DSP::Oscillator osc;
    EZ_DSP::SynthVoice voices[8];
    int midi_note = 60;
    int octave_shifter = 0;
    //EZ_DSP::Phaser phaser;
    
    int osc_type = 1; float osc_freq = 1000.f; float osc_amp = 1.f;
    float param_1 = 10000.f; float param_2 = 2.f; float param_3 = 0.f;
    float param_5 = 0.f;
    int param_4_value = 1;
    juce::Slider osc_freq_slider, osc_amp_slider,
                param_1_slider, param_2_slider, param_3_slider, param_5_slider;
    juce::ComboBox osc_type_box, param_4_box;
    float sliderHeight = 20; float sliderWidth = 200; float gap = 10;
    const juce::StringArray osc_types = {"SINE","TRIANGLE","SAW","SQUARE"};
    const juce::StringArray param_4_types = {"LOWPASS","HIGHPASS","BANDPASS",
                                        "NOTCH","LOWSHELF","HIGHSHELF","PEAK"};
    //const juce::StringArray param_4_types = {"1","2","3","4","5"};
    const juce::StringArray param_names = {"CUTOFF", "Q", "Gain"};
    juce::ToggleButton bypass;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

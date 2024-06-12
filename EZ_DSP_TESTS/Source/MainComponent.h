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
                        public juce::ComboBox::Listener,
                        public juce::Timer
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
        }else if (slider == &atk_slider){
            attack = atk_slider.getValue();
        }else if (slider == &dec_slider){
            decay = dec_slider.getValue();
        }else if (slider == &param_5_slider){
            param_5 = param_5_slider.getValue();
        }else if (slider == &sus_slider){
            sustain = sus_slider.getValue();
        }else if (slider == &rel_slider){
            release = rel_slider.getValue();
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
    void timerCallback() override;
    

private:
    //==============================================================================
    // Your private member variables go here...
    EZ_DSP::SynthVoice voices[8];
    int midi_note = 60;
    int octave_shifter = 0;
    //EZ_DSP::Phaser phaser;
    
    int osc_type = 1; float attack = 100.f; float decay = 100.f; float sustain = 1.f; float release = 100.f;
    float param_1 = 10000.f; float param_2 = 2.f; float param_3 = 0.f;
    float param_5 = 0.f;
    int param_4_value = 1;
    juce::Slider atk_slider, dec_slider, sus_slider, rel_slider,
                param_1_slider, param_2_slider, param_3_slider, param_5_slider;
    juce::ComboBox osc_type_box, param_4_box;
    float sliderHeight = 20; float sliderWidth = 200; float gap = 10;
    const juce::StringArray osc_types = {"SINE","TRIANGLE","SAW","SQUARE"};
    const juce::StringArray param_4_types = {"LOWPASS","HIGHPASS","BANDPASS",
                                        "NOTCH","LOWSHELF","HIGHSHELF","PEAK"};
    //const juce::StringArray param_4_types = {"1","2","3","4","5"};
    const juce::StringArray param_names = {"CUTOFF", "Q", "Gain"};
    juce::ToggleButton bypass;
    juce::String oct_text = juce::String("");
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

#include "MainComponent.h"
#include <sstream>

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);
    Timer::startTimerHz(30);
    
    dec_slider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    dec_slider.setRange(0.f, 1000.f);
    dec_slider.setName(juce::String("Decay"));
    atk_slider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    atk_slider.setName(juce::String("Attack"));
    atk_slider.setRange(0.f, 1000.f);
    sus_slider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    sus_slider.setRange(0.f, 1.f);
    sus_slider.setName(juce::String("Sustain"));
    rel_slider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    rel_slider.setRange(0.f, 10000.f);
    rel_slider.setName(juce::String("Release"));
    param_1_slider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    param_1_slider.setRange(0.01f, 5.f);
    param_2_slider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    param_2_slider.setRange(0.f, 1.f);
    param_3_slider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    param_3_slider.setRange(-25.f, 25.f);
    param_5_slider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    param_5_slider.setRange(10.f, 15000.f);
    
    osc_type_box.setTextWhenNothingSelected(juce::String("OSC Type"));
    param_4_box.setTextWhenNothingSelected(juce::String("Filter Type"));
    osc_type_box.addItemList(osc_types, 1);
    param_4_box.addItemList(param_4_types, 1);
    
    param_1_slider.addListener(this);
    param_2_slider.addListener(this);
    param_3_slider.addListener(this);
    param_5_slider.addListener(this);
    param_4_box.addListener(this);
    dec_slider.addListener(this);
    atk_slider.addListener(this);
    osc_type_box.addListener(this);
    rel_slider.addListener(this);
    sus_slider.addListener(this);
    bypass.setToggleState(false, juce::NotificationType::sendNotification);
    addAndMakeVisible(bypass);
    addAndMakeVisible(dec_slider);
    addAndMakeVisible(atk_slider);
    addAndMakeVisible(osc_type_box);
    addAndMakeVisible(param_1_slider);
    addAndMakeVisible(param_2_slider);
    addAndMakeVisible(param_3_slider);
    addAndMakeVisible(param_4_box);
    addAndMakeVisible(param_5_slider);
    addAndMakeVisible(rel_slider);
    addAndMakeVisible(sus_slider);
    
    
    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
    
    for (int i = 0; i < 8; i++){
        voices[i].init(sampleRate);
        voices[i].setADSR(100.f, 0.f, 1.f, 1000.f);
    }
    
        
    //phaser.init(sampleRate);
    //phaser.setDryWet(1.f);
    
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    for (int i = 0; i < 8; i++){
        voices[i].setWaveType(osc_type-1);
        voices[i].setSustain(sustain);
        voices[i].setAttack(attack);
        voices[i].setRelease(release);
        voices[i].setDecay(decay);
    }
    
    //phaser.setFreq(param_5);
    //phaser.setFeedback(param_3);
    //phaser.setLfoDepth(param_2);
    //phaser.setLfoFrequency(param_1);
    //phaser.setNumPoles(param_4_value);

    bufferToFill.clearActiveBufferRegion();
    auto buffer = bufferToFill.buffer;
    //auto* leftChannel = buffer->getWritePointer(0);
    //auto* rightChannel =  buffer->getWritePointer(1);
    for (int i=0; i < buffer->getNumSamples();i++){
        
        float osc_val = 0.f;
        for (int i = 0; i < 8; i++){
            osc_val += voices[i].tick();
        }
        //float left_sample = leftChannel[i];
        //float right_sample = rightChannel[i];
        float effectedL = osc_val;
        float effectedR = effectedL;
        float out_sampleL = bypass.getToggleStateValue().getValue() ? osc_val : effectedL;
        float out_sampleR = bypass.getToggleStateValue().getValue() ? osc_val : effectedR;
        //float out_sampleL = osc_val;
        //float out_sampleR = osc_val;
        buffer->setSample(0, i, out_sampleL);
        buffer->setSample(1, i, out_sampleR);
    }
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setColour(juce::Colours::white);
    g.drawText(juce::String("Octave Shift: " + std::to_string(octave_shifter)), 10, 200, 100, 30, juce::Justification::centred);
    g.drawText(juce::String("Atk"), 205, 40, 20,20,juce::Justification::centred);
    g.drawText(juce::String("Dec"), 202, 70, 27,20,juce::Justification::centred);
    g.drawText(juce::String("Sus"), 205, 100, 20,20,juce::Justification::centred);
    g.drawText(juce::String("Rel"), 205, 130, 20,20,juce::Justification::centred);
    // You can add your drawing code here!
}

void MainComponent::resized()
{
    osc_type_box.setBounds(10, 10, sliderWidth, 20);
    atk_slider.setBounds(10, osc_type_box.getBottom()+gap, sliderWidth, sliderHeight);
    dec_slider.setBounds(10, atk_slider.getBottom()+gap, sliderWidth, sliderHeight);
    sus_slider.setBounds(10, dec_slider.getBottom()+gap, sliderWidth, sliderHeight);
    rel_slider.setBounds(10, sus_slider.getBottom()+gap, sliderWidth, sliderHeight);
    
    float params_x = osc_type_box.getRight()+gap*2;
    param_4_box.setBounds(params_x, 10, sliderWidth, 20);
    param_1_slider.setBounds(params_x, param_4_box.getBottom(), sliderWidth, sliderHeight);
    param_2_slider.setBounds(params_x, param_1_slider.getBottom(), sliderWidth, sliderHeight);
    param_3_slider.setBounds(params_x, param_2_slider.getBottom(), sliderWidth, sliderHeight);
    param_5_slider.setBounds(params_x, param_3_slider.getBottom(), sliderWidth, sliderHeight);
    bypass.setBounds(500, 10, 100, 100);
    
    
}
/*
bool MainComponent::keyPressed(const juce::KeyPress &key){
    switch (key.getKeyCode()){
        case 65: //a
            break;
        case 87: //w
            break;
        case 83: //s
            break;
        case 69: //e
            break;
        case 68: //d
            break;
        case 70: //f
            break;
        case 84: //t
            break;
        case 71: //g
            break;
        case 89: //y
            break;
        case 72: //h
            break;
        case 85: //u
            break;
        case 74: //j
            break;
        case 75: //k
            break;
        case 79: //o
            break;
        case 76: //l
            break;
        case 80: //p
            break;
        case 186: //;
            break;
        case 222: //'
            break;
        case 90: //z
            break;
        case 88: //x
            break;
    };
    return true;
}
*/
bool MainComponent::keyStateChanged(bool isKeyDown){
    if (isKeyDown){
        if (juce::KeyPress::isKeyCurrentlyDown(65)){
            voices[0].noteOn(60+octave_shifter);
        }
        if (juce::KeyPress::isKeyCurrentlyDown(90)){
            octave_shifter -= 12;
        }
        if (juce::KeyPress::isKeyCurrentlyDown(88)){
            octave_shifter += 12;
        }
    }else{
        if (!juce::KeyPress::isKeyCurrentlyDown(65)){
            voices[0].noteOff();
        }
    }
    return true;
}

void MainComponent::timerCallback(){
    repaint();
}

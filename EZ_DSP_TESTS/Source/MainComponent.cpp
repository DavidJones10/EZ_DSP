#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);
    
    osc_amp_slider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    osc_amp_slider.setRange(0.f, 1.f);
    osc_amp_slider.setName(juce::String("OSC AMP"));
    osc_freq_slider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    osc_freq_slider.setName(juce::String("OSC FREQ"));
    osc_freq_slider.setRange(20.f, 20000.f);
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
    osc_amp_slider.addListener(this);
    osc_freq_slider.addListener(this);
    osc_type_box.addListener(this);
    bypass.setToggleState(false, juce::NotificationType::sendNotification);
    addAndMakeVisible(bypass);
    addAndMakeVisible(osc_amp_slider);
    addAndMakeVisible(osc_freq_slider);
    addAndMakeVisible(osc_type_box);
    addAndMakeVisible(param_1_slider);
    addAndMakeVisible(param_2_slider);
    addAndMakeVisible(param_3_slider);
    addAndMakeVisible(param_4_box);
    addAndMakeVisible(param_5_slider);
    
    
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
    trem.init(sampleRate);
    
    osc.init(sampleRate);
    osc.setWaveType(EZ_DSP::Oscillator::WaveType::SAW);
    osc.setFreq(osc_freq);
    osc.setAmplitude(1.f);
        
    //phaser.init(sampleRate);
    //phaser.setDryWet(1.f);
    
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    osc.setWaveType(osc_type-1);
    osc.setAmplitude(osc_amp);
    osc.setFreq(osc_freq);
    
    //phaser.setFreq(param_5);
    //phaser.setFeedback(param_3);
    //phaser.setLfoDepth(param_2);
    //phaser.setLfoFrequency(param_1);
    //phaser.setNumPoles(param_4_value);
    trem.setRate(param_1);
    trem.setDepth(param_2);
    bufferToFill.clearActiveBufferRegion();
    auto buffer = bufferToFill.buffer;
    //auto* leftChannel = buffer->getWritePointer(0);
    //auto* rightChannel =  buffer->getWritePointer(1);
    
    for (int i=0; i < buffer->getNumSamples();i++){

        float osc_val = osc.tick();
        //float left_sample = leftChannel[i];
        //float right_sample = rightChannel[i];
        float effectedL = trem.process(osc_val);
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

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    osc_type_box.setBounds(10, 10, sliderWidth, 20);
    osc_freq_slider.setBounds(10, osc_type_box.getBottom()+gap, sliderWidth, sliderHeight);
    osc_amp_slider.setBounds(10, osc_freq_slider.getBottom()+gap, sliderWidth, sliderHeight);
    
    float params_x = osc_type_box.getRight()+gap*2;
    param_4_box.setBounds(params_x, 10, sliderWidth, 20);
    param_1_slider.setBounds(params_x, param_4_box.getBottom(), sliderWidth, sliderHeight);
    param_2_slider.setBounds(params_x, param_1_slider.getBottom(), sliderWidth, sliderHeight);
    param_3_slider.setBounds(params_x, param_2_slider.getBottom(), sliderWidth, sliderHeight);
    param_5_slider.setBounds(params_x, param_3_slider.getBottom(), sliderWidth, sliderHeight);
    bypass.setBounds(500, 10, 100, 100);
    
    
}

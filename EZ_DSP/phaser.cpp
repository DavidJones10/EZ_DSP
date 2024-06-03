#include "phaser.h"
#include "dsp.h"

using namespace EZ_DSP;
// Phaser Engine
void PhaserEngine::init(float sample_rate)
{
    sample_rate = sampleRate;
    lfo.init(sample_rate);
    delLine.init();
    feedback = .3f;
    setFreq(200.f);
    delLine.setDelay(0.f);

    freqOffset = 30.f; // 30Hz
    delTime = 0.f;
    last_sample = 0.f;
    setLfoFrequency(.3f);
    setLfoDepth(.9f);
}

float PhaserEngine::process(float input)
{
    fonepole(delTime, sampleRate / (lfo.tick() + allpassFreq + freqOffset), .0001f);
    last_sample = delLine.allpass(input + feedback*last_sample, delTime, .3f);
    return (1.f-dryWet) * input + dryWet * last_sample;
}

void PhaserEngine::setFeedback(float fback)
{
    feedback = fclamp(fback, 0.f, .75f);
}

void PhaserEngine::setLfoFrequency(float frequency)
{
    frequency = fclamp(frequency, 0.f, 10.f);
    lfo.setFrequency(frequency);
}

void PhaserEngine::setLfoDepth(float depth)
{
    lfoAmp = fclamp(depth,0.f, 1.f);
    lfo.setAmplitude(lfoAmp*allpassFreq);
}

void PhaserEngine::setFreq(float apFreq)
{
    allpassFreq = fclamp(apFreq, 0.f, 20000.f);
    lfo.setAmplitude(lfoAmp*allpassFreq);
}

void PhaserEngine::setDryWet(float wet)
{
    dryWet = fclamp(wet,0.f,1.f);
}


// Actual Phaser effect

void Phaser::init(float sample_rate)
{
    for (int i=0; i < maxPoles; i++)
    {
        engines[i].init(sample_rate);
    }
    numPoles = 4;
}

float Phaser::process(float input)
{
    float out = 0.f;
    for (int i=0; i < numPoles; i++)
    {
        out += engines[i].process(input);
    } 
    return out / numPoles;
}

void Phaser::setNumPoles(int poles)
{
    numPoles = clampInt(poles,1, 8);
}

void Phaser::setFeedback(float fback)
{
    for (int i=0; i < maxPoles; i++)
    {
        engines[i].setFeedback(fback);
    }    
}

void Phaser::setLfoDepth(float depth)
{
    for (int i=0; i < maxPoles; i++)
    {
        engines[i].setLfoDepth(depth);
    }
}

void Phaser::setLfoFrequency(float frequency)
{
    for (int i=0; i < maxPoles; i++)
    {
        engines[i].setLfoFrequency(frequency);
    }
}

void Phaser::setFreq(float allpassFreq){
    for (int i=0; i < maxPoles; i++)
    {
        engines[i].setFreq(allpassFreq);
    }
}

void Phaser::setDryWet(float wet)
{
    for (int i=0; i < maxPoles; i++)
    {
        engines[i].setDryWet(wet);
    }
}



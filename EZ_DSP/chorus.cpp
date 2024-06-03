#include "chorus.h"
#include "dsp.h"

using namespace EZ_DSP;
// Chorus Engine: use 2 for normal stereo effect in chorus
void ChorusEngine::init(float sample_rate)
{
    sampleRate = sample_rate;
    lfo.init(sampleRate);
    delLine.init();
    LfoAmp  = 0.f;
    feedback = .2f;
    setDelay(.75);
    setLfoFrequency(.3f);
    setLfoDepth(.9f);
}

float ChorusEngine::process(float input)
{
    float out = delLine.read(lfo.tick()+delay);
    
    delLine.write(input + out * feedback);
    
    return input *(1.f-dryWet) + out*dryWet; 
}

void ChorusEngine::setLfoDepth(float depth)
{
    depth = fclamp(depth, 0.f, 1.f);
    LfoAmp = depth * delay;
    lfo.setAmplitude(LfoAmp);
}

void ChorusEngine::setFeedback(float fback)
{
    feedback = fclamp(fback, 0.f, .99f);
    feedback *= .97f; // keeps feedback from being too high
}

void ChorusEngine::setLfoFrequency(float freq)
{
    freq = fclamp(freq, 0.f, 10.f);
    lfo.setFrequency(freq);
}

void ChorusEngine::addPhaseOffset(float offset)
{
    offset = fclamp(offset, 0.f, 1.f);
    lfo.add_phase_offset(offset*20.f);
}

void ChorusEngine::setDelay(float delayScaled)
{
    delayScaled = fclamp(delayScaled, 0.f, 1.f);
    delayScaled = (10.1f + delayScaled * 34.9f); //scaled between 10 and 35ms
    setDelayMs(delayScaled);
}

void ChorusEngine::setDelayMs(float ms)
{
    ms = fclamp(ms, 10.f, 35.f);
    delay = ms * .001f * sampleRate;
    LfoAmp = fmin(LfoAmp, delay);
    lfo.setAmplitude(LfoAmp);
}

void ChorusEngine::setDryWet(float wet)
{
    dryWet = fclamp(wet,0.f,1.f);
}

//=================================================================
// Actual stereo chorus
void Chorus::init(float sample_rate)
{
    engines[0].init(sample_rate);
    engines[1].init(sample_rate);
    signalL = signalR = 0.f;
    gainFraction = .5f;
}
// Returns a mono sample, will be the left channel
float Chorus::process(float input)
{
    signalL = engines[0].process(input)*gainFraction;
    signalR = engines[1].process(input)*gainFraction;
    return signalL;
}
// Returns the sample in the right channel
float Chorus::getRight()
{
    return signalR;
}


void Chorus::setLfoDepth(float depthLeft, float depthRight)
{
    engines[0].setLfoDepth(depthLeft);
    engines[1].setLfoDepth(depthRight);
}

void Chorus::setLfoDepth(float depth)
{
    setLfoDepth(depth, depth);
}

void Chorus::setLfoFreq(float freqLeft, float freqRight)
{
    engines[0].setLfoFrequency(freqLeft);
    engines[1].setLfoFrequency(freqRight);
}

void Chorus::setLfoFreq(float freq)
{
    setLfoFreq(freq, freq);
}

void Chorus::setDelay(float delayLeft, float delayRight)
{
    engines[0].setDelay(delayLeft);
    engines[1].setDelay(delayRight);
}

void Chorus::setDelay(float delay)
{
    setDelay(delay, delay);
}

void Chorus::setDelayMs(float msLeft, float msRight)
{
    engines[0].setDelayMs(msLeft);
    engines[1].setDelayMs(msRight);
}

void Chorus::setDelayMs(float ms)
{
    setDelayMs(ms, ms);
}

void Chorus::setFeedback(float fbackLeft, float fbackRight)
{
    engines[0].setFeedback(fbackLeft);
    engines[1].setFeedback(fbackRight);

}

void Chorus::setFeedback(float fback)
{
    setFeedback(fback, fback);
}

void Chorus::setPhaseOffset(float offset)
{
    engines[1].addPhaseOffset(offset);
}

void Chorus::setDryWet(float wet)
{
    engines[0].setDryWet(wet);
    engines[1].setDryWet(wet);
}

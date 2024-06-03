#include "flanger.h"
#include "dsp.h"

using namespace EZ_DSP;

void Flanger::init(float sample_rate)
{
    sampleRate = sample_rate;
    osc.init(sample_rate);
    setFeedback(.2f);
    delLine.init();
    LfoAmp = 0.f;
    setDelay(.75f);
    setLfoFrequency(.3f);
    setLfoDepth(.9f);
}

float Flanger::process(float in)
{
    float out = delLine.read(1.f + osc.tick() + delay);
    delLine.write(in + out * feedback);
    
    return in *(1.f-dryWet) + out*dryWet; 
}

void Flanger::setFeedback(float fback)
{
    feedback = fclamp(fback, 0.f, 1.f);
    feedback *= .97f; // keeps feedback from being too high
}

void Flanger::setDelay(float del)
{
    del = fclamp(del, 0.f, 1.f);
    del = (.1f + del * 6.9f);
    setDelayMs(del);
}

void Flanger::setDelayMs(float delayMs)
{
    delayMs = fclamp(delayMs, .1, 10.f);
    delay = delayMs * .001f * sampleRate;
    LfoAmp = fmin(LfoAmp, delay);
    osc.setAmplitude(LfoAmp);
}

void Flanger::setLfoDepth(float depth)
{
    depth = fclamp(depth, 0.f, .95f);
    LfoAmp = depth * delay;
    osc.setAmplitude(LfoAmp);
}

void Flanger::setLfoFrequency(float freq)
{
    freq = fclamp(freq, .1f, 5.f);
    osc.setFrequency(freq);
}

void Flanger::setDryWet(float wet)
{
    dryWet = fclamp(wet,0,.99f);
}


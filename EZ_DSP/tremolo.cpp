#include "tremolo.h"

using namespace EZ_DSP;

void Tremolo::init(float sample_rate)
{
    osc.init(sample_rate);
    setDepth(1.f);
    setRate(1.f);
}

float Tremolo::process(float input)
{
    float modulator = osc.tick() + dc_offset;
    return (modulator * input);
}

void Tremolo::setRate(float rate)
{
    rate = fclamp(rate, 0.f, 10.f);
    osc.setFrequency(rate);
}

void Tremolo::setDepth(float depth)
{
    depth = fclamp(depth, 0.f, 1.f);
    depth *= .5f;
    osc.setAmplitude(depth);
    dc_offset = 1.0f - depth;
}


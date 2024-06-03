/*
  ==============================================================================

    wave_table_osc.h
    Created: 29 May 2024 4:18:53pm
    Author:  David Jones

  ==============================================================================
*/

#pragma once
#ifndef WAVE_TABLE_OSC_h
#define WAVE_TABLE_OSC_h
#include "dsp.h"
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
namespace EZ_DSP{
// Should be double the highest desired frequency
template <size_t max_number_samples>
class WaveTableOsc{
public:
    void init(float sample_rate){
        setFrequency(1.f);
        setAmplitude(1.f);
        calculate_wavetable();
        sampleRateInv = 1.f / sample_rate;
        phase_offset = 0.f;
    }
    
    float tick(){
        if (phase + phase_offset >= max_number_samples){
            phase -= max_number_samples;
        }
        float out_sample = lerp_from_phase(phase+phase_offset);
        phase += phase_step;
        return out_sample * amplitude;
    }
    
    void setFrequency(float frequency){
        phase_step = frequency * sampleRateInv * max_number_samples;
    }
    
    void setAmplitude(float amplitude_){
        amplitude = amplitude_;
    }
    
    void add_phase_offset(float offset){
        phase_offset = offset;
    }
private:
    
    void calculate_wavetable(){
        for (int i=0; i < max_number_samples; i++){
            waveTable[i] = sinf(TWOPI_F*i/max_number_samples);
        }
    }
    
    float lerp_from_phase(float phase_){
        float intpart,fractpart;
        float truncated = trunc(phase_);
        float floor_samp = get_wave_sample(static_cast<size_t>(truncated));
        float ceil_samp = get_wave_sample(static_cast<size_t>(truncated+1));
        fractpart = modf(phase_, &intpart);
        return floor_samp * (1.f-fractpart) + ceil_samp*fractpart;
    }
    
    float get_wave_sample(size_t index){
        index = index % max_number_samples;
        return waveTable[index];
    }
    
    float waveTable[max_number_samples];
    float amplitude, phase_step, sampleRateInv, phase, phase_offset;
};
}; // namespace EZ_DSP

#endif

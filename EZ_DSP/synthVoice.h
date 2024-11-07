/*
  ==============================================================================

    synthVoice.h
    Created: 11 Jun 2024 6:48:07pm
    Author:  David Jones

  ==============================================================================
*/

#pragma once
#ifndef SYNTH_VOICE_H
#define SYNTH_VOICE_H
#include "adsr.h"
#include "oscillator.h" //TODO replace with wavetable osc after support for other wave types
// TODO add note bending

namespace EZ_DSP{
struct SynthVoice{
public:
    void init(float sampleRate);
    float tick();
    void noteOn(int midi_note);
    void noteOff();
    void setWaveType(uint8_t wave_type);
    void setAttack(float attack_);
    void setDecay(float decay_);
    void setSustain(float sustain_);
    void setRelease(float release_);
    void setADSR(float attack_, float decay_, float sustain_, float release_);
    inline bool isActive(){
        return adsr.isActive();
    }
    int stateAsInt(){
        return adsr.stateAsInt();
    }
    int note;
private:
    Oscillator osc;
    Adsr adsr;

};

}; // namespace EZ_DSP

#endif //SYNTH_VOICE_H

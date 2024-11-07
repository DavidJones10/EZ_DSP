/*
  ==============================================================================

    synthVoice.cpp
    Created: 11 Jun 2024 6:48:07pm
    Author:  David Jones

  ==============================================================================
*/

#include "synthVoice.h"
#include "dsp.h"

using namespace EZ_DSP;

 void SynthVoice::init(float sampleRate){
    osc.init(sampleRate);
    osc.setWaveType(0);
    osc.setAmplitude(1.f);
    adsr.init(sampleRate);
    setADSR(100.f, 100.f, 1.f, 200.f);
}

 float SynthVoice::tick(){
    return osc.tick()*adsr.tick();
}

 void SynthVoice::setWaveType(uint8_t wave_type){
    osc.setWaveType(wave_type);
}

void SynthVoice::noteOn(int midi_note){
    osc.setFreq(midiToHz(midi_note));
    adsr.noteOn();
}

void SynthVoice::noteOff(){
    adsr.noteOff();
}

 void SynthVoice::setAttack(float attack_){
    adsr.setAttack(attack_);
}

 void SynthVoice::setDecay(float decay_){
    adsr.setDecay(decay_);
}

 void SynthVoice::setSustain(float sustain_){
    adsr.setSustain(sustain_);
}

 void SynthVoice::setRelease(float release_){
    adsr.setRelease(release_);
}

 void SynthVoice::setADSR(float attack_, float decay_, float sustain_, float release_){
    adsr.setADSR(attack_, decay_, sustain_, release_);
}

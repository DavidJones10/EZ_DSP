/*
  ==============================================================================

    adsr.cpp
    Created: 11 Jun 2024 6:47:33pm
    Author:  David Jones

  ==============================================================================
*/

#include "adsr.h"
#include "dsp.h"

using namespace EZ_DSP;


 void Adsr::init(float sample_rate){
    sampleRate = sample_rate;
    state = Adsr::AdsrState::INACTIVE;
}

float Adsr::tick(){
    
    switch (state){
        case Adsr::AdsrState::ATTACK:
            envelope_value += atk_step;
            if (envelope_value >= 1.f){
                envelope_value = 1.f;
                state = Adsr::AdsrState::DECAY;
            }
            break;
        case Adsr::AdsrState::DECAY:
            envelope_value -= decay_step;
            if (envelope_value <= sustain_value){
                envelope_value = sustain_value;
                state = Adsr::AdsrState::SUSTAIN;
            }
            break;
        case Adsr::AdsrState::SUSTAIN:
            envelope_value = sustain_value;
            break;
        case Adsr::AdsrState::RELEASE:
            envelope_value -= release_step;
            if (envelope_value <=0.f){
                envelope_value = 0.f;
                state = Adsr::AdsrState::INACTIVE;
            }
            break;
        case Adsr::AdsrState::INACTIVE:
            envelope_value = 0.f;
            break;
    }
    return envelope_value;
}

 void Adsr::noteOn(){
    state = Adsr::AdsrState::ATTACK;
}

 void Adsr::noteOff(){
    state = Adsr::AdsrState::RELEASE;
}

 void Adsr::setAttack(float attack_){
    attack_ = fclamp(attack_, 0.01, 100000.f);
    atk_step = getStep(attack_, 1.f);
}

 void Adsr::setDecay(float decay_){
    decay_ = fclamp(decay_, 01.f, 100000.f);
    decay_step = getStep(decay_, 1.f-sustain_value);
}

 void Adsr::setSustain(float sustain_){
    sustain_value = fclamp(sustain_, 0.f, 1.f);
}

 void Adsr::setRelease(float release_){
    release_ = fclamp(release_, 0.01f, 100000.f);
    release_step = getStep(release_, sustain_value);
}

 void Adsr::setADSR(float attack_, float decay_, float sustain_, float release_){
    setSustain(sustain_);
    setAttack(attack_);
    setDecay(decay_);
    setRelease( release_);
}

 float Adsr::getStep(float time_ms, float distance){
    return distance / (time_ms*0.001f*sampleRate);
}



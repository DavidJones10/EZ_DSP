/*
  ==============================================================================

    adsr.h
    Created: 11 Jun 2024 6:47:33pm
    Author:  David Jones

  ==============================================================================
*/

#pragma once
#ifndef ADSR_H
#define ADSR_H

namespace EZ_DSP{
struct Adsr{
    enum AdsrState{
        ATTACK,  // 0
        DECAY,   // 1
        SUSTAIN, // 2
        RELEASE, // 3
        INACTIVE // 4
    };
public:
    // Sustain must be set before decay and release
    void init(float sample_rate);
    float tick();
    void noteOn();
    void noteOff();
    // Sets attack in ms
    void setAttack(float attack_);
    void setDecay(float decay_);
    
    void setSustain(float sustain_);
    void setRelease(float release_);
    void setADSR(float attack_, float decay_, float sustain_, float release_);
    inline bool isActive(){
        if (state==AdsrState::INACTIVE){
            return false;
        }else{
            return true;
        }
    }
    int stateAsInt(){
        return static_cast<int>(state);
    }
    
private:
    float sampleRate,sustain_value;
    float atk_step, decay_step, release_step;
    float envelope_value;
    inline float getStep(float time_ms, float distance);
    AdsrState state;
    
};//Adsr
};//EZ_DSP

#endif //ADSR_H

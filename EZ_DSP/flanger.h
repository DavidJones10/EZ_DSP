#pragma once
#ifndef EZ_DSP_FLANGER_H
#define EZ_DSP_FLANGER_H

#include "delay_line.h"
#include "wave_table_osc.h"

namespace EZ_DSP
{
class Flanger 
{
    public:
        /** Initializes variables for Flanger effect
            \param sample_rate sample rate for audio engine
        */
        void init(float sample_rate);

        /** Processes input and returns flanger-effected output
         * \param input input sample 
        */
        float process(float input);

        /** Sets feedback for internal delay 
            \param fback expects 0-1 float value
        */
        void setFeedback(float fback);

        /** Sets amplitude of modulator
            \param depth expects 0-1 float value
        */
        void setLfoDepth(float depth);

        /** Sets modulation rate
            \param frequency Hz value between 0 and 5Hz
        */
        void setLfoFrequency(float frequency);

        /** Sets internal delay as a float range
            \param delayInSamples expects value from 0-1 Scales to .1-7ms
        */
        void setDelay(float delayInSamples);

        /** Sets internal delay in ms
            \param delayMS expects value in ms between .1 and 10ms
        */
        void setDelayMs(float delayMS);

        /** Sets dry wet for flanger effect
            \param wet expects value from 0-1
        */
        void setDryWet(float wet);

    private:
        float feedback;
        float delay;
        float sampleRate;
        float dryWet;
        float LfoAmp;
        static constexpr int32_t maxDelaySize = 960; // .02 * 48000 max size of 20ms
        DelayLine<float,maxDelaySize> delLine;
        WaveTableOsc<256> osc;

};
}
#endif


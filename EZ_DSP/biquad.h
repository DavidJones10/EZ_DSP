#pragma once
#ifndef EZ_DSP_BIQUAD_H
#define EZ_DSP_BIQUAD_H
#include "dsp.h"

namespace EZ_DSP
{
class Biquad
{
public:
    /** Initializes the filter 
     * @param sample_rate 
     */
    void init(float sample_rate);
    /** Processes input sample through biquad filter
     * @param input_sample 
     * @return filtered sample 
     */
    float process(float input_sample);
    /**
     * @brief Sets the resonance of the filter
     * 
     * @param q Quality of the filter
     */
    inline void setQ(float q_)
    {
        q = q_;
        calculateCoefficients();
    }
    /**
     * @brief Set the Center Frequency of the filter
     * 
     * @param centerFreq_ 20-20000 Hz
     */
    inline void setCenterFreq(float centerFreq_)
    {
        centerFreq_ = fclamp(cutoff,20.f, 20000.f);
        cutoff = centerFreq_;
        calculateCoefficients();
    }
    /**
     * @brief Set the Filter Type of the biquad filter
     * 
     * @param type from EZ_DSP::FilterType
     */
    inline void setFilterType(FilterType type)
    {
        filterType = type;
        calculateCoefficients();
    }
    /**
     * @brief Set the Gain of the filter
     * 
     * @param dbGain 
     */
    inline void setGain(float dbGain)
    {
        dBGain = dbGain;
        calculateCoefficients();
    }
    /// Set param values for all params at once
    inline void setAllParams(float cutoff_, float q_, float dbGain, FilterType type)
    {
        cutoff = cutoff_;
        q = q_;
        filterType = type;
        dBGain = dbGain;
        calculateCoefficients();
    }
    /// Method for setting coefficients manually
    inline void setRawCoefficients(float a0_, float a1_, float a2_, float b0_, float b1_, float b2_){
        a0 = a0_;
        a1 = a1_;
        a2 = a2_;
        b0 = b0_;
        b1 = b1_;
        b2 = b2_;
    }

private:
    float sampleRate, cutoff, q, dBGain; 
    float b0, b1, b2, a0, a1, a2;
    float  v1, v2;
    void calculateCoefficients();
    FilterType filterType;
};
}

#endif

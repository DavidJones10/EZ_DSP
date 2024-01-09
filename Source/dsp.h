#pragma once
#ifndef EZ_DSP_UTIL
#define EZ_DSP_UTIL
#include <cassert>
#include <cstdint>
#include <random>
#include <cmath>

#define PI_F 3.1415927410125732421875f
#define TWOPI_F (2.0f * PI_F)
#define HALFPI_F (PI_F * 0.5f)
namespace EZ_DSP 
{
    enum class FilterType
    {
        LOWPASS,
        HIGHPASS,
        BANDPASS,
        NOTCH,
        LOWSHELF,
        HIGHSHELF,
        PEAK
    };

    // fast maximum and minimum for floats, uses assembly if arm architecture
    inline float fmax(float a, float b)
    {
        float max;
        #ifdef __arm__
            asm("vmaxnm.f32 %[d], %[n], %[m]" : [d] "=t"(max) : [n] "t"(a), [m] "t"(b) :);
        #else
            max = (a > b) ? a : b;
        #endif // __arm__
            return max;
    }

    inline float fmin(float a, float b)
    {
        float min;
        #ifdef __arm__
            asm("vminnm.f32 %[d], %[n], %[m]" : [d] "=t"(min) : [n] "t"(a), [m] "t"(b) :);
        #else 
            min = a < b ? a : b;
        #endif //__arm__
            return min;
    }
    // clamp for floats
    inline float fclamp(float in, float min, float max)
    {
        return fmin(fmax(in,min),max);
    }    
    // max, min and clamp for integers
   inline int maxInt(int a, int b)
    {
        int max;
        max = (a > b) ? a : b;
        return max;
    }
    inline int minInt(int a, int b)
    {
        int min;
        min = (a < b) ? a : b;
        return min;
    }
    inline int clampInt(int in, int min, int max)
    {
        return minInt(maxInt(in,min), max);
    }
    // maps a float input to values between float mininmum and maximum
    // assumes 0-1 input
    inline float fmap(float in, float min, float max)
    {
	    return fclamp(min + in * (max - min), min, max);
    }
    // scales an input range to values between outMin and outMax
    inline float fScale(float in, float inMin, float inMax, float outMin, float outMax)
    {
        in = fclamp(in, inMin, inMax);
        float scaledValue = outMin + ((in - inMin) / (inMax-inMin) * (outMax - outMin));
        return scaledValue;
    }


    /*
    One Pole LOWPASS filter
    out is passed by reference
    coeff can be calculated with:
    coeff = 1.0 / (time * sample_rate) ; where time is in seconds
    FOR EXAMPLE:
    float output = 0, input = 1, delay = 40ms;
    fonepole(output, input, 1/(delay *.001 * 44100) )
    cout<<output;     yields "0.000566893"
    */
   inline void fonepole(float &out, float in, float coeff)
   {
        out += coeff * (in - out);
   }

   inline float bpmToMs(float bpm)
   {
        bpm = fclamp(bpm, 30.f, 250.f);
        float milliseconds = 60000.0 / (bpm);
        return milliseconds;
   }

   inline float bpmToHz(float bpm, int numSteps)
   {
        float Hz = bpm / 60.f;
        Hz *= (numSteps/4);
        return Hz;
   }

   inline float dBToLinear(float dB)
   {
        return pow(10.0f, dB / 40.0f);
   }


}
#endif

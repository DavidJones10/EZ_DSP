#include "biquad.h"
using namespace EZ_DSP;
void Biquad::init(float sample_rate)
{
    sampleRate = sample_rate;
    v1 = v2 = 0;
    calculateCoefficients();
}
float Biquad::process(float in_sample) {
    float v0 = in_sample - a1 * v1 - a2 * v2;
    float output = b0 * v0 + b1 * v1 + b2 * v2;
    v2 = v1;
    v1 = v0;
    return output;
}
void Biquad::calculateCoefficients()
{
    float w0 = TWOPI_F * cutoff / sampleRate;
    float alpha = sinf(w0) / (2.f * q);
    float cosW0 = cosf(w0);
    float A = 0.f;
    switch (filterType)
    {
        case FilterType::LOWPASS:
            b0 =  (1.f - cosW0)*.5f;
            b1 =   1.f - cosW0;
            b2 =  (1.f - cosW0)*.5f;
            a0 =   1.f + alpha;
            a1 =  -2.f*cosW0;
            a2 =   1.f - alpha;
            break;
        case FilterType::HIGHPASS:
            b0 =  (1.f + cosW0)*.5f;
            b1 = -(1.f + cosW0);
            b2 =  (1.f + cosW0)*.5f;
            a0 =   1.f + alpha;
            a1 =  -2.f*cosW0;
            a2 =   1.f - alpha;
            break;
        case FilterType::BANDPASS:
            b0 =   q*alpha;
            b1 =   0.f;
            b2 =  -q*alpha;
            a0 =   1.f + alpha;
            a1 =  -2.f*cosW0;
            a2 =   1.f - alpha;
            break;
        case FilterType::NOTCH:
            b0 =   1.f;
            b1 =  -2.f*cosW0;
            b2 =   1.f;
            a0 =   1.f + alpha;
            a1 =  -2.f*cosW0;
            a2 =   1.f - alpha;
            break;
        case FilterType::LOWSHELF:
            A = dBToLinear(dBGain);
            b0 =   A*( (A+1.f) - (A-1.f)*cosW0 + 2.f*sqrt(A)*alpha);
            b1 =   2.f*A*( (A-1.f) - (A+1.f)*cosW0);
            b2 =   A*( (A+1.f) - (A-1.f)*cosW0 - 2.f*sqrt(A)*alpha);
            a0 =  (A+1.f) + (A-1.f)*cosW0 + 2.f*sqrt(A)*alpha;
            a1 =  -2.f*( (A-1.f) + (A+1.f)*cosW0);
            a2 =  (A+1.f) + (A-1.f)*cosW0 - 2.f*sqrt(A)*alpha;
            break;
        case FilterType::HIGHSHELF:
            A = dBToLinear(dBGain);
            b0 =    A*( (A+1.f) + (A-1.f)*cosW0 + 2.f*sqrt(A)*alpha );
            b1 = -2.f*A*( (A-1.f) + (A+1.f)*cosW0                   );
            b2 =    A*( (A+1.f) + (A-1.f)*cosW0 - 2.f*sqrt(A)*alpha );
            a0 =        (A+1.f) - (A-1.f)*cosW0 + 2.f*sqrt(A)*alpha;
            a1 =    2.f*( (A-1.f) - (A+1.f)*cosW0                   );
            a2 =        (A+1.f) - (A-1.f)*cosW0 - 2.f*sqrt(A)*alpha;
            break;
        case FilterType::PEAK:
            A = dBToLinear(dBGain);
            b0 =   1.f + alpha*A;
            b1 =  -2.f*cosW0;
            b2 =   1.f - alpha*A;
            a0 =   1.f + alpha/A;
            a1 =  -2.f*cosW0;
            a2 =   1.f - alpha/A;
            break;
        default:
            b0 = (1.0f - cosW0) *.5f / a0;
            b1 = (1.0f - cosW0) / a0;
            b2 = (1.0f - cosW0) *.5f / a0;
            break;
    }
    float a0_recip = 1.f/a0;
    a1 *= a0_recip;
    a2 *= a0_recip;
    b0 *= a0_recip;
    b1 *= a0_recip;
    b2 *= a0_recip;
    a0 = 1.f;
    
}

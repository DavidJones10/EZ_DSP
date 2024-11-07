#pragma once 
#ifndef EZ_DSP_OVERDRIVE_H
#define EZ_DSP_OVERDRIVE_H
#include "dsp.h"

namespace EZ_DSP
{
class Overdrive
{
public:
    float process(float input);
    void init(float sample_rate);
    void setDrive(float drive_);
    void setPreGain(float gainPre);
    void setPostGain(float gainPost);

private:
    float drive;
    float preGain;
    float postGain;
};
}
#endif

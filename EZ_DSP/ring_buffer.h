/*
  ==============================================================================

    ring_buffer.h
    Created: 30 May 2024 4:17:55pm
    Author:  David Jones

  ==============================================================================
*/

#pragma once
#ifndef RING_BUFFER_H
#define RING_BUFFER_H
#include <math.h>
#include "dsp.h"

namespace EZ_DSP {
template<typename sampleType, size_t bufferSize>
struct RingBuffer{
public:
    /// Fills the buffer with 0s and resets read and write pointers to 0
    void reset(){
        for (int i=0; i < bufferSize; i++){
            buffer[i] = static_cast<sampleType>(0);
        }
        read_ptr = 0;
        write_ptr = 0;
    }
    /// Get sample at specified index
    sampleType get(size_t idx){
        if (idx < 0){
            idx = 0;
        }
        idx = idx % bufferSize;
        return buffer[idx];
    }
    /// Returns sample at read index and advances the read pointer by 1
    sampleType pop(){
        sampleType value = buffer[read_ptr];
        read_ptr = (read_ptr + 1) % bufferSize;
        return value;
    }
    /// Writes the input sample to the buffer at the write pointer and advances the write pointer by 1
    void push(sampleType sample){
        buffer[write_ptr] = sample;
        write_ptr = (write_ptr + 1) % bufferSize;
    }
    /// Writes the input sample at the input index
    void put(sampleType sample, size_t idx){
        if (idx < 0){
            idx = 0;
        }
        idx = idx % bufferSize;
        buffer[idx] = sample;
    }
    /// Get sample at read index without advancing the pointer
    sampleType peek(size_t idx){
        return buffer[idx];
    }
    /// Returns the read index
    size_t get_read_index(){
        return read_ptr;
    }
    /// Returns the write index
    size_t get_write_index(){
        return write_ptr;
    }
    /// Sets the read index
    void set_read_index(size_t idx){
        if (idx < 0){
            idx = 0;
        }
        idx = idx % bufferSize;
        read_ptr = idx;
    }
    /// Sets the write index
    void set_write_index(size_t idx){
        if (idx < 0){
            idx = 0;
        }
        idx = idx % bufferSize;
        write_ptr = idx;
    }
    /// Returns a sample from a fractional index, interpolates between samples if necessary
    float get_frac(float sample_idx){
        float intpart, fractpart;
        size_t truncated = static_cast<size_t>(trunc(sample_idx));
        float sample1 = get(truncated);
        float sample2 = get(truncated+1);
        fractpart = modf(sample_idx, &intpart);
        return lerp(sample1, sample2, fractpart);
    }
    
    size_t len(){
        
    }
    
private:
    size_t read_ptr = 0;
    size_t write_ptr = 0;
    sampleType buffer[bufferSize];
};
};
#endif

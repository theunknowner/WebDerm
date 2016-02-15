/*
 * parallel.h
 *
 *  Created on: Dec 1, 2015
 *      Author: jason
 */

#ifndef PARALLEL_H_
#define PARALLEL_H_

#include "../global.h"

template <class type>

class Parallel_Buffer: public cv::ParallelLoopBody
{
private:
  type *bufferToClip;
  type minValue, maxValue;

public:
  Parallel_Buffer(type* bufferToProcess, const type min, const type max)
    : bufferToClip(bufferToProcess), minValue(min), maxValue(max){}

  virtual void operator()( const cv::Range &r ) const {
    register type *inputOutputBufferPTR=bufferToClip+r.start;
    for (register int jf = r.start; jf != r.end; ++jf, ++inputOutputBufferPTR)
    {
        if (*inputOutputBufferPTR>maxValue)
            *inputOutputBufferPTR=maxValue;
        else if (*inputOutputBufferPTR<minValue)
            *inputOutputBufferPTR=minValue;
    }
  }
};

#endif /* PARALLEL_H_ */

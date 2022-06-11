#pragma once

#include <hybridggpo.h>

//Strategy to predict a random input
class VMRandomInputPredictionStrategy : public IInputPredictionStrategy
{
    virtual void onFrameAdvanced(int frame, char const* buffer, void* values, int size) override {
        frame;
        buffer;
        values;
        size;
    }

    virtual void requestPrediction(int requestedFrame, char const* buffer, void* values, int size) override
    {
        requestedFrame;
        buffer;
        size;

        *(int*)(values) = rand();
    }
};
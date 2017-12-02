#ifndef DISCRETSIGNALSPROVIDERSERVICE_H
#define DISCRETSIGNALSPROVIDERSERVICE_H

#include "struct_signals.h"


const int FIRSTSIGNAL_RATE = 4;
const int SECONDSIGNAL_RATE = 20;
const int THIRDSIGNAL_RATE = 2;

class DiscretSignalsProviderService
{
    DiscretSignalsSet data;
public:

    DiscretSignalsSet getData() {
        static int ticks = 0;
        ticks++;

        if (!(ticks % FIRSTSIGNAL_RATE)) data.values[0] = std::rand() % 2;
        if (!(ticks % SECONDSIGNAL_RATE)) data.values[1] = std::rand() % 2;
        if (!(ticks % THIRDSIGNAL_RATE)) data.values[2] = std::rand() % 2;

        return data;
    }

    void setControlSignal(bool controlSignal) {
        data.values[3] = (int) controlSignal;
    }
};

#endif // DISCRETSIGNALSPROVIDERSERVICE_H

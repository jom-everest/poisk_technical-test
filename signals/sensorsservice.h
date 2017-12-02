#ifndef SENSORSSERVICE_H
#define SENSORSSERVICE_H

#include <cmath>

const int RED_COEFF = 4;

struct SensorsData {
    double first;
    double second;
};

class SensorsService
{
    double am = 1;
    double ph = M_PI / 2;

public:
    SensorsData getData() {
        static int ticks = 0;
        ticks++;

        return {am * sin((double)ticks/RED_COEFF), am * sin((double)ticks/RED_COEFF + ph)};
    }

    void setPh(double ph) {
        this->ph = ph;
    }

    void setAm(double am) {
        this->am = am;
    }

    void setParams(double am, double ph) {
        this->am = am;
        this->ph = ph;
    }
};

#endif // SENSORSSERVICE_H

#ifndef STATEDISCRIT_H
#define STATEDISCRIT_H

const int ANALOGSIGNALSSET_SIZE = 4;
const int DISCRETSIGNALSSET_SIZE = 4;


struct DiscretSignalsSet
{
    int values[DISCRETSIGNALSSET_SIZE] = {0, 0, 0, 0};
	DiscretSignalsSet() {}
};

struct AnalogSignalsSet
{
    double values[ANALOGSIGNALSSET_SIZE] = {0.7, 0, 0, 0};
    AnalogSignalsSet() {}
};

#endif // STATEDISCRIT_H

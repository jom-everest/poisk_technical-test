
#ifndef ANALYSERANALOGSIGNALS_H
#define ANALYSERANALOGSIGNALS_H

#include "struct_signals.h"

#include <set>
#include <utility>

class AnalyserAnalogSignals
{
    std::pair<double, double> restriction[ANALOGSIGNALSSET_SIZE] = {{-2, 2}, {-2, 2}, {-2, 2}, {-2, 2}};
public:
    void setRestriction(int index, std::pair<double, double> r) {
        restriction[index] = r;
    }

    void setRestrictionMin(int index, double v) {
        restriction[index].first = v;
    }

    void setRestrictionMax(int index, double v) {
        restriction[index].second = v;
    }

    bool isOutrange(AnalogSignalsSet &set, std::set<int>& indexesProblemSignals) {
        indexesProblemSignals.clear();
        for (int i = 0; i < ANALOGSIGNALSSET_SIZE; i++) {
            if ((set.values[i] < restriction[i].first) || (set.values[i] > restriction[i].second))
                indexesProblemSignals.insert(i);
        }

        return !indexesProblemSignals.empty();
    }
};

#endif // ANALYSERANALOGSIGNALS_H

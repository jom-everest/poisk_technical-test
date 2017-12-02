#ifndef ANALOGSIGNALSSERVICE_H
#define ANALOGSIGNALSSERVICE_H

#include "sensorsservice.h"
#include "struct_signals.h"


class AnalogSignalsService
{
    DiscretSignalsSet dSet;
public:
    AnalogSignalsSet compute(const SensorsData& sensorsData, DiscretSignalsSet& dSet_new) {
        AnalogSignalsSet aSet;

        //при получении «1» от первого дискретного сигнала в выходной файл в качестве первого параметра
        //записывается текущее мнгновенное значение первой функции;
        aSet.values[0] = (dSet_new.values[0] == 1) ? sensorsData.first : 0;

        //при любом изменении второго дискретного сигнала  в выходной файл в качестве второго параметра
        //записывается текущее мнгновенное значение второй функции;
        aSet.values[1] = (dSet_new.values[1] != dSet.values[1]) ? sensorsData.second : 0;

        //при получении «0» от третьего дискретного сигнала в выходной файл в качестве третьего параметра
        //записывается текущее мнгновенное значение разницы первой и второй функций;
        aSet.values[2] = (dSet_new.values[2] == 0) ? sensorsData.first - sensorsData.second : 0;

        //при получении «1» от четвертого дискретного сигнала в выходной файл в качестве четвертого параметра
        //записывается текущее мнгновенное значение суммы первой и второй функций и все остальные сигналы;
        aSet.values[3] = (dSet_new.values[3] == 1) ? sensorsData.first + sensorsData.second : 0;

        dSet = dSet_new;
        return aSet;
    }

    void setDiscretSignalsSet(DiscretSignalsSet dSet) {
        this->dSet = dSet;
    }
};

#endif // ANALOGSIGNALSSERVICE_H

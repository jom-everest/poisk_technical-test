#ifndef ANALOGSIGNALSREADER_FROMFILE_H
#define ANALOGSIGNALSREADER_FROMFILE_H

#include <string>
#include <fstream>
#include "consts.h"
#include "struct_signals.h"


// передача данных через текстовый файл Аналоговых Сигналов (4 штуки - 4 строки)
class AnalogSignals_FileTransfer
{
    std::string fileName = ANALOG_DATA_TRANSFER_FILENAME;

public:
    // чтение из файла и запись в переданную переменную set,
    // возвращает true в случае удачи, false - если прочесть данные не удалось
    bool readFromFile(AnalogSignalsSet& set) {
        AnalogSignalsSet aSet;
        std::string str;
        std::ifstream ifs(fileName, std::ifstream::in);

        for(int i = 0; i < ANALOGSIGNALSSET_SIZE; i++) {
            std::getline(ifs, str);
            if (!ifs.good()) return false;
            aSet.values[i] = std::stod(str);
        }
        set = aSet;
        return true;
    }

    // запись в файл данных из параметра set,
    // возвращает true в случае удачи, false - если запись данных не удалась
    bool writeToFile(const AnalogSignalsSet& set) {
        std::ofstream ofs (fileName, std::ofstream::trunc);

        for (int i = 0; i < ANALOGSIGNALSSET_SIZE; i++) {
            ofs << std::to_string(set.values[i]) << '\n';
            if (!ofs.good()) return false;
        }
        return true;
    }
};

#endif // ANALOGSIGNALSREADER_FROMFILE_H

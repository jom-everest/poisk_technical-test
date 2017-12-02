#ifndef discreTSIGNALS_FILETRANSFER_H
#define discreTSIGNALS_FILETRANSFER_H


#include <string>
#include "consts.h"
#include "struct_signals.h"

class DiscretSignals_FileTransfer
{
    std::string fileName = DISCRET_DATA_TRANSFER_FILENAME;

public:

    bool readFromFile(DiscretSignalsSet& set) {
        char byte;
        std::ifstream ifs(fileName, std::ifstream::in | std::ifstream::binary);

        ifs.get(byte);
        if (!ifs.good()) return false;
        set.values[0] = byte & 0x01;
        set.values[1] = (byte & 0x02) >> 1;
        set.values[2] = (byte & 0x04) >> 2;
        set.values[3] = (byte & 0x08) >> 3;
        return true;
    }

    bool writeToFile(const DiscretSignalsSet& set) {
        std::ofstream ofs (fileName, std::ofstream::trunc | std::ofstream::binary);

        char byte = char((set.values[3]<<3) + (set.values[2]<<2) + (set.values[1]<<1) + set.values[0]);
        ofs << byte;
        if (!ofs.good()) return false;

        return true;
    }

};

#endif // discreTSIGNALS_FILETRANSFER_H

#ifndef ANALOGSIGNASSERVICE_H
#define ANALOGSIGNASSERVICE_H

#include "analogsignals_filetransfer.h"


//-----------------------
class TransferDataService_AS
{
    AnalogSignals_FileTransfer transfer;

public:
    bool recieveData(AnalogSignalsSet& set) {
        return transfer.readFromFile(set);
    }

    bool sendData(const AnalogSignalsSet& set) {
        return transfer.writeToFile(set);
    }
};


#endif // ANALOGSIGNASSERVICE_H

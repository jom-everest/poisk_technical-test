#ifndef DISCRETSERVICE_H
#define DISCRETSERVICE_H

#include "discritsignals_filetransfer.h"
#include "struct_signals.h"


class TransferDataService_DS
{
    DiscretSignals_FileTransfer transfer;

public:
    bool recieveData(DiscretSignalsSet& set) {
        return transfer.readFromFile(set);
    }

    bool sendData(const DiscretSignalsSet& set) {
        return transfer.writeToFile(set);
    }
};

#endif // DISCRETSERVICE_H

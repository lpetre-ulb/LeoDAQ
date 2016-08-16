#ifndef TDCMODULE_H
#define TDCMODULE_H

#include <QString>
#include <CAENVMElib.h>

class TDCModule
{

private:
    // handle to connection to the VME crate
    int handleChef;
    // base address of the module
    int baseAddress;

public:
    TDCModule(int handleChef_);

    QString setBaseAddress(int baseAddress_);
    QString getFirmwareRevision();


};



#endif // TDCMODULE_H

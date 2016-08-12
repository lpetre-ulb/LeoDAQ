#ifndef HVMODULE_H
#define HVMODULE_H

#include <QString>
#include <CAENVMElib.h>

class HVModule
{
private:
    // handle to connection to the VME crate
    int handleChef;
    // base address of the module
    int baseAddress;

public:
    HVModule(int handleChef_);
    QString setBaseAddress(int baseAddress_);
    QString getBoardModel();
    CVErrorCodes getVMAX(u_int16_t* value16);
    CVErrorCodes getIMAX(u_int16_t* value16);


};

#endif // HVMODULE_H

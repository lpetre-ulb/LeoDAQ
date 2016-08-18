#ifndef SCALERMODULE_H
#define SCALERMODULE_H

#include <QString>
#include <CAENVMElib.h>

class ScalerModule
{
private:
    // handle to connection to the VME crate
    int handleChef;
    // base address of the module
    int baseAddress;

public:
    ScalerModule(int handleChef_);
    QString setBaseAddress(int baseAddress_);
    void setPulserMode(unsigned char width, CVTimeUnits unit);
    void startPulser();
    double getTruePulsePeriod();
    void readChannels(std::vector<int> *values);
    void resetChannels();
    u_int32_t readChannel(int ch);




private:
    QString getModuleType();

};

#endif // SCALERMODULE_H

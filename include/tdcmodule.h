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

    int startChannel;
    int stopChannel;

    // taking data
    u_int16_t tdcWindowWidth;
    int16_t tdcWindowOffset;

    void waitForReadOK();
    void waitForWriteOK();


public:
    TDCModule(int handleChef_);

    QString setBaseAddress(int baseAddress_);
    QString getFirmwareRevision();

    void clear();
    bool setTriggerMode(u_int16_t windowWidth, int16_t windowOffset);
    void setStartStopChannels(int start, int stop);

    void readEvents(std::vector<int>* values);


};



#endif // TDCMODULE_H

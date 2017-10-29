#ifndef TDCMODULE_H
#define TDCMODULE_H

#include <vector>
#include <QString>

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
    uint16_t tdcWindowWidth;
    int16_t tdcWindowOffset;

    void waitForReadOK();
    void waitForWriteOK();


public:
    TDCModule(int handleChef_);

    QString setBaseAddress(int baseAddress_);
    QString getFirmwareRevision();

    void clear();
    bool setTriggerMode(uint16_t windowWidth, int16_t windowOffset);
    void setStartStopChannels(int start, int stop);

    void readEvents(std::vector<long> *values);
    void setTriggerAcquisitionMode();
    void setContinuousAcquisitionMode();
    void setWindowWidth(uint16_t);
    void setWindowOffset(int16_t);
    void setExtraSearchWindow();
    void setRejectMargin();
    void readAcqMode();
    void readConfig();


};



#endif // TDCMODULE_H

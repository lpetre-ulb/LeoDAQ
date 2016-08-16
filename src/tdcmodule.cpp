#include "tdcmodule.h"
#include <unistd.h>
#include <qdebug.h>

TDCModule::TDCModule(int handleChef_):
    handleChef(handleChef_)
{
     baseAddress = -1;
}

QString TDCModule::setBaseAddress(int baseAddress_)
{
    baseAddress = baseAddress_ << 16;
    return getFirmwareRevision();

}

QString TDCModule::getFirmwareRevision()
{
    u_int16_t value16;
    // read board parameters
    QString fwRev;
    CVErrorCodes res = CAENVME_ReadCycle(handleChef, baseAddress + 0x1026, &value16, cvA32_U_DATA, cvD16);
    qDebug() << ((value16) & 0xff);
    fwRev += QString::number((value16 >> 4) & 0xf);
    fwRev += QString(".");
    fwRev += QString::number(value16 & 0xf);

    if (res != cvSuccess || fwRev != "0.5"){
        qDebug() << "Wrong board or wrong firmware revision: " << fwRev;
        return "UNKNOWN";
    }
    else {
        return fwRev;
    }
}


void TDCModule::clear()
{
    u_int16_t value16 = 0x0000;
    CAENVME_WriteCycle(handleChef, baseAddress + 0x1016, &value16, cvA32_U_DATA, cvD16);
}

bool TDCModule::setTriggerMode(u_int16_t windowWidth, int16_t windowOffset)
{
    tdcWindowWidth = windowWidth;
    tdcWindowOffset = windowOffset;


    if (tdcWindowOffset + tdcWindowWidth < 0) { //1st case: the trigger matching window precedes the trigger arrival
        if (-tdcWindowOffset > 102375 || tdcWindowOffset == 0) return false;
    }
    else if (tdcWindowOffset + tdcWindowWidth > 1000) { // 2nd case: the trigger matching is straddling the trigger or delayed with respect to the trigger
        return false;
    }

    uint16_t value16;

    // set window width
    waitForWriteOK();
    value16 = 0x1000; // OPCODE 10XX set window width
    CAENVME_WriteCycle(handleChef, baseAddress + 0x102E, &value16, cvA32_U_DATA, cvD16);

    waitForWriteOK();
    value16 = tdcWindowWidth / 25;
    CAENVME_WriteCycle(handleChef, baseAddress + 0x102E, &value16, cvA32_U_DATA, cvD16);

    waitForWriteOK();
    value16 = 0x1100; // OPCODE 11XX set window offset
    CAENVME_WriteCycle(handleChef, baseAddress + 0x102E, &value16, cvA32_U_DATA, cvD16);

    waitForWriteOK();
    value16 = tdcWindowOffset / 25; // cast to unsigned
    CAENVME_WriteCycle(handleChef, baseAddress + 0x102E, &value16, cvA32_U_DATA, cvD16);

    waitForWriteOK();
    value16 = 0x1200; // OPCODE 12XX set extra search margin
    CAENVME_WriteCycle(handleChef, baseAddress + 0x102E, &value16, cvA32_U_DATA, cvD16);

    waitForWriteOK();
    value16 = 0x0004; // 0.1 us extra search margin value
    CAENVME_WriteCycle(handleChef, baseAddress + 0x102E, &value16, cvA32_U_DATA, cvD16);

    waitForWriteOK();
    value16 = 0x1300; // OPCODE 13XX set reject margin
    CAENVME_WriteCycle(handleChef, baseAddress + 0x102E, &value16, cvA32_U_DATA, cvD16);

    waitForWriteOK();
    value16 = 0x0001; // 0.025 us reject margin value
    CAENVME_WriteCycle(handleChef, baseAddress + 0x102E, &value16, cvA32_U_DATA, cvD16);

    return true;

}

void TDCModule::readEvents(std::vector<int>* values)
{
    uint16_t value16;
    uint32_t value32;

    // read Status register
    CAENVME_ReadCycle(handleChef, baseAddress + 0x1002, &value16, cvA32_U_DATA, cvD16);

    if (value16 & 0x1) {
        CAENVME_ReadCycle(handleChef, baseAddress + 0x0000, &value32, cvA32_U_DATA, cvD32);
        int header = ((value32 >> 27) & 0x1F);
        if (header == 0x8) { // Global header
            CAENVME_ReadCycle(handleChef, baseAddress + 0x0000, &value32, cvA32_U_DATA, cvD32);
            header = ((value32 >> 27) & 0x1F);
            if (header == 0x1) { // TDC header
                int nChannel = 0;
                int tdcTimes[16];
                QString output = "";
                while (true) {
                    CAENVME_ReadCycle(handleChef, baseAddress + 0x0000, &value32, cvA32_U_DATA, cvD32);
                    header = ((value32 >> 27) & 0x1F);
                    if (header != 0x0) {
                        break; // if not TDC measurement
                    }
                    int channel = ((value32 >> 21) & 0x1F);
                    int tdcTime = (value32 & 0x1FFFFF) * 25;
                    output += QString::number(channel) + " " + QString::number(tdcTime) + "\n";
                    if (channel == startChannel || channel == stopChannel) {
                        tdcTimes[channel] = tdcTime;
                        nChannel++;
                    }
                }
                if (nChannel == 2) {
                    qDebug() << output;
                    output += "\n";
                    double difference = (tdcTimes[stopChannel] - tdcTimes[startChannel])/1000.0;
                    if (difference < 0) {
                        difference += 52429.500;
                    }
                    //output = QString::number(difference) + "\n";
                    //stream << output;
                    //qDebug() << "difference: " << QString::number(difference);
                    values->push_back(difference);
                }
            }
        }
    }
}

void TDCModule::setStartStopChannels(int start, int stop)
{
    startChannel = start;
    stopChannel = stop;
}

void TDCModule::waitForWriteOK()
{
    uint16_t value16;
    bool WRITE_OK = false;

    do {
        CAENVME_ReadCycle(handleChef, baseAddress + 0x1030, &value16, cvA32_U_DATA, cvD16);
        WRITE_OK = ((value16 >> 0) & 0x1);
        usleep(10);
    } while (!WRITE_OK);
    return;

}

void TDCModule::waitForReadOK()
{
    uint16_t value16;
    bool READ_OK = false;

    do {
        CAENVME_ReadCycle(handleChef, baseAddress + 0x1030, &value16, cvA32_U_DATA, cvD16);
        READ_OK = ((value16 >> 1) & 0x1);
        usleep(10);
    } while (!READ_OK);
    return;

}

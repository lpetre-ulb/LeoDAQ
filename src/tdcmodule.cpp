#include "tdcmodule.h"

#include <vector>
#include <unistd.h>
#include <QDebug>
#include <QThread>
#include <CAENVMElib.h>

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
    // Read board parameters
    u_int16_t value16;
    CVErrorCodes ret = CAENVME_ReadCycle(handleChef, baseAddress + 0x1026, &value16, cvA32_U_DATA, cvD16);

    qDebug() << "Raw firmware version reading:" << ((value16) & 0xff);

    QString fwRev;
    fwRev += QString::number((value16 >> 4) & 0xf);
    fwRev += QString(".");
    fwRev += QString::number(value16 & 0xf);

    if (ret != cvSuccess || fwRev != "0.5"){
        qWarning() << "Wrong board or wrong firmware revision:" << fwRev;
        return "UNKNOWN";
    }
    else {
        return fwRev;
    }
}


void TDCModule::clear()
{
    u_int16_t value16 = 0x0000;
    CVErrorCodes ret = CAENVME_WriteCycle(handleChef, baseAddress + 0x1016, &value16, cvA32_U_DATA, cvD16);

    if (ret != cvSuccess) {
        qWarning() << "Error while trying to clear:";
        qWarning() << "Error code is: " << ret;
        qWarning() << "Meaning: " << CAENVME_DecodeError(ret);
    }
}

void TDCModule::readConfig()
{
        uint16_t value16;
        waitForWriteOK();
        value16 = 0x1600; // OPCODE 16XX read tdc config
        CAENVME_WriteCycle(handleChef, baseAddress + 0x102E, &value16, cvA32_U_DATA, cvD16);

        waitForReadOK();
        CAENVME_ReadCycle(handleChef, baseAddress + 0x102E, &value16, cvA32_U_DATA, cvD16);
        qInfo() << "Match window width: " << (value16);

        waitForReadOK();
        CAENVME_ReadCycle(handleChef, baseAddress + 0x102E, &value16, cvA32_U_DATA, cvD16);
        qInfo() << "window offset: " << (value16);
        waitForReadOK();
        CAENVME_ReadCycle(handleChef, baseAddress + 0x102E, &value16, cvA32_U_DATA, cvD16);
        qInfo() << "Extra search window " << (value16);
        waitForReadOK();
        CAENVME_ReadCycle(handleChef, baseAddress + 0x102E, &value16, cvA32_U_DATA, cvD16);
        qInfo() << "reject margin " << (value16);
        waitForReadOK();
        CAENVME_ReadCycle(handleChef, baseAddress + 0x102E, &value16, cvA32_U_DATA, cvD16);
        qInfo() << "trigger time substraction: " << (value16 & 0x1);
}



void TDCModule::setTriggerAcquisitionMode()
{
	// Set trigger acquisition mode itself
        uint16_t value16 = 0x0000;
        waitForWriteOK();
        CVErrorCodes ret = CAENVME_WriteCycle(handleChef, baseAddress + 0x102E, &value16, cvA32_U_DATA, cvD16);

        if (ret != cvSuccess) {
            qWarning() << "Error while trying to write set trigger acquisition mode opcode:";
            qWarning() << "Error code is: " << ret;
            qWarning() << "Meaning: " << CAENVME_DecodeError(ret);
        }

	// Enable substraction of trigger time
        value16 = 0x1400;
        waitForWriteOK();
        ret = CAENVME_WriteCycle(handleChef, baseAddress + 0x102E, &value16, cvA32_U_DATA, cvD16);

        if (ret != cvSuccess) {
            qWarning() << "Error while trying to enable substraction of trigger time:";
            qWarning() << "Error code is: " << ret;
            qWarning() << "Meaning: " << CAENVME_DecodeError(ret);
        }
}

void TDCModule::setContinuousAcquisitionMode()
{
        waitForWriteOK();

        uint16_t value16 = 0x0100;
	CVErrorCodes ret = CAENVME_WriteCycle(handleChef, baseAddress + 0x102E, &value16, cvA32_U_DATA, cvD16);

        if (ret != cvSuccess) {
            qWarning() << "Error while trying to write set trigger acquisition mode opcode:";
            qWarning() << "Error code is: " << ret;
            qWarning() << "Meaning: " << CAENVME_DecodeError(ret);
        }
}

void TDCModule::setWindowWidth(uint16_t width)
{
        uint16_t value16b;
        // set window width
        waitForWriteOK();
        value16b = 0x1000;
        CAENVME_WriteCycle(handleChef, baseAddress + 0x102E, &value16b, cvA32_U_DATA, cvD16);
        waitForWriteOK();
        value16b = width / 25;
        CAENVME_WriteCycle(handleChef, baseAddress + 0x102E, &value16b, cvA32_U_DATA, cvD16);
}

void TDCModule::setWindowOffset(int32_t offset)
{
	// OPCODE 11XX set window offset
        uint16_t value16 = 0x1100;
        waitForWriteOK();
        CAENVME_WriteCycle(handleChef, baseAddress + 0x102E, &value16, cvA32_U_DATA, cvD16);

	int16_t value16b = static_cast<int16_t>(offset/25);
        waitForWriteOK();
        CAENVME_WriteCycle(handleChef, baseAddress + 0x102E, &value16b, cvA32_U_DATA, cvD16);

}

void TDCModule::setExtraSearchWindow()
{
        uint16_t value16;
        waitForWriteOK();
        value16 = 0x1200; // OPCODE 12XX set extra search margin
        CAENVME_WriteCycle(handleChef, baseAddress + 0x102E, &value16, cvA32_U_DATA, cvD16);
        waitForWriteOK();
        value16 = 0x0004; // 0.1 us extra search margin value
        CAENVME_WriteCycle(handleChef, baseAddress + 0x102E, &value16, cvA32_U_DATA, cvD16);

}

void TDCModule::setRejectMargin()
{

    uint16_t value16 = 0x1300; // OPCODE 13XX set reject margin
    waitForWriteOK();
    CAENVME_WriteCycle(handleChef, baseAddress + 0x102E, &value16, cvA32_U_DATA, cvD16);
    waitForWriteOK();
    value16 = 0x0001; // 0.025 us reject margin value
    CAENVME_WriteCycle(handleChef, baseAddress + 0x102E, &value16, cvA32_U_DATA, cvD16);

}

void TDCModule::readAcqMode()
{
        uint16_t value16;

        waitForWriteOK();
        value16 = 0x0200;
        CAENVME_WriteCycle(handleChef, baseAddress + 0x102E, &value16, cvA32_U_DATA, cvD16);

        waitForReadOK();
        CAENVME_ReadCycle(handleChef, baseAddress + 0x102E, &value16, cvA32_U_DATA, cvD16);
        qInfo() << "Acquisition mode: " << (value16 & 0x1);
}


TDCModule::Errors TDCModule::setTriggerMode(uint16_t windowWidth, int32_t windowOffset)
{
    if (windowOffset < -51200 || windowOffset > 1000)
        return WRONG_WINDOW_OFFSET;
    if (windowWidth < 25 || windowWidth > 51200)
        return WRONG_WINDOW_WIDTH;

    // The trigger matching is straddling the trigger or delayed with respect to the trigger
    if (windowOffset + static_cast<int32_t>(windowWidth) > 1000) {
        return WRONG_CONDITION_ON_DELAYED_TRIGGER; }

    tdcWindowWidth = windowWidth;
    tdcWindowOffset = windowOffset;

    setTriggerAcquisitionMode();
    setWindowWidth(tdcWindowWidth);
    setWindowOffset(tdcWindowOffset);

    qInfo() << "Checking trigger window parameters...";
    readAcqMode();
    readConfig();
    qInfo() << "Good to go !";

    return NO_ERROR;
}

void TDCModule::readEvents(std::vector<long>* values)
{
    uint16_t value16;
    uint32_t value32;

    // read Status register
    //qWarning() << "Read status register";
    CVErrorCodes ret = CAENVME_ReadCycle(handleChef, baseAddress + 0x1002, &value16, cvA32_U_DATA, cvD16);
    if (ret != cvSuccess) {
        qWarning() << "Error while trying to read status register:";
        qWarning() << "Error code is: " << ret;
        qWarning() << "Meaning: " << CAENVME_DecodeError(ret);
    }

    if (value16 & 0x1) {
        //qWarning() << "Data ready";
        ret = CAENVME_ReadCycle(handleChef, baseAddress + 0x0000, &value32, cvA32_U_DATA, cvD32);
        if (ret != cvSuccess) {
            qWarning() << "Error while trying to read event buffer:";
            qWarning() << "Error code is: " << ret;
            qWarning() << "Meaning: " << CAENVME_DecodeError(ret);
        }
        int header = ((value32 >> 27) & 0x1F);
        if (header == 0x8) { // Global header
            ret = CAENVME_ReadCycle(handleChef, baseAddress + 0x0000, &value32, cvA32_U_DATA, cvD32);
            if (ret != cvSuccess) {
                qWarning() << "Error while trying to read event buffer:";
                qWarning() << "Error code is: " << ret;
                qWarning() << "Meaning: " << CAENVME_DecodeError(ret);
            }
            header = ((value32 >> 27) & 0x1F);
            if (header == 0x1) { // TDC header
                int nChannel = 0;
                long tdcTimes[16];
                QString output = "";
                while (true) {
                    ret = CAENVME_ReadCycle(handleChef, baseAddress + 0x0000, &value32, cvA32_U_DATA, cvD32);
                    if (ret != cvSuccess) {
                        qWarning() << "Error while trying to read event buffer:";
                        qWarning() << "Error code is: " << ret;
                        qWarning() << "Meaning: " << CAENVME_DecodeError(ret);
                    }
                    header = ((value32 >> 27) & 0x1F);
                    if (header != 0x0) {
                        break; // if not TDC measurement
                    }
                    int channel = ((value32 >> 21) & 0x1F);
                    qWarning() << "tdc time: " << (value32 & 0x1FFFFF);
                    int tdcTime = (value32 & 0x1FFFFF) * 25;
                    output += QString::number(channel) + " " + QString::number(tdcTime) + "\n";
                    if (channel == startChannel || channel == stopChannel) {
                        tdcTimes[channel] = tdcTime;
                        nChannel++;
                    }
                }
                if (nChannel == 2) {
                    //qWarning() << output;
                    output += "\n";
                    long difference = (tdcTimes[stopChannel] - tdcTimes[startChannel]);
                    //if (difference < 0) {
                    //    difference += 52429.500;
                    //}
                    //output = QString::number(difference) + "\n";
                    //stream << output;
                    //qWarning() << "difference: " << QString::number(difference);
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
    CVErrorCodes ret;
    do {
        ret = CAENVME_ReadCycle(handleChef, baseAddress + 0x1030, &value16, cvA32_U_DATA, cvD16);
        WRITE_OK = ((value16 >> 0) & 0x1);
        QThread::msleep(100);
        if (ret != cvSuccess) {
            qWarning() << "Error while trying to read status:";
            qWarning() << "Error code is: " << ret;
            qWarning() << "Meaning: " << CAENVME_DecodeError(ret);
        }
    } while (!WRITE_OK || ret != cvSuccess);
    return;

}

void TDCModule::waitForReadOK()
{
    uint16_t value16;
    bool READ_OK = false;
    CVErrorCodes ret;

    do {
        ret = CAENVME_ReadCycle(handleChef, baseAddress + 0x1030, &value16, cvA32_U_DATA, cvD16);
        READ_OK = ((value16 >> 1) & 0x1);
        QThread::msleep(100);
        if (ret != cvSuccess) {
            qWarning() << "Error while trying to read status:";
            qWarning() << "Error code is: " << ret;
            qWarning() << "Meaning: " << CAENVME_DecodeError(ret);
        }
    } while (!READ_OK || ret != cvSuccess);
    return;

}

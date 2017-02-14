#include "tdcmodule.h"
#include <unistd.h>
#include <qdebug.h>
#include <iostream>
#include <QThread>

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
    CVErrorCodes ret = CAENVME_ReadCycle(handleChef, baseAddress + 0x1026, &value16, cvA32_U_DATA, cvD16);
    qDebug() << ((value16) & 0xff);
    fwRev += QString::number((value16 >> 4) & 0xf);
    fwRev += QString(".");
    fwRev += QString::number(value16 & 0xf);

    if (ret != cvSuccess || fwRev != "0.5"){
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
    CVErrorCodes ret = CAENVME_WriteCycle(handleChef, baseAddress + 0x1016, &value16, cvA32_U_DATA, cvD16);
    if (ret != cvSuccess) {
        qDebug() << "Error while trying to clear:";
        qDebug() << "Error code is: " << ret;
        qDebug() << "Meaning: " << CAENVME_DecodeError(ret);
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
        std::cout << "Match window width: " << (value16) << std::endl;

        waitForReadOK();
        CAENVME_ReadCycle(handleChef, baseAddress + 0x102E, &value16, cvA32_U_DATA, cvD16);
        std::cout << "window offset: " << (value16) << std::endl;
        waitForReadOK();
        CAENVME_ReadCycle(handleChef, baseAddress + 0x102E, &value16, cvA32_U_DATA, cvD16);
        std::cout << "Extra search window " << (value16) << std::endl;
        waitForReadOK();
        CAENVME_ReadCycle(handleChef, baseAddress + 0x102E, &value16, cvA32_U_DATA, cvD16);
        std::cout << "reject margin " << (value16) << std::endl;
        waitForReadOK();
        CAENVME_ReadCycle(handleChef, baseAddress + 0x102E, &value16, cvA32_U_DATA, cvD16);
        std::cout << "trigger time substraction: " << (value16 & 0x1) << std::endl;
}



void TDCModule::setTriggerAcquisitionMode()
{
        //char a = 'a';
        //std::cout << __LINE__ << " Enter a char: ";
        //std::cin >> a;
        //waitForWriteOK();
        //std::cout << __LINE__ << " Enter a char: ";
        //std::cin >> a;
        uint16_t value16;
        value16 = 0x0000;
        //std::cout << __LINE__ << " Enter a char: ";
        //std::cin >> a;
        CAENVME_WriteCycle(handleChef, baseAddress + 0x102E, &value16, cvA32_U_DATA, cvD16);
        /*if (ret != cvSuccess) {
            qDebug() << "Error while trying to write set trigger acquisition mode opcode:";
            qDebug() << "Error code is: " << ret;
            qDebug() << "Meaning: " << CAENVME_DecodeError(ret);
        }*/
        //std::cout << __LINE__ << " Enter a char: ";
        //std::cin >> a;
}

void TDCModule::setContinuousAcquisitionMode()
{
        //char a = 'a';
        //std::cout << __LINE__ << " Enter a char: ";
        //std::cin >> a;
        //waitForWriteOK();
        //std::cout << __LINE__ << " Enter a char: ";
        //std::cin >> a;
        uint16_t value16;
        value16 = 0x0100;
        //std::cout << __LINE__ << " Enter a char: ";
        //std::cin >> a;
        CAENVME_WriteCycle(handleChef, baseAddress + 0x102E, &value16, cvA32_U_DATA, cvD16);
        /*if (ret != cvSuccess) {
            qDebug() << "Error while trying to write set trigger acquisition mode opcode:";
            qDebug() << "Error code is: " << ret;
            qDebug() << "Meaning: " << CAENVME_DecodeError(ret);
        }*/
        //std::cout << __LINE__ << " Enter a char: ";
        //std::cin >> a;
}

void TDCModule::setWindowWidth(uint16_t width)
{
        uint16_t value16b;
        //char a = 'a';
        //std::cout << __LINE__ << " Enter a char: ";
        //std::cin >> a;
        // set window width
        waitForWriteOK();
        value16b = 0x1000;
        CAENVME_WriteCycle(handleChef, baseAddress + 0x102E, &value16b, cvA32_U_DATA, cvD16);
        //std::cout << __LINE__ << " Enter a char: ";
        //std::cin >> a;
        waitForWriteOK();
        //std::cout << __LINE__ << " Enter a char: ";
        //std::cin >> a;
        value16b = width / 25;
        CAENVME_WriteCycle(handleChef, baseAddress + 0x102E, &value16b, cvA32_U_DATA, cvD16);
        //std::cout << __LINE__ << " Enter a char: ";
        //std::cin >> a;
}

void TDCModule::setWindowOffset(int16_t offset)
{
        uint16_t value16;
        //char a = 'a';
        //std::cout << __LINE__ << " Enter a char: ";
        //std::cin >> a;
        waitForWriteOK();
        value16 = 0x1100; // OPCODE 11XX set window offset
        CAENVME_WriteCycle(handleChef, baseAddress + 0x102E, &value16, cvA32_U_DATA, cvD16);
        //std::cout << __LINE__ << " Enter a char: ";
        //std::cin >> a;
        waitForWriteOK();
        value16 = offset / 25; // cast to unsigned
        CAENVME_WriteCycle(handleChef, baseAddress + 0x102E, &value16, cvA32_U_DATA, cvD16);

}

void TDCModule::setExtraSearchWindow()
{
        uint16_t value16;
        //char a = 'a';
        //std::cout << __LINE__ << " Enter a char: ";
        //std::cin >> a;
        waitForWriteOK();
        value16 = 0x1200; // OPCODE 12XX set extra search margin
        CAENVME_WriteCycle(handleChef, baseAddress + 0x102E, &value16, cvA32_U_DATA, cvD16);
        //std::cout << __LINE__ << " Enter a char: ";
        //std::cin >> a;
        waitForWriteOK();
        value16 = 0x0004; // 0.1 us extra search margin value
        CAENVME_WriteCycle(handleChef, baseAddress + 0x102E, &value16, cvA32_U_DATA, cvD16);

}

void TDCModule::setRejectMargin()
{

    uint16_t value16 = 0x1300; // OPCODE 13XX set reject margin
    //char a = 'a';
    //std::cout << __LINE__ << " Enter a char: ";
    //std::cin >> a;
    waitForWriteOK();
    CAENVME_WriteCycle(handleChef, baseAddress + 0x102E, &value16, cvA32_U_DATA, cvD16);
    //std::cout << __LINE__ << " Enter a char: ";
    //std::cin >> a;
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
        std::cout << "Acquisition mode: " << (value16 & 0x1) << std::endl;
}


bool TDCModule::setTriggerMode(uint16_t windowWidth, int16_t windowOffset)
{

    readConfig();
    //setTriggerAcquisitionMode();
    readAcqMode();

    tdcWindowWidth = windowWidth;
    tdcWindowOffset = windowOffset;


    if (tdcWindowOffset + tdcWindowWidth < 0) { //1st case: the trigger matching window precedes the trigger arrival
        if (-tdcWindowOffset > 102375 || tdcWindowOffset == 0) return false;
    }
    else if (tdcWindowOffset + tdcWindowWidth > 1000) { // 2nd case: the trigger matching is straddling the trigger or delayed with respect to the trigger
        return false;
    }

    setTriggerAcquisitionMode();
    setWindowWidth(windowWidth);
    setWindowOffset(windowOffset);

    readConfig();
    readAcqMode();

    //uint16_t value16;

    //setWindowWidth(tdcWindowWidth / 25);
    /*
    // set window width
    waitForWriteOK();
    value16 = 0x1000; // OPCODE 10XX set window width

    CVErrorCodes ret = CAENVME_WriteCycle(handleChef, baseAddress + 0x102E, &value16, cvA32_U_DATA, cvD16);
    if (ret != cvSuccess) {
        qDebug() << "Error while trying to write set window width opcode:";
        qDebug() << "Error code is: " << ret;
        qDebug() << "Meaning: " << CAENVME_DecodeError(ret);
	return false;
    }

    waitForWriteOK();
    std::cout << tdcWindowWidth << std::endl;
    value16 = tdcWindowWidth / 25;
    std::cout << value16 << std::endl;
    std::cout << __LINE__ << " Enter a char: ";
    std::cin >> a;
    ret = CAENVME_WriteCycle(handleChef, baseAddress + 0x102E, &value16, cvA32_U_DATA, cvD16);
    if (ret != cvSuccess) {
        qDebug() << "Error while trying to write window width:";
        qDebug() << "Error code is: " << ret;
        qDebug() << "Meaning: " << CAENVME_DecodeError(ret);
	return false;
    }
    */

    //setWindowOffset((uint16_t)(tdcWindowOffset / 25));
    /*
    waitForWriteOK();
    value16 = 0x1100; // OPCODE 11XX set window offset
    std::cout << __LINE__ << " Enter a char: ";
    std::cin >> a;
    ret = CAENVME_WriteCycle(handleChef, baseAddress + 0x102E, &value16, cvA32_U_DATA, cvD16);
    if (ret != cvSuccess) {
        qDebug() << "Error while trying to write set window offset opcode:";
        qDebug() << "Error code is: " << ret;
        qDebug() << "Meaning: " << CAENVME_DecodeError(ret);
	return false;
    }

    waitForWriteOK();
    std::cout << tdcWindowOffset << std::endl;
    value16 = tdcWindowOffset / 25; // cast to unsigned
    std::cout << value16 << std::endl;
    std::cout << __LINE__ << " Enter a char: ";
    std::cin >> a;
    ret = CAENVME_WriteCycle(handleChef, baseAddress + 0x102E, &value16, cvA32_U_DATA, cvD16);
    if (ret != cvSuccess) {
        qDebug() << "Error while trying to write window offset:";
        qDebug() << "Error code is: " << ret;
        qDebug() << "Meaning: " << CAENVME_DecodeError(ret);
	return false;
    }
    */

    //setExtraSearchWindow();
    /*
    waitForWriteOK();
    value16 = 0x1200; // OPCODE 12XX set extra search margin
    ret = CAENVME_WriteCycle(handleChef, baseAddress + 0x102E, &value16, cvA32_U_DATA, cvD16);
    if (ret != cvSuccess) {
        qDebug() << "Error while trying to write extra search margin opcode:";
        qDebug() << "Error code is: " << ret;
        qDebug() << "Meaning: " << CAENVME_DecodeError(ret);
	return false;
    }

    waitForWriteOK();
    value16 = 0x0004; // 0.1 us extra search margin value
    ret = CAENVME_WriteCycle(handleChef, baseAddress + 0x102E, &value16, cvA32_U_DATA, cvD16);
    if (ret != cvSuccess) {
        qDebug() << "Error while trying to write extra search margin:";
        qDebug() << "Error code is: " << ret;
        qDebug() << "Meaning: " << CAENVME_DecodeError(ret);
	return false;
    }
    */

    //setRejectMargin();
    /*
    waitForWriteOK();
    value16 = 0x1300; // OPCODE 13XX set reject margin
    ret = CAENVME_WriteCycle(handleChef, baseAddress + 0x102E, &value16, cvA32_U_DATA, cvD16);
    if (ret != cvSuccess) {
        qDebug() << "Error while trying to write set reject margin opcode:";
        qDebug() << "Error code is: " << ret;
        qDebug() << "Meaning: " << CAENVME_DecodeError(ret);
	return false;
    }

    waitForWriteOK();
    value16 = 0x0001; // 0.025 us reject margin value
    ret = CAENVME_WriteCycle(handleChef, baseAddress + 0x102E, &value16, cvA32_U_DATA, cvD16);
    if (ret != cvSuccess) {
        qDebug() << "Error while trying to write reject margin:";
        qDebug() << "Error code is: " << ret;
        qDebug() << "Meaning: " << CAENVME_DecodeError(ret);
	return false;
    }
    */
    qDebug() << "Good to go: true";
    return true;

}

void TDCModule::readEvents(std::vector<long>* values)
{
    uint16_t value16;
    uint32_t value32;

    // read Status register
    //qDebug() << "Read status register";
    CVErrorCodes ret = CAENVME_ReadCycle(handleChef, baseAddress + 0x1002, &value16, cvA32_U_DATA, cvD16);
    if (ret != cvSuccess) {
        qDebug() << "Error while trying to read status register:";
        qDebug() << "Error code is: " << ret;
        qDebug() << "Meaning: " << CAENVME_DecodeError(ret);
    }

    if (value16 & 0x1) {
        //qDebug() << "Data ready";
        ret = CAENVME_ReadCycle(handleChef, baseAddress + 0x0000, &value32, cvA32_U_DATA, cvD32);
        if (ret != cvSuccess) {
            qDebug() << "Error while trying to read event buffer:";
            qDebug() << "Error code is: " << ret;
            qDebug() << "Meaning: " << CAENVME_DecodeError(ret);
        }
        int header = ((value32 >> 27) & 0x1F);
        if (header == 0x8) { // Global header
            ret = CAENVME_ReadCycle(handleChef, baseAddress + 0x0000, &value32, cvA32_U_DATA, cvD32);
            if (ret != cvSuccess) {
                qDebug() << "Error while trying to read event buffer:";
                qDebug() << "Error code is: " << ret;
                qDebug() << "Meaning: " << CAENVME_DecodeError(ret);
            }
            header = ((value32 >> 27) & 0x1F);
            if (header == 0x1) { // TDC header
                int nChannel = 0;
                long tdcTimes[16];
                QString output = "";
                while (true) {
                    ret = CAENVME_ReadCycle(handleChef, baseAddress + 0x0000, &value32, cvA32_U_DATA, cvD32);
                    if (ret != cvSuccess) {
                        qDebug() << "Error while trying to read event buffer:";
                        qDebug() << "Error code is: " << ret;
                        qDebug() << "Meaning: " << CAENVME_DecodeError(ret);
                    }
                    header = ((value32 >> 27) & 0x1F);
                    if (header != 0x0) {
                        break; // if not TDC measurement
                    }
                    int channel = ((value32 >> 21) & 0x1F);
                    qDebug() << "tdc time: " << (value32 & 0x1FFFFF);
                    int tdcTime = (value32 & 0x1FFFFF) * 25;
                    output += QString::number(channel) + " " + QString::number(tdcTime) + "\n";
                    if (channel == startChannel || channel == stopChannel) {
                        tdcTimes[channel] = tdcTime;
                        nChannel++;
                    }
                }
                if (nChannel == 2) {
                    //qDebug() << output;
                    output += "\n";
                    long difference = (tdcTimes[stopChannel] - tdcTimes[startChannel]);
                    //if (difference < 0) {
                    //    difference += 52429.500;
                    //}
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
    CVErrorCodes ret;
    do {
        ret = CAENVME_ReadCycle(handleChef, baseAddress + 0x1030, &value16, cvA32_U_DATA, cvD16);
        WRITE_OK = ((value16 >> 0) & 0x1);
        QThread::msleep(100);
        if (ret != cvSuccess) {
            qDebug() << "Error while trying to write reject margin:";
            qDebug() << "Error code is: " << ret;
            qDebug() << "Meaning: " << CAENVME_DecodeError(ret);
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
            qDebug() << "Error while trying to read status:";
            qDebug() << "Error code is: " << ret;
            qDebug() << "Meaning: " << CAENVME_DecodeError(ret);
        }
    } while (!READ_OK || ret != cvSuccess);
    return;

}

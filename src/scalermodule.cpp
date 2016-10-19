#include "scalermodule.h"
#include <CAENVMElib.h>
#include <qdebug.h>

ScalerModule::ScalerModule(int handleChef_):
    handleChef(handleChef_)
{
    baseAddress = -1;
}

QString ScalerModule::setBaseAddress(int baseAddress_)
{
    baseAddress = baseAddress_ << 16;
    return getModuleType();

}

QString ScalerModule::getModuleType()
{
    u_int16_t value16;

    // check fixed code of the board first
    CVErrorCodes res = CAENVME_ReadCycle(handleChef, baseAddress + 0xFA, &value16, cvA32_U_DATA, cvD16);

    if (res != cvSuccess || value16 != 0xFAF5){
        qDebug() << "Wrong board";
        return "UNKNOWN";
    }
    else {
        CAENVME_ReadCycle(handleChef, baseAddress + 0xFC, &value16, cvA32_U_DATA, cvD16);
        return QString::number(value16 & 0x3FF);
    }

}

void ScalerModule::startPulser()
{
    CAENVME_StartPulser(handleChef, cvPulserA);
}

void ScalerModule::setPulserMode(unsigned char width, CVTimeUnits unit, int repeat)
{
    // set pulser mode
    CAENVME_SetPulserConf(handleChef, cvPulserA, width+1, width, unit, repeat, cvManualSW, cvManualSW);
    CAENVME_SetOutputConf(handleChef, cvOutput0, cvInverted, cvActiveLow, cvMiscSignals);
    qDebug() << "New pulser width: " << width << " " << unit;
}

double ScalerModule::getTruePulsePeriod()
{
    unsigned char period;
    double truePeriod = 0; // in ms
    unsigned char width;
    double trueWidth = 0; // in ms
    CVTimeUnits unit;
    unsigned char pulseNo;
    CVIOSources start, reset;
    CAENVME_GetPulserConf(handleChef, cvPulserA, &period, &width, &unit, &pulseNo, &start, &reset);

    switch (unit) {
        case cvUnit25ns:
            truePeriod = (int)(period) * 0.000025;
            trueWidth = (int)(width) * 0.000025;
            break;
        case cvUnit104ms:
            truePeriod = (int)(period) * 104;
            trueWidth = (int)(width) * 104;
            break;
        case cvUnit410us:
            truePeriod = (int)(period) * 0.41;
            trueWidth = (int)(width) * 0.41;
            break;
        case cvUnit1600ns:
            truePeriod = (int)(period) * 0.0016;
            trueWidth = (int)(width) * 0.0016;
            break;
    }

    qDebug() << "Period: " << truePeriod << " ms";
    qDebug() << "Width: " << trueWidth << " ms";
    return truePeriod;
}

u_int32_t ScalerModule::readChannel(int ch)
{
    u_int32_t value32 = 0;
    CAENVME_ReadCycle(handleChef, baseAddress + 0x10 + ch * 4, &value32, cvA32_U_DATA, cvD32);
    return value32;
}

void ScalerModule::readChannels(std::vector<int>* val)
{
    unsigned char values[64];
    int sizeOut = 0;
    CAENVME_BLTReadCycle(handleChef, baseAddress + 0x10, values, 64, cvA32_U_DATA, cvD32, &sizeOut);

    int *ch;
    for (int i = 0; i < 16; ++i) {
        ch = (int*) (values + i*4);
        (*val)[i] = *ch;
    }

}

void ScalerModule::resetChannels()
{
    uint16_t value16 = 0;
    CAENVME_ReadCycle(handleChef, baseAddress + 0x50, &value16, cvA32_U_DATA, cvD16);
}

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

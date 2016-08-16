#include "tdcmodule.h"
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

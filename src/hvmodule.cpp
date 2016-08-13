#include "hvmodule.h"
#include <qdebug.h>

HVModule::HVModule(int handleChef_):
    handleChef(handleChef_)
{
    baseAddress = -1;
}

QString HVModule::setBaseAddress(int baseAddress_)
{
    baseAddress = baseAddress_ << 16;
    return getBoardModel();

}

CVErrorCodes HVModule::getVMAX(u_int16_t* value16) {
    return CAENVME_ReadCycle(handleChef, baseAddress + 0x0050, value16, cvA32_U_DATA, cvD16);
}

CVErrorCodes HVModule::getIMAX(u_int16_t* value16) {
    return CAENVME_ReadCycle(handleChef, baseAddress + 0x0054, value16, cvA32_U_DATA, cvD16);
}

QString HVModule::getBoardModel()
{
    u_int16_t value16;
    // read board parameters
    QString model;
    CVErrorCodes res = CAENVME_ReadCycle(handleChef, baseAddress + 0x8116, &value16, cvA32_U_DATA, cvD16);
    model += QString((char) (value16 & 0xff));
    model += QString((char) ((value16 >> 8) & 0xff));
    CAENVME_ReadCycle(handleChef, baseAddress + 0x8118, &value16, cvA32_U_DATA, cvD16);
    model += QString((char) (value16 & 0xff));
    model += QString((char) ((value16 >> 8) & 0xff));
    CAENVME_ReadCycle(handleChef, baseAddress + 0x811A, &value16, cvA32_U_DATA, cvD16);
    model += QString((char) (value16 & 0xff));
    model += QString((char) ((value16 >> 8) & 0xff));


    if (res != cvSuccess || !model.startsWith("V653")){
        qDebug() << "Wrong board";
        return "UNKNOWN";
    }
    else {
        return model;
    }
}



double HVModule::getVoltage(int channel)
{
    double voltage;
    if (channel < 0 || channel > 5) voltage = 888888;
    else {
        u_int16_t value16;
        CVErrorCodes res = CAENVME_ReadCycle(handleChef, baseAddress + 0x80 * channel + 0x88, &value16, cvA32_U_DATA, cvD16);
        if (res != cvSuccess) voltage = 888888;
        else voltage = value16*0.1;
    }
    return voltage;
}


double HVModule::getCurrent(int channel)
{
    double current;
    if (channel < 0 || channel > 5) current = 888888;
    else {
        u_int16_t value16;
        CVErrorCodes res = CAENVME_ReadCycle(handleChef, baseAddress + 0x80 * channel + 0x8C, &value16, cvA32_U_DATA, cvD16);
        if (res != cvSuccess) current = 888888;
        else current = value16*0.05;
    }
    return current;
}

u_int16_t HVModule::getChStatus(int channel)
{
    u_int16_t status;
    if (channel < 0 || channel > 5) status = 0xFFFF;
    else {
        u_int16_t value16;
        CVErrorCodes res = CAENVME_ReadCycle(handleChef, baseAddress + 0x80 * channel + 0x8C, &value16, cvA32_U_DATA, cvD16);
        if (res != cvSuccess) status = 0xFFFF;
        else status = value16;
    }
    return status;
}

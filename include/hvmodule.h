#ifndef HVMODULE_H
#define HVMODULE_H

class HVModule
{
private:
    // handle to connection to the VME crate
    int handleChef;
    // base address of the module
    int baseAddress;

public:
    HVModule(int handleChef_, int baseAddress_);
};

#endif // HVMODULE_H

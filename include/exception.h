#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <util.h>
#include <uart.h>
#include <timer.h>
#include <larchintrin.h>

class Exception {
public:
    void HandleMachineError();
    void HandleTLBException();
    void HandleDefaultException();
    Exception();
    void IntOn();
    void IntOff();
};

extern Exception SysException;
#endif

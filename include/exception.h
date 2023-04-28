#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <util.h>
#include <uart.h>
#include <clock.h>
#include <larchintrin.h>

class Exception {
    static void handleMachineError() {
        uPut << "Machine Error\n";
        __asm__ ("ertn");
    }
    static void handleTLBException() {
        __asm__ ("ertn");
    }
    static void handleDefaultException() {
        u32 estate = __csrrd_d(0x5);
        switch (getPartical(estate, 21, 16)) {
            case 0:
                for (u8 intrOp = 12; intrOp >= 0; --intrOp) {
                    if (estate & (1 << intrOp)) {
                        switch (intrOp) {
                            case 11:
                                SysClock.ClockIntrClear();
                        }
                        break;
                    }
                }
                break;
            default:
                uPut << "Exception\n";
                uPut << "estate: " << estate << "\n";
        }
        __asm__ ("ertn");
    }
public:
    Exception();
    void IntOn();
    void IntOff();
};

extern Exception SysException;
#endif

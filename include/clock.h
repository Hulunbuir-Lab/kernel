#ifndef CLOCK_H
#define CLOCK_H

#include <util.h>
#include <uart.h>
#include <larchintrin.h>

class Clock {
public:
    Clock();
    void ClockOn();
    void ClockOff();
    void ClockIntrClear();
};

extern Clock SysClock;

#endif

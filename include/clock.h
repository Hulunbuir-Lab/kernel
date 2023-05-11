#ifndef CLOCK_H
#define CLOCK_H

#include <util.h>
#include <uart.h>
#include <compare>

class RTCVal;

class Clock {
    u32 *const baseAddress;
public:
    Clock(u32 *addr);
    RTCVal GetRTC();
};

extern Clock SysClock;

class RTCVal{
    u32 val;
public:
    friend auto operator <=> (const RTCVal & a, const RTCVal & b) {
        RTCVal now = SysClock.GetRTC();
        return (a.val - now.val) <=> (b.val - now.val);
    }
    friend auto operator + (const RTCVal & a, const RTCVal & b) {
        return RTCVal(a.val + b.val);
    }
    operator unsigned int() {
        return val;
    }
    RTCVal(u32 val = 0);
};

#endif

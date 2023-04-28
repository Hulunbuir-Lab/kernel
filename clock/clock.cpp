#include <clock.h>

Clock::Clock() {
    __csrwr_d(0x1100010, 0x41);
}

void Clock::ClockOff(){
    u64 tcfg = __csrrd_d(0x41);
    tcfg &= (~1ull);
    __csrwr_d(tcfg, 0x41);
}

void Clock::ClockOn() {
    u64 tcfg = __csrrd_d(0x41);
    tcfg |= 1;
    __csrwr_d(tcfg, 0x41);
}

void Clock::ClockIntrClear() {
    __csrwr_d(1, 0x44);
    ClockOn();
}


Clock SysClock;

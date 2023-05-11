#include <clock.h>

Clock::Clock(u32* addr): baseAddress(addr) {
    *(baseAddress + 8) = 0;
    *(baseAddress + 24) = 0;
    *(baseAddress + 16) = (1 << 8) + (1 << 11) + (1 << 13);
}

RTCVal Clock::GetRTC() {
    return RTCVal(*(baseAddress + 26));
}

RTCVal::RTCVal(u32 val): val(val) {}

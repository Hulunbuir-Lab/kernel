#ifndef SDCARD_H
#define SDCARD_H

#include <util.h>
#include <uart.h>

class SDCard {
    u32 *baseAddress;
    void execCmd(u8 cmdId);
public:
    SDCard(void* addr);
    void test();
};

#endif

#include <sdcard.h>

SDCard::SDCard(void* addr): baseAddress((u32 *)addr) {
    *(baseAddress) = 1;
    *(baseAddress + 1) = 3;
    *(baseAddress + 16) = 0x3FF;
}

void SDCard::execCmd(u8 cmdId) {
    *(baseAddress + 3) = cmdId + (1 << 8);
    while (!(*(baseAddress + 15) & 1)) {
        uPut << (void *) *(baseAddress + 15) << '\n';
    }
    uPut << '\n';
    uPut << *(baseAddress + 5) << ' ' << *(baseAddress + 6) << ' ' << *(baseAddress + 7) << ' ' << *(baseAddress + 8) << '\n';
}

void SDCard::test() {
    execCmd(0);
}

#include <exception.h>

Exception::Exception() {
    __csrwr_d((u64) handleDefaultException, 0xC);
    __csrwr_d((u64) handleTLBException, 0x88);
    __csrwr_d((u64) handleMachineError, 0x93);
    __csrwr_w((1 << 13) - 1, 0x4);
}

void Exception::IntOff() {
    u32 crmd = __csrrd_w(0x0);
    crmd &= (~(1u << 2));
    __csrwr_w(crmd, 0x0);
}

void Exception::IntOn(){
    u32 crmd = __csrrd_w(0x0);
    crmd |= (1u << 2);
    __csrwr_w(crmd, 0x0);
}

Exception SysException;

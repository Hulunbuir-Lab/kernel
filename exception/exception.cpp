#include <exception.h>

extern "C" {
    void LoadContext();
    void StoreContext();
}

Exception::Exception() {
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

void Exception::HandleDefaultException() {
    u32 estate = __csrrd_d(0x5);
    switch (getPartical(estate, 21, 16)) {
        case 0:
            for (u8 intrOp = 12; intrOp >= 0; --intrOp) {
                if (estate & (1 << intrOp)) {
                    switch (intrOp) {
                        case 11:
                            SysTimer.TimerIntClear();
                    }
                    break;
                }
            }
            break;
        default:
            uPut << "Exception\n";
            uPut << "estate: " << estate << "\n";
    }
}

void Exception::HandleTLBException() {

}

void Exception::HandleMachineError(){
    uPut << "Machine Error\n";
}


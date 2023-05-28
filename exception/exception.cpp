#include <exception.h>

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
    u64 estate = __csrrd_d(0x5);
    switch (getPartical(estate, 21, 16)) {
        case 0:
            for (u8 intrOp = 12; intrOp >= 0; --intrOp) {
                if (estate & (1 << intrOp)) {
                    switch (intrOp) {
                        case 11:
                            ++Jiffies::sysJiffies;
                            processController.HandleSchedule();
                            SysTimer.TimerIntClear();
                    }
                    break;
                }
            }
            break;
        case 0xB:
            {
                u16 code = getPartical(__csrrd_d(0x8), 14, 0);
                if (code == 0) {
                    uPut << 'A';
                    for (u32 i = 0; i < 3000000; ++i);
                }
                else if (code == 1) {
                    uPut << 'B';
                    for (u32 i = 0; i < 3000000; ++i);
                }
            }
            break;
        default:
            uPut << "Exception\n";
            uPut << "ESTATE: " << (void*) estate << '\n';
            uPut << "ERA: " << (void*) __csrrd_d(0x6) << '\n';
            uPut << "BADV: " << (void*) __csrrd_d(0x7) << '\n';
            uPut << "BADI: " << (void*) __csrrd_d(0x8) << '\n';
            while (1);
    }
}

void Exception::HandleTLBException() {
    if (!processController.CurrentProcess) {
        uPut << (void*)__csrrd_d(0x89) << '\n' << (void*)__csrrd_d(0x90) << '\n' << "Kernel Panic" << '\n';
        while (1);
    }

    u64 addr = __csrrd_d(0x89);
    auto zone = processController.CurrentProcess->GetSpace()->ZoneTree->find([addr](Zone* t)->u8{
        if (t->VStart <= addr && addr <= t->VEnd) return 1;
        else if (addr < t->VStart) return 0;
        else return 2;
    });
    zone->val->OnPageFault(addr);
}

void Exception::HandleMachineError(){
    uPut << "Machine Error:\n";
    uPut << "MERRERA: " << (void*) __csrrd_d(0x94) << '\n';
    uPut << "MERRCTL: " << (void*) __csrrd_d(0x90) << '\n';
    uPut << "MERRINFO1: " << (void*) __csrrd_d(0x91) << '\n';
    uPut << "MERRINFO2: " << (void*) __csrrd_d(0x92) << '\n';
    while (1);
}

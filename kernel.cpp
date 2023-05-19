#include <uart.h>
#include <mem.h>
#include <util.h>
#include <exception.h>
#include <larchintrin.h>
#include <timer.h>
#include <lwp.h>

const u64 PageAreaStart = upAlign((u64 ) &KernelEnd, PAGE_SIZE_BIT);
const u64 vaddrEnd = 1ull << (getPartical(getCPUCFG(1), 19, 12) - 1);

UART uPut((u8 *)(0x800000001ff40800llu));
Exception SysException;
Timer SysTimer;

PageAllocator pageAllocator;

SlabNodeArea slabNodeZone;
SlabNodeAllocator slabNodeAllocator;
SlabArea defaultSlabZone;
SlabAllocator defaultSlabAllocator;

ProcessController processController;

extern "C" {
    void handleDefaultException() {
        SysException.HandleDefaultException();
    }

    void handleTLBException() {
        SysException.HandleTLBException();
    }

    void handleMachineError() {
        SysException.HandleMachineError();
    }
}

inline void invokeInit() {
    using func_ptr = void (*) (void);
    extern char __init_array_start, __init_array_end;
    for (func_ptr *func = (func_ptr *) &__init_array_start; func != (func_ptr *) &__init_array_end; ++func) {
        if (func) (*func)();
    }
}

inline void initMem() {
    __csrwr_d(0x13E4D52C, 0x1C);
    __csrwr_d(0x267, 0x1D);
}

inline void initException() {
    extern void *HandleDefaultExceptionEntry, *HandleMachineErrorEntry, *HandleTLBExceptionEntry;
    __csrwr_d((u64)&HandleDefaultExceptionEntry, 0xC);
    __csrwr_d((u64)&HandleTLBExceptionEntry, 0x88);
    __csrwr_d((u64)&HandleMachineErrorEntry, 0x93);

    SysException.IntOn();
    //SysTimer.TimerOn();
}

extern "C" void KernelMain() {
    invokeInit();
    initMem();
    initException();

    //extern void StartProcess();

    //StartProcess();
    while (1);
}

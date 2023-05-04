#include <uart.h>
#include <mem.h>
#include <util.h>
#include <exception.h>
#include <larchintrin.h>
#include <clock.h>

const u64 PageAreaStart = upAlign((u64 ) &KernelEnd, PAGE_SIZE_BIT);
const u64 vaddrEnd = 1ull << (getPartical(getCPUCFG(1), 19, 12) - 1);

UART uPut((u8 *)(0x1ff40800llu));

PageAllocator pageAllocator;

MemSpace kernelSpace(0, vaddrEnd);
TNode<Zone> kernelDirectZone(DirectZone(0, PageAreaStart - 1, 0));
TNode<Zone> kernelDynamicZone(DynamicZone(PageAreaStart, vaddrEnd));

MemSpace *currentMemSpace;

SlabNodeZone slabNodeZone;
SlabNodeAllocator slabNodeAllocator;
DefaultSlabZone defaultSlabZone;
DefaultSlabAllocator defaultSlabAllocator;

void invokeInit() {
    using func_ptr = void (*) (void);
    extern char __init_array_start, __init_array_end;
    for (func_ptr *func = (func_ptr *) &__init_array_start; func != (func_ptr *) &__init_array_end; ++func) {
        if (func) (*func)();
    }
}

void initMem()
{
    __csrwr_d(0x13E4D52C, 0x1C);
    __csrwr_d(0x267, 0x1D);

    kernelSpace.MMUService.setPageTable(pageAllocator.AllocPageMem(0));
    kernelSpace.AddZone(&kernelDirectZone);
    kernelSpace.AddZone(&kernelDynamicZone);
}

extern "C" void KernelMain() {
    invokeInit();
    initMem();
    SysException.IntOn();

    while (1);
}

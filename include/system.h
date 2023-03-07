#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED

#include <uart.h>
#include <mem.h>
#include <util.h>

extern void* KernelEnd;

class System {
    void *acpiBase;
    u64 efiMemMapStart;
    u64 efiMemMapEnd;
    u64 efiMemMapDescriptorSize;

    UART uPut;
    PageAllocator pageAllocator;
    MemSpace kernelSpace;
    DirectZone kernelDirectZone;
    DynamicZone kernelDynamicZone;
public:
    System();
    void InitMem(KernelInfo & info);
    void Run();
};

#endif // SYSTEM_H_INCLUDED

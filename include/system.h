#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED

#include <uart.h>

class System {
    void *acpiBase;
    u64 efiMemMapStart;
    u64 efiMemMapEnd;
    u64 efiMemMapDescriptorSize;

    UART uPut;
public:
    System(KernelInfo & info);
    void Run();
};

#endif // SYSTEM_H_INCLUDED

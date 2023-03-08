#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED

#include <uart.h>
#include <mem.h>
#include <util.h>

class System {
    void *acpiBase;
    u64 efiMemMapStart;
    u64 efiMemMapEnd;
    u64 efiMemMapDescriptorSize;

    void InitPage();
    void InitMem();
public:
    System(KernelInfo& info);
    void Init();
    void Run();
};

#endif // SYSTEM_H_INCLUDED

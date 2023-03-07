#include <system.h>

System::System()
    :uPut((u8 *)0x1fe001e0),
     kernelSpace(0, (1ull << (getPartical(getCPUCFG(1), 11, 4) - 1),
     kernelDirectZone(0, (u64) KernelEnd + PAGE_SIZE & (~(PAGE_SIZE - 1))),
     kernelDynamicZone()
    {}

void System::InitMem(KernelInfo& info)
{
    acpiBase = info.XdspAddress;
    efiMemMapStart = (u64) info.MemMapAddress;
    efiMemMapEnd = (u64) info.MemMapAddress + info.MemMapSize;
    efiMemMapDescriptorSize = info.MemMapDescriptorSize;

    efiMemDescriptor *aMem;
    u64 maxMem = 0;

    for (u64 pt = efiMemMapStart; pt < efiMemMapEnd; pt += efiMemMapDescriptorSize) {
        aMem = (efiMemDescriptor *) pt;
        maxMem = kMax(maxMem, aMem->PhyStart + (aMem->PageNum << PAGE_SIZE_BIT));
    }


    if (maxMem < 0xC0000000)

    for (u64 pt = efiMemMapStart; pt < efiMemMapEnd; pt += efiMemMapDescriptorSize) {
        aMem = (efiMemDescriptor *) pt;
        if (aMem->PhyStart > (u64) &KernelEnd) {
        }
    }
}


void System::Run() {
    while (1);
}

#include <system.h>

System::System(KernelInfo& info) {
    acpiBase = info.XdspAddress;
    efiMemMapStart = (u64) info.MemMapAddress;
    efiMemMapEnd = (u64) info.MemMapAddress + info.MemMapSize;
    efiMemMapDescriptorSize = info.MemMapDescriptorSize;
}

void System::InitPage()
{
    efiMemDescriptor *aMem;
    u64 maxMem = 0, dynamicMemMapDespStart = 0;

    for (u64 pt = efiMemMapStart; pt < efiMemMapEnd; pt += efiMemMapDescriptorSize) {
        aMem = (efiMemDescriptor *) pt;
        maxMem = kMax(maxMem, aMem->PhyStart + (aMem->PageNum << PAGE_SIZE_BIT));
        if (dynamicMemMapDespStart == 0 && aMem->PhyStart >= separator) {
            dynamicMemMapDespStart = pt;
        }
    }

    if (maxMem < 0xC0000000)
        pageAllocator.SetPageInfo(upAlign((u64)(&KernelEnd), PAGE_SIZE_BIT + PAGE_GROUP_SIZE_BIT), separator);
    else
        pageAllocator.SetPageInfo(0x90000000, separator);

    for (u64 pt = dynamicMemMapDespStart; pt < efiMemMapEnd; pt += efiMemMapDescriptorSize) {
        aMem = (efiMemDescriptor *) pt;
        switch (aMem->Type) {
            case EfiLoaderCode:
            case EfiLoaderData:
            case EfiBootServicesCode:
            case EfiBootServicesData:
            case EfiConventionalMemory:
                pageAllocator.AddArea(aMem->PhyStart, aMem->PhyStart + (aMem->PageNum << PAGE_SIZE_BIT) - 1, 0);
            default:
                pageAllocator.AddArea(aMem->PhyStart, aMem->PhyStart + (aMem->PageNum << PAGE_SIZE_BIT) - 1, 1);
        }
    }
}

void System::Init()
{
    InitPage();
    InitMem();
}

void System::Run() {
    while (1);
}

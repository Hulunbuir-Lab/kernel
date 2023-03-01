#include <system.h>

SystemManager::SystemManager(KernelInfo & info) :uPut((_kU8 *)0x1fe001e0UL){
    acpiBase = info.xdspAddress;
    efiMemMapStart = (_kU64) info.memMapAddress;
    efiMemMapEnd = (_kU64) info.memMapAddress + info.memMapSize;
    efiMemMapDescriptorSize = info.memMapDescriptorSize;
}

void SystemManager::Run() {
    for (_kU64 pt = efiMemMapStart; pt < efiMemMapEnd; pt += efiMemMapDescriptorSize) {
        efiMemDescriptor *aMem = (efiMemDescriptor *) pt;
        uPut.UARTPut(aMem->type);
        uPut.UARTPut('\t');
        uPut.UARTPut((void *)aMem->phyStart);
        uPut.UARTPut('\t');
        uPut.UARTPut((void *)aMem->virtStart);
        uPut.UARTPut('\t');
        uPut.UARTPut((void *)aMem->pageNum);
        uPut.UARTPut('\n');
    }
}

#include <system.h>

System::System(KernelInfo & info) :uPut((u8 *)0x1fe001e0UL){
    acpiBase = info.xdspAddress;
    efiMemMapStart = (u64) info.memMapAddress;
    efiMemMapEnd = (u64) info.memMapAddress + info.memMapSize;
    efiMemMapDescriptorSize = info.memMapDescriptorSize;
}

void System::Run() {
    for (u64 pt = efiMemMapStart; pt < efiMemMapEnd; pt += efiMemMapDescriptorSize) {
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

#include <mem.h>

PageAllocator pageAllocator;
MemSpace kernelSpace(0, vaddrEnd);
DirectZone kernelDirectZone(0, separator - 1);
DynamicZone kernelDynamicZone(separator, vaddrEnd);
MemSpace *currentMemSpace;

void Zone::OnPageFault(u64 vaddr)
{
    space->MMUService.AddItem(vaddr, (u64) pageAllocator.AllocMem(0));
}

MemSpace::MemSpace(u64 vStart, u64 vEnd) :VStart(vStart), VEnd(vEnd) {}

void MemSpace::AddZone(Zone* t)
{
    t->space = this;
    if (Root->find([t](Zone *ct)->u8{
        if (t->VEnd < ct->VStart) return -1;
        else if (t->VStart > ct->VEnd) return 1;
        else return 0;
    }))
}

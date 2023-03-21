#include <mem.h>

PageAllocator pageAllocator;
MemSpace kernelSpace(0, vaddrEnd);
TNode<Zone> kernelDirectZone(DirectZone(0, separator - 1, 0));
TNode<Zone> kernelDynamicZone(DynamicZone(separator, vaddrEnd));
MemSpace *currentMemSpace;

void Zone::OnPageFault(u64 vaddr)
{
    space->MMUService.AddItem(vaddr, (u64) pageAllocator.AllocMem(0));
}

void DirectZone::OnPageFault(u64 vaddr)
{
    space->MMUService.AddItem(vaddr, vaddr + Offset);
}

MemSpace::MemSpace(u64 vStart, u64 vEnd) :VStart(vStart), VEnd(vEnd) {}

void MemSpace::AddZone(Zone* t)
{
    t->space = this;
    if (Root->find([t](Zone *ct)->u8{
        if (t->VEnd < ct->VStart) return 1;
        else if (t->VStart > ct->VEnd) return -1;
        else return 0;
    }) == nullptr) {
        Root->insert(&kernelDirectZone, [t](Zone *ct)->bool{
            if (ct->VStart > t->VEnd) return 0;
            else return 1;
        });
    }
}

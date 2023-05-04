#include <mem.h>

void Zone::OnPageFault(u64 vaddr)
{
    space->MMUService.AddItem(vaddr, (u64) pageAllocator.AllocPageMem(0));
}

void DirectZone::OnPageFault(u64 vaddr)
{
    space->MMUService.AddItem(vaddr, vaddr + Offset);
}

MemSpace::MemSpace(u64 vStart, u64 vEnd) :VStart(vStart), VEnd(vEnd) {}

void MemSpace::AddZone(TNode<Zone>* t)
{
    t->val.space = this;
    if (Root->find([t](Zone *ct)->u8{
        if (t->val.VEnd < ct->VStart) return 2;
        else if (t->val.VStart > ct->VEnd) return 0;
        else return 1;
    }) == nullptr) {
        Root->insert(&kernelDirectZone, [t](Zone *ct)->bool{
            if (ct->VStart > t->val.VEnd) return 0;
            else return 1;
        });
    }
}

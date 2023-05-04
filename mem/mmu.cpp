#include <mem.h>

struct PTE{
    u8 rplv:1;
    u8 nx:1;
    u8 nr:1;
    u16 resv1:13;
    u64 pa:36;
    u8 resv2:3;
    u8 w:1;
    u8 p:1;
    u8 g:1;
    u8 mat:2;
    u8 plv:2;
    u8 d:1;
    u8 v:1;
};

void MMU::AddItem(u64 vaddr, u64 paddr)
{
    PTE* p1 = (PTE*) pageTable + getPartical(vaddr, 47, 39);
    if (p1->pa == 0) {
        p1->pa = (u64) pageAllocator.AllocPageMem(0) >> 12;
    }

    PTE* p2 = (PTE*) (p1->pa << 12) + getPartical(vaddr, 38, 30);
    if (p2->pa == 0) {
        p2->pa = (u64) pageAllocator.AllocPageMem(0) >> 12;
    }

    PTE* p3 = (PTE*) (p2->pa << 12) + getPartical(vaddr, 29, 21);
    if (p2->pa == 0) {
        p2->pa = (u64) pageAllocator.AllocPageMem(0) >> 12;
    }

    PTE* p4 = (PTE*) (p3->pa << 12) + getPartical(vaddr, 20, 12);
    p4->pa = paddr >> 12;
}

void MMU::DeleteItem(u64 vaddr)
{
    PTE* p1 = (PTE*) pageTable + getPartical(vaddr, 47, 39);
    if (p1->pa == 0) return;

    PTE* p2 = (PTE*) (p1->pa << 12) + getPartical(vaddr, 38, 30);
    if (p2->pa == 0) return;

    PTE* p3 = (PTE*) (p2->pa << 12) + getPartical(vaddr, 29, 21);
    if (p2->pa == 0) return;

    PTE* p4 = (PTE*) (p3->pa << 12) + getPartical(vaddr, 20, 12);
    p4->pa = 0;
}

u64 MMU::V2P(u64 vaddr)
{
    PTE* p1 = (PTE*) pageTable + getPartical(vaddr, 47, 39);
    if (p1->pa == 0) return 0;

    PTE* p2 = (PTE*) (p1->pa << 12) + getPartical(vaddr, 38, 30);
    if (p2->pa == 0) return 0;

    PTE* p3 = (PTE*) (p2->pa << 12) + getPartical(vaddr, 29, 21);
    if (p2->pa == 0) return 0;

    PTE* p4 = (PTE*) (p3->pa << 12) + getPartical(vaddr, 20, 12);
    return p4->pa;
}

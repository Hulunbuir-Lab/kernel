#ifndef MEM_H
#define MEM_H

#include <util.h>
#include <uart.h>
#include <tree.h>

#define PAGE_GROUP_SIZE_BIT 12
#define PAGE_SIZE_BIT 12
#define PAGE_SIZE (1 << PAGE_SIZE_BIT)
#define PAGEINFO_SIZE_BIT 5

struct Page {
	Page *Prev;
	Page *Next;
	u64 RefCount;
    u64 SizeBit;
};

class MMU {
    void *pageTable;
public:
    void setPageTable(void* addr) {
        pageTable = addr;
    }
    void setPGDL();
    void AddItem(u64 vaddr, u64 paddr);
    void DeleteItem(u64 vaddr);
    u64 V2P(u64 vaddr);
};

class MemSpace;

class Zone {
public:
    Zone(u64 start, u64 end) :VStart(start), VEnd(end){}
	virtual void OnPageFault(u64 vaddr);
	u64 VStart;
	u64 VEnd;
    MemSpace *space;
    u8 Attr;
};

class MemSpace {
public:
	u64 VStart;
	u64 VEnd;
	MMU MMUService;
	Tree<Zone> *Root;
    MemSpace(u64 vStart, u64 vEnd);
    void AddZone(TNode<Zone> *t);
    void DeleteZone(Zone *t);
};

class DirectZone : public Zone {
public:
    u64 Offset;
    DirectZone(u64 vstart, u64 vend, u64 offset): Zone(vstart, vend), Offset(offset){}
    virtual void OnPageFault(u64 vaddr) override;
};

class PageAllocator {
    Page *pageInfo;
    u64 pageAreaStart;
	Page* buddyHeadList[PAGE_GROUP_SIZE_BIT];
	Page* getBuddyPage(Page *t) {
      return (Page *)((u64)t ^ (1 << (t->SizeBit + PAGEINFO_SIZE_BIT)));
    }
    void setupPage(Page* t, u8 sizeBit) {
      t->SizeBit = sizeBit;
    }
    void addPageToBuddy(Page* t);
    void deletePageFromBuddy(Page *t);
public:
	PageAllocator();
    void SetPageInfo(u64 pageInfoAddress, u64 pageAreaStart);
	u64 PageToAddr(Page* t) {
      return (((t - pageInfo) << PAGE_SIZE_BIT) + (u64) pageAreaStart);
    }
	Page* AddrToPage(u64 t) {
      return pageInfo + ((t - pageAreaStart) >> PAGE_SIZE_BIT);
    }
	Page* AllocPage(u8 sizeBit);
    void* AllocPageMem(u8 sizeBit) {
        Page * t = AllocPage(sizeBit);
        return (void*) PageToAddr(t);
    }
	void FreePage(Page* t);
    void FreePageMem(void* addr) {
        Page *t = AddrToPage((u64) addr);
        FreePage(t);
    }
    void AddArea(u64 start, u64 end, bool isMaskedAsIllegal);
    void ListPage();
};

struct SlabNode {
    void* Start;
    u16 Size;
    SlabNode *Next;
};

class Slab {
protected:
    void* pageMem;
public:
    virtual void* Malloc(u16 size) = 0;
    virtual bool Free(void* addr) = 0;
    Slab *Next;
};

class SlabNodeArea: public Slab {
    u8 bitMap[28];
    bool used;
public:
    SlabNodeArea();
    void* Malloc(u16 size);
    bool Free(void* addr);
};

class SlabArea: public Slab{
    SlabNode *availableMem;
    SlabNode *usedMem;
    friend class SlabAllocator;
    bool used;
public:
    SlabArea();
    void* Malloc(u16 size);
    bool Free(void* addr);
};

class SlabNodeAllocator {
    SlabNodeArea *slabNodeZonePtr;
    u64 zoneNum;
    u64 nodeNum;
public:
    SlabNodeAllocator();
    SlabNode* AllocNode();
    void FreeNode(SlabNode *node);
};

class SlabAllocator {
    SlabArea *defaultSlabZonePtr;
public:
    SlabAllocator();
    void* Malloc(u16 size);
    bool Free(void* addr);
    void ListZone();
};

extern void* KernelEnd;
extern const u64 PageAreaStart;
extern const u64 vaddrEnd;

extern PageAllocator pageAllocator;

extern SlabNodeArea slabNodeZone;
extern SlabNodeAllocator slabNodeAllocator;
extern SlabArea defaultSlabZone;
extern SlabAllocator defaultSlabAllocator;

extern MemSpace *currentMemspace;

#endif

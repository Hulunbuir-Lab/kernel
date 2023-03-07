#ifndef MEM_H
#define MEM_H

#include <util.h>
#include <array>

#define PAGE_GROUP_SIZE_BIT 12
#define PAGE_SIZE_BIT 12
#define PAGE_SIZE (1 << PAGE_SIZE_BIT)
#define PAGEINFO_SIZE_BIT 5

struct KernelInfo {
  void *XdspAddress;
  void *MemMapAddress;
  u64 MemMapSize;
  u64 MemMapDescriptorSize;
};

enum efiMemType: u64{
  EfiReservedMemoryType,
  EfiLoaderCode,
  EfiLoaderData,
  EfiBootServicesCode,
  EfiBootServicesData,
  EfiRuntimeServicesCode,
  EfiRuntimeServicesData,
  EfiConventionalMemory,
  EfiUnusableMemory,
  EfiACPIReclaimMemory,
  EfiACPIMemoryNVS,
  EfiMemoryMappedIO,
  EfiMemoryMappedIOPortSpace,
  EfiPalCode,
  EfiPersistentMemory,
  EfiMaxMemoryType
};

struct efiMemDescriptor {
    efiMemType Type;
    u64 PhyStart;
    u64 VirtStart;
    u64 PageNum;
    u64 Attr;
};

struct Page {
	Page *Prev;
	Page *Next;
	u64 RefCount;
    u64 SizeBit;
};

class MemSpace;

class Zone {
public:
    Zone(u64 start, u64 end) :VStart(start), VEnd(end){}
	virtual void OnPageFault();
	u64 VStart;
	u64 VEnd;
	Zone *Left;
	Zone *Right;
    MemSpace *Fa;
    u8 Attr;
};

class MemSpace {
public:
	u64 VStart;
	u64 VEnd;
	u64* PageTable;
	Zone *Root;
    MemSpace(u64 vStart, u64 vEnd);
    bool AddZone(Zone *t);
    bool DeleteZone(Zone *t);
};

class DirectZone : Zone {
public:
    DirectZone(u64 start, u64 end): Zone(start, end){}
};

class DynamicZone : Zone {
public:
    DynamicZone(u64 start, u64 end): Zone(start, end){}
};

class PageAllocator {
    Page *pageInfo;
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
    void SetPageInfo(u64 pageInfoAddress);
	void* PageToAddr(Page* t) {
      return (void *)(((t - pageInfo) << (PAGE_GROUP_SIZE_BIT + PAGEINFO_SIZE_BIT)) + (u64) pageInfo);
    }
	Page* AddrToPage(u64 t) {
      return pageInfo + ((t - (u64) pageInfo) >> (PAGE_GROUP_SIZE_BIT + PAGEINFO_SIZE_BIT));
    }
	Page* AllocPage(u8 sizeBit);
	void FreePage(Page* t);
    void AddArea(u64 start, u64 end, bool isMaskedAsIllegal);
};

#endif

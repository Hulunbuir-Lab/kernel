#ifndef MEM_H
#define MEM_H

#include <util.h>
#include <array>

#define PAGE_GROUP_SIZE_BIT 12
#define PAGE_SIZE_BIT 12
#define PAGE_SIZE (1 << PAGE_SIZE_BIT)
#define PAGEINFO_SIZE_BIT 5
#define HI

struct KernelInfo {
  void *xdspAddress;
  void *memMapAddress;
  u64 memMapSize;
  u64 memMapDescriptorSize;
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
    efiMemType type;
    u64 phyStart;
    u64 virtStart;
    u64 pageNum;
    u64 attr;
};

struct Page {
	Page *prev;
	Page *next;
	u64 refCount;
    u64 sizeBit;
};

class VNode {
public:
	virtual void onPageFault();
	u64 vStart;
	u64 vEnd;
	VNode *left;
	VNode *right;
};

class MemSpace {
	u64 vStart;
	u64 vEnd;
	u64* pageTable;
	VNode *root;
};

class PageAllocator {
    Page *pageInfo;
	std::array<Page*, PAGE_GROUP_SIZE_BIT> buddyHeadList;
	Page* getBuddyPage(Page *t) {
      return (Page *)((u64)t ^ (1 << (t->sizeBit + PAGEINFO_SIZE_BIT)));
    }
    void setupPage(Page* t, u8 sizeBit) {
      t->sizeBit = sizeBit;
    }
    void addPageToBuddy(Page* t);
    void deletePageFromBuddy(Page *t);
public:
	PageAllocator(u64 start, u64 end);
	void* pageToAddr(Page* t) {
      return (void *)(((t - pageInfo) << (PAGE_GROUP_SIZE_BIT + PAGEINFO_SIZE_BIT)) + (u64) pageInfo);
    }
	Page* addrToPage(u64 t) {
      return pageInfo + ((t - (u64) pageInfo) >> (PAGE_GROUP_SIZE_BIT + PAGEINFO_SIZE_BIT));
    }
	Page* allocPage(u8 sizeBit);
	void freePage(Page* t);
};

#endif

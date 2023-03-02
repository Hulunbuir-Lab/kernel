#ifndef MEM_H
#define MEM_H

#include <util.h>
#include <array>

#define PAGE_GROUP_SIZE_BIT 12
#define PAGE_SIZE_BIT 12
#define PAGE_SIZE (1 << PAGE_SIZE_BIT)
#define HIGH_MEM_START 0x90000000
#define PAGEINFO_SIZE_BIT 4

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
    u64 reserve;
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
    Page *pageInfo = (Page*) HIGH_MEM_START;
	std::array<Page*, PAGE_GROUP_SIZE_BIT> buddyHeadList;
	void addPage(Page* t);
	void deletePage(Page* t);
	Page* getBuddyPage(Page *t);
public:
	PageAllocator();
	void* pageToAddr(Page* t) {
      return (void *)(((t - pageInfo) << (PAGE_GROUP_SIZE_BIT + PAGEINFO_SIZE_BIT)) + HIGH_MEM_START);
    }
	Page* addrToPage(void* t) {
      return pageInfo + (((u64) t - HIGH_MEM_START) >> (PAGE_GROUP_SIZE_BIT + PAGEINFO_SIZE_BIT));
    }
	void* allocMem(u8 sizeBit);
	void freeMem(u8 sizeBit);
};

#endif

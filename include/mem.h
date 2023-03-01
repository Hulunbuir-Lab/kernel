#ifndef MEM_H
#define MEM_H

#include <util.h>
#include <array>

#define PAGE_GROUP_SIZE_BIT 12

struct Page {
	Page *prev;
	Page *next;
	u64 refCount;
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
	std::array<Page*, PAGE_GROUP_SIZE_BIT> buddyHeadList;
	void addPage(Page* t);
	void deletePage(Page* t);
	Page* getBuddyPage(Page *t);
public:
	PageAllocator(u64 start, u64 end);
	void* pageToAddr(Page* t);
	Page* addrToPage(void* t);
	void* allocMem(u8 sizeBit);
	void freeMem(u8 sizeBit);
};

#endif

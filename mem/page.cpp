#include <mem.h>

PageAllocator::PageAllocator(){}

void PageAllocator::SetPageInfo(u64 pageInfoAddress, u64 pageAreaStart)
{
    pageInfo = (Page *) pageInfoAddress;
    this->pageAreaStart = pageAreaStart;
}

void PageAllocator::AddArea(u64 start, u64 end, bool isMaskedAsIllegal)
{
    u8 currentPageSizeBit = PAGE_GROUP_SIZE_BIT - 1;
    Page *t;
    for (u64 pt = start; pt <= end; pt += (1 << (currentPageSizeBit + PAGE_SIZE_BIT))) {
        while (pt + (1 << (currentPageSizeBit + PAGE_SIZE_BIT)) - 1 > end) --currentPageSizeBit;
        t = AddrToPage(pt);
        setupPage(t, isMaskedAsIllegal ? PAGE_GROUP_SIZE_BIT : currentPageSizeBit);
        if (!isMaskedAsIllegal) addPageToBuddy(t);
    }
}

void PageAllocator::addPageToBuddy(Page* t)
{
    if (buddyHeadList[t->SizeBit]) {
        t->Next = buddyHeadList[t->SizeBit];
        buddyHeadList[t->SizeBit]->Prev = t;
    }
    buddyHeadList[t->SizeBit] = t;
}

void PageAllocator::deletePageFromBuddy(Page* t)
{
    if (t->Next) t->Next->Prev = t->Prev;
    if (t->Prev) t->Prev->Next = t->Next;
    else buddyHeadList[t->SizeBit] = t->Next;
    t->Next = t->Prev = nullptr;
}

Page* PageAllocator::AllocPage(u8 sizeBit) {
    Page *p = nullptr;
    for (u8 i = sizeBit; i < PAGE_GROUP_SIZE_BIT; ++i) {
        if (buddyHeadList[sizeBit] != nullptr) {
            p = buddyHeadList[sizeBit];
            break;
        }
    }
    deletePageFromBuddy(p);
    Page *buddy;
    if (p -> SizeBit != sizeBit) {
        for (; p->SizeBit > sizeBit; --p->SizeBit) {
            buddy = getBuddyPage(p);
            setupPage(buddy, p->SizeBit - 1);
            addPageToBuddy(buddy);
        }
    }
    ++p->RefCount;
    return p;
}

void PageAllocator::FreePage(Page* t)
{
    --t->RefCount;
    if (t->RefCount != 0) return;
    Page *buddy;
    for (; t->SizeBit < PAGE_GROUP_SIZE_BIT; ++t->SizeBit) {
        buddy = getBuddyPage(t);
        if (buddy->RefCount == 0) {
            deletePageFromBuddy(buddy);
            setupPage(buddy, t->SizeBit);
            t = buddy - t > 0 ? t : buddy;
        } else {
            break;
        }
    }
    addPageToBuddy(t);
}

void PageAllocator::ListPage() {
    for (u8 i = 0; i < PAGE_GROUP_SIZE_BIT; ++i) {
        u64 c = 0;
        for (Page *t = buddyHeadList[i]; t != nullptr; t = t->Next) {
            ++c;
        }
        uPut << "2 ^ " << i << " = " << c << '\n';
    }
}

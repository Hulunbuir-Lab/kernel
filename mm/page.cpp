#include <mem.h>

PageAllocator::PageAllocator(u64 start, u64 end)
{
    pageInfo = (Page*) start;
    u8 currentPageSizeBit = PAGE_GROUP_SIZE_BIT - 1;
    Page *t;
    for (u64 pt = start; pt < end; pt += (1 << (currentPageSizeBit + PAGE_SIZE_BIT))) {
        while (pt + (1 << (currentPageSizeBit + PAGE_SIZE_BIT)) > end) --currentPageSizeBit;
        t = addrToPage(pt);
        setupPage(t, currentPageSizeBit);
        addPageToBuddy(t);
    }
}

void PageAllocator::addPageToBuddy(Page* t)
{
    if (buddyHeadList[t->sizeBit]) {
        t->next = buddyHeadList[t->sizeBit];
        buddyHeadList[t->sizeBit]->prev = t;
    }
    buddyHeadList[t->sizeBit] = t;
}

void PageAllocator::deletePageFromBuddy(Page* t)
{
    if (t->next) t->next->prev = t->prev;
    if (t->prev) t->prev->next = t->next;
    else buddyHeadList[t->sizeBit] = t->next;
    t->next = t->prev = nullptr;
}

Page* PageAllocator::allocPage(u8 sizeBit) {
    Page *p = nullptr;
    for (u8 i = sizeBit; i < PAGE_GROUP_SIZE_BIT; ++i) {
        if (buddyHeadList[sizeBit] != nullptr) {
            p = buddyHeadList[sizeBit];
            break;
        }
    }
    deletePageFromBuddy(p);
    Page *buddy;
    if (p -> sizeBit != sizeBit) {
        for (; p->sizeBit > sizeBit; --p->sizeBit) {
            buddy = getBuddyPage(p);
            setupPage(buddy, p->sizeBit - 1);
            addPageToBuddy(buddy);
        }
    }
    ++p->refCount;
    return p;
}

void PageAllocator::freePage(Page* t)
{
    --t->refCount;
    if (t->refCount != 0) return;
    Page *buddy;
    for (; t->sizeBit < PAGE_GROUP_SIZE_BIT; ++t->sizeBit) {
        buddy = getBuddyPage(t);
        if (buddy->refCount == 0) {
            deletePageFromBuddy(buddy);
            setupPage(buddy, t->sizeBit);
            t = buddy - t > 0 ? t : buddy;
        } else {
            break;
        }
    }
    addPageToBuddy(t);
}

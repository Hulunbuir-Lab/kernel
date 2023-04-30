
#include <mem.h>

static DefaultSlabZone defaultSlabZone;
DefaultSlabAllocator defaultSlabAllocator;
SlabNodeAllocator slabNodeAllocator;
static SlabNodeZone slabNodeZone;

DefaultSlabAllocator::DefaultSlabAllocator(){
    defaultSlabZonePtr = &defaultSlabZone;
}

void * DefaultSlabAllocator::Malloc(u16 size){
    DefaultSlabZone* zone = defaultSlabZonePtr;
    void *addr = zone->Malloc(size);
    while (addr == nullptr && zone->Next != nullptr) {
        zone = reinterpret_cast<DefaultSlabZone*>(zone->Next);
        addr = zone->Malloc(size);
    }
    return addr;
}

void DefaultSlabAllocator::Free(void* addr){
    DefaultSlabZone* zone = defaultSlabZonePtr;
    bool status = zone->Free(addr);
    while (status == false && zone->Next != nullptr) {
        zone = reinterpret_cast<DefaultSlabZone*>(zone->Next);
        status = zone->Free(addr);
    }
}

DefaultSlabZone::DefaultSlabZone(): used(false){}

void * DefaultSlabZone::Malloc(u16 size) {
    if (used == false) {
        Next = new ((void*) pageMem) DefaultSlabZone;
        availableMem = slabNodeAllocator.AllocNode();
        availableMem->Start = (DefaultSlabZone*) pageMem + 1;
        availableMem->Size = 4096 - sizeof(DefaultSlabZone);
        used = true;
    }
    SlabNode *nodePtr = availableMem;
    SlabNode *pre = nullptr;
    while (nodePtr != nullptr) {
        if (nodePtr->Size == size) {
            if (pre == nullptr) {
                availableMem = nodePtr->Next;
            } else {
                pre->Next = nodePtr->Next;
            }
            nodePtr->Next = usedMem;
            usedMem = nodePtr;
            return nodePtr->Start;
        } else if (nodePtr->Size > size) {
            SlabNode *newNode = slabNodeAllocator.AllocNode();
            newNode->Start = nodePtr->Start;
            newNode->Size = size;
            newNode->Next = usedMem;
            usedMem = newNode;
            nodePtr->Start = (void*)((u64) nodePtr->Start + size);
            nodePtr->Size = nodePtr->Size - size;
            return newNode->Start;
        }
        pre = nodePtr;
        nodePtr = nodePtr->Next;
    }
    return nullptr;
}

bool DefaultSlabZone::Free(void* addr){
    if (((u64) addr & 0xFFFFFFFFFFFFF000) != (u64) pageMem) return false;
    SlabNode *node = usedMem;
    SlabNode *p = nullptr;
    while (node != nullptr) {
        if (node->Start == addr) {
            if (p == nullptr) {
                usedMem = node->Next;
            } else {
                p->Next = node->Next;
            }
            node->Next = availableMem;
            availableMem = node;
            return true;
        }
        p = node;
        node = node->Next;
    }
    return false;
}

SlabNodeAllocator::SlabNodeAllocator(): zoneNum(1), nodeNum(0){
    slabNodeZonePtr = &slabNodeZone;
}

SlabNode * SlabNodeAllocator::AllocNode() {
    SlabNodeZone* zone = slabNodeZonePtr;
    void* node = zone->Malloc(sizeof(SlabNode));
    while (node == nullptr && zone->Next != nullptr) {
        zone = reinterpret_cast<SlabNodeZone*>(zone->Next);
        node = zone->Malloc(sizeof(SlabNode));
    }
    return (SlabNode*) node;
}

void SlabNodeAllocator::FreeNode(SlabNode* node) {
    SlabNodeZone* zone = slabNodeZonePtr;
    bool status = zone->Free(node);
    while (status == false && zone->Next != nullptr) {
        zone = reinterpret_cast<SlabNodeZone*>(zone->Next);
        status = zone->Free(node);
    }
}

SlabNodeZone::SlabNodeZone(): used(false){
    pageMem = pageAllocator.AllocMem(0);
    for (u8 i = 0; i < 28; ++i) bitMap[i] = 0;
}

void * SlabNodeZone::Malloc(u16 size) {
    if (size != sizeof(SlabNode)) return nullptr;
    if (used == false) {
        Next = new ((void*) ((u64) pageMem + 4032)) SlabNodeZone;
        used = true;
    }
    for (u8 i = 0; i < 28; ++i) {
        for (u8 j = 0; j < 8; ++j) {
            if ((bitMap[i] & (1 << j)) == 0) {
                bitMap[i] |= (1 << j);
                return (void *)((u64)pageMem + (i * 8 + j) * size);
            }
        }
    }
    return nullptr;
}


bool SlabNodeZone::Free(void* addr) {
    if (((u64) addr & 0xFFFFFFFFFFFFF000) != (u64) pageMem) return false;
    u16 t = (u64) addr / sizeof(SlabNode);
    u8 i = t / 8;
    u8 j = t % 8;
    if (bitMap[i] & (1 << j)) {
        bitMap[i] &= (~(1 << j));
        return true;
    }
    return false;
}

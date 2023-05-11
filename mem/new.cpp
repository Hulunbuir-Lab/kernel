#include <new>
#include <mem.h>

void* operator new(std::size_t count) {
    if (count <= 0) count = 4096;
    if (count <= 4096 - sizeof(SlabArea)) return defaultSlabAllocator.Malloc(count);
    u32 t = 4096;
    for (u8 i = 0; i < 12; ++i, t <<= 1) {
        if (t >= count) return pageAllocator.AllocPageMem(i);
    }
    return pageAllocator.AllocPageMem(11);
}

void operator delete(void* ptr) throw() {
    if (!defaultSlabAllocator.Free(ptr)) {
        pageAllocator.FreePageMem(ptr);
    }
}

void* operator new[](std::size_t count) {
    if (count <= 0) count = 4096;
    if (count < 4096) return defaultSlabAllocator.Malloc(count);
    u32 t = 4096;
    for (u8 i = 0; i < 12; ++i, t <<= 1) {
        if (t >= count) return pageAllocator.AllocPageMem(i);
    }
    return pageAllocator.AllocPageMem(11);
}

void operator delete[](void* ptr) throw() {
    if (!defaultSlabAllocator.Free(ptr)) {
        pageAllocator.FreePageMem(ptr);
    }
}

#include <mem.h>

PageAllocator pageAllocator;
MemSpace kernelSpace(0, vaddrEnd);
DirectZone kernelDirectZone(0, separator - 1);
DynamicZone kernelDynamicZone(separator, vaddrEnd);
MemSpace *currentMemSpace;

void Zone::OnPageFault(u64 vaddr) {

}

MemSpace::MemSpace(u64 vStart, u64 vEnd) :VStart(vStart), VEnd(vEnd) {}

bool MemSpace::AddZone(Zone* t)
{

}

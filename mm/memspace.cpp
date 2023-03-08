#include <mem.h>

void Zone::OnPageFault() {

}

MemSpace::MemSpace(u64 vStart, u64 vEnd) :VStart(vStart), VEnd(vEnd) {}

#include <util.h>

u64 getCPUCFG(u64 input) {
    u64 rel;
    __asm__(
        "cpucfg %0, %1"
        :"=r"(rel)
        :"r"(input)
        :
    );
    return rel;
}

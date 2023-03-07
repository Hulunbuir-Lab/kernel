#include <util.h>

template<typename T>
void kSwap(T& a, T& b)
{
    T c;
    c = a;
    a = b;
    b = c;
}

u32 kStrlen(const char* s) {
    u32 c = 0;
    for (const char *pt = s; *pt != '\0'; ++pt, ++c);
    return c;
}

u64 getCPUCfG(u64 input) {
    u64 rel;
    __asm__(
        "cpucfg %0, %1"
        :"=r"(rel)
        :"r"(input)
        :
    );
    return rel;
}

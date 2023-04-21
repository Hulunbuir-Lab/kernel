#include <util.h>
#include <larchintrin.h>

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

u64 kCsrrd(u8 csr)
{
    //return __builtin_loongarch_csrrd_d(csr);
    return 0;
}

void kCsrwr(u64 val, u8 csr)
{
    //__builtin_loongarch_csrwr_d(val, csr);
}

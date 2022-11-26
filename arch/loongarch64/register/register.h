#ifndef REGISTER_H_INCLUDED
#define REGISTER_H_INCLUDED

#include <common/variable/variable.h>

static inline u64 csrrd(u16 csrNum) {
    u64 value;
    __asm__ volatile (
        "csrrd %0, %1"
        : "=r"(value)
        : "i"(csrNum)
    );
    return value;
}

static inline void csrwr(u64 value, u16 csrNum) {
    __asm__ volatile (
        "csrwr %0, %1"
        :
        : "r"(value), "i"(csrNum)
    );
}

#endif // REGISTER_H_INCLUDED

#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

using u64 = unsigned long long;
using u32 = unsigned int;
using u16 = unsigned short;
using u8 = unsigned char;

template <class T>
void kSwap(T & a, T & b);

u32 kStrlen(const char* s);

inline u64 kCsrrd(u16 csrNum) {
    u64 value;
    __asm__ volatile (
        "csrrd %0, %1"
        : "=r"(value)
        : "i"(csrNum)
    );
    return value;
}

inline void kCsrwr(u64 value, u16 csrNum) {
    __asm__ volatile (
        "csrwr %0, %1"
        :
        : "r"(value), "i"(csrNum)
    );
}

template <class T>
inline T kMax(T a, T b) {
    return a > b ? a : b;
}

template <class T>
inline T kMin(T a, T b) {
    return a < b ? a : b;
}

u64 getCPUCFG(u64 input);

template <class T>
inline T getPartical(T val, u8 high, u8 low) {
    return (val >> low) % (1 << (high - low + 1));
}

#endif // UTIL_H_INCLUDED

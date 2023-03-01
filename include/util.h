#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

using u64 = unsigned long long;
using u32 = unsigned int;
using u16 = unsigned short;
using u8 = unsigned char;

template <class T>
void _kSwap(T & a, T & b);

u32 _kStrlen(const char* s);

inline u64 _kCsrrd(u16 csrNum) {
    u64 value;
    __asm__ volatile (
        "csrrd %0, %1"
        : "=r"(value)
        : "i"(csrNum)
    );
    return value;
}

inline void _kCsrwr(u64 value, u16 csrNum) {
    __asm__ volatile (
        "csrwr %0, %1"
        :
        : "r"(value), "i"(csrNum)
    );
}



#endif // UTIL_H_INCLUDED

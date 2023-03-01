#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

using _kU64 = unsigned long long;
using _kU32 = unsigned int;
using _kU16 = unsigned short;
using _kU8 = unsigned char;

template <class T>
void _kSwap(T & a, T & b);

_kU32 _kStrlen(const char* s);

inline _kU64 _kCsrrd(_kU16 csrNum) {
    _kU64 value;
    __asm__ volatile (
        "csrrd %0, %1"
        : "=r"(value)
        : "i"(csrNum)
    );
    return value;
}

inline void _kCsrwr(_kU64 value, _kU16 csrNum) {
    __asm__ volatile (
        "csrwr %0, %1"
        :
        : "r"(value), "i"(csrNum)
    );
}



#endif // UTIL_H_INCLUDED

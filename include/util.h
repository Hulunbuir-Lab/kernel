#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <compare>

using u64 = unsigned long long;
using u32 = unsigned int;
using u16 = unsigned short;
using u8 = unsigned char;

template <class T>
void kSwap(T & a, T & b);

u32 kStrlen(const char* s);

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

inline u64 upAlign(u64 addr, u8 sizeBit) {
    return addr + (1ull << sizeBit) & (~((1ull << sizeBit) - 1));
}

class Mutex {
    u32 locked = 0;
public:
    void Lock();
    void Unlock();
};

class Jiffies{
    u64 val;
    static u64 sysJiffies;
    friend class Exception;
public:
    friend auto operator <=> (const Jiffies & a, const Jiffies & b) {
        Jiffies now = GetJiffies();
        return (a.val - now.val) <=> (b.val - now.val);
    }
    friend auto operator + (const Jiffies & a, const Jiffies & b) {
        return Jiffies(a.val + b.val);
    }
    operator unsigned int() {
        return val;
    }
    static Jiffies GetJiffies() {
        return Jiffies(sysJiffies);
    }
    Jiffies(u64 val = 0):val(val){}
};

#endif // UTIL_H_INCLUDED

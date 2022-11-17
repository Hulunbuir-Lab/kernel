using u64 = unsigned long long;
using u32 = unsigned int;
using u16 = unsigned short;
using u8 = unsigned char;

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

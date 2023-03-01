#include <util.h>

template<typename T>
void _kSwap(T& a, T& b)
{
    T c;
    c = a;
    a = b;
    b = c;
}

_kU32 _kStrlen(const char* s) {
    _kU32 c = 0;
    for (const char *pt = s; *pt != '\0'; ++pt, ++c);
    return c;
}

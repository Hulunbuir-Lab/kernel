#include "string.h"

u32 strlen(const char* s) {
    u64 c = 0;
    for (const char *pt = s; *pt != '\0'; ++pt, ++c);
    return c;
}


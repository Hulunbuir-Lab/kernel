#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED

#include <util.h>

int             memcmp(const void*, const void*, u32);
void*           memmove(void*, const void*, u32);
void*           memset(void*, int, u32);
char*           safestrcpy(char*, const char*, int);
int             strlen(const char*);
int             strncmp(const char*, const char*, u32);
char*           strncpy(char*, const char*, int);
void* memcpy(void *dst, const void *src, u32 n);
char* strchr(const char* str, char ch);
char toupper(char c);
int islower(int c);

#endif // STRING_H_INCLUDED

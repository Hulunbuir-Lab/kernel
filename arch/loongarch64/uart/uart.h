#ifndef UART_H_INCLUDED
#define UART_H_INCLUDED

#include <register/register.h>
#include <common/variable/variable.h>
#include <common/string/string.h>

class UARTManager {
    u8 *const baseAddress = (u8 *) 0x1fe001e0UL;
public:
    UARTManager();
    void UARTPutchar(char c);
    template <class T>
    void UARTPut(T a);
    void UARTPut(const char *x);
};

#endif // UART_H_INCLUDED

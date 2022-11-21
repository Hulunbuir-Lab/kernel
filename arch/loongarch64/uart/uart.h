#ifndef UART_H_INCLUDED
#define UART_H_INCLUDED

#include <register/register.h>
#include <common/common.h>

class UARTManager {
    u8 *const baseAddress = (u8 *) 0x1fe001e0UL;

public:
    UARTManager();
    void UARTPutchar(u8 c);
};

#endif // UART_H_INCLUDED

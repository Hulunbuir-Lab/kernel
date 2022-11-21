#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED

#include <uart/uart.h>

class SystemManager {
    UARTManager mUARTManager;
public:
    SystemManager();
    void Run();
};

#endif // SYSTEM_H_INCLUDED

#include "system.h"

SystemManager::SystemManager() {}

void SystemManager::Run() {
    mUARTManager.UARTPutchar('H');
    mUARTManager.UARTPutchar('o');
    mUARTManager.UARTPutchar('l');
    mUARTManager.UARTPutchar('a');
    mUARTManager.UARTPutchar('!');
}

#include "system.h"

SystemManager::SystemManager() {}

void SystemManager::Run() {
    mUARTManager.UARTPut("Hello World!");
}

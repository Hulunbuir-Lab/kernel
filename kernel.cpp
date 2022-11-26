#include <system/system.h>

extern "C" void kernelMain() {
    SystemManager os;
    os.Run();
    while (1);
}

extern "C" void __stack_chk_fail(void) {}

extern "C" void __stack_chk_guard(void) {}

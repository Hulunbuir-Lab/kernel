#include <system.h>

void invokeInit() {
    using func_ptr = void (*) (void);
    extern char __init_array_start, __init_array_end;
    for (func_ptr *func = (func_ptr *) &__init_array_start; func != (func_ptr *) &__init_array_end; ++func) {
        if (func) (*func)();
    }
}

extern "C" void KernelMain(KernelInfo info) {
    invokeInit();
    System os(info);
    os.Init();
    os.Run();
}


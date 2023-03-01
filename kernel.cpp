#include <system.h>

extern "C" void kernelMain(KernelInfo info) {
    SystemManager os(info);
    os.Run();
    while (1);
}


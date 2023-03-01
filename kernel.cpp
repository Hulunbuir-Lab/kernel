#include <system.h>

extern "C" void kernelMain(KernelInfo info) {
    System os(info);
    os.Run();
    while (1);
}


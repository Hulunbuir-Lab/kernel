#include <system.h>

extern "C" void KernelMain(KernelInfo info) {
    System os;
    os.InitMem(info);
    os.Run();
}


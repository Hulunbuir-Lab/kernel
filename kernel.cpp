#include <system/system.h>

extern "C" void kernelMain() {
    SystemManager os;
    os.Run();
    while (1);
}

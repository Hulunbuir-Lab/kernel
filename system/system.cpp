#include <system.h>

void System::InitPage()
{
    pageAllocator.SetPageInfo(upAlign((u64)(&KernelEnd), PAGE_SIZE_BIT + PAGE_GROUP_SIZE_BIT), separator);

    pageAllocator.AddArea(separator, 0xFFFFFFF, 0);
    pageAllocator.AddArea(0x10000000, 0x7FFFFFFF, 1);
    pageAllocator.AddArea(0x80000000, 0xFFFFFFFF, 0);
}

void System::InitMem()
{
    __csrwr_d(0x13E4D52C, 0x1C);
    __csrwr_d(0x267, 0x1D);

    kernelSpace.MMUService.setPageTable(pageAllocator.AllocMem(0));
    kernelSpace.AddZone(&kernelDirectZone);
    kernelSpace.AddZone(&kernelDynamicZone);
}


void System::Init()
{
    SysException.IntOff();
    InitPage();
    InitMem();
    SysException.IntOn();
    SysClock.ClockOn();
}

void System::Run() {
    while (1);
}

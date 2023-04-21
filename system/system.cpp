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
    int pgc = 0x13E4D52C;
    kCsrwr(pgc, 0x1C);
    pgc = 0x267;
    kCsrwr(pgc, 0x1D);

    kernelSpace.MMUService.setPageTable(pageAllocator.AllocMem(0));
    kernelSpace.AddZone(&kernelDirectZone);
    kernelSpace.AddZone(&kernelDynamicZone);
}


void System::Init()
{
    InitPage();
    InitMem();
}

void System::Run() {
    while (1);
}

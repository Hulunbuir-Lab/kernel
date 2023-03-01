#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED

#include <uart.h>

struct KernelInfo {
  void *xdspAddress;
  void *memMapAddress;
  u64 memMapSize;
  u64 memMapDescriptorSize;
};

enum efiMemType: u64{
  EfiReservedMemoryType,
  EfiLoaderCode,
  EfiLoaderData,
  EfiBootServicesCode,
  EfiBootServicesData,
  EfiRuntimeServicesCode,
  EfiRuntimeServicesData,
  EfiConventionalMemory,
  EfiUnusableMemory,
  EfiACPIReclaimMemory,
  EfiACPIMemoryNVS,
  EfiMemoryMappedIO,
  EfiMemoryMappedIOPortSpace,
  EfiPalCode,
  EfiPersistentMemory,
  EfiMaxMemoryType
};

struct efiMemDescriptor {
    efiMemType type;
    u64 phyStart;
    u64 virtStart;
    u64 pageNum;
    u64 attr;
};

class System {
    void *acpiBase;
    u64 efiMemMapStart;
    u64 efiMemMapEnd;
    u64 efiMemMapDescriptorSize;

    UART uPut;
public:
    System(KernelInfo & info);
    void Run();
};

#endif // SYSTEM_H_INCLUDED

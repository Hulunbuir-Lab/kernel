#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED

#include <uart.h>

struct KernelInfo {
  void *xdspAddress;
  void *memMapAddress;
  _kU64 memMapSize;
  _kU64 memMapDescriptorSize;
};

enum efiMemType: _kU64{
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
    _kU64 phyStart;
    _kU64 virtStart;
    _kU64 pageNum;
    _kU64 attr;
};

class SystemManager {
    void *acpiBase;
    _kU64 efiMemMapStart;
    _kU64 efiMemMapEnd;
    _kU64 efiMemMapDescriptorSize;

    UARTManager uPut;
public:
    SystemManager(KernelInfo & info);
    void Run();
};

#endif // SYSTEM_H_INCLUDED

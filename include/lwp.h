#ifndef LWP_H
#define LWP_H

#include <mem.h>
#include <exception.h>
#include <larchintrin.h>

const u16 prioRatios[8] = { 100, 200, 300, 400, 500, 600, 700, 800 };

struct ELFHeader {
    u32 MagicNum;
    u8 BitSize;
    u8 Endian;
    u8 HeaderVer;
    u8 OSABI;
    u64 Padding;
    u16 ELFType;
    u16 Architecture;
    u32 ELFVersion;
    u64 ProgramEntryPosition;
    u64 ProgramHeaderTablePosition;
    u64 SectionHeaderTablePosition;
};

class ELFProgram {
public:
    ELFProgram();
};

class Process {
    MemSpace *space;
    TNode<Zone> *text;
    TNode<Zone> *stack;
    u64 reg[30];
    u64 sp;
    u64 pc;
public:
    Process(u8 priority, u8 nice, void* startAddress);
    ~Process();
    u8 Id;
    Process *Next;
    u8 Priority;
    u8 Nice;
    Jiffies Deadline;
    void Resume();
    void Pause();
    MemSpace *GetSpace() {
        return space;
    }
};

class ProcessController {
    u8 headBitMap = 0;
    Process *bfsHead[8];
public:
    ProcessController();
    Process *CurrentProcess;
    void HandleSchedule();
    void StopCurrentProcess();
    void InsertProcess(Process *process);
};

extern ProcessController processController;

#endif

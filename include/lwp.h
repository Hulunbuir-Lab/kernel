#ifndef LWP_H
#define LWP_H

#include <mem.h>
#include <clock.h>
#include <larchintrin.h>

const u16 prioRatios[8] = { 100, 200, 300, 400, 500, 600, 700, 800 };

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
    static void testEntryA();
    static void testEntryB();
    Process *Next;
    u8 Priority;
    u8 Nice;
    RTCVal Deadline;
    void Resume();
    void Pause();
};

class ProcessController {
    u8 headBitMap = 0;
    Process *head[8];
public:
    ProcessController();
    Process *CurrentProcess;
    void HandleSchedule();
    void StopCurrentProcess();
    void InsertProcess(Process *process);
};

extern ProcessController processController;

#endif

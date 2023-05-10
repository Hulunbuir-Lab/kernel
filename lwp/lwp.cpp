#include <lwp.h>

extern u64 ContextReg[30];

void Process::testEntryA() {
    while (1)
    uPut << "A";
}

void Process::testEntryB() {
    while (1)
    uPut << "B";
}

Process::Process(u8 priority, u8 nice, void* startAddress):Priority(priority), Nice(nice){
    space = new MemSpace(0, 0xFFFFFFFFFFFFFFFF);
    text = new TNode<Zone>(DirectZone(0, 0x0FF, (u64) startAddress));
    stack = new TNode<Zone>(Zone(0x100, 0x5FF));
    space->AddZone(text);
    space->AddZone(stack);
    pc = (u64) startAddress;
    sp = 0x600;
}

Process::~Process() {
    delete text;
    delete stack;
    delete space;
}

inline void Process::Resume() {
    for (u64 i = 0; i < 30; ++i) {
        ContextReg[i] = reg[i];
    }
    __csrwr_d(sp, 0x30);
    __csrwr_d(pc, 0x6);
}

inline void Process::Pause() {
    for (u64 i = 0; i < 30; ++i) {
        reg[i] = ContextReg[i];
    }
    sp = __csrrd_d(0x30);
    pc = __csrrd_d(0x6);
}

ProcessController::ProcessController() {

}

void ProcessController::StopCurrentProcess() {
    if (CurrentProcess == nullptr) return;
    CurrentProcess->Pause();
    CurrentProcess->Deadline = RTCVal(prioRatios[CurrentProcess->Nice]) + SysClock.GetRTC();
    CurrentProcess->Next = head[CurrentProcess->Priority];
    head[CurrentProcess->Priority] = CurrentProcess;
    headBitMap &= (1 << (CurrentProcess->Priority));
}

inline void ProcessController::HandleSchedule() {
    if (!(headBitMap & (0x11111111u << (CurrentProcess->Priority)))) return;
    StopCurrentProcess();
    for (u8 i = 7; i >= 0; --i) {
        if (headBitMap & (1 << i)) {
            Process *processToExec = head[i], *from;
            for (Process *pt = head[i]->Next; pt != nullptr; from = pt, pt = pt->Next) {
                if (pt->Deadline < processToExec->Deadline) processToExec = pt;
            }
            CurrentProcess = processToExec;
            if (from != nullptr) from->Next = CurrentProcess->Next;
            else {
                head[i] = CurrentProcess->Next;
                if (head[i] == nullptr) headBitMap &= (~(1 << i));
            }
            CurrentProcess->Resume();
            break;
        }
    }
}

void ProcessController::InsertProcess(Process* process) {
    if (CurrentProcess == nullptr || process->Priority > CurrentProcess->Priority) {
        StopCurrentProcess();
        CurrentProcess = process;
        CurrentProcess->Resume();
        return;
    }
    process->Deadline = RTCVal(prioRatios[process->Nice]) + SysClock.GetRTC();
    process->Next = head[process->Priority];
    head[process->Priority] = process;
}

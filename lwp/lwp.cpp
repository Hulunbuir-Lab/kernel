#include <lwp.h>

extern u64 ContextReg[30];

static u8 t = 0;

static u8 getId() {
    return t++;
}

__attribute__((aligned(4 * 1024)))
static void testEntryA() {
    while (1) {
        __asm__ (
            "syscall 0"
        );
    }
}

__attribute__((aligned(4 * 1024)))
static void testEntryB() {
    while (1) {
        __asm__ (
            "syscall 1"
        );
    }
}

Process::Process(u8 priority, u8 nice, void* startAddress):Priority(priority), Nice(nice){
    Id = getId();
    space = new MemSpace(0, 0xFFFFFFFFFFFFFFFF);
    text = new TNode<Zone>(new DirectZone(0, 0xFFF, (u64) startAddress, ZoneConfig{1, 3, 0, 0, 0, 0}));
    stack = new TNode<Zone>(new Zone(0x1000, 0x5FFF, ZoneConfig{1, 3, 0, 1, 0, 0}));
    space->AddZone(text);
    space->AddZone(stack);

    pc = 0x0;
    sp = 0x6000;
}

Process::~Process() {
    delete text;
    delete stack;
    delete space;
}

void Process::Resume() {
    for (u64 i = 0; i < 30; ++i) {
        ContextReg[i] = reg[i];
    }
    __csrwr_d(sp, 0x30);
    __csrwr_d(pc, 0x6);
    __csrwr_d(Id, 0x18);
    GetSpace()->MMUService.setPGDL();
}

void Process::Pause() {
    for (u64 i = 0; i < 30; ++i) {
        reg[i] = ContextReg[i];
    }
    sp = __csrrd_d(0x30);
    pc = __csrrd_d(0x6);
}

void ProcessController::StopCurrentProcess() {
    if (CurrentProcess == nullptr) return;
    CurrentProcess->Pause();
    CurrentProcess->Deadline = Jiffies(prioRatios[CurrentProcess->Nice]) + Jiffies::GetJiffies();
    CurrentProcess->Next = bfsHead[CurrentProcess->Priority];
    bfsHead[CurrentProcess->Priority] = CurrentProcess;
    headBitMap |= (1 << (CurrentProcess->Priority));
    CurrentProcess = nullptr;
}

void ProcessController::HandleSchedule() {
    if (!(headBitMap & (0x11111111u << (CurrentProcess->Priority)))) return;
    StopCurrentProcess();
    for (u8 i = 7; i >= 0; --i) {
        if (headBitMap & (1 << i)) {
            Process *processToExec = bfsHead[i], *processToExecFrom = nullptr;
            for (Process *pt = bfsHead[i], *from = nullptr; pt != nullptr; from = pt, pt = pt->Next) {
                if (pt->Deadline < processToExec->Deadline) {
                    processToExecFrom = from;
                    processToExec = pt;
                }
            }
            CurrentProcess = processToExec;
            if (processToExecFrom != nullptr) {
                processToExecFrom->Next = CurrentProcess->Next;
            }
            else {
                bfsHead[i] = CurrentProcess->Next;
                if (bfsHead[i] == nullptr) headBitMap &= (~(1 << i));
            }
            CurrentProcess->Next = nullptr;
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
    process->Deadline = Jiffies(prioRatios[process->Nice]) + Jiffies::GetJiffies();
    process->Next = bfsHead[process->Priority];
    bfsHead[process->Priority] = process;
    headBitMap |= (1 << process->Priority);
}

ProcessController::ProcessController() {
    InsertProcess(new Process(7, 0, (void *) testEntryA));
    InsertProcess(new Process(7, 0, (void *) testEntryB));
}

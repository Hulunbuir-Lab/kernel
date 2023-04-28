#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED

#include <uart.h>
#include <mem.h>
#include <util.h>
#include <exception.h>
#include <larchintrin.h>
#include <clock.h>

class System {
    void InitPage();
    void InitMem();
public:
    void Init();
    void Run();
};

#endif // SYSTEM_H_INCLUDED

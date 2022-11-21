#include "uart.h"

UARTManager::UARTManager() {
    // disable interrupts
    *(baseAddress + 1) = 0;
    // special mode to set baud rate
    *(baseAddress + 3) = 0x80;
    // LSB for baud rate of 38.4K
    *baseAddress = 0x03;
    // MSB for baud rate of 38.4K
    *(baseAddress + 1) = 0;
    // leave set-baud mode and set word length
    // to 8 bits, no parity
    *(baseAddress + 3) = 0x03;
    // reset and enable FIFOs
    *(baseAddress + 2) = 0x07;
    // enable transmit and receive interrupts.
    *(baseAddress + 1) = 0x03;
}

// alternate version of uartputc() that doesn't
// use interrupts, for use by kernel printf() and
// to echo characters. it spins waiting for the uart's
// output register to be empty.
void UARTManager::UARTPutchar(u8 c) {

    // wait for Transmit Holding Empty to be set in LSR
    while((*(baseAddress + 5) & (1 << 5)) == 0);
    *baseAddress = c;
}

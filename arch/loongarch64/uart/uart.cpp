#include "uart.h"

#define RHR 0                 // receive holding register (for input bytes)
#define THR 0                 // transmit holding register (for output bytes)
#define IER 1                 // interrupt enable register
#define IER_RX_ENABLE (1<<0)
#define IER_TX_ENABLE (1<<1)
#define FCR 2                 // FIFO control register
#define FCR_FIFO_ENABLE (1<<0)
#define FCR_FIFO_CLEAR (3<<1) // clear the content of the two FIFOs
#define ISR 2                 // interrupt status register
#define LCR 3                 // line control register
#define LCR_EIGHT_BITS (3<<0)
#define LCR_BAUD_LATCH (1<<7) // special mode to set baud rate
#define LSR 5                 // line status register
#define LSR_RX_READY (1<<0)   // input is waiting to be read from RHR
#define LSR_TX_IDLE (1<<5)    // THR can accept another character to send

// the transmit output buffer.
#define UART_TX_BUF_SIZE 32

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
void UARTManager::uartPutchar(u8 c) {

    // wait for Transmit Holding Empty to be set in LSR
    while((*(baseAddress + 5) & LSR_TX_IDLE) == 0);
    *baseAddress = c;
}

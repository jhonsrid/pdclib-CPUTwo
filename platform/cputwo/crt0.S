; CPUTwo C runtime startup
; Sets up stack pointer and calls main()
; Return value from main is written to UART then halts.

.text
.globl _start
_start:
    lui  sp, 1008       ; SP = 0x03F00000 (top of RAM, below MMIO)
    ori  sp, sp, 0
    jmp  lr, main       ; call main()
    ; main returned, exit code in r0
    halt

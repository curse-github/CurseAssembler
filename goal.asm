section .text code execute read
_main:
    PUSH rBP
    MOV rBP rSP
    SUB rSP 0x20
    MOV eCX code
    JMP _exit

section .data read write
code:
int64 0x0f
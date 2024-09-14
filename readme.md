```c++
/*
Possible instruction arguments
    eCX
    rCX
    // indirect or indirect + constant
    [eCX]
    [eCX-300]
    [rCX]
    [rCX+300]
    // relative pointers
    [eIP+0]
    [eIP+300]
    [rIP+0]
    [rIP+300]
    // [base+index*scale+displacement]
    [300]
    [eDI*2]
    [eDI*2-300]
    [eCX+eDI*2]
    [eCX+eDI*2+300]
    [rDI*2]
    [rDI*2+300]
    [rCX+rDI*2]
    [rCX+rDI*2-300]
    25
    300
    // variables (needs to be able to handle names similar to registers)
    var
    variable
    i
    // to the code could look like registers at first glance, but might be variable names
    eaxb
    easy
    ripper

registers:
        eIP: -2, rIP: -2
        invalid: -1
        AL: 0, AX: 0, eAX: 0, rAX: 0
        CL: 1, CX: 0, eCX: 0, rCX: 0
        DL: 2, DX: 0, eDX: 0, rDX: 0
        BL: 3, BX: 0, eBX: 0, rBX: 0
        AH: 4, SP: 0, eSP: 0, rSP: 0
        CH: 5, BP: 0, eBP: 0, rBP: 0
        DH: 6, DI: 0, eSI: 0, rSI: 0
        BH: 7, DI: 0, eDI: 0, rDI: 0

numbers:
    150        // decimal, obviously
    0b10010110 // binary
    0d150      // also decimal, but explicitly
    0x96       // hexidecimal

sample file:
    section .text code execute read
    entry _main:
        PUSH rBP
        MOV rBP rSP
        SUB rSP 0x20
        MOV eCX code
        JMP _exit

    section .data read write
    code:
    int64 0x0f
*/
```
#include <fstream>
#include <string>
#include <cstring>
#include <iostream>

#include "globalConstants.h"
#include "intelConstants.h"

const uint32_t exit_code = 0x00000028;
int main(int argc, char *argv[]) {
    std::ofstream outFile(std::string("./", 2) + argv[2] + ".exe", std::ios::binary);

    debugInstructionOutput=true;
    bitMode=64;
    //bitMode=32;

    /*
    ADD(outFile,"eCX","eBX");
    ADD(outFile,"[eCX]","eBX");
    ADD(outFile,"[eCX-300]","eBX");
    ADD(outFile,"[eIP+0]","eBX");
    ADD(outFile,"[eIP+300]","eBX");
    ADD(outFile,"[300]","eBX");
    ADD(outFile,"[eDI*2]","eBX");
    ADD(outFile,"[eDI*2-300]","eBX");
    ADD(outFile,"[eCX+eDI*2]","eBX");
    ADD(outFile,"[eCX+eDI*2+300]","eBX");
    ADD(outFile,"eCX","15001");
    ADD(outFile,"[eCX]","15001");
    ADD(outFile,"[eCX-300]","15001");
    ADD(outFile,"[eIP+0]","15001");
    ADD(outFile,"[eIP+300]","15001");
    ADD(outFile,"[300]","15001");
    ADD(outFile,"[eDI*2]","15001");
    ADD(outFile,"[eDI*2-300]","15001");
    ADD(outFile,"[eCX+eDI*2]","15001");
    ADD(outFile,"[eCX+eDI*2+300]","15001");
    ADD(outFile,"eDI","25");
    ADD(outFile,"eDI","1930");
    ADD(outFile,"eDI","eCX");
    ADD(outFile,"eDI","[eCX]");
    ADD(outFile,"eDI","[eCX-300]");
    ADD(outFile,"eDI","[eIP+0]");
    ADD(outFile,"eDI","[eIP+300]");
    ADD(outFile,"eDI","[300]");
    ADD(outFile,"eDI","[eDI*2]");
    ADD(outFile,"eDI","[eDI*2-300]");
    ADD(outFile,"eDI","[eCX+eDI*2]");
    ADD(outFile,"eDI","[eCX+eDI*2+300]");
    if (bitMode==64) {
        ADD(outFile,"[rCX]","eBX");
        ADD(outFile,"[rCX+300]","eBX");
        ADD(outFile,"[rIP+0]","eBX");
        ADD(outFile,"[rIP+300]","eBX");
        ADD(outFile,"[rDI*2]","eBX");
        ADD(outFile,"[rDI*2+300]","eBX");
        ADD(outFile,"[rCX+rDI*2]","eBX");
        ADD(outFile,"[rCX+rDI*2-300]","eBX");
        ADD(outFile,"rCX","15001");
        ADD(outFile,"[rCX]","15001");
        ADD(outFile,"[rCX+300]","15001");
        ADD(outFile,"[rIP+0]","15001");
        ADD(outFile,"[rIP+300]","15001");
        ADD(outFile,"[rDI*2]","15001");
        ADD(outFile,"[rDI*2+300]","15001");
        ADD(outFile,"[rCX+rDI*2]","15001");
        ADD(outFile,"[rCX+rDI*2-300]","15001");
        ADD(outFile,"eDI","[rCX]");
        ADD(outFile,"eDI","[rCX+300]");
        ADD(outFile,"eDI","[rIP+0]");
        ADD(outFile,"eDI","[rIP+300]");
        ADD(outFile,"eDI","[rDI*2]");
        ADD(outFile,"eDI","[rDI*2+300]");
        ADD(outFile,"eDI","[rCX+rDI*2]");
        ADD(outFile,"eDI","[rCX+rDI*2-300]");
        ADD(outFile,"rAX","25");
        ADD(outFile,"rAX","1930");
        ADD(outFile,"rAX","rCX");
        ADD(outFile,"rAX","[eCX]");
        ADD(outFile,"rAX","[eCX-300]");
        ADD(outFile,"rAX","[rCX]");
        ADD(outFile,"rAX","[rCX+300]");
        ADD(outFile,"rAX","[eIP+0]");
        ADD(outFile,"rAX","[eIP+300]");
        ADD(outFile,"rAX","[rIP+0]");
        ADD(outFile,"rAX","[rIP+300]");
        ADD(outFile,"rAX","[300]");
        ADD(outFile,"rAX","[eDI*2]");
        ADD(outFile,"rAX","[eDI*2-300]");
        ADD(outFile,"rAX","[eCX+eDI*2]");
        ADD(outFile,"rAX","[eCX+eDI*2+300]");
        ADD(outFile,"rAX","[rDI*2]");
        ADD(outFile,"rAX","[rDI*2+300]");
        ADD(outFile,"rAX","[rCX+rDI*2]");
        ADD(outFile,"rAX","[rCX+rDI*2-300]");
    }
    
    DEC(outFile,"eCX");
    DEC(outFile,"[eCX]");
    DEC(outFile,"[eCX-300]");
    DEC(outFile,"[eIP+0]");
    DEC(outFile,"[eIP+300]");
    DEC(outFile,"[300]");
    DEC(outFile,"[eDI*2]");
    DEC(outFile,"[eDI*2-300]");
    DEC(outFile,"[eCX+eDI*2]");
    DEC(outFile,"[eCX+eDI*2+300]");
    if (bitMode==64) {
        DEC(outFile,"rCX");
        DEC(outFile,"[rCX]");
        DEC(outFile,"[rCX+300]");
        DEC(outFile,"[rIP+0]");
        DEC(outFile,"[rIP+300]");
        DEC(outFile,"[rDI*2]");
        DEC(outFile,"[rDI*2+300]");
        DEC(outFile,"[rCX+rDI*2]");
        DEC(outFile,"[rCX+rDI*2-300]");
    }
    
    INC(outFile,"eCX");
    INC(outFile,"[eCX]");
    INC(outFile,"[eCX-300]");
    INC(outFile,"[eIP+0]");
    INC(outFile,"[eIP+300]");
    INC(outFile,"[300]");
    INC(outFile,"[eDI*2]");
    INC(outFile,"[eDI*2-300]");
    INC(outFile,"[eCX+eDI*2]");
    INC(outFile,"[eCX+eDI*2+300]");
    if (bitMode==64) {
        INC(outFile,"rCX");
        INC(outFile,"[rCX]");
        INC(outFile,"[rCX+300]");
        INC(outFile,"[rIP+0]");
        INC(outFile,"[rIP+300]");
        INC(outFile,"[rDI*2]");
        INC(outFile,"[rDI*2+300]");
        INC(outFile,"[rCX+rDI*2]");
        INC(outFile,"[rCX+rDI*2-300]");
    }

    //CALL(outFile,"15");
    //CALL(outFile,"15000");
    CALL(outFile,"[eCX]");
    CALL(outFile,"[eCX-300]");
    CALL(outFile,"[eIP+0]");
    CALL(outFile,"[eIP+300]");
    CALL(outFile,"[300]");
    CALL(outFile,"[eDI*2]");
    CALL(outFile,"[eDI*2-300]");
    CALL(outFile,"[eCX+eDI*2]");
    CALL(outFile,"[eCX+eDI*2+300]");
    if (bitMode==64) {
        CALL(outFile,"[rCX]");
        CALL(outFile,"[rCX+300]");
        CALL(outFile,"[rIP+0]");
        CALL(outFile,"[rIP+300]");
        CALL(outFile,"[rDI*2]");
        CALL(outFile,"[rDI*2+300]");
        CALL(outFile,"[rCX+rDI*2]");
        CALL(outFile,"[rCX+rDI*2-300]");
    }

    //JMP(outFile,"15");
    //JMP(outFile,"15000");
    JMP(outFile,"[eCX]");
    JMP(outFile,"[eCX-300]");
    JMP(outFile,"[eIP+0]");
    JMP(outFile,"[eIP+300]");
    JMP(outFile,"[300]");
    JMP(outFile,"[eDI*2]");
    JMP(outFile,"[eDI*2-300]");
    JMP(outFile,"[eCX+eDI*2]");
    JMP(outFile,"[eCX+eDI*2+300]");
    if (bitMode==64) {
        JMP(outFile,"[rCX]");
        JMP(outFile,"[rCX+300]");
        JMP(outFile,"[rIP+0]");
        JMP(outFile,"[rIP+300]");
        JMP(outFile,"[rDI*2]");
        JMP(outFile,"[rDI*2+300]");
        JMP(outFile,"[rCX+rDI*2]");
        JMP(outFile,"[rCX+rDI*2-300]");
    }

    PUSH(outFile,"15");
    PUSH(outFile,"15000");
    if (bitMode==32) PUSH(outFile,"eCX");
    PUSH(outFile,"[eCX]");
    PUSH(outFile,"[eCX-300]");
    PUSH(outFile,"[eIP+0]");
    PUSH(outFile,"[eIP+300]");
    PUSH(outFile,"[300]");
    PUSH(outFile,"[eDI*2]");
    PUSH(outFile,"[eDI*2-300]");
    PUSH(outFile,"[eCX+eDI*2]");
    PUSH(outFile,"[eCX+eDI*2+300]");
    if (bitMode==64) {
        PUSH(outFile,"rCX");
        PUSH(outFile,"[rCX]");
        PUSH(outFile,"[rCX+300]");
        PUSH(outFile,"[rIP+0]");
        PUSH(outFile,"[rIP+300]");
        PUSH(outFile,"[rDI*2]");
        PUSH(outFile,"[rDI*2+300]");
        PUSH(outFile,"[rCX+rDI*2]");
        PUSH(outFile,"[rCX+rDI*2-300]");
    }

    if (bitMode==32) POP(outFile,"eCX");
    POP(outFile,"[eCX]");
    POP(outFile,"[eCX-300]");
    POP(outFile,"[eIP+0]");
    POP(outFile,"[eIP+300]");
    POP(outFile,"[300]");
    POP(outFile,"[eDI*2]");
    POP(outFile,"[eDI*2-300]");
    POP(outFile,"[eCX+eDI*2]");
    POP(outFile,"[eCX+eDI*2+300]");
    if (bitMode==64) {
        POP(outFile,"rCX");
        POP(outFile,"[rCX]");
        POP(outFile,"[rCX+300]");
        POP(outFile,"[rIP+0]");
        POP(outFile,"[rIP+300]");
        POP(outFile,"[rDI*2]");
        POP(outFile,"[rDI*2+300]");
        POP(outFile,"[rCX+rDI*2]");
        POP(outFile,"[rCX+rDI*2-300]");
    }
    */

    //LEA(outFile,"eCX","eBX");
    //LEA(outFile,"eDI","eCX");
    LEA(outFile,"eDI","[eCX]");
    LEA(outFile,"eDI","[eCX-300]");
    LEA(outFile,"eDI","[eIP+0]");
    LEA(outFile,"eDI","[eIP+300]");
    LEA(outFile,"eDI","[300]");
    LEA(outFile,"eDI","[eDI*2]");
    LEA(outFile,"eDI","[eDI*2-300]");
    LEA(outFile,"eDI","[eCX+eDI*2]");
    LEA(outFile,"eDI","[eCX+eDI*2+300]");
    if (bitMode==64) {
        LEA(outFile,"eDI","[rCX]");
        LEA(outFile,"eDI","[rCX+300]");
        LEA(outFile,"eDI","[rIP+0]");
        LEA(outFile,"eDI","[rIP+300]");
        LEA(outFile,"eDI","[rDI*2]");
        LEA(outFile,"eDI","[rDI*2+300]");
        LEA(outFile,"eDI","[rCX+rDI*2]");
        LEA(outFile,"eDI","[rCX+rDI*2-300]");
        //LEA(outFile,"rAX","rCX");
        LEA(outFile,"rAX","[eCX]");
        LEA(outFile,"rAX","[eCX-300]");
        LEA(outFile,"rAX","[rCX]");
        LEA(outFile,"rAX","[rCX+300]");
        LEA(outFile,"rAX","[eIP+0]");
        LEA(outFile,"rAX","[eIP+300]");
        LEA(outFile,"rAX","[rIP+0]");
        LEA(outFile,"rAX","[rIP+300]");
        LEA(outFile,"rAX","[300]");
        LEA(outFile,"rAX","[eDI*2]");
        LEA(outFile,"rAX","[eDI*2-300]");
        LEA(outFile,"rAX","[eCX+eDI*2]");
        LEA(outFile,"rAX","[eCX+eDI*2+300]");
        LEA(outFile,"rAX","[rDI*2]");
        LEA(outFile,"rAX","[rDI*2+300]");
        LEA(outFile,"rAX","[rCX+rDI*2]");
        LEA(outFile,"rAX","[rCX+rDI*2-300]");
    }
    /*
    NOP(outFile);
    NOP(outFile,"eCX");
    NOP(outFile,"[eCX]");
    NOP(outFile,"[eCX-300]");
    NOP(outFile,"[eIP+0]");
    NOP(outFile,"[eIP+300]");
    NOP(outFile,"[300]");
    NOP(outFile,"[eDI*2]");
    NOP(outFile,"[eDI*2-300]");
    NOP(outFile,"[eCX+eDI*2]");
    NOP(outFile,"[eCX+eDI*2+300]");
    if (bitMode==64) {
        NOP(outFile,"rCX");
        NOP(outFile,"[rCX]");
        NOP(outFile,"[rCX+300]");
        NOP(outFile,"[rIP+0]");
        NOP(outFile,"[rIP+300]");
        NOP(outFile,"[rDI*2]");
        NOP(outFile,"[rDI*2+300]");
        NOP(outFile,"[rCX+rDI*2]");
        NOP(outFile,"[rCX+rDI*2-300]");
    }

    XCHG(outFile,"eAX","eDI");
    XCHG(outFile,"eCX","eBX");
    XCHG(outFile,"[eCX]","eBX");
    XCHG(outFile,"[eCX-300]","eBX");
    XCHG(outFile,"[eIP+0]","eBX");
    XCHG(outFile,"[eIP+300]","eBX");
    XCHG(outFile,"[300]","eBX");
    XCHG(outFile,"[eDI*2]","eBX");
    XCHG(outFile,"[eDI*2-300]","eBX");
    XCHG(outFile,"[eCX+eDI*2]","eBX");
    XCHG(outFile,"[eCX+eDI*2+300]","eBX");
    XCHG(outFile,"eDI","eCX");
    XCHG(outFile,"eDI","[eCX]");
    XCHG(outFile,"eDI","[eCX-300]");
    XCHG(outFile,"eDI","[eIP+0]");
    XCHG(outFile,"eDI","[eIP+300]");
    XCHG(outFile,"eDI","[300]");
    XCHG(outFile,"eDI","[eDI*2]");
    XCHG(outFile,"eDI","[eDI*2-300]");
    XCHG(outFile,"eDI","[eCX+eDI*2]");
    XCHG(outFile,"eDI","[eCX+eDI*2+300]");
    if (bitMode==64) {
        XCHG(outFile,"[rCX]","eBX");
        XCHG(outFile,"[rCX+300]","eBX");
        XCHG(outFile,"[rIP+0]","eBX");
        XCHG(outFile,"[rIP+300]","eBX");
        XCHG(outFile,"[rDI*2]","eBX");
        XCHG(outFile,"[rDI*2+300]","eBX");
        XCHG(outFile,"[rCX+rDI*2]","eBX");
        XCHG(outFile,"[rCX+rDI*2-300]","eBX");
        XCHG(outFile,"eDI","[rCX]");
        XCHG(outFile,"eDI","[rCX+300]");
        XCHG(outFile,"eDI","[rIP+0]");
        XCHG(outFile,"eDI","[rIP+300]");
        XCHG(outFile,"eDI","[rDI*2]");
        XCHG(outFile,"eDI","[rDI*2+300]");
        XCHG(outFile,"eDI","[rCX+rDI*2]");
        XCHG(outFile,"eDI","[rCX+rDI*2-300]");
        XCHG(outFile,"rAX","rCX");
        XCHG(outFile,"rAX","[eCX]");
        XCHG(outFile,"rAX","[eCX-300]");
        XCHG(outFile,"rAX","[rCX]");
        XCHG(outFile,"rAX","[rCX+300]");
        XCHG(outFile,"rAX","[eIP+0]");
        XCHG(outFile,"rAX","[eIP+300]");
        XCHG(outFile,"rAX","[rIP+0]");
        XCHG(outFile,"rAX","[rIP+300]");
        XCHG(outFile,"rAX","[300]");
        XCHG(outFile,"rAX","[eDI*2]");
        XCHG(outFile,"rAX","[eDI*2-300]");
        XCHG(outFile,"rAX","[eCX+eDI*2]");
        XCHG(outFile,"rAX","[eCX+eDI*2+300]");
        XCHG(outFile,"rAX","[rDI*2]");
        XCHG(outFile,"rAX","[rDI*2+300]");
        XCHG(outFile,"rAX","[rCX+rDI*2]");
        XCHG(outFile,"rAX","[rCX+rDI*2-300]");
    }
    
    MOV(outFile,"eCX","eBX");
    MOV(outFile,"[eCX]","eBX");
    MOV(outFile,"[eCX-300]","eBX");
    MOV(outFile,"[eIP+0]","eBX");
    MOV(outFile,"[eIP+300]","eBX");
    MOV(outFile,"[300]","eBX");
    MOV(outFile,"[eDI*2]","eBX");
    MOV(outFile,"[eDI*2-300]","eBX");
    MOV(outFile,"[eCX+eDI*2]","eBX");
    MOV(outFile,"[eCX+eDI*2+300]","eBX");
    MOV(outFile,"eCX","15001");
    MOV(outFile,"[eCX]","15001");
    MOV(outFile,"[eCX-300]","15001");
    MOV(outFile,"[eIP+0]","15001");
    MOV(outFile,"[eIP+300]","15001");
    MOV(outFile,"[300]","15001");
    MOV(outFile,"[eDI*2]","15001");
    MOV(outFile,"[eDI*2-300]","15001");
    MOV(outFile,"[eCX+eDI*2]","15001");
    MOV(outFile,"[eCX+eDI*2+300]","15001");
    MOV(outFile,"eDI","25");
    MOV(outFile,"eDI","1930");
    MOV(outFile,"eDI","eCX");
    MOV(outFile,"eDI","[eCX]");
    MOV(outFile,"eDI","[eCX-300]");
    MOV(outFile,"eDI","[eIP+0]");
    MOV(outFile,"eDI","[eIP+300]");
    MOV(outFile,"eDI","[300]");
    MOV(outFile,"eDI","[eDI*2]");
    MOV(outFile,"eDI","[eDI*2-300]");
    MOV(outFile,"eDI","[eCX+eDI*2]");
    MOV(outFile,"eDI","[eCX+eDI*2+300]");
    if (bitMode==64) {
        MOV(outFile,"[rCX]","eBX");
        MOV(outFile,"[rCX+300]","eBX");
        MOV(outFile,"[rIP+0]","eBX");
        MOV(outFile,"[rIP+300]","eBX");
        MOV(outFile,"[rDI*2]","eBX");
        MOV(outFile,"[rDI*2+300]","eBX");
        MOV(outFile,"[rCX+rDI*2]","eBX");
        MOV(outFile,"[rCX+rDI*2-300]","eBX");
        MOV(outFile,"rCX","15001");
        MOV(outFile,"[rCX]","15001");
        MOV(outFile,"[rCX+300]","15001");
        MOV(outFile,"[rIP+0]","15001");
        MOV(outFile,"[rIP+300]","15001");
        MOV(outFile,"[rDI*2]","15001");
        MOV(outFile,"[rDI*2+300]","15001");
        MOV(outFile,"[rCX+rDI*2]","15001");
        MOV(outFile,"[rCX+rDI*2-300]","15001");
        MOV(outFile,"eDI","[rCX]");
        MOV(outFile,"eDI","[rCX+300]");
        MOV(outFile,"eDI","[rIP+0]");
        MOV(outFile,"eDI","[rIP+300]");
        MOV(outFile,"eDI","[rDI*2]");
        MOV(outFile,"eDI","[rDI*2+300]");
        MOV(outFile,"eDI","[rCX+rDI*2]");
        MOV(outFile,"eDI","[rCX+rDI*2-300]");
        MOV(outFile,"rAX","25");
        MOV(outFile,"rAX","1930");
        MOV(outFile,"rAX","rCX");
        MOV(outFile,"rAX","[eCX]");
        MOV(outFile,"rAX","[eCX-300]");
        MOV(outFile,"rAX","[rCX]");
        MOV(outFile,"rAX","[rCX+300]");
        MOV(outFile,"rAX","[eIP+0]");
        MOV(outFile,"rAX","[eIP+300]");
        MOV(outFile,"rAX","[rIP+0]");
        MOV(outFile,"rAX","[rIP+300]");
        MOV(outFile,"rAX","[300]");
        MOV(outFile,"rAX","[eDI*2]");
        MOV(outFile,"rAX","[eDI*2-300]");
        MOV(outFile,"rAX","[eCX+eDI*2]");
        MOV(outFile,"rAX","[eCX+eDI*2+300]");
        MOV(outFile,"rAX","[rDI*2]");
        MOV(outFile,"rAX","[rDI*2+300]");
        MOV(outFile,"rAX","[rCX+rDI*2]");
        MOV(outFile,"rAX","[rCX+rDI*2-300]");
    }

    RET(outFile);
    RET(outFile,"170");
    RETF(outFile);
    RETF(outFile,"170");
    INT3(outFile);
    INT(outFile,"170");
    INT(outFile,"3");
    HLT(outFile);

    TEST(outFile,"eCX","15001");
    TEST(outFile,"[eCX]","15001");
    TEST(outFile,"[eCX-300]","15001");
    TEST(outFile,"[eIP+0]","15001");
    TEST(outFile,"[eIP+300]","15001");
    TEST(outFile,"[300]","15001");
    TEST(outFile,"[eDI*2]","15001");
    TEST(outFile,"[eDI*2-300]","15001");
    TEST(outFile,"[eCX+eDI*2]","15001");
    TEST(outFile,"[eCX+eDI*2+300]","15001");
    if (bitMode==64) {
        TEST(outFile,"rCX","15001");
        TEST(outFile,"[rCX]","15001");
        TEST(outFile,"[rCX+300]","15001");
        TEST(outFile,"[rIP+0]","15001");
        TEST(outFile,"[rIP+300]","15001");
        TEST(outFile,"[rDI*2]","15001");
        TEST(outFile,"[rDI*2+300]","15001");
        TEST(outFile,"[rCX+rDI*2]","15001");
        TEST(outFile,"[rCX+rDI*2-300]","15001");
    }

    NOT(outFile,"eCX");
    NOT(outFile,"[eCX]");
    NOT(outFile,"[eCX-300]");
    NOT(outFile,"[eIP+0]");
    NOT(outFile,"[eIP+300]");
    NOT(outFile,"[300]");
    NOT(outFile,"[eDI*2]");
    NOT(outFile,"[eDI*2-300]");
    NOT(outFile,"[eCX+eDI*2]");
    NOT(outFile,"[eCX+eDI*2+300]");
    if (bitMode==64) {
        NOT(outFile,"rCX");
        NOT(outFile,"[rCX]");
        NOT(outFile,"[rCX+300]");
        NOT(outFile,"[rIP+0]");
        NOT(outFile,"[rIP+300]");
        NOT(outFile,"[rDI*2]");
        NOT(outFile,"[rDI*2+300]");
        NOT(outFile,"[rCX+rDI*2]");
        NOT(outFile,"[rCX+rDI*2-300]");
    }

    NEG(outFile,"eCX");
    NEG(outFile,"[eCX]");
    NEG(outFile,"[eCX-300]");
    NEG(outFile,"[eIP+0]");
    NEG(outFile,"[eIP+300]");
    NEG(outFile,"[300]");
    NEG(outFile,"[eDI*2]");
    NEG(outFile,"[eDI*2-300]");
    NEG(outFile,"[eCX+eDI*2]");
    NEG(outFile,"[eCX+eDI*2+300]");
    if (bitMode==64) {
        NEG(outFile,"rCX");
        NEG(outFile,"[rCX]");
        NEG(outFile,"[rCX+300]");
        NEG(outFile,"[rIP+0]");
        NEG(outFile,"[rIP+300]");
        NEG(outFile,"[rDI*2]");
        NEG(outFile,"[rDI*2+300]");
        NEG(outFile,"[rCX+rDI*2]");
        NEG(outFile,"[rCX+rDI*2-300]");
    }

    MUL(outFile,"eCX");
    MUL(outFile,"[eCX]");
    MUL(outFile,"[eCX-300]");
    MUL(outFile,"[eIP+0]");
    MUL(outFile,"[eIP+300]");
    MUL(outFile,"[300]");
    MUL(outFile,"[eDI*2]");
    MUL(outFile,"[eDI*2-300]");
    MUL(outFile,"[eCX+eDI*2]");
    MUL(outFile,"[eCX+eDI*2+300]");
    if (bitMode==64) {
        MUL(outFile,"rCX");
        MUL(outFile,"[rCX]");
        MUL(outFile,"[rCX+300]");
        MUL(outFile,"[rIP+0]");
        MUL(outFile,"[rIP+300]");
        MUL(outFile,"[rDI*2]");
        MUL(outFile,"[rDI*2+300]");
        MUL(outFile,"[rCX+rDI*2]");
        MUL(outFile,"[rCX+rDI*2-300]");
    }

    UDIV(outFile,"eCX");
    UDIV(outFile,"[eCX]");
    UDIV(outFile,"[eCX-300]");
    UDIV(outFile,"[eIP+0]");
    UDIV(outFile,"[eIP+300]");
    UDIV(outFile,"[300]");
    UDIV(outFile,"[eDI*2]");
    UDIV(outFile,"[eDI*2-300]");
    UDIV(outFile,"[eCX+eDI*2]");
    UDIV(outFile,"[eCX+eDI*2+300]");
    if (bitMode==64) {
        UDIV(outFile,"rCX");
        UDIV(outFile,"[rCX]");
        UDIV(outFile,"[rCX+300]");
        UDIV(outFile,"[rIP+0]");
        UDIV(outFile,"[rIP+300]");
        UDIV(outFile,"[rDI*2]");
        UDIV(outFile,"[rDI*2+300]");
        UDIV(outFile,"[rCX+rDI*2]");
        UDIV(outFile,"[rCX+rDI*2-300]");
    }
    
    CLC(outFile);
    STC(outFile);
    CLI(outFile);
    STI(outFile);
    CLD(outFile);
    STD(outFile);
    */

    outFile.close();
    return 0;
}
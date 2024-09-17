#include <fstream>
#include <string>
#include <cstring>
#include <iostream>

#include "globalConstants.h"

#include "intelConstants.h"

const uint32_t exit_code = 0x00000028;
int main(int argc, char *argv[]) {
    if (argc < 3) return 1;
    //debugInstructionOutput=true;
    if (strcmp(argv[1],"Linux")==0) {
        std::ofstream outFile(std::string("./", 2) + argv[2] + ".out", std::ios::binary);
        // ELF
        elf_encoding = ELF_ENCODING_LSB;
        elf_osAbi = ELF_OS_LINUX;
        elf_type = ELF_TYPE_EXE;
        elf_machine = ELF_MACHINE_AMD64;

        Elf64Handler elfHandler;
        Elf64SegmentHandler *textSeg = elfHandler.addSeg(ELF_SEGMENT_TYPE_LOAD, ELF_SEGMENT_FLAG_READ | ELF_SEGMENT_FLAG_EXECUTE);
        Elf64SegmentHandler *dataSeg = elfHandler.addSeg(ELF_SEGMENT_TYPE_LOAD, ELF_SEGMENT_FLAG_READ);
        
        textSeg->defineLabel("_main");
        LEA(textSeg,"rAX","code");
        MOV(textSeg,"rBX","[rAX]");
        MOV(textSeg,"rAX",std::to_string(LINUX_SYSCALL_EXIT).c_str());
        INT(textSeg,"0x80");
        dataSeg->defineLabel("code");
        pushDword(dataSeg,15,true);

        elfHandler.push(outFile);
        outFile.close();
    } else if (strcmp(argv[1],"Win64")==0) {
        std::ofstream outFile(std::string("./", 2) + argv[2] + ".exe", std::ios::binary);
        // PE
        Pe64Handler peHandler;
        Pe64SectionHandler *textSec = peHandler.addSec(".text   ",IMAGE_SCN_CNT_CODE|IMAGE_SCN_MEM_EXECUTE|IMAGE_SCN_MEM_READ);
        Pe64SectionHandler *dataSec = peHandler.addSec(".data   ",IMAGE_SCN_CNT_INITIALIZED_DATA|IMAGE_SCN_MEM_READ);

        textSec->defineLabel("_main");
        POP(textSec,"rBP");
        MOV(textSec,"rBP","rSP");
        SUB(textSec,"rSP","0x20");
        MOV(textSec,"eCX","code");
        JMP(textSec,"[rIP+0x2024]");// jumps to _exit, at 0x3038
        dataSec->defineLabel("code");
        pushDword(dataSec,15,true);
        peHandler.addImport(0x25,"_exit","api-ms-win-crt-runtime-l1-1-0.dll");

        peHandler.push(outFile);
        outFile.close();
    }
    return 0;
}
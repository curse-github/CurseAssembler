#include <fstream>
#include <string>
#include <cstring>
#include <iostream>

#include "globalConstants.h"

#include "intelConstants.h"

const uint32_t exit_code = 0x00000028;
int main(int argc, char *argv[]) {
    if (argc < 3) return 1;
    if (strcmp(argv[1],"Linux")==0) {
        std::ofstream outFile(std::string("./", 2) + argv[2] + ".out", std::ios::binary);
        // ELF
        elf_architecture = ELF_ARCHITECTURE_32;
        elf_encoding = ELF_ENCODING_LSB;
        elf_osAbi = ELF_OS_LINUX;
        elf_type = ELF_TYPE_EXE;
        elf_machine = ELF_MACHINE_x86;

        ElfHandler elfHandler;
        ElfSegmentHandler *dataSeg = elfHandler.addSeg(ELF_SEGMENT_TYPE_LOAD, ELF_SEGMENT_FLAG_READ);
        ElfSegmentHandler *textSeg = elfHandler.addSeg(ELF_SEGMENT_TYPE_LOAD, ELF_SEGMENT_FLAG_READ | ELF_SEGMENT_FLAG_EXECUTE, true);
            // exit(exit_code); or exit(0x28); or exit(40);
        // or
            // MOV eAX exit_code
            // MOV eBX 1
            // XCHG eAX eBX
            // INT 0x80
        // or
            // MOV eAX exit_code
            // MOV eBX 1
            // XCHG eAX eBX
            // SYSCALL
        // or
            // EXIT exit_code

        //4 bytes
        //pushWord(dataSeg, exit_code, elfHandler.isLSB());
        //5 bytes
        //MOVeaxAddr32(textSeg, VirtAddr32 + sizeof(elfHdr32) + sizeof(elfSegmentHdr32) * 2);
        //5 bytes
        //MOVv<ElfSegmentHandler *>(textSeg,"eBX",LINUX_SYSCALL_EXIT);
        //1 bytes
        //XCHGv<ElfSegmentHandler *>(textSeg,"rcx","eBX");
        //2 bytes
        //INT(textSeg,0x80);
        //17 bytes total

        elfHandler.push(outFile);
        outFile.close();
    } else if (strcmp(argv[1],"Win32")==0) {
        std::ofstream outFile(std::string("./", 2) + argv[2] + ".exe", std::ios::binary);
        // PE
        Pe64Handler peHandler;
        Pe64SectionHandler *textSec = peHandler.addSeg(".text   ",IMAGE_SCN_CNT_CODE|IMAGE_SCN_MEM_EXECUTE|IMAGE_SCN_MEM_READ,"entry");
        Pe64SectionHandler *rdataSec = peHandler.addSeg(".rdata  ",IMAGE_SCN_CNT_INITIALIZED_DATA|IMAGE_SCN_MEM_READ,"");
            // exit(0x0F); or exit(15);
        // or
            // push rBP
            // mov rBP, rSP
            // sub rSP, 0x20
            // mov eCX, 0x0f
            // call 0x0e
            // nop nop nop nop nop nop nop nop nop nop nop nop nop nop
            // jmp dword ptr [rip+2012]
            // nop nop
            // nop dword ptr [rax+rax+0x0]

        PUSH(textSec,"rbp");
        MOV(textSec,"rBP","rSP");
        SUB(textSec,"rSP","0x20");
        MOV(textSec,"eCX","0x0f");
        CALL(textSec,"0x0e");
        for (unsigned int i = 0; i < 14; i++) NOP(textSec);
        JMP(textSec,"[rip+0x2012]");
        NOP(textSec);NOP(textSec);NOP(textSec);NOP(textSec);NOP(textSec);
        NOP(textSec,"[rax+rax+0x00000000]");
        pushWord(textSec,0xFFFFFFFF,false);pushWord(textSec,0xFFFFFFFF,false);pushWord(textSec,0x00000000,false);pushWord(textSec,0x00000000,false);
        pushWord(textSec,0xFFFFFFFF,false);pushWord(textSec,0xFFFFFFFF,false);pushWord(textSec,0x00000000,false);pushWord(textSec,0x00000000,false);

        const char* tmp = "GCC: (Rev3, Built by MSYS2 project) 13.2.0\0\0\0\0\0\0";
        pushChars(rdataSec,(const uint8_t*)tmp,48,true);

        peHandler.addImport("api-ms-win-crt-runtime-l1-1-0.dll",peHintNameTable(0x25,"_exit"));
        peHandler.push(outFile);
        outFile.close();
    }
    return 0;
}
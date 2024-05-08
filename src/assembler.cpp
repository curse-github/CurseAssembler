#include <fstream>
#include <string>
#include <cstring>

#include "ELF.h"
#include "PE.h"
#include "globalConstants.h"

const unsigned int exit_code = 0x00000028;
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
        //4 bytes
        pushWord(dataSeg, exit_code, elfHandler.isLSB());
        //5 bytes
        MOVeaxAddr32(textSeg, VirtAddr32 + sizeof(elfHdr32) + sizeof(elfSegmentHdr32) * 2);
        //5 bytes
        MOV32(textSeg, "eBX", LINUX_SYSCALL_EXIT);
        //1 bytes
        XCHG_eAX(textSeg, "eBX");
        //2 bytes
        SYSCALL(textSeg);
        //17 bytes total

        elfHandler.push(outFile);
        outFile.close();
    } else if (strcmp(argv[1],"Win32")==0) {
        std::ofstream outFile(std::string("./", 2) + argv[2] + ".exe", std::ios::binary);
        pushWord(outFile,0x4D5A5000u,false);pushWord(outFile,0x02000000u,false); pushWord(outFile,0x04000F00u,false);pushWord(outFile,0xFFFF0000u,false);// 0x00000000
        pushWord(outFile,0xB8000000u,false);pushWord(outFile,0x00000000u,false); pushWord(outFile,0x40001A00u,false);pushWord(outFile,0x00000000u,false);// 0x00000010
        pushWord(outFile,0x00000000u,false);pushWord(outFile,0x00000000u,false); pushWord(outFile,0x00000000u,false);pushWord(outFile,0x00000000u,false);// 0x00000020
        pushWord(outFile,0x496E556Eu,false);pushWord(outFile,0x00000000u,false); pushWord(outFile,0x00000000u,false);pushWord(outFile,0x00010000u,false);// 0x00000030
        pushWord(outFile,0xBA10000Eu,false);pushWord(outFile,0x1FB409CDu,false); pushWord(outFile,0x21B8014Cu,false);pushWord(outFile,0xCD219090u,false);// 0x00000040
        pushWord(outFile,0x54686973u,false);pushWord(outFile,0x2070726Fu,false); pushWord(outFile,0x6772616Du,false);pushWord(outFile,0x206D7573u,false);// 0x00000050
        pushWord(outFile,0x74206265u,false);pushWord(outFile,0x2072756Eu,false); pushWord(outFile,0x20756E64u,false);pushWord(outFile,0x65722057u,false);// 0x00000060
        pushWord(outFile,0x696E3332u,false);pushWord(outFile,0x0D0A2437u,false); pushWord(outFile,0x00000000u,false);pushWord(outFile,0x00000000u,false);// 0x00000070
        for (unsigned int i = 0; i < 8; i++) {
            pushWord(outFile,0x00000000u,false);pushWord(outFile,0x00000000,false); pushWord(outFile,0x00000000u,false);pushWord(outFile,0x00000000,false);// 0x00000080 - 0x000000F0
        }
        // PE
        PeHandler peHandler;
        PeSectionHandler *textSec = peHandler.addSeg("text    ",IMAGE_SCN_CNT_CODE|IMAGE_SCN_MEM_EXECUTE|IMAGE_SCN_MEM_READ,true);
        //exit(exit_code); or exit(0x28); or exit(40);
        PUSH(textSec,"eBP");
        DEC(textSec,"eAX");
        MOV32(textSec,"eBP","eSP");
        DEC(textSec,"eAX");


        peHandler.push(outFile);
        outFile.close();
    }
    return 0;
}
#include <fstream>
#include <string>
#include <cstring>

#include "ELF.h"
#include "PE.h"
#include "globalConstants.h"

#include "intelConstants.h"

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
        // PE
        PeHandler peHandler;
        PeSectionHandler *textSec = peHandler.addSeg(".text   ",IMAGE_SCN_CNT_CODE|IMAGE_SCN_MEM_EXECUTE|IMAGE_SCN_MEM_READ,"entry");
        PeSectionHandler *rdataSec = peHandler.addSeg(".rdata  ",IMAGE_SCN_CNT_INITIALIZED_DATA|IMAGE_SCN_MEM_READ,"");
        PeSectionHandler *idataSec = peHandler.addSeg(".idata  ",IMAGE_SCN_CNT_INITIALIZED_DATA|IMAGE_SCN_MEM_READ|IMAGE_SCN_MEM_WRITE,"import");
            // exit(0x0F); or exit(15);
        // or
            // PUSH eBP
            // DEC eAX
            // MOV eBP eSP
            // DEC eAX
            // SUB esp 20
            // MOV eCX 0x0F
            // CALL 0x0E or CALL 13
            // NOP NOP NOP NOP NOP NOP NOP NOP NOP NOP NOP NOP NOP NOP
            // 
        //or
            // EXIT 15
        // 55
        PUSH(textSec,"eBP");
        // 48
        DEC(textSec,"eAX");
        // 89 E5
        MOV(textSec,"eBP","eSP");
        // 48
        DEC(textSec,"eAX");
        //sub esp, +$20: 83 EC 20
        //SUBb(textSec,"eSP",0x20);
        pushByte(textSec,INTEL_INSTR_SUB_REGv_Ib);
        pushByte(textSec,INTEL_ModRM_MOD_Reg|0x00101000|INTEL_ModRM_RM_eSP);
        pushByte(textSec,0x20);
        // B9 0F 00 00 00
        MOV32(textSec,"eCX",0x0000000F);
        //Call dword 0x0000000E: E8 0E 00 00 00
        pushByte(textSec,INTEL_INSTR_CALL_Jp);
        pushDword(textSec,0x000000000000000E,PE_IS_LSB);
        // 14 NOPs: 90 90 90 90 90 90 90 90 90 90 90 90 90 90
        for (size_t i = 0; i < 14; i++) NOP(textSec);
        INCindAddr();
        pushHalfWord(textSec,0xFF05,false);pushWord(textSec,0x00000000,PE_IS_LSB);// inc indirect
        DECindAddr();
        pushHalfWord(textSec,0xFF0D,false);pushWord(textSec,0x00000000,PE_IS_LSB);// dec indirect
        CALLindAddr();
        pushHalfWord(textSec,0xFF15,false);pushWord(textSec,0x00000000,PE_IS_LSB);// call indirect
        pushHalfWord(textSec,0xFF1D,false);pushWord(textSec,0x00000000,PE_IS_LSB);// call indirect far
        JMPindAddr();
        pushHalfWord(textSec,0xFF25,false);pushWord(textSec,0x00000000,PE_IS_LSB);// jmp indirect
        pushHalfWord(textSec,0xFF2D,false);pushWord(textSec,0x00000000,PE_IS_LSB);// jmp indirect far
        PUSHindAddr();
        pushHalfWord(textSec,0xFF35,false);pushWord(textSec,0x00000000,PE_IS_LSB);// push indirect
        //pushWord(textSec,0xFF251240u,false);pushWord(textSec,0x00009090u,false); pushWord(textSec,0x0F1F8400u,false);pushWord(textSec,0x00000000u,false);
        //pushWord(textSec,0xFFFFFFFFu,false);pushWord(textSec,0xFFFFFFFFu,false); pushWord(textSec,0x00000000u,false);pushWord(textSec,0x00000000u,false);
        //pushWord(textSec,0xFFFFFFFFu,false);pushWord(textSec,0xFFFFFFFFu,false); pushWord(textSec,0x00000000u,false); pushWord(textSec,0x00000000u,false);


        peHandler.push(outFile);
        outFile.close();
    }
    return 0;
}
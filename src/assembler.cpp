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
        //XCHGv<ElfSegmentHandler *>(textSeg,"eAX","eBX");
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
        Pe64SectionHandler *idataSec = peHandler.addSeg(".idata  ",IMAGE_SCN_CNT_INITIALIZED_DATA|IMAGE_SCN_MEM_READ|IMAGE_SCN_MEM_WRITE,"import");
            // exit(0x0F); or exit(15);
        // or
            // push eBP
            // mov eBP eSP
            // sub eSP 0x20
            // mov eSP 0x0f
            // call 0x0e
        
        ADD(textSec,"eax","eax");
        ADD(textSec,"ecx","eax");
        ADD(textSec,"[eax]","eax");
        ADD(textSec,"[ecx]","eax");
        ADD(textSec,"[rax]","eax");
        ADD(textSec,"[rcx]","eax");
        ADD(textSec,"[0x012c]","eax");
        ADD(textSec,"[eax+0x012c]","eax");
        ADD(textSec,"[ecx+0x012c]","eax");
        ADD(textSec,"[rax+0x012c]","eax");
        ADD(textSec,"[rcx+0x012c]","eax");
        ADD(textSec,"[eax*4+0x012c]","eax");
        ADD(textSec,"[ecx*4+0x012c]","eax");
        ADD(textSec,"[rax*4+0x012c]","eax");
        ADD(textSec,"[rcx*4+0x012c]","eax");
        ADD(textSec,"[eax+ecx*4]","eax");
        ADD(textSec,"[ecx+eax*4]","eax");
        ADD(textSec,"[rax+rcx*4]","eax");
        ADD(textSec,"[rcx+rax*4]","eax");
        ADD(textSec,"[eax+ecx*4+0x012c]","eax");
        ADD(textSec,"[ecx+eax*4+0x012c]","eax");
        ADD(textSec,"[rax+rcx*4+0x012c]","eax");
        ADD(textSec,"[rcx+rax*4+0x012c]","eax");
        
        ADD(textSec,"eax","ecx");
        ADD(textSec,"ecx","ecx");
        ADD(textSec,"[eax]","ecx");
        ADD(textSec,"[ecx]","ecx");
        ADD(textSec,"[rax]","ecx");
        ADD(textSec,"[rcx]","ecx");
        ADD(textSec,"[0x012c]","ecx");
        ADD(textSec,"[eax+0x012c]","ecx");
        ADD(textSec,"[ecx+0x012c]","ecx");
        ADD(textSec,"[rax+0x012c]","ecx");
        ADD(textSec,"[rcx+0x012c]","ecx");
        ADD(textSec,"[eax*4+0x012c]","ecx");
        ADD(textSec,"[ecx*4+0x012c]","ecx");
        ADD(textSec,"[rax*4+0x012c]","ecx");
        ADD(textSec,"[rcx*4+0x012c]","ecx");
        ADD(textSec,"[eax+ecx*4]","ecx");
        ADD(textSec,"[ecx+eax*4]","ecx");
        ADD(textSec,"[rax+rcx*4]","ecx");
        ADD(textSec,"[rcx+rax*4]","ecx");
        ADD(textSec,"[eax+ecx*4+0x012c]","ecx");
        ADD(textSec,"[ecx+eax*4+0x012c]","ecx");
        ADD(textSec,"[rax+rcx*4+0x012c]","ecx");
        ADD(textSec,"[rcx+rax*4+0x012c]","ecx");
        
        ADD(textSec,"rax","rax");
        ADD(textSec,"rcx","rax");
        ADD(textSec,"[eax]","rax");
        ADD(textSec,"[ecx]","rax");
        ADD(textSec,"[rax]","rax");
        ADD(textSec,"[rcx]","rax");
        ADD(textSec,"[0x012c]","rax");
        ADD(textSec,"[eax+0x012c]","rax");
        ADD(textSec,"[ecx+0x012c]","rax");
        ADD(textSec,"[rax+0x012c]","rax");
        ADD(textSec,"[rcx+0x012c]","rax");
        ADD(textSec,"[eax*4+0x012c]","rax");
        ADD(textSec,"[ecx*4+0x012c]","rax");
        ADD(textSec,"[rax*4+0x012c]","rax");
        ADD(textSec,"[rcx*4+0x012c]","rax");
        ADD(textSec,"[eax+ecx*4]","rax");
        ADD(textSec,"[ecx+eax*4]","rax");
        ADD(textSec,"[rax+rcx*4]","rax");
        ADD(textSec,"[rcx+rax*4]","rax");
        ADD(textSec,"[eax+ecx*4+0x012c]","rax");
        ADD(textSec,"[ecx+eax*4+0x012c]","rax");
        ADD(textSec,"[rax+rcx*4+0x012c]","rax");
        ADD(textSec,"[rcx+rax*4+0x012c]","rax");
        
        ADD(textSec,"rax","rcx");
        ADD(textSec,"rcx","rcx");
        ADD(textSec,"[eax]","rcx");
        ADD(textSec,"[ecx]","rcx");
        ADD(textSec,"[rax]","rcx");
        ADD(textSec,"[rcx]","rcx");
        ADD(textSec,"[0x012c]","rcx");
        ADD(textSec,"[eax+0x012c]","rcx");
        ADD(textSec,"[ecx+0x012c]","rcx");
        ADD(textSec,"[rax+0x012c]","rcx");
        ADD(textSec,"[rcx+0x012c]","rcx");
        ADD(textSec,"[eax*4+0x012c]","rcx");
        ADD(textSec,"[ecx*4+0x012c]","rcx");
        ADD(textSec,"[rax*4+0x012c]","rcx");
        ADD(textSec,"[rcx*4+0x012c]","rcx");
        ADD(textSec,"[eax+ecx*4]","rcx");
        ADD(textSec,"[ecx+eax*4]","rcx");
        ADD(textSec,"[rax+rcx*4]","rcx");
        ADD(textSec,"[rcx+rax*4]","rcx");
        ADD(textSec,"[eax+ecx*4+0x012c]","rcx");
        ADD(textSec,"[ecx+eax*4+0x012c]","rcx");
        ADD(textSec,"[rax+rcx*4+0x012c]","rcx");
        ADD(textSec,"[rcx+rax*4+0x012c]","rcx");
        
        ADD(textSec,"eax","0x012c");
        ADD(textSec,"ecx","0x012c");
        ADD(textSec,"rax","0x012c");
        ADD(textSec,"rcx","0x012c");
        ADD(textSec,"[eax]","0x012c");
        ADD(textSec,"[ecx]","0x012c");
        ADD(textSec,"[rax]","0x012c");
        ADD(textSec,"[rcx]","0x012c");
        ADD(textSec,"[0x012c]","0x012c");
        ADD(textSec,"[eax+0x012c]","0x012c");
        ADD(textSec,"[ecx+0x012c]","0x012c");
        ADD(textSec,"[rax+0x012c]","0x012c");
        ADD(textSec,"[rcx+0x012c]","0x012c");
        ADD(textSec,"[eax*4+0x012c]","0x012c");
        ADD(textSec,"[ecx*4+0x012c]","0x012c");
        ADD(textSec,"[rax*4+0x012c]","0x012c");
        ADD(textSec,"[rcx*4+0x012c]","0x012c");
        ADD(textSec,"[eax+ecx*4]","0x012c");
        ADD(textSec,"[ecx+eax*4]","0x012c");
        ADD(textSec,"[rax+rcx*4]","0x012c");
        ADD(textSec,"[rcx+rax*4]","0x012c");
        ADD(textSec,"[eax+ecx*4+0x012c]","0x012c");
        ADD(textSec,"[ecx+eax*4+0x012c]","0x012c");
        ADD(textSec,"[rax+rcx*4+0x012c]","0x012c");
        ADD(textSec,"[rcx+rax*4+0x012c]","0x012c");
        
        ADD(textSec,"eax","[eax]");
        ADD(textSec,"ecx","[eax]");
        ADD(textSec,"rax","[eax]");
        ADD(textSec,"rcx","[eax]");
        
        ADD(textSec,"eax","[ecx]");
        ADD(textSec,"ecx","[ecx]");
        ADD(textSec,"rax","[ecx]");
        ADD(textSec,"rcx","[ecx]");
        
        ADD(textSec,"eax","[rax]");
        ADD(textSec,"ecx","[rax]");
        ADD(textSec,"rax","[rax]");
        ADD(textSec,"rcx","[rax]");
        
        ADD(textSec,"eax","[rcx]");
        ADD(textSec,"ecx","[rcx]");
        ADD(textSec,"rax","[rcx]");
        ADD(textSec,"rcx","[rcx]");
        
        ADD(textSec,"eax","[0x012c]");
        ADD(textSec,"ecx","[0x012c]");
        ADD(textSec,"rax","[0x012c]");
        ADD(textSec,"rcx","[0x012c]");
        
        ADD(textSec,"eax","[eax+0x012c]");
        ADD(textSec,"ecx","[eax+0x012c]");
        ADD(textSec,"rax","[eax+0x012c]");
        ADD(textSec,"rcx","[eax+0x012c]");
        
        ADD(textSec,"eax","[ecx+0x012c]");
        ADD(textSec,"ecx","[ecx+0x012c]");
        ADD(textSec,"rax","[ecx+0x012c]");
        ADD(textSec,"rcx","[ecx+0x012c]");
        
        ADD(textSec,"eax","[rax+0x012c]");
        ADD(textSec,"ecx","[rax+0x012c]");
        ADD(textSec,"rax","[rax+0x012c]");
        ADD(textSec,"rcx","[rax+0x012c]");
        
        ADD(textSec,"eax","[rcx+0x012c]");
        ADD(textSec,"ecx","[rcx+0x012c]");
        ADD(textSec,"rax","[rcx+0x012c]");
        ADD(textSec,"rcx","[rcx+0x012c]");
        
        ADD(textSec,"eax","[eax*4+0x012c]");
        ADD(textSec,"ecx","[eax*4+0x012c]");
        ADD(textSec,"rax","[eax*4+0x012c]");
        ADD(textSec,"rcx","[eax*4+0x012c]");
        
        ADD(textSec,"eax","[ecx*4+0x012c]");
        ADD(textSec,"ecx","[ecx*4+0x012c]");
        ADD(textSec,"rax","[ecx*4+0x012c]");
        ADD(textSec,"rcx","[ecx*4+0x012c]");
        
        ADD(textSec,"eax","[rax*4+0x012c]");
        ADD(textSec,"ecx","[rax*4+0x012c]");
        ADD(textSec,"rax","[rax*4+0x012c]");
        ADD(textSec,"rcx","[rax*4+0x012c]");
        
        ADD(textSec,"eax","[rcx*4+0x012c]");
        ADD(textSec,"ecx","[rcx*4+0x012c]");
        ADD(textSec,"rax","[rcx*4+0x012c]");
        ADD(textSec,"rcx","[rcx*4+0x012c]");
        
        ADD(textSec,"eax","[eax+ecx*4]");
        ADD(textSec,"ecx","[eax+ecx*4]");
        ADD(textSec,"rax","[eax+ecx*4]");
        ADD(textSec,"rcx","[eax+ecx*4]");
        
        ADD(textSec,"eax","[ecx+eax*4]");
        ADD(textSec,"ecx","[ecx+eax*4]");
        ADD(textSec,"rax","[ecx+eax*4]");
        ADD(textSec,"rcx","[ecx+eax*4]");
        
        ADD(textSec,"eax","[rax+rcx*4]");
        ADD(textSec,"ecx","[rax+rcx*4]");
        ADD(textSec,"rax","[rax+rcx*4]");
        ADD(textSec,"rcx","[rax+rcx*4]");
        
        ADD(textSec,"eax","[rcx+rax*4]");
        ADD(textSec,"ecx","[rcx+rax*4]");
        ADD(textSec,"rax","[rcx+rax*4]");
        ADD(textSec,"rcx","[rcx+rax*4]");
        
        ADD(textSec,"eax","[eax+ecx*4+0x012c]");
        ADD(textSec,"ecx","[eax+ecx*4+0x012c]");
        ADD(textSec,"rax","[eax+ecx*4+0x012c]");
        ADD(textSec,"rcx","[eax+ecx*4+0x012c]");
        
        ADD(textSec,"eax","[ecx+eax*4+0x012c]");
        ADD(textSec,"ecx","[ecx+eax*4+0x012c]");
        ADD(textSec,"rax","[ecx+eax*4+0x012c]");
        ADD(textSec,"rcx","[ecx+eax*4+0x012c]");
        
        ADD(textSec,"eax","[rax+rcx*4+0x012c]");
        ADD(textSec,"ecx","[rax+rcx*4+0x012c]");
        ADD(textSec,"rax","[rax+rcx*4+0x012c]");
        ADD(textSec,"rcx","[rax+rcx*4+0x012c]");
        
        ADD(textSec,"eax","[rcx+rax*4+0x012c]");
        ADD(textSec,"ecx","[rcx+rax*4+0x012c]");
        ADD(textSec,"rax","[rcx+rax*4+0x012c]");
        ADD(textSec,"rcx","[rcx+rax*4+0x012c]");

        std::cout << std::endl;
        
        /*PUSHv(textSec,"eBP");
        MOVv(textSec,"eBP", "eSP");
        SUBv(textSec,"eSP", 32);
        MOVv(textSec,"eSP",15);
        CALL(textSec,14);

        pushWord(rdataSec,0x4743433A,false);pushWord(rdataSec,0x20285265,false);pushWord(rdataSec,0x76332C20,false);pushWord(rdataSec,0x4275696C,false);
        pushWord(rdataSec,0x74206279,false);pushWord(rdataSec,0x204D5359,false);pushWord(rdataSec,0x53322070,false);pushWord(rdataSec,0x726F6A65,false);
        pushWord(rdataSec,0x63742920,false);pushWord(rdataSec,0x31332E32,false);pushWord(rdataSec,0x2E300000,false);pushWord(rdataSec,0x00000000,false);

        pushWord(idataSec,0x28300000,false);pushWord(idataSec,0x00000000,false);pushWord(idataSec,0x00000000,false);pushWord(idataSec,0x54300000,false);
        pushWord(idataSec,0x38300000,false);pushWord(idataSec,0x00000000,false);pushWord(idataSec,0x00000000,false);pushWord(idataSec,0x00000000,false);
        pushWord(idataSec,0x00000000,false);pushWord(idataSec,0x00000000,false);pushWord(idataSec,0x48300000,false);pushWord(idataSec,0x00000000,false);
        pushWord(idataSec,0x00000000,false);pushWord(idataSec,0x00000000,false);pushWord(idataSec,0x48300000,false);pushWord(idataSec,0x00000000,false);
        pushWord(idataSec,0x00000000,false);pushWord(idataSec,0x00000000,false);pushWord(idataSec,0x25005F65,false);pushWord(idataSec,0x78697400,false);
        pushWord(idataSec,0x00300000,false);pushWord(idataSec,0x6170692D,false);pushWord(idataSec,0x6D732D77,false);pushWord(idataSec,0x696E2D63,false);
        pushWord(idataSec,0x72742D72,false);pushWord(idataSec,0x756E7469,false);pushWord(idataSec,0x6D652D6C,false);pushWord(idataSec,0x312D312D,false);
        pushWord(idataSec,0x302E646C,false);pushWord(idataSec,0x6C000000,false);*/

        peHandler.push(outFile);
        outFile.close();
    }
    return 0;
}
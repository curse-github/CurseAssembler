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
        
        /*
        ADD(textSec,"[edi]","eax");
        ADD(textSec,"[rcx*2]","eax");
        ADD(textSec,"[0x012c]","eax");
        ADD(textSec,"[edi+edi]","eax"); // [edi*2]
        //ADD(textSec,"[rcx*2+edi]","eax"); // invalid
        ADD(textSec,"[0x012c+edi]","eax");
        //ADD(textSec,"[edi+rcx*2]","eax"); // invalid
        ADD(textSec,"[rcx*2+rcx*2]","eax"); // [rcx*4]
        ADD(textSec,"[0x012c+rcx*2]","eax");
        ADD(textSec,"[edi+0x012c]","eax");
        ADD(textSec,"[rcx*2+0x012c]","eax");
        ADD(textSec,"[0x012c+0x012c]","eax");
        ADD(textSec,"[edi+edi+edi]","eax"); // [edi+edi*2]
        //ADD(textSec,"[rcx*2+edi+edi]","eax"); // invalid
        ADD(textSec,"[0x012c+edi+edi]","eax"); // [edi*2+0x12c]
        //ADD(textSec,"[edi+rcx*2+edi]","eax"); // invalid
        //ADD(textSec,"[rcx*2+rcx*2+edi]","eax"); // invalid
        //ADD(textSec,"[0x012c+rcx*2+edi]","eax");// invalid
        ADD(textSec,"[edi+0x012c+edi]","eax"); // [edi*2+0x012c]
        //ADD(textSec,"[rcx*2+0x012c+edi]","eax");// invalid
        ADD(textSec,"[0x012c+0x012c+edi]","eax"); // [edi+0x0258]
        //ADD(textSec,"[edi+edi+rcx*2]","eax"); // invalid
        //ADD(textSec,"[rcx*2+edi+rcx*2]","eax"); // invalid
        //ADD(textSec,"[0x012c+edi+rcx*2]","eax"); // invalid
        //ADD(textSec,"[edi+rcx*2+rcx*2]","eax"); // invalid
        //ADD(textSec,"[rcx*2+rcx*2+rcx*2]","eax"); // invalid
        ADD(textSec,"[0x012c+rcx*2+rcx*2]","eax"); // [rcx*4+0x012c]
        //ADD(textSec,"[edi+0x012c+rcx*2]","eax"); // invalid
        ADD(textSec,"[rcx*2+0x012c+rcx*2]","eax"); // [rcx*4+0x012c]
        ADD(textSec,"[0x012c+0x012c+rcx*2]","eax"); // [rcx*2+0x0258]
        ADD(textSec,"[edi+edi+0x012c]","eax"); // [edi*2+0x012c]
        //ADD(textSec,"[rcx*2+edi+0x012c]","eax"); // invalid
        ADD(textSec,"[0x012c+edi+0x012c]","eax"); // [edi+0x0258]
        //ADD(textSec,"[edi+rcx*2+0x012c]","eax"); // invalid
        ADD(textSec,"[rcx*2+rcx*2+0x012c]","eax"); // [rcx*4+0x012c]
        ADD(textSec,"[0x012c+rcx*2+0x012c]","eax"); // [rcx*2+0x0258]
        ADD(textSec,"[edi+0x012c+0x012c]","eax"); // [edi+0x0258]
        ADD(textSec,"[rcx*2+0x012c+0x012c]","eax"); // [rcx*2+0x0258]
        ADD(textSec,"[0x012c+0x012c+0x012c]","eax"); // [0x384]
        ADD(textSec,"[edi]","0x012c");
        ADD(textSec,"[rcx*2]","0x012c");
        ADD(textSec,"[0x012c]","0x012c");
        ADD(textSec,"[edi+edi]","0x012c"); // [edi*2]
        //ADD(textSec,"[rcx*2+edi]","0x012c"); // invalid
        ADD(textSec,"[0x012c+edi]","0x012c");
        //ADD(textSec,"[edi+rcx*2]","0x012c"); // invalid
        ADD(textSec,"[rcx*2+rcx*2]","0x012c"); // [rcx*4]
        ADD(textSec,"[0x012c+rcx*2]","0x012c");
        ADD(textSec,"[edi+0x012c]","0x012c");
        ADD(textSec,"[rcx*2+0x012c]","0x012c");
        ADD(textSec,"[0x012c+0x012c]","0x012c");
        ADD(textSec,"[edi+edi+edi]","0x012c"); // [edi+edi*2]
        //ADD(textSec,"[rcx*2+edi+edi]","0x012c"); // invalid
        ADD(textSec,"[0x012c+edi+edi]","0x012c"); // [edi*2+0x12c]
        //ADD(textSec,"[edi+rcx*2+edi]","0x012c"); // invalid
        //ADD(textSec,"[rcx*2+rcx*2+edi]","0x012c"); // invalid
        //ADD(textSec,"[0x012c+rcx*2+edi]","0x012c");// invalid
        ADD(textSec,"[edi+0x012c+edi]","0x012c"); // [edi*2+0x012c]
        //ADD(textSec,"[rcx*2+0x012c+edi]","0x012c");// invalid
        ADD(textSec,"[0x012c+0x012c+edi]","0x012c"); // [edi+0x0258]
        //ADD(textSec,"[edi+edi+rcx*2]","0x012c"); // invalid
        //ADD(textSec,"[rcx*2+edi+rcx*2]","0x012c"); // invalid
        //ADD(textSec,"[0x012c+edi+rcx*2]","0x012c"); // invalid
        //ADD(textSec,"[edi+rcx*2+rcx*2]","0x012c"); // invalid
        //ADD(textSec,"[rcx*2+rcx*2+rcx*2]","0x012c"); // invalid
        ADD(textSec,"[0x012c+rcx*2+rcx*2]","0x012c"); // [rcx*4+0x012c]
        //ADD(textSec,"[edi+0x012c+rcx*2]","0x012c"); // invalid
        ADD(textSec,"[rcx*2+0x012c+rcx*2]","0x012c"); // [rcx*4+0x012c]
        ADD(textSec,"[0x012c+0x012c+rcx*2]","0x012c"); // [rcx*2+0x0258]
        ADD(textSec,"[edi+edi+0x012c]","0x012c"); // [edi*2+0x012c]
        //ADD(textSec,"[rcx*2+edi+0x012c]","0x012c"); // invalid
        ADD(textSec,"[0x012c+edi+0x012c]","0x012c"); // [edi+0x0258]
        //ADD(textSec,"[edi+rcx*2+0x012c]","0x012c"); // invalid
        ADD(textSec,"[rcx*2+rcx*2+0x012c]","0x012c"); // [rcx*4+0x012c]
        ADD(textSec,"[0x012c+rcx*2+0x012c]","0x012c"); // [rcx*2+0x0258]
        ADD(textSec,"[edi+0x012c+0x012c]","0x012c"); // [edi+0x0258]
        ADD(textSec,"[rcx*2+0x012c+0x012c]","0x012c"); // [rcx*2+0x0258]
        ADD(textSec,"[0x012c+0x012c+0x012c]","0x012c"); // [0x384]
        ADD(textSec,"eax","[edi]");
        ADD(textSec,"eax","[rcx*2]");
        ADD(textSec,"eax","[0x012c]");
        ADD(textSec,"eax","[edi+edi]"); // [edi*2]
        //ADD(textSec,"eax","[rcx*2+edi]"); // invalid
        ADD(textSec,"eax","[0x012c+edi]");
        //ADD(textSec,"eax","[edi+rcx*2]"); // invalid
        ADD(textSec,"eax","[rcx*2+rcx*2]"); // [rcx*4]
        ADD(textSec,"eax","[0x012c+rcx*2]");
        ADD(textSec,"eax","[edi+0x012c]");
        ADD(textSec,"eax","[rcx*2+0x012c]");
        ADD(textSec,"eax","[0x012c+0x012c]");
        ADD(textSec,"eax","[edi+edi+edi]"); // [edi+edi*2]
        //ADD(textSec,"eax","[rcx*2+edi+edi]"); // invalid
        ADD(textSec,"eax","[0x012c+edi+edi]"); // [edi*2+0x12c]
        //ADD(textSec,"eax","[edi+rcx*2+edi]"); // invalid
        //ADD(textSec,"eax","[rcx*2+rcx*2+edi]"); // invalid
        //ADD(textSec,"eax","[0x012c+rcx*2+edi]");// invalid
        ADD(textSec,"eax","[edi+0x012c+edi]"); // [edi*2+0x012c]
        //ADD(textSec,"eax","[rcx*2+0x012c+edi]");// invalid
        ADD(textSec,"eax","[0x012c+0x012c+edi]"); // [edi+0x0258]
        //ADD(textSec,"eax","[edi+edi+rcx*2]"); // invalid
        //ADD(textSec,"eax","[rcx*2+edi+rcx*2]"); // invalid
        //ADD(textSec,"eax","[0x012c+edi+rcx*2]"); // invalid
        //ADD(textSec,"eax","[edi+rcx*2+rcx*2]"); // invalid
        //ADD(textSec,"eax","[rcx*2+rcx*2+rcx*2]"); // invalid
        ADD(textSec,"eax","[0x012c+rcx*2+rcx*2]"); // [rcx*4+0x012c]
        //ADD(textSec,"eax","[edi+0x012c+rcx*2]"); // invalid
        ADD(textSec,"eax","[rcx*2+0x012c+rcx*2]"); // [rcx*4+0x012c]
        ADD(textSec,"eax","[0x012c+0x012c+rcx*2]"); // [rcx*2+0x0258]
        ADD(textSec,"eax","[edi+edi+0x012c]"); // [edi*2+0x012c]
        //ADD(textSec,"eax","[rcx*2+edi+0x012c]"); // invalid
        ADD(textSec,"eax","[0x012c+edi+0x012c]"); // [edi+0x0258]
        //ADD(textSec,"eax","[edi+rcx*2+0x012c]"); // invalid
        ADD(textSec,"eax","[rcx*2+rcx*2+0x012c]"); // [rcx*4+0x012c]
        ADD(textSec,"eax","[0x012c+rcx*2+0x012c]"); // [rcx*2+0x0258]
        ADD(textSec,"eax","[edi+0x012c+0x012c]"); // [edi+0x0258]
        ADD(textSec,"eax","[rcx*2+0x012c+0x012c]"); // [rcx*2+0x0258]
        ADD(textSec,"eax","[0x012c+0x012c+0x012c]"); // [0x384]
        */
        ADD(textSec,"[eax+eax*2]","eax");
        ADD(textSec,"[eax*2]","eax");
        ADD(textSec,"[eax]","eax");
        ADD(textSec,"[eax+eax*2+1]","eax");
        ADD(textSec,"[eax*2+1]","eax");
        ADD(textSec,"[eax+1]","eax");

        /*
        ADD(textSec,"eax","eax");
        ADD(textSec,"rax","eax"); // invalid
        ADD(textSec,"0xfa","eax"); // invalid
        ADD(textSec,"0x012c","eax"); // invalid
        ADD(textSec,"[eax]","eax");
        ADD(textSec,"[rax]","eax");
        ADD(textSec,"[0xfa]","eax");
        ADD(textSec,"[0x012c]","eax");
        ADD(textSec,"[eax+0xfa]","eax");
        ADD(textSec,"[rax+0xfa]","eax");
        ADD(textSec,"[eax+0x012c]","eax");
        ADD(textSec,"[rax+0x012c]","eax");
        ADD(textSec,"eax","rax"); // invalid
        ADD(textSec,"rax","rax");
        ADD(textSec,"0xfa","rax"); // invalid
        ADD(textSec,"0x012c","rax"); // invalid
        ADD(textSec,"[eax]","rax");
        ADD(textSec,"[rax]","rax");
        ADD(textSec,"[0xfa]","rax");
        ADD(textSec,"[0x012c]","rax");
        ADD(textSec,"[eax+0xfa]","rax");
        ADD(textSec,"[rax+0xfa]","rax");
        ADD(textSec,"[eax+0x012c]","rax");
        ADD(textSec,"[rax+0x012c]","rax");
        ADD(textSec,"eax","0xfa");
        ADD(textSec,"rax","0xfa");
        ADD(textSec,"0xfa","0xfa"); // invalid
        ADD(textSec,"0x012c","0xfa"); // invalid
        ADD(textSec,"[eax]","0xfa");
        ADD(textSec,"[rax]","0xfa");
        ADD(textSec,"[0xfa]","0xfa");
        ADD(textSec,"[0x012c]","0xfa");
        ADD(textSec,"[eax+0xfa]","0xfa");
        ADD(textSec,"[rax+0xfa]","0xfa");
        ADD(textSec,"[eax+0x012c]","0xfa");
        ADD(textSec,"[rax+0x012c]","0xfa");
        ADD(textSec,"eax","0x012c");
        ADD(textSec,"rax","0x012c");
        ADD(textSec,"0xfa","0x012c"); // invalid
        ADD(textSec,"0x012c","0x012c"); // invalid
        ADD(textSec,"[eax]","0x012c");
        ADD(textSec,"[rax]","0x012c");
        ADD(textSec,"[0xfa]","0x012c");
        ADD(textSec,"[0x012c]","0x012c");
        ADD(textSec,"[eax+0xfa]","0x012c");
        ADD(textSec,"[rax+0xfa]","0x012c");
        ADD(textSec,"[eax+0x012c]","0x012c");
        ADD(textSec,"[rax+0x012c]","0x012c");
        ADD(textSec,"eax","[eax]");
        ADD(textSec,"rax","[eax]");
        ADD(textSec,"0xfa","[eax]"); // invalid
        ADD(textSec,"0x012c","[eax]"); // invalid
        ADD(textSec,"[eax]","[eax]"); // invalid
        ADD(textSec,"[rax]","[eax]"); // invalid
        ADD(textSec,"[0xfa]","[eax]"); // invalid
        ADD(textSec,"[0x012c]","[eax]"); // invalid
        ADD(textSec,"[eax+0xfa]","[eax]"); // invalid
        ADD(textSec,"[rax+0xfa]","[eax]"); // invalid
        ADD(textSec,"[eax+0x012c]","[eax]"); // invalid
        ADD(textSec,"[rax+0x012c]","[eax]"); // invalid
        ADD(textSec,"eax","[rax]");
        ADD(textSec,"rax","[rax]");
        ADD(textSec,"0xfa","[rax]"); // invalid
        ADD(textSec,"0x012c","[rax]"); // invalid
        ADD(textSec,"[eax]","[rax]"); // invalid
        ADD(textSec,"[rax]","[rax]"); // invalid
        ADD(textSec,"[0xfa]","[rax]"); // invalid
        ADD(textSec,"[0x012c]","[rax]"); // invalid
        ADD(textSec,"[eax+0xfa]","[rax]"); // invalid
        ADD(textSec,"[rax+0xfa]","[rax]"); // invalid
        ADD(textSec,"[eax+0x012c]","[rax]"); // invalid
        ADD(textSec,"[rax+0x012c]","[rax]"); // invalid
        */

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
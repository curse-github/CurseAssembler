#include <fstream>
#include <string>
#include <cstring>

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
        pushWord(dataSeg, exit_code, elfHandler.isLSB());
        //5 bytes
        //MOVeaxAddr32(textSeg, VirtAddr32 + sizeof(elfHdr32) + sizeof(elfSegmentHdr32) * 2);
        //5 bytes
        MOVv<ElfSegmentHandler *>(textSeg,"eBX",LINUX_SYSCALL_EXIT);
        //1 bytes
        XCHGv<ElfSegmentHandler *>(textSeg,"eAX","eBX");
        //2 bytes
        INT(textSeg,0x80);
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
            // push eBP
            // mov eBP eSP
            // sub eSP 0x20
            // mov eSP 0x0f
            // cal 0x0e
        PUSHv(textSec,"eBP");
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
        pushWord(idataSec,0x302E646C,false);pushWord(idataSec,0x6C000000,false);

        peHandler.push(outFile);
        outFile.close();
    }
    return 0;
}
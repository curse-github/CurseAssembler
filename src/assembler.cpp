#include <fstream>
#include <string>

//#include "ELF.h"
#include "PE.h"
#include "globalConstants.h"

const unsigned char exit_code = 28;
int main(int argc, char *argv[]) {
    if (argc < 2) return 1;
    std::ofstream outFile(std::string("./", 2) + argv[1] + ".out", std::ios::binary);

    // ELF
    /*elf_architecture = ELF_ARCHITECTURE_32;
    elf_encoding = ELF_ENCODING_LSB;
    elf_osAbi = ELF_OS_LINUX;
    elf_type = ELF_TYPE_EXE;
    elf_machine = ELF_MACHINE_x86;

    ElfHandler elfHandler;
    ElfSegmentHandler *dataSeg = elfHandler.addSeg(ELF_SEGMENT_TYPE_LOAD, ELF_SEGMENT_FLAG_READ);
    ElfSegmentHandler *textSeg = elfHandler.addSeg(ELF_SEGMENT_TYPE_LOAD, ELF_SEGMENT_FLAG_READ | ELF_SEGMENT_FLAG_EXECUTE, true);

    pushWord(dataSeg, 0x00000019, elfHandler.isLSB());
    MOVeaxAddr32(textSeg, VirtAddr32 + sizeof(elfHdr32) + sizeof(elfSegmentHdr32) * 2);
    MOV32(textSeg, "eBX", 0x00000001);
    XCHG_eAX(textSeg, "eBX");
    SYSCALL(textSeg);

    elfHandler.push(outFile);*/

    // PE
    pushByte(outFile, 0x00);
    pushWord(outFile, 0x00000000, false);

    outFile.close();
    return 0;
}
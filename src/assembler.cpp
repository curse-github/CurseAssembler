#include <cstring>
#include <vector>

#include "elf.h"
#include "intelConstants.h"
const unsigned int VirtAddr32 = 0x00400000;
const unsigned int Align32 = 0x00000000;
const unsigned long VirtAddr64 = 0x0000000000400000;
const unsigned long Align64 = 0x0000000000000000;

const unsigned char exit_code = 28;
int main(int argc, char *argv[]) {
    if (argc < 2) return 1;
    std::ofstream outFile(std::string("./", 2) + argv[1] + ".out", std::ios::binary);
    elf_architecture = ELF_ARCHITECTURE_32;
    elf_encoding = ELF_ENCODING_LSB;
    elf_osAbi = ELF_OS_LINUX;
    elf_type = ELF_TYPE_EXE;
    elf_machine = ELF_MACHINE_x86;

    ElfHandler elfHandler;
    ElfSegmentHandler *dataSeg = elfHandler.addSeg(ELF_SEGMENT_TYPE_LOAD, ELF_SEGMENT_FLAG_READ);
    ElfSegmentHandler *textSeg = elfHandler.addSeg(ELF_SEGMENT_TYPE_LOAD, ELF_SEGMENT_FLAG_READ | ELF_SEGMENT_FLAG_EXECUTE,true);

    pushWord(dataSeg, 0x0000000f, elfHandler.isLSB());
    MOVaddr32(textSeg, elfHandler.VirtAddr32 + sizeof(elfHdr32) + sizeof(segmentHdr32)*2);
    MOV32(textSeg,"eBX",0x00000001);
    XCHG_eAX(textSeg,"eBX");
    SYSCALL(textSeg);

    elfHandler.push_to(outFile);
    outFile.close();
    return 0;
}
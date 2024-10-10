#include "globalConstants.h"

int main(int argc, char *argv[]) {
    if (argc < 3) return 1;
    //debugInstructionOutput=true;
    if (strcmp(argv[1],"Linux")==0) {
        std::ofstream outFile(std::string("./", 2) + argv[2] + ".out", std::ios::binary);
        // ELF
        elf_encoding = ELF_ENCODING_LSB;
        elf_osAbi = 0;//ELF_OS_LINUX;
        elf_type = ELF_TYPE_SO;//ELF_TYPE_EXE;
        elf_machine = ELF_MACHINE_AMD64;

        Elf64Handler elfHandler;
        Elf64SegmentHandler *textSeg = elfHandler.addSeg(ELF_SEGMENT_TYPE_LOAD, ELF_SEGMENT_FLAG_READ|ELF_SEGMENT_FLAG_EXECUTE);
        Elf64SegmentHandler *dataSeg = elfHandler.addSeg(ELF_SEGMENT_TYPE_LOAD, ELF_SEGMENT_FLAG_READ);
        
        textSeg->defineLabel("JmpToEx");
        pushDword(textSeg->data,0xfa1e0ff3,true);
        JMP(textSeg,"_Z2exv");
        NOP(textSeg);NOP(textSeg);NOP(textSeg);
        textSeg->defineLabel("_main");
        pushDword(textSeg->data,0xfa1e0ff3,true);
        PUSH(textSeg,"rBP");
        MOV(textSeg,"rBP","rSP");
        CALL(textSeg,"JmpToEx");
        MOV(textSeg,"eax","0");
        POP(textSeg,"rbp");
        RET(textSeg);

        dataSeg->defineLabel("code");
        pushDword(dataSeg,15,true);

        elfHandler.addImport("libfake.so");

        elfHandler.push(outFile);
        outFile.close();
    } else if (strcmp(argv[1],"Win64")==0) {
        std::ofstream outFile(std::string("./", 2) + argv[2] + ".exe", std::ios::binary);
        // PE
        Pe64Handler peHandler;
        Pe64SectionHandler *textSec = peHandler.addSec(".text   ",IMAGE_SCN_CNT_CODE            |IMAGE_SCN_MEM_READ|IMAGE_SCN_MEM_EXECUTE);
        Pe64SectionHandler *dataSec = peHandler.addSec(".data   ",IMAGE_SCN_CNT_INITIALIZED_DATA|IMAGE_SCN_MEM_READ);

        textSec->defineLabel("_main");
        POP(textSec,"rBP");
        MOV(textSec,"rBP","rSP");
        CALL(textSec,"ex");

        dataSec->defineLabel("code");
        pushDword(dataSec,15,true);

        peHandler.addImport("api-ms-win-crt-runtime-l1-1-0.dll");
        peHandler.addImport("libfake.dll");

        peHandler.push(outFile);
        outFile.close();
    }
    return 0;
}
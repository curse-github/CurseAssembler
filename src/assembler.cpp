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
        
        std::vector<const char *> args = {
            "eax","eax",
            "ecx","eax",
            "[eax]","eax",
            "[ecx]","eax",
            "[rax]","eax",
            "[rcx]","eax",
            "[0x012c]","eax",
            "[eax+0x012c]","eax",
            "[ecx+0x012c]","eax",
            "[rax+0x012c]","eax",
            "[rcx+0x012c]","eax",
            "[eax*4+0x012c]","eax",
            "[ecx*4+0x012c]","eax",
            "[rax*4+0x012c]","eax",
            "[rcx*4+0x012c]","eax",
            "[eax+ecx*4]","eax",
            "[ecx+eax*4]","eax",
            "[rax+rcx*4]","eax",
            "[rcx+rax*4]","eax",
            "[eax+ecx*4+0x012c]","eax",
            "[ecx+eax*4+0x012c]","eax",
            "[rax+rcx*4+0x012c]","eax",
            "[rcx+rax*4+0x012c]","eax",
            
            "eax","ecx",
            "ecx","ecx",
            "[eax]","ecx",
            "[ecx]","ecx",
            "[rax]","ecx",
            "[rcx]","ecx",
            "[0x012c]","ecx",
            "[eax+0x012c]","ecx",
            "[ecx+0x012c]","ecx",
            "[rax+0x012c]","ecx",
            "[rcx+0x012c]","ecx",
            "[eax*4+0x012c]","ecx",
            "[ecx*4+0x012c]","ecx",
            "[rax*4+0x012c]","ecx",
            "[rcx*4+0x012c]","ecx",
            "[eax+ecx*4]","ecx",
            "[ecx+eax*4]","ecx",
            "[rax+rcx*4]","ecx",
            "[rcx+rax*4]","ecx",
            "[eax+ecx*4+0x012c]","ecx",
            "[ecx+eax*4+0x012c]","ecx",
            "[rax+rcx*4+0x012c]","ecx",
            "[rcx+rax*4+0x012c]","ecx",
            
            "rax","rax",
            "rcx","rax",
            "[eax]","rax",
            "[ecx]","rax",
            "[rax]","rax",
            "[rcx]","rax",
            "[0x012c]","rax",
            "[eax+0x012c]","rax",
            "[ecx+0x012c]","rax",
            "[rax+0x012c]","rax",
            "[rcx+0x012c]","rax",
            "[eax*4+0x012c]","rax",
            "[ecx*4+0x012c]","rax",
            "[rax*4+0x012c]","rax",
            "[rcx*4+0x012c]","rax",
            "[eax+ecx*4]","rax",
            "[ecx+eax*4]","rax",
            "[rax+rcx*4]","rax",
            "[rcx+rax*4]","rax",
            "[eax+ecx*4+0x012c]","rax",
            "[ecx+eax*4+0x012c]","rax",
            "[rax+rcx*4+0x012c]","rax",
            "[rcx+rax*4+0x012c]","rax",
            
            "rax","rcx",
            "rcx","rcx",
            "[eax]","rcx",
            "[ecx]","rcx",
            "[rax]","rcx",
            "[rcx]","rcx",
            "[0x012c]","rcx",
            "[eax+0x012c]","rcx",
            "[ecx+0x012c]","rcx",
            "[rax+0x012c]","rcx",
            "[rcx+0x012c]","rcx",
            "[eax*4+0x012c]","rcx",
            "[ecx*4+0x012c]","rcx",
            "[rax*4+0x012c]","rcx",
            "[rcx*4+0x012c]","rcx",
            "[eax+ecx*4]","rcx",
            "[ecx+eax*4]","rcx",
            "[rax+rcx*4]","rcx",
            "[rcx+rax*4]","rcx",
            "[eax+ecx*4+0x012c]","rcx",
            "[ecx+eax*4+0x012c]","rcx",
            "[rax+rcx*4+0x012c]","rcx",
            "[rcx+rax*4+0x012c]","rcx",
            
            "eax","0x012c",
            "ecx","0x012c",
            "rax","0x012c",
            "rcx","0x012c",
            "[eax]","0x012c",
            "[ecx]","0x012c",
            "[rax]","0x012c",
            "[rcx]","0x012c",
            "[0x012c]","0x012c",
            "[eax+0x012c]","0x012c",
            "[ecx+0x012c]","0x012c",
            "[rax+0x012c]","0x012c",
            "[rcx+0x012c]","0x012c",
            "[eax*4+0x012c]","0x012c",
            "[ecx*4+0x012c]","0x012c",
            "[rax*4+0x012c]","0x012c",
            "[rcx*4+0x012c]","0x012c",
            "[eax+ecx*4]","0x012c",
            "[ecx+eax*4]","0x012c",
            "[rax+rcx*4]","0x012c",
            "[rcx+rax*4]","0x012c",
            "[eax+ecx*4+0x012c]","0x012c",
            "[ecx+eax*4+0x012c]","0x012c",
            "[rax+rcx*4+0x012c]","0x012c",
            "[rcx+rax*4+0x012c]","0x012c",
            
            "eax","[eax]",
            "ecx","[eax]",
            "rax","[eax]",
            "rcx","[eax]",
            
            "eax","[ecx]",
            "ecx","[ecx]",
            "rax","[ecx]",
            "rcx","[ecx]",
            
            "eax","[rax]",
            "ecx","[rax]",
            "rax","[rax]",
            "rcx","[rax]",
            
            "eax","[rcx]",
            "ecx","[rcx]",
            "rax","[rcx]",
            "rcx","[rcx]",
            
            "eax","[0x012c]",
            "ecx","[0x012c]",
            "rax","[0x012c]",
            "rcx","[0x012c]",
            
            "eax","[eax+0x012c]",
            "ecx","[eax+0x012c]",
            "rax","[eax+0x012c]",
            "rcx","[eax+0x012c]",
            
            "eax","[ecx+0x012c]",
            "ecx","[ecx+0x012c]",
            "rax","[ecx+0x012c]",
            "rcx","[ecx+0x012c]",
            
            "eax","[rax+0x012c]",
            "ecx","[rax+0x012c]",
            "rax","[rax+0x012c]",
            "rcx","[rax+0x012c]",
            
            "eax","[rcx+0x012c]",
            "ecx","[rcx+0x012c]",
            "rax","[rcx+0x012c]",
            "rcx","[rcx+0x012c]",
            
            "eax","[eax*4+0x012c]",
            "ecx","[eax*4+0x012c]",
            "rax","[eax*4+0x012c]",
            "rcx","[eax*4+0x012c]",
            
            "eax","[ecx*4+0x012c]",
            "ecx","[ecx*4+0x012c]",
            "rax","[ecx*4+0x012c]",
            "rcx","[ecx*4+0x012c]",
            
            "eax","[rax*4+0x012c]",
            "ecx","[rax*4+0x012c]",
            "rax","[rax*4+0x012c]",
            "rcx","[rax*4+0x012c]",
            
            "eax","[rcx*4+0x012c]",
            "ecx","[rcx*4+0x012c]",
            "rax","[rcx*4+0x012c]",
            "rcx","[rcx*4+0x012c]",
            
            "eax","[eax+ecx*4]",
            "ecx","[eax+ecx*4]",
            "rax","[eax+ecx*4]",
            "rcx","[eax+ecx*4]",
            
            "eax","[ecx+eax*4]",
            "ecx","[ecx+eax*4]",
            "rax","[ecx+eax*4]",
            "rcx","[ecx+eax*4]",
            
            "eax","[rax+rcx*4]",
            "ecx","[rax+rcx*4]",
            "rax","[rax+rcx*4]",
            "rcx","[rax+rcx*4]",
            
            "eax","[rcx+rax*4]",
            "ecx","[rcx+rax*4]",
            "rax","[rcx+rax*4]",
            "rcx","[rcx+rax*4]",
            
            "eax","[eax+ecx*4+0x012c]",
            "ecx","[eax+ecx*4+0x012c]",
            "rax","[eax+ecx*4+0x012c]",
            "rcx","[eax+ecx*4+0x012c]",
            
            "eax","[ecx+eax*4+0x012c]",
            "ecx","[ecx+eax*4+0x012c]",
            "rax","[ecx+eax*4+0x012c]",
            "rcx","[ecx+eax*4+0x012c]",
            
            "eax","[rax+rcx*4+0x012c]",
            "ecx","[rax+rcx*4+0x012c]",
            "rax","[rax+rcx*4+0x012c]",
            "rcx","[rax+rcx*4+0x012c]",
            
            "eax","[rcx+rax*4+0x012c]",
            "ecx","[rcx+rax*4+0x012c]",
            "rax","[rcx+rax*4+0x012c]",
            "rcx","[rcx+rax*4+0x012c]"
        };
        /*
        for (unsigned int i = 0; i < args.size(); i++) { ADD(textSec,args[i],args[i+1]); i++; }
        for (unsigned int i = 0; i < args.size(); i++) { OR(textSec,args[i],args[i+1]); i++; }
        for (unsigned int i = 0; i < args.size(); i++) { ADC(textSec,args[i],args[i+1]); i++; }
        for (unsigned int i = 0; i < args.size(); i++) { SBB(textSec,args[i],args[i+1]); i++; }
        for (unsigned int i = 0; i < args.size(); i++) { AND(textSec,args[i],args[i+1]); i++; }
        for (unsigned int i = 0; i < args.size(); i++) { SUB(textSec,args[i],args[i+1]); i++; }
        for (unsigned int i = 0; i < args.size(); i++) { XOR(textSec,args[i],args[i+1]); i++; }
        for (unsigned int i = 0; i < args.size(); i++) { CMP(textSec,args[i],args[i+1]); i++; }

        INC(textSec, "eax");
        INC(textSec, "ecx");
        INC(textSec, "rax");
        INC(textSec, "rcx");
        INC(textSec, "[eax]");
        INC(textSec, "[ecx]");
        INC(textSec, "[rax]");
        INC(textSec, "[rcx]");
        INC(textSec, "[0x012c]");
        INC(textSec, "[eax+0x012c]");
        INC(textSec, "[ecx+0x012c]");
        INC(textSec, "[rax+0x012c]");
        INC(textSec, "[rcx+0x012c]");
        INC(textSec, "[eax*4+0x012c]");
        INC(textSec, "[ecx*4+0x012c]");
        INC(textSec, "[rax*4+0x012c]");
        INC(textSec, "[rcx*4+0x012c]");
        INC(textSec, "[eax+ecx*4]");
        INC(textSec, "[ecx+eax*4]");
        INC(textSec, "[rax+rcx*4]");
        INC(textSec, "[rcx+rax*4]");
        INC(textSec, "[eax+ecx*4+0x012c]");
        INC(textSec, "[ecx+eax*4+0x012c]");
        INC(textSec, "[rax+rcx*4+0x012c]");
        INC(textSec, "[rcx+rax*4+0x012c]");

        DEC(textSec, "eax");
        DEC(textSec, "ecx");
        DEC(textSec, "rax");
        DEC(textSec, "rcx");
        DEC(textSec, "[eax]");
        DEC(textSec, "[ecx]");
        DEC(textSec, "[rax]");
        DEC(textSec, "[rcx]");
        DEC(textSec, "[0x012c]");
        DEC(textSec, "[eax+0x012c]");
        DEC(textSec, "[ecx+0x012c]");
        DEC(textSec, "[rax+0x012c]");
        DEC(textSec, "[rcx+0x012c]");
        DEC(textSec, "[eax*4+0x012c]");
        DEC(textSec, "[ecx*4+0x012c]");
        DEC(textSec, "[rax*4+0x012c]");
        DEC(textSec, "[rcx*4+0x012c]");
        DEC(textSec, "[eax+ecx*4]");
        DEC(textSec, "[ecx+eax*4]");
        DEC(textSec, "[rax+rcx*4]");
        DEC(textSec, "[rcx+rax*4]");
        DEC(textSec, "[eax+ecx*4+0x012c]");
        DEC(textSec, "[ecx+eax*4+0x012c]");
        DEC(textSec, "[rax+rcx*4+0x012c]");
        DEC(textSec, "[rcx+rax*4+0x012c]");
        
        CALL(textSec, "0xfa");
        CALL(textSec, "0x012c");
        CALL(textSec, "[eax]");
        CALL(textSec, "[ecx]");
        CALL(textSec, "[rax]");
        CALL(textSec, "[rcx]");
        CALL(textSec, "[0x012c]");
        CALL(textSec, "[eax+0x012c]");
        CALL(textSec, "[ecx+0x012c]");
        CALL(textSec, "[rax+0x012c]");
        CALL(textSec, "[rcx+0x012c]");
        CALL(textSec, "[eax*4+0x012c]");
        CALL(textSec, "[ecx*4+0x012c]");
        CALL(textSec, "[rax*4+0x012c]");
        CALL(textSec, "[rcx*4+0x012c]");
        CALL(textSec, "[eax+ecx*4]");
        CALL(textSec, "[ecx+eax*4]");
        CALL(textSec, "[rax+rcx*4]");
        CALL(textSec, "[rcx+rax*4]");
        CALL(textSec, "[eax+ecx*4+0x012c]");
        CALL(textSec, "[ecx+eax*4+0x012c]");
        CALL(textSec, "[rax+rcx*4+0x012c]");
        CALL(textSec, "[rcx+rax*4+0x012c]");
        
        JMP(textSec, "0xfa");
        JMP(textSec, "0x012c");
        JMP(textSec, "[eax]");
        JMP(textSec, "[ecx]");
        JMP(textSec, "[rax]");
        JMP(textSec, "[rcx]");
        JMP(textSec, "[0x012c]");
        JMP(textSec, "[eax+0x012c]");
        JMP(textSec, "[ecx+0x012c]");
        JMP(textSec, "[rax+0x012c]");
        JMP(textSec, "[rcx+0x012c]");
        JMP(textSec, "[eax*4+0x012c]");
        JMP(textSec, "[ecx*4+0x012c]");
        JMP(textSec, "[rax*4+0x012c]");
        JMP(textSec, "[rcx*4+0x012c]");
        JMP(textSec, "[eax+ecx*4]");
        JMP(textSec, "[ecx+eax*4]");
        JMP(textSec, "[rax+rcx*4]");
        JMP(textSec, "[rcx+rax*4]");
        JMP(textSec, "[eax+ecx*4+0x012c]");
        JMP(textSec, "[ecx+eax*4+0x012c]");
        JMP(textSec, "[rax+rcx*4+0x012c]");
        JMP(textSec, "[rcx+rax*4+0x012c]");

        PUSH(textSec, "0xfa");
        PUSH(textSec, "0x012c");
        PUSH(textSec, "[eax]");
        PUSH(textSec, "[ecx]");
        PUSH(textSec, "[rax]");
        PUSH(textSec, "[rcx]");
        PUSH(textSec, "[0x012c]");
        PUSH(textSec, "[eax+0x012c]");
        PUSH(textSec, "[ecx+0x012c]");
        PUSH(textSec, "[rax+0x012c]");
        PUSH(textSec, "[rcx+0x012c]");
        PUSH(textSec, "[eax*4+0x012c]");
        PUSH(textSec, "[ecx*4+0x012c]");
        PUSH(textSec, "[rax*4+0x012c]");
        PUSH(textSec, "[rcx*4+0x012c]");
        PUSH(textSec, "[eax+ecx*4]");
        PUSH(textSec, "[ecx+eax*4]");
        PUSH(textSec, "[rax+rcx*4]");
        PUSH(textSec, "[rcx+rax*4]");
        PUSH(textSec, "[eax+ecx*4+0x012c]");
        PUSH(textSec, "[ecx+eax*4+0x012c]");
        PUSH(textSec, "[rax+rcx*4+0x012c]");
        PUSH(textSec, "[rcx+rax*4+0x012c]");

        POP(textSec, "[eax]");
        POP(textSec, "[ecx]");
        POP(textSec, "[rax]");
        POP(textSec, "[rcx]");
        POP(textSec, "[0x012c]");
        POP(textSec, "[eax+0x012c]");
        POP(textSec, "[ecx+0x012c]");
        POP(textSec, "[rax+0x012c]");
        POP(textSec, "[rcx+0x012c]");
        POP(textSec, "[eax*4+0x012c]");
        POP(textSec, "[ecx*4+0x012c]");
        POP(textSec, "[rax*4+0x012c]");
        POP(textSec, "[rcx*4+0x012c]");
        POP(textSec, "[eax+ecx*4]");
        POP(textSec, "[ecx+eax*4]");
        POP(textSec, "[rax+rcx*4]");
        POP(textSec, "[rcx+rax*4]");
        POP(textSec, "[eax+ecx*4+0x012c]");
        POP(textSec, "[ecx+eax*4+0x012c]");
        POP(textSec, "[rax+rcx*4+0x012c]");
        POP(textSec, "[rcx+rax*4+0x012c]");

        XCHG(textSec, "eax","eax");
        XCHG(textSec, "ecx","eax");
        XCHG(textSec, "[eax]","eax");
        XCHG(textSec, "[ecx]","eax");
        XCHG(textSec, "[rax]","eax");
        XCHG(textSec, "[rcx]","eax");
        XCHG(textSec, "[0x012c]","eax");
        XCHG(textSec, "[eax+0x012c]","eax");
        XCHG(textSec, "[ecx+0x012c]","eax");
        XCHG(textSec, "[rax+0x012c]","eax");
        XCHG(textSec, "[rcx+0x012c]","eax");
        XCHG(textSec, "[eax*4+0x012c]","eax");
        XCHG(textSec, "[ecx*4+0x012c]","eax");
        XCHG(textSec, "[rax*4+0x012c]","eax");
        XCHG(textSec, "[rcx*4+0x012c]","eax");
        XCHG(textSec, "[eax+ecx*4]","eax");
        XCHG(textSec, "[ecx+eax*4]","eax");
        XCHG(textSec, "[rax+rcx*4]","eax");
        XCHG(textSec, "[rcx+rax*4]","eax");
        XCHG(textSec, "[eax+ecx*4+0x012c]","eax");
        XCHG(textSec, "[ecx+eax*4+0x012c]","eax");
        XCHG(textSec, "[rax+rcx*4+0x012c]","eax");
        XCHG(textSec, "[rcx+rax*4+0x012c]","eax");
        XCHG(textSec, "eax","ecx");
        XCHG(textSec, "ecx","ecx");
        XCHG(textSec, "[eax]","ecx");
        XCHG(textSec, "[ecx]","ecx");
        XCHG(textSec, "[rax]","ecx");
        XCHG(textSec, "[rcx]","ecx");
        XCHG(textSec, "[0x012c]","ecx");
        XCHG(textSec, "[eax+0x012c]","ecx");
        XCHG(textSec, "[ecx+0x012c]","ecx");
        XCHG(textSec, "[rax+0x012c]","ecx");
        XCHG(textSec, "[rcx+0x012c]","ecx");
        XCHG(textSec, "[eax*4+0x012c]","ecx");
        XCHG(textSec, "[ecx*4+0x012c]","ecx");
        XCHG(textSec, "[rax*4+0x012c]","ecx");
        XCHG(textSec, "[rcx*4+0x012c]","ecx");
        XCHG(textSec, "[eax+ecx*4]","ecx");
        XCHG(textSec, "[ecx+eax*4]","ecx");
        XCHG(textSec, "[rax+rcx*4]","ecx");
        XCHG(textSec, "[rcx+rax*4]","ecx");
        XCHG(textSec, "[eax+ecx*4+0x012c]","ecx");
        XCHG(textSec, "[ecx+eax*4+0x012c]","ecx");
        XCHG(textSec, "[rax+rcx*4+0x012c]","ecx");
        XCHG(textSec, "[rcx+rax*4+0x012c]","ecx");
        XCHG(textSec, "rax","rax");
        XCHG(textSec, "rcx","rax");
        XCHG(textSec, "[eax]","rax");
        XCHG(textSec, "[ecx]","rax");
        XCHG(textSec, "[rax]","rax");
        XCHG(textSec, "[rcx]","rax");
        XCHG(textSec, "[0x012c]","rax");
        XCHG(textSec, "[eax+0x012c]","rax");
        XCHG(textSec, "[ecx+0x012c]","rax");
        XCHG(textSec, "[rax+0x012c]","rax");
        XCHG(textSec, "[rcx+0x012c]","rax");
        XCHG(textSec, "[eax*4+0x012c]","rax");
        XCHG(textSec, "[ecx*4+0x012c]","rax");
        XCHG(textSec, "[rax*4+0x012c]","rax");
        XCHG(textSec, "[rcx*4+0x012c]","rax");
        XCHG(textSec, "[eax+ecx*4]","rax");
        XCHG(textSec, "[ecx+eax*4]","rax");
        XCHG(textSec, "[rax+rcx*4]","rax");
        XCHG(textSec, "[rcx+rax*4]","rax");
        XCHG(textSec, "[eax+ecx*4+0x012c]","rax");
        XCHG(textSec, "[ecx+eax*4+0x012c]","rax");
        XCHG(textSec, "[rax+rcx*4+0x012c]","rax");
        XCHG(textSec, "[rcx+rax*4+0x012c]","rax");
        XCHG(textSec, "rax","rcx");
        XCHG(textSec, "rcx","rcx");
        XCHG(textSec, "[eax]","rcx");
        XCHG(textSec, "[ecx]","rcx");
        XCHG(textSec, "[rax]","rcx");
        XCHG(textSec, "[rcx]","rcx");
        XCHG(textSec, "[0x012c]","rcx");
        XCHG(textSec, "[eax+0x012c]","rcx");
        XCHG(textSec, "[ecx+0x012c]","rcx");
        XCHG(textSec, "[rax+0x012c]","rcx");
        XCHG(textSec, "[rcx+0x012c]","rcx");
        XCHG(textSec, "[eax*4+0x012c]","rcx");
        XCHG(textSec, "[ecx*4+0x012c]","rcx");
        XCHG(textSec, "[rax*4+0x012c]","rcx");
        XCHG(textSec, "[rcx*4+0x012c]","rcx");
        XCHG(textSec, "[eax+ecx*4]","rcx");
        XCHG(textSec, "[ecx+eax*4]","rcx");
        XCHG(textSec, "[rax+rcx*4]","rcx");
        XCHG(textSec, "[rcx+rax*4]","rcx");
        XCHG(textSec, "[eax+ecx*4+0x012c]","rcx");
        XCHG(textSec, "[ecx+eax*4+0x012c]","rcx");
        XCHG(textSec, "[rax+rcx*4+0x012c]","rcx");
        XCHG(textSec, "[rcx+rax*4+0x012c]","rcx");

        XCHG(textSec, "eax", "eax");
        XCHG(textSec, "eax", "ecx");
        XCHG(textSec, "eax", "[eax]");
        XCHG(textSec, "eax", "[ecx]");
        XCHG(textSec, "eax", "[rax]");
        XCHG(textSec, "eax", "[rcx]");
        XCHG(textSec, "eax", "[0x012c]");
        XCHG(textSec, "eax", "[eax+0x012c]");
        XCHG(textSec, "eax", "[ecx+0x012c]");
        XCHG(textSec, "eax", "[rax+0x012c]");
        XCHG(textSec, "eax", "[rcx+0x012c]");
        XCHG(textSec, "eax", "[eax*4+0x012c]");
        XCHG(textSec, "eax", "[ecx*4+0x012c]");
        XCHG(textSec, "eax", "[rax*4+0x012c]");
        XCHG(textSec, "eax", "[rcx*4+0x012c]");
        XCHG(textSec, "eax", "[eax+ecx*4]");
        XCHG(textSec, "eax", "[ecx+eax*4]");
        XCHG(textSec, "eax", "[rax+rcx*4]");
        XCHG(textSec, "eax", "[rcx+rax*4]");
        XCHG(textSec, "eax", "[eax+ecx*4+0x012c]");
        XCHG(textSec, "eax", "[ecx+eax*4+0x012c]");
        XCHG(textSec, "eax", "[rax+rcx*4+0x012c]");
        XCHG(textSec, "eax", "[rcx+rax*4+0x012c]");
        XCHG(textSec, "ecx", "eax");
        XCHG(textSec, "ecx", "ecx");
        XCHG(textSec, "ecx", "[eax]");
        XCHG(textSec, "ecx", "[ecx]");
        XCHG(textSec, "ecx", "[rax]");
        XCHG(textSec, "ecx", "[rcx]");
        XCHG(textSec, "ecx", "[0x012c]");
        XCHG(textSec, "ecx", "[eax+0x012c]");
        XCHG(textSec, "ecx", "[ecx+0x012c]");
        XCHG(textSec, "ecx", "[rax+0x012c]");
        XCHG(textSec, "ecx", "[rcx+0x012c]");
        XCHG(textSec, "ecx", "[eax*4+0x012c]");
        XCHG(textSec, "ecx", "[ecx*4+0x012c]");
        XCHG(textSec, "ecx", "[rax*4+0x012c]");
        XCHG(textSec, "ecx", "[rcx*4+0x012c]");
        XCHG(textSec, "ecx", "[eax+ecx*4]");
        XCHG(textSec, "ecx", "[ecx+eax*4]");
        XCHG(textSec, "ecx", "[rax+rcx*4]");
        XCHG(textSec, "ecx", "[rcx+rax*4]");
        XCHG(textSec, "ecx", "[eax+ecx*4+0x012c]");
        XCHG(textSec, "ecx", "[ecx+eax*4+0x012c]");
        XCHG(textSec, "ecx", "[rax+rcx*4+0x012c]");
        XCHG(textSec, "ecx", "[rcx+rax*4+0x012c]");
        XCHG(textSec, "rax", "rax");
        XCHG(textSec, "rax", "rcx");
        XCHG(textSec, "rax", "[eax]");
        XCHG(textSec, "rax", "[ecx]");
        XCHG(textSec, "rax", "[rax]");
        XCHG(textSec, "rax", "[rcx]");
        XCHG(textSec, "rax", "[0x012c]");
        XCHG(textSec, "rax", "[eax+0x012c]");
        XCHG(textSec, "rax", "[ecx+0x012c]");
        XCHG(textSec, "rax", "[rax+0x012c]");
        XCHG(textSec, "rax", "[rcx+0x012c]");
        XCHG(textSec, "rax", "[eax*4+0x012c]");
        XCHG(textSec, "rax", "[ecx*4+0x012c]");
        XCHG(textSec, "rax", "[rax*4+0x012c]");
        XCHG(textSec, "rax", "[rcx*4+0x012c]");
        XCHG(textSec, "rax", "[eax+ecx*4]");
        XCHG(textSec, "rax", "[ecx+eax*4]");
        XCHG(textSec, "rax", "[rax+rcx*4]");
        XCHG(textSec, "rax", "[rcx+rax*4]");
        XCHG(textSec, "rax", "[eax+ecx*4+0x012c]");
        XCHG(textSec, "rax", "[ecx+eax*4+0x012c]");
        XCHG(textSec, "rax", "[rax+rcx*4+0x012c]");
        XCHG(textSec, "rax", "[rcx+rax*4+0x012c]");
        XCHG(textSec, "rcx", "rax");
        XCHG(textSec, "rcx", "rcx");
        XCHG(textSec, "rcx", "[eax]");
        XCHG(textSec, "rcx", "[ecx]");
        XCHG(textSec, "rcx", "[rax]");
        XCHG(textSec, "rcx", "[rcx]");
        XCHG(textSec, "rcx", "[0x012c]");
        XCHG(textSec, "rcx", "[eax+0x012c]");
        XCHG(textSec, "rcx", "[ecx+0x012c]");
        XCHG(textSec, "rcx", "[rax+0x012c]");
        XCHG(textSec, "rcx", "[rcx+0x012c]");
        XCHG(textSec, "rcx", "[eax*4+0x012c]");
        XCHG(textSec, "rcx", "[ecx*4+0x012c]");
        XCHG(textSec, "rcx", "[rax*4+0x012c]");
        XCHG(textSec, "rcx", "[rcx*4+0x012c]");
        XCHG(textSec, "rcx", "[eax+ecx*4]");
        XCHG(textSec, "rcx", "[ecx+eax*4]");
        XCHG(textSec, "rcx", "[rax+rcx*4]");
        XCHG(textSec, "rcx", "[rcx+rax*4]");
        XCHG(textSec, "rcx", "[eax+ecx*4+0x012c]");
        XCHG(textSec, "rcx", "[ecx+eax*4+0x012c]");
        XCHG(textSec, "rcx", "[rax+rcx*4+0x012c]");
        XCHG(textSec, "rcx", "[rcx+rax*4+0x012c]");
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
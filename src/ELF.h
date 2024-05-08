#ifndef _ELF_H
#define _ELF_H

#include <vector>

#include "elfConstants.h"
#include "globalConstants.h"

extern unsigned char elf_architecture;
extern unsigned char elf_encoding;
extern unsigned short elf_type;
extern unsigned char elf_osAbi;
extern unsigned short elf_machine;

#pragma region structs
struct elfSegmentHdr32 {
    unsigned int s_type;            // word
    unsigned int s_fileOffset;      // Offset
    unsigned int s_virtualAddress;  // Address
    unsigned int s_physAddress;     // Address
    unsigned int s_sizeFile;        // word
    unsigned int s_sizeMemory;      // word
    unsigned int s_flags;           // word
    unsigned int s_align;           // word

    elfSegmentHdr32(const unsigned int &type, const unsigned int &flags) {
        // segment type
        if (type == ELF_SEGMENT_TYPE_LOAD || type == ELF_SEGMENT_TYPE_DYN || type == ELF_SEGMENT_TYPE_INTP || type == ELF_SEGMENT_TYPE_NT || type == ELF_SEGMENT_TYPE_PHDR || type == ELF_SEGMENT_TYPE_TLS)
            s_type = type;
        else
            s_type = ELF_SEGMENT_TYPE_INV;
        // file offset
        s_fileOffset = 0x00000000;  // currently unknown
        // virtual address
        s_virtualAddress = 0x00000000;  // currently unknown
        // physical address
        s_physAddress = 0x00000000;
        // size in the file
        s_sizeFile = 0x00000000;  // currently unknown
        // size in memory
        s_sizeMemory = 0x00000000;  // currently unknown
        // flags
        s_flags = flags;
        // bit aligment
        s_align = Align32;
    }
    void push(std::ofstream &stream) {
        bool LSB = elf_encoding == ELF_ENCODING_LSB;  // is little endian
        pushWord(stream, s_type, LSB);
        pushWord(stream, s_fileOffset, LSB);
        pushWord(stream, s_virtualAddress, LSB);
        pushWord(stream, s_physAddress, LSB);
        pushWord(stream, s_sizeFile, LSB);
        pushWord(stream, s_sizeMemory, LSB);
        pushWord(stream, s_flags, LSB);
        pushWord(stream, s_align, LSB);
    }
};
/*struct elfSegmentHdr64 {
    unsigned int s_type;             // word
    unsigned int s_flags;            // word
    unsigned long s_fileOffset;      // Offset
    unsigned long s_virtualAddress;  // Address
    unsigned long s_physAddress;     // Address
    unsigned long s_sizeFile;        // dword
    unsigned long s_sizeMemory;      // dword
    unsigned long s_align;           // dword
};*/

struct elfSectionHdr32 {};
// struct elfSectionHdr64 {};

extern unsigned char elf_architecture;
extern unsigned char elf_encoding;
extern unsigned short elf_type;
extern unsigned char elf_osAbi;
extern unsigned short elf_machine;

struct elfHdr32 {
    unsigned char e_magic[4];         // 4 bytes
    unsigned char e_architecture;     // byte
    unsigned char e_encoding;         // byte
    unsigned char e_metadataVersion;  // byte
    unsigned char e_abi;              // byte
    unsigned char e_abiVersion;       // byte
    unsigned char e_padding[7];       // 7 bytes
    unsigned short e_fileType;        // half word
    unsigned short e_machineType;     // half word
    unsigned int e_version;           // word
    unsigned int e_entryAddress;      // Address
    unsigned int e_segmentHdrOffset;  // Offset
    unsigned int e_sectionHdrOffset;  // Offset
    unsigned int e_flags;             // word
    unsigned short e_elfHdrSize;      // half word
    unsigned short e_segmentHdrSize;  // half word
    unsigned short e_numSegmentHdrs;  // half word
    unsigned short e_sectionHdrSize;  // half word
    unsigned short e_numSectionHdrs;  // half word
    unsigned short e_stringTableNdx;  // half word

    elfHdr32() {
        e_magic[0] = 0x7f;
        e_magic[1] = 'E';
        e_magic[2] = 'L';
        e_magic[3] = 'F';
        // architecture
        e_architecture = ELF_ARCHITECTURE_32;
        // data encoding
        if (elf_encoding == ELF_ENCODING_MSB || elf_encoding == ELF_ENCODING_LSB)
            e_encoding = elf_encoding;
        else
            e_encoding = ELF_ENCODING_INV;
        // metadata version
        e_metadataVersion = 0x01;
        // OS ABI
        if (elf_osAbi == ELF_OS_NONE || elf_osAbi == ELF_OS_HPUX || elf_osAbi == ELF_OS_NETBSD || elf_osAbi == ELF_OS_LINUX)
            e_abi = elf_osAbi;
        else
            e_abi = ELF_OS_INV;
        // ABI version
        e_abiVersion = 0x00;
        // padding
        e_padding[0] = 0xBA;  // BASEBALL
        e_padding[1] = 0x5E;
        e_padding[2] = 0xBA;
        e_padding[3] = 0x11;
        e_padding[4] = 0x00;
        e_padding[5] = 0x00;
        e_padding[6] = 0x00;
        // file type
        if (elf_type == ELF_TYPE_REL || elf_type == ELF_TYPE_EXE || elf_type == ELF_TYPE_SO || elf_type == ELF_TYPE_CORE)
            e_fileType = elf_type;
        else
            e_fileType = ELF_TYPE_INV;
        // machine type
        if (elf_machine == ELF_MACHINE_x86 || elf_machine == ELF_MACHINE_MIPS || elf_machine == ELF_MACHINE_ARM || elf_machine == ELF_MACHINE_AMD64 || elf_machine == ELF_MACHINE_ARM8 || elf_machine == ELF_MACHINE_RISCV)
            e_machineType = elf_machine;
        else
            e_machineType = ELF_TYPE_INV;
        // version
        e_version = 0x00000001;
        // entry
        e_entryAddress = 0x00000000;
        // phoff
        e_segmentHdrOffset = sizeof(elfHdr32);
        // shoff
        e_sectionHdrOffset = 0x00000000;
        // flags
        e_flags = 0x00000000;
        // ehsize
        e_elfHdrSize = sizeof(elfHdr32);
        // phentsize
        e_segmentHdrSize = sizeof(elfSegmentHdr32);
        // phnum
        e_numSegmentHdrs = 0x0000;
        // shentsize
        e_sectionHdrSize = 0;  // sizeof(sectionHdr32);
        // shnum
        e_numSectionHdrs = 0x0000;
        // shstrndx
        e_stringTableNdx = 0x0000;
    }

    void push(std::ofstream &stream) const {
        bool LSB = e_encoding == ELF_ENCODING_LSB;  // is little endian
        for (unsigned char i = 0; i < 4; i++) pushByte(stream, e_magic[i]);
        pushByte(stream, e_architecture);
        pushByte(stream, e_encoding);
        pushByte(stream, e_metadataVersion);
        pushByte(stream, e_abi);
        pushByte(stream, e_abiVersion);
        for (unsigned char i = 0; i < 7; i++) pushByte(stream, e_padding[i]);
        pushHalfWord(stream, e_fileType, LSB);
        pushHalfWord(stream, e_machineType, LSB);
        pushWord(stream, e_version, LSB);
        pushWord(stream, e_entryAddress, LSB);      // pushAddress
        pushWord(stream, e_segmentHdrOffset, LSB);  // pushOffset
        pushWord(stream, e_sectionHdrOffset, LSB);  // pushOffset
        pushWord(stream, e_flags, LSB);
        pushHalfWord(stream, e_elfHdrSize, LSB);
        pushHalfWord(stream, e_segmentHdrSize, LSB);
        pushHalfWord(stream, e_numSegmentHdrs, LSB);
        pushHalfWord(stream, e_sectionHdrSize, LSB);
        pushHalfWord(stream, e_numSectionHdrs, LSB);
        pushHalfWord(stream, e_stringTableNdx, LSB);
    }
};

/*struct elfHdr64 {
    unsigned char e_magic[4];          // 4 bytes
    unsigned char e_architecture;      // byte
    unsigned char e_encoding;          // byte
    unsigned char e_metadataVersion;   // byte
    unsigned char e_abi;               // byte
    unsigned char e_abiVersion;        // byte
    unsigned char e_padding[7];        // 7 bytes
    unsigned short e_fileType;         // half word
    unsigned short e_machineType;      // half word
    unsigned int e_version;            // word
    unsigned long e_entryAddress;      // Address
    unsigned long e_segmentHdrOffset;  // Offset
    unsigned long e_sectionHdrOffset;  // Offset
    unsigned int e_flags;              // word
    unsigned short e_elfHdrSize;       // half word
    unsigned short e_segmentHdrSize;   // half word
    unsigned short e_numSegmentHdrs;   // half word
    unsigned short e_sectionHdrSize;   // half word
    unsigned short e_numSectionHdrs;   // half word
    unsigned short e_stringTableNdx;   // half word
};*/
#pragma endregion structs

#pragma region handlers
class ElfSegmentHandler;

class ElfHandler {
private:
    std::vector<ElfSegmentHandler *> segmentHeaders32;
    elfHdr32 elfHeader;

public:
    ElfHandler();
    constexpr bool isLSB() const {
        return elfHeader.e_encoding == ELF_ENCODING_LSB;
    };
    void push(std::ofstream &stream);
    ElfSegmentHandler *addSeg(const unsigned int &type, const unsigned int &flags, const bool &_isEntry = false);
};

class ElfSegmentHandler {
    ElfHandler &elfHandler;
    elfSegmentHdr32 segmentHeader;
    std::vector<unsigned char> data;

public:
    bool isEntry;
    ElfSegmentHandler(ElfHandler &_elfHandler, const unsigned int &type, const unsigned int &flags, const bool &_isEntry);
    constexpr bool isLSB() const {
        return elfHandler.isLSB();
    };
    void pushHeader(std::ofstream &stream);
    void pushData(std::ofstream &stream);

    unsigned int getSize();
    void setOffset(const unsigned int &offset);

    friend void pushChars(ElfSegmentHandler *segment, const unsigned char *chars, unsigned int len, const bool &LSB);
    friend void pushByte(ElfSegmentHandler *segment, const unsigned char &byte);
    friend void pushHalfWord(ElfSegmentHandler *segment, const unsigned short &halfword, const bool &LSB);
    friend void pushWord(ElfSegmentHandler *segment, const unsigned int &word, const bool &LSB);
    friend void pushDword(ElfSegmentHandler *segment, const unsigned long &dword, const bool &LSB);

    friend void INC(ElfSegmentHandler *segment, const char *reg);
    friend void DEC(ElfSegmentHandler *segment, const char *reg);
    friend void PUSH(ElfSegmentHandler *segment, const char *reg);
    friend void PUSH(ElfSegmentHandler *segment, unsigned int value);
    friend void POP(ElfSegmentHandler *segment, const char *reg);
    friend void NOP(ElfSegmentHandler *segment);
    friend void XCHG_eAX(ElfSegmentHandler *segment, const char *reg);
    friend void MOVeaxAddr32(ElfSegmentHandler *segment, const unsigned int &addr);
    friend void MOVeaxAddr64(ElfSegmentHandler *segment, const unsigned long &addr);
    friend void MOVaddrEax32(ElfSegmentHandler *segment, const unsigned int &addr);
    friend void MOVaddrEax64(ElfSegmentHandler *segment, const unsigned long &addr);
    friend void MOV32(ElfSegmentHandler *segment, const char *reg, const unsigned int &value);
    friend void MOV32(ElfSegmentHandler *segment, const char *reg1, const char *reg2);
    friend void MOV8_low(ElfSegmentHandler *segment, const char *reg, const unsigned char &value);
    friend void MOV8_high(ElfSegmentHandler *segment, const char *reg, const unsigned char &value);
    friend void INT(ElfSegmentHandler *segment, unsigned char value);
    friend void SYSCALL(ElfSegmentHandler *segment);
    friend void JMP32(ElfSegmentHandler *segment, unsigned int value);
    friend void JMP64(ElfSegmentHandler *segment, unsigned long value);
    friend void JMPoffset(ElfSegmentHandler *segment, unsigned char value);
};
#pragma endregion  // handlers

#pragma endregion helpers
void pushChars(ElfSegmentHandler *segment, const unsigned char *chars, unsigned int len, const bool &LSB);
void pushByte(ElfSegmentHandler *segment, const unsigned char &byte);
void pushHalfWord(ElfSegmentHandler *segment, const unsigned short &halfword, const bool &LSB);
void pushWord(ElfSegmentHandler *segment, const unsigned int &word, const bool &LSB);
void pushDword(ElfSegmentHandler *segment, const unsigned long &dword, const bool &LSB);

void INC(ElfSegmentHandler *segment, const char *reg);
void DEC(ElfSegmentHandler *segment, const char *reg);
void PUSH(ElfSegmentHandler *segment, const char *reg);
void PUSH(ElfSegmentHandler *segment, unsigned int value);
void POP(ElfSegmentHandler *segment, const char *reg);
void NOP(ElfSegmentHandler *segment);
void XCHG_eAX(ElfSegmentHandler *segment, const char *reg);
void MOVeaxAddr32(ElfSegmentHandler *segment, const unsigned int &addr);
void MOVeaxAddr64(ElfSegmentHandler *segment, const unsigned long &addr);
void MOVaddrEax32(ElfSegmentHandler *segment, const unsigned int &addr);
void MOVaddrEax64(ElfSegmentHandler *segment, const unsigned long &addr);
void MOV32(ElfSegmentHandler *segment, const char *reg, const unsigned int &value);
void MOV32(ElfSegmentHandler *segment, const char *reg1, const char *reg2);
void MOV8_low(ElfSegmentHandler *segment, const char *reg, const unsigned char &value);
void MOV8_high(ElfSegmentHandler *segment, const char *reg, const unsigned char &value);
void INT(ElfSegmentHandler *segment, unsigned char value);
void SYSCALL(ElfSegmentHandler *segment);
void JMP32(ElfSegmentHandler *segment, unsigned int value);
void JMP64(ElfSegmentHandler *segment, unsigned long value);
void JMPoffset(ElfSegmentHandler *segment, unsigned char value);

#pragma endregion helpers

#endif  // _ELF_H
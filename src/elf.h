#ifndef _ELF_H
#define _ELF_H

#include <vector>

#include "elfConstants.h"
#include "globalConstants.h"

#pragma region structs

struct sectionHdr32 {};
// struct sectionHdr64 {};

extern unsigned char elf_architecture;
extern unsigned char elf_encoding;
extern unsigned short elf_type;
extern unsigned char elf_osAbi;
extern unsigned short elf_machine;

struct elfHdr32 {
public:
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

    void push_to(std::ofstream &stream) const {
        bool LSB = e_encoding == ELF_ENCODING_LSB;  // is little endian
        for (unsigned char i = 0; i < 4; i++)
            pushByte(stream, e_magic[i], LSB);
        pushByte(stream, e_architecture, LSB);
        pushByte(stream, e_encoding, LSB);
        pushByte(stream, e_metadataVersion, LSB);
        pushByte(stream, e_abi, LSB);
        pushByte(stream, e_abiVersion, LSB);
        for (unsigned char i = 0; i < 7; i++)
            pushByte(stream, e_padding[i], LSB);
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

struct segmentHdr32 {
public:
    unsigned int s_type;
    unsigned int s_fileOffset;
    unsigned int s_virtualAddress;
    unsigned int s_physAddress;
    unsigned int s_sizeFile;
    unsigned int s_sizeMemory;
    unsigned int s_flags;
    unsigned int s_align;
};
/*struct segmentHdr64 {
    public:
    unsigned int s_type;
    unsigned int s_flags;
    unsigned long s_fileOffset;
    unsigned long s_virtualAddress;
    unsigned long s_physAddress;
    unsigned long s_sizeFile;
    unsigned long s_sizeMemory;
    unsigned long s_align;
};*/

#pragma endregion structs

#pragma region handlers
class ElfSegmentHandler;
class ElfHandler {
private:
    std::vector<ElfSegmentHandler *> segmentHeaders32;

public:
    elfHdr32 elfHeader;
    const unsigned int VirtAddr32 = 0x00400000;
    const unsigned int Align32 = 0x00000000;
    // const unsigned long VirtAddr64 = 0x0000000000400000;
    // const unsigned long Align64 = 0x0000000000000000;

    ElfHandler();
    constexpr bool isLSB() const { return elfHeader.e_encoding==ELF_ENCODING_LSB; };
    void push_to(std::ofstream &stream);
    ElfSegmentHandler *addSeg(const unsigned int &type, const unsigned int &flags, const bool &_isEntry=false);
};
class ElfSegmentHandler {
public:
    ElfHandler elfHandler;
    segmentHdr32 segmentHeader;
    std::vector<unsigned char> data;
    bool isEntry;
    ElfSegmentHandler(ElfHandler &_elfHandler, const unsigned int &type, const unsigned int &offset, const unsigned int &size, const unsigned int &flags, const bool &_isEntry);
    constexpr bool isLSB() const { return elfHandler.isLSB(); };

    void push_to(std::ofstream &stream) const;
};
#pragma endregion// handlers

#pragma endregion helpers
void pushChars(ElfSegmentHandler *segment, const unsigned char *chars, unsigned int len, const bool &LSB);
void pushByte(ElfSegmentHandler *segment, const unsigned char &byte, const bool &LSB);
void pushHalfWord(ElfSegmentHandler *segment, const unsigned short &halfword, const bool &LSB);
void pushWord(ElfSegmentHandler *segment, const unsigned int &word, const bool &LSB);
void pushDword(ElfSegmentHandler *segment, const unsigned long &dword, const bool &LSB);

void INC(ElfSegmentHandler *segment, const char* reg);
void DEC(ElfSegmentHandler *segment, const char* reg);
void PUSH(ElfSegmentHandler *segment, const char* reg);
void PUSH(ElfSegmentHandler *segment, unsigned int value);
void POP(ElfSegmentHandler *segment, const char* reg);
void NOP(ElfSegmentHandler *segment);
void XCHG_eAX(ElfSegmentHandler *segment, const char* reg);
void MOVaddr32(ElfSegmentHandler *segment, const unsigned int &addr);
void MOVaddr64(ElfSegmentHandler *segment, const unsigned long &addr);
void MOV32(ElfSegmentHandler *segment, const char* reg, const unsigned int &value);
void MOV8_low(ElfSegmentHandler *segment, const char* reg, const unsigned char &value);
void MOV8_high(ElfSegmentHandler *segment, const char* reg, const unsigned char &value);
void INT(ElfSegmentHandler *segment, unsigned char value);
void SYSCALL(ElfSegmentHandler *segment);
void JMP32(ElfSegmentHandler *segment, unsigned int value);
void JMP64(ElfSegmentHandler *segment, unsigned long value);
void JMPoffset(ElfSegmentHandler *segment, unsigned char value);

#pragma endregion helpers

#endif  // _ELF_H
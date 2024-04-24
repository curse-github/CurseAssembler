#include "elf.h"

unsigned char elf_architecture;
unsigned char elf_encoding;
unsigned short elf_type;
unsigned char elf_osAbi;
unsigned short elf_machine;

#pragma region elfHandler
ElfHandler::ElfHandler() {
    elfHeader.e_magic[0] = 0x7f;
    elfHeader.e_magic[1] = 'E';
    elfHeader.e_magic[2] = 'L';
    elfHeader.e_magic[3] = 'F';
    elfHeader.e_padding[0] = 0xBA;  // BASEBALL
    elfHeader.e_padding[1] = 0x5E;
    elfHeader.e_padding[2] = 0xBA;
    elfHeader.e_padding[3] = 0x11;
    elfHeader.e_padding[4] = 0x00;
    elfHeader.e_padding[5] = 0x00;
    elfHeader.e_padding[6] = 0x00;
    // architecture
    elfHeader.e_architecture = ELF_ARCHITECTURE_32;
    // data encoding
    if (elf_encoding == ELF_ENCODING_MSB || elf_encoding == ELF_ENCODING_LSB)
        elfHeader.e_encoding = elf_encoding;
    else
        elfHeader.e_encoding = ELF_ENCODING_INV;
    // metadata version
    elfHeader.e_metadataVersion = 0x01;
    // OS ABI
    if (elf_osAbi == ELF_OS_NONE || elf_osAbi == ELF_OS_HPUX || elf_osAbi == ELF_OS_NETBSD || elf_osAbi == ELF_OS_LINUX)
        elfHeader.e_abi = elf_osAbi;
    else
        elfHeader.e_abi = ELF_OS_INV;
    // ABI version
    elfHeader.e_abiVersion = 0x00;
    // file type
    if (elf_type == ELF_TYPE_REL || elf_type == ELF_TYPE_EXE || elf_type == ELF_TYPE_SO || elf_type == ELF_TYPE_CORE)
        elfHeader.e_fileType = elf_type;
    else
        elfHeader.e_fileType = ELF_TYPE_INV;
    // machine type
    if (elf_machine == ELF_MACHINE_x86 || elf_machine == ELF_MACHINE_MIPS || elf_machine == ELF_MACHINE_ARM || elf_machine == ELF_MACHINE_AMD64 || elf_machine == ELF_MACHINE_ARM8 || elf_machine == ELF_MACHINE_RISCV)
        elfHeader.e_machineType = elf_machine;
    else
        elfHeader.e_machineType = ELF_TYPE_INV;
    // version
    elfHeader.e_version = 0x00000001;
    // entry
    elfHeader.e_entryAddress = 0x00000000;
    // phoff
    elfHeader.e_segmentHdrOffset = sizeof(elfHdr32);
    // shoff
    elfHeader.e_sectionHdrOffset = 0x00000000;
    // flags
    elfHeader.e_flags = 0x00000000;
    // ehsize
    elfHeader.e_elfHdrSize = sizeof(elfHdr32);
    // phentsize
    elfHeader.e_segmentHdrSize = sizeof(segmentHdr32);
    // phnum
    elfHeader.e_numSegmentHdrs = 0x0000;
    // shentsize
    elfHeader.e_sectionHdrSize = 0;  // sizeof(sectionHdr32);
    // shnum
    elfHeader.e_numSectionHdrs = 0x0000;
    // shstrndx
    elfHeader.e_stringTableNdx = 0x0000;
}
void ElfHandler::push_to(std::ofstream &stream) {
    bool LSB = elfHeader.e_encoding == ELF_ENCODING_LSB;  // is little endian
    unsigned short numHeaders = segmentHeaders32.size();
    elfHeader.e_numSegmentHdrs = numHeaders;
    unsigned int baseOffset = sizeof(elfHdr32) + numHeaders * sizeof(segmentHdr32);
    unsigned int runningOffset = baseOffset;
    for (unsigned int i = 0; i < numHeaders; i++) {
        segmentHeaders32[i]->segmentHeader.s_fileOffset = runningOffset;
        segmentHeaders32[i]->segmentHeader.s_virtualAddress = VirtAddr32 + runningOffset;
        if (segmentHeaders32[i]->isEntry) elfHeader.e_entryAddress = VirtAddr32 + runningOffset;
        runningOffset += segmentHeaders32[i]->data.size();
    }
    elfHeader.push_to(stream);
    for (unsigned int i = 0; i < numHeaders; i++) segmentHeaders32[i]->push_to(stream);
    for (unsigned int i = 0; i < numHeaders; i++) pushChars(stream, segmentHeaders32[i]->data);
}
ElfSegmentHandler *ElfHandler::addSeg(const unsigned int &type, const unsigned int &flags, const bool &_isEntry) {
    ElfSegmentHandler *hdr = new ElfSegmentHandler(*this, type, 0, 0, flags, _isEntry);
    segmentHeaders32.push_back(hdr);
    elfHeader.e_numSegmentHdrs = segmentHeaders32.size();
    return hdr;
}
#pragma endregion  // elfHandler

#pragma region segmentHandler
ElfSegmentHandler::ElfSegmentHandler(ElfHandler &_elfHandler, const unsigned int &type, const unsigned int &offset, const unsigned int &size, const unsigned int &flags, const bool &_isEntry) : elfHandler(_elfHandler), isEntry(_isEntry) {
    // segment type
    if (type == ELF_SEGMENT_TYPE_LOAD || type == ELF_SEGMENT_TYPE_DYN || type == ELF_SEGMENT_TYPE_INTP || type == ELF_SEGMENT_TYPE_NT || type == ELF_SEGMENT_TYPE_PHDR || type == ELF_SEGMENT_TYPE_TLS)
        segmentHeader.s_type = type;
    else
        segmentHeader.s_type = ELF_SEGMENT_TYPE_INV;
    // file offset
    segmentHeader.s_fileOffset = offset;
    // virtual address
    segmentHeader.s_virtualAddress = elfHandler.VirtAddr32 + offset;
    // physical address
    segmentHeader.s_physAddress = 0x00000000;
    // size in the file
    segmentHeader.s_sizeFile = size;
    // size in memory
    segmentHeader.s_sizeMemory = size;
    // flags
    segmentHeader.s_flags = flags;
    // bit aligment
    segmentHeader.s_align = elfHandler.Align32;
}
void ElfSegmentHandler::push_to(std::ofstream &stream) const {
    bool LSB = elfHandler.elfHeader.e_encoding == ELF_ENCODING_LSB;  // is little endian
    pushWord(stream, segmentHeader.s_type, LSB);
    pushWord(stream, segmentHeader.s_fileOffset, LSB);      // pushOffset
    pushWord(stream, segmentHeader.s_virtualAddress, LSB);  // pushAddress
    pushWord(stream, segmentHeader.s_physAddress, LSB);     // pushAddress
    pushWord(stream, segmentHeader.s_sizeFile, LSB);
    pushWord(stream, segmentHeader.s_sizeMemory, LSB);
    pushWord(stream, segmentHeader.s_flags, LSB);
    pushWord(stream, segmentHeader.s_align, LSB);
}
#pragma endregion  // segmentHandler

#pragma region helpers
void pushChars(ElfSegmentHandler *segment, const unsigned char *chars, unsigned int len, const bool &LSB) {
    pushChars(segment->data, chars, len, LSB);
}
void pushByte(ElfSegmentHandler *segment, const unsigned char &byte, const bool &LSB) {
    pushByte(segment->data, byte, LSB);
}
void pushHalfWord(ElfSegmentHandler *segment, const unsigned short &halfword, const bool &LSB) {
    pushHalfWord(segment->data, halfword, LSB);
}
void pushWord(ElfSegmentHandler *segment, const unsigned int &word, const bool &LSB) {
    pushWord(segment->data, word, LSB);
}
void pushDword(ElfSegmentHandler *segment, const unsigned long &dword, const bool &LSB) {
    pushDword(segment->data, dword, LSB);
}

void INC(ElfSegmentHandler *segment, const char *reg) {
    INC(segment->data, reg);
}
void DEC(ElfSegmentHandler *segment, const char *reg) {
    DEC(segment->data, reg);
}
void PUSH(ElfSegmentHandler *segment, const char *reg) {
    PUSH(segment->data, reg);
}
void PUSH(ElfSegmentHandler *segment, unsigned int value) {
    PUSH(segment->data, value, segment->isLSB());
}
void POP(ElfSegmentHandler *segment, const char *reg) {
    POP(segment->data, reg);
}
void NOP(ElfSegmentHandler *segment) {
    NOP(segment->data);
}
void XCHG_eAX(ElfSegmentHandler *segment, const char *reg) {
    XCHG_eAX(segment->data, reg);
}
void MOVeaxAddr32(ElfSegmentHandler *segment, const unsigned int &addr) {
    MOVeaxAddr32(segment->data, addr, segment->isLSB());
}
void MOVeaxAddr64(ElfSegmentHandler *segment, const unsigned long &addr) {
    MOVeaxAddr64(segment->data, addr, segment->isLSB());
}
void MOVaddrEax32(ElfSegmentHandler *segment, const unsigned int &addr) {
    MOVaddrEax32(segment->data, addr, segment->isLSB());
}
void MOVaddrEax64(ElfSegmentHandler *segment, const unsigned long &addr) {
    MOVaddrEax64(segment->data, addr, segment->isLSB());
}
void MOV32(ElfSegmentHandler *segment, const char *reg, const unsigned int &value) {
    MOV32(segment->data, reg, value, segment->isLSB());
}
void MOV8_low(ElfSegmentHandler *segment, const char *reg, const unsigned char &value) {
    MOV8_low(segment->data, reg, value);
}
void MOV8_high(ElfSegmentHandler *segment, const char *reg, const unsigned char &value) {
    MOV8_high(segment->data, reg, value);
}
void INT(ElfSegmentHandler *segment, unsigned char value) {
    INT(segment->data, value);
}
void SYSCALL(ElfSegmentHandler *segment) {
    SYSCALL(segment->data, true);
}
void JMP32(ElfSegmentHandler *segment, unsigned int value) {
    JMP32(segment->data, value, segment->isLSB());
}
void JMP64(ElfSegmentHandler *segment, unsigned long value) {
    JMP64(segment->data, value, segment->isLSB());
}
void JMPoffset(ElfSegmentHandler *segment, unsigned char value) {
    JMPoffset(segment->data, value);
}

#pragma endregion  // helpers
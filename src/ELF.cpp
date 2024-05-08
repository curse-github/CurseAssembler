#include "ELF.h"

unsigned char elf_architecture;
unsigned char elf_encoding;
unsigned short elf_type;
unsigned char elf_osAbi;
unsigned short elf_machine;

#pragma region ElfSegmentHandler
ElfSegmentHandler::ElfSegmentHandler(ElfHandler &_elfHandler, const unsigned int &type, const unsigned int &flags, const bool &_isEntry) : elfHandler(_elfHandler), segmentHeader(type, flags), isEntry(_isEntry) {
}
void ElfSegmentHandler::pushHeader(std::ofstream &stream) {
    segmentHeader.s_sizeMemory = segmentHeader.s_sizeFile = data.size();
    segmentHeader.push(stream);
}
void ElfSegmentHandler::pushData(std::ofstream &stream) {
    pushChars(stream, data);
}
unsigned int ElfSegmentHandler::getSize() {
    return data.size();
}
void ElfSegmentHandler::setOffset(const unsigned int &offset) {
    segmentHeader.s_fileOffset = offset;
    segmentHeader.s_virtualAddress = offset;
}

void pushChars(ElfSegmentHandler *segment, const unsigned char *chars, unsigned int len, const bool &LSB) {
    pushChars(segment->data, chars, len, LSB);
}
void pushByte(ElfSegmentHandler *segment, const unsigned char &byte) {
    pushByte(segment->data, byte);
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

//1 bytes
void INC(ElfSegmentHandler *segment, const char *reg) {
    INC(segment->data, reg);
}
//1 bytes
void DEC(ElfSegmentHandler *segment, const char *reg) {
    DEC(segment->data, reg);
}
//1 bytes
void PUSH(ElfSegmentHandler *segment, const char *reg) {
    PUSH(segment->data, reg);
}
//5 bytes
void PUSH(ElfSegmentHandler *segment, unsigned int value) {
    PUSH(segment->data, value, segment->isLSB());
}
//1 bytes
void POP(ElfSegmentHandler *segment, const char *reg) {
    POP(segment->data, reg);
}
//1 bytes
void NOP(ElfSegmentHandler *segment) {
    NOP(segment->data);
}
//1 bytes
void XCHG_eAX(ElfSegmentHandler *segment, const char *reg) {
    XCHG_eAX(segment->data, reg);
}
//5 bytes
void MOVeaxAddr32(ElfSegmentHandler *segment, const unsigned int &addr) {
    MOVeaxAddr32(segment->data, addr, segment->isLSB());
}
//9 bytes
void MOVeaxAddr64(ElfSegmentHandler *segment, const unsigned long &addr) {
    MOVeaxAddr64(segment->data, addr, segment->isLSB());
}
//5 bytes
void MOVaddrEax32(ElfSegmentHandler *segment, const unsigned int &addr) {
    MOVaddrEax32(segment->data, addr, segment->isLSB());
}
//9 bytes
void MOVaddrEax64(ElfSegmentHandler *segment, const unsigned long &addr) {
    MOVaddrEax64(segment->data, addr, segment->isLSB());
}
//5 bytes
void MOV32(ElfSegmentHandler *segment, const char *reg, const unsigned int &value) {
    MOV32(segment->data, reg, value, segment->isLSB());
}
//2 bytes
void MOV32(ElfSegmentHandler *segment, const char *reg1, const char *reg2) {
    MOV32(segment->data,reg1,reg2);
}
//2 bytes
void MOV8_low(ElfSegmentHandler *segment, const char *reg, const unsigned char &value) {
    MOV8_low(segment->data, reg, value);
}
//2 bytes
void MOV8_high(ElfSegmentHandler *segment, const char *reg, const unsigned char &value) {
    MOV8_high(segment->data, reg, value);
}
//2 bytes
void INT(ElfSegmentHandler *segment, unsigned char value) {
    INT(segment->data, value);
}
//2 bytes
void SYSCALL(ElfSegmentHandler *segment) {
    SYSCALL(segment->data, true);
}
//5 bytes
void JMP32(ElfSegmentHandler *segment, unsigned int value) {
    JMP32(segment->data, value, segment->isLSB());
}
//9 bytes
void JMP64(ElfSegmentHandler *segment, unsigned long value) {
    JMP64(segment->data, value, segment->isLSB());
}
//2 bytes
void JMPoffset(ElfSegmentHandler *segment, unsigned char value) {
    JMPoffset(segment->data, value);
}

#pragma endregion  // ElfSegmentHandler

#pragma region elfHandler
ElfHandler::ElfHandler() : elfHeader() {
}
void ElfHandler::push(std::ofstream &stream) {
    bool LSB = elfHeader.e_encoding == ELF_ENCODING_LSB;  // is little endian
    // set some beginning stuff
    unsigned short numHeaders = segmentHeaders32.size();
    elfHeader.e_numSegmentHdrs = numHeaders;
    unsigned int baseOffset = sizeof(elfHdr32) + numHeaders * sizeof(elfSegmentHdr32);
    // get offsets, virtual addresses, and entry point
    unsigned int runningOffset = baseOffset;
    for (unsigned int i = 0; i < numHeaders; i++) {
        segmentHeaders32[i]->setOffset(runningOffset);
        if (segmentHeaders32[i]->isEntry) elfHeader.e_entryAddress = VirtAddr32 + runningOffset;
        runningOffset += segmentHeaders32[i]->getSize();
    }
    // push all the data
    elfHeader.push(stream);
    for (unsigned int i = 0; i < numHeaders; i++) segmentHeaders32[i]->pushHeader(stream);
    for (unsigned int i = 0; i < numHeaders; i++) segmentHeaders32[i]->pushData(stream);
}
ElfSegmentHandler *ElfHandler::addSeg(const unsigned int &type, const unsigned int &flags, const bool &_isEntry) {
    ElfSegmentHandler *hdr = new ElfSegmentHandler(*this, type, flags, _isEntry);
    segmentHeaders32.push_back(hdr);
    return hdr;
}
#pragma endregion  // elfHandler
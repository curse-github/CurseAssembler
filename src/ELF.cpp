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
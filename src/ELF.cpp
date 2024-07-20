#include "ELF.h"

uint8_t elf_architecture;
uint8_t elf_encoding;
uint16_t elf_type;
uint8_t elf_osAbi;
uint16_t elf_machine;

#pragma region ElfSegmentHandler
ElfSegmentHandler::ElfSegmentHandler(ElfHandler &_elfHandler, const uint32_t &type, const uint32_t &flags, const bool &_isEntry) : elfHandler(_elfHandler), segmentHeader(type, flags), isEntry(_isEntry) {
}
void ElfSegmentHandler::pushHeader(std::ofstream &stream) {
    segmentHeader.s_sizeMemory = segmentHeader.s_sizeFile = data.size();
    segmentHeader.push(stream);
}
void ElfSegmentHandler::pushData(std::ofstream &stream) {
    pushChars(stream, data);
}
uint32_t ElfSegmentHandler::getSize() {
    return data.size();
}
void ElfSegmentHandler::setOffset(const uint32_t &offset) {
    segmentHeader.s_fileOffset = offset;
    segmentHeader.s_virtualAddress = offset;
}

void pushChars(ElfSegmentHandler *segment, const uint8_t *chars, uint32_t len, const bool &LSB) {
    pushChars(segment->data, chars, len, LSB);
}
void pushByte(ElfSegmentHandler *segment, const uint8_t &byte) {
    pushByte(segment->data, byte);
}
void pushHalfWord(ElfSegmentHandler *segment, const uint16_t &halfword, const bool &LSB) {
    pushHalfWord(segment->data, halfword, LSB);
}
void pushWord(ElfSegmentHandler *segment, const uint32_t &word, const bool &LSB) {
    pushWord(segment->data, word, LSB);
}
void pushDword(ElfSegmentHandler *segment, const uint64_t &dword, const bool &LSB) {
    pushDword(segment->data, dword, LSB);
}
#pragma endregion  // ElfSegmentHandler

#pragma region elfHandler
ElfHandler::ElfHandler() : elfHeader() {
    bitMode=32;
}
void ElfHandler::push(std::ofstream &stream) {
    bool LSB = elfHeader.e_encoding == ELF_ENCODING_LSB;  // is little endian
    // set some beginning stuff
    uint16_t numHeaders = segmentHeaders32.size();
    elfHeader.e_numSegmentHdrs = numHeaders;
    uint32_t baseOffset = sizeof(elfHdr32) + numHeaders * sizeof(elfSegmentHdr32);
    // get offsets, virtual addresses, and entry point
    uint32_t runningOffset = baseOffset;
    for (uint32_t i = 0; i < numHeaders; i++) {
        segmentHeaders32[i]->setOffset(runningOffset);
        if (segmentHeaders32[i]->isEntry) elfHeader.e_entryAddress = VirtAddr32 + runningOffset;
        runningOffset += segmentHeaders32[i]->getSize();
    }
    // push all the data
    elfHeader.push(stream);
    for (uint32_t i = 0; i < numHeaders; i++) segmentHeaders32[i]->pushHeader(stream);
    for (uint32_t i = 0; i < numHeaders; i++) segmentHeaders32[i]->pushData(stream);
}
ElfSegmentHandler *ElfHandler::addSeg(const uint32_t &type, const uint32_t &flags, const bool &_isEntry) {
    ElfSegmentHandler *hdr = new ElfSegmentHandler(*this, type, flags, _isEntry);
    segmentHeaders32.push_back(hdr);
    return hdr;
}
#pragma endregion  // elfHandler
#include "ELF.h"

uint8_t elf_encoding;
uint16_t elf_type;
uint8_t elf_osAbi;
uint16_t elf_machine;

#pragma region Elf64SegmentHandler
template <>
void pushChars(Elf64SegmentHandler*& reciever, const uint8_t* chars, const size_t& len, const bool& LSB) {
    pushChars(reciever->data, chars, len, LSB);
}
Elf64SegmentHandler::Elf64SegmentHandler(Elf64Handler &_elfHandler, const uint32_t &type, const uint32_t &flags, const bool &_isEntry) : elfHandler(_elfHandler), segmentHeader(type, flags), isEntry(_isEntry) {
}
void Elf64SegmentHandler::pushHeader(std::ofstream &stream) {
    segmentHeader.s_sizeMemory = segmentHeader.s_sizeFile = data.size();
    segmentHeader.push(stream);
}
void Elf64SegmentHandler::pushData(std::ofstream &stream) {
    pushChars(stream, data);
}
uint32_t Elf64SegmentHandler::getSize() {
    return data.size();
}
void Elf64SegmentHandler::setOffset(const uint32_t &offset) {
    segmentHeader.s_fileOffset = offset;
    segmentHeader.s_virtualAddress = offset;
}

#pragma endregion  // Elf64SegmentHandler

#pragma region elfHandler
Elf64Handler::Elf64Handler() : elfHeader() {
    bitMode=64;
}
void Elf64Handler::push(std::ofstream &stream) {
    bool LSB = elfHeader.e_encoding == ELF_ENCODING_LSB;  // is little endian
    // set some beginning stuff
    uint16_t numHeaders = segmentHeaders.size();
    elfHeader.e_numSegmentHdrs = numHeaders;
    uint32_t baseOffset = sizeof(elfHdr64) + numHeaders * sizeof(elfSegmentHdr64);
    // get offsets, virtual addresses, and entry point
    uint32_t runningOffset = baseOffset;
    for (uint32_t i = 0; i < numHeaders; i++) {
        segmentHeaders[i]->setOffset(runningOffset);
        if (segmentHeaders[i]->isEntry) elfHeader.e_entryAddress = VirtAddr64 + runningOffset;
        runningOffset += segmentHeaders[i]->getSize();
    }
    // push all the data
    elfHeader.push(stream);
    for (uint32_t i = 0; i < numHeaders; i++) segmentHeaders[i]->pushHeader(stream);
    for (uint32_t i = 0; i < numHeaders; i++) segmentHeaders[i]->pushData(stream);
}
Elf64SegmentHandler *Elf64Handler::addSeg(const uint32_t &type, const uint32_t &flags, const bool &_isEntry) {
    Elf64SegmentHandler *hdr = new Elf64SegmentHandler(*this, type, flags, _isEntry);
    segmentHeaders.push_back(hdr);
    return hdr;
}
#pragma endregion  // elfHandler
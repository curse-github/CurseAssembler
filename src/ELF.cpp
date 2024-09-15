#include "ELF.h"

uint8_t elf_encoding;
uint16_t elf_type;
uint8_t elf_osAbi;
uint16_t elf_machine;

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
        runningOffset += segmentHeaders[i]->getSize();
    }
    for (unsigned int i = 0; i < labelResolutions.size(); i++) {
        Elf64LabelResolution resolution = labelResolutions[i];
        bool found=false;
        for (unsigned int j = 0; j < labels.size(); j++) {
            Elf64Label label = labels[j];
            if (resolution.name.compare(label.name)==0) {
                found=true;
                setDwordAt(resolution.base->data,resolution.setAt,(label.base->getOffset()+label.offset)-(resolution.base->getOffset()+resolution.setAt)-resolution.relativeToOffset,PE_IS_LSB);
            }
        }
        if (!found) { std::cout << "label/variable definition could not be found" << std::endl; return; }
    }
    for (unsigned int i = 0; i < labels.size(); i++) if (labels[i].name.compare("_main")==0) { elfHeader.e_entryAddress=(labels[i].base->getOffset()+labels[i].offset); continue;}
    // push all the data
    elfHeader.push(stream);
    for (uint32_t i = 0; i < numHeaders; i++) segmentHeaders[i]->pushHeader(stream);
    for (uint32_t i = 0; i < numHeaders; i++) segmentHeaders[i]->pushData(stream);
}
Elf64SegmentHandler *Elf64Handler::addSeg(const uint32_t &type, const uint32_t &flags) {
    Elf64SegmentHandler *hdr = new Elf64SegmentHandler(*this, type, flags);
    segmentHeaders.push_back(hdr);
    return hdr;
}
void Elf64Handler::defineLabel(const std::string& name, Elf64SegmentHandler* base, const uint32_t& offset) {
    labels.push_back(Elf64Label(name,base,offset));
}
void Elf64Handler::resolveLabel(const std::string& name, Elf64SegmentHandler* base, const uint32_t& setAt, const int32_t& relativeToOffset) {
    labelResolutions.push_back(Elf64LabelResolution(name,base,setAt,relativeToOffset));
}
#pragma endregion  // elfHandler

#pragma region Elf64SegmentHandler
template <>
void pushChars(Elf64SegmentHandler*& reciever, const uint8_t* chars, const size_t& len, const bool& LSB) {
    pushChars(reciever->data, chars, len, LSB);
}
Elf64SegmentHandler::Elf64SegmentHandler(Elf64Handler &_elfHandler, const uint32_t &type, const uint32_t &flags) : elfHandler(_elfHandler), segmentHeader(type, flags) {
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
    segmentHeader.s_fileOffset = segmentHeader.s_virtualAddress = offset;
}
uint32_t Elf64SegmentHandler::getOffset() {
    return segmentHeader.s_fileOffset;
}
void Elf64SegmentHandler::defineLabel(const std::string& name) {
    elfHandler.defineLabel(name,this,data.size());
}
void Elf64SegmentHandler::resolveLabel(const std::string& name, const int32_t& relativeToOffset) {
    elfHandler.resolveLabel(name,this,data.size()-relativeToOffset,relativeToOffset);
}

#pragma endregion  // Elf64SegmentHandler
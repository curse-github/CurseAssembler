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
    elfHeader.e_numSegmentHdrs = numHeaders+1;
    uint32_t baseOffset = roundToAlign(sizeof(elfHdr64) + ((numHeaders+1) * sizeof(elfSegmentHdr64)),SECTION_ALIGN);//FILE_ALIGN);
    // get offsets, virtual addresses, and entry point
    uint32_t runningOffset = baseOffset;
    uint32_t runningRVA = roundToAlign(baseOffset,SECTION_ALIGN);
    for (uint32_t i = 0; i < numHeaders; i++) {
        segmentHeaders[i]->setOffset(runningOffset);
        segmentHeaders[i]->setRVA(runningRVA);
        segmentHeaders[i]->setSectionAlign(SECTION_ALIGN);
        segmentHeaders[i]->setFileAlign(SECTION_ALIGN);//FILE_ALIGN);
        uint32_t segmentSize = segmentHeaders[i]->getSize();
        uint32_t virtSize = roundToAlign(segmentSize,SECTION_ALIGN);
        uint32_t fileSize = roundToAlign(segmentSize,SECTION_ALIGN);//FILE_ALIGN);
        runningOffset += fileSize;
        runningRVA += virtSize;
    }
    for (unsigned int i = 0; i < labelResolutions.size(); i++) {
        Elf64LabelResolution resolution = labelResolutions[i];
        bool found=false;
        for (unsigned int j = 0; j < labels.size(); j++) {
            Elf64Label label = labels[j];
            if (resolution.name.compare(label.name)==0) {
                found=true;
                setDwordAt(resolution.base->data,resolution.setAt,(label.base->getRVA()+label.offset)-(resolution.base->getRVA()+resolution.setAt)-resolution.relativeToOffset,true);
            }
        }
        if (!found) { std::cout << "label/variable definition could not be found" << std::endl; return; }
    }
    for (unsigned int i = 0; i < labels.size(); i++) if (labels[i].name.compare("_main")==0) { elfHeader.e_entryAddress=(0x0000000000400000+labels[i].base->getRVA()+labels[i].offset); break;}
    // push all the data
    elfHeader.push(stream);
    elfSegmentHdr64 tmp(ELF_SEGMENT_TYPE_LOAD,ELF_SEGMENT_FLAG_READ);
    tmp.s_align=SECTION_ALIGN;
    tmp.s_fileOffset = 0;
    tmp.s_virtualAddress = tmp.s_physAddress = 0x0000000000400000;
    tmp.s_sizeFile = tmp.s_sizeMemory = 0xAC;
    tmp.push(stream);
    for (uint32_t i = 0; i < numHeaders; i++) segmentHeaders[i]->pushHeader(stream);
    padBytes(stream,baseOffset-(sizeof(elfHdr64) + ((numHeaders+1) * sizeof(elfSegmentHdr64))));
    for (uint32_t i = 0; i < numHeaders; i++) {
        segmentHeaders[i]->pushData(stream);
        padBytes(stream,SECTION_ALIGN-(segmentHeaders[i]->getSize()%(SECTION_ALIGN)));
        //padBytes(stream,FILE_ALIGN-(segmentHeaders[i]->getSize()%(FILE_ALIGN)));
    }
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
    size_t size = data.size();
    segmentHeader.s_sizeFile = segmentHeader.s_sizeMemory = data.size();
    segmentHeader.push(stream);
}
void Elf64SegmentHandler::pushData(std::ofstream &stream) {
    pushChars(stream, data);
}
uint32_t Elf64SegmentHandler::getSize() {
    return data.size();
}
// alignment stuff
void Elf64SegmentHandler::setSectionAlign(const uint32_t& align) {
    segmentHeader.s_align = sectionAlignment = align;
}
void Elf64SegmentHandler::setFileAlign(const uint32_t& align) {
    fileAlignment = align;
}
// offset stuff
void Elf64SegmentHandler::setOffset(const uint32_t &offset) {
    segmentHeader.s_fileOffset = offset;
}
uint32_t Elf64SegmentHandler::getOffset() {
    return segmentHeader.s_fileOffset;
}
// RVA stuff
void Elf64SegmentHandler::setRVA(const uint32_t &Rva) {
    segmentHeader.s_virtualAddress = segmentHeader.s_physAddress = 0x0000000000400000+Rva;// offset in memory, can be different if you have uninitialized data
}
uint32_t Elf64SegmentHandler::getRVA() {
    return segmentHeader.s_virtualAddress-0x0000000000400000;
};
// label stuff
void Elf64SegmentHandler::defineLabel(const std::string& name) {
    elfHandler.defineLabel(name,this,data.size());
}
void Elf64SegmentHandler::resolveLabel(const std::string& name, const int32_t& relativeToOffset) {
    elfHandler.resolveLabel(name,this,data.size()-relativeToOffset,relativeToOffset);
}

#pragma endregion  // Elf64SegmentHandler
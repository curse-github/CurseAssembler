#include "PE.h"
#include <string.h>

#pragma region PeSectionHandler
PeSectionHandler::PeSectionHandler(PeHandler &_peHandler, const char name[8], uint32_t characteristics,  const char *_type) : peHandler(_peHandler), sectionHeader(name, characteristics), type(_type) {
}
void PeSectionHandler::pushHeader(std::ofstream &stream) {
    // set some stuff first
    size_t size = data.size();
    sectionHeader.s_virtualSize = size;
    sectionHeader.s_rawDataSize = size+fileAlignment-(size%fileAlignment);
    sectionHeader.push(stream);
}
void PeSectionHandler::pushData(std::ofstream &stream) {
    pushChars(stream, data);
}
uint32_t PeSectionHandler::getSize() {
    return data.size();
}
void PeSectionHandler::setSectionAlign(const uint32_t& align) {
    sectionAlignment=align;
}
void PeSectionHandler::setFileAlign(const uint32_t& align) {
    fileAlignment=align;
}
void PeSectionHandler::setOffset(const uint32_t &offset) {
    sectionHeader.s_rawDataPointer = offset;// offset in file
}
void PeSectionHandler::setRVA(const uint32_t &Rva) {
    sectionHeader.s_virtualAddress = Rva;// offset in memory, can be different if you have uninitialized data
}

void pushChars(PeSectionHandler *section, const uint8_t *chars, uint32_t len, const bool &LSB) {
    pushChars(section->data, chars, len, LSB);
}
void pushByte(PeSectionHandler *section, const uint8_t &byte) {
    pushByte(section->data, byte);
}
void pushHalfWord(PeSectionHandler *section, const uint16_t &halfword, const bool &LSB) {
    pushHalfWord(section->data, halfword, LSB);
}
void pushWord(PeSectionHandler *section, const uint32_t &word, const bool &LSB) {
    pushWord(section->data, word, LSB);
}
void pushDword(PeSectionHandler *section, const uint64_t &dword, const bool &LSB) {
    pushDword(section->data, dword, LSB);
}
#pragma endregion// PeSectionHandler

#pragma region PeHandler
PeHandler::PeHandler() : peHeader(), peStdFieldsHeader(), peSpecFieldsHeader(), peDataDirHeader() {
}
#include <iostream>
#include <iomanip>
uint32_t roundToAlign(const uint32_t &value, const uint32_t &align) {
    return value+align-(value%align);
}
void PeHandler::push(std::ofstream &stream) {
    // 100 bytes of MZ header
    pushWord(stream,0x4D5A9000u,false);pushWord(stream,0x03000000u,false);pushWord(stream,0x04000000u,false);pushWord(stream,0xFFFF0000u,false);// 0x00000010
    pushWord(stream,0xB8000000u,false);pushWord(stream,0x00000000u,false);pushWord(stream,0x40000000u,false);pushWord(stream,0x00000000u,false);// 0x00000020
    pushWord(stream,0x00000000u,false);pushWord(stream,0x00000000u,false);pushWord(stream,0x00000000u,false);pushWord(stream,0x00000000u,false);// 0x00000030
    pushWord(stream,0x00000000u,false);pushWord(stream,0x00000000u,false);pushWord(stream,0x00000000u,false);pushWord(stream,0x00000100u,true );// 0x00000040
    pushWord(stream,0x0E1FBA0Eu,false);pushWord(stream,0x00B409CDu,false);pushWord(stream,0x21B8014Cu,false);pushWord(stream,0xCD215468u,false);// 0x00000050
    pushWord(stream,0x69732070u,false);pushWord(stream,0x726F6772u,false);pushWord(stream,0x616D2063u,false);pushWord(stream,0x616E6E6Fu,false);// 0x00000060
    pushWord(stream,0x74206265u,false);pushWord(stream,0x2072756Eu,false);pushWord(stream,0x20696E20u,false);pushWord(stream,0x444F5320u,false);// 0x00000070
    pushWord(stream,0x6D6F6465u,false);pushWord(stream,0x2E0D0D0Au,false);pushWord(stream,0x24000000u,false);pushWord(stream,0x00000000u,false);// 0x00000080
    for (int i = 0; i < 8; i++) { pushWord(stream,0x00000000u,false);pushWord(stream,0x00000000u,false);pushWord(stream,0x00000000u,false);pushWord(stream,0x00000000u,false); }

    #define SECTION_ALIGN 0x1000u
    #define FILE_ALIGN 0x200u

    uint16_t numHeaders = sectionHeaders32.size();
    peHeader.p_numberOfSections = numHeaders;
    peHeader.p_sizeOfOptionalHeader = sizeof(peOptHdrStdFields32) + sizeof(peOptHdrSpecFields32) + sizeof(peOptHdrDataDirs32);
    peSpecFieldsHeader.p_sizeOfHeaders = sizeof(peHdr32) + peHeader.p_sizeOfOptionalHeader + (numHeaders * sizeof(peSectionHdr));
    uint32_t baseOffset = roundToAlign(0x100 + peSpecFieldsHeader.p_sizeOfHeaders,FILE_ALIGN);
    peSpecFieldsHeader.p_sizeOfHeaders = roundToAlign(peSpecFieldsHeader.p_sizeOfHeaders,FILE_ALIGN);
    //checksum?

    uint32_t sizeOfInitializedData=0;
    uint32_t sizeOfUninitializedData=0;
    uint32_t entryPoint=0;
    uint32_t sizeOfCode=0;

    std::cout << "baseOffset: 0x" << std::hex << baseOffset << std::endl;
    uint32_t runningOffset = baseOffset;
    std::cout << "runningOffset: 0x" << std::hex << runningOffset << std::endl;
    uint32_t runningRVA = roundToAlign(baseOffset,SECTION_ALIGN);
    std::cout << "runningRVA: 0x" << std::hex << runningRVA << std::endl;
    for (uint32_t i = 0; i < numHeaders; i++) {
        sectionHeaders32[i]->setOffset(runningOffset);
        sectionHeaders32[i]->setRVA(runningRVA);
        sectionHeaders32[i]->setSectionAlign(SECTION_ALIGN);
        sectionHeaders32[i]->setFileAlign(FILE_ALIGN);
        uint32_t sectionSize = sectionHeaders32[i]->getSize();
        uint32_t virtSize = roundToAlign(sectionSize,SECTION_ALIGN);
        uint32_t fileSize = roundToAlign(sectionSize,FILE_ALIGN);
        if (strcmp(sectionHeaders32[i]->type,"") != 0) {
            if (strcmp(sectionHeaders32[i]->type,"entry") == 0) {
                entryPoint = runningRVA;
            } else if (strcmp(sectionHeaders32[i]->type,"exception") == 0) {
                peDataDirHeader.p_exceptionTableRVA = runningRVA;
                peDataDirHeader.p_exceptionTableSize = sectionSize;
            } else if (strcmp(sectionHeaders32[i]->type,"import") == 0) {
                peDataDirHeader.p_importTableRVA = runningRVA;
                peDataDirHeader.p_importTableSize = sectionSize;
            }
        }
        runningRVA += virtSize;
        
        if (sectionHeaders32[i]->isCode()) sizeOfCode+=fileSize;
        else {
            sizeOfInitializedData+=fileSize;
            sizeOfUninitializedData+=0;
        }
        runningOffset += fileSize;
        std::cout << std::endl << "section->size(): 0x" << std::hex << sectionSize << std::endl;
        std::cout << "runningOffset: 0x" << std::hex << runningOffset << std::endl;
        std::cout << "runningRVA: 0x" << std::hex << runningRVA << std::endl;
    }

    peStdFieldsHeader.p_sizeOfCode = sizeOfCode;
    peStdFieldsHeader.p_sizeOfInitializedData = sizeOfInitializedData;
    peStdFieldsHeader.p_sizeOfUninitializedData = sizeOfUninitializedData;

    peStdFieldsHeader.p_addressOfEntryPoint = entryPoint;
    peStdFieldsHeader.p_baseOfCode = 0x1000;
    peStdFieldsHeader.p_baseOfData = 0;
    peSpecFieldsHeader.p_sizeOfImage = runningRVA;
    // push all the data
    peHeader.push(stream);
    peStdFieldsHeader.push(stream);
    peSpecFieldsHeader.push(stream);
    peDataDirHeader.push(stream);
    for (uint32_t i = 0; i < numHeaders; i++) sectionHeaders32[i]->pushHeader(stream);// push all sections headers
    padBytes(stream,baseOffset-(0x100 + sizeof(peHdr32) + peHeader.p_sizeOfOptionalHeader + (numHeaders * sizeof(peSectionHdr))));

    for (uint32_t i = 0; i < numHeaders; i++) {// push all section content
        sectionHeaders32[i]->pushData(stream);
        padBytes(stream,0x200-(sectionHeaders32[i]->getSize()%(0x200)));
    }
}
PeSectionHandler *PeHandler::addSeg(const char name[8], uint32_t characteristics, const char *type) {
    PeSectionHandler *hdr = new PeSectionHandler(*this, name, characteristics, type);
    sectionHeaders32.push_back(hdr);
    return hdr;
}
#pragma endregion// PeHandler
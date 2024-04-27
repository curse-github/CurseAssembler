#include "PE.h"

#pragma region PeSectionHandler
PeSectionHandler::PeSectionHandler(PeHandler &_peHandler, const char name[8], unsigned int characteristics, const bool &_isEntry) : peHandler(_peHandler), sectionHeader(name, characteristics), isEntry(_isEntry) {
}
void PeSectionHandler::pushHeader(std::ofstream &stream) {
    // set some stuff first
    sectionHeader.s_virtualSize = sectionHeader.s_rawDataSize = data.size();
    sectionHeader.push(stream);
}
void PeSectionHandler::pushData(std::ofstream &stream) {
    pushChars(stream, data);
}
unsigned int PeSectionHandler::getSize() {
    return data.size();
}
void PeSectionHandler::setOffset(const unsigned int &offset) {
    sectionHeader.s_rawDataPointer = offset;// offset in file
}
void PeSectionHandler::setRVA(const unsigned int &Rva) {
    sectionHeader.s_virtualAddress = Rva;// offset in memory, can be different if you have uninitialized data
}

void pushChars(PeSectionHandler *section, const unsigned char *chars, unsigned int len, const bool &LSB) {
    pushChars(section->data, chars, len, LSB);
}
void pushByte(PeSectionHandler *section, const unsigned char &byte) {
    pushByte(section->data, byte);
}
void pushHalfWord(PeSectionHandler *section, const unsigned short &halfword, const bool &LSB) {
    pushHalfWord(section->data, halfword, LSB);
}
void pushWord(PeSectionHandler *section, const unsigned int &word, const bool &LSB) {
    pushWord(section->data, word, LSB);
}
void pushDword(PeSectionHandler *section, const unsigned long &dword, const bool &LSB) {
    pushDword(section->data, dword, LSB);
}

void INC(PeSectionHandler *section, const char *reg) {
    INC(section->data, reg);
}
void DEC(PeSectionHandler *section, const char *reg) {
    DEC(section->data, reg);
}
void PUSH(PeSectionHandler *section, const char *reg) {
    PUSH(section->data, reg);
}
void PUSH(PeSectionHandler *section, unsigned int value) {
    PUSH(section->data, value, PE_IS_LSB);
}
void POP(PeSectionHandler *section, const char *reg) {
    POP(section->data, reg);
}
void NOP(PeSectionHandler *section) {
    NOP(section->data);
}
void XCHG_eAX(PeSectionHandler *section, const char *reg) {
    XCHG_eAX(section->data, reg);
}
void MOVeaxAddr32(PeSectionHandler *section, const unsigned int &addr) {
    MOVeaxAddr32(section->data, addr, PE_IS_LSB);
}
void MOVeaxAddr64(PeSectionHandler *section, const unsigned long &addr) {
    MOVeaxAddr64(section->data, addr, PE_IS_LSB);
}
void MOVaddrEax32(PeSectionHandler *section, const unsigned int &addr) {
    MOVaddrEax32(section->data, addr, PE_IS_LSB);
}
void MOVaddrEax64(PeSectionHandler *section, const unsigned long &addr) {
    MOVaddrEax64(section->data, addr, PE_IS_LSB);
}
void MOV32(PeSectionHandler *section, const char *reg, const unsigned int &value) {
    MOV32(section->data, reg, value, PE_IS_LSB);
}
void MOV8_low(PeSectionHandler *section, const char *reg, const unsigned char &value) {
    MOV8_low(section->data, reg, value);
}
void MOV8_high(PeSectionHandler *section, const char *reg, const unsigned char &value) {
    MOV8_high(section->data, reg, value);
}
void INT(PeSectionHandler *section, unsigned char value) {
    INT(section->data, value);
}
void SYSCALL(PeSectionHandler *section) {
    SYSCALL(section->data, true);
}
void JMP32(PeSectionHandler *section, unsigned int value) {
    JMP32(section->data, value, PE_IS_LSB);
}
void JMP64(PeSectionHandler *section, unsigned long value) {
    JMP64(section->data, value, PE_IS_LSB);
}
void JMPoffset(PeSectionHandler *section, unsigned char value) {
    JMPoffset(section->data, value);
}
#pragma endregion// PeSectionHandler

#pragma region PeHandler
PeHandler::PeHandler() : peHeader(), peStdFieldsHeader(), peSpecFieldsHeader() {
}
#include <iostream>
#include <iomanip>
void PeHandler::push(std::ofstream &stream) {
    unsigned short numHeaders = sectionHeaders32.size();
    peHeader.p_numberOfSections = numHeaders;
    unsigned short numDataDirs = peDataDirHeaders.size();
    peSpecFieldsHeader.p_numberAndSizeOfDataDirs=numDataDirs;
    peHeader.p_sizeOfOptionalHeader = sizeof(peOptHdrStdFields32) + sizeof(peOptHdrSpecFields32) + (numDataDirs * sizeof(peOptHdrDataDirs32));
    unsigned int baseOffset = sizeof(peHdr32) + peHeader.p_sizeOfOptionalHeader + (numHeaders * sizeof(peSectionHdr));
    peSpecFieldsHeader.p_sizeOfHeaders = baseOffset;
    std::cout << "p_sizeOfHeaders: " << std::hex << peSpecFieldsHeader.p_sizeOfHeaders << std::endl;
    //checksum?

    unsigned int sizeOfInitializedData=0;
    unsigned int sizeOfUninitializedData=0;
    unsigned int entryPoint=0;
    unsigned int sizeOfCode=0;
    unsigned int runningOffset = 0x100 + baseOffset;
    unsigned int runningRVA=0;
    for (unsigned int i = 0; i < numHeaders; i++) {
        sectionHeaders32[i]->setOffset(runningOffset);
        sectionHeaders32[i]->setRVA(runningRVA);
        if (sectionHeaders32[i]->isEntry) { entryPoint = runningRVA; std::cout << "entryPoint: " << runningRVA << std::endl; }// current RVA
        runningOffset += sectionHeaders32[i]->getSize();
        runningRVA += sectionHeaders32[i]->getSize();
        if (sectionHeaders32[i]->isCode()) sizeOfCode+=sectionHeaders32[i]->getSize();
        else {
            sizeOfInitializedData+=sectionHeaders32[i]->getSize();
            sizeOfUninitializedData+=0;
        }
    }
    peStdFieldsHeader.p_sizeOfCode = sizeOfCode;
    peStdFieldsHeader.p_sizeOfInitializedData = sizeOfInitializedData;
    peStdFieldsHeader.p_sizeOfUninitializedData = sizeOfUninitializedData;
    peStdFieldsHeader.p_addressOfEntryPoint = entryPoint;
    peStdFieldsHeader.p_baseOfCode = baseOffset;
    peStdFieldsHeader.p_baseOfData = baseOffset;
    peSpecFieldsHeader.p_sizeOfImage = peSpecFieldsHeader.p_sizeOfHeaders + sizeOfInitializedData;
    std::cout << "p_sizeOfImage: " << std::hex << peSpecFieldsHeader.p_sizeOfImage << std::endl;
    // push all the data
    peHeader.push(stream);
    peStdFieldsHeader.push(stream);
    peSpecFieldsHeader.push(stream);
    for (unsigned int i = 0; i < numDataDirs; i++) peDataDirHeaders[i]->push(stream);// push all data sections
    for (unsigned int i = 0; i < numHeaders; i++) sectionHeaders32[i]->pushHeader(stream);// push all sections headers
    for (unsigned int i = 0; i < numHeaders; i++) sectionHeaders32[i]->pushData(stream);// push all section content
}
PeSectionHandler *PeHandler::addSeg(const char name[8], unsigned int characteristics, const bool &_isEntry) {
    PeSectionHandler *hdr = new PeSectionHandler(*this, name, characteristics, _isEntry);
    sectionHeaders32.push_back(hdr);
    return hdr;
}
#pragma endregion// PeHandler
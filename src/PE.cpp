#include "PE.h"

//utility func
#include <iostream>
#include <iomanip>
uint32_t roundToAlign(const uint32_t &value, const uint32_t &align) {
    unsigned int mod = value%align;
    if (mod==0) return value;
    return value+align-mod;
}


#include <string.h>

#pragma region Pe32SectionHandler
Pe32SectionHandler::Pe32SectionHandler(Pe32Handler &_peHandler, const char name[8], uint32_t characteristics,  const char *_type) : peHandler(_peHandler), sectionHeader(name, characteristics), type(_type) {
}
void Pe32SectionHandler::pushHeader(std::ofstream &stream) {
    // set some stuff first
    size_t size = data.size();
    sectionHeader.s_virtualSize = size;
    sectionHeader.s_rawDataSize = size+fileAlignment-(size%fileAlignment);
    sectionHeader.push(stream);
}
void Pe32SectionHandler::pushData(std::ofstream &stream) {
    pushChars(stream, data);
}
uint32_t Pe32SectionHandler::getSize() {
    return data.size();
}
void Pe32SectionHandler::setSectionAlign(const uint32_t& align) {
    sectionAlignment=align;
}
void Pe32SectionHandler::setFileAlign(const uint32_t& align) {
    fileAlignment=align;
}
void Pe32SectionHandler::setOffset(const uint32_t &offset) {
    sectionHeader.s_rawDataPointer = offset;// offset in file
}
void Pe32SectionHandler::setRVA(const uint32_t &Rva) {
    sectionHeader.s_virtualAddress = Rva;// offset in memory, can be different if you have uninitialized data
}

void pushChars(Pe32SectionHandler *section, const uint8_t *chars, uint32_t len, const bool &LSB) {
    pushChars(section->data, chars, len, LSB);
}
void pushByte(Pe32SectionHandler *section, const uint8_t &byte) {
    pushByte(section->data, byte);
}
void pushHalfWord(Pe32SectionHandler *section, const uint16_t &halfword, const bool &LSB) {
    pushHalfWord(section->data, halfword, LSB);
}
void pushWord(Pe32SectionHandler *section, const uint32_t &word, const bool &LSB) {
    pushWord(section->data, word, LSB);
}
void pushDword(Pe32SectionHandler *section, const uint64_t &dword, const bool &LSB) {
    pushDword(section->data, dword, LSB);
}
#pragma endregion// Pe32SectionHandler

#pragma region Pe32Handler
Pe32Handler::Pe32Handler() : peHeader(), peStdFieldsHeader(), peSpecFieldsHeader(), peDataDirHeader() {
    bitMode=32;
}
Pe32Handler::~Pe32Handler() {
    for (unsigned int i = 0; i < sectionHeaders32.size(); i++) delete sectionHeaders32[i];
}
void Pe32Handler::push(std::ofstream &stream) {
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

    uint16_t numHeaders = sectionHeaders32.size();
    peHeader.p_numberOfSections = numHeaders;
    peHeader.p_sizeOfOptionalHeader = sizeof(peOptHdrStdFields32) + sizeof(peOptHdrSpecFields32) + sizeof(peOptHdrDataDirs);
    peSpecFieldsHeader.p_sizeOfHeaders = sizeof(peHdr) + peHeader.p_sizeOfOptionalHeader + (numHeaders * sizeof(peSectionHdr));
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
    padBytes(stream,baseOffset-(0x100 + sizeof(peHdr) + peHeader.p_sizeOfOptionalHeader + (numHeaders * sizeof(peSectionHdr))));

    for (uint32_t i = 0; i < numHeaders; i++) {// push all section content
        sectionHeaders32[i]->pushData(stream);
        padBytes(stream,0x200-(sectionHeaders32[i]->getSize()%(0x200)));
    }
}
Pe32SectionHandler *Pe32Handler::addSeg(const char name[8], uint32_t characteristics, const char *type) {
    Pe32SectionHandler *hdr = new Pe32SectionHandler(*this, name, characteristics, type);
    sectionHeaders32.push_back(hdr);
    return hdr;
}
void Pe32Handler::addImport(const std::string& name, const peHintNameTable& hint) {
    importNames.push_back(name);
    importHints.push_back(hint);
}
#pragma endregion// Pe32Handler

#pragma region Pe64SectionHandler
Pe64SectionHandler::Pe64SectionHandler(Pe64Handler &_peHandler, const char name[8], uint32_t characteristics,  const char *_type) : peHandler(_peHandler), sectionHeader(name, characteristics), type(_type) {
}
void Pe64SectionHandler::pushHeader(std::ofstream &stream) {
    // set some stuff first
    size_t size = data.size();
    sectionHeader.s_virtualSize = size;
    sectionHeader.s_rawDataSize = size+fileAlignment-(size%fileAlignment);
    sectionHeader.push(stream);
}
void Pe64SectionHandler::pushData(std::ofstream &stream) {
    pushChars(stream, data);
}
uint32_t Pe64SectionHandler::getSize() {
    return data.size();
}
void Pe64SectionHandler::setSectionAlign(const uint32_t& align) {
    sectionAlignment=align;
}
void Pe64SectionHandler::setFileAlign(const uint32_t& align) {
    fileAlignment=align;
}
void Pe64SectionHandler::setOffset(const uint32_t &offset) {
    sectionHeader.s_rawDataPointer = offset;// offset in file
}
void Pe64SectionHandler::setRVA(const uint32_t &Rva) {
    sectionHeader.s_virtualAddress = Rva;// offset in memory, can be different if you have uninitialized data
}
void pushChars(Pe64SectionHandler *section, const uint8_t *chars, uint32_t len, const bool &LSB) {
    pushChars(section->data, chars, len, LSB);
}
void pushByte(Pe64SectionHandler *section, const uint8_t &byte) {
    pushByte(section->data, byte);
}
void pushHalfWord(Pe64SectionHandler *section, const uint16_t &halfword, const bool &LSB) {
    pushHalfWord(section->data, halfword, LSB);
}
void pushWord(Pe64SectionHandler *section, const uint32_t &word, const bool &LSB) {
    pushWord(section->data, word, LSB);
}
void pushDword(Pe64SectionHandler *section, const uint64_t &dword, const bool &LSB) {
    pushDword(section->data, dword, LSB);
}
#pragma endregion// Pe64SectionHandler

#pragma region Pe64Handler
Pe64Handler::Pe64Handler() : peHeader(), peStdFieldsHeader(), peSpecFieldsHeader(), peDataDirHeader() {
    bitMode=64;
}
Pe64Handler::~Pe64Handler() {
    for (unsigned int i = 0; i < sectionHeaders64.size(); i++) delete sectionHeaders64[i];
}
void Pe64Handler::push(std::ofstream &stream) {
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

    uint16_t numHeaders = sectionHeaders64.size();
    peHeader.p_numberOfSections = numHeaders+static_cast<uint16_t>(importNames.size()!=0);
    peHeader.p_sizeOfOptionalHeader = sizeof(peOptHdrStdFields64) + sizeof(peOptHdrSpecFields64) + sizeof(peOptHdrDataDirs);
    peSpecFieldsHeader.p_sizeOfHeaders = sizeof(peHdr) + peHeader.p_sizeOfOptionalHeader + (numHeaders * sizeof(peSectionHdr)) + ((importNames.size()!=0)?sizeof(peSectionHdr):0);
    uint32_t baseOffset = roundToAlign(0x100 + peSpecFieldsHeader.p_sizeOfHeaders,FILE_ALIGN);
    peSpecFieldsHeader.p_sizeOfHeaders = roundToAlign(peSpecFieldsHeader.p_sizeOfHeaders,FILE_ALIGN);
    //checksum?

    uint32_t sizeOfInitializedData=0;
    uint32_t sizeOfUninitializedData=0;
    uint32_t sizeOfCode=0;

    std::cout << "baseOffset: 0x" << std::hex << baseOffset << std::endl;
    uint32_t runningOffset = baseOffset;
    std::cout << "runningOffset: 0x" << std::hex << runningOffset << std::endl;
    uint32_t runningRVA = roundToAlign(baseOffset,SECTION_ALIGN);
    std::cout << "runningRVA: 0x" << std::hex << runningRVA << std::endl;
    for (uint32_t i = 0; i < numHeaders; i++) {
        sectionHeaders64[i]->setOffset(runningOffset);
        sectionHeaders64[i]->setRVA(runningRVA);
        sectionHeaders64[i]->setSectionAlign(SECTION_ALIGN);
        sectionHeaders64[i]->setFileAlign(FILE_ALIGN);
        uint32_t sectionSize = sectionHeaders64[i]->getSize();
        uint32_t virtSize = roundToAlign(sectionSize,SECTION_ALIGN);
        uint32_t fileSize = roundToAlign(sectionSize,FILE_ALIGN);
        if (strcmp(sectionHeaders64[i]->type,"") != 0) {
            if (strcmp(sectionHeaders64[i]->type,"entry") == 0) {
                peStdFieldsHeader.p_addressOfEntryPoint = runningRVA;
            } else if (strcmp(sectionHeaders64[i]->type,"exception") == 0) {
                peDataDirHeader.p_exceptionTableRVA = runningRVA;
                peDataDirHeader.p_exceptionTableSize = sectionSize;
            }
        }
        runningRVA += virtSize;
        
        if (sectionHeaders64[i]->isCode()) sizeOfCode+=fileSize;
        else {
            sizeOfInitializedData+=fileSize;
            //sizeOfUninitializedData+=0;
        }
        runningOffset += fileSize;
        std::cout << std::endl << "section->size(): 0x" << std::hex << sectionSize << std::endl;
        std::cout << "runningOffset: 0x" << runningOffset << std::endl;
        std::cout << "runningRVA: 0x" << runningRVA << std::endl << std::resetiosflags;
    }
    unsigned int numImports=importNames.size();
    if (numImports!=0) {
        numHeaders++;
        Pe64SectionHandler *hdr = new Pe64SectionHandler(*this, ".idata  ",IMAGE_SCN_CNT_INITIALIZED_DATA|IMAGE_SCN_MEM_READ|IMAGE_SCN_MEM_WRITE,"");
        sectionHeaders64.push_back(hdr);
        hdr->setOffset(runningOffset);
        hdr->setRVA(runningRVA);
        hdr->setSectionAlign(SECTION_ALIGN);
        hdr->setFileAlign(FILE_ALIGN);
        
        uint32_t sizeofImportDirTable = (numImports+1)*sizeof(peImportDirTable);
        uint32_t lookupTableRVA = runningRVA+sizeofImportDirTable;
        uint32_t sizeofImportLookupTable = (numImports+1)*sizeof(pe64ImportLookupTable);
        uint32_t lookupAddressRVA = lookupTableRVA+sizeofImportLookupTable;
        uint32_t sizeofImportAddressTable = (numImports+1)*sizeof(pe64ImportAddressTable);
        uint32_t hintsRVA = lookupAddressRVA+sizeofImportAddressTable;
        uint32_t sizeofImportHintNameTable = 0;
        importHints.push_back(peHintNameTable(0,"0"));
        for (unsigned int i = 0; i <= numImports; i++) sizeofImportHintNameTable+=importHints[i].getSize();
        uint32_t stringsRVA = roundToAlign(hintsRVA+sizeofImportHintNameTable,4);
        std::cout << std::endl << "stringsRVA1: " << std::hex << (hintsRVA+sizeofImportHintNameTable) << std::endl;
        std::cout << "stringsRVA2: " << stringsRVA << std::endl << std::resetiosflags;
        
        
        uint32_t runningStringsRVA=stringsRVA;
        for (unsigned int i = 0; i < numImports; i++) {
            peImportDirTable(lookupTableRVA,runningStringsRVA,lookupAddressRVA).push(hdr->data);
            runningStringsRVA+=importNames[i].size();
        }
        peImportDirTable().push(hdr->data);

        for (unsigned int i = 0; i < numImports; i++) pe64ImportLookupTable(false,hintsRVA).push(hdr->data);
        pe64ImportLookupTable().push(hdr->data);

        for (unsigned int i = 0; i < numImports; i++) pe64ImportAddressTable(false,hintsRVA).push(hdr->data);
        pe64ImportAddressTable().push(hdr->data);

        for (unsigned int i = 0; i <= numImports; i++) importHints[i].push(hdr->data);
        
        padBytes(hdr->data,stringsRVA-(hintsRVA+sizeofImportHintNameTable));
        for (unsigned int i = 0; i < numImports; i++) {
            pushChars(hdr,(const uint8_t*)importNames[i].c_str(),importNames[i].size(),true);
        }

        uint32_t sectionSize = hdr->getSize();
        uint32_t virtSize = roundToAlign(sectionSize,SECTION_ALIGN);
        uint32_t fileSize = roundToAlign(sectionSize,FILE_ALIGN);
        peDataDirHeader.p_importTableRVA = runningRVA;
        peDataDirHeader.p_importTableSize = sectionSize;
        runningRVA += virtSize;
        sizeOfInitializedData+=fileSize;
        runningOffset += fileSize;
    }

    peStdFieldsHeader.p_sizeOfCode = sizeOfCode;
    peStdFieldsHeader.p_sizeOfInitializedData = sizeOfInitializedData;
    peStdFieldsHeader.p_sizeOfUninitializedData = sizeOfUninitializedData;

    peStdFieldsHeader.p_baseOfCode = 0x1000;
    peSpecFieldsHeader.p_sizeOfImage = runningRVA;
    // push all the data
    peHeader.push(stream);
    peStdFieldsHeader.push(stream);
    peSpecFieldsHeader.push(stream);
    peDataDirHeader.push(stream);
    for (unsigned int i = 0; i < numHeaders; i++) sectionHeaders64[i]->pushHeader(stream);// push all sections headers
    padBytes(stream,baseOffset-(0x100 + sizeof(peHdr) + peHeader.p_sizeOfOptionalHeader + (numHeaders * sizeof(peSectionHdr))));
    for (unsigned int i = 0; i < numHeaders; i++) {// push all section content
        sectionHeaders64[i]->pushData(stream);
        padBytes(stream,0x200-(sectionHeaders64[i]->getSize()%(0x200)));
    }
}
Pe64SectionHandler *Pe64Handler::addSeg(const char name[8], uint32_t characteristics, const char *type) {
    Pe64SectionHandler *hdr = new Pe64SectionHandler(*this, name, characteristics, type);
    sectionHeaders64.push_back(hdr);
    return hdr;
}
void Pe64Handler::addImport(const std::string& name, const peHintNameTable& hint) {
    importNames.push_back(name);
    importHints.push_back(hint);
}
#pragma endregion// Pe64Handler
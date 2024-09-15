#include "PE.h"

#include <iostream>
#include <iomanip>
#include <string.h>

//utility func
uint32_t roundToAlign(const uint32_t &value, const uint32_t &align) {
    unsigned int mod = value%align;
    if (mod==0) return value;
    return value+align-mod;
}


#pragma region Pe64Handler
Pe64Handler::Pe64Handler() : peHeader(), peStdFieldsHeader(), peSpecFieldsHeader(), peDataDirHeader() {
    bitMode=64;
}
Pe64Handler::~Pe64Handler() {
    for (unsigned int i = 0; i < sectionHeaders64.size(); i++) delete sectionHeaders64[i];
}
void Pe64Handler::push(std::ofstream &stream) {
    // 100 bytes of MZ header
    pushDword(stream,0x4D5A9000u,false);pushDword(stream,0x03000000u,false);pushDword(stream,0x04000000u,false);pushDword(stream,0xFFFF0000u,false);// 0x00000010
    pushDword(stream,0xB8000000u,false);pushDword(stream,0x00000000u,false);pushDword(stream,0x40000000u,false);pushDword(stream,0x00000000u,false);// 0x00000020
    pushDword(stream,0x00000000u,false);pushDword(stream,0x00000000u,false);pushDword(stream,0x00000000u,false);pushDword(stream,0x00000000u,false);// 0x00000030
    pushDword(stream,0x00000000u,false);pushDword(stream,0x00000000u,false);pushDword(stream,0x00000000u,false);pushDword(stream,0x00000100u,true );// 0x00000040
    pushDword(stream,0x0E1FBA0Eu,false);pushDword(stream,0x00B409CDu,false);pushDword(stream,0x21B8014Cu,false);pushDword(stream,0xCD215468u,false);// 0x00000050
    pushDword(stream,0x69732070u,false);pushDword(stream,0x726F6772u,false);pushDword(stream,0x616D2063u,false);pushDword(stream,0x616E6E6Fu,false);// 0x00000060
    pushDword(stream,0x74206265u,false);pushDword(stream,0x2072756Eu,false);pushDword(stream,0x20696E20u,false);pushDword(stream,0x444F5320u,false);// 0x00000070
    pushDword(stream,0x6D6F6465u,false);pushDword(stream,0x2E0D0D0Au,false);pushDword(stream,0x24000000u,false);pushDword(stream,0x00000000u,false);// 0x00000080
    for (int i = 0; i < 8; i++) { pushDword(stream,0x00000000u,false);pushDword(stream,0x00000000u,false);pushDword(stream,0x00000000u,false);pushDword(stream,0x00000000u,false); }

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
    
    uint32_t runningOffset = baseOffset;
    uint32_t runningRVA = roundToAlign(baseOffset,SECTION_ALIGN);
    for (uint32_t i = 0; i < numHeaders; i++) {
        sectionHeaders64[i]->setOffset(runningOffset);
        sectionHeaders64[i]->setRVA(runningRVA);
        sectionHeaders64[i]->setSectionAlign(SECTION_ALIGN);
        sectionHeaders64[i]->setFileAlign(FILE_ALIGN);
        uint32_t sectionSize = sectionHeaders64[i]->getSize();
        uint32_t virtSize = roundToAlign(sectionSize,SECTION_ALIGN);
        uint32_t fileSize = roundToAlign(sectionSize,FILE_ALIGN);
        runningRVA += virtSize;
        
        if (sectionHeaders64[i]->isCode()) sizeOfCode+=fileSize;
        else {
            sizeOfInitializedData+=fileSize;
        }
        runningOffset += fileSize;
    }
    for (unsigned int i = 0; i < labelResolutions.size(); i++) {
        Pe64LabelResolution resolution = labelResolutions[i];
        bool found=false;
        for (unsigned int j = 0; j < labels.size(); j++) {
            Pe64Label label = labels[j];
            if (resolution.name.compare(label.name)==0) {
                found=true;
                setDwordAt(resolution.base->data,resolution.setAt,(label.base->getRVA()+label.offset)-(resolution.base->getRVA()+resolution.setAt)-resolution.relativeToOffset,PE_IS_LSB);
            }
        }
        if (!found) { std::cout << "label/variable definition could not be found" << std::endl; return; }
    }
    for (unsigned int i = 0; i < labels.size(); i++) if (labels[i].name.compare("_main")==0) { peStdFieldsHeader.p_addressOfEntryPoint=(labels[i].base->getRVA()+labels[i].offset); continue;}
    
    // create and fill import section
    unsigned int numImports=importNames.size();
    if (numImports!=0) {
        numHeaders++;
        Pe64SectionHandler *idataHdr = new Pe64SectionHandler(*this, ".idata  ",IMAGE_SCN_CNT_INITIALIZED_DATA|IMAGE_SCN_MEM_READ|IMAGE_SCN_MEM_WRITE);
        sectionHeaders64.push_back(idataHdr);
        idataHdr->setOffset(runningOffset);
        idataHdr->setRVA(runningRVA);
        idataHdr->setSectionAlign(SECTION_ALIGN);
        idataHdr->setFileAlign(FILE_ALIGN);
        
        uint32_t lookupTableRVA = runningRVA+((numImports+1)*sizeof(peImportDirTable));// add the size of the dir table to the address of the dir table
        uint32_t lookupAddressRVA = lookupTableRVA+((numImports+1)*sizeof(pe64ImportLookupTable));// add the size of the lookup table to the address of the lookup table
        uint32_t hintsRVA = lookupAddressRVA+((numImports+1)*sizeof(pe64ImportAddressTable));// add the size of the address table to the address of the address table
        uint32_t sizeofImportHintNameTable = 0;
        importHints.push_back(peHintNameTable(0,""));
        for (unsigned int i = 0; i <= numImports; i++) sizeofImportHintNameTable+=importHints[i].getSize();
        uint32_t stringsRVA = roundToAlign(hintsRVA+sizeofImportHintNameTable,4);
        
        uint32_t runningStringsRVA=stringsRVA;
        for (unsigned int i = 0; i < numImports; i++) {
            peImportDirTable(lookupTableRVA,runningStringsRVA,lookupAddressRVA).push(idataHdr->data);
            runningStringsRVA+=importNames[i].size();
        }
        peImportDirTable().push(idataHdr->data);

        for (unsigned int i = 0; i < numImports; i++) pe64ImportLookupTable(false,hintsRVA).push(idataHdr->data);
        pe64ImportLookupTable().push(idataHdr->data);

        for (unsigned int i = 0; i < numImports; i++) pe64ImportAddressTable(false,hintsRVA).push(idataHdr->data);
        pe64ImportAddressTable().push(idataHdr->data);

        for (unsigned int i = 0; i <= numImports; i++) importHints[i].push(idataHdr->data);
        
        padBytes(idataHdr->data,stringsRVA-(hintsRVA+sizeofImportHintNameTable));
        for (unsigned int i = 0; i < numImports; i++) {
            pushChars(idataHdr,(const uint8_t*)importNames[i].c_str(),importNames[i].size(),true);
        }
        padBytes(idataHdr->data,roundToAlign(runningStringsRVA,4)-runningStringsRVA);

        uint32_t sectionSize = idataHdr->getSize();
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
Pe64SectionHandler *Pe64Handler::addSeg(const char name[9], uint32_t characteristics) {
    Pe64SectionHandler *hdr = new Pe64SectionHandler(*this, name, characteristics);
    sectionHeaders64.push_back(hdr);
    return hdr;
}
void Pe64Handler::addImport(const uint16_t& hint, const std::string& hintName, const std::string& dllName) {
    importHints.push_back(peHintNameTable(hint,hintName.c_str()));
    importNames.push_back(dllName);
}
void Pe64Handler::defineLabel(const std::string& name, Pe64SectionHandler* base, const uint32_t& offset) {
    labels.push_back(Pe64Label(name,base,offset));
}
void Pe64Handler::resolveLabel(const std::string& name, Pe64SectionHandler* base, const uint32_t& setAt, const int32_t& relativeToOffset) {
    labelResolutions.push_back(Pe64LabelResolution(name,base,setAt,relativeToOffset));
}
#pragma endregion// Pe64Handler

#pragma region Pe64SectionHandler
template <>
void pushChars(Pe64SectionHandler*& reciever, const uint8_t* chars, const size_t& len, const bool& LSB) {
    pushChars(reciever->data, chars, len, LSB);
}
Pe64SectionHandler::Pe64SectionHandler(Pe64Handler &_peHandler, const char name[8], uint32_t characteristics) : peHandler(_peHandler), sectionHeader(name, characteristics) {
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
std::string Pe64SectionHandler::getName() {
    return this->sectionHeader.s_name;
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
uint32_t Pe64SectionHandler::getRVA() {
    return sectionHeader.s_virtualAddress;
};
void Pe64SectionHandler::defineLabel(const std::string& name) {
    peHandler.defineLabel(name,this,data.size());
}
void Pe64SectionHandler::resolveLabel(const std::string& name, const int32_t& relativeToOffset) {
    peHandler.resolveLabel(name,this,data.size()-relativeToOffset,relativeToOffset);
}
#pragma endregion// Pe64SectionHandler
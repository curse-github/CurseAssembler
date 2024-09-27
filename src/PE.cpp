#include "PE.h"

peSectionPlusOffset RVAtoSectionPlusOffset(const uint32_t& RVA, const uint32_t& size, std::vector<peSectionHdr>& sections) {
    for (size_t i = 0; i < sections.size(); i++) {
        if ((RVA>=sections[i].s_virtualAddress)&&((RVA+size)<=(sections[i].s_virtualAddress+sections[i].s_virtualSize))) {
            return peSectionPlusOffset {
                &sections[i],
                (int)i,
                RVA-sections[i].s_virtualAddress
            };
        }
    }
    return {nullptr,-1,0};
}

#pragma region struct print functions
void peOptHdrStdFields::print(std::vector<peSectionHdr>& sections, std::string delimmiter) {
    std::cout << "p_majorLinkerVersion: " << (int)p_majorLinkerVersion << delimmiter;
    std::cout << "p_minorLinkerVersion: " << (int)p_minorLinkerVersion << delimmiter;
    std::cout << "p_sizeOfCode: " << intToHex(p_sizeOfCode) << delimmiter;
    std::cout << "p_sizeOfInitializedData: " << intToHex(p_sizeOfInitializedData) << delimmiter;
    std::cout << "p_sizeOfUninitializedData: " << intToHex(p_sizeOfUninitializedData) << delimmiter;
    std::cout << "p_addressOfEntryPoint: " << intToHex(p_addressOfEntryPoint);
    if (p_addressOfEntryPoint!=0) {
        peSectionPlusOffset sectionPlusOffset = RVAtoSectionPlusOffset(p_addressOfEntryPoint,0,sections);
        if (sectionPlusOffset.section!=nullptr) std::cout << " -> <" << sectionPlusOffset.section->getName() << ">+" << intToHex(sectionPlusOffset.offset);
    }
    std::cout << delimmiter;
    std::cout << "p_baseOfCode: " << intToHex(p_baseOfCode) << '\n';
}
void peOptHdrDataDirs::print(std::vector<peSectionHdr>& sections, std::string delimmiter) {
    std::cout << "p_exportTableRVA: " << intToHex(p_exportTableRVA);
    if (p_exportTableRVA!=0) {
        peSectionPlusOffset sectionPlusOffset = RVAtoSectionPlusOffset(p_exportTableRVA,p_exportTableSize,sections);
        if (sectionPlusOffset.section!=nullptr) std::cout << " -> <" << sectionPlusOffset.section->getName() << ">+" << intToHex(sectionPlusOffset.offset);
    }
    std::cout << delimmiter;
    std::cout << "p_exportTableSize: " << intToHex(p_exportTableSize) << delimmiter;
    std::cout << "p_importTableRVA: " << intToHex(p_importTableRVA);
    if (p_importTableRVA!=0) {
        peSectionPlusOffset sectionPlusOffset = RVAtoSectionPlusOffset(p_importTableRVA,p_importTableSize,sections);
        if (sectionPlusOffset.section!=nullptr) std::cout << " -> <" << sectionPlusOffset.section->getName() << ">+" << intToHex(sectionPlusOffset.offset);
    }
    std::cout << delimmiter;
    std::cout << "p_importTableSize: " << intToHex(p_importTableSize) << delimmiter;
    std::cout << "p_resourceTableRVA: " << intToHex(p_resourceTableRVA);
    if (p_resourceTableRVA!=0) {
        peSectionPlusOffset sectionPlusOffset = RVAtoSectionPlusOffset(p_resourceTableRVA,p_resourceTableSize,sections);
        if (sectionPlusOffset.section!=nullptr) std::cout << " -> <" << sectionPlusOffset.section->getName() << ">+" << intToHex(sectionPlusOffset.offset);
    }
    std::cout << delimmiter;
    std::cout << "p_resourceTableSize: " << intToHex(p_resourceTableSize) << delimmiter;
    std::cout << "p_exceptionTableRVA: " << intToHex(p_exceptionTableRVA);
    if (p_exceptionTableRVA!=0) {
        peSectionPlusOffset sectionPlusOffset = RVAtoSectionPlusOffset(p_exceptionTableRVA,p_exceptionTableSize,sections);
        if (sectionPlusOffset.section!=nullptr) std::cout << " -> <" << sectionPlusOffset.section->getName() << ">+" << intToHex(sectionPlusOffset.offset);
    }
    std::cout << delimmiter;
    std::cout << "p_exceptionTableSize: " << intToHex(p_exceptionTableSize) << delimmiter;
    std::cout << "p_certificateTableRVA: " << intToHex(p_certificateTableRVA);
    if (p_certificateTableRVA!=0) {
        peSectionPlusOffset sectionPlusOffset = RVAtoSectionPlusOffset(p_certificateTableRVA,p_certificateTableSize,sections);
        if (sectionPlusOffset.section!=nullptr) std::cout << " -> <" << sectionPlusOffset.section->getName() << ">+" << intToHex(sectionPlusOffset.offset);
    }
    std::cout << delimmiter;
    std::cout << "p_certificateTableSize: " << intToHex(p_certificateTableSize) << delimmiter;
    std::cout << "p_baseRelocationTableRVA: " << intToHex(p_baseRelocationTableRVA);
    if (p_baseRelocationTableRVA!=0) {
        peSectionPlusOffset sectionPlusOffset = RVAtoSectionPlusOffset(p_baseRelocationTableRVA,p_baseRelocationTableSize,sections);
        if (sectionPlusOffset.section!=nullptr) std::cout << " -> <" << sectionPlusOffset.section->getName() << ">+" << intToHex(sectionPlusOffset.offset);
    }
    std::cout << delimmiter;
    std::cout << "p_baseRelocationTableSize: " << intToHex(p_baseRelocationTableSize) << delimmiter;
    std::cout << "p_debugRVA: " << intToHex(p_debugRVA);
    if (p_debugRVA!=0) {
        peSectionPlusOffset sectionPlusOffset = RVAtoSectionPlusOffset(p_debugRVA,p_debugSize,sections);
        if (sectionPlusOffset.section!=nullptr) std::cout << " -> <" << sectionPlusOffset.section->getName() << ">+" << intToHex(sectionPlusOffset.offset);
    }
    std::cout << delimmiter;
    std::cout << "p_debugSize: " << intToHex(p_debugSize) << delimmiter;
    std::cout << "p_architecture: " << intToHex(p_architecture) << delimmiter;
    std::cout << "p_globalPtrRVA: " << intToHex(p_globalPtrRVA);
    if (p_globalPtrRVA!=0) {
        peSectionPlusOffset sectionPlusOffset = RVAtoSectionPlusOffset(p_globalPtrRVA,p_globalPtrSize,sections);
        if (sectionPlusOffset.section!=nullptr) std::cout << " -> <" << sectionPlusOffset.section->getName() << ">+" << intToHex(sectionPlusOffset.offset);
    }
    std::cout << delimmiter;
    std::cout << "p_globalPtrSize: " << intToHex(p_globalPtrSize) << delimmiter;
    std::cout << "p_TlsTableRVA: " << intToHex(p_TlsTableRVA);
    if (p_TlsTableRVA!=0) {
        peSectionPlusOffset sectionPlusOffset = RVAtoSectionPlusOffset(p_TlsTableRVA,p_TlsTableSize,sections);
        if (sectionPlusOffset.section!=nullptr) std::cout << " -> <" << sectionPlusOffset.section->getName() << ">+" << intToHex(sectionPlusOffset.offset);
    }
    std::cout << delimmiter;
    std::cout << "p_TlsTableSize: " << intToHex(p_TlsTableSize) << delimmiter;
    std::cout << "p_loadConfigTableRVA: " << intToHex(p_loadConfigTableRVA);
    if (p_loadConfigTableRVA!=0) {
        peSectionPlusOffset sectionPlusOffset = RVAtoSectionPlusOffset(p_loadConfigTableRVA,p_loadConfigTableSize,sections);
        if (sectionPlusOffset.section!=nullptr) std::cout << " -> <" << sectionPlusOffset.section->getName() << ">+" << intToHex(sectionPlusOffset.offset);
    }
    std::cout << delimmiter;
    std::cout << "p_loadConfigTableSize: " << intToHex(p_loadConfigTableSize) << delimmiter;
    std::cout << "p_boundImportRVA: " << intToHex(p_boundImportRVA);
    if (p_boundImportRVA!=0) {
        peSectionPlusOffset sectionPlusOffset = RVAtoSectionPlusOffset(p_boundImportRVA,p_boundImportSize,sections);
        if (sectionPlusOffset.section!=nullptr) std::cout << " -> <" << sectionPlusOffset.section->getName() << ">+" << intToHex(sectionPlusOffset.offset);
    }
    std::cout << delimmiter;
    std::cout << "p_boundImportSize: " << intToHex(p_boundImportSize) << delimmiter;
    std::cout << "p_ImprtAddressTableRVA: " << intToHex(p_ImprtAddressTableRVA);
    if (p_ImprtAddressTableRVA!=0) {
        peSectionPlusOffset sectionPlusOffset = RVAtoSectionPlusOffset(p_ImprtAddressTableRVA,p_ImprtAddressTableSize,sections);
        if (sectionPlusOffset.section!=nullptr) std::cout << " -> <" << sectionPlusOffset.section->getName() << ">+" << intToHex(sectionPlusOffset.offset);
    }
    std::cout << delimmiter;
    std::cout << "p_ImprtAddressTableSize: " << intToHex(p_ImprtAddressTableSize) << delimmiter;
    std::cout << "p_delayImportDescriptorRVA: " << intToHex(p_delayImportDescriptorRVA);
    if (p_delayImportDescriptorRVA!=0) {
        peSectionPlusOffset sectionPlusOffset = RVAtoSectionPlusOffset(p_delayImportDescriptorRVA,p_delayImportDescriptorSize,sections);
        if (sectionPlusOffset.section!=nullptr) std::cout << " -> <" << sectionPlusOffset.section->getName() << ">+" << intToHex(sectionPlusOffset.offset);
    }
    std::cout << delimmiter;
    std::cout << "p_delayImportDescriptorSize: " << intToHex(p_delayImportDescriptorSize) << delimmiter;
    std::cout << "p_ClrRuntimeHeaderRVA: " << intToHex(p_ClrRuntimeHeaderRVA);
    if (p_ClrRuntimeHeaderRVA!=0) {
        peSectionPlusOffset sectionPlusOffset = RVAtoSectionPlusOffset(p_ClrRuntimeHeaderRVA,p_ClrRuntimeHeaderSize,sections);
        if (sectionPlusOffset.section!=nullptr) std::cout << " -> <" << sectionPlusOffset.section->getName() << ">+" << intToHex(sectionPlusOffset.offset);
    }
    std::cout << delimmiter;
    std::cout << "p_ClrRuntimeHeaderSize: " << intToHex(p_ClrRuntimeHeaderSize) << delimmiter;
    std::cout << "p_zero: " << intToHex(p_zero) << '\n';
}
#pragma endregion struct print functions

#pragma region Dll parsing
std::string getEnvVar( std::string const & key ) {
    char * val = getenv( key.c_str() );
    return val == NULL ? std::string("") : std::string(val);
}
std::vector<std::string> StringSplit(std::string input, char delimitter) {
    std::vector<std::string> tmpVec;
    std::string tmpStr = "";
    for (size_t i = 0; i < input.size(); i++) {
        if (input[i]==delimitter) {
            tmpVec.push_back(tmpStr);tmpStr="";
        } else tmpStr+=input[i];
    }
    if (tmpStr.size()>0) tmpVec.push_back(tmpStr);
    return tmpVec;
}
std::string getFullFilePath(const std::string& path) {
    if (path[0]=='.') {// relative path
        if (std::filesystem::exists(std::filesystem::path{path})) return path;
        else return "";
    } else if ((path[0]=='\\')||((path[0]>='A')&&(path[0]<='Z')&&(path[1]==':'))) {// absolute path
        if (std::filesystem::exists(std::filesystem::path{path})) return path;
        else return "";
    } else {// might be path on the PATH
        std::string tmpFullPath = std::string("./")+path;
        if (std::filesystem::exists(std::filesystem::path{tmpFullPath})) return tmpFullPath;
        std::vector<std::string> paths = StringSplit(getEnvVar("PATH"),';');
        for (size_t i = 0; i < paths.size(); i++) {
            tmpFullPath = paths[i]+'\\'+path;
            if (std::filesystem::exists(std::filesystem::path{tmpFullPath})) return tmpFullPath;
        }
        return "";
    }
}
std::vector<dllExportData> parseDll(const std::string& name) {
    // get file
    std::string fullPath = getFullFilePath(name);
    if (fullPath.size()==0) std::cout << "File not found.\n";
    //std::cout << "reading: \"" << fullPath << "\"\n";
    std::ifstream inFile(fullPath, std::ios::binary);
    if (inFile.bad()) { std::cout << "File ifStream bad.\n"; return {}; }
    uint32_t count = 0;

    peHdr peHeader;
    peOptHdrStdFields stdFieldsHeader;
    peOptHdrSpecFields64 specFieldsHeader;
    peOptHdrDataDirs dataDirs;
    uint16_t p_numberOfSections = 0;
    std::vector<peSectionHdr> sectionHeaders;
    std::vector<std::vector<uint8_t>> sectionData;
    peSectionPlusOffset exportTableLocation;
    peExportDirTable exportDirTable(0,0);
    std::string dllName;
    std::vector<dllExportData> exports;
    try {
        // read pe header and optional header
        peHdr::peHdrPullReturnData peHdrPullReturn = peHeader.read(inFile,count);
        if (!peHdrPullReturn.isValid) { inFile.close(); return {}; }
        if ((peHeader.p_characteristics&IMAGE_FILE_CHARACTERISTIC_DLL)==0) { std::cout << "File is not a DLL"; inFile.close(); return {}; }
        bool stdFieldsHdrPullReturn = stdFieldsHeader.read(inFile,count);
        if (!stdFieldsHdrPullReturn) { inFile.close(); return {}; }
        bool specFieldsHdrPullReturn = specFieldsHeader.read(inFile,count);
        if (!specFieldsHdrPullReturn) { inFile.close(); return {}; }
        bool dataDirsPullReturn = dataDirs.read(inFile,count);
        if (!dataDirsPullReturn) { inFile.close(); return {}; }
        // read section headers
        p_numberOfSections = peHeader.p_numberOfSections;
        for (size_t i = 0; i < p_numberOfSections; i++) {
            peSectionHdr sectionHdrI("        ");
            sectionHdrI.read(inFile,count);
            sectionHeaders.push_back(sectionHdrI);
        }
        // read section data
        for (size_t i = 0; i < p_numberOfSections; i++) {
            trashBytes(inFile,count,sectionHeaders[i].s_rawDataPointer-count);
            sectionData.push_back(readBytes(inFile, count, sectionHeaders[i].s_virtualSize));
        }
        // find export table
        if (dataDirs.p_exportTableRVA==0) { std::cout << "DLL has no export table."; inFile.close(); return {}; }
        exportTableLocation = RVAtoSectionPlusOffset(dataDirs.p_exportTableRVA,dataDirs.p_exportTableSize,sectionHeaders);
        if (exportTableLocation.section==nullptr) { std::cout << "DLL export table could not be found."; inFile.close(); return {}; }
        const peSectionHdr* sectionWithExportTable = exportTableLocation.section;
        std::vector<uint8_t>* sectionDataWithExportTable = &sectionData[exportTableLocation.index];
        exportDirTable.readAt(*sectionDataWithExportTable,exportTableLocation.offset);
        // parse data in export table
        dllName = readStringAt(*sectionDataWithExportTable,exportDirTable.e_exportNameTableRVA-sectionWithExportTable->s_virtualAddress);
        for (uint16_t i = 0; i < exportDirTable.e_numNamePointers; i++) {
            uint32_t namePointer = readDwordAt(*sectionDataWithExportTable,exportDirTable.e_namePointerTableRVA-sectionWithExportTable->s_virtualAddress+i*4);
            std::string name = readStringAt(*sectionDataWithExportTable,namePointer-sectionWithExportTable->s_virtualAddress);
            exports.push_back(dllExportData{i,name,dllName});
        }
        inFile.close();
        return exports;
    } catch(const int& err) { std::cout << "EOF reached before expected"; inFile.close(); return {}; }
    catch ( ... ) { std::cout << "error..."; inFile.close(); return {}; }
}
#pragma endregion Dll parsing

#pragma region Pe64Handler
Pe64Handler::Pe64Handler() : peHeader(), peStdFieldsHeader(), peSpecFieldsHeader(), peDataDirHeader() {
    bitMode=64;
}
Pe64Handler::~Pe64Handler() {
    for (unsigned int i = 0; i < sectionHeaders.size(); i++) delete sectionHeaders[i];
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

    // get all exports from the avaliable dlls that the user gave
    std::vector<dllExportData> allExports;
    std::vector<int32_t> exportImportIndex;
    const size_t numDlls = avaliableDlls.size();
    for (size_t i = 0; i < numDlls; i++) {
        std::vector<dllExportData> exports = parseDll(avaliableDlls[i]);
        const size_t numExports = exports.size();
        for (size_t j = 0; j < numExports; j++) { allExports.push_back(exports[j]); exportImportIndex.push_back(-1); }
    }
    const size_t numExports = allExports.size();
    // search though the sections of code accessing labels
    // if the label is found in this file, 
    // if it is not found within this code, search for it in the dll, and mark it to be imported
    // also save label indices if the label is found
    std::vector<int32_t> labelResIdxToLabelIdx;
    int32_t numImports=0;
    for (unsigned int i = 0; i < labelResolutions.size(); i++) {
        Pe64LabelResolution resolution = labelResolutions[i];
        bool found=false;
        for (unsigned int j = 0; j < labels.size(); j++) {
            Pe64Label label = labels[j];
            if (resolution.name.compare(label.name)==0) { found=true; labelResIdxToLabelIdx.push_back(j); break; }
        }
        if (!found) {// label was not found in code, search for it in dlls
            labelResIdxToLabelIdx.push_back(-1);
            for (size_t j = 0; j < numExports; j++) {
                if (allExports[j].name.compare(resolution.name)==0) {
                    found=true;
                    if (exportImportIndex[j]==-1) {
                        imports.push_back({allExports[j].namePtrIndex,&allExports[j].name,&allExports[j].dllName});
                        exportImportIndex[j]=numImports;
                        labelResIdxToLabelIdx[i]=labels.size()+numImports;
                        numImports++;
                    }
                }
            }
        }
        if (!found) { std::cout << "unresolved symbol " << resolution.name << " could not be found" << std::endl; return; }
    }

    uint16_t numHeaders = sectionHeaders.size();
    peHeader.p_numberOfSections = numHeaders+static_cast<uint16_t>(numImports>0);
    peHeader.p_sizeOfOptionalHeader = sizeof(peOptHdrStdFields) + sizeof(peOptHdrSpecFields64) + sizeof(peOptHdrDataDirs);
    peSpecFieldsHeader.p_sizeOfHeaders = sizeof(peHdr) + peHeader.p_sizeOfOptionalHeader + (numHeaders * sizeof(peSectionHdr)) + ((numImports>0)?sizeof(peSectionHdr):0);
    uint32_t baseOffset = roundToAlign(0x100 + peSpecFieldsHeader.p_sizeOfHeaders,FILE_ALIGN);
    peSpecFieldsHeader.p_sizeOfHeaders = roundToAlign(peSpecFieldsHeader.p_sizeOfHeaders,FILE_ALIGN);
    //checksum?

    uint32_t sizeOfInitializedData=0;
    uint32_t sizeOfUninitializedData=0;
    uint32_t sizeOfCode=0;
    
    uint32_t runningOffset = baseOffset;
    uint32_t runningRVA = roundToAlign(baseOffset,SECTION_ALIGN);
    for (uint32_t i = 0; i < numHeaders; i++) {
        sectionHeaders[i]->setOffset(runningOffset);
        sectionHeaders[i]->setRVA(runningRVA);
        sectionHeaders[i]->setSectionAlign(SECTION_ALIGN);
        sectionHeaders[i]->setFileAlign(FILE_ALIGN);
        uint32_t sectionSize = sectionHeaders[i]->getSize();
        uint32_t virtSize = roundToAlign(sectionSize,SECTION_ALIGN);
        uint32_t fileSize = roundToAlign(sectionSize,FILE_ALIGN);
        runningOffset += fileSize;
        runningRVA += virtSize;
        
        if (sectionHeaders[i]->isCode()) sizeOfCode+=fileSize;
        else {
            sizeOfInitializedData+=fileSize;
        }
    }
    // create and fill import section
    if (numImports!=0) {
        numHeaders++;
        Pe64SectionHandler *idataHandler = new Pe64SectionHandler(*this, ".idata  ",IMAGE_SCN_CNT_INITIALIZED_DATA|IMAGE_SCN_MEM_READ|IMAGE_SCN_MEM_WRITE);
        sectionHeaders.push_back(idataHandler);
        idataHandler->setOffset(runningOffset);
        idataHandler->setRVA(runningRVA);
        idataHandler->setSectionAlign(SECTION_ALIGN);
        idataHandler->setFileAlign(FILE_ALIGN);
        
        uint32_t importLookupTableRVA = runningRVA+((numImports+1)*sizeof(peImportDirTable));// add the size of the dir table to the address of the dir table
        uint32_t importAddressTableRVA = importLookupTableRVA+((numImports+1)*8);// add the size of the lookup table to the address of the lookup table
        uint32_t hintsRVA = importAddressTableRVA+((numImports+1)*8);// add the size of the address table to the address of the address table
        uint32_t sizeofImportHintNameTable = 3;
        for (unsigned int i = 0; i < numImports; i++) sizeofImportHintNameTable+=peHintNameTable(imports[i].hint,imports[i].name).getSize();
        uint32_t stringsRVA = roundToAlign(hintsRVA+sizeofImportHintNameTable,4);
        uint32_t runningStringsRVA=stringsRVA;
        for (unsigned int i = 0; i < numImports; i++) {
            peImportDirTable(importLookupTableRVA+i*8,runningStringsRVA,importAddressTableRVA+i*8).push(idataHandler->data);
            runningStringsRVA+=imports[i].dllName->size()+1;
        }
        peImportDirTable().push(idataHandler->data);
        
        // Import lookup table: The RVAs of each hint/name in the Hint/Name table, termiated with a 64bit 0
        uint32_t runningHintsRVA=hintsRVA;
        for (unsigned int i = 0; i < numImports; i++) {
            pushQword(idataHandler->data,runningHintsRVA,true);
            runningHintsRVA+=peHintNameTable(imports[i].hint,imports[i].name).getSize();
        }
        pushQword(idataHandler->data,0,true);

        // Import address table: 64bit 1s for each import with a 0 to mark the end, these get overridden in the binding phase and get overridden with the address to the actual code
        for (unsigned int i = 0; i < numImports; i++) {
            //std::cout << '"' << (*imports[i].name) << "\": " << intToHex(importAddressTableRVA+i*8) << '\n';
            idataHandler->defineLabel(*imports[i].name);
            pushQword(idataHandler->data,1,true);
        }
        pushQword(idataHandler->data,0,true);

        // Hint/Name table: 16 bit ordinal and name, with an empty ordinal and string(just null character) to terminate the list
        for (unsigned int i = 0; i < numImports; i++) peHintNameTable(imports[i].hint,imports[i].name).push(idataHandler->data);
        pushWord(idataHandler,0,true);
        pushByte(idataHandler,0);
        
        // Import names: push dll name for each import, referenced by the name pointer table
        padBytes(idataHandler->data,stringsRVA-(hintsRVA+sizeofImportHintNameTable));
        for (unsigned int i = 0; i < numImports; i++) {
            const std::string* name = imports[i].dllName;
            pushChars(idataHandler,(const uint8_t*)name->c_str(),name->size(),true);
            pushByte(idataHandler,0);
        }
        padBytes(idataHandler->data,roundToAlign(runningStringsRVA,4)-runningStringsRVA);

        uint32_t sectionSize = idataHandler->getSize();
        uint32_t virtSize = roundToAlign(sectionSize,SECTION_ALIGN);
        uint32_t fileSize = roundToAlign(sectionSize,FILE_ALIGN);
        peDataDirHeader.p_importTableRVA = runningRVA;
        peDataDirHeader.p_importTableSize = sectionSize;
        peDataDirHeader.p_ImprtAddressTableRVA = importAddressTableRVA;
        peDataDirHeader.p_ImprtAddressTableSize = ((numImports+1)*8);
        runningRVA += virtSize;
        sizeOfInitializedData+=fileSize;
        runningOffset += fileSize;
    }
    
    for (unsigned int i = 0; i < labelResolutions.size(); i++) {
        Pe64LabelResolution resolution = labelResolutions[i];
        Pe64Label label = labels[labelResIdxToLabelIdx[i]];
        setDwordAt(resolution.base->data,resolution.setAt,(label.base->getRVA()+label.offset)-(resolution.base->getRVA()+resolution.setAt)-resolution.relativeToOffset,true);
    }
    for (unsigned int i = 0; i < labels.size(); i++) if (labels[i].name.compare("_main")==0) { peStdFieldsHeader.p_addressOfEntryPoint=(labels[i].base->getRVA()+labels[i].offset); break;}

    peStdFieldsHeader.p_sizeOfCode = sizeOfCode;
    peStdFieldsHeader.p_sizeOfInitializedData = sizeOfInitializedData;
    peStdFieldsHeader.p_sizeOfUninitializedData = sizeOfUninitializedData;

    peStdFieldsHeader.p_baseOfCode = SECTION_ALIGN;
    peSpecFieldsHeader.p_sizeOfImage = runningRVA;
    // push all the data
    peHeader.push(stream);
    peStdFieldsHeader.push(stream);
    peSpecFieldsHeader.push(stream);
    peDataDirHeader.push(stream);
    for (unsigned int i = 0; i < numHeaders; i++) sectionHeaders[i]->pushHeader(stream);// push all sections headers
    padBytes(stream,baseOffset-(0x100 + sizeof(peHdr) + peHeader.p_sizeOfOptionalHeader + (numHeaders * sizeof(peSectionHdr))));
    for (unsigned int i = 0; i < numHeaders; i++) {// push all section content
        sectionHeaders[i]->pushData(stream);
        padBytes(stream,FILE_ALIGN-(sectionHeaders[i]->getSize()%(FILE_ALIGN)));
    }
}
Pe64SectionHandler *Pe64Handler::addSec(const char name[9], uint32_t characteristics) {
    Pe64SectionHandler *hdr = new Pe64SectionHandler(*this, name, characteristics);
    sectionHeaders.push_back(hdr);
    return hdr;
}
void Pe64Handler::defineLabel(const std::string& name, Pe64SectionHandler* base, const uint32_t& offset) {
    labels.push_back(Pe64Label(name,base,offset));
}
void Pe64Handler::resolveLabel(const std::string& name, Pe64SectionHandler* base, const uint32_t& setAt, const int32_t& relativeToOffset) {
    labelResolutions.push_back(Pe64LabelResolution(name,base,setAt,relativeToOffset));
}
void Pe64Handler::addImport(const std::string& dllName) {
    avaliableDlls.push_back(dllName);
}
#pragma endregion// Pe64Handler

#pragma region Pe64SectionHandler
template <>
void pushChars(Pe64SectionHandler*& reciever, const uint8_t* chars, const size_t& len, const bool& LSB) {
    pushChars(reciever->data, chars, len, LSB);
}
Pe64SectionHandler::Pe64SectionHandler(Pe64Handler &_peHandler, const char name[8], uint32_t characteristics) : peHandler(_peHandler), sectionHeader(name) {
    sectionHeader.s_characteristics=characteristics;
}
void Pe64SectionHandler::pushHeader(std::ofstream &stream) {
    // set some stuff first
    size_t size = data.size();
    sectionHeader.s_virtualSize = size;
    sectionHeader.s_rawDataSize = roundToAlign(size,fileAlignment);
    sectionHeader.push(stream);
}
void Pe64SectionHandler::pushData(std::ofstream &stream) {
    pushChars(stream, data);
}
uint32_t Pe64SectionHandler::getSize() {
    return data.size();
}
// alignments stuff
void Pe64SectionHandler::setSectionAlign(const uint32_t& align) {
    sectionAlignment=align;
}
void Pe64SectionHandler::setFileAlign(const uint32_t& align) {
    fileAlignment=align;
}
// offset stuff
void Pe64SectionHandler::setOffset(const uint32_t &offset) {
    sectionHeader.s_rawDataPointer = offset;// offset in file
}
uint32_t Pe64SectionHandler::getOffset() {
    return sectionHeader.s_rawDataPointer;
}
// RVA stuff
void Pe64SectionHandler::setRVA(const uint32_t &Rva) {
    sectionHeader.s_virtualAddress = Rva;// offset in memory, can be different if you have uninitialized data
}
uint32_t Pe64SectionHandler::getRVA() {
    return sectionHeader.s_virtualAddress;
};
// Label stuff
void Pe64SectionHandler::defineLabel(const std::string& name) {
    peHandler.defineLabel(name,this,data.size());
}
void Pe64SectionHandler::resolveLabel(const std::string& name, const int32_t& relativeToOffset) {
    peHandler.resolveLabel(name,this,data.size()-relativeToOffset,relativeToOffset);
}
#pragma endregion// Pe64SectionHandler
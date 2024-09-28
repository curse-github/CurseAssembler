#include "ELF.h"

#pragma region section printing func
elfSectionPlusOffset RVAtoSectionPlusOffset(const uint32_t& RVA, const uint32_t& size, std::vector<elfSectionHdr64>& sections) {
    for (size_t i = 0; i < sections.size(); i++) {
        if ((RVA>=sections[i].s_virtualAddress)&&((RVA+size)<=(sections[i].s_virtualAddress+sections[i].s_size))) {
            return elfSectionPlusOffset {
                &sections[i],
                (int)i,
                (uint32_t)(RVA-sections[i].s_virtualAddress)
            };
        }
    }
    return {nullptr,-1,0};
}
elfSectionPlusOffset fileOffsettoSectionPlusOffset(const uint32_t& offset, const uint32_t& size, std::vector<elfSectionHdr64>& sections) {
    for (size_t i = 0; i < sections.size(); i++) {
        if ((offset>=sections[i].s_fileOffset)&&((offset+size)<=(sections[i].s_fileOffset+sections[i].s_size))) {
            return elfSectionPlusOffset {
                &sections[i],
                (int)i,
                (uint32_t)(offset-sections[i].s_fileOffset)
            };
        }
    }
    return {nullptr,-1,0};
}

void elf64Dyn::print(std::vector<uint8_t>& file, const uint32_t& exportStrTabOffset, std::vector<elfSectionHdr64>& sections, const uint32_t& secNameStrTableOffset) {
    if (dy_tag==ELF_DYN_TAG_NULL)
        std::cout << "{ tag: NONE/NULL, val/ptr: " << intToHex(dy_valPtr) << " }\n";
    else if (dy_tag==ELF_DYN_TAG_NEEDED)
        std::cout << "{ type: NEEDED, namePtr: " << dy_valPtr << " -> \"" << readStringAt(file,exportStrTabOffset+dy_valPtr) << "\" }\n";
    else if (dy_tag==ELF_DYN_TAG_PLTRELSZ)
        std::cout << "{ type: PLTRELSZ, table size: " << dy_valPtr << " bytes }\n";
    else if (dy_tag==ELF_DYN_TAG_PLTGOT) {
        std::cout << "{ type: PLTGOT, ptr: " << intToHex(dy_valPtr);
        if (dy_valPtr!=0) {
            elfSectionPlusOffset sectionPlusOffset = RVAtoSectionPlusOffset(dy_valPtr,1,sections);
            if ((sectionPlusOffset.section!=nullptr)&&(sectionPlusOffset.section->s_nameIdx!=0)) std::cout << " -> <" << readStringAt(file,secNameStrTableOffset+sectionPlusOffset.section->s_nameIdx) << ">+" << intToHex(sectionPlusOffset.offset);
        }
        std::cout << " }\n";
    } else if (dy_tag==ELF_DYN_TAG_HASH)
        std::cout << "{ type: HASH, ptr: " << intToHex(dy_valPtr) << " }\n";
    else if (dy_tag==ELF_DYN_TAG_STRTAB) {
        std::cout << "{ type: STRTAB, ptr: " << intToHex(dy_valPtr);
        if (dy_valPtr!=0) {
            elfSectionPlusOffset sectionPlusOffset = RVAtoSectionPlusOffset(dy_valPtr,1,sections);
            if ((sectionPlusOffset.section!=nullptr)&&(sectionPlusOffset.section->s_nameIdx!=0)) std::cout << " -> <" << readStringAt(file,secNameStrTableOffset+sectionPlusOffset.section->s_nameIdx) << ">+" << intToHex(sectionPlusOffset.offset);
        }
        std::cout << " }\n";
    } else if (dy_tag==ELF_DYN_TAG_SYMTAB) {
        std::cout << "{ type: SYMTAB, ptr: " << intToHex(dy_valPtr);
        if (dy_valPtr!=0) {
            elfSectionPlusOffset sectionPlusOffset = RVAtoSectionPlusOffset(dy_valPtr,1,sections);
            if ((sectionPlusOffset.section!=nullptr)&&(sectionPlusOffset.section->s_nameIdx!=0)) std::cout << " -> <" << readStringAt(file,secNameStrTableOffset+sectionPlusOffset.section->s_nameIdx) << ">+" << intToHex(sectionPlusOffset.offset);
        }
        std::cout << " }\n";
    } else if (dy_tag==ELF_DYN_TAG_RELA)
        std::cout << "{ type: RELA, ptr: " << intToHex(dy_valPtr) << " }\n";
    else if (dy_tag==ELF_DYN_TAG_RELASZ)
        std::cout << "{ type: RELASZ, table size: " << dy_valPtr << " bytes }\n";
    else if (dy_tag==ELF_DYN_TAG_RELAENT)
        std::cout << "{ type: RELAENT, entry size: " << dy_valPtr << " bytes }\n";
    else if (dy_tag==ELF_DYN_TAG_STRSZ)
        std::cout << "{ type: STRSZ, table size: " << dy_valPtr << " bytes }\n";
    else if (dy_tag==ELF_DYN_TAG_SYMENT)
        std::cout << "{ type: SYMENT, entry size: " << dy_valPtr << " bytes }\n";
    else if (dy_tag==ELF_DYN_TAG_SYMBOLIC)
        std::cout << "{ type: SYMBOLIC }\n";
    else if (dy_tag==ELF_DYN_TAG_REL)
        std::cout << "{ type: REL, ptr: " << intToHex(dy_valPtr) << " }\n";
    else if (dy_tag==ELF_DYN_TAG_RELSZ)
        std::cout << "{ type: RELSZ, table size: " << dy_valPtr << " bytes }\n";
    else if (dy_tag==ELF_DYN_TAG_RELENT)
        std::cout << "{ type: RELENT, entry size: " << dy_valPtr << " bytes }\n";
    else if (dy_tag==ELF_DYN_TAG_PLTREL) {
        std::cout << "{ type: PLTREL, val: " << dy_valPtr;
        if (dy_valPtr==ELF_DYN_TAG_RELA) std::cout << " -> RELA }\n";
        else if (dy_valPtr==ELF_DYN_TAG_REL) std::cout << " -> REL }\n";
        else std::cout << " }\n";
    } else if (dy_tag==ELF_DYN_TAG_DEBUG)
        std::cout << "{ type: DEBUG, ptr: " << intToHex(dy_valPtr) << " }\n";
    else if (dy_tag==ELF_DYN_TAG_TEXTREL)
        std::cout << "{ type: TEXTREL }\n";
    else if (dy_tag==ELF_DYN_TAG_JMPREL) {
        std::cout << "{ type: JMPREL, ptr: " << intToHex(dy_valPtr);
        if (dy_valPtr!=0) {
            elfSectionPlusOffset sectionPlusOffset = RVAtoSectionPlusOffset(dy_valPtr,1,sections);
            if ((sectionPlusOffset.section!=nullptr)&&(sectionPlusOffset.section->s_nameIdx!=0)) std::cout << " -> <" << readStringAt(file,secNameStrTableOffset+sectionPlusOffset.section->s_nameIdx) << ">+" << intToHex(sectionPlusOffset.offset);
        }
        std::cout << " }\n";
    } else if (dy_tag==ELF_DYN_TAG_FLAGS)
        std::cout << "{ type: FLAGS, val: " << intToHex(dy_valPtr) << " }\n";
    else if (dy_tag==ELF_DYN_TAG_ENCODING)
        std::cout << "{ type: ENCODING, val/ptr: " << intToHex(dy_valPtr) << " }\n";
    else if (dy_tag==ELF_DYN_TAG_DT_VALRNGLO)
        std::cout << "{ type: VALRNGLO, val/ptr: " << intToHex(dy_valPtr) << " }\n";
    else if (dy_tag==ELF_DYN_TAG_CHECKSUM)
        std::cout << "{ type: CHECKSUM, val: " << intToHex(dy_valPtr) << " }\n";
    else if (dy_tag==ELF_DYN_TAG_PLTPADSZ)
        std::cout << "{ type: PLTPADSZ, size: " << dy_valPtr << " bytes }\n";
    else if (dy_tag==ELF_DYN_TAG_MOVEENT)
        std::cout << "{ type: MOVEENT, entry size: " << dy_valPtr << " bytes }\n";
    else if (dy_tag==ELF_DYN_TAG_MOVESZ)
        std::cout << "{ type: MOVESZ, table size: " << dy_valPtr << " bytes }\n";
    else if (dy_tag==ELF_DYN_TAG_SYMINSZ)
        std::cout << "{ type: SYMINSZ, table size: " << dy_valPtr << " bytes }\n";
    else if (dy_tag==ELF_DYN_TAG_SYMINENT)
        std::cout << "{ type: SYMINENT, entry size: " << dy_valPtr << " bytes }\n";
    else if (dy_tag==ELF_DYN_TAG_VALRNGHI)
        std::cout << "{ type: VALRNGHI, val/ptr: " << intToHex(dy_valPtr) << " }\n";
    else if (dy_tag==ELF_DYN_TAG_ADDRRNGLO)
        std::cout << "{ type: ADDRRNGLO, val/ptr: " << intToHex(dy_valPtr) << " }\n";
    else if (dy_tag==ELF_DYN_TAG_CONFIG)
        std::cout << "{ type: CONFIG, ptr: " << intToHex(dy_valPtr) << " }\n";
    else if (dy_tag==ELF_DYN_TAG_DEPAUDIT)
        std::cout << "{ type: DEPAUDIT, ptr: " << intToHex(dy_valPtr) << " }\n";
    else if (dy_tag==ELF_DYN_TAG_AUDIT)
        std::cout << "{ type: AUDIT, ptr: " << intToHex(dy_valPtr) << " }\n";
    else if (dy_tag==ELF_DYN_TAG_PLTPAD)
        std::cout << "{ type: PLTPAD, ptr: " << intToHex(dy_valPtr) << " }\n";
    else if (dy_tag==ELF_DYN_TAG_MOVETAB) {
        std::cout << "{ type: MOVETAB, ptr: " << intToHex(dy_valPtr);
        if (dy_valPtr!=0) {
            elfSectionPlusOffset sectionPlusOffset = RVAtoSectionPlusOffset(dy_valPtr,1,sections);
            if ((sectionPlusOffset.section!=nullptr)&&(sectionPlusOffset.section->s_nameIdx!=0)) std::cout << " -> <" << readStringAt(file,secNameStrTableOffset+sectionPlusOffset.section->s_nameIdx) << ">+" << intToHex(sectionPlusOffset.offset);
        }
        std::cout << " }\n";
    } else if (dy_tag==ELF_DYN_TAG_SYMINFO)
        std::cout << "{ type: SYMINFO, ptr: " << intToHex(dy_valPtr) << " }\n";
    else if (dy_tag==ELF_DYN_TAG_ADDRRNGHI)
        std::cout << "{ type: ADDRRNGHI, val/ptr: " << intToHex(dy_valPtr) << " }\n";
    else if (dy_tag==ELF_DYN_TAG_RELACOUNT)
        std::cout << "{ type: RELACOUNT, val: " << intToHex(dy_valPtr) << " }\n";
    else if (dy_tag==ELF_DYN_TAG_RELCOUNT)
        std::cout << "{ type: RELCOUNT, val: " << intToHex(dy_valPtr) << " }\n";
    else if (dy_tag==ELF_DYN_TAG_VERDEF) {
        std::cout << "{ type: VERDEF, ptr: " << intToHex(dy_valPtr);
        if (dy_valPtr!=0) {
            elfSectionPlusOffset sectionPlusOffset = RVAtoSectionPlusOffset(dy_valPtr,1,sections);
            if ((sectionPlusOffset.section!=nullptr)&&(sectionPlusOffset.section->s_nameIdx!=0)) std::cout << " -> <" << readStringAt(file,secNameStrTableOffset+sectionPlusOffset.section->s_nameIdx) << ">+" << intToHex(sectionPlusOffset.offset);
        }
        std::cout << " }\n";
    } else if (dy_tag==ELF_DYN_TAG_VERDEFNUM)
        std::cout << "{ type: VERDEFNUM, num: " << dy_valPtr << " }\n";
    else if (dy_tag==ELF_DYN_TAG_VERNEED) {
        std::cout << "{ type: VERNEED, ptr: " << intToHex(dy_valPtr);
        if (dy_valPtr!=0) {
            elfSectionPlusOffset sectionPlusOffset = RVAtoSectionPlusOffset(dy_valPtr,1,sections);
            if ((sectionPlusOffset.section!=nullptr)&&(sectionPlusOffset.section->s_nameIdx!=0)) std::cout << " -> <" << readStringAt(file,secNameStrTableOffset+sectionPlusOffset.section->s_nameIdx) << ">+" << intToHex(sectionPlusOffset.offset);
        }
        std::cout << " }\n";
    } else if (dy_tag==ELF_DYN_TAG_VERNEEDNUM)
        std::cout << "{ type: VERNEEDNUM, num: " << dy_valPtr << " }\n";
    else
        //std::cout << "{ tag: " << intToHex(dy_tag) << ", val/ptr: " << intToHex(dy_valPtr) << " }\n";
        std::cout << "{ }\n";
}
void elf64Rela::print(std::vector<uint8_t>& file, std::vector<elfSectionHdr64>& sections, const uint32_t& secNameStrTableOffset, std::string delimmiter) {
    std::cout << "{" << delimmiter;
    std::cout << "    offset: " << intToHex(r_offset) << " -> ";
    if (r_offset!=0) {
        elfSectionPlusOffset sectionPlusOffset = RVAtoSectionPlusOffset(r_offset,1,sections);
        if ((sectionPlusOffset.section!=nullptr)&&(sectionPlusOffset.section->s_nameIdx!=0)) std::cout << '<' << readStringAt(file,secNameStrTableOffset+sectionPlusOffset.section->s_nameIdx) << ">+" << intToHex(sectionPlusOffset.offset);
        else std::cout << "section not found";
    } else std::cout << "section not found";
    std::cout << "," << delimmiter;
    std::cout << "    SYMTAB index: " << intToHex(r_symtabIndex) << "," << delimmiter;
    std::cout << "    type: " << intToHex(r_type) << " -> ";
    if (r_type==ELF64_REL_TYPE_AMD64_NONE) std::cout << "AMD64_NONE";
    else if (r_type==ELF64_REL_TYPE_AMD64_64) std::cout << "AMD64_64";
    else if (r_type==ELF64_REL_TYPE_AMD64_PC32) std::cout << "AMD64_PC32";
    else if (r_type==ELF64_REL_TYPE_AMD64_GOT32) std::cout << "AMD64_GOT32";
    else if (r_type==ELF64_REL_TYPE_AMD64_PLT32) std::cout << "AMD64_PLT32";
    else if (r_type==ELF64_REL_TYPE_AMD64_GLOB_DAT) std::cout << "AMD64_GLOB_DAT";
    else if (r_type==ELF64_REL_TYPE_AMD64_JUMP_SLOT) std::cout << "AMD64_JUMP_SLOT";
    else if (r_type==ELF64_REL_TYPE_AMD64_RELATIVE) std::cout << "AMD64_RELATIVE";
    else if (r_type==ELF64_REL_TYPE_AMD64_32) std::cout << "AMD64_32";
    else if (r_type==ELF64_REL_TYPE_AMD64_32s) std::cout << "AMD64_32s";
    else if (r_type==ELF64_REL_TYPE_AMD64_PC64) std::cout << "AMD64_PC64";
    else if (r_type==ELF64_REL_TYPE_AMD64_GOTOFF64) std::cout << "AMD64_GOTOFF64";
    else if (r_type==ELF64_REL_TYPE_AMD64_GOTPC32) std::cout << "AMD64_GOTPC32";
    else if (r_type==ELF64_REL_TYPE_AMD64_SIZE32) std::cout << "AMD64_SIZE32";
    else if (r_type==ELF64_REL_TYPE_AMD64_SIZE64) std::cout << "AMD64_SIZE64";
    else std::cout << "unknown";
    std::cout << "," << delimmiter;
    std::cout << "    addend: " << intToHex(r_addend) << delimmiter << "}\n";
}
#pragma endregion section printing func

#pragma region So parsing
std::vector<soExportData> parseSo(const std::string& name) {
    // get file
    std::string fullPath = getFullFilePath(name);
    if (fullPath.size()==0) std::cout << "File not found.\n";
    std::cout << "reading: \"" << fullPath << "\"\n";
    std::ifstream inFile(fullPath, std::ios::binary);
    if (inFile.bad()) { std::cout << "File ifStream bad.\n"; inFile.close(); return {}; }
    uint32_t count = 0;

    std::vector<uint8_t> allData;
    elfHdr64 elfHeader;
    uint16_t e_numSectionHdrs = 0;
    std::vector<elfSectionHdr64> sectionHeaders;
    uint32_t secNameStrTableOffset = 0;
    std::vector<soExportData> exports;
    try {
        // read elf header
        pushChars(allData,readBytes(inFile,count,sizeof(elfHdr64)));
        if (!elfHeader.readAt(allData,0)) { inFile.close(); return {}; }
        pushChars(allData,readBytes(inFile,count,elfHeader.e_segmentHdrOffset-count));
        // read section headers
        e_numSectionHdrs=elfHeader.e_numSectionHdrs;
        if (e_numSectionHdrs==0) { std::cout << "File has no sections.\n"; inFile.close(); return {}; }
        const uint32_t sectionHdrStart = elfHeader.e_sectionHdrOffset;
        const uint32_t sectionHdrEnd = sectionHdrStart+sizeof(elfSectionHdr64)*e_numSectionHdrs;
        if (sectionHdrEnd>count) pushChars(allData,readBytes(inFile,count,sectionHdrEnd-count));
        for (uint16_t i = 0; i < e_numSectionHdrs; i++) {
            elfSectionHdr64 sectionHdrI(0,0,0);
            if (!sectionHdrI.readAt(allData,sectionHdrStart+sizeof(elfSectionHdr64)*i)) { std::cout << "error reading section header #" << i << "\n"; inFile.close(); return {}; }
            sectionHeaders.push_back(sectionHdrI);
        }
        secNameStrTableOffset=sectionHeaders[elfHeader.e_stringTableNdx].s_fileOffset;
        // loop through section headers to find dynsym section
        for (size_t i = 0; i < e_numSectionHdrs; i++) {
            // parse dynsym entries
            if (sectionHeaders[i].s_type==ELF_SECTION_TYPE_DYNSYM) {elfSectionHdr64 exportStrTabSec = sectionHeaders[sectionHeaders[i].s_link];
                uint32_t sectionStart = sectionHeaders[i].s_fileOffset;
                uint32_t sectionEnd = sectionStart+sectionHeaders[i].s_size;
                if (sectionEnd>count) pushChars(allData,readBytes(inFile,count,sectionEnd-count));// read in the bytes if it does not have enough
                uint32_t numEntries = sectionHeaders[i].s_size/sizeof(elf64Symbol);
                for (uint32_t j = 0; j < numEntries; j++) {
                    elf64Symbol symI;
                    if (!symI.readAt(allData,sectionStart+sizeof(elf64Symbol)*j)) std::cout << "error reading symbol in .so file";
                    elfSectionHdr64 exportStrTabSec = sectionHeaders[sectionHeaders[i].s_link];
                    if ((symI.sy_typeNbinding&0x0F)==ELF_SYMBOL_TYPE_FUNC)
                        exports.push_back({j,readStringAt(allData,exportStrTabSec.s_fileOffset+symI.sy_name_idx),name});
                }
            }
        }
        return exports;
    } catch(const int& err) { std::cout << "EOF reached before expected"; inFile.close(); return {}; }
    catch ( ... ) { std::cout << "error..."; inFile.close(); return {}; }
};
#pragma endregion So parsing

uint8_t elf_encoding;
uint16_t elf_type;
uint8_t elf_osAbi;
uint16_t elf_machine;

#pragma region elfHandler
Elf64Handler::Elf64Handler() : elfHeader() {
    bitMode=64;
    sectionStrTab="";
    sectionStrTab.push_back('\0');
    curStrTabOffset=1;
}
void Elf64Handler::push(std::ofstream &stream) {
    // get all exports from the avaliable dlls that the user gave
    std::vector<soExportData> allExports;
    std::vector<int32_t> exportImportIndex;
    const size_t numDlls = avaliableSos.size();
    for (size_t i = 0; i < numDlls; i++) {
        std::vector<soExportData> exports = parseSo(avaliableSos[i]);
        const size_t numExports = exports.size();
        for (size_t j = 0; j < numExports; j++) { allExports.push_back(exports[j]); exportImportIndex.push_back(-1); }
    }
    const size_t numExports = allExports.size();
    // search though the segments of code accessing labels
    // if the label is found in this file, 
    // if it is not found within this code, search for it in the dll, and mark it to be imported
    // also save label indices if the label is found
    std::vector<int32_t> labelResIdxToLabelIdx;
    int32_t numImports=0;
    for (unsigned int i = 0; i < labelResolutions.size(); i++) {
        Elf64LabelResolution resolution = labelResolutions[i];
        bool found=false;
        for (unsigned int j = 0; j < labels.size(); j++) {
            Elf64Label label = labels[j];
            if (resolution.name.compare(label.name)==0) { found=true; labelResIdxToLabelIdx.push_back(j); break; }
        }
        if (!found) {// label was not found in code, search for it in dlls
            labelResIdxToLabelIdx.push_back(-1);
            for (size_t j = 0; j < numExports; j++) {
                if (allExports[j].name.compare(resolution.name)==0) {
                    found=true;
                    if (exportImportIndex[j]==-1) {
                        imports.push_back({allExports[j].symTabIndex,&allExports[j].name,&allExports[j].soName});
                        exportImportIndex[j]=numImports;
                        labelResIdxToLabelIdx[i]=labels.size()+numImports;
                        numImports++;
                    }
                }
            }
        }
        if (!found) { std::cout << "unresolved symbol " << resolution.name << " could not be found" << std::endl; return; }
    }
    if (numImports>0) {
        //Elf64SegmentHandler *dynamicSegment = new Elf64SegmentHandler(*this, ELF_SEGMENT_TYPE_DYN, IMAGE_SCN_MEM_READ|IMAGE_SCN_MEM_WRITE);
        segmentHeaders.push_back(new Elf64SegmentHandler(*this, ELF_SEGMENT_TYPE_DYN, IMAGE_SCN_MEM_READ|IMAGE_SCN_MEM_WRITE));
    }

    // set some beginning stuff
    uint16_t numHeaders = segmentHeaders.size();
    elfHeader.e_segmentHdrOffset=sizeof(elfHdr64);
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

    // create and fill .dynamic, .dynstr, .dynsym, .plt.got, and .rela.plt sections within a DYNAMIC type segment
    if (numImports>0) {
        Elf64SegmentHandler *dynamicSegment = segmentHeaders[segmentHeaders.size()-1];
        
        elfSectionHdr64* shStrTabSec = addSec(".shstrtab",ELF_SECTION_TYPE_STRTAB,0);
        elfHeader.e_stringTableNdx=0;
        elfSectionHdr64* dynStrTabSec = addSec(".dynstrtab",ELF_SECTION_TYPE_STRTAB,0);
        std::vector<uint32_t> dynStrTabIndexes;
        elfSectionHdr64* gotPltSec = addSec(".got.plt",ELF_SECTION_TYPE_PROGBITS,ELF_SECTION_FLAG_WRITE|ELF_SECTION_FLAG_ALLOC);
        elfSectionHdr64* relaPltSec = addSec(".rela.plt",ELF_SECTION_TYPE_RELA,ELF_SECTION_FLAG_ALLOC);
        elfSectionHdr64* dynSymSec = addSec(".dynsym",ELF_SECTION_TYPE_DYNSYM,ELF_SECTION_FLAG_ALLOC);
        dynSymSec->s_link=1;
        elfSectionHdr64* dynamicSec = addSec(".dynamic",ELF_SECTION_TYPE_DYNAMIC,ELF_SECTION_FLAG_WRITE|ELF_SECTION_FLAG_ALLOC);
        dynamicSec->s_link=1;
        uint32_t align = 0x100;

        // .shstrtab section
        shStrTabSec->s_virtualAddress=runningRVA;
        shStrTabSec->s_fileOffset=runningOffset;
        // push section header string table
        shStrTabSec->s_size=sectionStrTab.size();
        for (size_t i = 0; i < shStrTabSec->s_size; i++) {
            pushByte(dynamicSegment,sectionStrTab[i]);
        }
        // add padding to vector, runningOffset, and runningRVA
        runningRVA+=shStrTabSec->s_size; runningOffset+=shStrTabSec->s_size;
        padBytes(dynamicSegment->data,roundToAlign(runningOffset,align)-runningOffset);
        runningRVA=roundToAlign(runningRVA,align); runningOffset=roundToAlign(runningOffset,align);

        // .dynstrtab section
        dynStrTabSec->s_virtualAddress=runningRVA;
        dynStrTabSec->s_fileOffset=runningOffset;
        // push dynamic string table
        dynStrTabIndexes.push_back(dynStrTabSec->s_size);
        dynStrTabSec->s_size=1;
        pushByte(dynamicSegment,'\0');
        // push import names
        for (size_t i = 0; i < numImports; i++) {
            std::string nm = *imports[i].name;
            dynStrTabIndexes.push_back(dynStrTabSec->s_size);
            for (size_t j = 0; j < nm.size(); j++) pushByte(dynamicSegment,nm[j]);
            pushByte(dynamicSegment,'\0');
            dynStrTabSec->s_size+=nm.size()+1;
            dynStrTabIndexes.push_back(dynStrTabSec->s_size);
        }
        // push so names
        for (size_t i = 0; i < numImports; i++) {
            std::string nm = *imports[i].soName;
            dynStrTabIndexes.push_back(dynStrTabSec->s_size);
            for (size_t j = 0; j < nm.size(); j++) pushByte(dynamicSegment,nm[j]);
            pushByte(dynamicSegment,'\0');
            dynStrTabSec->s_size+=nm.size()+1;
        }
        // add padding to vector, runningOffset, and runningRVA
        runningRVA+=dynStrTabSec->s_size; runningOffset+=dynStrTabSec->s_size;
        padBytes(dynamicSegment->data,roundToAlign(runningOffset,align)-runningOffset);
        runningRVA=roundToAlign(runningRVA,align); runningOffset=roundToAlign(runningOffset,align);

        // .got.plt section
        gotPltSec->s_virtualAddress=runningRVA;
        gotPltSec->s_fileOffset=runningOffset;
        gotPltSec->s_entSize=8;
        gotPltSec->s_size=8+numImports*8;
        pushQword(dynamicSegment,0,true);
        // push got entries, which are just 64 bit numbers
        for (size_t i = 0; i < numImports; i++) {
            dynamicSegment->defineLabel(*imports[i].name);
            pushQword(dynamicSegment,0,true);
        }
        // add padding to vector, runningOffset, and runningRVA
        runningRVA+=gotPltSec->s_size; runningOffset+=gotPltSec->s_size;
        padBytes(dynamicSegment->data,roundToAlign(runningOffset,align)-runningOffset);
        runningRVA=roundToAlign(runningRVA,align); runningOffset=roundToAlign(runningOffset,align);

        // .rela.plt section
        relaPltSec->s_virtualAddress=runningRVA;
        relaPltSec->s_fileOffset=runningOffset;
        relaPltSec->s_size=numImports*sizeof(elf64Rela);
        // push rela entries
        for (size_t i = 0; i < numImports; i++) {
            elf64Rela rela;
            rela.r_offset=gotPltSec->s_virtualAddress+i*8;
            rela.r_symtabIndex=imports[i].symTabIndex;
            rela.r_type=ELF64_REL_TYPE_AMD64_JUMP_SLOT;
            rela.push(dynamicSegment->data);
        }
        // add padding to vector, runningOffset, and runningRVA
        runningRVA+=relaPltSec->s_size; runningOffset+=relaPltSec->s_size;
        padBytes(dynamicSegment->data,roundToAlign(runningOffset,align)-runningOffset);
        runningRVA=roundToAlign(runningRVA,align); runningOffset=roundToAlign(runningOffset,align);
        
        // .dynsym section
        dynSymSec->s_virtualAddress=runningRVA;
        dynSymSec->s_fileOffset=runningOffset;
        dynSymSec->s_size=sizeof(elf64Symbol)+numImports*sizeof(elf64Symbol);
        // push dynamic symbols
        elf64Symbol nullSym;
        nullSym.sy_name_idx=0;
        nullSym.sy_typeNbinding=0;
        nullSym.sy_secIdx=0;
        nullSym.sy_value=0;
        nullSym.sy_size=0;
        nullSym.push(dynamicSegment->data);
        for (size_t i = 0; i < numImports; i++) {
            elf64Symbol dynSym;
            dynSym.sy_name_idx=dynStrTabIndexes[i+1];
            dynSym.sy_typeNbinding=((ELF_SYMBOL_BINDING_GLOBAL)<<4)+ELF_SYMBOL_TYPE_FUNC;
            dynSym.sy_secIdx=0;
            dynSym.sy_value=0;
            dynSym.sy_size=0;
            dynSym.push(dynamicSegment->data);
        }
        // add padding to vector, runningOffset, and runningRVA
        runningRVA+=dynSymSec->s_size; runningOffset+=dynSymSec->s_size;
        padBytes(dynamicSegment->data,roundToAlign(runningOffset,align)-runningOffset);
        runningRVA=roundToAlign(runningRVA,align); runningOffset=roundToAlign(runningOffset,align);

        // .dynamic section
        dynamicSec->s_virtualAddress=runningRVA;
        dynamicSec->s_fileOffset=runningOffset;
        // push dynamic data
        elf64Dyn(ELF_DYN_TAG_NEEDED,dynStrTabIndexes[1+numImports]).push(dynamicSegment->data);// first dll name in dynamic string table
        elf64Dyn(ELF_DYN_TAG_STRTAB,dynStrTabSec->s_virtualAddress).push(dynamicSegment->data);
        elf64Dyn(ELF_DYN_TAG_STRSZ,dynStrTabSec->s_size).push(dynamicSegment->data);
        elf64Dyn(ELF_DYN_TAG_SYMTAB,dynSymSec->s_virtualAddress).push(dynamicSegment->data);
        elf64Dyn(ELF_DYN_TAG_SYMENT,sizeof(elf64Symbol)).push(dynamicSegment->data);
        elf64Dyn(ELF_DYN_TAG_PLTGOT,gotPltSec->s_virtualAddress).push(dynamicSegment->data);
        elf64Dyn(ELF_DYN_TAG_PLTREL,ELF_DYN_TAG_RELA).push(dynamicSegment->data);
        elf64Dyn(ELF_DYN_TAG_JMPREL,relaPltSec->s_virtualAddress).push(dynamicSegment->data);
        elf64Dyn(ELF_DYN_TAG_PLTRELSZ,sizeof(elf64Rela)).push(dynamicSegment->data);
        elf64Dyn(ELF_DYN_TAG_NULL,0).push(dynamicSegment->data);
        dynamicSec->s_size=10*sizeof(elf64Dyn);
        // add padding to vector, runningOffset, and runningRVA
        runningRVA+=dynamicSec->s_size; runningOffset+=dynamicSec->s_size;
        padBytes(dynamicSegment->data,roundToAlign(runningOffset,align)-runningOffset);
        runningRVA=roundToAlign(runningRVA,align); runningOffset=roundToAlign(runningOffset,align);
    }
    elfHeader.e_sectionHdrOffset=roundToAlign(runningOffset,SECTION_ALIGN);

    //for (size_t i = 0; i < labels.size(); i++)
    //    std::cout << '"' << labels[i].name << "\": " << intToHex(labels[i].base->getRVA()+labels[i].offset) << '\n';
    
    for (unsigned int i = 0; i < labelResolutions.size(); i++) {
        Elf64LabelResolution resolution = labelResolutions[i];
        Elf64Label label = labels[labelResIdxToLabelIdx[i]];
        setDwordAt(resolution.base->data,resolution.setAt,(label.base->getRVA()+label.offset)-(resolution.base->getRVA()+resolution.setAt)-resolution.relativeToOffset,true);
    }
    for (unsigned int i = 0; i < labels.size(); i++) if (labels[i].name.compare("_main")==0) { elfHeader.e_entryAddress=(0x0000000000400000+labels[i].base->getRVA()+labels[i].offset); break;}

    // push all the data
    elfHeader.e_numSegmentHdrs = numHeaders+1;
    elfHeader.e_numSectionHdrs = sectionHeaders.size();
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
    for (size_t i = 0; i < elfHeader.e_numSectionHdrs; i++) sectionHeaders[i]->push(stream);
}
Elf64SegmentHandler* Elf64Handler::addSeg(const uint32_t &type, const uint32_t &flags) {
    Elf64SegmentHandler *hdr = new Elf64SegmentHandler(*this, type, flags);
    segmentHeaders.push_back(hdr);
    return hdr;
}
elfSectionHdr64* Elf64Handler::addSec(const std::string& name, const uint32_t& type, const uint32_t& flags) {
    // create section
    sectionHeaders.push_back(new elfSectionHdr64(curStrTabOffset,type,flags));
    // push section name
    for (size_t i = 0; i < name.size(); i++) sectionStrTab.push_back(name[i]);
    sectionStrTab.push_back('\0');
    curStrTabOffset+=name.size()+1;
    // return pointer
    return sectionHeaders[sectionHeaders.size()-1];
}
void Elf64Handler::defineLabel(const std::string& name, Elf64SegmentHandler* base, const uint32_t& offset) {
    labels.push_back(Elf64Label(name,base,offset));
}
void Elf64Handler::resolveLabel(const std::string& name, Elf64SegmentHandler* base, const uint32_t& setAt, const int32_t& relativeToOffset) {
    labelResolutions.push_back(Elf64LabelResolution(name,base,setAt,relativeToOffset));
}
void Elf64Handler::addImport(const std::string& soName) {
    avaliableSos.push_back(soName);
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
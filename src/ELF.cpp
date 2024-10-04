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
    } else if (dy_tag==ELF_DYN_TAG_RUNPATH) {
        std::cout << "{ type: RUNPATH, ptr: " << intToHex(dy_valPtr);
        if (dy_valPtr!=0) {
            std::cout << " -> \"" << readStringAt(file,exportStrTabOffset+dy_valPtr) << "\"";
        }
        std::cout << " }\n";
    } else if (dy_tag==ELF_DYN_TAG_FLAGS) {
        std::cout << "{ type: FLAGS, val: " << intToHex(dy_valPtr);
        const bool hasFlagOrigin = ((dy_valPtr&ELF_DYN_F_ORIGIN)>0);
        const bool hasFlagSymbolic = ((dy_valPtr&ELF_DYN_F_SYMBOLIC)>0);
        const bool hasFlagTextRel = ((dy_valPtr&ELF_DYN_F_TEXTREL)>0);
        const bool hasFlagBindNow = ((dy_valPtr&ELF_DYN_F_BIND_NOW)>0);
        const bool hasFlagStaticTLS = ((dy_valPtr&ELF_DYN_F_STATIC_TLS)>0);
        if (hasFlagOrigin||hasFlagSymbolic||hasFlagTextRel||hasFlagBindNow||hasFlagStaticTLS) std::cout << " -> ";
        if (hasFlagOrigin) std::cout << "ORGIN";
        if (hasFlagOrigin&&(hasFlagSymbolic||hasFlagTextRel||hasFlagBindNow||hasFlagStaticTLS)) std::cout << ", ";
        if (hasFlagSymbolic) std::cout << "SYMBOLIC";
        if (hasFlagSymbolic&&(hasFlagTextRel||hasFlagBindNow||hasFlagStaticTLS)) std::cout << ", ";
        if (hasFlagTextRel) std::cout << "TEXTREL";
        if (hasFlagTextRel&&(hasFlagBindNow||hasFlagStaticTLS)) std::cout << ", ";
        if (hasFlagBindNow) std::cout << "BIND_NOW";
        if (hasFlagBindNow&&hasFlagStaticTLS) std::cout << ", ";
        if (hasFlagStaticTLS) std::cout << "STATIC_TLS";
        std::cout << " }\n";
    } else if (dy_tag==ELF_DYN_TAG_ENCODING)
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
    else if (dy_tag==ELF_DYN_TAG_FLAGS_1) {
        std::cout << "{ type: FLAGS_1, val: " << intToHex(dy_valPtr);
        const bool hasFlagNow = ((dy_valPtr&ELF_DYN_F1_NOW)>0);
        const bool hasFlagNoDelete = ((dy_valPtr&ELF_DYN_F1_NODELETE)>0);
        const bool hasFlagNoDefLib = ((dy_valPtr&ELF_DYN_F1_NODEFLIB)>0);
        const bool hasFlagEdited = ((dy_valPtr&ELF_DYN_F1_EDITED)>0);
        if (hasFlagNow||hasFlagNoDelete||hasFlagNoDefLib||hasFlagEdited) std::cout << " -> ";
        if (hasFlagNow) std::cout << "NOW";
        if (hasFlagNow&&(hasFlagNoDelete||hasFlagNoDefLib||hasFlagEdited)) std::cout << ", ";
        if (hasFlagNoDelete) std::cout << "NODELETE";
        if (hasFlagNoDelete&&(hasFlagNoDefLib||hasFlagEdited)) std::cout << ", ";
        if (hasFlagNoDefLib) std::cout << "NODEFLIB";
        if (hasFlagNoDefLib&&hasFlagEdited) std::cout << ", ";
        if (hasFlagEdited) std::cout << "EDITED";
        std::cout << " }\n";
    } else if (dy_tag==ELF_DYN_TAG_VERDEF) {
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
        std::cout << "{ type: " << intToHex(dy_tag) << " -> unknown, val/ptr: " << intToHex(dy_valPtr) << "}\n";
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
void elfSectionHdr64::print(std::vector<uint8_t>& file, std::vector<elfSectionHdr64>& sections, const uint32_t& strTabOffset, std::string delimmiter) {
    std::cout << "s_nameIdx: " << s_nameIdx;
    if (strTabOffset!=0)
        std::cout << " -> \"" << readStringAt(file,strTabOffset+s_nameIdx) << '\"';
    std::cout << delimmiter;
    std::cout << "s_type: " << intToHex(s_type) << " -> ";
    if (s_type==ELF_SECTION_TYPE_NULL) std::cout << "NULL";
    else if (s_type==ELF_SECTION_TYPE_PROGBITS) std::cout << "PROGBITS";
    else if (s_type==ELF_SECTION_TYPE_SYMTAB) std::cout << "SYMTAB";
    else if (s_type==ELF_SECTION_TYPE_STRTAB) std::cout << "STRTAB (string table)";
    else if (s_type==ELF_SECTION_TYPE_RELA) std::cout << "RELA (relocations with addends)";
    else if (s_type==ELF_SECTION_TYPE_HASH) std::cout << "HASH";
    else if (s_type==ELF_SECTION_TYPE_DYNAMIC) std::cout << "DYNAMIC";
    else if (s_type==ELF_SECTION_TYPE_NOTE) std::cout << "NOTE";
    else if (s_type==ELF_SECTION_TYPE_NOBITS) std::cout << "NOBITS (no data in file)";
    else if (s_type==ELF_SECTION_TYPE_REL) std::cout << "REL (relocations)";
    else if (s_type==ELF_SECTION_TYPE_DYNSYM) std::cout << "DYNSYM (dynamic symbol table)";
    else if (s_type==ELF_SECTION_TYPE_INIT_ARRAY) std::cout << "INIT_ARRAY (array of constructors)";
    else if (s_type==ELF_SECTION_TYPE_FINI_ARRAY) std::cout << "FINI_ARRAY (array of desructors)";
    else if (s_type==ELF_SECTION_TYPE_PREINIT_ARRAY) std::cout << "PREINIT_ARRAY";
    else if (s_type==ELF_SECTION_TYPE_GROUP) std::cout << "GROUP";
    else if (s_type==ELF_SECTION_TYPE_SYMTAB_SHNDX) std::cout << "SYMTAB_SHNDX";
    else if (s_type==ELF_SECTION_TYPE_NUM) std::cout << "NUM";
    else std::cout << "Unknown";
    std::cout << delimmiter;
    std::cout << "s_flags: " << intToHex(s_flags) << " -> ";
    const bool hasFlagWrite = ((s_flags&ELF_SECTION_FLAG_WRITE)>0);
    const bool hasFlagAlloc = ((s_flags&ELF_SECTION_FLAG_ALLOC)>0);
    const bool hasFlagExec = ((s_flags&ELF_SECTION_FLAG_EXEC)>0);
    const bool hasFlagMerge = ((s_flags&ELF_SECTION_FLAG_MERGE)>0);
    const bool hasFlagStrings = ((s_flags&ELF_SECTION_FLAG_STRINGS)>0);
    const bool hasFlagGROUP = ((s_flags&ELF_SECTION_FLAG_GROUP)>0);
    const bool hasFlagTLS = ((s_flags&ELF_SECTION_FLAG_TLS)>0);
    const bool hasFlagInfo = ((s_flags&ELF_SECTION_FLAG_INFO_LINK)>0);
    if (s_flags==0) {
        std::cout << "NONE/NULL";
    } else if (!hasFlagWrite&&!hasFlagAlloc&&!hasFlagExec&&!hasFlagMerge&&!hasFlagStrings&&!hasFlagGROUP&&!hasFlagTLS&&!hasFlagInfo)
        std::cout << "unknown";
    else {
        if (hasFlagWrite) std::cout << "Writable";
        if (hasFlagWrite&&(hasFlagAlloc||hasFlagExec||hasFlagMerge||hasFlagStrings||hasFlagGROUP||hasFlagTLS||hasFlagInfo)) std::cout << ", ";
        if (hasFlagAlloc) std::cout << "ALLOC";
        if (hasFlagAlloc&&(hasFlagExec||hasFlagMerge||hasFlagStrings||hasFlagGROUP||hasFlagTLS||hasFlagInfo)) std::cout << ", ";
        if (hasFlagExec) std::cout << "Executable";
        if (hasFlagExec&&(hasFlagMerge||hasFlagStrings||hasFlagGROUP||hasFlagTLS||hasFlagInfo)) std::cout << ", ";
        if (hasFlagMerge) std::cout << "Might merge";
        if (hasFlagMerge&&(hasFlagStrings||hasFlagGROUP||hasFlagTLS||hasFlagInfo)) std::cout << ", ";
        if (hasFlagStrings) std::cout << "Contains strings";
        if (hasFlagStrings&&(hasFlagGROUP||hasFlagTLS||hasFlagInfo)) std::cout << ", ";
        if (hasFlagGROUP) std::cout << "Is in group";
        if (hasFlagGROUP&&(hasFlagTLS||hasFlagInfo)) std::cout << ", ";
        if (hasFlagTLS) std::cout << "TLS";
        if (hasFlagTLS&&hasFlagInfo) std::cout << ", ";
        if (hasFlagInfo) std::cout << "has Info";
    }
    std::cout << delimmiter;
    std::cout << "s_virtualAddress: " << intToHex(s_virtualAddress) << delimmiter;
    std::cout << "s_fileOffset: " << intToHex(s_fileOffset) << delimmiter;
    std::cout << "s_size: " << intToHex(s_size) << delimmiter;
    std::cout << "s_link: " << intToHex(s_link);
    if (s_link>0) std::cout << " -> <" << readStringAt(file,strTabOffset+sections[s_link].s_nameIdx) << ">";
    std::cout << delimmiter;
    std::cout << "s_info: " << intToHex(s_info);
    if (hasFlagInfo&&(s_info>0)) std::cout << " -> <" << readStringAt(file,strTabOffset+sections[s_info].s_nameIdx) << ">";
    std::cout << delimmiter;
    std::cout << "s_align: " << intToHex(s_align) << delimmiter;
    std::cout << "s_entSize: " << intToHex(s_entSize);
    std::cout << '\n';
}
#pragma endregion section printing func

#pragma region So parsing
std::vector<soExportData> parseSo(const std::string& name) {
    // get file
    std::string fullPath = getFullFilePath(name);
    if (fullPath.size()==0) { std::cout << "File not found.\n"; return {}; }
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
    curStrTabOffset=0;

    elfSectionHdr64* nullSec = addSec("",ELF_SECTION_TYPE_NULL,0);
    elfSectionHdr64* shStrTabSec = addSec(".strtab",ELF_SECTION_TYPE_STRTAB,0);
    elfHeader.e_stringTableNdx=1;
}
void Elf64Handler::push(std::ofstream &stream) {
    // get all exports from the avaliable so files that the user gave
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

    // create section containing headers and str tables
    elfHeader.e_numSegmentHdrs = 1+segmentHeaders.size();
    elfHeader.e_segmentHdrOffset=sizeof(elfHdr64) + sectionStrTab.size();
    elfHeader.e_numSectionHdrs = 0;
    // segments that will only actually be added if there are imports
    elfSegmentHdr64 dynamicSegment(ELF_SEGMENT_TYPE_DYN, ELF_SEGMENT_FLAG_READ|ELF_SEGMENT_FLAG_WRITE);
    elfSegmentHdr64 dynamicLoadSegment(ELF_SEGMENT_TYPE_LOAD, ELF_SEGMENT_FLAG_READ|ELF_SEGMENT_FLAG_WRITE);
    elfSegmentHdr64 symbolsLoadSegment(ELF_SEGMENT_TYPE_LOAD, ELF_SEGMENT_FLAG_READ);
    elfSegmentHdr64 interpSegment(ELF_SEGMENT_TYPE_INTP, ELF_SEGMENT_FLAG_READ);
    // data for the dynamic string table
    std::string dynStrTab = "";
    std::vector<uint32_t> dynStrTabIndexes;
    uint32_t rPathAddr = 0;
    // if imports are need add the sections needed for that and push the names of the imports to the dynamic string table
    if (numImports>0) {
        elfHeader.e_segmentHdrOffset-=sectionStrTab.size();
        elfSectionHdr64* interpSec = addSec(".interp",ELF_SECTION_TYPE_PROGBITS,ELF_SECTION_FLAG_ALLOC);
        addSec(".dynsym",ELF_SECTION_TYPE_DYNSYM,ELF_SECTION_FLAG_ALLOC);
        elfSectionHdr64* dynStrTabSec = addSec(".dynstr",ELF_SECTION_TYPE_STRTAB,ELF_SECTION_FLAG_ALLOC);
        addSec(".rela.plt",ELF_SECTION_TYPE_RELA,ELF_SECTION_FLAG_ALLOC|ELF_SECTION_FLAG_INFO_LINK);
        addSec(".dynamic",ELF_SECTION_TYPE_DYNAMIC,ELF_SECTION_FLAG_WRITE|ELF_SECTION_FLAG_ALLOC);
        addSec(".got",ELF_SECTION_TYPE_PROGBITS,ELF_SECTION_FLAG_WRITE|ELF_SECTION_FLAG_ALLOC);

        interpSegment.s_fileOffset=interpSegment.s_virtualAddress=interpSegment.s_physAddress
        = interpSec->s_fileOffset=interpSec->s_virtualAddress
        = sizeof(elfHdr64)+addStrToTbl("/lib64/ld-linux-x86-64.so.2");
        interpSegment.s_sizeFile=interpSegment.s_sizeMemory=interpSec->s_size=28;

        elfHeader.e_segmentHdrOffset+=sectionStrTab.size();

        // .dynstrtab section
        // push dynamic string table
        dynStrTabIndexes.push_back(0);
        dynStrTab+='\0';
        // push import names
        for (size_t i = 0; i < numImports; i++) {
            dynStrTabIndexes.push_back(dynStrTab.size());
            dynStrTab+=*imports[i].name+'\0';
        }
        // push so names
        for (size_t i = 0; i < numImports; i++) {
            dynStrTabIndexes.push_back(dynStrTab.size());
            dynStrTab+=*imports[i].soName+'\0';
        }
        // push RPATH
        rPathAddr=dynStrTab.size();
        dynStrTab+=".:/usr/lib";
        dynStrTab+='\0';
        elfHeader.e_numSegmentHdrs+=4;
        elfHeader.e_segmentHdrOffset+=dynStrTab.size();
        elfHeader.e_numSectionHdrs = sectionHeaders.size();

        dynStrTabSec->s_fileOffset = sizeof(elfHdr64)+sectionStrTab.size();
        dynStrTabSec->s_virtualAddress = 0x0000000000000000+sizeof(elfHdr64)+sectionStrTab.size();
        dynStrTabSec->s_size=dynStrTab.size();
    }

    // set parameters of the head segment
    elfSectionHdr64* shStrTabSec = sectionHeaders[1];
    shStrTabSec->s_fileOffset = sizeof(elfHdr64);
    shStrTabSec->s_virtualAddress = 0x0000000000000000+sizeof(elfHdr64);
    shStrTabSec->s_size=sectionStrTab.size();
    elfSegmentHdr64 headSeg(ELF_SEGMENT_TYPE_LOAD,ELF_SEGMENT_FLAG_READ);
    headSeg.s_virtualAddress = headSeg.s_physAddress = 0x0000000000000000;
    headSeg.s_fileOffset = 0;
    headSeg.s_sizeFile = headSeg.s_sizeMemory = elfHeader.e_segmentHdrOffset + elfHeader.e_numSegmentHdrs * sizeof(elfSegmentHdr64);
    // set offsets, and virtual addresses of the rest of the segments
    uint32_t baseOffset = roundToAlign(headSeg.s_sizeFile,SECTION_ALIGN);//FILE_ALIGN);
    uint32_t runningOffset = baseOffset;
    uint32_t runningRVA = roundToAlign(baseOffset,SECTION_ALIGN);
    for (uint32_t i = 0; i < segmentHeaders.size(); i++) {
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

    // create and fill .dynamic, .dynstr, .dynsym, .plt.got, and .rela.plt sections within a DYNAMIC type and LOAD type segments
    std::vector<uint8_t> dynamicData;
    if (numImports>0) {
        symbolsLoadSegment.s_virtualAddress=symbolsLoadSegment.s_physAddress = 0x0000000000000000+runningRVA;
        symbolsLoadSegment.s_fileOffset = runningOffset;
        dynamicLoadSegment.s_align=dynamicSegment.s_align = symbolsLoadSegment.s_align = SECTION_ALIGN;

        elfSectionHdr64* dynSymSec    = sectionHeaders[3];
        elfSectionHdr64* dynStrTabSec = sectionHeaders[4];
        elfSectionHdr64* relaPltSec   = sectionHeaders[5];
        elfSectionHdr64* dynamicSec   = sectionHeaders[6];
        elfSectionHdr64* gotPltSec    = sectionHeaders[7];
        dynSymSec->s_link=4;
        dynamicSec->s_link=4;
        uint32_t align = 0x80;

        // .rela.plt section
        relaPltSec->s_virtualAddress=0x0000000000000000+runningRVA;
        relaPltSec->s_fileOffset=runningOffset;
        relaPltSec->s_size=numImports*sizeof(elf64Rela);
        relaPltSec->s_link=3;
        relaPltSec->s_info=7;
        relaPltSec->s_align=8;
        relaPltSec->s_entSize=sizeof(elf64Rela);
        // push rela entries
        for (size_t i = 0; i < numImports; i++) {
            elf64Rela rela;
            rela.r_offset=0x3100+3*8+i*8;
            rela.r_symtabIndex=1+i;
            rela.r_type=ELF64_REL_TYPE_AMD64_JUMP_SLOT;
            rela.r_addend=0;
            rela.push(dynamicData);
        }
        // add padding to vector, runningOffset, and runningRVA
        runningRVA+=relaPltSec->s_size; runningOffset+=relaPltSec->s_size;
        padBytes(dynamicData,roundToAlign(runningOffset,align)-runningOffset);
        runningRVA=roundToAlign(runningRVA,align); runningOffset=roundToAlign(runningOffset,align);
        
        // .dynsym section
        dynSymSec->s_virtualAddress=0x0000000000000000+runningRVA;
        dynSymSec->s_fileOffset=runningOffset;
        dynSymSec->s_size=sizeof(elf64Symbol)+numImports*sizeof(elf64Symbol);
        dynSymSec->s_info=1;
        dynSymSec->s_align=8;
        dynSymSec->s_entSize=sizeof(elf64Symbol);
        // push dynamic symbols
        elf64Symbol().push(dynamicData);// push a null symbol
        for (size_t i = 0; i < numImports; i++) {
            elf64Symbol dynSym;
            dynSym.sy_name_idx=1+i;
            dynSym.sy_typeNbinding=((ELF_SYMBOL_BINDING_GLOBAL)<<4)+ELF_SYMBOL_TYPE_FUNC;
            dynSym.sy_secIdx=0;
            dynSym.sy_value=0;
            dynSym.sy_size=0;
            dynSym.push(dynamicData);
        }
        // add padding to vector, runningOffset, and runningRVA
        runningRVA+=dynSymSec->s_size; runningOffset+=dynSymSec->s_size;
        padBytes(dynamicData,roundToAlign(runningOffset,align)-runningOffset);
        runningRVA=roundToAlign(runningRVA,align); runningOffset=roundToAlign(runningOffset,align);

        symbolsLoadSegment.s_sizeFile = symbolsLoadSegment.s_sizeMemory = dynamicData.size();

        // .got.plt section
        gotPltSec->s_virtualAddress=0x0000000000000000+runningRVA;
        dynamicLoadSegment.s_virtualAddress=dynamicLoadSegment.s_physAddress = gotPltSec->s_virtualAddress;
        gotPltSec->s_fileOffset=runningOffset;
        dynamicLoadSegment.s_fileOffset = gotPltSec->s_fileOffset;
        gotPltSec->s_align=8;
        gotPltSec->s_entSize=8;
        gotPltSec->s_size=3*8+numImports*8;
        // push got entries, which are just 64 bit numbers
        pushQword(dynamicData,0x0,true);
        pushQword(dynamicData,0,true);
        pushQword(dynamicData,0,true);
        for (size_t i = 0; i < numImports; i++) {
            defineLabel(*imports[i].name,&symbolsLoadSegment,dynamicData.size());
            pushQword(dynamicData,0x0,true);
        }
        // add padding to vector, runningOffset, and runningRVA
        runningRVA+=gotPltSec->s_size; runningOffset+=gotPltSec->s_size;
        padBytes(dynamicData,roundToAlign(runningOffset,align)-runningOffset);
        runningRVA=roundToAlign(runningRVA,align); runningOffset=roundToAlign(runningOffset,align);

        // .dynamic section
        dynamicSec->s_virtualAddress=0x0000000000000000+runningRVA;
        dynamicSegment.s_virtualAddress=dynamicSegment.s_physAddress = dynamicSec->s_virtualAddress;
        dynamicSec->s_fileOffset=runningOffset;
        dynamicSegment.s_fileOffset = dynamicSec->s_fileOffset;
        dynamicSec->s_align=8;
        dynamicSec->s_entSize=sizeof(elf64Dyn);
        // push dynamic data
        elf64Dyn(ELF_DYN_TAG_NEEDED,dynStrTabIndexes[1+numImports]).push(dynamicData);// first dll name in dynamic string table
        elf64Dyn(ELF_DYN_TAG_RUNPATH,rPathAddr).push(dynamicData);
        //elf64Dyn(0x000000006ffffef5,0).push(dynamicData);
        elf64Dyn(ELF_DYN_TAG_STRTAB,dynStrTabSec->s_virtualAddress).push(dynamicData);
        elf64Dyn(ELF_DYN_TAG_SYMTAB,dynSymSec->s_virtualAddress).push(dynamicData);
        elf64Dyn(ELF_DYN_TAG_STRSZ,dynStrTabSec->s_size).push(dynamicData);
        elf64Dyn(ELF_DYN_TAG_SYMENT,sizeof(elf64Symbol)).push(dynamicData);
        elf64Dyn(ELF_DYN_TAG_DEBUG,0).push(dynamicData);
        elf64Dyn(ELF_DYN_TAG_PLTGOT,gotPltSec->s_virtualAddress).push(dynamicData);
        elf64Dyn(ELF_DYN_TAG_PLTRELSZ,sizeof(elf64Rela)).push(dynamicData);
        elf64Dyn(ELF_DYN_TAG_PLTREL,ELF_DYN_TAG_RELA).push(dynamicData);
        elf64Dyn(ELF_DYN_TAG_JMPREL,relaPltSec->s_virtualAddress).push(dynamicData);
        elf64Dyn(ELF_DYN_TAG_FLAGS,ELF_DYN_F_BIND_NOW).push(dynamicData);
        elf64Dyn(ELF_DYN_TAG_FLAGS_1,ELF_DYN_F1_NOW).push(dynamicData);
        elf64Dyn(ELF_DYN_TAG_NULL,0).push(dynamicData);
        dynamicSec->s_size=14*sizeof(elf64Dyn);
        dynamicLoadSegment.s_sizeFile=dynamicLoadSegment.s_sizeMemory = dynamicSec->s_virtualAddress+dynamicSec->s_size-gotPltSec->s_virtualAddress;
        dynamicSegment.s_sizeFile=dynamicSegment.s_sizeMemory = dynamicSec->s_size;
        // add padding to vector, runningOffset, and runningRVA
        runningRVA+=dynamicSec->s_size; runningOffset+=dynamicSec->s_size;
        padBytes(dynamicData,roundToAlign(runningOffset,align)-runningOffset);
        runningRVA=roundToAlign(runningRVA,align); runningOffset=roundToAlign(runningOffset,align);
    }
    elfHeader.e_sectionHdrOffset=roundToAlign(runningOffset,SECTION_ALIGN);

    // resolve labels
    for (unsigned int i = 0; i < labelResolutions.size(); i++) {
        Elf64LabelResolution resolution = labelResolutions[i];
        Elf64Label label = labels[labelResIdxToLabelIdx[i]];
        setDwordAt(resolution.base->data,resolution.setAt,(label.base->s_virtualAddress-0x0000000000000000+label.offset)-(resolution.base->getRVA()+resolution.setAt)-resolution.relativeToOffset,true);
    }
    for (unsigned int i = 0; i < labels.size(); i++) if (labels[i].name.compare("_main")==0) { elfHeader.e_entryAddress=(labels[i].base->s_virtualAddress+labels[i].offset); break;}

    // push elf header and string tables
    elfHeader.push(stream);
    for (size_t i = 0; i < sectionStrTab.size(); i++)
        pushByte(stream,sectionStrTab[i]);
    if (numImports>0)
        for (size_t i = 0; i < dynStrTab.size(); i++)
            pushByte(stream,dynStrTab[i]);
    //push segment headers
    if (numImports>0) { interpSegment.push(stream); }
    headSeg.push(stream);
    for (uint32_t i = 0; i < segmentHeaders.size(); i++)
        segmentHeaders[i]->pushHeader(stream);
    if (numImports>0) { symbolsLoadSegment.push(stream); dynamicLoadSegment.push(stream); dynamicSegment.push(stream); }
    // push segment data
    padBytes(stream,baseOffset-headSeg.s_sizeFile);
    for (uint32_t i = 0; i < segmentHeaders.size(); i++) {
        segmentHeaders[i]->pushData(stream);
        padBytes(stream,SECTION_ALIGN-(segmentHeaders[i]->getSize()%(SECTION_ALIGN)));
    }
    // push data in the dynamic section, if there is imports
    if (numImports>0) { pushChars(stream,dynamicData); padBytes(stream,SECTION_ALIGN-(dynamicData.size()%(SECTION_ALIGN))); }
    // push section headers at the very end
    for (size_t i = 0; i < elfHeader.e_numSectionHdrs; i++) sectionHeaders[i]->push(stream);
}
uint32_t Elf64Handler::addStrToTbl(const std::string& str) {
    uint32_t tmp = curStrTabOffset;
    sectionStrTab+=str+'\0';
    curStrTabOffset+=str.size()+1;
    return tmp;
}
Elf64SegmentHandler* Elf64Handler::addSeg(const uint32_t &type, const uint32_t &flags) {
    Elf64SegmentHandler *hdr = new Elf64SegmentHandler(*this, type, flags);
    segmentHeaders.push_back(hdr);
    return hdr;
}
elfSectionHdr64* Elf64Handler::addSec(const std::string& name, const uint32_t& type, const uint32_t& flags) {
    sectionHeaders.push_back(new elfSectionHdr64(addStrToTbl(name),type,flags));
    return sectionHeaders[sectionHeaders.size()-1];
}
void Elf64Handler::defineLabel(const std::string& name, elfSegmentHdr64* base, const uint32_t& offset) {
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
void Elf64SegmentHandler::pushHeader(std::ofstream &stream, const int32_t& size) {
    segmentHeader.s_sizeFile = segmentHeader.s_sizeMemory = ((size==-1)?data.size():size);
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
    segmentHeader.s_virtualAddress = segmentHeader.s_physAddress = 0x0000000000000000+Rva;// offset in memory, can be different if you have uninitialized data
}
uint32_t Elf64SegmentHandler::getRVA() {
    return segmentHeader.s_virtualAddress-0x0000000000000000;
};
// label stuff
void Elf64SegmentHandler::defineLabel(const std::string& name) {
    elfHandler.defineLabel(name,&segmentHeader,data.size());
}
void Elf64SegmentHandler::resolveLabel(const std::string& name, const int32_t& relativeToOffset) {
    elfHandler.resolveLabel(name,this,data.size()-relativeToOffset,relativeToOffset);
}

#pragma endregion  // Elf64SegmentHandler
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

    headSeg=addSeg(ELF_SEGMENT_TYPE_LOAD,ELF_SEGMENT_FLAG_READ);
    headSeg->addSec("",ELF_SECTION_TYPE_NULL,0);
    headSeg->addSec(".headers",ELF_SECTION_TYPE_PROGBITS,ELF_SECTION_FLAG_ALLOC);
    headSeg->addSec(".shstrtab",ELF_SECTION_TYPE_STRTAB,ELF_SECTION_FLAG_ALLOC|ELF_SECTION_FLAG_STRINGS);
    elfHeader.e_stringTableNdx=2;
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
    int32_t numImports=0;
    for (unsigned int i = 0; i < labelResolutions.size(); i++) {
        Elf64LabelResolution resolution = labelResolutions[i];
        bool found=false;
        for (unsigned int j = 0; j < labels.size(); j++) {
            Elf64Label label = labels[j];
            if (resolution.name.compare(label.name)==0) { found=true; break; }
        }
        if (!found) {// label was not found in code, search for it in dlls
            for (size_t j = 0; j < numExports; j++) {
                if (allExports[j].name.compare(resolution.name)==0) {
                    found=true;
                    if (exportImportIndex[j]==-1) {
                        imports.push_back({allExports[j].symTabIndex,&allExports[j].name,&allExports[j].soName});
                        exportImportIndex[j]=numImports;
                        numImports++;
                    }
                }
            }
        }
        if (!found) { std::cout << "unresolved symbol " << resolution.name << " could not be found" << std::endl; return; }
    }

    elfSegmentHdr64 dynamicSeg(ELF_SEGMENT_TYPE_DYN , ELF_SEGMENT_FLAG_READ|ELF_SEGMENT_FLAG_WRITE);
    Elf64SegmentHandler* dynamicLoadSeg;
    Elf64SegmentHandler* symbolsLoadSeg;
    elfSegmentHdr64 interpSeg(ELF_SEGMENT_TYPE_INTP, ELF_SEGMENT_FLAG_READ);
    
    Elf64SectionHandler* interpSec;
    Elf64SectionHandler* dynsymSec;
    Elf64SectionHandler* dynstrSec;
    Elf64SectionHandler* relapltSec;
    Elf64SectionHandler* gotpltSec;
    Elf64SectionHandler* dynamicSec;
    
    if (numImports>0) {
        dynamicLoadSeg = addSeg(ELF_SEGMENT_TYPE_LOAD, ELF_SEGMENT_FLAG_READ|ELF_SEGMENT_FLAG_WRITE);
        symbolsLoadSeg = addSeg(ELF_SEGMENT_TYPE_LOAD, ELF_SEGMENT_FLAG_READ);

        interpSec = headSeg->addSec(".interp"  ,ELF_SECTION_TYPE_PROGBITS, ELF_SECTION_FLAG_ALLOC);

        dynstrSec = headSeg->addSec(".dynstr"  ,ELF_SECTION_TYPE_STRTAB, ELF_SECTION_FLAG_ALLOC);
        
        relapltSec = symbolsLoadSeg->addSec(".rela.plt",ELF_SECTION_TYPE_RELA, ELF_SECTION_FLAG_ALLOC|ELF_SECTION_FLAG_INFO_LINK);
        relapltSec->sectionHeader.s_link=6;// index of .dynsym
        relapltSec->sectionHeader.s_info=8;// index of .got.plt
        relapltSec->sectionHeader.s_align=8;
        relapltSec->sectionHeader.s_entSize=sizeof(elf64Rela);
        
        dynsymSec = symbolsLoadSeg->addSec(".dynsym"  ,ELF_SECTION_TYPE_DYNSYM, ELF_SECTION_FLAG_ALLOC|ELF_SECTION_FLAG_STRINGS);
        dynsymSec->sectionHeader.s_link=4;// index of .dynstr
        dynsymSec->sectionHeader.s_align=8;
        dynsymSec->sectionHeader.s_entSize=sizeof(elf64Symbol);
        
        gotpltSec = dynamicLoadSeg->addSec(".got.plt" ,ELF_SECTION_TYPE_PROGBITS, ELF_SECTION_FLAG_ALLOC|ELF_SECTION_FLAG_WRITE);
        gotpltSec->sectionHeader.s_align=8;
        gotpltSec->sectionHeader.s_entSize=8;

        dynamicSec = dynamicLoadSeg->addSec(".dynamic" ,ELF_SECTION_TYPE_DYNAMIC, ELF_SECTION_FLAG_ALLOC|ELF_SECTION_FLAG_WRITE);
        dynamicSec->sectionHeader.s_link=4;// index of .dynstr
        dynamicSec->sectionHeader.s_align=8;
        dynamicSec->sectionHeader.s_entSize=sizeof(elf64Dyn);

        // .interp section
        interpSec->defineLabel("interp");
        std::string interpreter = "/lib64/ld-linux-x86-64.so.2";
        for (size_t i = 0; i < interpreter.size(); i++)
            pushByte(interpSec,interpreter[i]);
        pushByte(interpSec,'\0');

        // .dynstr section
        std::vector<uint32_t> dynStrTabIndexes;
        dynStrTabIndexes.push_back(0);
        pushByte(dynstrSec,'\0');
        // push import names
        for (size_t i = 0; i < numImports; i++) {
            std::string str = *imports[i].name;
            dynStrTabIndexes.push_back(dynstrSec->data.size());
            for (size_t i = 0; i < str.size(); i++)
                pushByte(dynstrSec,str[i]);
            pushByte(dynstrSec,'\0');
        }
        // push so names
        for (size_t i = 0; i < numImports; i++) {
            std::string str = *imports[i].soName;
            dynStrTabIndexes.push_back(dynstrSec->data.size());
            for (size_t i = 0; i < str.size(); i++)
                pushByte(dynstrSec,str[i]);
            pushByte(dynstrSec,'\0');
        }
        // push RPATH
        dynStrTabIndexes.push_back(dynstrSec->data.size());
        uint32_t runPathOffset=dynstrSec->data.size();
        std::string str = ".:/usr/lib";
        for (size_t i = 0; i < str.size(); i++)
            pushByte(dynstrSec,str[i]);
        pushByte(dynstrSec,'\0');

        // .rela.plt section
        for (size_t i = 0; i < numImports; i++) {
            elf64Rela rela;
            rela.r_offset=0x3000+3*8+i*8;
            rela.r_symtabIndex=1+i;
            rela.r_type=ELF64_REL_TYPE_AMD64_JUMP_SLOT;
            rela.r_addend=0;
            rela.push(relapltSec->data);
        }

        // .dynstr section
        elf64Symbol().push(dynsymSec->data);// push a null symbol
        for (size_t i = 0; i < numImports; i++) {
            elf64Symbol dynSym;
            dynSym.sy_name_idx=1+i;
            dynSym.sy_typeNbinding=((ELF_SYMBOL_BINDING_GLOBAL)<<4)+ELF_SYMBOL_TYPE_FUNC;
            dynSym.sy_secIdx=0;
            dynSym.sy_value=0;
            dynSym.sy_size=0;
            dynSym.push(dynsymSec->data);
        }

        // .got.plt section
        pushQword(gotpltSec->data,0,true);
        pushQword(gotpltSec->data,0,true);
        pushQword(gotpltSec->data,0,true);
        for (size_t i = 0; i < numImports; i++) {
            gotpltSec->defineLabel(*imports[i].name);
            pushQword(gotpltSec->data,0x0,true);
        }

        // .dynamic section
        elf64Dyn(ELF_DYN_TAG_NEEDED,dynStrTabIndexes[1+numImports]).push(dynamicSec->data);// first dll name in dynamic string table
        elf64Dyn(ELF_DYN_TAG_RUNPATH,runPathOffset).push(dynamicSec->data);
        //elf64Dyn(0x000000006ffffef5,0).push(dynamicData);
        elf64Dyn(ELF_DYN_TAG_STRTAB,0).push(dynamicSec->data); dynamicSec->resolveLabel(".dynstr",8,true);
        elf64Dyn(ELF_DYN_TAG_SYMTAB,0).push(dynamicSec->data); dynamicSec->resolveLabel(".dynsym",8,true);
        elf64Dyn(ELF_DYN_TAG_STRSZ,dynstrSec->data.size()).push(dynamicSec->data);
        elf64Dyn(ELF_DYN_TAG_SYMENT,sizeof(elf64Symbol)).push(dynamicSec->data);
        //elf64Dyn(ELF_DYN_TAG_DEBUG,0).push(dynamicSec->data);
        elf64Dyn(ELF_DYN_TAG_PLTGOT,0).push(dynamicSec->data); dynamicSec->resolveLabel(".got.plt",8,true);
        elf64Dyn(ELF_DYN_TAG_PLTRELSZ,sizeof(elf64Rela)).push(dynamicSec->data);
        elf64Dyn(ELF_DYN_TAG_PLTREL,ELF_DYN_TAG_RELA).push(dynamicSec->data);
        elf64Dyn(ELF_DYN_TAG_JMPREL,0).push(dynamicSec->data); dynamicSec->resolveLabel(".rela.plt",8,true);
        elf64Dyn(ELF_DYN_TAG_FLAGS,ELF_DYN_F_BIND_NOW).push(dynamicSec->data);
        elf64Dyn(ELF_DYN_TAG_FLAGS_1,ELF_DYN_F1_NOW|ELF_DYN_F1_SYMINTPOS).push(dynamicSec->data);
        elf64Dyn(ELF_DYN_TAG_NULL,0).push(dynamicSec->data);
    }

    for (size_t i = 0; i < sectionStrTab.size(); i++)
        pushByte(headSeg->sections[2],sectionStrTab[i]);
    
    //push headers
    elfHeader.push(headSeg->sections[1]->data);
    for (uint32_t i = 0; i < segments.size(); i++)
        segments[i]->pushHeader(headSeg->sections[1]->data);
    if (numImports>0) { interpSeg.push(headSeg->sections[1]->data); dynamicSeg.push(headSeg->sections[1]->data); }

    // set offsets, and virtual addresses of the rest of the segments
    uint32_t runningOffset = 0;
    uint32_t runningRVA = 0;
    for (uint32_t i = 0; i < segments.size(); i++) {
        segments[i]->setOffset(runningOffset);
        segments[i]->setRVA(runningRVA);
        segments[i]->setSectionAlign(SECTION_ALIGN);
        segments[i]->setFileAlign(SECTION_ALIGN);//FILE_ALIGN);
        segments[i]->segmentHeader.s_sizeFile = segments[i]->segmentHeader.s_sizeMemory = segments[i]->getSize();
        uint32_t segmentSize = segments[i]->getSize();
        uint32_t virtSize = roundToAlign(segmentSize,SECTION_ALIGN);
        uint32_t fileSize = roundToAlign(segmentSize,SECTION_ALIGN);//FILE_ALIGN);
        runningOffset += fileSize;
        runningRVA += virtSize;
    }

    // resolve labels
    for (unsigned int i = 0; i < labelResolutions.size(); i++) {
        Elf64LabelResolution resolution = labelResolutions[i];
        for (unsigned int j = 0; j < labels.size(); j++) {
            Elf64Label label = labels[j];
            if (resolution.name.compare(label.name)==0) {
                setDwordAt(
                    resolution.base->data,resolution.setAt,
                    (label.base->s_virtualAddress-0x0000000000000000+label.offset)-(resolution.isAbsolute?((resolution.base->getRVA()+resolution.setAt)-resolution.relativeToOffset):0),
                    true
                );
                break;
            }
        }
    }
    for (unsigned int i = 0; i < labels.size(); i++) {
        Elf64Label label = labels[i];
        if (labels[i].name.compare("_main")==0) {
            elfHeader.e_entryAddress=(labels[i].base->s_virtualAddress-0x0000000000000000+labels[i].offset); break;
        }
    }
    
    interpSeg.s_fileOffset = headSeg->sections[3]->sectionHeader.s_fileOffset;
    interpSeg.s_virtualAddress=interpSeg.s_physAddress = headSeg->sections[3]->sectionHeader.s_virtualAddress;
    interpSeg.s_sizeFile = interpSeg.s_sizeMemory = headSeg->sections[3]->sectionHeader.s_size;
    
    dynamicSeg.s_fileOffset = dynamicLoadSeg->sections[1]->sectionHeader.s_fileOffset;
    dynamicSeg.s_virtualAddress=dynamicSeg.s_physAddress = dynamicLoadSeg->sections[1]->sectionHeader.s_virtualAddress;
    dynamicSeg.s_sizeFile = dynamicSeg.s_sizeMemory = dynamicLoadSeg->sections[1]->sectionHeader.s_size;
    
    headSeg->sections[1]->data.clear();
    elfHeader.e_numSegmentHdrs=segments.size();
    if (numImports>0) elfHeader.e_numSegmentHdrs+=2;
    elfHeader.e_numSectionHdrs=0;
    for (size_t i = 0; i < segments.size(); i++)
        elfHeader.e_numSectionHdrs+=segments[i]->sections.size();
    elfHeader.e_sectionHdrOffset=runningOffset;
    elfHeader.push(headSeg->sections[1]->data);
    for (uint32_t i = 0; i < segments.size(); i++)
        segments[i]->pushHeader(headSeg->sections[1]->data);
    if (numImports>0) { interpSeg.push(headSeg->sections[1]->data); dynamicSeg.push(headSeg->sections[1]->data); }
    for (uint32_t i = 0; i < segments.size(); i++) {
        segments[i]->pushData(stream);
        padBytes(stream,SECTION_ALIGN-(segments[i]->segmentHeader.s_sizeFile%(SECTION_ALIGN)));
    }
    // push section headers at the very end
    for (size_t i = 0; i < segments.size(); i++) segments[i]->pushSectionHeaders(stream);
}
uint32_t Elf64Handler::addStrToTbl(const std::string& str) {
    uint32_t tmp = curStrTabOffset;
    sectionStrTab+=str+'\0';
    curStrTabOffset+=str.size()+1;
    return tmp;
}
Elf64SegmentHandler* Elf64Handler::addSeg(const uint32_t &type, const uint32_t &flags) {
    Elf64SegmentHandler *hdr = new Elf64SegmentHandler(*this, type, flags);
    segments.push_back(hdr);
    return hdr;
}
void Elf64Handler::defineLabel(const std::string& name, elfSectionHdr64* base, const uint32_t& offset) {
    labels.push_back(Elf64Label(name,base,offset));
}
void Elf64Handler::resolveLabel(const std::string& name, Elf64SectionHandler* base, const uint32_t& setAt, const int32_t& relativeToOffset, const bool& isAbsolute) {
    labelResolutions.push_back(Elf64LabelResolution(name,base,setAt,relativeToOffset,isAbsolute));
}
void Elf64Handler::addImport(const std::string& soName) {
    avaliableSos.push_back(soName);
}
#pragma endregion  // elfHandler

#pragma region Elf64SegmentHandler
Elf64SegmentHandler::Elf64SegmentHandler(Elf64Handler &_elfHandler, const uint32_t &type, const uint32_t &flags) : elfHandler(_elfHandler), segmentHeader(type, flags) {
}
void Elf64SegmentHandler::pushHeader(std::ofstream &stream) {
    segmentHeader.push(stream);
}
void Elf64SegmentHandler::pushHeader(std::vector<uint8_t>& vec) {
    segmentHeader.push(vec);
}
void Elf64SegmentHandler::pushData(std::ofstream &stream) {
    for (size_t i = 0; i < sections.size(); i++)
        pushChars(stream, sections[i]->data);
}
void Elf64SegmentHandler::pushSectionHeaders(std::ofstream &stream) {
    for (size_t i = 0; i < sections.size(); i++) {
        sections[i]->sectionHeader.push(stream);
    }
}
Elf64SectionHandler* Elf64SegmentHandler::addSec(const std::string& name, const uint32_t& type, const uint32_t& flags) {
    sections.push_back(new Elf64SectionHandler(*this,name,type,flags));
    return sections[sections.size()-1];
}

uint32_t Elf64SegmentHandler::getSize() {
    uint32_t tmp = 0;
    for (size_t i = 0; i < sections.size(); i++)
        tmp+=sections[i]->data.size();
    return tmp;
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
    uint32_t runningOffset = offset;
    for (size_t i = 0; i < sections.size(); i++) {
        sections[i]->sectionHeader.s_fileOffset=runningOffset;
        sections[i]->sectionHeader.s_size=sections[i]->data.size();
        runningOffset+=sections[i]->data.size();
    }
}
uint32_t Elf64SegmentHandler::getOffset() {
    return segmentHeader.s_fileOffset;
}
// RVA stuff
void Elf64SegmentHandler::setRVA(const uint32_t &Rva) {
    segmentHeader.s_virtualAddress = segmentHeader.s_physAddress = 0x0000000000000000+Rva;// offset in memory, can be different if you have uninitialized data
    uint32_t runningRVA = Rva;
    for (size_t i = 0; i < sections.size(); i++) {
        sections[i]->sectionHeader.s_virtualAddress=0x0000000000000000+runningRVA;
        sections[i]->sectionHeader.s_size=sections[i]->data.size();
        runningRVA+=sections[i]->data.size();
    }
}
uint32_t Elf64SegmentHandler::getRVA() {
    return segmentHeader.s_virtualAddress-0x0000000000000000;
};
#pragma endregion  // Elf64SegmentHandler

#pragma region Elf64SectionHandler
Elf64SectionHandler::Elf64SectionHandler(Elf64SegmentHandler &_segmentHandler, const std::string& name, const uint32_t& type, const uint32_t& flags)
    : segmentHandler(_segmentHandler), sectionHeader(elfSectionHdr64(segmentHandler.elfHandler.addStrToTbl(name),type,flags)) {
}
// RVA stuff
uint32_t Elf64SectionHandler::getRVA() {
    return sectionHeader.s_virtualAddress-0x0000000000000000;
};
// label stuff
void Elf64SectionHandler::defineLabel(const std::string& name) {
    segmentHandler.elfHandler.defineLabel(name,&sectionHeader,data.size());
}
void Elf64SectionHandler::resolveLabel(const std::string& name, const int32_t& relativeToOffset, const bool& isAbsolute) {
    segmentHandler.elfHandler.resolveLabel(name,this,data.size()-relativeToOffset,relativeToOffset,isAbsolute);
}
template <>
void pushChars(Elf64SectionHandler*& reciever, const uint8_t* chars, const size_t& len, const bool& LSB) {
    pushChars(reciever->data, chars, len, LSB);
}
#pragma endregion// Elf64SectionHandler
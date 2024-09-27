#ifndef _ELF_H
#define _ELF_H

#include "elfConstants.h"
#include "globalConstants.h"

extern uint8_t elf_encoding;
extern uint16_t elf_type;
extern uint8_t elf_osAbi;
extern uint16_t elf_machine;

#pragma region structs

struct elfSectionHdr64;
struct elfSectionPlusOffset {
    elfSectionHdr64* section=nullptr;
    int32_t index;
    uint32_t offset=0;
};
elfSectionPlusOffset RVAtoSectionPlusOffset(const uint32_t& RVA, const uint32_t& size, std::vector<elfSectionHdr64>& sections);
elfSectionPlusOffset fileOffsettoSectionPlusOffset(const uint32_t& offset, const uint32_t& size, std::vector<elfSectionHdr64>& sections);

struct elfSegmentHdr32 {
    uint32_t s_type;            // word
    uint32_t s_fileOffset;      // Offset
    uint32_t s_virtualAddress;  // Address
    uint32_t s_physAddress;     // Address
    uint32_t s_sizeFile;        // word
    uint32_t s_sizeMemory;      // word
    uint32_t s_flags;           // word
    uint32_t s_align;           // word

    elfSegmentHdr32(const uint32_t& type, const uint32_t& flags) {
        // segment type
        if (type == ELF_SEGMENT_TYPE_LOAD || type == ELF_SEGMENT_TYPE_DYN || type == ELF_SEGMENT_TYPE_INTP || type == ELF_SEGMENT_TYPE_NOTE || type == ELF_SEGMENT_TYPE_PHDR || type == ELF_SEGMENT_TYPE_TLS)
            s_type = type;
        else
            s_type = ELF_SEGMENT_TYPE_INV;
        s_fileOffset = 0x00000000;  // currently unknown
        s_virtualAddress = 0x00000000;  // currently unknown
        s_physAddress = 0x00000000;// not used in most executablews
        s_sizeFile = 0x00000000;  // currently unknown
        s_sizeMemory = 0x00000000;  // currently unknown
        s_flags = flags;
        s_align = SECTION_ALIGN;// bit aligment
    }
    void push(std::ofstream& stream) {
        bool LSB = elf_encoding == ELF_ENCODING_LSB;  // is little endian
        pushDword(stream, s_type, LSB);
        pushDword(stream, s_fileOffset, LSB);
        pushDword(stream, s_virtualAddress, LSB);
        pushDword(stream, s_physAddress, LSB);
        pushDword(stream, s_sizeFile, LSB);
        pushDword(stream, s_sizeMemory, LSB);
        pushDword(stream, s_flags, LSB);
        pushDword(stream, s_align, LSB);
    }
};
struct elfSegmentHdr64 {
    uint32_t s_type;            // word
    uint32_t s_flags;           // word
    uint64_t s_fileOffset;      // Offset
    uint64_t s_virtualAddress;  // Address
    uint64_t s_physAddress;     // Address
    uint64_t s_sizeFile;        // dword
    uint64_t s_sizeMemory;      // dword
    uint64_t s_align;           // dword
    elfSegmentHdr64(const uint32_t& type, const uint32_t& flags) {
        // segment type
        if (type == ELF_SEGMENT_TYPE_LOAD || type == ELF_SEGMENT_TYPE_DYN || type == ELF_SEGMENT_TYPE_INTP || type == ELF_SEGMENT_TYPE_NOTE || type == ELF_SEGMENT_TYPE_PHDR || type == ELF_SEGMENT_TYPE_TLS)
            s_type = type;
        else
            s_type = ELF_SEGMENT_TYPE_INV;
        s_fileOffset = 0x0000000000000000;  // currently unknown
        s_virtualAddress = 0x0000000000000000;  // currently unknown
        s_physAddress = 0x0000000000000000;
        s_sizeFile = 0x0000000000000000;  // currently unknown
        s_sizeMemory = 0x0000000000000000;  // currently unknown
        s_flags = flags;// flags
        s_align = 1;// bit aligment
    }
    void push(std::ofstream& stream) {
        bool LSB = elf_encoding == ELF_ENCODING_LSB;  // is little endian
        pushDword(stream, s_type, LSB);
        pushDword(stream, s_flags, LSB);
        pushQword(stream, s_fileOffset, LSB);
        pushQword(stream, s_virtualAddress, LSB);
        pushQword(stream, s_physAddress, LSB);
        pushQword(stream, s_sizeFile, LSB);
        pushQword(stream, s_sizeMemory, LSB);
        pushQword(stream, s_align, LSB);
    }
    bool read(std::ifstream& stream, uint32_t& count) {
        try {
            s_type = readDword(stream, count);
            s_flags = readDword(stream, count);
            s_fileOffset = readQword(stream, count);
            s_virtualAddress = readQword(stream, count);
            s_physAddress = readQword(stream, count);
            s_sizeFile = readQword(stream, count);
            s_sizeMemory = readQword(stream, count);
            s_align = readQword(stream, count);
            return true;
        } catch (int code) { return false; }
    }
    bool readAt(std::vector<uint8_t>& vec, const uint32_t& index) {
        try {
            s_type = readDwordAt(vec, index);
            s_flags = readDwordAt(vec, index+4);
            s_fileOffset = readQwordAt(vec, index+8);
            s_virtualAddress = readQwordAt(vec, index+16);
            s_physAddress = readQwordAt(vec, index+24);
            s_sizeFile = readQwordAt(vec, index+32);
            s_sizeMemory = readQwordAt(vec, index+40);
            s_align = readQwordAt(vec, index+48);
            return true;
        } catch (int code) { return false; }
    }
    void print(std::string delimmiter="\n") {
        std::cout << "s_type: " << intToHex(s_type) << " -> ";
        if ((s_type&ELF_SEGMENT_TYPE_LOAD)>0) std::cout << "LOAD";
        else if ((s_type&ELF_SEGMENT_TYPE_DYN)>0) std::cout << "DYNAMIC";
        else if ((s_type&ELF_SEGMENT_TYPE_INTP)>0) std::cout << "INTERP";
        else if ((s_type&ELF_SEGMENT_TYPE_NOTE)>0) std::cout << "NOTE";
        else if ((s_type&ELF_SEGMENT_TYPE_PHDR)>0) std::cout << "Program Header Table";
        else if ((s_type&ELF_SEGMENT_TYPE_TLS)>0) std::cout << "Thread local storage";
        std::cout << delimmiter;
        std::cout << "s_flags: " << intToHex(s_flags) << " -> ";
        if ((s_flags&ELF_SEGMENT_FLAG_READ)>0) std::cout << "R"; else std::cout << "-";
        if ((s_flags&ELF_SEGMENT_FLAG_WRITE)>0) std::cout << "W"; else std::cout << "-";
        if ((s_flags&ELF_SEGMENT_FLAG_EXECUTE)>0) std::cout << "E"; else std::cout << "-";
        std::cout << delimmiter;
        std::cout << "s_fileOffset: " << intToHex(s_fileOffset) << delimmiter;
        std::cout << "s_virtualAddress: " << intToHex(s_virtualAddress) << delimmiter;
        std::cout << "s_physAddress: " << intToHex(s_physAddress) << delimmiter;
        std::cout << "s_sizeFile: " << intToHex(s_sizeFile) << delimmiter;
        std::cout << "s_sizeMemory: " << intToHex(s_sizeMemory) << delimmiter;
        std::cout << "s_align: " << intToHex(s_align);
        std::cout << '\n';
    }
};

struct elfSectionHdr32 {};

struct elf64Symbol {// 24 bytes
    uint32_t sy_name_idx; // index to string table defined by section header
    uint8_t sy_typeNbinding; // Type and binding attributes
    uint8_t sy_other; // must be 0
    uint16_t sy_secIdx; // Associated section index
    uint64_t sy_value;
    uint64_t sy_size;
    elf64Symbol() {
        sy_name_idx=0;
        sy_typeNbinding=0;
        sy_other=0;
        sy_secIdx=0;
        sy_value=0;
        sy_size=0;
    }
    bool readAt(std::vector<uint8_t>& vec, const uint32_t& index) {
        try {
            sy_name_idx = readDwordAt(vec, index);
            sy_typeNbinding = readByteAt(vec, index+4);
            sy_other = readByteAt(vec, index+5);
            sy_secIdx = readWordAt(vec, index+6);
            sy_value = readQwordAt(vec, index+8);
            sy_size = readQwordAt(vec, index+16);
            return true;
        } catch (int code) { return false; }
    }
    void print(std::vector<uint8_t>& file, const uint32_t& strTabOffset, std::string delimmiter="\n") {
        std::cout << "{" << delimmiter << "    ";
        std::cout << "name_idx: " << sy_name_idx << " -> \"" << readStringAt(file,strTabOffset+sy_name_idx) << "\"";
        std::cout << ',' << delimmiter << "    ";
        std::cout << "type&binding: " << intToHex(sy_typeNbinding) << ',' << delimmiter << "        ";
        const uint8_t type = sy_typeNbinding&0x0F;
        const uint8_t binding = sy_typeNbinding>>4;
        std::cout << "type: " << intToHex(type) << " -> ";
        if (type==ELF_SYMBOL_TYPE_NOTYPE) std::cout << "NOTYPE";
        else if (type==ELF_SYMBOL_TYPE_OBJECT) std::cout << "OBJECT";
        else if (type==ELF_SYMBOL_TYPE_FUNC) std::cout << "FUNC";
        else if (type==ELF_SYMBOL_TYPE_SECTION) std::cout << "SECTION IDX";
        else if (type==ELF_SYMBOL_TYPE_FILE) std::cout << "FILE";
        else if (type==ELF_SYMBOL_TYPE_COMMON) std::cout << "COMMON";
        else if (type==ELF_SYMBOL_TYPE_TLS) std::cout << " TLS";
        std::cout << ',' << delimmiter << "        binding: " << intToHex(binding) << " -> ";
        if (binding==ELF_SYMBOL_BINDING_LOCAL) std::cout << "LOCAL";
        else if (binding==ELF_SYMBOL_BINDING_GLOBAL) std::cout << "GLOBAL";
        else if (binding==ELF_SYMBOL_BINDING_WEAK) std::cout << "WEAK";
        std::cout << ',' << delimmiter << "    ";
        std::cout << "secIdx: " << sy_secIdx << ',' << delimmiter << "    ";
        std::cout << "value: " << intToHex(sy_value) << ',' << delimmiter << "    ";
        std::cout << "size: " << intToHex(sy_size) << ',' << delimmiter << "    ";
        std::cout << delimmiter << "}\n";
    }
};
struct elf64Dyn {// 16 bytes
    uint64_t dy_tag;
    uint64_t dy_valPtr;
    bool readAt(std::vector<uint8_t>& vec, const uint32_t& index) {
        try {
            dy_tag = readQwordAt(vec, index);
            dy_valPtr = readQwordAt(vec, index+8);
            return true;
        } catch (int code) { return false; }
    }
    void print(std::vector<uint8_t>& file, const uint32_t& exportStrTabOffset, std::vector<elfSectionHdr64>& sections, const uint32_t& secNameStrTableOffset);
};
struct elf64Rel {// 16 bytes
    uint64_t r_offset;
    uint64_t r_info;
    bool readAt(std::vector<uint8_t>& vec, const uint32_t& index) {
        try {
            r_offset = readQwordAt(vec, index);
            r_info = readQwordAt(vec, index+8);
            return true;
        } catch (int code) { return false; }
    }
};
struct elf64Rela {// 24 bytes
    uint64_t r_offset;
    uint32_t r_symtabIndex;
    uint32_t r_type;
    int64_t r_addend;
    bool readAt(std::vector<uint8_t>& vec, const uint32_t& index) {
        try {
            r_offset = readQwordAt(vec, index);
            r_symtabIndex = readDwordAt(vec, index+12);
            r_type = readDwordAt(vec, index+8);
            r_addend = readQwordAt(vec, index+16);
            return true;
        } catch (int code) { return false; }
    }
    void print(std::vector<uint8_t>& file, std::vector<elfSectionHdr64>& sections, const uint32_t& secNameStrTableOffset, std::string delimmiter="\n");
};
struct elfSectionHdr64 {// 64 bytes
    uint32_t s_nameIdx;// index to a section of type ELF_SECTION_TYPE_STRTAB
    uint32_t s_type;
    uint64_t s_flags;
    uint64_t s_virtualAddress;
    uint64_t s_fileOffset;
    uint64_t s_size;
    uint32_t s_link;// index to link to another section, used for several different things
    uint32_t s_info;// just some information, again, used for several different things
    uint64_t s_align;
    uint64_t s_entSize;// entry size, used for sections contains lists of constant sized things.
    elfSectionHdr64(const uint32_t& type, const uint32_t& flags) {
        s_nameIdx=0;
        s_type=type;
        s_flags=flags;
        s_virtualAddress=0;
        s_fileOffset=0;
        s_size=0;
        s_link=0;
        s_info=0;
        s_align=0;
        s_entSize=0;
    }
    void push(std::ofstream& stream) {
        bool LSB = elf_encoding == ELF_ENCODING_LSB;  // is little endian
        pushDword(stream, s_nameIdx, LSB);
        pushDword(stream, s_type, LSB);
        pushQword(stream, s_flags, LSB);
        pushQword(stream, s_virtualAddress, LSB);
        pushQword(stream, s_fileOffset, LSB);
        pushQword(stream, s_size, LSB);
        pushDword(stream, s_link, LSB);
        pushDword(stream, s_info, LSB);
        pushQword(stream, s_align, LSB);
        pushQword(stream, s_entSize, LSB);
    }
    bool read(std::ifstream& stream, uint32_t& count) {
        try {
            s_nameIdx = readDword(stream, count);
            s_type = readDword(stream, count);
            s_flags = readQword(stream, count);
            s_virtualAddress = readQword(stream, count);
            s_fileOffset = readQword(stream, count);
            s_size = readQword(stream, count);
            s_link = readDword(stream, count);
            s_info = readDword(stream, count);
            s_align = readQword(stream, count);
            s_entSize = readQword(stream, count);
            return true;
        } catch (int code) { return false; }
    }
    bool readAt(std::vector<uint8_t>& vec, const uint32_t& index) {
        try {
            s_nameIdx = readDwordAt(vec, index);
            s_type = readDwordAt(vec, index+4);
            s_flags = readQwordAt(vec, index+8);
            s_virtualAddress = readQwordAt(vec, index+16);
            s_fileOffset = readQwordAt(vec, index+24);
            s_size = readQwordAt(vec, index+32);
            s_link = readDwordAt(vec, index+40);
            s_info = readDwordAt(vec, index+44);
            s_align = readQwordAt(vec, index+48);
            s_entSize = readQwordAt(vec, index+56);
            return true;
        } catch (int code) { return false; }
    }
    void print(std::vector<uint8_t>& file, const uint32_t& strTabOffset, std::string delimmiter="\n") {
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
        if (s_flags==0) {
            std::cout << "NONE/NULL";
        } else if (!hasFlagWrite&&!hasFlagAlloc&&!hasFlagExec&&!hasFlagMerge&&!hasFlagStrings&&!hasFlagGROUP&&!hasFlagTLS)
            std::cout << "unknown";
        else {
            if (hasFlagWrite) std::cout << "Writable";
            if (hasFlagWrite&&(hasFlagAlloc||hasFlagExec||hasFlagMerge||hasFlagStrings||hasFlagGROUP||hasFlagTLS)) std::cout << ", ";
            if (hasFlagAlloc) std::cout << "ALLOC";
            if (hasFlagAlloc&&(hasFlagExec||hasFlagMerge||hasFlagStrings||hasFlagGROUP||hasFlagTLS)) std::cout << ", ";
            if (hasFlagExec) std::cout << "Executable";
            if (hasFlagExec&&(hasFlagMerge||hasFlagStrings||hasFlagGROUP||hasFlagTLS)) std::cout << ", ";
            if (hasFlagMerge) std::cout << "Might merge";
            if (hasFlagMerge&&(hasFlagStrings||hasFlagGROUP||hasFlagTLS)) std::cout << ", ";
            if (hasFlagStrings) std::cout << "Contains strings";
            if (hasFlagStrings&&(hasFlagGROUP||hasFlagTLS)) std::cout << ", ";
            if (hasFlagGROUP) std::cout << "Is in group";
            if (hasFlagGROUP&&hasFlagTLS) std::cout << ", ";
            if (hasFlagTLS) std::cout << "TLS";
        }
        std::cout << delimmiter;
        std::cout << "s_virtualAddress: " << intToHex(s_virtualAddress) << delimmiter;
        std::cout << "s_fileOffset: " << intToHex(s_fileOffset) << delimmiter;
        std::cout << "s_size: " << intToHex(s_size) << delimmiter;
        std::cout << "s_link: " << intToHex(s_link) << delimmiter;
        std::cout << "s_info: " << intToHex(s_info) << delimmiter;
        std::cout << "s_align: " << intToHex(s_align) << delimmiter;
        std::cout << "s_entSize: " << intToHex(s_entSize);
        std::cout << '\n';
    }
};

extern uint16_t elf_type;
extern uint8_t elf_osAbi;
extern uint16_t elf_machine;

struct elfHdr32 {
    uint8_t e_magic[4];         // 4 bytes
    uint8_t e_architecture;     // byte
    uint8_t e_encoding;         // byte
    uint8_t e_metadataVersion;  // byte
    uint8_t e_abi;              // byte
    uint8_t e_abiVersion;       // byte
    uint8_t e_padding[7];       // 7 bytes
    uint16_t e_fileType;        // half word
    uint16_t e_machineType;     // half word
    uint32_t e_version;         // word
    uint32_t e_entryAddress;    // Address
    uint32_t e_segmentHdrOffset;// Offset
    uint32_t e_sectionHdrOffset;// Offset
    uint32_t e_flags;           // word
    uint16_t e_elfHdrSize;      // half word
    uint16_t e_segmentHdrSize;  // half word
    uint16_t e_numSegmentHdrs;  // half word
    uint16_t e_sectionHdrSize;  // half word
    uint16_t e_numSectionHdrs;  // half word
    uint16_t e_stringTableNdx;  // half word

    elfHdr32() {
        e_magic[0] = 0x7f;
        e_magic[1] = 'E';
        e_magic[2] = 'L';
        e_magic[3] = 'F';
        // architecture
        e_architecture = ELF_ARCHITECTURE_32;
        // data encoding
        if (elf_encoding == ELF_ENCODING_MSB || elf_encoding == ELF_ENCODING_LSB)
            e_encoding = elf_encoding;
        else
            e_encoding = ELF_ENCODING_INV;
        // metadata version
        e_metadataVersion = 0x01;
        // OS ABI
        if (elf_osAbi == ELF_OS_NONE || elf_osAbi == ELF_OS_HPUX || elf_osAbi == ELF_OS_NETBSD || elf_osAbi == ELF_OS_LINUX)
            e_abi = elf_osAbi;
        else
            e_abi = ELF_OS_INV;
        // ABI version
        e_abiVersion = 0x00;
        // padding
        e_padding[0] = 0x00;
        e_padding[1] = 0x00;
        e_padding[2] = 0x00;
        e_padding[3] = 0x00;
        e_padding[4] = 0x00;
        e_padding[5] = 0x00;
        e_padding[6] = 0x00;
        // file type
        if (elf_type == ELF_TYPE_REL || elf_type == ELF_TYPE_EXE || elf_type == ELF_TYPE_SO || elf_type == ELF_TYPE_CORE)
            e_fileType = elf_type;
        else
            e_fileType = ELF_TYPE_INV;
        // machine type
        if (elf_machine == ELF_MACHINE_x86 || elf_machine == ELF_MACHINE_MIPS || elf_machine == ELF_MACHINE_ARM || elf_machine == ELF_MACHINE_AMD64 || elf_machine == ELF_MACHINE_ARM8 || elf_machine == ELF_MACHINE_RISCV)
            e_machineType = elf_machine;
        else
            e_machineType = ELF_TYPE_INV;
        // version
        e_version = 0x00000001;
        // entry
        e_entryAddress = 0x00000000;
        // phoff
        e_segmentHdrOffset = sizeof(elfHdr32);
        // shoff
        e_sectionHdrOffset = 0x00000000;
        // flags
        e_flags = 0x00000000;
        // ehsize
        e_elfHdrSize = sizeof(elfHdr32);
        // phentsize
        e_segmentHdrSize = sizeof(elfSegmentHdr32);
        // phnum
        e_numSegmentHdrs = 0x0000;
        // shentsize
        e_sectionHdrSize = 0;  // sizeof(sectionHdr32);
        // shnum
        e_numSectionHdrs = 0x0000;
        // shstrndx
        e_stringTableNdx = 0x0000;
    }

    void push(std::ofstream& stream) const {
        bool LSB = e_encoding == ELF_ENCODING_LSB;  // is little endian
        for (uint8_t i = 0; i < 4; i++) pushByte(stream, e_magic[i]);
        pushByte(stream, e_architecture);
        pushByte(stream, e_encoding);
        pushByte(stream, e_metadataVersion);
        pushByte(stream, e_abi);
        pushByte(stream, e_abiVersion);
        for (uint8_t i = 0; i < 7; i++) pushByte(stream, e_padding[i]);
        pushWord(stream, e_fileType, LSB);
        pushWord(stream, e_machineType, LSB);
        pushDword(stream, e_version, LSB);
        pushDword(stream, e_entryAddress, LSB);
        pushDword(stream, e_segmentHdrOffset, LSB);
        pushDword(stream, e_sectionHdrOffset, LSB);
        pushDword(stream, e_flags, LSB);
        pushWord(stream, e_elfHdrSize, LSB);
        pushWord(stream, e_segmentHdrSize, LSB);
        pushWord(stream, e_numSegmentHdrs, LSB);
        pushWord(stream, e_sectionHdrSize, LSB);
        pushWord(stream, e_numSectionHdrs, LSB);
        pushWord(stream, e_stringTableNdx, LSB);
    }
};
struct elfHdr64 {// 63 bytes?
    uint8_t e_magic[4];          // 4 bytes
    uint8_t e_architecture;      // byte
    uint8_t e_encoding;          // byte
    uint8_t e_metadataVersion;   // byte
    uint8_t e_abi;               // byte
    uint8_t e_abiVersion;        // byte
    uint8_t e_padding[7];        // 7 bytes
    uint16_t e_fileType;         // half word
    uint16_t e_machineType;      // half word
    uint32_t e_version;          // word
    uint64_t e_entryAddress;     // Address
    uint64_t e_segmentHdrOffset; // Offset
    uint64_t e_sectionHdrOffset; // Offset
    uint32_t e_flags;            // word
    uint16_t e_elfHdrSize;       // half word
    uint16_t e_segmentHdrSize;   // half word
    uint16_t e_numSegmentHdrs;   // half word
    uint16_t e_sectionHdrSize;   // half word
    uint16_t e_numSectionHdrs;   // half word
    uint16_t e_stringTableNdx;   // half word
    elfHdr64() {
        e_magic[0] = 0x7f;
        e_magic[1] = 'E';
        e_magic[2] = 'L';
        e_magic[3] = 'F';
        e_architecture = ELF_ARCHITECTURE_64;
        e_encoding = ELF_ENCODING_LSB;
        e_metadataVersion = 0x01;
        if (elf_osAbi == ELF_OS_NONE || elf_osAbi == ELF_OS_HPUX || elf_osAbi == ELF_OS_NETBSD || elf_osAbi == ELF_OS_LINUX)
            e_abi = elf_osAbi;
        else
            e_abi = ELF_OS_INV;
        e_abiVersion = 0x00;
        e_padding[0] = 0x00;
        e_padding[1] = 0x00;
        e_padding[2] = 0x00;
        e_padding[3] = 0x00;
        e_padding[4] = 0x00;
        e_padding[5] = 0x00;
        e_padding[6] = 0x00;
        if (elf_type == ELF_TYPE_REL || elf_type == ELF_TYPE_EXE || elf_type == ELF_TYPE_SO || elf_type == ELF_TYPE_CORE)
            e_fileType = elf_type;
        else
            e_fileType = ELF_TYPE_INV;
        if (elf_machine == ELF_MACHINE_NONE || elf_machine == ELF_MACHINE_x86 || elf_machine == ELF_MACHINE_MIPS || elf_machine == ELF_MACHINE_ARM || elf_machine == ELF_MACHINE_AMD64 || elf_machine == ELF_MACHINE_ARM8 || elf_machine == ELF_MACHINE_RISCV)
            e_machineType = elf_machine;
        else
            e_machineType = ELF_TYPE_INV;
        e_version = 0x00000001;
        e_entryAddress = 0x0000000000000000;
        e_segmentHdrOffset = sizeof(elfHdr64);
        e_sectionHdrOffset = 0x0000000000000000;
        e_flags = 0x00000000;
        e_elfHdrSize = sizeof(elfHdr64);
        e_segmentHdrSize = sizeof(elfSegmentHdr64);
        e_numSegmentHdrs = 0x0000;
        e_sectionHdrSize = 0;  // sizeof(sectionHdr64);
        e_numSectionHdrs = 0x0000;
        e_stringTableNdx = 0x0000;
    }
    void push(std::ofstream& stream) const {
        bool LSB = e_encoding == ELF_ENCODING_LSB;  // is little endian
        for (uint8_t i = 0; i < 4; i++) pushByte(stream, e_magic[i]);
        pushByte(stream, e_architecture);
        pushByte(stream, e_encoding);
        pushByte(stream, e_metadataVersion);
        pushByte(stream, e_abi);
        pushByte(stream, e_abiVersion);
        for (uint8_t i = 0; i < 7; i++) pushByte(stream, e_padding[i]);
        pushWord(stream, e_fileType, LSB);
        pushWord(stream, e_machineType, LSB);
        pushDword(stream, e_version, LSB);
        pushQword(stream, e_entryAddress, LSB);
        pushQword(stream, e_segmentHdrOffset, LSB);
        pushQword(stream, e_sectionHdrOffset, LSB);
        pushDword(stream, e_flags, LSB);
        pushWord(stream, e_elfHdrSize, LSB);
        pushWord(stream, e_segmentHdrSize, LSB);
        pushWord(stream, e_numSegmentHdrs, LSB);
        pushWord(stream, e_sectionHdrSize, LSB);
        pushWord(stream, e_numSectionHdrs, LSB);
        pushWord(stream, e_stringTableNdx, LSB);
    }
    bool read(std::ifstream& stream, uint32_t& count) {
        try {
            // get PE header
            if ((readChar(stream,count)!=0x7f)||(readChar(stream,count)!='E')||(readChar(stream,count)!='L')||(readChar(stream,count)!='F')) { std::cout << "File not ELF file" << std::endl; return false; }
            e_magic[0] = 0x7f;
            e_magic[1] = 'E';
            e_magic[2] = 'L';
            e_magic[3] = 'F';
            e_architecture = readByte(stream, count);
            if (e_architecture!=ELF_ARCHITECTURE_64) { std::cout << "ELF header e_architecture bit not supported." << std::endl; return false; }
            e_encoding = readByte(stream, count);
            if (e_encoding!=ELF_ENCODING_LSB) { std::cout << "ELF header e_encoding not supported." << std::endl; return false; }
            e_metadataVersion = readByte(stream, count);
            if (e_metadataVersion!=1) { std::cout << "ELF header e_metadataVersion invalid." << std::endl; return false; }
            e_abi = readByte(stream, count);
            if ((e_abi!=ELF_OS_LINUX)&&(e_abi!=ELF_OS_NONE)) { std::cout << "ELF header not linux e_abi not supported." << std::endl; return false; }
            e_abiVersion = readByte(stream, count);
            if (e_abiVersion!=0) { std::cout << "ELF invalid e_abiVersion." << std::endl; return false; }
            for (size_t i = 0; i < 7; i++) e_padding[i]=readByte(stream, count);
            e_fileType = readWord(stream, count);
            e_machineType = readWord(stream, count);
            if ((e_machineType!=ELF_MACHINE_AMD64)&&(e_machineType!=ELF_MACHINE_NONE)) { std::cout << "ELF header e_machineType unknown." << std::endl; return false; }
            e_version = readDword(stream, count);
            e_entryAddress = readQword(stream, count);
            e_segmentHdrOffset = readQword(stream, count);
            e_sectionHdrOffset = readQword(stream, count);
            e_flags = readDword(stream, count);
            e_elfHdrSize = readWord(stream, count);
            if (e_elfHdrSize!=sizeof(elfHdr64)) { std::cout << "ELF header invalid e_elfHdrSize." << std::endl; return false; }
            e_segmentHdrSize = readWord(stream, count);
            if (e_segmentHdrSize!=sizeof(elfSegmentHdr64)) { std::cout << "ELF header invalid e_segmentHdrSize." << std::endl; return false; }
            e_numSegmentHdrs = readWord(stream, count);
            e_sectionHdrSize = readWord(stream, count);
            e_numSectionHdrs = readWord(stream, count);
            e_stringTableNdx = readWord(stream, count);
            return true;
        } catch (int code) { return false; }
    }
    bool readAt(std::vector<uint8_t>& vec, const uint32_t& index) {
        try {
            // get PE header
            if ((readCharAt(vec,index)!=0x7f)||(readCharAt(vec,index+1)!='E')||(readCharAt(vec,index+2)!='L')||(readCharAt(vec,index+3)!='F')) { std::cout << "File not ELF file" << std::endl; return false; }
            e_magic[0] = 0x7f;
            e_magic[1] = 'E';
            e_magic[2] = 'L';
            e_magic[3] = 'F';
            e_architecture = readByteAt(vec, index+4);
            if (e_architecture!=ELF_ARCHITECTURE_64) { std::cout << "ELF header e_architecture bit not supported." << std::endl; return false; }
            e_encoding = readByteAt(vec, index+5);
            if (e_encoding!=ELF_ENCODING_LSB) { std::cout << "ELF header e_encoding not supported." << std::endl; return false; }
            e_metadataVersion = readByteAt(vec, index+6);
            if (e_metadataVersion!=1) { std::cout << "ELF header e_metadataVersion invalid." << std::endl; return false; }
            e_abi = readByteAt(vec, index+7);
            if ((e_abi!=ELF_OS_LINUX)&&(e_abi!=ELF_OS_NONE)) { std::cout << "ELF header not linux e_abi not supported." << std::endl; return false; }
            e_abiVersion = readByteAt(vec, index+8);
            if (e_abiVersion!=0) { std::cout << "ELF invalid e_abiVersion." << std::endl; return false; }
            for (size_t i = 0; i < 7; i++) e_padding[i]=readByteAt(vec, index+9+i);
            e_fileType = readWordAt(vec, index+16);
            e_machineType = readWordAt(vec, index+18);
            if ((e_machineType!=ELF_MACHINE_AMD64)&&(e_machineType!=ELF_MACHINE_NONE)) { std::cout << "ELF header e_machineType unknown." << std::endl; return false; }
            e_version = readDwordAt(vec, index+20);
            e_entryAddress = readQwordAt(vec, index+24);
            e_segmentHdrOffset = readQwordAt(vec, index+32);
            e_sectionHdrOffset = readQwordAt(vec, index+40);
            e_flags = readDwordAt(vec, index+48);
            e_elfHdrSize = readWordAt(vec, index+52);
            if (e_elfHdrSize!=sizeof(elfHdr64)) { std::cout << "ELF header invalid e_elfHdrSize." << std::endl; return false; }
            e_segmentHdrSize = readWordAt(vec, index+54);
            if (e_segmentHdrSize!=sizeof(elfSegmentHdr64)) { std::cout << "ELF header invalid e_segmentHdrSize." << std::endl; return false; }
            e_numSegmentHdrs = readWordAt(vec, index+56);
            e_sectionHdrSize = readWordAt(vec, index+58);
            e_numSectionHdrs = readWordAt(vec, index+60);
            e_stringTableNdx = readWordAt(vec, index+62);
            return true;
        } catch (int code) { return false; }
    }
    void print(std::string delimmiter="\n") {
        std::cout << "p_magic: \"\\7f,E,L,F\"" << delimmiter;
        std::cout << "e_architecture: " << intToHex(e_architecture) << " -> 64bit" << delimmiter;
        std::cout << "e_encoding: " << intToHex(e_encoding) << " -> LSB (Least Significant Bit)" << delimmiter;
        std::cout << "e_metadataVersion: " << (int)e_metadataVersion << delimmiter;
        std::cout << "e_abi: " << intToHex(e_abi) << " -> ";
        if ((e_abi&ELF_OS_LINUX)>0) std::cout << "linux";
        else if (e_abi==ELF_OS_NONE) std::cout << "none/nondescript";
        std::cout << delimmiter;
        std::cout << "e_abiVersion: " << (int)e_abiVersion << delimmiter;
        std::cout << "e_paddin: \"\\0,\\0,\\0,\\0,\\0,\\0,\\0\"" << delimmiter;
        std::cout << "e_fileType: " << intToHex(e_fileType) << " -> ";
        const bool isRelocatable = ((e_fileType&ELF_TYPE_REL)>0);
        const bool isExecutable = ((e_fileType&ELF_TYPE_EXE)>0);
        const bool isSo = isRelocatable&isExecutable;// equivilent to (e_fileType&ELF_TYPE_SO)>0)
        const bool isCore = ((e_fileType&ELF_TYPE_CORE)>0);
        if (!isRelocatable&&!isExecutable&&!isSo&&!isCore) std::cout << "Unknown";
        else if (isSo) std::cout << "Shared Object (Dynamic Linked Library)";
        else {
            if (isRelocatable) std::cout << "Relocatable";
            if (isRelocatable&&(isExecutable||isCore)) std::cout << ", ";
            if (isExecutable) std::cout << "Executable";
            if (isExecutable&&isCore) std::cout << ", ";
            if (isCore) std::cout << "Core";
        }
        std::cout << delimmiter;
        std::cout << "e_machineType: " << intToHex(e_machineType) << " -> ";
        if ((e_machineType&ELF_MACHINE_AMD64)>0) std::cout << "AMD64";
        else if (e_machineType==ELF_MACHINE_NONE) std::cout << "none/nondescript";
        std::cout << delimmiter;
        std::cout << "e_version: " << e_version << delimmiter;
        std::cout << "e_entryAddress: " << intToHex(e_entryAddress) << delimmiter;
        std::cout << "e_segmentHdrOffset: " << intToHex(e_segmentHdrOffset) << delimmiter;
        std::cout << "e_sectionHdrOffset: " << intToHex(e_sectionHdrOffset) << delimmiter;
        std::cout << "e_flags: " << intToHex(e_flags) << delimmiter;
        std::cout << "e_elfHdrSize: " << intToHex(e_elfHdrSize) << delimmiter;
        std::cout << "e_segmentHdrSize: " << intToHex(e_segmentHdrSize) << delimmiter;
        std::cout << "e_numSegmentHdrs: " << e_numSegmentHdrs << delimmiter;
        std::cout << "e_sectionHdrSize: " << intToHex(e_sectionHdrSize) << delimmiter;
        std::cout << "e_numSectionHdrs: " << e_numSectionHdrs << delimmiter;
        std::cout << "e_stringTableNdx: " << e_stringTableNdx << '\n';
    }
};
#pragma endregion structs

struct soExportData {
    std::string name;
    std::string soName;
};
std::vector<soExportData> parseSo(const std::string& name);

#pragma region handlers
class Elf64SegmentHandler;

struct Elf64Label {
    std::string name;
    Elf64SegmentHandler* base;
    uint32_t offset;
    Elf64Label(const std::string& _name, Elf64SegmentHandler* _base, const uint32_t& _offset) : name(_name),base(_base),offset(_offset) {};
};
struct Elf64LabelResolution {
    std::string name;
    Elf64SegmentHandler* base;
    uint32_t setAt;
    int32_t relativeToOffset;
    Elf64LabelResolution(const std::string& _name, Elf64SegmentHandler* _base, const uint32_t& _setAt, const int32_t& _relativeToOffset) : name(_name),base(_base),setAt(_setAt),relativeToOffset(_relativeToOffset) {};
};
class Elf64Handler {
private:
    std::vector<Elf64SegmentHandler *> segmentHeaders;
    elfHdr64 elfHeader;

    std::vector<Elf64Label> labels;
    std::vector<Elf64LabelResolution> labelResolutions;

public:
    Elf64Handler();
    constexpr bool isLSB() const {
        return elfHeader.e_encoding == ELF_ENCODING_LSB;
    };
    void push(std::ofstream& stream);
    Elf64SegmentHandler *addSeg(const uint32_t& type, const uint32_t& flags);
    void defineLabel(const std::string& name, Elf64SegmentHandler* base, const uint32_t& offset);
    void resolveLabel(const std::string& name, Elf64SegmentHandler* base, const uint32_t& setAt, const int32_t& relativeToOffset);
};

class Elf64SegmentHandler {
    Elf64Handler& elfHandler;
    elfSegmentHdr64 segmentHeader;
public:
    uint32_t sectionAlignment=1;
    uint32_t fileAlignment=1;
    std::vector<uint8_t> data;

    Elf64SegmentHandler(Elf64Handler& _elfHandler, const uint32_t& type, const uint32_t& flags);
    constexpr bool isLSB() const {
        return elfHandler.isLSB();
    };
    void pushHeader(std::ofstream& stream);
    void pushData(std::ofstream& stream);

    uint32_t getSize();
    void setSectionAlign(const uint32_t& align);
    void setFileAlign(const uint32_t& align);
    void setOffset(const uint32_t& offset);
    uint32_t getOffset();
    void setRVA(const uint32_t& Rva);
    uint32_t getRVA();

    // calls the Pe64Handler with "base" set to this, "offset" set to the current position
    void defineLabel(const std::string& name);
    // calls the Pe64Handler with "base" set to this, "setAt" set to the current position - relativeToOffset
    void resolveLabel(const std::string& name, const int32_t& relativeToOffset);

    friend Elf64Handler;
    template <typename T>
    friend void pushChars(T& segment, const uint8_t* chars, const size_t& len, const bool& LSB);
};
template <>
void pushChars(Elf64SegmentHandler*& reciever, const uint8_t* chars, const size_t& len, const bool& LSB);
#pragma endregion  // handlers

#endif  // _ELF_H
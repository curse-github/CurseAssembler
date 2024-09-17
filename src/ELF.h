#ifndef _ELF_H
#define _ELF_H

#include <vector>

#include "elfConstants.h"
#include "globalConstants.h"

extern uint8_t elf_encoding;
extern uint16_t elf_type;
extern uint8_t elf_osAbi;
extern uint16_t elf_machine;

#pragma region structs
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
        if (type == ELF_SEGMENT_TYPE_LOAD || type == ELF_SEGMENT_TYPE_DYN || type == ELF_SEGMENT_TYPE_INTP || type == ELF_SEGMENT_TYPE_NT || type == ELF_SEGMENT_TYPE_PHDR || type == ELF_SEGMENT_TYPE_TLS)
            s_type = type;
        else
            s_type = ELF_SEGMENT_TYPE_INV;
        // file offset
        s_fileOffset = 0x00000000;  // currently unknown
        // virtual address
        s_virtualAddress = 0x00000000;  // currently unknown
        // physical address
        s_physAddress = 0x00000000;
        // size in the file
        s_sizeFile = 0x00000000;  // currently unknown
        // size in memory
        s_sizeMemory = 0x00000000;  // currently unknown
        // flags
        s_flags = flags;
        // bit aligment
        s_align = SECTION_ALIGN;
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
    uint32_t s_type;             // word
    uint32_t s_flags;            // word
    uint64_t s_fileOffset;      // Offset
    uint64_t s_virtualAddress;  // Address
    uint64_t s_physAddress;     // Address
    uint64_t s_sizeFile;        // dword
    uint64_t s_sizeMemory;      // dword
    uint64_t s_align;           // dword

    elfSegmentHdr64(const uint32_t& type, const uint32_t& flags) {
        // segment type
        if (type == ELF_SEGMENT_TYPE_LOAD || type == ELF_SEGMENT_TYPE_DYN || type == ELF_SEGMENT_TYPE_INTP || type == ELF_SEGMENT_TYPE_NT || type == ELF_SEGMENT_TYPE_PHDR || type == ELF_SEGMENT_TYPE_TLS)
            s_type = type;
        else
            s_type = ELF_SEGMENT_TYPE_INV;
        // file offset
        s_fileOffset = 0x0000000000000000;  // currently unknown
        // virtual address
        s_virtualAddress = 0x0000000000000000;  // currently unknown
        // physical address
        s_physAddress = 0x0000000000000000;
        // size in the file
        s_sizeFile = 0x0000000000000000;  // currently unknown
        // size in memory
        s_sizeMemory = 0x0000000000000000;  // currently unknown
        // flags
        s_flags = flags;
        // bit aligment
        s_align = 1;// not set yet
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
};

struct elfSectionHdr32 {};
struct elfSectionHdr64 {};

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

struct elfHdr64 {
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
        // architecture
        e_architecture = ELF_ARCHITECTURE_64;
        // data encoding
        e_encoding = ELF_ENCODING_LSB;
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
        e_entryAddress = 0x0000000000000000;
        // phoff
        e_segmentHdrOffset = sizeof(elfHdr64);
        // shoff
        e_sectionHdrOffset = 0x0000000000000000;
        // flags
        e_flags = 0x00000000;
        // ehsize
        e_elfHdrSize = sizeof(elfHdr64);
        // phentsize
        e_segmentHdrSize = sizeof(elfSegmentHdr64);
        // phnum
        e_numSegmentHdrs = 0x0000;
        // shentsize
        e_sectionHdrSize = 0;  // sizeof(sectionHdr64);
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
};
#pragma endregion structs

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
    std::vector<uint8_t> data;

    uint32_t sectionAlignment=1;
    uint32_t fileAlignment=1;
public:
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
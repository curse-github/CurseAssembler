#ifndef _PE_H
#define _PE_H

// https://upload.wikimedia.org/wikipedia/commons/1/1b/Portable_Executable_32_bit_Structure_in_SVG_fixed.svg
// https://learn.microsoft.com/en-us/windows/win32/debug/pe-format

#include <time.h>
#include <vector>

#define PE_IS_LSB true

// #include "PE/winnt.h"
#include "globalConstants.h"
#include "peConstants.h"

#pragma region structs
struct peHdr {  // 22 bytes
    uint8_t p_magic[4];
    uint16_t p_machine;
    uint16_t p_numberOfSections;    // limited at 96 sections
    uint32_t p_timeDateStamp;         // seconds since 00:00 January 1, 1970
    uint32_t p_pointerToSymbolTable;  // file offset of COFF symbol table
    uint32_t p_numberOfSymbols;
    uint16_t p_sizeOfOptionalHeader;  // not optional for executables
    uint16_t p_characteristics;
    peHdr() {
        p_magic[0] = 'P';
        p_magic[1] = 'E';
        p_magic[2] = '\0';
        p_magic[3] = '\0';
        p_machine = IMAGE_FILE_MACHINE_AMD64;
        p_numberOfSections = 0;  // currently unknown v
        p_timeDateStamp = time(0);
        p_pointerToSymbolTable = 0;  // currently unknown
        p_numberOfSymbols = 0;       // currently unknown
        p_sizeOfOptionalHeader = 0;  // currently unknown v
        p_characteristics = IMAGE_FILE_CHARACTERISTIC_RELOCS_STRIPPED | IMAGE_FILE_CHARACTERISTIC_EXECUTABLE_IMAGE | IMAGE_FILE_CHARACTERISTIC_32BIT_MACHINE | IMAGE_FILE_CHARACTERISTIC_DEBUG_STRIPPED;
    }
    void push(std::ofstream &stream) {
        pushChars(stream,p_magic,4,PE_IS_LSB);
        pushHalfWord(stream,p_machine,PE_IS_LSB);
        pushHalfWord(stream,p_numberOfSections,PE_IS_LSB);
        pushWord(stream,p_timeDateStamp,PE_IS_LSB);
        pushWord(stream,p_pointerToSymbolTable,PE_IS_LSB);
        pushWord(stream,p_numberOfSymbols,PE_IS_LSB);
        pushHalfWord(stream,p_sizeOfOptionalHeader,PE_IS_LSB);
        pushHalfWord(stream,p_characteristics,PE_IS_LSB);
    }
};

// Optional header

struct peOptHdrStdFields32 {  // 24 bytes
    uint16_t p_magic;
    uint8_t p_majorLinkerVersion;
    uint8_t p_minorLinkerVersion;
    uint32_t p_sizeOfCode;
    uint32_t p_sizeOfInitializedData;
    uint32_t p_sizeOfUninitializedData;
    uint32_t p_addressOfEntryPoint;
    uint32_t p_baseOfCode;
    uint32_t p_baseOfData;  // virtual address??
    peOptHdrStdFields32() {
        p_magic = IMAGE_FILE_MAGIC32;
        p_majorLinkerVersion = 0x02;
        p_minorLinkerVersion = 0x19;
        p_sizeOfCode = 0;               // currently unknown v
        p_sizeOfInitializedData = 0;    // currently unknown v
        p_sizeOfUninitializedData = 0;  // currently unknown v
        p_addressOfEntryPoint = 0;      // currently unknown v?
        p_baseOfCode = 0;               // currently unknown v
        p_baseOfData = 0;               // currently unknown v?
    }
    void push(std::ofstream &stream) {
        pushHalfWord(stream,p_magic,PE_IS_LSB);
        pushByte(stream,p_majorLinkerVersion);
        pushByte(stream,p_minorLinkerVersion);
        pushWord(stream,p_sizeOfCode,PE_IS_LSB);
        pushWord(stream,p_sizeOfInitializedData,PE_IS_LSB);
        pushWord(stream,p_sizeOfUninitializedData,PE_IS_LSB);
        pushWord(stream,p_addressOfEntryPoint,PE_IS_LSB);
        pushWord(stream,p_baseOfCode,PE_IS_LSB);
        pushWord(stream,p_baseOfData,PE_IS_LSB);
    }
};
struct peOptHdrStdFields64 {// 28 bytes
    uint16_t p_magic;
    uint8_t p_majorLinkerVersion;
    uint8_t p_minorLinkerVersion;
    uint32_t p_sizeOfCode;
    uint32_t p_sizeOfInitializedData;
    uint32_t p_sizeOfUninitializedData;
    uint32_t p_addressOfEntryPoint;
    uint32_t p_baseOfCode;
    peOptHdrStdFields64() {
        p_magic = IMAGE_FILE_MAGIC64;
        p_majorLinkerVersion = 0x02;
        p_minorLinkerVersion = 0x19;
        p_sizeOfCode = 0;               // currently unknown v
        p_sizeOfInitializedData = 0;    // currently unknown v
        p_sizeOfUninitializedData = 0;  // currently unknown v
        p_addressOfEntryPoint = 0;      // currently unknown v?
        p_baseOfCode = 0;               // currently unknown v
    }
    void push(std::ofstream &stream) {
        pushHalfWord(stream,p_magic,PE_IS_LSB);
        pushByte(stream,p_majorLinkerVersion);
        pushByte(stream,p_minorLinkerVersion);
        pushWord(stream,p_sizeOfCode,PE_IS_LSB);
        pushWord(stream,p_sizeOfInitializedData,PE_IS_LSB);
        pushWord(stream,p_sizeOfUninitializedData,PE_IS_LSB);
        pushWord(stream,p_addressOfEntryPoint,PE_IS_LSB);
        pushWord(stream,p_baseOfCode,PE_IS_LSB);
    }
};

struct peOptHdrSpecFields32 {
    uint32_t p_imageBase;
    uint32_t p_sectionAlignment;
    uint32_t p_fileAlignment;
    uint16_t p_majorOperatingSystemVersion;
    uint16_t p_minorOperatingSystemVersion;
    uint16_t p_majorImageVersion;
    uint16_t p_minorImageVersion;
    uint16_t p_majorSubsystemVersion;
    uint16_t p_minorSubsystemVersion;
    uint32_t p_win32VersionValue;  // must always be 0
    // must be multiple of p_sectionAlignment
    uint32_t p_sizeOfImage;
    // size of MS-DOS stub, PE header, optional header, and all section headers
    // must be a multiple of p_fileAlignment
    uint32_t p_sizeOfHeaders;
    uint32_t p_checkSum;
    uint16_t p_subSystem;
    uint16_t p_dllCharacteristics;
    uint32_t p_sizeOfStackReserve;
    uint32_t p_sizeOfStackCommit;
    uint32_t p_sizeOfHeapReserve;
    uint32_t p_sizeOfHeapCommit;
    uint32_t p_loaderFlags;  // must always be 0
    uint32_t p_numberAndSizeOfDataDirs;
    peOptHdrSpecFields32() {
        p_imageBase = VirtAddr32;
        p_sectionAlignment = SECTION_ALIGN;
        p_fileAlignment = FILE_ALIGN;
        p_majorOperatingSystemVersion = 4;
        p_minorOperatingSystemVersion = 0;
        p_majorImageVersion = 0;
        p_minorImageVersion = 0;
        p_majorSubsystemVersion = 5;
        p_minorSubsystemVersion = 2;
        p_win32VersionValue = 0;  // must always be 0
        p_sizeOfImage = 0;        // currently unknown v
        p_sizeOfHeaders = 0;      // currently unknown v
        p_checkSum = 0;           // currently unknown
        p_subSystem = IMAGE_SUBSYSTEM_WINDOWS_GUI;
        p_dllCharacteristics = 0x400;
        p_sizeOfStackReserve = 100000;   // currently unknown
        p_sizeOfStackCommit = 1000;    // currently unknown
        p_sizeOfHeapReserve = 100000;    // currently unknown
        p_sizeOfHeapCommit = 1000;     // currently unknown
        p_loaderFlags = 0;          // must always be 0
        p_numberAndSizeOfDataDirs = 13;
    }
    void push(std::ofstream &stream) {
        pushWord(stream,p_imageBase,PE_IS_LSB);
        pushWord(stream,p_sectionAlignment,PE_IS_LSB);
        pushWord(stream,p_fileAlignment,PE_IS_LSB);
        pushHalfWord(stream,p_majorOperatingSystemVersion,PE_IS_LSB);
        pushHalfWord(stream,p_minorOperatingSystemVersion,PE_IS_LSB);
        pushHalfWord(stream,p_majorImageVersion,PE_IS_LSB);
        pushHalfWord(stream,p_minorImageVersion,PE_IS_LSB);
        pushHalfWord(stream,p_majorSubsystemVersion,PE_IS_LSB);
        pushHalfWord(stream,p_minorSubsystemVersion,PE_IS_LSB);
        pushWord(stream,p_win32VersionValue,PE_IS_LSB);
        pushWord(stream,p_sizeOfImage,PE_IS_LSB);
        pushWord(stream,p_sizeOfHeaders,PE_IS_LSB);
        pushWord(stream,p_checkSum,PE_IS_LSB);
        pushHalfWord(stream,p_subSystem,PE_IS_LSB);
        pushHalfWord(stream,p_dllCharacteristics,PE_IS_LSB);
        pushWord(stream,p_sizeOfStackReserve,PE_IS_LSB);
        pushWord(stream,p_sizeOfStackCommit,PE_IS_LSB);
        pushWord(stream,p_sizeOfHeapReserve,PE_IS_LSB);
        pushWord(stream,p_sizeOfHeapCommit,PE_IS_LSB);
        pushWord(stream,p_loaderFlags,PE_IS_LSB);
        pushWord(stream,p_numberAndSizeOfDataDirs,PE_IS_LSB);
    }
};
struct peOptHdrSpecFields64 {
    uint64_t p_imageBase;
    uint32_t p_sectionAlignment;
    uint32_t p_fileAlignment;
    uint16_t p_majorOperatingSystemVersion;
    uint16_t p_minorOperatingSystemVersion;
    uint16_t p_majorImageVersion;
    uint16_t p_minorImageVersion;
    uint16_t p_majorSubsystemVersion;
    uint16_t p_minorSubsystemVersion;
    uint16_t p_win32VersionValue; // must always be 0
    // must be multiple of p_sectionAlignment
    uint32_t p_sizeOfImage;
    // size of MS-DOS stub, PE header, optional header, and all section headers
    // must be a multiple of p_fileAlignment
    uint32_t p_sizeOfHeaders;
    uint32_t p_checkSum;
    uint16_t p_subSystem;
    uint16_t p_dllCharacteristics;
    uint64_t p_sizeOfStackReserve;
    uint64_t p_sizeOfStackCommit;
    uint64_t p_sizeOfHeapReserve;
    uint64_t p_sizeOfHeapCommit;
    uint32_t p_loaderFlags; // must always be 0
    uint32_t p_numberAndSizeOfDataDirs;
    peOptHdrSpecFields64() {
        p_imageBase = VirtAddr32;
        p_sectionAlignment = SECTION_ALIGN;
        p_fileAlignment = FILE_ALIGN;
        p_majorOperatingSystemVersion = 4;
        p_minorOperatingSystemVersion = 0;
        p_majorImageVersion = 0;
        p_minorImageVersion = 0;
        p_majorSubsystemVersion = 5;
        p_minorSubsystemVersion = 2;
        p_win32VersionValue = 0;  // must always be 0
        p_sizeOfImage = 0;        // currently unknown v
        p_sizeOfHeaders = 0;      // currently unknown v
        p_checkSum = 0;           // currently unknown
        p_subSystem = IMAGE_SUBSYSTEM_WINDOWS_GUI;
        p_dllCharacteristics = 0x400;
        p_sizeOfStackReserve = 100000;   // currently unknown
        p_sizeOfStackCommit = 1000;    // currently unknown
        p_sizeOfHeapReserve = 100000;    // currently unknown
        p_sizeOfHeapCommit = 1000;     // currently unknown
        p_loaderFlags = 0;          // must always be 0
        p_numberAndSizeOfDataDirs = 13;
    }
    void push(std::ofstream &stream) {
        pushDword(stream,p_imageBase,PE_IS_LSB);
        pushWord(stream,p_sectionAlignment,PE_IS_LSB);
        pushWord(stream,p_fileAlignment,PE_IS_LSB);
        pushHalfWord(stream,p_majorOperatingSystemVersion,PE_IS_LSB);
        pushHalfWord(stream,p_minorOperatingSystemVersion,PE_IS_LSB);
        pushHalfWord(stream,p_majorImageVersion,PE_IS_LSB);
        pushHalfWord(stream,p_minorImageVersion,PE_IS_LSB);
        pushHalfWord(stream,p_majorSubsystemVersion,PE_IS_LSB);
        pushHalfWord(stream,p_minorSubsystemVersion,PE_IS_LSB);
        pushWord(stream,p_win32VersionValue,PE_IS_LSB);
        pushWord(stream,p_sizeOfImage,PE_IS_LSB);
        pushWord(stream,p_sizeOfHeaders,PE_IS_LSB);
        pushWord(stream,p_checkSum,PE_IS_LSB);
        pushHalfWord(stream,p_subSystem,PE_IS_LSB);
        pushHalfWord(stream,p_dllCharacteristics,PE_IS_LSB);
        pushDword(stream,p_sizeOfStackReserve,PE_IS_LSB);
        pushDword(stream,p_sizeOfStackCommit,PE_IS_LSB);
        pushDword(stream,p_sizeOfHeapReserve,PE_IS_LSB);
        pushDword(stream,p_sizeOfHeapCommit,PE_IS_LSB);
        pushWord(stream,p_loaderFlags,PE_IS_LSB);
        pushWord(stream,p_numberAndSizeOfDataDirs,PE_IS_LSB);
    }
};

struct peOptHdrDataDirs {
    uint32_t p_exportTableRVA;
    uint32_t p_exportTableSize;
    uint32_t p_importTableRVA;
    uint32_t p_importTableSize;
    uint32_t p_resourceTableRVA;
    uint32_t p_resourceTableSize;
    uint32_t p_exceptionTableRVA;
    uint32_t p_exceptionTableSize;
    uint32_t p_certificateTableRVA;
    uint32_t p_certificateTableSize;
    uint32_t p_baseRelocationTableRVA;
    uint32_t p_baseRelocationTableSize;
    uint32_t p_debugRVA;
    uint32_t p_debugSize;
    uint64_t p_architecture;// must always be 0
    uint32_t p_globalPtrRVA;// 0
    uint32_t p_globalPtrSize;// 0
    uint32_t p_TlsTableRVA;
    uint32_t p_TlsTableSize;
    uint32_t p_loadConfigTableRVA;
    uint32_t p_loadConfigTableSize;
    uint32_t p_boundImportRVA;
    uint32_t p_boundImportSize;
    uint32_t p_ImprtAddressTableRVA;
    uint32_t p_ImprtAddressTableSize;
    uint32_t p_delayImportDescriptorRVA;
    uint32_t p_delayImportDescriptorSize;
    uint32_t p_ClrRuntimeHeaderRVA;
    uint32_t p_ClrRuntimeHeaderSize;
    uint64_t p_zero;  // must always be 0, obviously
    peOptHdrDataDirs() {
        p_exportTableRVA = 0;          // currently unknown
        p_exportTableSize = 0;         // currently unknown
        p_importTableRVA = 0;          // currently unknown
        p_importTableSize = 0;         // currently unknown
        p_resourceTableRVA = 0;        // currently unknown
        p_resourceTableSize = 0;       // currently unknown
        p_exceptionTableRVA = 0;       // currently unknown
        p_exceptionTableSize = 0;      // currently unknown
        p_certificateTableRVA = 0;     // currently unknown
        p_certificateTableSize = 0;    // currently unknown
        p_baseRelocationTableRVA = 0;  // currently unknown
        p_baseRelocationTableSize = 0; // currently unknown
        p_debugRVA = 0;                // currently unknown
        p_debugSize = 0;               // currently unknown
        p_architecture = 0;            // must always be 0
        p_globalPtrRVA = 0;
        p_globalPtrSize = 0;
        p_TlsTableRVA = 0;               // currently unknown
        p_TlsTableSize = 0;              // currently unknown
        p_loadConfigTableRVA = 0;        // currently unknown
        p_loadConfigTableSize = 0;       // currently unknown
        p_boundImportRVA = 0;            // currently unknown
        p_boundImportSize = 0;           // currently unknown
        p_ImprtAddressTableRVA = 0;      // currently unknown
        p_ImprtAddressTableSize = 0;     // currently unknown
        p_delayImportDescriptorRVA = 0;  // currently unknown
        p_delayImportDescriptorSize = 0; // currently unknown
        p_ClrRuntimeHeaderRVA = 0;       // currently unknown
        p_ClrRuntimeHeaderSize = 0;      // currently unknown
        p_zero = 0;                      // must always be 0
    }
    void push(std::ofstream &stream) {
        pushWord(stream,p_exportTableRVA,PE_IS_LSB);
        pushWord(stream,p_exportTableSize,PE_IS_LSB);
        pushWord(stream,p_importTableRVA,PE_IS_LSB);
        pushWord(stream,p_importTableSize,PE_IS_LSB);
        pushWord(stream,p_resourceTableRVA,PE_IS_LSB);
        pushWord(stream,p_resourceTableSize,PE_IS_LSB);
        pushWord(stream,p_exceptionTableRVA,PE_IS_LSB);
        pushWord(stream,p_exceptionTableSize,PE_IS_LSB);
        pushWord(stream,p_certificateTableRVA,PE_IS_LSB);
        pushWord(stream,p_certificateTableSize,PE_IS_LSB);
        pushWord(stream,p_baseRelocationTableRVA,PE_IS_LSB);
        pushWord(stream,p_baseRelocationTableSize,PE_IS_LSB);
        pushWord(stream,p_debugRVA,PE_IS_LSB);
        pushWord(stream,p_debugSize,PE_IS_LSB);
        pushWord(stream,p_architecture,PE_IS_LSB);
        pushWord(stream,p_architecture,PE_IS_LSB);
        pushWord(stream,p_globalPtrRVA,PE_IS_LSB);
        pushWord(stream,p_globalPtrSize,PE_IS_LSB);
        pushWord(stream,p_TlsTableRVA,PE_IS_LSB);
        pushWord(stream,p_TlsTableSize,PE_IS_LSB);
        pushWord(stream,p_loadConfigTableRVA,PE_IS_LSB);
        pushWord(stream,p_loadConfigTableSize,PE_IS_LSB);
        pushWord(stream,p_boundImportRVA,PE_IS_LSB);
        pushWord(stream,p_boundImportSize,PE_IS_LSB);
        pushWord(stream,p_ImprtAddressTableRVA,PE_IS_LSB);
        pushWord(stream,p_ImprtAddressTableSize,PE_IS_LSB);
        pushWord(stream,p_delayImportDescriptorRVA,PE_IS_LSB);
        pushWord(stream,p_delayImportDescriptorSize,PE_IS_LSB);
        pushWord(stream,p_ClrRuntimeHeaderRVA,PE_IS_LSB);
        pushWord(stream,p_ClrRuntimeHeaderSize,PE_IS_LSB);
        pushWord(stream,p_zero,PE_IS_LSB);
        pushWord(stream,p_zero,PE_IS_LSB);
    }
};

// section tables

struct peSectionHdr {
    char s_name[8];
    uint32_t s_virtualSize;     // size in memory, invalid for object files
    uint32_t s_virtualAddress;  // virtual address in memory, invalid for object files
    // must be multiple of file alignment
    uint32_t s_rawDataSize;  // size in file
    // must be multiple of file alignment
    // if section contains only uninitialized data, this should be set to 0
    uint32_t s_rawDataPointer;
    uint32_t s_relocationsPointer;     // invalid for image file
    uint32_t s_lineNumbersPointer;     // invalid for image file
    uint16_t s_numberOfRelocations;  // invalid for image file
    uint16_t s_numberOfLineNumbers;  // invalid for image file
    uint32_t s_characteristics;
    peSectionHdr(const char name[8], uint32_t characteristics) {
        for (uint32_t i = 0; i < 8; i++) s_name[i]=name[i];
        s_virtualSize = 0;     // currently unknown
        s_virtualAddress = 0;  // currently unknown
        s_rawDataSize = 0;     // currently unknown
        s_rawDataPointer = 0;  // currently unknown
        s_relocationsPointer=0;
        s_lineNumbersPointer=0;
        s_numberOfRelocations=0;
        s_numberOfLineNumbers=0;
        s_characteristics=characteristics;
        //if (Align32==1) s_characteristics|=IMAGE_SCN_ALIGN_1BYTES;
        //else if (Align32==512) s_characteristics|=IMAGE_SCN_ALIGN_512BYTES;
    }
    void push(std::ofstream &stream) {
        for (uint32_t i = 0; i < 8; i++) { stream << s_name[i]; }
        pushWord(stream,s_virtualSize,PE_IS_LSB);
        pushWord(stream,s_virtualAddress,PE_IS_LSB);
        pushWord(stream,s_rawDataSize,PE_IS_LSB);
        pushWord(stream,s_rawDataPointer,PE_IS_LSB);
        pushWord(stream,s_relocationsPointer,PE_IS_LSB);
        pushWord(stream,s_lineNumbersPointer,PE_IS_LSB);
        pushHalfWord(stream,s_numberOfRelocations,PE_IS_LSB);
        pushHalfWord(stream,s_numberOfLineNumbers,PE_IS_LSB);
        pushWord(stream,s_characteristics,PE_IS_LSB);
    }
};
#pragma endregion// structs

#pragma region handlers
class Pe32SectionHandler;
class Pe32Handler {
private:
    std::vector<Pe32SectionHandler *> sectionHeaders32;
    peHdr peHeader;
    peOptHdrStdFields32 peStdFieldsHeader;
    peOptHdrSpecFields32 peSpecFieldsHeader;
    peOptHdrDataDirs peDataDirHeader;

public:
    Pe32Handler();
    void push(std::ofstream &stream);
    Pe32SectionHandler *addSeg(const char name[8], uint32_t characteristics, const char *type);

    friend Pe32SectionHandler;
};

class Pe32SectionHandler {
    Pe32Handler &peHandler;
    peSectionHdr sectionHeader;
    std::vector<uint8_t> data;

    uint32_t sectionAlignment=1;
    uint32_t fileAlignment=1;
public:
     const char *type;
    Pe32SectionHandler(Pe32Handler &_peHandler, const char name[8], uint32_t characteristics, const char *_type);
    bool isCode() {
        return ((sectionHeader.s_characteristics&IMAGE_SCN_MEM_EXECUTE)!=0);
    }
    void pushHeader(std::ofstream &stream);
    void pushData(std::ofstream &stream);

    uint32_t getSize();
    void setSectionAlign(const uint32_t& align);
    void setFileAlign(const uint32_t& align);
    void setOffset(const uint32_t &offset);
    void setRVA(const uint32_t &Rva);

    friend void pushChars(Pe32SectionHandler *section, const uint8_t *chars, uint32_t len, const bool &LSB);
    friend void pushByte(Pe32SectionHandler *section, const uint8_t &byte);
    friend void pushHalfWord(Pe32SectionHandler *section, const uint16_t &halfword, const bool &LSB);
    friend void pushWord(Pe32SectionHandler *section, const uint32_t &word, const bool &LSB);
    friend void pushDword(Pe32SectionHandler *section, const uint64_t &dword, const bool &LSB);
};
    void pushChars(Pe32SectionHandler *section, const uint8_t *chars, uint32_t len, const bool &LSB);
    void pushByte(Pe32SectionHandler *section, const uint8_t &byte);
    void pushHalfWord(Pe32SectionHandler *section, const uint16_t &halfword, const bool &LSB);
    void pushWord(Pe32SectionHandler *section, const uint32_t &word, const bool &LSB);
    void pushDword(Pe32SectionHandler *section, const uint64_t &dword, const bool &LSB);


class Pe64SectionHandler;
class Pe64Handler {
private:
    std::vector<Pe64SectionHandler *> sectionHeaders64;
    peHdr peHeader;
    peOptHdrStdFields64 peStdFieldsHeader;
    peOptHdrSpecFields64 peSpecFieldsHeader;
    peOptHdrDataDirs peDataDirHeader;

public:
    Pe64Handler();
    void push(std::ofstream &stream);
    Pe64SectionHandler *addSeg(const char name[8], uint32_t characteristics, const char *type);

    friend Pe64SectionHandler;
};

class Pe64SectionHandler {
    Pe64Handler &peHandler;
    peSectionHdr sectionHeader;
    std::vector<uint8_t> data;

    uint32_t sectionAlignment=1;
    uint32_t fileAlignment=1;
public:
     const char *type;
    Pe64SectionHandler(Pe64Handler &_peHandler, const char name[8], uint32_t characteristics, const char *_type);
    bool isCode() {
        return ((sectionHeader.s_characteristics&IMAGE_SCN_MEM_EXECUTE)!=0);
    }
    void pushHeader(std::ofstream &stream);
    void pushData(std::ofstream &stream);

    uint32_t getSize();
    void setSectionAlign(const uint32_t& align);
    void setFileAlign(const uint32_t& align);
    void setOffset(const uint32_t &offset);
    void setRVA(const uint32_t &Rva);

    friend void pushChars(Pe64SectionHandler *section, const uint8_t *chars, uint32_t len, const bool &LSB);
    friend void pushByte(Pe64SectionHandler *section, const uint8_t &byte);
    friend void pushHalfWord(Pe64SectionHandler *section, const uint16_t &halfword, const bool &LSB);
    friend void pushWord(Pe64SectionHandler *section, const uint32_t &word, const bool &LSB);
    friend void pushDword(Pe64SectionHandler *section, const uint64_t &dword, const bool &LSB);
};
    void pushChars(Pe64SectionHandler *section, const uint8_t *chars, uint32_t len, const bool &LSB);
    void pushByte(Pe64SectionHandler *section, const uint8_t &byte);
    void pushHalfWord(Pe64SectionHandler *section, const uint16_t &halfword, const bool &LSB);
    void pushWord(Pe64SectionHandler *section, const uint32_t &word, const bool &LSB);
    void pushDword(Pe64SectionHandler *section, const uint64_t &dword, const bool &LSB);
#pragma endregion// handlers

#endif  // _PE_H
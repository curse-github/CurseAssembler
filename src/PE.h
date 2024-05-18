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
struct peHdr32 {  // 22 bytes
    unsigned char p_magic[4];
    unsigned short p_machine;
    unsigned short p_numberOfSections;    // limited at 96 sections
    unsigned int p_timeDateStamp;         // seconds since 00:00 January 1, 1970
    unsigned int p_pointerToSymbolTable;  // file offset of COFF symbol table
    unsigned int p_numberOfSymbols;
    unsigned short p_sizeOfOptionalHeader;  // not optional for executables
    unsigned short p_characteristics;
    peHdr32() {
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
    unsigned short p_magic;
    unsigned char p_majorLinkerVersion;
    unsigned char p_minorLinkerVersion;
    unsigned int p_sizeOfCode;
    unsigned int p_sizeOfInitializedData;
    unsigned int p_sizeOfUninitializedData;
    unsigned int p_addressOfEntryPoint;
    unsigned int p_baseOfCode;
    unsigned int p_baseOfData;  // virtual address??
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
/*struct peOptHdrStdFields64 {// 28 bytes
    unsigned short p_magic;
    unsigned char p_majorLinkerVersion;
    unsigned char p_minorLinkerVersion;
    unsigned int p_sizeOfCode;
    unsigned int p_sizeOfInitializedData;
    unsigned int p_sizeOfUninitializedData;
    unsigned int p_addressOfEntryPoint;
    unsigned int p_baseOfCode;
};*/

struct peOptHdrSpecFields32 {
    unsigned int p_imageBase;
    unsigned int p_sectionAlignment;
    unsigned int p_fileAlignment;
    unsigned short p_majorOperatingSystemVersion;
    unsigned short p_minorOperatingSystemVersion;
    unsigned short p_majorImageVersion;
    unsigned short p_minorImageVersion;
    unsigned short p_majorSubsystemVersion;
    unsigned short p_minorSubsystemVersion;
    unsigned int p_win32VersionValue;  // must always be 0
    // must be multiple of p_sectionAlignment
    unsigned int p_sizeOfImage;
    // size of MS-DOS stub, PE header, optional header, and all section headers
    // must be a multiple of p_fileAlignment
    unsigned int p_sizeOfHeaders;
    unsigned int p_checkSum;
    unsigned short p_subSystem;
    unsigned short p_dllCharacteristics;
    unsigned int p_sizeOfStackReserve;
    unsigned int p_sizeOfStackCommit;
    unsigned int p_sizeOfHeapReserve;
    unsigned int p_sizeOfHeapCommit;
    unsigned int p_loaderFlags;  // must always be 0
    unsigned int p_numberAndSizeOfDataDirs;
    peOptHdrSpecFields32() {
        p_imageBase = VirtAddr32;
        p_sectionAlignment = 0x1000;
        p_fileAlignment = 0x200;
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
        p_sizeOfStackReserve = 0;   // currently unknown
        p_sizeOfStackCommit = 0;    // currently unknown
        p_sizeOfHeapReserve = 0;    // currently unknown
        p_sizeOfHeapCommit = 0;     // currently unknown
        p_loaderFlags = 0;          // must always be 0
        p_numberAndSizeOfDataDirs = 16;// most likely one, for an executable
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
/*struct peOptHdrSpecFields64 {
    unsigned long p_imageBase;
    unsigned int p_sectionAlignment;
    unsigned int p_fileAlignment;
    unsigned short p_majorOperatingSystemVersion;
    unsigned short p_minorOperatingSystemVersion;
    unsigned short p_majorImageVersion;
    unsigned short p_minorImageVersion;
    unsigned short p_majorSubsystemVersion;
    unsigned short p_minorSubsystemVersion;
    unsigned short p_win32VersionValue;
    unsigned int p_sizeOfImage;
    unsigned int p_sizeOfHeaders;
    unsigned int p_checkSum;
    unsigned short p_subSystem;
    unsigned short p_dllCharacteristics;
    unsigned long p_sizeOfStackReserve;
    unsigned long p_sizeOfStackCommit;
    unsigned long p_sizeOfHeapReserve;
    unsigned long p_sizeOfHeapCommit;
    unsigned int p_loaderFlags;
    unsigned int p_numberOfRvaAndSizes;
};*/

struct peOptHdrDataDirs32 {
    unsigned int p_exportTableRVA;
    unsigned int p_exportTableSize;
    unsigned int p_importTableRVA;
    unsigned int p_importTableSize;
    unsigned int p_resourceTableRVA;
    unsigned int p_resourceTableSize;
    unsigned int p_exceptionTableRVA;
    unsigned int p_exceptionTableSize;
    unsigned int p_certificateTableRVA;
    unsigned int p_certificateTableSize;
    unsigned int p_baseRelocationTableRVA;
    unsigned int p_baseRelocationTableSize;
    unsigned int p_debugRVA;
    unsigned int p_debugSize;
    unsigned long p_architecture;// must always be 0
    unsigned int p_globalPtrRVA;// 0
    unsigned int p_globalPtrSize;// 0
    unsigned int p_TlsTableRVA;
    unsigned int p_TlsTableSize;
    unsigned int p_loadConfigTableRVA;
    unsigned int p_loadConfigTableSize;
    unsigned int p_boundImportRVA;
    unsigned int p_boundImportSize;
    unsigned int p_ImprtAddressTableRVA;
    unsigned int p_ImprtAddressTableSize;
    unsigned int p_delayImportDescriptorRVA;
    unsigned int p_delayImportDescriptorSize;
    unsigned int p_ClrRuntimeHeaderRVA;
    unsigned int p_ClrRuntimeHeaderSize;
    unsigned long p_zero;  // must always be 0, obviously
    peOptHdrDataDirs32() {
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
        pushDword(stream,p_architecture,PE_IS_LSB);
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
        pushDword(stream,p_zero,PE_IS_LSB);
    }
};

// section tables

struct peSectionHdr {
    char s_name[8];
    unsigned int s_virtualSize;     // size in memory, invalid for object files
    unsigned int s_virtualAddress;  // virtual address in memory, invalid for object files
    // must be multiple of file alignment
    unsigned int s_rawDataSize;  // size in file
    // must be multiple of file alignment
    // if section contains only uninitialized data, this should be set to 0
    unsigned int s_rawDataPointer;
    unsigned int s_relocationsPointer;     // invalid for image file
    unsigned int s_lineNumbersPointer;     // invalid for image file
    unsigned short s_numberOfRelocations;  // invalid for image file
    unsigned short s_numberOfLineNumbers;  // invalid for image file
    unsigned int s_characteristics;
    peSectionHdr(const char name[8], unsigned int characteristics) {
        for (unsigned int i = 0; i < 8; i++) s_name[i]=name[i];
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
        for (unsigned int i = 0; i < 8; i++) { stream << s_name[i]; }
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
class PeSectionHandler;
class PeHandler {
private:
    std::vector<PeSectionHandler *> sectionHeaders32;
    peHdr32 peHeader;
    peOptHdrStdFields32 peStdFieldsHeader;
    peOptHdrSpecFields32 peSpecFieldsHeader;
    peOptHdrDataDirs32 peDataDirHeader;

public:
    PeHandler();
    void push(std::ofstream &stream);
    PeSectionHandler *addSeg(const char name[8], unsigned int characteristics, const char *type);

    friend PeSectionHandler;
};

class PeSectionHandler {
    PeHandler &peHandler;
    peSectionHdr sectionHeader;
    std::vector<unsigned char> data;

    unsigned int sectionAlignment=1;
    unsigned int fileAlignment=1;
public:
     const char *type;
    PeSectionHandler(PeHandler &_peHandler, const char name[8], unsigned int characteristics, const char *_type);
    bool isCode() {
        return ((sectionHeader.s_characteristics&IMAGE_SCN_MEM_EXECUTE)!=0);
    }
    void pushHeader(std::ofstream &stream);
    void pushData(std::ofstream &stream);

    unsigned int getSize();
    void setSectionAlign(const unsigned int& align);
    void setFileAlign(const unsigned int& align);
    void setOffset(const unsigned int &offset);
    void setRVA(const unsigned int &Rva);

    friend void pushChars(PeSectionHandler *section, const unsigned char *chars, unsigned int len, const bool &LSB);
    friend void pushByte(PeSectionHandler *section, const unsigned char &byte);
    friend void pushHalfWord(PeSectionHandler *section, const unsigned short &halfword, const bool &LSB);
    friend void pushWord(PeSectionHandler *section, const unsigned int &word, const bool &LSB);
    friend void pushDword(PeSectionHandler *section, const unsigned long &dword, const bool &LSB);
};
    void pushChars(PeSectionHandler *section, const unsigned char *chars, unsigned int len, const bool &LSB);
    void pushByte(PeSectionHandler *section, const unsigned char &byte);
    void pushHalfWord(PeSectionHandler *section, const unsigned short &halfword, const bool &LSB);
    void pushWord(PeSectionHandler *section, const unsigned int &word, const bool &LSB);
    void pushDword(PeSectionHandler *section, const unsigned long &dword, const bool &LSB);
#pragma endregion// handlers

#endif  // _PE_H
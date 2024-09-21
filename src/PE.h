#ifndef _PE_H
#define _PE_H

// https://upload.wikimedia.org/wikipedia/commons/1/1b/Portable_Executable_32_bit_Structure_in_SVG_fixed.svg
// https://learn.microsoft.com/en-us/windows/win32/debug/pe-format
// https://learn.microsoft.com/en-us/windows/win32/debug/pe-format#special-sections

#include "peConstants.h"
#include "globalConstants.h"

#pragma region structs

struct peSectionHdr;
struct SectionPlusOffset {
    peSectionHdr* section=nullptr;
    int32_t index;
    uint32_t offset=0;
};
SectionPlusOffset RVAtoSectionPlusOffset(const uint32_t& RVA, const uint32_t& size, std::vector<peSectionHdr>& sections);

#include <time.h>
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
        p_sizeOfOptionalHeader = 0;
        p_characteristics = IMAGE_FILE_CHARACTERISTIC_EXECUTABLE_IMAGE | IMAGE_FILE_CHARACTERISTIC_LINE_NUMS_STRIPPED | IMAGE_FILE_CHARACTERISTIC_LOCAL_SYMS_STRIPPED | IMAGE_FILE_CHARACTERISTIC_DEBUG_STRIPPED;
    }
    void push(std::ofstream& stream) {
        pushChars(stream,p_magic,4,true);
        pushWord(stream,p_machine,true);
        pushWord(stream,p_numberOfSections,true);
        pushDword(stream,p_timeDateStamp,true);
        pushDword(stream,p_pointerToSymbolTable,true);
        pushDword(stream,p_numberOfSymbols,true);
        pushWord(stream,p_sizeOfOptionalHeader,true);
        pushWord(stream,p_characteristics,true);
    }
    struct peHdrPullReturnData {
        bool isValid=true;
        uint32_t PeHeaderOffset=0;
    };
    peHdrPullReturnData read(std::ifstream& stream, uint32_t& count) {
        peHdrPullReturnData returnData;
        try {
            // skip past MZ header
            if ((readChar(stream,count)!='M')||(readChar(stream,count)!='Z')) { std::cout << "File not PE file" << std::endl; stream.close(); returnData.isValid=false; return returnData; }
            for (size_t i = 0; i < 0x3a; i++) readByte(stream,count);
            returnData.PeHeaderOffset = readDword(stream, count);
            if (returnData.PeHeaderOffset==0) { std::cout << "PE header offset not found." << std::endl; stream.close(); returnData.isValid=false; return returnData; }
            size_t numBytesToTrash = returnData.PeHeaderOffset-count;
            for (size_t i = 0; i < numBytesToTrash; i++) readByte(stream,count);

            // get PE header
            if ((readChar(stream,count)!='P')||(readChar(stream,count)!='E')||(readChar(stream,count)!='\0')||(readChar(stream,count)!='\0')) { std::cout << "File not PE file" << std::endl; stream.close(); returnData.isValid=false; return returnData; }
            p_magic[0] = 'P';
            p_magic[1] = 'E';
            p_magic[2] = '\0';
            p_magic[3] = '\0';
            p_machine = readWord(stream, count);
            if (p_machine!=IMAGE_FILE_MACHINE_AMD64) { std::cout << "PE header p_machine unknown." << std::endl; stream.close(); returnData.isValid=false; return returnData; }
            p_numberOfSections = readWord(stream, count);
            if (p_numberOfSections>96) { std::cout << "Invalid number of sections." << std::endl; stream.close(); returnData.isValid=false; return returnData; }
            p_timeDateStamp = readDword(stream, count);
            p_pointerToSymbolTable = readDword(stream, count);
            p_numberOfSymbols = readDword(stream, count);
            p_sizeOfOptionalHeader = readWord(stream, count);
            p_characteristics = readWord(stream, count);
            return returnData;
        } catch (int code) { returnData.isValid=false; return returnData; }
    }
    void print(std::string delimmiter="\n") {
        std::cout << "p_magic: \"P,E,\\0,\\0\"" << delimmiter;
        std::cout << "p_machine: " << intToHex(p_machine) << delimmiter;
        std::cout << "p_numberOfSections: " << p_numberOfSections << delimmiter;
        std::cout << "p_timeDateStamp: " << p_timeDateStamp << delimmiter;
        std::cout << "p_pointerToSymbolTable: " << p_pointerToSymbolTable << delimmiter;
        std::cout << "p_numberOfSymbols: " << p_numberOfSymbols << delimmiter;
        std::cout << "p_sizeOfOptionalHeader: " << intToHex(p_sizeOfOptionalHeader) << delimmiter;
        std::cout << "p_characteristics: " << intToHex(p_characteristics) << " -> ";
        const bool isExecutable = (p_characteristics&IMAGE_FILE_CHARACTERISTIC_EXECUTABLE_IMAGE)>0;
        const bool isDll = (p_characteristics&IMAGE_FILE_CHARACTERISTIC_DLL)>0;
        const bool relocStripped = (p_characteristics&IMAGE_FILE_CHARACTERISTIC_RELOCS_STRIPPED)>0;
        const bool lineNumStripped = (p_characteristics&IMAGE_FILE_CHARACTERISTIC_LINE_NUMS_STRIPPED)>0;
        const bool is32Bit = (p_characteristics&IMAGE_FILE_CHARACTERISTIC_32BIT_MACHINE)>0;
        const bool debugStripped = (p_characteristics&IMAGE_FILE_CHARACTERISTIC_DEBUG_STRIPPED)>0;
        if (isExecutable) std::cout << "Executable";
        if (isExecutable&&(isDll||relocStripped||lineNumStripped||is32Bit||debugStripped)) std::cout << ", ";
        if (isDll) std::cout << "DLL";
        if (isDll&&(relocStripped||lineNumStripped||is32Bit||debugStripped)) std::cout << ", ";
        if (relocStripped) std::cout << "no relocations";
        if (relocStripped&&(lineNumStripped||is32Bit||debugStripped)) std::cout << ", ";
        if (lineNumStripped) std::cout << "no line nums";
        if (lineNumStripped&&(is32Bit||debugStripped)) std::cout << ", ";
        if (is32Bit) std::cout << "32bit";
        if (is32Bit&&debugStripped) std::cout << ", ";
        if (debugStripped) std::cout << "no debug data";
        std::cout << '\n';
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
    void push(std::ofstream& stream) {
        pushWord(stream,p_magic,true);
        pushByte(stream,p_majorLinkerVersion);
        pushByte(stream,p_minorLinkerVersion);
        pushDword(stream,p_sizeOfCode,true);
        pushDword(stream,p_sizeOfInitializedData,true);
        pushDword(stream,p_sizeOfUninitializedData,true);
        pushDword(stream,p_addressOfEntryPoint,true);
        pushDword(stream,p_baseOfCode,true);
        pushDword(stream,p_baseOfData,true);
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
    void push(std::ofstream& stream) {
        pushWord(stream,p_magic,true);
        pushByte(stream,p_majorLinkerVersion);
        pushByte(stream,p_minorLinkerVersion);
        pushDword(stream,p_sizeOfCode,true);
        pushDword(stream,p_sizeOfInitializedData,true);
        pushDword(stream,p_sizeOfUninitializedData,true);
        pushDword(stream,p_addressOfEntryPoint,true);
        pushDword(stream,p_baseOfCode,true);
    }
    bool read(std::ifstream& stream, uint32_t& count) {
        try {
            uint16_t p_std_magic = readWord(stream, count);
            if (p_std_magic!=IMAGE_FILE_MAGIC64)  { std::cout << "Only 64 bit can be parsed currently" << std::endl; stream.close(); return false; }
            p_minorLinkerVersion = readByte(stream, count);
            p_minorLinkerVersion = readByte(stream, count);
            p_sizeOfCode = readDword(stream, count);
            p_sizeOfInitializedData = readDword(stream, count);
            p_sizeOfUninitializedData = readDword(stream, count);
            p_addressOfEntryPoint = readDword(stream, count);
            p_baseOfCode = readDword(stream, count);
            return true;
        } catch (int code) { return false; }
    }
    void print(std::vector<peSectionHdr>& sections, std::string delimmiter="\n");
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
        p_subSystem = IMAGE_SUBSYSTEM_WINDOWS_CUI;
        p_dllCharacteristics = 0x400;
        p_sizeOfStackReserve = 0x100000;   // currently unknown
        p_sizeOfStackCommit = 0x1000;    // currently unknown
        p_sizeOfHeapReserve = 0x100000;    // currently unknown
        p_sizeOfHeapCommit = 0x1000;     // currently unknown
        p_loaderFlags = 0;          // must always be 0
        p_numberAndSizeOfDataDirs = 13;
    }
    void push(std::ofstream& stream) {
        pushDword(stream,p_imageBase,true);
        pushDword(stream,p_sectionAlignment,true);
        pushDword(stream,p_fileAlignment,true);
        pushWord(stream,p_majorOperatingSystemVersion,true);
        pushWord(stream,p_minorOperatingSystemVersion,true);
        pushWord(stream,p_majorImageVersion,true);
        pushWord(stream,p_minorImageVersion,true);
        pushWord(stream,p_majorSubsystemVersion,true);
        pushWord(stream,p_minorSubsystemVersion,true);
        pushDword(stream,p_win32VersionValue,true);
        pushDword(stream,p_sizeOfImage,true);
        pushDword(stream,p_sizeOfHeaders,true);
        pushDword(stream,p_checkSum,true);
        pushWord(stream,p_subSystem,true);
        pushWord(stream,p_dllCharacteristics,true);
        pushDword(stream,p_sizeOfStackReserve,true);
        pushDword(stream,p_sizeOfStackCommit,true);
        pushDword(stream,p_sizeOfHeapReserve,true);
        pushDword(stream,p_sizeOfHeapCommit,true);
        pushDword(stream,p_loaderFlags,true);
        pushDword(stream,p_numberAndSizeOfDataDirs,true);
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
    uint32_t p_win32VersionValue; // must always be 0
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
        p_imageBase = VirtAddr64;
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
        p_subSystem = IMAGE_SUBSYSTEM_WINDOWS_CUI;
        p_dllCharacteristics = 0x400;
        p_sizeOfStackReserve = 0x100000;   // currently unknown
        p_sizeOfStackCommit = 0x1000;    // currently unknown
        p_sizeOfHeapReserve = 0x100000;    // currently unknown
        p_sizeOfHeapCommit = 0x1000;     // currently unknown
        p_loaderFlags = 0;          // must always be 0
        p_numberAndSizeOfDataDirs = 0x10;
    }
    void push(std::ofstream& stream) {
        pushQword(stream,p_imageBase,true);
        pushDword(stream,p_sectionAlignment,true);
        pushDword(stream,p_fileAlignment,true);
        pushWord(stream,p_majorOperatingSystemVersion,true);
        pushWord(stream,p_minorOperatingSystemVersion,true);
        pushWord(stream,p_majorImageVersion,true);
        pushWord(stream,p_minorImageVersion,true);
        pushWord(stream,p_majorSubsystemVersion,true);
        pushWord(stream,p_minorSubsystemVersion,true);
        pushDword(stream,p_win32VersionValue,true);
        pushDword(stream,p_sizeOfImage,true);
        pushDword(stream,p_sizeOfHeaders,true);
        pushDword(stream,p_checkSum,true);
        pushWord(stream,p_subSystem,true);
        pushWord(stream,p_dllCharacteristics,true);
        pushQword(stream,p_sizeOfStackReserve,true);
        pushQword(stream,p_sizeOfStackCommit,true);
        pushQword(stream,p_sizeOfHeapReserve,true);
        pushQword(stream,p_sizeOfHeapCommit,true);
        pushDword(stream,p_loaderFlags,true);
        pushDword(stream,p_numberAndSizeOfDataDirs,true);
    }
    bool read(std::ifstream& stream, uint32_t& count) {
        try {
            p_imageBase = readQword(stream, count);
            p_sectionAlignment = readDword(stream, count);
            p_fileAlignment = readDword(stream, count);
            p_majorOperatingSystemVersion = readWord(stream, count);
            p_minorOperatingSystemVersion = readWord(stream, count);
            p_majorImageVersion = readWord(stream, count);
            p_minorImageVersion = readWord(stream, count);
            p_majorSubsystemVersion = readWord(stream, count);
            p_minorSubsystemVersion = readWord(stream, count);
            p_win32VersionValue = readDword(stream, count);
            p_sizeOfImage = readDword(stream, count);
            p_sizeOfHeaders = readDword(stream, count);
            p_checkSum = readDword(stream, count);
            p_subSystem = readWord(stream, count);
            p_dllCharacteristics = readWord(stream, count);
            p_sizeOfStackReserve = readQword(stream, count);
            p_sizeOfStackCommit = readQword(stream, count);
            p_sizeOfHeapReserve = readQword(stream, count);
            p_sizeOfHeapCommit = readQword(stream, count);
            p_loaderFlags = readDword(stream, count);
            p_numberAndSizeOfDataDirs = readDword(stream, count);
            return true;
        } catch (int code) { return false; }
    }
    void print(std::string delimmiter="\n") {
        std::cout << "p_imageBase: " << intToHex(p_imageBase) << delimmiter;
        std::cout << "p_sectionAlignment: " << intToHex(p_sectionAlignment) << delimmiter;
        std::cout << "p_fileAlignment: " << intToHex(p_fileAlignment) << delimmiter;
        std::cout << "p_majorOperatingSystemVersion: " << p_majorOperatingSystemVersion << delimmiter;
        std::cout << "p_minorOperatingSystemVersion: " << p_minorOperatingSystemVersion << delimmiter;
        std::cout << "p_majorImageVersion: " << p_majorImageVersion << delimmiter;
        std::cout << "p_minorImageVersion: " << p_minorImageVersion << delimmiter;
        std::cout << "p_majorSubsystemVersion: " << p_majorSubsystemVersion << delimmiter;
        std::cout << "p_minorSubsystemVersion: " << p_minorSubsystemVersion << delimmiter;
        std::cout << "p_win32VersionValue: " << p_win32VersionValue << delimmiter;
        std::cout << "p_sizeOfImage: " << intToHex(p_sizeOfImage) << delimmiter;
        std::cout << "p_sizeOfHeaders: " << intToHex(p_sizeOfHeaders) << delimmiter;
        std::cout << "p_checkSum: " << intToHex(p_checkSum) << delimmiter;
        std::cout << "p_subSystem: " << intToHex(p_subSystem) << delimmiter;
        std::cout << "p_dllCharacteristics: " << intToHex(p_dllCharacteristics) << delimmiter;
        std::cout << "p_sizeOfStackReserve: " << intToHex(p_sizeOfStackReserve) << delimmiter;
        std::cout << "p_sizeOfStackCommit: " << intToHex(p_sizeOfStackCommit) << delimmiter;
        std::cout << "p_sizeOfHeapReserve: " << intToHex(p_sizeOfHeapReserve) << delimmiter;
        std::cout << "p_sizeOfHeapCommit: " << intToHex(p_sizeOfHeapCommit) << delimmiter;
        std::cout << "p_loaderFlags: " << intToHex(p_loaderFlags) << delimmiter;
        std::cout << "p_numberAndSizeOfDataDirs: " << intToHex(p_numberAndSizeOfDataDirs) << '\n';
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
        p_ImprtAddressTableRVA = 0x3038; // currently unknown
        p_ImprtAddressTableSize = 10;    // currently unknown
        p_delayImportDescriptorRVA = 0;  // currently unknown
        p_delayImportDescriptorSize = 0; // currently unknown
        p_ClrRuntimeHeaderRVA = 0;       // currently unknown
        p_ClrRuntimeHeaderSize = 0;      // currently unknown
        p_zero = 0;                      // must always be 0
    }
    void push(std::ofstream& stream) {
        pushDword(stream,p_exportTableRVA,true);
        pushDword(stream,p_exportTableSize,true);
        pushDword(stream,p_importTableRVA,true);
        pushDword(stream,p_importTableSize,true);
        pushDword(stream,p_resourceTableRVA,true);
        pushDword(stream,p_resourceTableSize,true);
        pushDword(stream,p_exceptionTableRVA,true);
        pushDword(stream,p_exceptionTableSize,true);
        pushDword(stream,p_certificateTableRVA,true);
        pushDword(stream,p_certificateTableSize,true);
        pushDword(stream,p_baseRelocationTableRVA,true);
        pushDword(stream,p_baseRelocationTableSize,true);
        pushDword(stream,p_debugRVA,true);
        pushDword(stream,p_debugSize,true);
        pushDword(stream,p_architecture,true);
        pushDword(stream,p_architecture,true);
        pushDword(stream,p_globalPtrRVA,true);
        pushDword(stream,p_globalPtrSize,true);
        pushDword(stream,p_TlsTableRVA,true);
        pushDword(stream,p_TlsTableSize,true);
        pushDword(stream,p_loadConfigTableRVA,true);
        pushDword(stream,p_loadConfigTableSize,true);
        pushDword(stream,p_boundImportRVA,true);
        pushDword(stream,p_boundImportSize,true);
        pushDword(stream,p_ImprtAddressTableRVA,true);
        pushDword(stream,p_ImprtAddressTableSize,true);
        pushDword(stream,p_delayImportDescriptorRVA,true);
        pushDword(stream,p_delayImportDescriptorSize,true);
        pushDword(stream,p_ClrRuntimeHeaderRVA,true);
        pushDword(stream,p_ClrRuntimeHeaderSize,true);
        pushDword(stream,p_zero,true);
        pushDword(stream,p_zero,true);
    }
    bool read(std::ifstream& stream, uint32_t& count) {
        try {
            p_exportTableRVA = readDword(stream, count);
            p_exportTableSize = readDword(stream, count);
            p_importTableRVA = readDword(stream, count);
            p_importTableSize = readDword(stream, count);
            p_resourceTableRVA = readDword(stream, count);
            p_resourceTableSize = readDword(stream, count);
            p_exceptionTableRVA = readDword(stream, count);
            p_exceptionTableSize = readDword(stream, count);
            p_certificateTableRVA = readDword(stream, count);
            p_certificateTableSize = readDword(stream, count);
            p_baseRelocationTableRVA = readDword(stream, count);
            p_baseRelocationTableSize = readDword(stream, count);
            p_debugRVA = readDword(stream, count);
            p_debugSize = readDword(stream, count);
            p_architecture = readQword(stream, count);
            p_globalPtrRVA = readDword(stream, count);
            p_globalPtrSize = readDword(stream, count);
            p_TlsTableRVA = readDword(stream, count);
            p_TlsTableSize = readDword(stream, count);
            p_loadConfigTableRVA = readDword(stream, count);
            p_loadConfigTableSize = readDword(stream, count);
            p_boundImportRVA = readDword(stream, count);
            p_boundImportSize = readDword(stream, count);
            p_ImprtAddressTableRVA = readDword(stream, count);
            p_ImprtAddressTableSize = readDword(stream, count);
            p_delayImportDescriptorRVA = readDword(stream, count);
            p_delayImportDescriptorSize = readDword(stream, count);
            p_ClrRuntimeHeaderRVA = readDword(stream, count);
            p_ClrRuntimeHeaderSize = readDword(stream, count);
            p_zero = readQword(stream, count);
            return true;
        } catch (int code) { return false; }
    }
    void print(std::vector<peSectionHdr>& sections, std::string delimmiter="\n");
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
    peSectionHdr(const char name[9]) {
        for (uint32_t i = 0; i < 8; i++) s_name[i]=name[i];
        s_virtualSize = 0;     // currently unknown
        s_virtualAddress = 0;  // currently unknown
        s_rawDataSize = 0;     // currently unknown
        s_rawDataPointer = 0;  // currently unknown
        s_relocationsPointer = 0;
        s_lineNumbersPointer = 0;
        s_numberOfRelocations = 0;
        s_numberOfLineNumbers = 0;
        s_characteristics = 0;
    }
    void push(std::ofstream& stream) {
        for (uint32_t i = 0; i < 8; i++) { stream << s_name[i]; }
        pushDword(stream,s_virtualSize,true);
        pushDword(stream,s_virtualAddress,true);
        pushDword(stream,s_rawDataSize,true);
        pushDword(stream,s_rawDataPointer,true);
        pushDword(stream,s_relocationsPointer,true);
        pushDword(stream,s_lineNumbersPointer,true);
        pushWord(stream,s_numberOfRelocations,true);
        pushWord(stream,s_numberOfLineNumbers,true);
        pushDword(stream,s_characteristics,true);
    }
    bool read(std::ifstream& stream, uint32_t& count) {
        try {
            for (size_t i = 0; i < 8; i++) s_name[i]=(char)readByte(stream, count);
            s_virtualSize = readDword(stream, count);
            s_virtualAddress = readDword(stream, count);
            s_rawDataSize = readDword(stream, count);
            s_rawDataPointer = readDword(stream, count);
            s_relocationsPointer = readDword(stream, count);
            s_lineNumbersPointer = readDword(stream, count);
            s_numberOfRelocations = readWord(stream, count);
            s_numberOfLineNumbers = readWord(stream, count);
            s_characteristics = readDword(stream, count);
            return true;
        } catch (int code) { return false; }
    }
    void print(std::string delimmiter="\n") {
        std::cout << "s_name: \"" << getName() << '"' << delimmiter;
        std::cout << "s_virtualSize: " << intToHex(s_virtualSize) << delimmiter;
        std::cout << "s_virtualAddress: " << intToHex(s_virtualAddress) << delimmiter;
        std::cout << "s_rawDataSize: " << intToHex(s_rawDataSize) << delimmiter;
        std::cout << "s_rawDataPointer: " << intToHex(s_rawDataPointer) << delimmiter;
        std::cout << "s_relocationsPointer: " << intToHex(s_relocationsPointer) << delimmiter;
        std::cout << "s_lineNumbersPointer: " << intToHex(s_lineNumbersPointer) << delimmiter;
        std::cout << "s_numberOfRelocations: " << intToHex(s_numberOfRelocations) << delimmiter;
        std::cout << "s_numberOfLineNumbers: " << intToHex(s_numberOfLineNumbers) << delimmiter;
        std::cout << "s_characteristics: " << intToHex(s_characteristics) << " -> ";
        if ((s_characteristics&IMAGE_SCN_CNT_CODE)>0) std::cout << "Code, ";
        if ((s_characteristics&IMAGE_SCN_CNT_INITIALIZED_DATA)>0) std::cout << "Initialized data, ";
        if ((s_characteristics&IMAGE_SCN_CNT_UNINITIALIZED_DATA)>0) std::cout << "Uninitialized data, ";
        if ((s_characteristics&IMAGE_SCN_MEM_READ)>0) std::cout << 'R'; else std::cout << '-';
        if ((s_characteristics&IMAGE_SCN_MEM_WRITE)>0) std::cout << 'W'; else std::cout << '-';
        if ((s_characteristics&IMAGE_SCN_MEM_EXECUTE)>0) std::cout << 'E'; else std::cout << '-';
        std::cout << '\n';
    }
    std::string getName() {
        std::string tmp = "        ";
        for (size_t i = 0; i < 8; i++) tmp[i]=s_name[i];
        return tmp;
    }
};

#pragma region import table structs
struct peImportDirTable {
    uint32_t i_lookupTableRVA;
    uint32_t i_timeStamp;
    uint32_t i_forwarderIndex;
    uint32_t i_nameRVA;
    uint32_t i_addressTableRVA;
    peImportDirTable(const uint32_t& lookupTableRVA=0, const uint32_t& nameRVA=0, const uint32_t& addressTableRVA=0) {
        i_lookupTableRVA=lookupTableRVA;
        i_timeStamp=0;
        i_forwarderIndex=0;
        i_nameRVA=nameRVA;
        i_addressTableRVA=addressTableRVA;
    }
    void push(std::vector<uint8_t>& stream) {
        pushDword(stream,i_lookupTableRVA,true);
        pushDword(stream,i_timeStamp,true);
        pushDword(stream,i_forwarderIndex,true);
        pushDword(stream,i_nameRVA,true);
        pushDword(stream,i_addressTableRVA,true);
    }
};

struct peHintNameTable {
    uint16_t i_hint;
    std::string* i_name;
    peHintNameTable(const uint16_t& hint, std::string* name) {
        i_hint=hint;
        i_name=name;
    }
    void push(std::vector<uint8_t>& stream) {
        pushWord(stream,i_hint,true);
        pushChars(stream,(const uint8_t*)i_name->c_str(),i_name->size(),true);
        pushByte(stream,0);
    }
    uint32_t getSize() {
        return sizeof(i_hint)+i_name->size()+1;
    }
};
#pragma endregion import table stuff

#pragma region export table structs
struct peExportDirTable {
    uint32_t e_exportFlags;// should be 0
    uint32_t e_timeStamp;
    uint16_t e_majorVersion;
    uint16_t e_minorVersion;
    uint32_t e_exportNameTableRVA;
    uint32_t e_ordinalBase;
    uint32_t e_numAddressTableEntries;
    uint32_t e_numNamePointers;
    uint32_t e_addressTableRVA;
    uint32_t e_namePointerTableRVA;
    uint32_t e_ordinalTableRVA;
    peExportDirTable(const uint32_t& nameRVA=0, const uint32_t& namePointerTableRVA=0) {
        e_exportFlags=0;
        e_timeStamp=0;
        e_majorVersion=0;
        e_minorVersion=0;
        e_exportNameTableRVA=nameRVA;
        e_ordinalBase=0;
        e_numAddressTableEntries=0;
        e_numNamePointers=0;
        e_addressTableRVA=0;
        e_namePointerTableRVA=namePointerTableRVA;
        e_ordinalTableRVA=0;
    }
    void push(std::vector<uint8_t>& stream) {
        pushDword(stream,e_exportFlags,true);// should be 0
        pushDword(stream,e_timeStamp,true);
        pushWord(stream,e_majorVersion,true);
        pushWord(stream,e_minorVersion,true);
        pushDword(stream,e_exportNameTableRVA,true);
        pushDword(stream,e_ordinalBase,true);
        pushDword(stream,e_numAddressTableEntries,true);
        pushDword(stream,e_numNamePointers,true);
        pushDword(stream,e_addressTableRVA,true);
        pushDword(stream,e_namePointerTableRVA,true);
        pushDword(stream,e_ordinalTableRVA,true);
    }
    bool read(std::ifstream& stream, uint32_t& count) {
        try {
            e_exportFlags = readDword(stream, count);// should be 0
            e_timeStamp = readDword(stream, count);
            e_majorVersion = readWord(stream, count);
            e_minorVersion = readWord(stream, count);
            e_exportNameTableRVA = readDword(stream, count);
            e_ordinalBase = readDword(stream, count);
            e_numAddressTableEntries = readDword(stream, count);
            e_numNamePointers = readDword(stream, count);
            e_addressTableRVA = readDword(stream, count);
            e_namePointerTableRVA = readDword(stream, count);
            e_ordinalTableRVA = readDword(stream, count);
            return true;
        } catch (int code) { return false; }
    }
    bool readAt(std::vector<uint8_t>& vec, const uint32_t index) {
        try {
            e_exportFlags = readDwordAt(vec, index);// should be 0
            e_timeStamp = readDwordAt(vec, index+4);
            e_majorVersion = readWordAt(vec, index+8);
            e_minorVersion = readWordAt(vec, index+10);
            e_exportNameTableRVA = readDwordAt(vec, index+12);
            e_ordinalBase = readDwordAt(vec, index+16);
            e_numAddressTableEntries = readDwordAt(vec, index+20);
            e_numNamePointers = readDwordAt(vec, index+24);
            e_addressTableRVA = readDwordAt(vec, index+28);
            e_namePointerTableRVA = readDwordAt(vec, index+32);
            e_ordinalTableRVA = readDwordAt(vec, index+36);
            return true;
        } catch (int code) { return false; }
    }
    void print(std::string delimmiter="\n") {
        std::cout << "e_exportFlags: " << intToHex(e_exportFlags) << delimmiter;
        std::cout << "e_timeStamp: " << e_timeStamp << delimmiter;
        std::cout << "e_majorVersion: " << e_majorVersion << delimmiter;
        std::cout << "e_minorVersion: " << e_minorVersion << delimmiter;
        std::cout << "e_exportNameTableRVA: " << intToHex(e_exportNameTableRVA) << delimmiter;
        std::cout << "e_ordinalBase: " << e_ordinalBase << delimmiter;
        std::cout << "e_numAddressTableEntries: " << e_numAddressTableEntries << delimmiter;
        std::cout << "e_numNamePointers: " << e_numNamePointers << delimmiter;
        std::cout << "e_addressTableRVA: " << intToHex(e_addressTableRVA) << delimmiter;
        std::cout << "e_namePointerRVA: " << intToHex(e_namePointerTableRVA) << delimmiter;
        std::cout << "e_ordinalTableRVA: " << intToHex(e_ordinalTableRVA) << '\n';
    }
};
// Export address table: first, after the dir table is list of RVAs, which is either an address to code/memory in the file, or it is address to a string referencing another Dll
// Export name pointer table: after this should be an array of RVAs to string of the names of each export
// Ordinal table: list of 16bit numbers that are the ordinal of each name pointer, used to index into the export address table
// Export name table: finally there is an array of null terminated strings of the names that the Export name pointer table can point to.
#pragma endregion export table structs

#pragma endregion// structs

std::string getEnvVar( std::string const & key );
std::vector<std::string> StringSplit(std::string input, char delimitter);
std::string getFullFilePath(const std::string& path);
struct dllExportData {
    uint16_t namePtrIndex;
    std::string name;
    std::string dllName;
};
std::vector<dllExportData> parseDll();

#pragma region handlers
class Pe64SectionHandler;

struct Pe64Label {
    std::string name;
    Pe64SectionHandler* base;
    uint32_t offset;
    Pe64Label(const std::string& _name, Pe64SectionHandler* _base, const uint32_t& _offset) : name(_name),base(_base),offset(_offset) {};
};
struct Pe64LabelResolution {
    std::string name;
    Pe64SectionHandler* base;
    uint32_t setAt;
    int32_t relativeToOffset;
    Pe64LabelResolution(const std::string& _name, Pe64SectionHandler* _base, const uint32_t& _setAt, const int32_t& _relativeToOffset) : name(_name),base(_base),setAt(_setAt),relativeToOffset(_relativeToOffset) {};
};
struct pe64ImportData {
    uint16_t hint;
    std::string* name;
    std::string* dllName;
};
class Pe64Handler {
private:
    std::vector<Pe64SectionHandler *> sectionHeaders;
    peHdr peHeader;
    peOptHdrStdFields64 peStdFieldsHeader;
    peOptHdrSpecFields64 peSpecFieldsHeader;
    peOptHdrDataDirs peDataDirHeader;

    std::vector<Pe64Label> labels;
    std::vector<Pe64LabelResolution> labelResolutions;

    std::vector<std::string> avaliableDlls;
    std::vector<pe64ImportData> imports;

public:
    Pe64Handler();
    ~Pe64Handler();
    void push(std::ofstream& stream);
    Pe64SectionHandler *addSec(const char name[9], uint32_t characteristics);
    void defineLabel(const std::string& name, Pe64SectionHandler* base, const uint32_t& offset);
    void resolveLabel(const std::string& name, Pe64SectionHandler* base, const uint32_t& setAt, const int32_t& relativeToOffset);
    void addImport(const std::string& dllName);

    friend Pe64SectionHandler;
};

class Pe64SectionHandler {
    Pe64Handler& peHandler;
    peSectionHdr sectionHeader;
public:
    uint32_t sectionAlignment=1;
    uint32_t fileAlignment=1;
    std::vector<uint8_t> data;

    Pe64SectionHandler(Pe64Handler& _peHandler, const char name[8], uint32_t characteristics);
    bool isCode() {
        return ((sectionHeader.s_characteristics&IMAGE_SCN_CNT_CODE)!=0);
    }
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

    friend Pe64Handler;
    template <typename T>
    friend void pushChars(T& section, const uint8_t* chars, const size_t& len, const bool& LSB);
};
template <>
void pushChars(Pe64SectionHandler*& reciever, const uint8_t* chars, const size_t& len, const bool& LSB);
#pragma endregion// handlers

#endif  // _PE_H
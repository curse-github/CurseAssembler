#ifndef _PE_H
#define _PE_H

// https://upload.wikimedia.org/wikipedia/commons/1/1b/Portable_Executable_32_bit_Structure_in_SVG_fixed.svg
// https://learn.microsoft.com/en-us/windows/win32/debug/pe-format

#include <time.h>

#include <vector>

// #include "PE/winnt.h"
#include "globalConstants.h"
#include "peConstants.h"

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
        p_numberOfSections = 0;  // currently unknown
        p_timeDateStamp = time(0);
        p_pointerToSymbolTable = 0;  // currently unknown
        p_numberOfSymbols = 0;       // currently unknown
        p_sizeOfOptionalHeader = 0;  // currently unknown
        p_characteristics = IMAGE_FILE_CHARACTERISTIC_EXECUTABLE_IMAGE | IMAGE_FILE_CHARACTERISTIC_32BIT_MACHINE;
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
        p_majorLinkerVersion = 0;
        p_minorLinkerVersion = 0;
        p_sizeOfCode = 0;               // currently unknown
        p_sizeOfInitializedData = 0;    // currently unknown
        p_sizeOfUninitializedData = 0;  // currently unknown
        p_addressOfEntryPoint = 0;      // currently unknown
        p_baseOfCode = 0;               // currently unknown
        p_baseOfData = 0;               // currently unknown
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
    unsigned short p_win32VersionValue;  // must always be 0
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
    unsigned int p_numberOfRvaAndSizes;
    peOptHdrSpecFields32() {
        p_imageBase = VirtAddr32;
        p_sectionAlignment = Align32;
        p_fileAlignment = Align32;
        p_majorOperatingSystemVersion = 0;
        p_minorOperatingSystemVersion = 0;
        p_majorImageVersion = 0;
        p_minorImageVersion = 0;
        p_majorSubsystemVersion = 0;
        p_minorSubsystemVersion = 0;
        p_win32VersionValue = 0;  // must always be 0
        p_sizeOfHeaders = 0;      // currently unknown
        p_checkSum = 0;           // currently unknown
        p_subSystem = IMAGE_SUBSYSTEM_WINDOWS_CUI;
        p_dllCharacteristics = 0;
        p_sizeOfStackReserve = 0;   // currently unknown
        p_sizeOfStackCommit = 0;    // currently unknown
        p_sizeOfHeapReserve = 0;    // currently unknown
        p_sizeOfHeapCommit = 0;     // currently unknown
        p_loaderFlags = 0;          // must always be 0
        p_numberOfRvaAndSizes = 0;  // currently unknown
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
    unsigned long p_exportTable;
    unsigned long p_importTable;
    unsigned long p_resourceTable;
    unsigned long p_exceptionTable;
    unsigned long p_certificateTable;
    unsigned long p_baseRelocationTable;
    unsigned long p_debug;
    unsigned long p_architecture;  // must always be 0
    unsigned long p_globalPtr;
    unsigned long p_TlsTable;
    unsigned long p_loadConfigTable;
    unsigned long p_boundImport;
    unsigned long p_IAT;
    unsigned long p_delayImportDescriptor;
    unsigned long p_ClrRuntimeHeader;
    unsigned long p_zero;  // must always be 0, obviously
    peOptHdrDataDirs32() {
        p_exportTable = 0;            // currently unknown
        p_importTable = 0;            // currently unknown
        p_resourceTable = 0;          // currently unknown
        p_exceptionTable = 0;         // currently unknown
        p_certificateTable = 0;       // currently unknown
        p_baseRelocationTable = 0;    // currently unknown
        p_debug = 0;                  // currently unknown
        p_architecture = 0;           // must always be 0
        p_globalPtr = 0;              // ??????
        p_TlsTable = 0;               // currently unknown
        p_loadConfigTable = 0;        // currently unknown
        p_boundImport = 0;            // currently unknown
        p_IAT = 0;                    // currently unknown
        p_delayImportDescriptor = 0;  // currently unknown
        p_ClrRuntimeHeader = 0;       // currently unknown
        p_zero = 0;                   // must always be 0
    }
};

// section tables

struct peSectionHdr {
    char s_name[8];
    unsigned int s_virtualSize;
    unsigned int s_virtualAddress;
    unsigned int s_rawDataSize;
    unsigned int s_rawDataPointer;
    unsigned int s_relocationsPointer;  // ignore unless object file
    unsigned int s_lineNumbersPointer;
    unsigned short s_numberOfRelocations;
    unsigned short s_numberOfLongNumbers;
    unsigned int s_characteristics;
    peSectionHdr(const char name[8], unsigned int characteristics) {
        for (unsigned int i = 0; i < 8; i++) s_name[i]=name[i];
        s_virtualSize = 0;  // currently unknown
        s_virtualAddress = 0;  // currently unknown
        s_rawDataSize = 0;  // currently unknown
        s_rawDataPointer = 0;  // currently unknown
        s_relocationsPointer=0;
        s_lineNumbersPointer=0;// currently unknown
        s_numberOfRelocations=0;// currently unknown
        s_numberOfLongNumbers=0;// currently unknown
        s_characteristics=characteristics;
    }
};
class PeSegmentHandler;
class PeHandler {
private:
    std::vector<PeSegmentHandler *> segmentHeaders32;

public:
};

#endif  // _PE_H
#include "globalConstants.h"
#ifdef _WIN32
    namespace WIN {
        #include <windows.h>
    }
    enum class Color {Red=FOREGROUND_RED,Green=FOREGROUND_GREEN,Blue=FOREGROUND_BLUE,DarkCyan=FOREGROUND_GREEN|FOREGROUND_BLUE,Cyan=FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_INTENSITY,White=FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_INTENSITY};
    void setPrintColor(const Color& color) {// function that sets the console output color based on the enum passed in
        WIN::SetConsoleOutputCP(CP_UTF8);
        WIN::SetConsoleTextAttribute(WIN::GetStdHandle((WIN::DWORD)-11),(int)color);
    }
#else
    enum class Color {Red=31,Green=32,Blue=34,DarkCyan=36,Cyan=96,White=0};
    void setPrintColor(const Color& color) {// function that sets the console output color based on the enum passed in
        std::cout << "\u001b[" << (int)color << 'm';
    }
#endif

void printBytes(const std::vector<uint8_t>& vec, const uint32_t& startOffset, const bool& isInMemory=false, const uint32_t& startAddress=0, const uint32_t& bytesPerRow = 0x20u) {
    // header
    setPrintColor(Color::Cyan);std::cout << "  Offset";
    if (isInMemory) { setPrintColor(Color::White); std::cout << ",  "; }
    if (isInMemory) { setPrintColor(Color::Cyan); std::cout << "Address"; }
    setPrintColor(Color::White); std::cout << ": ";
    setPrintColor(Color::Cyan); for (size_t i = 0; i < bytesPerRow; i++) std::cout << intToHex((uint8_t)i,"") << ' ';
    setPrintColor(Color::White); std::cout << "| ";
    setPrintColor(Color::Cyan); std::cout << "Ascii\n";
    setPrintColor(Color::White);
    // first row, if it needs to be offset
    int32_t actualStartOffset = startOffset;
    int32_t actualStartAddress = startAddress;
    int32_t size = vec.size();
    if ((startOffset%bytesPerRow)!=0) {
        setPrintColor(Color::Cyan); std::cout << intToHex((uint32_t)(startOffset-(startOffset%bytesPerRow)),"");
        if (isInMemory) setPrintColor(Color::White); std::cout << ", ";
        if (isInMemory) setPrintColor(Color::Cyan); std::cout << intToHex((uint32_t)(startAddress-(startOffset%bytesPerRow)),"");
        setPrintColor(Color::White); std::cout << ": ";
        for (uint32_t i = 0; i < bytesPerRow-(startOffset%bytesPerRow); i++) std::cout << "-- ";
        for (uint32_t i = 0; i < (startOffset%bytesPerRow); i++) std::cout << intToHex(vec[i],"") << ' ';
        std::cout << "| ";
        for (uint32_t i = 0; i < bytesPerRow-(startOffset%bytesPerRow); i++) std::cout << ' ';
        for (uint32_t i = 0; i < (startOffset%bytesPerRow); i++) {
            const char chr = vec[i];
            std::cout << (((chr>=' ')&&(chr<='~'))?chr:'.');
        }
        actualStartOffset+=(startOffset%bytesPerRow);
        actualStartAddress+=(startOffset%bytesPerRow);
        if (size>=(startOffset%bytesPerRow)) size-=(startOffset%bytesPerRow); else size=0;
        std::cout << '\n';
    }
    for (size_t i = 0; i < size/bytesPerRow; i++) {
        setPrintColor(Color::Cyan); std::cout << intToHex((uint32_t)(actualStartOffset+i*bytesPerRow),"");
        if (isInMemory) { setPrintColor(Color::White); std::cout << ", "; }
        if (isInMemory) { setPrintColor(Color::Cyan); std::cout << intToHex((uint32_t)(actualStartAddress+i*bytesPerRow),""); }
        setPrintColor(Color::White); std::cout << ": ";
        for (uint32_t j = 0; j < bytesPerRow; j++) std::cout << intToHex(vec[i*bytesPerRow+j],"") << ' ';
        std::cout << "| ";
        for (uint32_t j = 0; j < bytesPerRow; j++) {
            const char chr = vec[i*bytesPerRow+j];
            std::cout << (((chr>=' ')&&(chr<='~'))?chr:'.');
        }
        std::cout << '\n';
    }
    const uint32_t end = size;
    if ((end%bytesPerRow)!=0) {
        const uint32_t lastRowStart = end-(end%bytesPerRow);
        const uint32_t lastRowStartOffset = actualStartOffset+lastRowStart;
        const uint32_t lastRowStartAddress = actualStartAddress+lastRowStart;
        setPrintColor(Color::Cyan); std::cout << intToHex(lastRowStartOffset,"");
        if (isInMemory) { setPrintColor(Color::White); std::cout << ", "; }
        if (isInMemory) { setPrintColor(Color::Cyan); std::cout << intToHex(lastRowStartAddress,""); }
        setPrintColor(Color::White); std::cout << ": ";
        for (uint32_t i = lastRowStart; i < end; i++) std::cout << intToHex(vec[i],"") << ' ';
        for (uint32_t i = 0; i < bytesPerRow-(end%bytesPerRow); i++) std::cout << "-- ";
        std::cout << "| ";
        for (uint32_t i = lastRowStart; i < end; i++) {
            const char chr = vec[i];
            std::cout << (((chr>=' ')&&(chr<='~'))?chr:'.');
        }
        std::cout << '\n';
    }
}
int main(int argc, char *argv[]) {
    // get file
    std::string fullPath = getFullFilePath(argv[1]);
    if (fullPath.size()==0) std::cout << "File not found.\n";
    std::cout << "reading: \"" << fullPath << "\"\n";
    std::ifstream inFile(fullPath, std::ios::binary);
    if (inFile.bad()) { std::cout << "File ifStream bad.\n"; inFile.close(); return 1; }
    uint32_t count = 0;

    std::vector<uint8_t> allData;
    elfHdr64 elfHeader;
    uint16_t e_numSegmentHdrs = 0;
    uint16_t e_numSectionHdrs = 0;
    std::vector<elfSegmentHdr64> segmentHeaders;
    std::vector<std::vector<uint8_t>> segmentData;
    std::vector<elfSectionHdr64> sectionHeaders;
    std::vector<std::vector<uint8_t>> sectionData;
    uint32_t secNameStrTableOffset = 0;
    try {
        // read elf header
        pushChars(allData,readBytes(inFile,count,sizeof(elfHdr64)));
        if (!elfHeader.readAt(allData,0)) { inFile.close(); return 1; }
        pushChars(allData,readBytes(inFile,count,elfHeader.e_segmentHdrOffset-count));
        elfHeader.print();
        // read segment headers
        e_numSegmentHdrs=elfHeader.e_numSegmentHdrs;
        if (e_numSegmentHdrs==0) { std::cout << "File has no segments.\n"; inFile.close(); return 1; }
        const uint32_t segmentHdrStart = elfHeader.e_segmentHdrOffset;
        const uint32_t segmentHdrEnd = segmentHdrStart+sizeof(elfSegmentHdr64)*e_numSegmentHdrs;
        if (segmentHdrEnd>count) pushChars(allData,readBytes(inFile,count,segmentHdrEnd-count));
        for (uint16_t i = 0; i < e_numSegmentHdrs; i++) {
            elfSegmentHdr64 segmentHdrI(0,0);
            if (!segmentHdrI.readAt(allData,segmentHdrStart+sizeof(elfSegmentHdr64)*i)) { std::cout << "error reading segment header #" << (int)segmentHeaders.size() << "\n"; inFile.close(); return 1; }
            segmentHeaders.push_back(segmentHdrI);
        }
        // read segment data
        for (uint16_t i = 0; i < e_numSegmentHdrs; i++) {
            if (segmentHeaders[i].s_sizeFile==0) continue;
            uint32_t segmentStart = segmentHeaders[i].s_fileOffset;
            uint32_t segmentEnd = segmentStart+segmentHeaders[i].s_sizeFile;
            if (segmentEnd>count) pushChars(allData,readBytes(inFile,count,segmentEnd-count));// read in the bytes if it does not have enough
            segmentData.push_back(readBytesAt(allData, segmentStart, segmentHeaders[i].s_sizeFile));
        }
        if (elfHeader.e_numSectionHdrs>0) {
            // read section headers
            e_numSectionHdrs=elfHeader.e_numSectionHdrs;
            const uint32_t sectionHdrStart = elfHeader.e_sectionHdrOffset;
            const uint32_t sectionHdrEnd = sectionHdrStart+sizeof(elfSectionHdr64)*e_numSectionHdrs;
            if (sectionHdrEnd>count) pushChars(allData,readBytes(inFile,count,sectionHdrEnd-count));
            for (uint16_t i = 0; i < e_numSectionHdrs; i++) {
                elfSectionHdr64 sectionHdrI(0,0,0);
                if (!sectionHdrI.readAt(allData,sectionHdrStart+sizeof(elfSectionHdr64)*i)) { std::cout << "error reading section header #" << i << "\n"; inFile.close(); return 1; }
                sectionHeaders.push_back(sectionHdrI);
            }
            secNameStrTableOffset=sectionHeaders[elfHeader.e_stringTableNdx].s_fileOffset;
            // read section data
            for (uint16_t i = 0; i < e_numSectionHdrs; i++) {
                if (sectionHeaders[i].s_size==0) continue;
                uint32_t sectionStart = sectionHeaders[i].s_fileOffset;
                uint32_t sectionEnd = sectionStart+sectionHeaders[i].s_size;
                if (sectionEnd>count) pushChars(allData,readBytes(inFile,count,sectionEnd-count));// read in the bytes if it does not have enough
                sectionData.push_back(readBytesAt(allData, sectionStart, sectionHeaders[i].s_size));
            }
        } else {
            std::cout << "File has no sections.\n"; inFile.close();
        }
    } catch(const int& err) { std::cout << "EOF reached before expected"; inFile.close(); return 1; }
    catch ( ... ) { std::cout << "error..."; inFile.close(); return 1; }

    std::cout << '\n';
    // print elf header
    elfHeader.print();
    // print segments
    uint32_t segmentDataIndex = 0;
    for (uint16_t i = 0; i < e_numSegmentHdrs; i++) {
        std::cout << "\nSegment #" << (i+1) << ":\n    ";
        segmentHeaders[i].print("\n    ");
        setPrintColor(Color::Green); std::cout << "Segment #" << (i+1) << " has " << segmentHeaders[i].s_sizeMemory << " bytes of data\n"; setPrintColor(Color::White);
        if (segmentHeaders[i].s_sizeFile>0) { printBytes(segmentData[segmentDataIndex],segmentHeaders[i].s_fileOffset,true,segmentHeaders[i].s_virtualAddress,32); segmentDataIndex++; }
        else { setPrintColor(Color::Red); std::cout << "No data in file\n"; setPrintColor(Color::White); }
    }
    if (elfHeader.e_numSectionHdrs>0) {// has sections
        std::cout << "\n\n";
        //print sections
        uint32_t sectionDataIndex = 0;
        for (uint16_t i = 0; i < e_numSectionHdrs; i++) {
            std::cout << "\nSection #" << (i+1) << " Header:\n    ";
            sectionHeaders[i].print(allData, secNameStrTableOffset, "\n    ");
            if (sectionHeaders[i].s_size==0) {
                setPrintColor(Color::Red); std::cout << "Section #" << (i+1) << " has no data in file\n"; setPrintColor(Color::White); continue;
            } else if ((sectionHeaders[i].s_type==ELF_SECTION_TYPE_DYNSYM)||(sectionHeaders[i].s_type==ELF_SECTION_TYPE_SYMTAB)) {
                // print symbol tables
                if (sectionHeaders[i].s_type==ELF_SECTION_TYPE_DYNSYM) { setPrintColor(Color::Green); std::cout << "Section #" << (i+1) << " is a dynamic symbol table\n"; setPrintColor(Color::White); }
                else if (sectionHeaders[i].s_type==ELF_SECTION_TYPE_SYMTAB) { setPrintColor(Color::Green); std::cout << "Section #" << (i+1) << " is a symbol table\n"; setPrintColor(Color::White); }
                elfSectionHdr64 exportStrTabSec = sectionHeaders[sectionHeaders[i].s_link];
                uint32_t numEntries = sectionHeaders[i].s_size/sizeof(elf64Symbol);
                if (sectionHeaders[i].s_type==ELF_SECTION_TYPE_DYNSYM) std::cout << "num dynsym entries: " << numEntries << '\n';
                else if (sectionHeaders[i].s_type==ELF_SECTION_TYPE_SYMTAB) std::cout << "num sym entries: " << numEntries << '\n';
                for (uint32_t j = 0; j < numEntries; j++) {
                    elf64Symbol symI;
                    if (!symI.readAt(sectionData[sectionDataIndex],sizeof(elf64Symbol)*j)) std::cout << "error reading symbol in section #" << (i+1) << '\n';
                    setPrintColor(Color::Cyan); std::cout << "    " << intToHex((uint32_t)(sectionHeaders[i].s_fileOffset+sizeof(elf64Symbol)*j));
                    setPrintColor(Color::White); std::cout << ": ";
                    symI.print(allData,exportStrTabSec.s_fileOffset,"\n    ");
                }
            } else if (sectionHeaders[i].s_type==ELF_SECTION_TYPE_RELA) {
                // print string tables
                setPrintColor(Color::Green); std::cout << "Section #" << (i+1) << " is a RELA table\n"; setPrintColor(Color::White);
                //elfSectionHdr64 exportStrTabSec = sectionHeaders[sectionHeaders[i].s_link];
                uint32_t numEntries = sectionHeaders[i].s_size/sizeof(elf64Rela);
                for (size_t j = 0; j < numEntries; j++) {
                    elf64Rela relaEntr;
                    if (!relaEntr.readAt(sectionData[sectionDataIndex],sizeof(elf64Rela)*j)) std::cout << "error reading relaEntr in section #" << (i+1) << '\n';
                    setPrintColor(Color::Cyan); std::cout << "    " << intToHex((uint32_t)(sectionHeaders[i].s_fileOffset+sizeof(elf64Dyn)*j));
                    setPrintColor(Color::White); std::cout << ": ";
                    relaEntr.print(allData,sectionHeaders, secNameStrTableOffset,"\n    ");
                }
            } else if (sectionHeaders[i].s_type==ELF_SECTION_TYPE_DYNAMIC) {
                // print entries from dynamic section
                setPrintColor(Color::Green); std::cout << "Section #" << (i+1) << " is a dynamic data section\n"; setPrintColor(Color::White);
                elfSectionHdr64 exportStrTabSec = sectionHeaders[sectionHeaders[i].s_link];
                uint32_t numEntries = sectionHeaders[i].s_size/sizeof(elf64Dyn);
                for (size_t j = 0; j < numEntries; j++) {
                    elf64Dyn dynEntr(0,0);
                    if (!dynEntr.readAt(sectionData[sectionDataIndex],sizeof(elf64Dyn)*j)) std::cout << "error reading dynEntr in section #" << (i+1) << '\n';
                    setPrintColor(Color::Cyan); std::cout << "    " << intToHex((uint32_t)(sectionHeaders[i].s_fileOffset+sizeof(elf64Dyn)*j));
                    setPrintColor(Color::White); std::cout << ": ";
                    dynEntr.print(allData,exportStrTabSec.s_fileOffset, sectionHeaders, secNameStrTableOffset);
                }
            } else if (sectionHeaders[i].s_type==ELF_SECTION_TYPE_STRTAB) {
                // print string tables
                setPrintColor(Color::Green); std::cout << "Section #" << (i+1) << " is a string table\n"; setPrintColor(Color::White);
                setPrintColor(Color::Cyan); std::cout << "    Offset";
                setPrintColor(Color::White); std::cout << ", ";
                setPrintColor(Color::Cyan); std::cout << "Addr\n";
                uint16_t j = 0;
                while (j<sectionHeaders[i].s_size) {
                    std::string tmpStr = readStringAt(sectionData[sectionDataIndex],j);
                    setPrintColor(Color::Cyan); std::cout << "    " << intToHex((uint16_t)j);
                    setPrintColor(Color::White); std::cout << ", ";
                    setPrintColor(Color::Cyan); std::cout << intToHex((uint32_t)(sectionHeaders[i].s_fileOffset+j));
                    setPrintColor(Color::White); std::cout << ": \"" << tmpStr << "\"\n";
                    j+=tmpStr.size()+1;
                }
            } else {
                // otherwise just print bytes in both hex and ascii
                setPrintColor(Color::Green); std::cout << "Section #" << (i+1) << " has " << sectionHeaders[i].s_size << " bytes of data\n"; setPrintColor(Color::White);
                uint32_t entsize = sectionHeaders[i].s_entSize;
                if ((entsize==0)||(entsize==1)) entsize=32;
                if ((sectionHeaders[i].s_virtualAddress==0) && (sectionHeaders[i].s_fileOffset!=0))
                    printBytes(sectionData[sectionDataIndex],sectionHeaders[i].s_fileOffset,false,0,entsize);
                else
                    printBytes(sectionData[sectionDataIndex],sectionHeaders[i].s_fileOffset,true,sectionHeaders[i].s_virtualAddress,entsize);
            }
            sectionDataIndex++;
        }
    }
    inFile.close();
    return 0;
}
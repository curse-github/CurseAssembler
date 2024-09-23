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

void printBytes(const std::vector<uint8_t>& vec, const uint32_t& startOffset) {
    setPrintColor(Color::Cyan);
    std::cout << "  Offset: 00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f\n";
    setPrintColor(Color::White);
    if ((startOffset%16u)!=0) for (uint32_t i = 0; i < 16u-(startOffset%16u); i++) std::cout << "   ";
    size_t size = vec.size();
    for (size_t i = 0; i < size; i++) {
        if (((startOffset+i)%16u)==0) { setPrintColor(Color::Cyan); std::cout << intToHex((uint32_t)(startOffset+i),""); setPrintColor(Color::White); std::cout << ": ";}
        std::cout << intToHex(vec[i],"");
        if (((startOffset+i+1)%16u)==0) std::cout << '\n'; else std::cout << ' ';
    }
    if (((startOffset+size)%16u)!=0) std::cout << '\n';
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
    std::vector<elfSegmentHdr64> segmentHeaders;
    std::vector<std::vector<uint8_t>> segmentData;
    std::vector<elfSectionHdr64> sectionHeaders;
    std::vector<std::vector<uint8_t>> sectionData;
    try {
        // read elf header
        pushChars(allData,readBytes(inFile,count,sizeof(elfHdr64)));
        bool peHdrPullReturn = elfHeader.readAt(allData,0);
        if (!peHdrPullReturn) { inFile.close(); return 1; }
        trashBytes(inFile,count,elfHeader.e_segmentHdrOffset-count);
        // read segment headers
        e_numSegmentHdrs=elfHeader.e_numSegmentHdrs;
        for (uint16_t i = 0; i < e_numSegmentHdrs; i++) {
            pushChars(allData,readBytes(inFile,count,sizeof(elfSegmentHdr64)));
            elfSegmentHdr64 segmentHdrI(0,0);
            if (!segmentHdrI.readAt(allData,sizeof(elfHdr64)+sizeof(elfSegmentHdr64)*i)) { std::cout << "error reading segment header #" << (int)segmentHeaders.size() << "\n"; inFile.close(); return 1; }
            segmentHeaders.push_back(segmentHdrI);
        }
        // read segment data
        if (e_numSegmentHdrs==0) { std::cout << "File has no segments.\n"; inFile.close(); return 1; }
        for (uint16_t i = 0; i < e_numSegmentHdrs; i++) {
            if (segmentHeaders[i].s_sizeFile==0) continue;
            uint32_t start = segmentHeaders[i].s_fileOffset;
            uint32_t end = start+segmentHeaders[i].s_sizeFile;
            if (end>count) pushChars(allData,readBytes(inFile,count,end-count));// read in the bytes if it does not have enough
            segmentData.push_back(readBytesAt(allData, start, segmentHeaders[i].s_sizeFile));
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
        if (segmentHeaders[i].s_sizeFile>0) { printBytes(segmentData[segmentDataIndex],segmentHeaders[i].s_fileOffset); segmentDataIndex++; }
        else { setPrintColor(Color::Red); std::cout << "No data in file\n"; setPrintColor(Color::White); }
    }

    inFile.close();
    return 0;
}
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
enum class Color {Red=0,Green=1,Blue=2,DarkCyan=3,Cyan=4,White=5};
void setPrintColor(const Color& color) {// function that sets the console output color based on the enum passed in
}
#endif

void printBytes(const std::vector<uint8_t>& vec, const uint32_t& startOffset) {
    setPrintColor(Color::Cyan);
    std::cout << "  Offset 00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f\n";
    setPrintColor(Color::White);
    if ((startOffset%16u)!=0) for (uint32_t i = 0; i < 16u-(startOffset%16u); i++) std::cout << "   ";
    size_t size = vec.size();
    for (size_t i = 0; i < size; i++) {
        if (((startOffset+i)%16u)==0) { setPrintColor(Color::Cyan); std::cout << intToHex((uint32_t)(startOffset+i),"") << ' '; setPrintColor(Color::White); }
        std::cout << intToHex(vec[i],"");
        if (((startOffset+i+1)%16u)==0) std::cout << '\n'; else std::cout << ' ';
    }
    if (((startOffset+size)%16u)!=0) std::cout << '\n';
}
bool tmp(const elfSegmentHdr64& a, const elfSegmentHdr64& b) {
    return a.s_fileOffset<b.s_fileOffset;
}
int main(int argc, char *argv[]) {
    // get file
    std::string fullPath = getFullFilePath(argv[1]);
    if (fullPath.size()==0) std::cout << "File not found.\n";
    std::cout << "reading: \"" << fullPath << "\"\n";
    std::ifstream inFile(fullPath, std::ios::binary);
    if (inFile.bad()) { std::cout << "File ifStream bad.\n"; return 1; }
    uint32_t count = 0;

    elfHdr64 elfHeader;
    uint16_t e_numSegmentHdrs = 0;
    std::vector<elfSegmentHdr64> segmentHeaders;
    std::vector<std::vector<uint8_t>> sectionData;
    try {
        // read elf header
        bool peHdrPullReturn = elfHeader.read(inFile,count);
        if (!peHdrPullReturn) return 1;
        std::cout << "trashing " << (elfHeader.e_segmentHdrOffset-count) << " bytes\n";
        trashBytes(inFile,count,elfHeader.e_segmentHdrOffset-count);
        elfHeader.print();
        std::cout << '\n';
        // read segment headers
        e_numSegmentHdrs=elfHeader.e_numSegmentHdrs;
        for (uint16_t i = 0; i < e_numSegmentHdrs; i++) {
            elfSegmentHdr64 segmentHdrI(0,0);
            if (!segmentHdrI.read(inFile,count)) { std::cout << "error reading segment header #" << (int)segmentHeaders.size() << "\n"; return 1; }
            if ((segmentHdrI.s_type!=ELF_SEGMENT_TYPE_NOTE)&&(segmentHdrI.s_sizeFile>0)&&(segmentHdrI.s_fileOffset>=(sizeof(elfSegmentHdr64)+e_numSegmentHdrs*sizeof(elfSegmentHdr64))))
                segmentHeaders.push_back(segmentHdrI);
        }
        std::sort(segmentHeaders.begin(),segmentHeaders.end(),tmp);
        e_numSegmentHdrs=segmentHeaders.size();
        for (uint16_t i = 0; i < e_numSegmentHdrs; i++) {
            std::cout << "Section #" << i << " header:\n    ";
            segmentHeaders[i].print("\n    ");
        }
        // read segment data
        if (e_numSegmentHdrs==0) { std::cout << "File has no segments.\n"; return 1; }
        for (uint16_t i = 0; i < e_numSegmentHdrs; i++) {
            trashBytes(inFile,count,segmentHeaders[i].s_fileOffset-count);
            std::cout << "count: " << intToHex(count) << '\n';
            sectionData.push_back(readBytes(inFile, count, segmentHeaders[i].s_sizeMemory));
        }
        for (uint16_t i = 0; i < e_numSegmentHdrs; i++) {
            std::cout << "Section #" << i << " data:";
            printBytes(sectionData[i],segmentHeaders[i].s_fileOffset);
        }
    } catch(const int& err) { std::cout << "EOF reached before expected"; return 1; }
    catch ( ... ) { std::cout << "error..."; return 1; }


    inFile.close();
    return 0;
}
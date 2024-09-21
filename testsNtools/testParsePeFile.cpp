#include "globalConstants.h"
namespace WIN {
    #include <windows.h>
}
enum class Color {Red=FOREGROUND_RED,Green=FOREGROUND_GREEN,Blue=FOREGROUND_BLUE,DarkCyan=FOREGROUND_GREEN|FOREGROUND_BLUE,Cyan=FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_INTENSITY,White=FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_INTENSITY};
void setPrintColor(const Color& color) {// function that sets the console output color based on the enum passed in
    WIN::SetConsoleOutputCP(CP_UTF8);
    WIN::SetConsoleTextAttribute(WIN::GetStdHandle((WIN::DWORD)-11),(int)color);
}

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
int main(int argc, char *argv[]) {
    // get file
    std::string fullPath = getFullFilePath(argv[1]);
    if (fullPath.size()==0) std::cout << "File not found.\n";
    std::cout << "reading: \"" << fullPath << "\"\n";
    std::ifstream inFile(fullPath, std::ios::binary);
    if (inFile.bad()) { std::cout << "File ifStream bad.\n"; return 1; }
    uint32_t count = 0;

    peHdr peHeader;
    peOptHdrStdFields64 stdFieldsHeader;
    peOptHdrSpecFields64 specFieldsHeader;
    peOptHdrDataDirs dataDirs;
    uint16_t p_numberOfSections = 0;
    uint32_t PeHeaderOffset = 0;
    std::vector<peSectionHdr> sectionHeaders;
    std::vector<std::vector<uint8_t>> sectionData;

    bool isDll = false;
    SectionPlusOffset exportTableLocation;
    peExportDirTable exportDirTable(0,0);
    std::string dllName;
    std::vector<std::string> exportNames;
    std::vector<dllExportData> exports;
    try {
        peHdr::peHdrPullReturnData peHdrPullReturn = peHeader.read(inFile,count);
        if (!peHdrPullReturn.isValid) return 1;
        isDll=((peHeader.p_characteristics&IMAGE_FILE_CHARACTERISTIC_DLL)!=0);
        PeHeaderOffset = peHdrPullReturn.PeHeaderOffset;
        bool stdFieldsHdrPullReturn = stdFieldsHeader.read(inFile,count);
        if (!stdFieldsHdrPullReturn) return 1;
        bool specFieldsHdrPullReturn = specFieldsHeader.read(inFile,count);
        if (!specFieldsHdrPullReturn) return 1;
        bool dataDirsPullReturn = dataDirs.read(inFile,count);
        if (!dataDirsPullReturn) return 1;
        p_numberOfSections = peHeader.p_numberOfSections;
        for (size_t i = 0; i < p_numberOfSections; i++) {
            peSectionHdr sectionHdrI("        ");
            sectionHdrI.read(inFile,count);
            sectionHeaders.push_back(sectionHdrI);
        }
        // read sections
        for (size_t i = 0; i < p_numberOfSections; i++) {
            trashBytes(inFile,count,sectionHeaders[i].s_rawDataPointer-count);
            sectionData.push_back(readBytes(inFile, count, sectionHeaders[i].s_virtualSize));
        }
        if (isDll) {
            // find export table
            if (dataDirs.p_exportTableRVA==0) { std::cout << "DLL has no export table."; return {}; }
            exportTableLocation = RVAtoSectionPlusOffset(dataDirs.p_exportTableRVA,dataDirs.p_exportTableSize,sectionHeaders);
            if (exportTableLocation.section==nullptr) { std::cout << "DLL export table could not be found."; return {}; }
            const peSectionHdr* sectionWithExportTable = exportTableLocation.section;
            std::vector<uint8_t>* sectionDataWithExportTable = &sectionData[exportTableLocation.index];
            exportDirTable.readAt(*sectionDataWithExportTable,exportTableLocation.offset);
            // parse data in export table
            dllName = readStringAt(*sectionDataWithExportTable,exportDirTable.e_exportNameTableRVA-sectionWithExportTable->s_virtualAddress);
            for (uint16_t i = 0; i < exportDirTable.e_numNamePointers; i++) {
                uint32_t namePointer = readDwordAt(*sectionDataWithExportTable,exportDirTable.e_namePointerTableRVA-sectionWithExportTable->s_virtualAddress+i*4);
                std::string name = readStringAt(*sectionDataWithExportTable,namePointer-sectionWithExportTable->s_virtualAddress);
                //uint16_t ordinal = readWordAt(*sectionDataWithExportTable,exportDirTable.e_ordinalTableRVA-sectionWithExportTable->s_virtualAddress+i*2);
                exports.push_back({i,name,dllName});
            }
        }
    } catch(const int& err) { std::cout << "EOF reached before expected"; return 1; }
    catch ( ... ) { std::cout << "error..."; return 1; }

    uint16_t p_characteristics = peHeader.p_characteristics;
    uint32_t p_addressOfEntryPoint = stdFieldsHeader.p_addressOfEntryPoint;
    uint64_t p_imageBase = specFieldsHeader.p_imageBase;
    uint32_t p_sectionAlignment = specFieldsHeader.p_sectionAlignment;
    uint32_t p_fileAlignment = specFieldsHeader.p_fileAlignment;
    uint32_t p_sizeOfImage = specFieldsHeader.p_sizeOfImage;

    // print pe headers and optional header
    std::cout << "\nPeHeaderOffset: " << intToHex(PeHeaderOffset) << '\n';
    peHeader.print(); std::cout << '\n';
    stdFieldsHeader.print(sectionHeaders); std::cout << '\n';
    specFieldsHeader.print(); std::cout << '\n';
    dataDirs.print(sectionHeaders);
    // print sections
    for (size_t i = 0; i < p_numberOfSections; i++) {
        std::cout << "\nSection " << sectionHeaders[i].getName() << ":\n    ";
        sectionHeaders[i].print("\n    ");
        printBytes(sectionData[i],sectionHeaders[i].s_rawDataPointer);
    }
    if (isDll) {
        // print export header
        std::cout << "\n\nexport header:\n";
        exportDirTable.print();
        std::cout << "\ndll name: \"" << dllName << "\"\n\n";
        //std::cout << "export names:\n";
        //for (size_t i = 0; i < exportNames.size(); i++) std::cout << "    #" << i << ": \"" << exportNames[i] << "\"\n";
        std::cout << "exports:\n";
        uint32_t longestNameLen = 0;
        for (size_t i = 0; i < exports.size(); i++) {
            if (exports[i].name.size()>longestNameLen) longestNameLen=exports[i].name.size();
        }
        for (size_t i = 0; i < exports.size(); i++) {
            std::cout << "    #" << i << ": { ";
            std::cout << "namePtrIndex: " << exports[i].namePtrIndex << ", ";
            std::cout << "name: \"" << exports[i].name << "\", " << std::string(longestNameLen-exports[i].name.size(),' ');
            std::cout << "dllName: \"" << exports[i].dllName << "\" ";
            std::cout << "},\n";
        }
    }

    inFile.close();
    return 0;
}
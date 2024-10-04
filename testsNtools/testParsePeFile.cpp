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
    int32_t actualStartAddress = startAddress+startOffset;
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
        const uint32_t lastRowStartOffset = startOffset+lastRowStart;
        const uint32_t lastRowStartAddress = startAddress+lastRowStart;
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
    if (fullPath.size()==0) { std::cout << "File not found.\n"; return 1; }
    std::cout << "reading: \"" << fullPath << "\"\n";
    std::ifstream inFile(fullPath, std::ios::binary);
    if (inFile.bad()) { std::cout << "File ifStream bad.\n"; return 1; }
    uint32_t count = 0;

    peHdr peHeader;
    peOptHdrStdFields stdFieldsHeader;
    peOptHdrSpecFields64 specFieldsHeader;
    peOptHdrDataDirs dataDirs;
    uint16_t p_numberOfSections = 0;
    uint32_t PeHeaderOffset = 0;
    std::vector<peSectionHdr> sectionHeaders;
    std::vector<std::vector<uint8_t>> sectionData;

    bool isDll = false;
    peSectionPlusOffset exportTableLocation;
    peExportDirTable exportDirTable(0,0);
    std::string dllName;
    std::vector<std::string> exportNames;
    std::vector<dllExportData> exports;
    try {
        // read pe header and optional header
        peHdr::peHdrPullReturnData peHdrPullReturn = peHeader.read(inFile,count);
        if (!peHdrPullReturn.isValid) { inFile.close(); return 1; }
        PeHeaderOffset = peHdrPullReturn.PeHeaderOffset;
        bool stdFieldsHdrPullReturn = stdFieldsHeader.read(inFile,count);
        if (!stdFieldsHdrPullReturn) { inFile.close(); return 1; }
        bool specFieldsHdrPullReturn = specFieldsHeader.read(inFile,count);
        if (!specFieldsHdrPullReturn) { inFile.close(); return 1; }
        bool dataDirsPullReturn = dataDirs.read(inFile,count);
        if (!dataDirsPullReturn) { inFile.close(); return 1; }
        // read sectopm headers
        p_numberOfSections = peHeader.p_numberOfSections;
        for (size_t i = 0; i < p_numberOfSections; i++) {
            peSectionHdr sectionHdrI("        ");
            if (!sectionHdrI.read(inFile,count)) { std::cout << "error reading section header #" << i << "\n"; inFile.close(); return 1; }
            sectionHeaders.push_back(sectionHdrI);
        }
        // read section data
        for (size_t i = 0; i < p_numberOfSections; i++) {
            trashBytes(inFile,count,sectionHeaders[i].s_rawDataPointer-count);
            sectionData.push_back(readBytes(inFile, count, sectionHeaders[i].s_virtualSize));
        }
        isDll=((peHeader.p_characteristics&IMAGE_FILE_CHARACTERISTIC_DLL)!=0);
        if (isDll) {
            // find export table
            if (dataDirs.p_exportTableRVA==0) { std::cout << "DLL has no export table."; inFile.close(); return 1; }
            exportTableLocation = RVAtoSectionPlusOffset(dataDirs.p_exportTableRVA,dataDirs.p_exportTableSize,sectionHeaders);
            if (exportTableLocation.section==nullptr) { std::cout << "DLL export table could not be found."; inFile.close(); return 1; }
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
    } catch(const int& err) { std::cout << "EOF reached before expected"; inFile.close(); return 1; }
    catch ( ... ) { std::cout << "error..."; inFile.close(); return 1; }

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
        std::cout << "\nSection #" << (i+1) << ":\n    ";
        sectionHeaders[i].print("\n    ");
        printBytes(sectionData[i],sectionHeaders[i].s_rawDataPointer,sectionHeaders[i].s_virtualAddress,32);
    }
    if (isDll) {
        // print export header
        std::cout << "\n\nexport header:\n";
        exportDirTable.print();
        std::cout << "\ndll name: \"" << dllName << "\"\n";
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
#include <string.h>
#include <iostream>
#include <string>
#include <algorithm>// for std::min and std::max
using std::string,std::cout,std::endl;

//utility func
uint32_t roundToAlign(const uint32_t &value, const uint32_t &align) {
    unsigned int mod = value%align;
    if (mod==0) return value;
    return value+align-mod;
}

#include "globalConstants.h"
#include "intelConstants.h"

uint8_t bitMode = 0;

#pragma region helper functions
template <>
void pushChars(std::ostream& reciever, const uint8_t* chars, const size_t& len, const bool& LSB) {
    reciever << std::hex << std::setfill('0') << std::setw(2);
    if (LSB)
        for (short i = 0; i < len; i++) reciever << (int)chars[i] << " ";
    else
        for (short i = len - 1; i >= 0; i--) reciever << (int)chars[i] << " ";
    reciever << std::dec << std::setfill(' ');
}
template <>
void pushChars(std::ofstream& reciever, const uint8_t* chars, const size_t& len, const bool& LSB) {
    if (LSB)
        for (short i = 0; i < len; i++) reciever << chars[i];
    else
        for (short i = len - 1; i >= 0; i--) reciever << chars[i];
}
template <>
void pushChars(std::vector<uint8_t>& reciever, const uint8_t* chars, const size_t& len, const bool& LSB) {
    if (LSB)
        for (short i = 0; i < len; i++) reciever.push_back(chars[i]);
    else
        for (short i = len - 1; i >= 0; i--) reciever.push_back(chars[i]);
}

template <typename T>
void pushChars(T& reciever, const std::vector<uint8_t>& vector) {
    for (size_t i = 0; i < vector.size(); i++) pushChars(reciever,&vector[i],1,false);
}
template void pushChars(std::ostream& stream, const std::vector<uint8_t>& vector);
template void pushChars(std::ofstream& file, const std::vector<uint8_t>& vector);
template void pushChars(std::vector<uint8_t>& vector1, const std::vector<uint8_t>& vector2);
template void pushChars(Elf64SegmentHandler*& segment, const std::vector<uint8_t>& vector);
template void pushChars(Pe64SectionHandler*& section, const std::vector<uint8_t>& vector);

void setCharsAt(std::vector<uint8_t>& vector, const size_t& index, const uint8_t* chars, const size_t& len, const bool& LSB) {
    if (LSB)
        for (short i = 0; i < len; i++) vector[index+i]=chars[i];
    else
        for (short i = len - 1; i >= 0; i--) vector[index+i]=chars[i];
}

template <typename T>
void pushByte(T& reciever, const uint8_t& byte) {
    pushChars(reciever,&byte, 1, false);
}
template <typename T>
void pushWord(T& reciever, const uint16_t& word, const bool& LSB) {
    pushChars(reciever, static_cast<const uint8_t* >(static_cast<const void *>(&word)), 2, LSB);
}
template <typename T>
void pushDword(T& reciever, const uint32_t& dword, const bool& LSB) {
    pushChars(reciever, static_cast<const uint8_t* >(static_cast<const void *>(&dword)), 4, LSB);
}
template <typename T>
void pushQword(T& reciever, const uint64_t& qword, const bool& LSB) {
    pushChars(reciever, static_cast<const uint8_t* >(static_cast<const void *>(&qword)), 8, LSB);
}
template void pushByte (std::ostream& stream, const uint8_t& byte);
template void pushByte (std::ofstream& file, const uint8_t& byte);
template void pushByte (std::vector<uint8_t>& vector, const uint8_t& byte);
template void pushByte (Elf64SegmentHandler*& vector, const uint8_t& byte);
template void pushByte (Pe64SectionHandler*& section, const uint8_t& byte);
template void pushWord (std::ostream& stream, const uint16_t& word, const bool& LSB);
template void pushWord (std::ofstream& file, const uint16_t& word, const bool& LSB);
template void pushWord (std::vector<uint8_t>& vector, const uint16_t& word, const bool& LSB);
template void pushWord (Elf64SegmentHandler*& vector, const uint16_t& word, const bool& LSB);
template void pushWord (Pe64SectionHandler*& section, const uint16_t& word, const bool& LSB);
template void pushDword(std::ostream& stream, const uint32_t& dword, const bool& LSB);
template void pushDword(std::ofstream& file, const uint32_t& dword, const bool& LSB);
template void pushDword(std::vector<uint8_t>& vector, const uint32_t& dword, const bool& LSB);
template void pushDword(Elf64SegmentHandler*& vector, const uint32_t& dword, const bool& LSB);
template void pushDword(Pe64SectionHandler*& section, const uint32_t& dword, const bool& LSB);
template void pushQword(std::ostream& stream, const uint64_t& qword, const bool& LSB);
template void pushQword(std::ofstream& file, const uint64_t& qword, const bool& LSB);
template void pushQword(std::vector<uint8_t>& vector, const uint64_t& qword, const bool& LSB);
template void pushQword(Elf64SegmentHandler*& vector, const uint64_t& qword, const bool& LSB);
template void pushQword(Pe64SectionHandler*& section, const uint64_t& qword, const bool& LSB);
void setByteAt(std::vector<uint8_t>& vector, const size_t& index, const uint8_t& byte) {
    setCharsAt(vector, index,& byte, 1, false);
}
void setWordAt(std::vector<uint8_t>& vector, const size_t& index, const uint16_t& word, const bool& LSB) {
    setCharsAt(vector, index, static_cast<const uint8_t* >(static_cast<const void *>(&word)), 2, LSB);
}
void setDwordAt(std::vector<uint8_t>& vector, const size_t& index, const uint32_t& dword, const bool& LSB) {
    setCharsAt(vector, index, static_cast<const uint8_t* >(static_cast<const void *>(&dword)), 4, LSB);
}
void setQwordAt(std::vector<uint8_t>& vector, const size_t& index, const uint64_t& qword, const bool& LSB) {
    setCharsAt(vector, index, static_cast<const uint8_t* >(static_cast<const void *>(&qword)), 8, LSB);
}

void padBytes(std::ofstream& stream, const size_t& numBytes) {
    uint8_t C8[5]{0x00,0x00,0x00,0x00,0x00};
    for (int i = 0; i < numBytes; i++) {
        stream << C8[i%5];
    }
}
void padBytes(std::vector<uint8_t>& vector, const size_t& numBytes) {
    uint8_t C8[5]{0x00,0x00,0x00,0x00,0x00};
    for (int i = 0; i < numBytes; i++) {
        vector.push_back(C8[i%5]);
    }
}

char toLower(char chr) {
    return ((chr>='A')&&(chr<='Z'))?(chr-'A'+'a'):chr;
}
char toUpper(char chr) {
    return ((chr>='a')&&(chr<='z'))?(chr-'a'+'A'):chr;
}
template<typename T>
std::string int_to_hex(T i) {
    std::stringstream stream;
    stream << "0x" << std::hex << i;
    return stream.str();
}
#pragma endregion helper functions

#pragma region parsers
struct registerInfo {
    string properString="err";
    int8_t offset = -1;
    uint8_t len = 0;
    uint8_t bit = 0;
};
// return offset of register, or -2 for eIP or rIP, or -1 for invalid
registerInfo parseRegister(const string& str) {
    char tmp1 = toLower(str[0]);
    if (str.size()<=1) return {"",-1,0u,0u};// reached end of string
    char tmp2 = toLower(str[1]);
    if (tmp1=='e' || tmp1=='r') { // most likely 3 characters
        if (str[2]=='\0') return {"",-1,0u,0u};// reached end of string
        char tmp3 = toLower(str[2]);
        if (tmp3=='x') {
            if (tmp2=='a')      return {string(1,tmp1)+toUpper(tmp2)+toUpper(tmp3),0,3,(uint8_t)((tmp1=='r')?64u:32u)}; // eAX or rAX
            else if (tmp2=='c') return {string(1,tmp1)+toUpper(tmp2)+toUpper(tmp3),1,3u,(uint8_t)((tmp1=='r')?64u:32u)}; // eCX or rCX
            else if (tmp2=='d') return {string(1,tmp1)+toUpper(tmp2)+toUpper(tmp3),2,3u,(uint8_t)((tmp1=='r')?64u:32u)}; // eDX or rDX
            else if (tmp2=='b') return {string(1,tmp1)+toUpper(tmp2)+toUpper(tmp3),3,3u,(uint8_t)((tmp1=='r')?64u:32u)}; // eBX or rBX
            else return {"",-1,0u,0u};
        } else if (tmp3=='p') {
            if (tmp2=='s')      return {string(1,tmp1)+toUpper(tmp2)+toUpper(tmp3),4,3u,(uint8_t)((tmp1=='r')?64u:32u)}; // eSP or rSP
            else if (tmp2=='b') return {string(1,tmp1)+toUpper(tmp2)+toUpper(tmp3),5,3u,(uint8_t)((tmp1=='r')?64u:32u)}; // rBP or rBP
            else if (tmp2=='i') return {string(1,tmp1)+toUpper(tmp2)+toUpper(tmp3),-2,3u,(uint8_t)((tmp1=='r')?64u:32u)}; // eIP or rIP
            else return {"",-1,0u,0u};
        } else if (tmp3=='i') {
            if (tmp2=='s')      return {string(1,tmp1)+toUpper(tmp2)+toUpper(tmp3),6,3u,(uint8_t)((tmp1=='r')?64u:32u)}; // eSI or rSI
            else if (tmp2=='d') return {string(1,tmp1)+toUpper(tmp2)+toUpper(tmp3),7,3u,(uint8_t)((tmp1=='r')?64u:32u)}; // eDI or rDI
            else return {"",-1,0u,0u};
        } else return {"",-1,0u,0u};
    } else if ((tmp1>='a')&&(tmp1<='d')) { // this or the conditions below, means it is most likely 2 characters
        int8_t tmpOffset = -1;
        if (tmp1=='a') tmpOffset = 0; // AL, AX
        else if (tmp1=='c') tmpOffset = 1; // CL, CX
        else if (tmp1=='d') {
            if (tmp2=='i') return {string(1,toUpper(tmp1))+toUpper(tmp2),7,2u,16u}; // DI
            else tmpOffset = 2; // DL, DX
        } else if (tmp1=='b')  {
            if (tmp2=='p') return {string(1,toUpper(tmp1))+toUpper(tmp2),5,2u,16u}; // BP
            else tmpOffset = 3; // BL, BX
        }
        else return {"",-1,0u,0u};
        if (tmp2=='h') return {string(1,toUpper(tmp1))+toUpper(tmp2),static_cast<int8_t>(tmpOffset+4),2u,8u}; // if it is AH, CH, DH, or BH add 4 to the value of AL, CL, DL, and BL respectively
        if (tmp2!='l' && tmp2!='x') return {"",-1,0u,0u};
        return {string(1,toUpper(tmp1))+toUpper(tmp2),tmpOffset,2,(uint8_t)((tmp2=='l')?8u:16u)};
    } else if (tmp2=='p') {
        if (tmp1=='s') return {string(1,toUpper(tmp1))+toUpper(tmp2),4,2u,16u}; // SP
        //if (tmp1=='b') return {5,2u,16u,string(1,toUpper(tmp1))+toUpper(tmp2)}; // BP
        return {"",-1,0u,0u};
    } else if (tmp2=='i') {
        if (tmp1=='s') return {string(1,toUpper(tmp1))+toUpper(tmp2),6,2u,16u}; // SI
        //if (tmp1=='d') return {7,2u,16u,string(1,toUpper(tmp1))+toUpper(tmp2)}; // DI
        return {"",-1,0u,0u};
    }
    return {"",-1,0u,0u};
}
struct numberInfo {
    bool isError = false;
    int val = 0;
    int len = 0;
};
numberInfo parseNum(string str) {
    numberInfo info;
    int strLen = str.size();
    size_t index = 0;
    bool isNegative = str[index]=='-';
    unsigned int numBase = 10;
    std::string numStr = "";
    if (isNegative) { numStr+='-'; info.len++; index++; }
    if (str[index]=='0'&&(str[index+1]=='b'||str[index+1]=='d'||str[index+1]=='x')) {// for numbers in the form of 0xb0001101, 0d25, or 0x19
        if (str[index+1]=='b') numBase=2;// get number base based on the number abbreviation
        else if (str[index+1]=='x') numBase=16;
        // add 2 to length for the 0b/0d/0x
        info.len+=2;index+=2;
    }
    // loop through string until it reaches 32 characters, or finds an ending character
    bool foundEnd = false;
    for (unsigned int i = index; i < std::min(static_cast<unsigned int>(32u+index),static_cast<unsigned int>(strLen)); i++) {
        const char chr = std::tolower(str[i]);
        if (chr==']'||chr=='+') { foundEnd=true; break; }
        else if ((chr>='0' && chr<='9') || (chr>='a' && chr<='z') || (chr>='A' && chr<='Z')) {
            info.len++; numStr+=chr;
        } else if (chr=='_'||chr==',') { info.len++; continue; }// ignore underscores and commas
        else break;// if there is a character that is not a number
        if (i==strLen-1) { foundEnd=true; break; }
    }
    if (!foundEnd) { info.isError=true; return info; }
    // parse number from the string
    char* p;
    info.val=strtol(numStr.c_str(),&p,numBase);// the character p represents the first character that "strtol" finds that is not valid, or '\0' if it did not find an invalid character
    if (*p!=0) { info.isError=true; return info; }
    return info;
}

struct argumentType {
    string properString = "";
    bool isError = false;
    uint8_t errorCode = 0;
    uint8_t bit = 0;
    bool isJustRegister = false;
    uint8_t registerOffset = 0;
    uint8_t registerRegOp = 0;
    bool isJustNumber = false;
    uint8_t NumValNumBytes = 0;// used for just number arguments and address offsets
    int numValOneByte = 0;
    int numValFourBytes = 0;
    bool isIndirect = false;
    uint8_t ModRMByte = 0;
    bool requiresSIB = false;
    uint8_t SibByte = 0;
    bool isVar = false;
    std::string varName = "";
};
argumentType parseArgument(const string& str) {
    argumentType returnVal;
    int tmp1 = toLower(str[0]);
    if (tmp1=='[') { // argument is indirect
        returnVal.isIndirect=true;
        bool nextNeedsToBeNumber = false;
        registerInfo reg1 = {"",-1,0u,0u};
        registerInfo reg2 = {"",-1,0u,0u};
        int8_t reg2Scale = 1;
        bool hasEip = false;
        bool hasNumber = false;
        int numValue = 0;
        int index = 1;
        while (true) {
            int tmpI = toLower(str[index]);
            if (((tmpI>='0')&&(tmpI<='9')) || (tmpI=='-')) {
                if (hasNumber) { returnVal.isError=true; returnVal.errorCode=1; break; }
                numberInfo numInfo = parseNum(str.substr(index));
                if (numInfo.isError) { returnVal.isError=true; returnVal.errorCode=2; return returnVal; }
                hasNumber=true;
                numValue+=numInfo.val;
                index+=numInfo.len;
            } else if (!nextNeedsToBeNumber&&((tmpI=='e') || (tmpI=='r') || (tmpI>='a' && tmpI<='d') || tmpI=='s')) {// starts with 'e', 'r', 'a', 'c', 'd', 'b', 's'
                registerInfo regInfo = parseRegister(str.substr(index));
                if (regInfo.offset==-1) { returnVal.isError=true; returnVal.errorCode=3; return returnVal; }
                if (regInfo.bit>bitMode) { returnVal.isError=true; returnVal.errorCode=4; return returnVal; }
                if (regInfo.bit<32) { returnVal.isError=true; returnVal.errorCode=5; return returnVal; }// 8 bit or 32 bit are not valid for indirect mode
                if ((returnVal.bit!=0)&&(returnVal.bit!=regInfo.bit)) { returnVal.isError=true; returnVal.errorCode=6; return returnVal; }
                returnVal.bit=regInfo.bit;
                index+=regInfo.len;
                uint8_t scale = 1;
                if (regInfo.offset==-2) {
                    if ((scale!=1)||hasEip||(reg1.offset!=-1)||(reg2.offset!=-1)) { returnVal.isError=true; returnVal.errorCode=7; return returnVal; }// return if it already has Eip, or has any other register
                    hasEip=true;
                } else {
                    if (hasEip) { returnVal.isError=true; returnVal.errorCode=8; break; }
                    if (str[index]=='*') {
                        scale = (uint8_t)(str[index+1]-'0');
                        if ((scale!=8)&&(scale!=4)&&(scale!=2)&&(scale!=1)) { returnVal.isError=true; returnVal.errorCode=9; return returnVal; }// invalid scale
                        index+=2;
                    }
                    if (scale==1) {
                        if (reg1.offset==-1) {
                            // currently has no reg1, so set it to the new reg
                            reg1=regInfo;
                        } else if (reg1.offset==regInfo.offset) {
                            if (reg2.offset==-1) {
                                // combine new reg + reg1 into reg1*2
                                reg2=reg1;
                                reg2Scale=2;
                                reg1.offset=-1;
                            } else if (reg2Scale==1) {
                                // combine new reg + reg1 + reg2 into reg2 + reg1*2
                                reg1=reg2;
                                reg2=regInfo;
                                reg2Scale=2;
                            } else if ((reg2.offset==regInfo.offset)&&(reg2Scale==2)) {
                                // combine new reg + reg1 + reg2*2 into reg1*4
                                reg2Scale=4;
                                reg1.offset=-1;
                            } else { returnVal.isError=true; returnVal.errorCode=10; return returnVal; }
                        } else if ((reg2.offset==regInfo.offset)&&(reg2Scale==1)) {
                            // if the new reg matches reg2*1 add them together
                            reg2Scale=2;
                        } else if ((reg1.offset==reg2.offset)&&(reg2Scale==1)) {
                            // combine reg1 and reg2 into reg2*2 and set reg1 to new reg
                            reg1=regInfo;
                            reg2Scale=2;
                        } else { returnVal.isError=true; returnVal.errorCode=11; return returnVal; }
                    } else {// scale is not 1
                        if (reg2.offset==-1) {
                            // reg2 is empty, set reg2*x to the new reg*x
                            reg2=regInfo;
                            reg2Scale=scale;
                        } else if ((reg2.offset==regInfo.offset)&&(reg2Scale==scale)&&(scale<=4)) {
                            // combine (new reg)*x + reg2*x into reg2*2x
                            reg2Scale*=2;
                        } else if ((reg1.offset==-1)&&(reg2Scale==1)) {
                            // move reg2*1 to reg1 and (new reg)*x to reg2*x
                            reg1=reg2;
                            reg2=regInfo;
                            reg2Scale=scale;
                        } else if ((reg1.offset==regInfo.offset)&&(reg2.offset==regInfo.offset)&&(reg2Scale==1)&&(scale==2)) {
                            // combine new reg + reg1 + reg2 into reg2*4
                            reg1.offset=-1;
                            reg2Scale=4;
                        } else { returnVal.isError=true; returnVal.errorCode=12; return returnVal; }
                    }
                }
            }
            if (str[index]=='+') { index++; continue; }
            else if (str[index]=='-') { nextNeedsToBeNumber=true; continue; }
            else break;
        }
        if (hasNumber&&(numValue==0)&&((reg1.offset!=-1)||(reg2.offset!=-1))) hasNumber=false;
        returnVal.properString = "[";
        if (hasEip) returnVal.properString += (returnVal.bit==64)?"rIP":"eIP";
        if (reg1.offset!=-1) returnVal.properString += reg1.properString;
        if ((reg1.offset!=-1)&&(reg2.offset!=-1)) returnVal.properString += '+';
        if (reg2.offset!=-1) returnVal.properString += reg2.properString+'*'+std::to_string(reg2Scale);
        if ((hasEip||(reg1.offset!=-1)||(reg2.offset!=-1))&&hasNumber&&(numValue>=0)) returnVal.properString += '+';
        if (hasNumber) returnVal.properString += std::string(((numValue<0)?"-":""))+int_to_hex(abs(numValue));
        returnVal.properString += ']';
        if (hasEip) {
            returnVal.ModRMByte = INTEL_ModRM_MOD_Address|INTEL_ModRM_RM_DisplaceOnly;
            returnVal.NumValNumBytes=4;
            if (!hasNumber) { returnVal.numValOneByte=0; returnVal.numValFourBytes=0; }
            else if ((numValue>=2147483648)||(numValue<(-2147483648))) {
                returnVal.isError=true; returnVal.errorCode=13; return returnVal;
            } else if (numValue>=0) {
                returnVal.numValOneByte=returnVal.numValFourBytes=numValue;
            } else if (numValue<0) {
                returnVal.numValOneByte=256+numValue;
                returnVal.numValFourBytes=4294967296+numValue;
            }
        } else if ((bitMode==32)&&(reg1.offset==-1)&&(reg2.offset==-1)&&hasNumber) {
            returnVal.ModRMByte = INTEL_ModRM_MOD_Address|INTEL_ModRM_RM_DisplaceOnly;
            returnVal.NumValNumBytes=4;
            if (!hasNumber) { returnVal.numValOneByte=0; returnVal.numValFourBytes=0; }
            else if ((numValue>=2147483648)||(numValue<(-2147483648))) {
                returnVal.isError=true; returnVal.errorCode=16; return returnVal;
            } else if (numValue>=0) {
                returnVal.numValOneByte=returnVal.numValFourBytes=numValue;
            } else if (numValue<0) {
                returnVal.numValOneByte=256+numValue;
                returnVal.numValFourBytes=4294967296+numValue;
            }
        } else if ((reg2.offset!=-1)||(hasNumber&&(reg1.offset==-1))) {
            returnVal.requiresSIB=true;
            if (reg1.offset==INTEL_SIB_Base_None) { cout << "Register \"" << reg1.properString << "\" is an invalid address base" << endl; returnVal.isError=true; returnVal.errorCode=14; return returnVal; }
            if ((reg2.offset<<3)==INTEL_SIB_Index_None) { cout << "Register \"" << reg2.properString << "\" is an invalid address index" << endl; returnVal.isError=true; returnVal.errorCode=15; return returnVal; }
            if (hasNumber) returnVal.NumValNumBytes=(((numValue>=128)||(numValue<(-128)))?4:1);
            if ((reg1.offset==-1)) returnVal.NumValNumBytes=4;
            returnVal.ModRMByte = ((hasNumber && (reg1.offset!=-1))?((returnVal.NumValNumBytes==4)?INTEL_ModRM_MOD_4byteDisp:INTEL_ModRM_MOD_1byteDisp):INTEL_ModRM_MOD_Address)|INTEL_ModRM_RM_NoDisplace;
            const uint8_t scale = (reg2Scale==1)?INTEL_SIB_Scale_1:((reg2Scale==2)?INTEL_SIB_Scale_2:((reg2Scale==4)?INTEL_SIB_Scale_4:INTEL_SIB_Scale_8));
            const uint8_t index = reg2.offset<<3;
            const uint8_t Base = ((reg1.offset!=-1)?reg1.offset:INTEL_SIB_Base_None);
            returnVal.SibByte = ((reg2.offset!=-1)?(scale|index):(INTEL_SIB_Scale_None|INTEL_SIB_Index_None))|Base;
            if (!hasNumber) { returnVal.numValOneByte=0; returnVal.numValFourBytes=0; }
            else if ((numValue>=2147483648)||(numValue<(-2147483648))) {
                returnVal.isError=true; returnVal.errorCode=16; return returnVal;
            } else if (numValue>=0) {
                returnVal.numValOneByte=returnVal.numValFourBytes=numValue;
            } else if (numValue<0) {
                returnVal.numValOneByte=256+numValue;
                returnVal.numValFourBytes=4294967296+numValue;
            }
        } else {
            if (hasNumber) returnVal.NumValNumBytes=(((numValue>=128)||(numValue<(-128)))?4:1);
            returnVal.ModRMByte = (hasNumber?((returnVal.NumValNumBytes==4)?INTEL_ModRM_MOD_4byteDisp:INTEL_ModRM_MOD_1byteDisp):INTEL_ModRM_MOD_Address)|reg1.offset;
            if ((numValue>=2147483648)||(numValue<(-2147483648))) {
                returnVal.isError=true; returnVal.errorCode=17; return returnVal;
            } else if (!hasNumber) { returnVal.numValOneByte=0; returnVal.numValFourBytes=0; }
            else if (numValue<0) {
                returnVal.numValOneByte=256+numValue;
                returnVal.numValFourBytes=4294967296+numValue;
            } else {
                returnVal.numValOneByte=returnVal.numValFourBytes=numValue;
            }
        }
        return returnVal;
    } else if (((tmp1>='0')&&(tmp1<='9')) || (tmp1=='-')) {
        numberInfo numInfo = parseNum(str);
        if (numInfo.isError) { returnVal.isError=true; returnVal.errorCode=18; return returnVal; }
        returnVal.isJustNumber=true;
        returnVal.NumValNumBytes=(((numInfo.val>=128)||(numInfo.val<(-128)))?4:1);
        if (numInfo.val<0) { returnVal.isError=true; returnVal.errorCode=19; return returnVal; }
        returnVal.numValOneByte=returnVal.numValFourBytes=numInfo.val;
        returnVal.properString=std::string(((numInfo.val<0)?"-":""))+int_to_hex(abs(numInfo.val));
        return returnVal;
    } else if (((tmp1>='a')&&(tmp1<='e'))||(tmp1=='s')||(tmp1=='r')) {// starts with 'e', 'r', 'a', 'c', 'd', 'b', 's'
        // could be a register or a label
        registerInfo regInfo = parseRegister(str);
        if (regInfo.offset>=0) {
            if (regInfo.bit>bitMode) { returnVal.isError=true; returnVal.errorCode=20; return returnVal; }
            returnVal.bit=regInfo.bit;
            returnVal.isJustRegister=true;
            returnVal.registerOffset=regInfo.offset;
            returnVal.registerRegOp=regInfo.offset<<3;
            returnVal.ModRMByte=INTEL_ModRM_MOD_Reg|regInfo.offset;
            returnVal.properString=regInfo.properString;
            return returnVal;
        } if (regInfo.offset==-2) { returnVal.isError=true; returnVal.errorCode=21; return returnVal; }// else will just continue to if statement below
    }
    if ((tmp1>='a' && tmp1<='z')||(tmp1>='A' && tmp1<='Z')) {// starts with 'a'-'z' or 'A'-'Z'
        // is a label
        // check that there is not spaces or special characters other than '_' and '-'
        for (size_t i = 0; i < str.size(); i++) {
            char chr = str[i];
            if (((chr<'A')||(chr>'z'))&&(chr!='_')&&(chr!='-')) { returnVal.isError=true; returnVal.errorCode=22; return returnVal; }
        }
        returnVal.bit=bitMode;
        returnVal.isIndirect=true;
        returnVal.ModRMByte = INTEL_ModRM_MOD_Address|INTEL_ModRM_RM_DisplaceOnly;
        returnVal.NumValNumBytes=4;
        returnVal.numValOneByte=returnVal.numValFourBytes=0;
        returnVal.isVar=true;
        returnVal.varName=str;
        returnVal.properString=str;
        return returnVal;
    } else { returnVal.isError=true; returnVal.errorCode=24; return returnVal; } // is definitely invalid
}
#pragma endregion parsers

bool debugInstructionOutput = false;

#pragma region resolveVar
template <>
void resolveVar<std::ofstream>(std::ofstream& reciever, const std::string& varName){}
template <>
void resolveVar<std::vector<uint8_t>>(std::vector<uint8_t>& reciever, const std::string& varName){}
template <>
void resolveVar<Elf64SegmentHandler*>(Elf64SegmentHandler*& reciever, const std::string& varName){
    reciever->resolveLabel(varName,4);
}
template <>
void resolveVar<Pe64SectionHandler*>(Pe64SectionHandler*& reciever, const std::string& varName){
    reciever->resolveLabel(varName,4);
}
#pragma endregion resolveVar

#pragma region AddOrAdcSbbAndSubXorCmp
template <typename T>
void AddOrAdcSbbAndSubXorCmp(T& reciever, const char* dst, const char* src, const char* instructionName, const uint8_t& op1Code=INTEL_ModRM_OP1_ADD_RM_I, const uint8_t& RMv_REGv=INTEL_INSTR_ADD_RMv_REGv, const uint8_t& REGv_RMv=INTEL_INSTR_ADD_REGv_RMv, const uint8_t& eAX_Iv=INTEL_INSTR_ADD_eAX_Iv) {
    if (bitMode==0) return;
    argumentType dstInfo = parseArgument(dst);
    if(dstInfo.isError) { cout << "dst: \"" << dst << "\": error" << endl; return; }
    argumentType srcInfo = parseArgument(src);
    if(srcInfo.isError) { cout << "src: \"" << src << "\": error" << endl; return; }

    if (dstInfo.isIndirect && srcInfo.isIndirect) { cout << "dst and src cannot both be indirect." << endl; return; }
    if (dstInfo.isJustNumber) { cout << "dst cannot be just a number." << endl; return; }
    if ((dstInfo.bit!=0 && dstInfo.bit<32) || (srcInfo.bit!=0 && srcInfo.bit<32)) { cout << "invalid bit" << endl; return; }// dont have handling for these yet

    if (!srcInfo.isIndirect) {
        // Either specify the register that is the src, or the instruction code for the OP1 general instruction
        // This is done up here because it is used in all 4 out of 5 of the cases below and is the exact same value for each
        if (dstInfo.isJustRegister && (dstInfo.registerOffset==INTEL_REG_OFF_eAX) && srcInfo.isJustNumber && srcInfo.NumValNumBytes==4) {
            // ex: instr eax, num
            if (dstInfo.bit==64) pushByte(reciever, INTEL_INSTR64_OperandSz_OVRD);
            pushByte(reciever, eAX_Iv);
            pushDword(reciever, srcInfo.numValFourBytes, true);
            if (debugInstructionOutput) cout << instructionName << " " << dstInfo.properString << ", " << srcInfo.properString << endl;
        } else {
            // ex: instr (reg or [eip/rip+num] or [reg+num] or [base+index*scale+num]), (reg or num)
            if (!dstInfo.isIndirect&&!srcInfo.isJustNumber&&(dstInfo.bit!=srcInfo.bit)) { cout << "Operand size mis-match." << endl; return; }
            if (dstInfo.isIndirect&&(bitMode==64)&&(dstInfo.bit==32)) pushByte(reciever, INTEL_INSTR_AddrSz_OVRD);
            if ((srcInfo.isJustRegister&&(srcInfo.bit==64))||(dstInfo.isJustRegister&&(dstInfo.bit==64))) pushByte(reciever, INTEL_INSTR64_OperandSz_OVRD);
            pushByte(reciever, srcInfo.isJustRegister?RMv_REGv:((srcInfo.isJustNumber&&(srcInfo.NumValNumBytes==4))?INTEL_INSTR_OP1_RMv_Iv:INTEL_INSTR_OP1_RMv_Ib));// Specify if src is a register or an immutable
            const uint8_t RegOp = srcInfo.isJustRegister?srcInfo.registerRegOp:op1Code;
            pushByte(reciever, dstInfo.ModRMByte|RegOp);
            if (dstInfo.requiresSIB) pushByte(reciever, dstInfo.SibByte);
            if (dstInfo.NumValNumBytes==4) pushDword(reciever, dstInfo.numValFourBytes, true);
            else if (dstInfo.NumValNumBytes==1) pushByte(reciever, dstInfo.numValOneByte);
            if (dstInfo.isVar) resolveVar(reciever,dstInfo.varName);
            if (srcInfo.isJustNumber) {
                if (srcInfo.NumValNumBytes==4) pushDword(reciever, srcInfo.numValFourBytes, true);
                else if (srcInfo.NumValNumBytes==1) pushByte(reciever, srcInfo.numValOneByte);
            }
            if (debugInstructionOutput) {
                if (dstInfo.isIndirect) cout << instructionName << " " << ((srcInfo.bit==64)?"QWORD":"DWORD") << " PTR " << dstInfo.properString << ", " << srcInfo.properString << endl;
                else cout << instructionName << " " << dstInfo.properString << ", " << srcInfo.properString << endl;
            }
        }
    } else {
        // ex: instr reg, ([eip/rip+num] or [reg+num] or [base+index*scale+num])
        if (bitMode==64 && srcInfo.bit==32) pushByte(reciever, INTEL_INSTR_AddrSz_OVRD);
        if (dstInfo.bit==64) pushByte(reciever, INTEL_INSTR64_OperandSz_OVRD);
        pushByte(reciever, REGv_RMv);
        pushByte(reciever, srcInfo.ModRMByte|dstInfo.registerRegOp);
        if (srcInfo.requiresSIB) pushByte(reciever, srcInfo.SibByte);
        if (srcInfo.NumValNumBytes==4) pushDword(reciever, srcInfo.numValFourBytes, true);
        else if (srcInfo.NumValNumBytes==1) pushByte(reciever, srcInfo.numValOneByte);
        if (srcInfo.isVar) resolveVar(reciever,srcInfo.varName);
        if (debugInstructionOutput) cout << instructionName << " " << dstInfo.properString << ", " << ((dstInfo.bit==64)?"QWORD":"DWORD") << " PTR " << srcInfo.properString << endl;
    }
}
template <typename T>
void ADD(T& reciever, const char* dst, const char* src) {
    AddOrAdcSbbAndSubXorCmp(reciever,dst,src,"ADD",INTEL_ModRM_OP1_ADD_RM_I,INTEL_INSTR_ADD_RMv_REGv,INTEL_INSTR_ADD_REGv_RMv,INTEL_INSTR_ADD_eAX_Iv);
}
template <typename T>
void OR(T& reciever, const char* dst, const char* src) {
    AddOrAdcSbbAndSubXorCmp(reciever,dst,src,"OR" ,INTEL_ModRM_OP1_OR_RM_I ,INTEL_INSTR_OR_RMv_REGv ,INTEL_INSTR_OR_REGv_RMv ,INTEL_INSTR_OR_eAX_Iv );
}
template <typename T>
void ADC(T& reciever, const char* dst, const char* src) {
    AddOrAdcSbbAndSubXorCmp(reciever,dst,src,"ADC",INTEL_ModRM_OP1_ADC_RM_I,INTEL_INSTR_ADC_RMv_REGv,INTEL_INSTR_ADC_REGv_RMv,INTEL_INSTR_ADC_eAX_Iv);
}
template <typename T>
void SBB(T& reciever, const char* dst, const char* src) {
    AddOrAdcSbbAndSubXorCmp(reciever,dst,src,"SBB",INTEL_ModRM_OP1_SBB_RM_I,INTEL_INSTR_SBB_RMv_REGv,INTEL_INSTR_SBB_REGv_RMv,INTEL_INSTR_SBB_eAX_Iv);
}
template <typename T>
void AND(T& reciever, const char* dst, const char* src) {
    AddOrAdcSbbAndSubXorCmp(reciever,dst,src,"AND",INTEL_ModRM_OP1_AND_RM_I,INTEL_INSTR_AND_RMv_REGv,INTEL_INSTR_AND_REGv_RMv,INTEL_INSTR_AND_eAX_Iv);
}
template <typename T>
void SUB(T& reciever, const char* dst, const char* src) {
    AddOrAdcSbbAndSubXorCmp(reciever,dst,src,"SUB",INTEL_ModRM_OP1_SUB_RM_I,INTEL_INSTR_SUB_RMv_REGv,INTEL_INSTR_SUB_REGv_RMv,INTEL_INSTR_SUB_eAX_Iv);
}
template <typename T>
void XOR(T& reciever, const char* dst, const char* src) {
    AddOrAdcSbbAndSubXorCmp(reciever,dst,src,"XOR",INTEL_ModRM_OP1_XOR_RM_I,INTEL_INSTR_XOR_RMv_REGv,INTEL_INSTR_XOR_REGv_RMv,INTEL_INSTR_XOR_eAX_Iv);
}
template <typename T>
void CMP(T& reciever, const char* dst, const char* src) {
    AddOrAdcSbbAndSubXorCmp(reciever,dst,src,"CMP",INTEL_ModRM_OP1_CMP_RM_I,INTEL_INSTR_CMP_RMv_REGv,INTEL_INSTR_CMP_REGv_RMv,INTEL_INSTR_CMP_eAX_Iv);
}
#pragma region AddOrAdcSbbAndSubXorCmp instatiations
template void ADD(std::ofstream& reciever, const char* dst, const char* src);
template void OR (std::ofstream& reciever, const char* dst, const char* src);
template void ADC(std::ofstream& reciever, const char* dst, const char* src);
template void SBB(std::ofstream& reciever, const char* dst, const char* src);
template void AND(std::ofstream& reciever, const char* dst, const char* src);
template void SUB(std::ofstream& reciever, const char* dst, const char* src);
template void XOR(std::ofstream& reciever, const char* dst, const char* src);
template void CMP(std::ofstream& reciever, const char* dst, const char* src);
template void ADD(std::vector<uint8_t>& reciever, const char* dst, const char* src);
template void OR (std::vector<uint8_t>& reciever, const char* dst, const char* src);
template void ADC(std::vector<uint8_t>& reciever, const char* dst, const char* src);
template void SBB(std::vector<uint8_t>& reciever, const char* dst, const char* src);
template void AND(std::vector<uint8_t>& reciever, const char* dst, const char* src);
template void SUB(std::vector<uint8_t>& reciever, const char* dst, const char* src);
template void XOR(std::vector<uint8_t>& reciever, const char* dst, const char* src);
template void CMP(std::vector<uint8_t>& reciever, const char* dst, const char* src);
template void ADD(Elf64SegmentHandler*& reciever, const char* dst, const char* src);
template void OR (Elf64SegmentHandler*& reciever, const char* dst, const char* src);
template void ADC(Elf64SegmentHandler*& reciever, const char* dst, const char* src);
template void SBB(Elf64SegmentHandler*& reciever, const char* dst, const char* src);
template void AND(Elf64SegmentHandler*& reciever, const char* dst, const char* src);
template void SUB(Elf64SegmentHandler*& reciever, const char* dst, const char* src);
template void XOR(Elf64SegmentHandler*& reciever, const char* dst, const char* src);
template void CMP(Elf64SegmentHandler*& reciever, const char* dst, const char* src);
template void ADD(Pe64SectionHandler*& reciever, const char* dst, const char* src);
template void OR (Pe64SectionHandler*& reciever, const char* dst, const char* src);
template void ADC(Pe64SectionHandler*& reciever, const char* dst, const char* src);
template void SBB(Pe64SectionHandler*& reciever, const char* dst, const char* src);
template void AND(Pe64SectionHandler*& reciever, const char* dst, const char* src);
template void SUB(Pe64SectionHandler*& reciever, const char* dst, const char* src);
template void XOR(Pe64SectionHandler*& reciever, const char* dst, const char* src);
template void CMP(Pe64SectionHandler*& reciever, const char* dst, const char* src);
#pragma endregion AddOrAdcSbbAndSubXorCmp instatiations
#pragma endregion AddOrAdcSbbAndSubXorCmp

#pragma region IncDec
template <typename T>
void IncDec(T& reciever, const char* arg, const char* instructionName, const uint8_t& op3Code=INTEL_ModRM_OP3_INC_RM, const uint8_t& offsetInstr=INTEL_INSTR32_INCpRv) {
    if (bitMode==0) return;
    argumentType argInfo = parseArgument(arg);
    if(argInfo.isError) { cout << "arg: \"" << arg << "\": error" << endl; return; }

    if (argInfo.isJustNumber) { cout << "arg cannot be just a number." << endl; return; }
    if ((argInfo.bit!=0 && argInfo.bit<32)) { cout << "invalid bit" << endl; return; }// dont have handling for these yet

    if ((bitMode==32)&&argInfo.isJustRegister) {
        // ex: instr (eax thru edi)
        pushByte(reciever, offsetInstr+argInfo.registerOffset);
        if (debugInstructionOutput) cout << instructionName << " " << argInfo.properString << endl;
    } else {
        // ex: instr (reg or [eip/rip+num] or [reg+num] or [base+index*scale+num])
        if (argInfo.isIndirect&&(bitMode==64)&&(argInfo.bit==32)) pushByte(reciever, INTEL_INSTR_AddrSz_OVRD);
        if ((argInfo.isJustRegister&&(argInfo.bit==64))||(argInfo.isIndirect&&(bitMode==64))) pushByte(reciever, INTEL_INSTR64_OperandSz_OVRD);
        pushByte(reciever, INTEL_INSTR_OP3v);
        pushByte(reciever, argInfo.ModRMByte|op3Code);
        if (argInfo.requiresSIB) pushByte(reciever, argInfo.SibByte);
        if (argInfo.NumValNumBytes==4) pushDword(reciever, argInfo.numValFourBytes, true);// implies that it is indirect, because if it is just a number the function would have returned above
        else if (argInfo.NumValNumBytes==1) pushByte(reciever, argInfo.numValOneByte);
        if (argInfo.isVar) resolveVar(reciever,argInfo.varName);
        if (debugInstructionOutput) {
            if (argInfo.isIndirect) cout << instructionName << " " << ((bitMode==64)?"QWORD":"DWORD") << " PTR " << argInfo.properString << endl;
            else cout << instructionName << " " << argInfo.properString << endl;
        }
    }
}
template <typename T>
void INC(T& reciever, const char* arg) {
    IncDec(reciever,arg,"INC",INTEL_ModRM_OP3_INC_RM,INTEL_INSTR32_INCpRv);
}
template <typename T>
void DEC(T& reciever, const char* arg) {
    IncDec(reciever,arg,"DEC",INTEL_ModRM_OP3_DEC_RM,INTEL_INSTR32_DECpRv);
}
template void INC(std::ofstream& reciever, const char* arg);
template void DEC(std::ofstream& reciever, const char* arg);
template void INC(std::vector<uint8_t>& reciever, const char* arg);
template void DEC(std::vector<uint8_t>& reciever, const char* arg);
template void INC(Elf64SegmentHandler*& reciever, const char* arg);
template void DEC(Elf64SegmentHandler*& reciever, const char* arg);
template void INC(Pe64SectionHandler*& reciever, const char* arg);
template void DEC(Pe64SectionHandler*& reciever, const char* arg);
#pragma endregion IncDec

#pragma region CallJmp
template <typename T>
void CallJmp(T& reciever, const char* arg, const char* instructionName, const uint8_t& op3Code=INTEL_ModRM_OP3_CALL_RM, const uint8_t& op3CodeFar=INTEL_ModRM_OP3_CALL_RM, const uint8_t& INSTR_Jv=INTEL_INSTR_CALL_Jv) {
    if (bitMode==0) return;
    argumentType argInfo = parseArgument(arg);
    if(argInfo.isError) { cout << "arg: \"" << arg << "\": error" << endl; return; }

    if (argInfo.isJustRegister) { cout << "arg cannot be just a register." << endl; return; }
    if ((argInfo.bit!=0 && argInfo.bit<32)) { cout << "invalid bit" << endl; return; }// dont have handling for these yet

    if (argInfo.isJustNumber) {
        // ex: call/jmp num
        cout << "Just a number as an argument is not currently supported for " << instructionName << "." << endl;// return;
        if (argInfo.bit==64) pushByte(reciever, INTEL_INSTR64_OperandSz_OVRD);
        pushByte(reciever, INSTR_Jv);
        pushDword(reciever, argInfo.numValFourBytes, true);
        if (debugInstructionOutput) cout << instructionName << " " << argInfo.properString << endl;
    } else {
        // ex: instr (reg or [eip/rip+num] or [reg+num] or [base+index*scale+num])
        if (argInfo.isIndirect&&(bitMode==64)&&(argInfo.bit==32)) pushByte(reciever, INTEL_INSTR_AddrSz_OVRD);
        if (argInfo.isJustRegister&&(argInfo.bit==64)) pushByte(reciever, INTEL_INSTR64_OperandSz_OVRD);
        pushByte(reciever, INTEL_INSTR_OP3v);
        pushByte(reciever, argInfo.ModRMByte|op3Code);
        if (argInfo.requiresSIB) pushByte(reciever, argInfo.SibByte);
        if (argInfo.NumValNumBytes==4) pushDword(reciever, argInfo.numValFourBytes, true);// implies that it is indirect, because if it is just a number the function would have returned above
        else if (argInfo.NumValNumBytes==1) pushByte(reciever, argInfo.numValOneByte);
        if (argInfo.isVar) resolveVar(reciever,argInfo.varName);
        if (debugInstructionOutput) {
            if (argInfo.isIndirect) cout << instructionName << " " << ((bitMode==64)?"QWORD":"DWORD") << " PTR " << argInfo.properString << endl;
            else cout << instructionName << " " << argInfo.properString << endl;
        }
    }
}
template <typename T>
void CALL(T& reciever, const char* arg) {
    CallJmp(reciever,arg,"CALL",INTEL_ModRM_OP3_CALL_RM,INTEL_ModRM_OP3_CALLF_m,INTEL_INSTR_CALL_Jv);
}
template <typename T>
void JMP(T& reciever, const char* arg) {
    CallJmp(reciever,arg,"JMP",INTEL_ModRM_OP3_JMP_RM,INTEL_ModRM_OP3_JMPF_m,INTEL_INSTR_JMP_Jv);
}
/*template <typename T>
void CALLF(T& reciever, const char* arg) {
    
}*/
/*template <typename T>
void JMPF(T& reciever, const char* arg) {
    
}*/
template void CALL(std::ofstream& reciever, const char* arg);
template void JMP(std::ofstream& reciever, const char* arg);
//template void CALLF(std::ofstream& reciever, const char* arg);
//template void JMPF(std::ofstream& reciever, const char* arg);
template void CALL(std::vector<uint8_t>& reciever, const char* arg);
template void JMP(std::vector<uint8_t>& reciever, const char* arg);
//template void CALLF(std::vector<uint8_t>& reciever, const char* arg);
//template void JMPF(std::vector<uint8_t>& reciever, const char* arg);
template void CALL(Elf64SegmentHandler*& reciever, const char* arg);
template void JMP(Elf64SegmentHandler*& reciever, const char* arg);
//template void CALLF(Elf64SegmentHandler& reciever, const char* arg);
//template void JMPF(Elf64SegmentHandler& reciever, const char* arg);
template void CALL(Pe64SectionHandler*& reciever, const char* arg);
template void JMP(Pe64SectionHandler*& reciever, const char* arg);
//template void CALLF(Pe64SectionHandler& reciever, const char* arg);
//template void JMPF(Pe64SectionHandler& reciever, const char* arg);
#pragma endregion CallJmp

#pragma region PushPop
template <typename T>
void PUSH(T& reciever, const char* arg) {
    if (bitMode==0) return;
    argumentType argInfo = parseArgument(arg);
    if(argInfo.isError) { cout << "arg: \"" << arg << "\": error" << endl; return; }

    if (!argInfo.isIndirect&&(argInfo.bit!=0)&&(argInfo.bit!=bitMode)) { cout << "PUSH arguments can only be " << (int)bitMode << " bit." << endl; return; }
    if ((argInfo.bit!=0)&&(argInfo.bit<32)) { cout << "invalid bit" << endl; return; }// dont have handling for these yet

    if (argInfo.isJustNumber) {
        // ex: PUSH num
        if (argInfo.NumValNumBytes==4) {
            pushByte(reciever, INTEL_INSTR_PUSH_Iv);
            pushDword(reciever, argInfo.numValFourBytes, true);
        } else if (argInfo.NumValNumBytes==1) {
            pushByte(reciever, INTEL_INSTR_PUSH_Ib);
            pushByte(reciever, argInfo.numValOneByte);
        }
        if (debugInstructionOutput) cout << "PUSH " << argInfo.properString << endl;
    } else if (argInfo.isJustRegister) {
        // ex: PUSH reg
        pushByte(reciever, INTEL_INSTR_PUSHpRv+argInfo.registerOffset);
        if (debugInstructionOutput) cout << "PUSH " << argInfo.properString << endl;
    } else {// argInfo.isIndirect
        // ex: PUSH (reg or [eip/rip+num] or [reg+num] or [base+index*scale+num])
        if (argInfo.isIndirect&&(bitMode==64)&&(argInfo.bit==32)) pushByte(reciever, INTEL_INSTR_AddrSz_OVRD);
        if (argInfo.isJustRegister&&(argInfo.bit==64)) pushByte(reciever, INTEL_INSTR64_OperandSz_OVRD);
        pushByte(reciever, INTEL_INSTR_OP3v);
        pushByte(reciever, argInfo.ModRMByte|INTEL_ModRM_OP3_PUSH_RM);
        if (argInfo.requiresSIB) pushByte(reciever, argInfo.SibByte);
        if (argInfo.NumValNumBytes==4) pushDword(reciever, argInfo.numValFourBytes, true);// implies that it is indirect, because if it is just a number the function would have returned above
        else if (argInfo.NumValNumBytes==1) pushByte(reciever, argInfo.numValOneByte);
        if (argInfo.isVar) resolveVar(reciever,argInfo.varName);
        if (debugInstructionOutput) {
            if (argInfo.isIndirect) cout << "PUSH " << ((bitMode==64)?"QWORD":"DWORD") << " PTR " << argInfo.properString << endl;
            else cout << "PUSH " << argInfo.properString << endl;
        }
    }
}
template <typename T>
void POP(T& reciever, const char* arg) {
    if (bitMode==0) return;
    argumentType argInfo = parseArgument(arg);
    if(argInfo.isError) { cout << "arg: \"" << arg << "\": error" << endl; return; }

    if (argInfo.isJustNumber) { cout << "arg cannot be just a number." << endl; return; }
    if (!argInfo.isIndirect&&(argInfo.bit!=bitMode)) { cout << "POP arguments can only be " << bitMode << " bit." << endl; return; }
    if ((argInfo.bit!=0)&&(argInfo.bit<32)) { cout << "invalid bit" << endl; return; }// dont have handling for these yet

    if (argInfo.isJustRegister) {
        // ex: POP reg
        pushByte(reciever, INTEL_INSTR_POPpRv+argInfo.registerOffset);
        if (debugInstructionOutput) cout << "POP " << argInfo.properString << endl;
    } else {// argInfo.isIndirect
        // ex: POP (reg or [eip/rip+num] or [reg+num] or [base+index*scale+num])
        if (argInfo.isIndirect&&(bitMode==64)&&(argInfo.bit==32)) pushByte(reciever, INTEL_INSTR_AddrSz_OVRD);
        if (argInfo.isJustRegister&&(argInfo.bit==64)) pushByte(reciever, INTEL_INSTR64_OperandSz_OVRD);
        pushByte(reciever, INTEL_INSTR_POP_RMv);
        pushByte(reciever, argInfo.ModRMByte);
        if (argInfo.requiresSIB) pushByte(reciever, argInfo.SibByte);
        if (argInfo.NumValNumBytes==4) pushDword(reciever, argInfo.numValFourBytes, true);// implies that it is indirect, because if it is just a number the function would have returned above
        else if (argInfo.NumValNumBytes==1) pushByte(reciever, argInfo.numValOneByte);
        if (argInfo.isVar) resolveVar(reciever,argInfo.varName);
        if (debugInstructionOutput) {
            if (argInfo.isIndirect) cout << "POP " << ((bitMode==64)?"QWORD":"DWORD") << " PTR " << argInfo.properString << endl;
            else cout << "POP " << argInfo.properString << endl;
        }
    }
}
template void PUSH(std::ofstream& reciever, const char* arg);
template void POP(std::ofstream& reciever, const char* arg);
template void PUSH(std::vector<uint8_t>& reciever, const char* arg);
template void POP(std::vector<uint8_t>& reciever, const char* arg);
template void PUSH(Elf64SegmentHandler*& reciever, const char* arg);
template void POP(Elf64SegmentHandler*& reciever, const char* arg);
template void PUSH(Pe64SectionHandler*& reciever, const char* arg);
template void POP(Pe64SectionHandler*& reciever, const char* arg);
#pragma endregion PushPop

#pragma region jump if instructions
/*
template <typename T>
void JOV(T& reciever, const char* arg) {
    if (bitMode==0) return;
    instructionArgInfo argInfo = processArg(arg);
    if(!argInfo.isValid) { cout << "arg: \"" << arg << "\": \"" << argInfo.reason << "\"" << endl; return; }
    
    if(argInfo.isIndirect) { cout << "arg cannot be indirect" << endl; return; }
    if(argInfo.hasReg1) { cout << "arg cannot be a register" << endl; return; }
    if (argInfo.bit!=0 && argInfo.bit<32) return;// Dont have handling for these yet
}
template <typename T>
void JNOV(T& reciever, const char* arg) {
    if (bitMode==0) return;
    instructionArgInfo argInfo = processArg(arg);
    if(!argInfo.isValid) { cout << "arg: \"" << arg << "\": \"" << argInfo.reason << "\"" << endl; return; }
    
    if(argInfo.isIndirect) { cout << "arg cannot be indirect" << endl; return; }
    if(argInfo.hasReg1) { cout << "arg cannot be a register" << endl; return; }
    if (argInfo.bit!=0 && argInfo.bit<32) return;// Dont have handling for these yet
}
template <typename T>
void JULT(T& reciever, const char* arg) {
    if (bitMode==0) return;
    instructionArgInfo argInfo = processArg(arg);
    if(!argInfo.isValid) { cout << "arg: \"" << arg << "\": \"" << argInfo.reason << "\"" << endl; return; }
    
    if(argInfo.isIndirect) { cout << "arg cannot be indirect" << endl; return; }
    if(argInfo.hasReg1) { cout << "arg cannot be a register" << endl; return; }
    if (argInfo.bit!=0 && argInfo.bit<32) return;// Dont have handling for these yet
}
template <typename T>
void JUGE(T& reciever, const char* arg) {
    if (bitMode==0) return;
    instructionArgInfo argInfo = processArg(arg);
    if(!argInfo.isValid) { cout << "arg: \"" << arg << "\": \"" << argInfo.reason << "\"" << endl; return; }
    
    if(argInfo.isIndirect) { cout << "arg cannot be indirect" << endl; return; }
    if(argInfo.hasReg1) { cout << "arg cannot be a register" << endl; return; }
    if (argInfo.bit!=0 && argInfo.bit<32) return;// Dont have handling for these yet
}
template <typename T>
void JET(T& reciever, const char* arg) {
    if (bitMode==0) return;
    instructionArgInfo argInfo = processArg(arg);
    if(!argInfo.isValid) { cout << "arg: \"" << arg << "\": \"" << argInfo.reason << "\"" << endl; return; }
    
    if(argInfo.isIndirect) { cout << "arg cannot be indirect" << endl; return; }
    if(argInfo.hasReg1) { cout << "arg cannot be a register" << endl; return; }
    if (argInfo.bit!=0 && argInfo.bit<32) return;// Dont have handling for these yet
}
template <typename T>
void JNE(T& reciever, const char* arg) {
    if (bitMode==0) return;
    instructionArgInfo argInfo = processArg(arg);
    if(!argInfo.isValid) { cout << "arg: \"" << arg << "\": \"" << argInfo.reason << "\"" << endl; return; }
    
    if(argInfo.isIndirect) { cout << "arg cannot be indirect" << endl; return; }
    if(argInfo.hasReg1) { cout << "arg cannot be a register" << endl; return; }
    if (argInfo.bit!=0 && argInfo.bit<32) return;// Dont have handling for these yet
}
template <typename T>
void JULE(T& reciever, const char* arg) {
    if (bitMode==0) return;
    instructionArgInfo argInfo = processArg(arg);
    if(!argInfo.isValid) { cout << "arg: \"" << arg << "\": \"" << argInfo.reason << "\"" << endl; return; }
    
    if(argInfo.isIndirect) { cout << "arg cannot be indirect" << endl; return; }
    if(argInfo.hasReg1) { cout << "arg cannot be a register" << endl; return; }
    if (argInfo.bit!=0 && argInfo.bit<32) return;// Dont have handling for these yet
}
template <typename T>
void JUGT(T& reciever, const char* arg) {
    if (bitMode==0) return;
    instructionArgInfo argInfo = processArg(arg);
    if(!argInfo.isValid) { cout << "arg: \"" << arg << "\": \"" << argInfo.reason << "\"" << endl; return; }
    
    if(argInfo.isIndirect) { cout << "arg cannot be indirect" << endl; return; }
    if(argInfo.hasReg1) { cout << "arg cannot be a register" << endl; return; }
    if (argInfo.bit!=0 && argInfo.bit<32) return;// Dont have handling for these yet
}
template <typename T>
void JS(T& reciever, const char* arg) {
    if (bitMode==0) return;
    instructionArgInfo argInfo = processArg(arg);
    if(!argInfo.isValid) { cout << "arg: \"" << arg << "\": \"" << argInfo.reason << "\"" << endl; return; }
    
    if(argInfo.isIndirect) { cout << "arg cannot be indirect" << endl; return; }
    if(argInfo.hasReg1) { cout << "arg cannot be a register" << endl; return; }
    if (argInfo.bit!=0 && argInfo.bit<32) return;// Dont have handling for these yet
}
template <typename T>
void JNS(T& reciever, const char* arg) {
    if (bitMode==0) return;
    instructionArgInfo argInfo = processArg(arg);
    if(!argInfo.isValid) { cout << "arg: \"" << arg << "\": \"" << argInfo.reason << "\"" << endl; return; }
    
    if(argInfo.isIndirect) { cout << "arg cannot be indirect" << endl; return; }
    if(argInfo.hasReg1) { cout << "arg cannot be a register" << endl; return; }
    if (argInfo.bit!=0 && argInfo.bit<32) return;// Dont have handling for these yet
}
template <typename T>
void JP(T& reciever, const char* arg) {
    if (bitMode==0) return;
    instructionArgInfo argInfo = processArg(arg);
    if(!argInfo.isValid) { cout << "arg: \"" << arg << "\": \"" << argInfo.reason << "\"" << endl; return; }
    
    if(argInfo.isIndirect) { cout << "arg cannot be indirect" << endl; return; }
    if(argInfo.hasReg1) { cout << "arg cannot be a register" << endl; return; }
    if (argInfo.bit!=0 && argInfo.bit<32) return;// Dont have handling for these yet
};
template <typename T>
void JNP(T& reciever, const char* arg) {
    if (bitMode==0) return;
    instructionArgInfo argInfo = processArg(arg);
    if(!argInfo.isValid) { cout << "arg: \"" << arg << "\": \"" << argInfo.reason << "\"" << endl; return; }
    
    if(argInfo.isIndirect) { cout << "arg cannot be indirect" << endl; return; }
    if(argInfo.hasReg1) { cout << "arg cannot be a register" << endl; return; }
    if (argInfo.bit!=0 && argInfo.bit<32) return;// Dont have handling for these yet
}
template <typename T>
void JLT(T& reciever, const char* arg) {
    if (bitMode==0) return;
    instructionArgInfo argInfo = processArg(arg);
    if(!argInfo.isValid) { cout << "arg: \"" << arg << "\": \"" << argInfo.reason << "\"" << endl; return; }
    
    if(argInfo.isIndirect) { cout << "arg cannot be indirect" << endl; return; }
    if(argInfo.hasReg1) { cout << "arg cannot be a register" << endl; return; }
    if (argInfo.bit!=0 && argInfo.bit<32) return;// Dont have handling for these yet
}
template <typename T>
void JGE(T& reciever, const char* arg) {
    if (bitMode==0) return;
    instructionArgInfo argInfo = processArg(arg);
    if(!argInfo.isValid) { cout << "arg: \"" << arg << "\": \"" << argInfo.reason << "\"" << endl; return; }
    
    if(argInfo.isIndirect) { cout << "arg cannot be indirect" << endl; return; }
    if(argInfo.hasReg1) { cout << "arg cannot be a register" << endl; return; }
    if (argInfo.bit!=0 && argInfo.bit<32) return;// Dont have handling for these yet
}
template <typename T>
void JLE(T& reciever, const char* arg) {
    if (bitMode==0) return;
    instructionArgInfo argInfo = processArg(arg);
    if(!argInfo.isValid) { cout << "arg: \"" << arg << "\": \"" << argInfo.reason << "\"" << endl; return; }
    
    if(argInfo.isIndirect) { cout << "arg cannot be indirect" << endl; return; }
    if(argInfo.hasReg1) { cout << "arg cannot be a register" << endl; return; }
    if (argInfo.bit!=0 && argInfo.bit<32) return;// Dont have handling for these yet
}
template <typename T>
void JGT(T& reciever, const char* arg) {
    if (bitMode==0) return;
    instructionArgInfo argInfo = processArg(arg);
    if(!argInfo.isValid) { cout << "arg: \"" << arg << "\": \"" << argInfo.reason << "\"" << endl; return; }
    
    if(argInfo.isIndirect) { cout << "arg cannot be indirect" << endl; return; }
    if(argInfo.hasReg1) { cout << "arg cannot be a register" << endl; return; }
    if (argInfo.bit!=0 && argInfo.bit<32) return;// Dont have handling for these yet
}
template <typename T>
void JCXZ(T& reciever, const char* arg) {
    if (bitMode==0) return;
    instructionArgInfo argInfo = processArg(arg);
    if(!argInfo.isValid) { cout << "arg: \"" << arg << "\": \"" << argInfo.reason << "\"" << endl; return; }
    
    if(argInfo.isIndirect) { cout << "arg cannot be indirect" << endl; return; }
    if(argInfo.hasReg1) { cout << "arg cannot be a register" << endl; return; }
    if (argInfo.bit!=0 && argInfo.bit<32) return;// Dont have handling for these yet
}
*/
#pragma endregion jump if instructions

#pragma region NOP
template <typename T>
void NOP(T& reciever) {
    if (bitMode==0) return;
    pushByte(reciever, INTEL_INSTR_NOP);
    if (debugInstructionOutput) cout << "NOP" << endl;
}
template <typename T>
void NOP(T& reciever, const char* arg) {
    if (bitMode==0) return;
    argumentType argInfo = parseArgument(arg);
    if(argInfo.isError) { cout << "arg: \"" << arg << "\": error" << endl; return; }
    
    if (argInfo.isJustNumber) { cout << "arg cannot be just a number." << endl; return; }
    if ((argInfo.bit!=0)&&(argInfo.bit<32)) { cout << "invalid bit" << endl; return; }// dont have handling for these yet

    // ex: POP (reg or [eip/rip+num] or [reg+num] or [base+index*scale+num])
    if (argInfo.isIndirect&&(bitMode==64)&&(argInfo.bit==32)) pushByte(reciever, INTEL_INSTR_AddrSz_OVRD);
    if ((argInfo.isJustRegister&&(argInfo.bit==64))||(argInfo.isIndirect&&(bitMode==64))) pushByte(reciever, INTEL_INSTR64_OperandSz_OVRD);
    pushWord(reciever, INTEL_INSTR_NOP_RMv,false);
    pushByte(reciever, argInfo.ModRMByte);
    if (argInfo.requiresSIB) pushByte(reciever, argInfo.SibByte);
    if (argInfo.NumValNumBytes==4) pushDword(reciever, argInfo.numValFourBytes, true);// implies that it is indirect, because if it is just a number the function would have returned above
    else if (argInfo.NumValNumBytes==1) pushByte(reciever, argInfo.numValOneByte);
    if (argInfo.isVar) resolveVar(reciever,argInfo.varName);
    if (debugInstructionOutput) {
        if (argInfo.isIndirect) cout << "NOP " << ((bitMode==64)?"QWORD":"DWORD") << " PTR " << argInfo.properString << endl;
        else cout << "NOP " << argInfo.properString << endl;
    }
}
template void NOP(std::ofstream& reciever);
template void NOP(std::ofstream& reciever, const char* arg);
template void NOP(std::vector<uint8_t>& reciever);
template void NOP(std::vector<uint8_t>& reciever, const char* arg);
template void NOP(Elf64SegmentHandler*& reciever);
template void NOP(Elf64SegmentHandler*& reciever, const char* arg);
template void NOP(Pe64SectionHandler*& reciever);
template void NOP(Pe64SectionHandler*& reciever, const char* arg);
#pragma endregion NOP

#pragma region XCHG
template <typename T>
void XCHG(T& reciever, const char* arg1, const char* arg2) {
    if (bitMode==0) return;
    argumentType arg1Info = parseArgument(arg1);
    if(arg1Info.isError) { cout << "arg1: \"" << arg1 << "\": error" << endl; return; }
    argumentType arg2Info = parseArgument(arg2);
    if(arg2Info.isError) { cout << "arg2: \"" << arg2 << "\": error" << endl; return; }

    if (arg1Info.isIndirect && arg2Info.isIndirect) { cout << "both registers cannot be indirect." << endl; return; }
    if (arg1Info.isJustNumber) { cout << "arg1 cannot be just a number." << endl; return; }
    if (arg2Info.isJustNumber) { cout << "arg2 cannot be just a number." << endl; return; }
    if ((arg1Info.bit!=0 && arg1Info.bit<32) || (arg2Info.bit!=0 && arg2Info.bit<32)) return;// dont have handling for these yet

    if (arg1Info.isJustRegister&&arg2Info.isJustRegister&&(arg1Info.registerOffset==INTEL_REG_OFF_eAX)) {
        // ex: xchg eAX, reg
        if (arg1Info.bit!=arg2Info.bit) { cout << "Operand size mis-match." << endl; return; }
        if (arg1Info.bit==64 && (arg2Info.registerOffset!=INTEL_REG_OFF_eAX)) pushByte(reciever, INTEL_INSTR64_OperandSz_OVRD);
        pushByte(reciever, INTEL_INSTR_XCHG_eAX_REGpRv+arg2Info.registerOffset);
        if (debugInstructionOutput) {
            if (arg2Info.registerOffset==INTEL_REG_OFF_eAX) cout << "NOP" << endl;
            else cout << "XCHG " << arg1Info.properString << ", " << arg2Info.properString << endl;
        }
    } else if (arg1Info.isJustRegister&&arg2Info.isJustRegister&&(arg2Info.registerOffset==INTEL_REG_OFF_eAX)) {
        // ex: xchg reg, eAX
        if (arg1Info.bit!=arg2Info.bit) { cout << "Operand size mis-match." << endl; return; }
        if (arg1Info.bit==64 && (arg1Info.registerOffset!=INTEL_REG_OFF_eAX)) pushByte(reciever, INTEL_INSTR64_OperandSz_OVRD);
        pushByte(reciever, INTEL_INSTR_XCHG_eAX_REGpRv+arg1Info.registerOffset);
        if (debugInstructionOutput) {
            if (arg1Info.registerOffset==INTEL_REG_OFF_eAX) cout << "NOP" << endl;
            else cout << "XCHG " << arg1Info.properString << ", " << arg2Info.properString << endl;
        }
    } else if (!arg2Info.isIndirect) {
        // ex: xchg (reg or [eip/rip+num] or [reg+num] or [base+index*scale+num]), reg
        if (arg1Info.isIndirect&&(bitMode==64)&&(arg1Info.bit==32)) pushByte(reciever, INTEL_INSTR_AddrSz_OVRD);
        if (arg2Info.isJustRegister&&(arg2Info.bit==64)) pushByte(reciever, INTEL_INSTR64_OperandSz_OVRD);
        pushByte(reciever, INTEL_INSTR_XCHG_REGv_RMv);
        const uint8_t RegOp = arg2Info.isJustRegister?arg2Info.registerRegOp:0;
        pushByte(reciever, arg1Info.ModRMByte|RegOp);
        if (arg1Info.requiresSIB) pushByte(reciever, arg1Info.SibByte);
        if (arg1Info.NumValNumBytes==4) pushDword(reciever, arg1Info.numValFourBytes, true);
        if (arg1Info.isVar) resolveVar(reciever,arg1Info.varName);
        else if (arg1Info.NumValNumBytes==1) pushByte(reciever, arg1Info.numValOneByte);
        cout << "XCHG " << ((arg2Info.bit==64)?"QWORD":"DWORD") << " PTR " << arg1Info.properString << ", " << arg2Info.properString << endl;
    } else {
        // ex: xchg reg, (reg or [eip/rip+num] or [reg+num] or [base+index*scale+num])
        if (arg1Info.isJustRegister&&arg2Info.isJustRegister&&(arg1Info.bit!=arg2Info.bit)) { cout << "Operand size mis-match." << endl; return; }
        if (arg2Info.isIndirect&&(bitMode==64)&&(arg2Info.bit==32)) pushByte(reciever, INTEL_INSTR_AddrSz_OVRD);
        if ((arg2Info.isJustRegister&&(arg2Info.bit==64))||(arg1Info.isJustRegister&&(arg1Info.bit==64))) pushByte(reciever, INTEL_INSTR64_OperandSz_OVRD);
        pushByte(reciever, INTEL_INSTR_XCHG_REGv_RMv);
        const uint8_t RegOp = arg1Info.isJustRegister?arg1Info.registerRegOp:0;
        pushByte(reciever, arg2Info.ModRMByte|RegOp);
        if (arg2Info.requiresSIB) pushByte(reciever, arg2Info.SibByte);
        if (arg2Info.NumValNumBytes==4) pushDword(reciever, arg2Info.numValFourBytes, true);
        else if (arg2Info.NumValNumBytes==1) pushByte(reciever, arg2Info.numValOneByte);
        if (arg2Info.isVar) resolveVar(reciever,arg2Info.varName);
        if (debugInstructionOutput) {
            if (arg2Info.isIndirect) cout << "XCHG " << arg1Info.properString << ", " << ((arg1Info.bit==64)?"QWORD":"DWORD") << " PTR " << arg2Info.properString << endl;
            else if (debugInstructionOutput) cout << "XCHG " << arg1Info.properString << ", " << arg2Info.properString << endl;
        }
    }
}
template void XCHG(std::ofstream& reciever, const char* arg1, const char* arg2);
template void XCHG(std::vector<uint8_t>& reciever, const char* arg1, const char* arg2);
template void XCHG(Elf64SegmentHandler*& reciever, const char* arg1, const char* arg2);
template void XCHG(Pe64SectionHandler*& reciever, const char* arg1, const char* arg2);
#pragma endregion XCHG

#pragma region MOV
template <typename T>
void MOV(T& reciever, const char* dst, const char* src) {
    if (bitMode==0) return;
    argumentType dstInfo = parseArgument(dst);
    if(dstInfo.isError) { cout << "dst: \"" << dst << "\": error" << endl; return; }
    argumentType srcInfo = parseArgument(src);
    if(srcInfo.isError) { cout << "src: \"" << src << "\": error" << endl; return; }

    if (dstInfo.isIndirect && srcInfo.isIndirect) { cout << "dst and src cannot both be indirect." << endl; return; }
    if (dstInfo.isJustNumber) { cout << "dst cannot be just a number." << endl; return; }
    if ((dstInfo.bit!=0 && dstInfo.bit<32) || (srcInfo.bit!=0 && srcInfo.bit<32)) { cout << "invalid bit" << endl; return; }// dont have handling for these yet

    if (srcInfo.isJustNumber&&dstInfo.isJustRegister&&(dstInfo.bit<64)) {
        // ex: mov reg, num
        if (dstInfo.bit==64) pushByte(reciever, INTEL_INSTR64_OperandSz_OVRD);
        pushByte(reciever, INTEL_INSTR_MOV_REGpRv_Iv+dstInfo.registerOffset);
        pushDword(reciever, srcInfo.numValFourBytes, true);
        if (debugInstructionOutput) cout << "MOV " << dstInfo.properString << ", " << srcInfo.properString << endl;
    } else if (!srcInfo.isIndirect) {
        // ex: mov (reg or [eip/rip+num] or [reg+num] or [base+index*scale+num]), (reg or num)
        if (!dstInfo.isIndirect&&!srcInfo.isJustNumber&&(dstInfo.bit!=srcInfo.bit)) { cout << "Operand size mis-match." << endl; return; }
        if (dstInfo.isIndirect&&(bitMode==64)&&(dstInfo.bit==32)) pushByte(reciever, INTEL_INSTR_AddrSz_OVRD);
        if ((srcInfo.isJustRegister&&(srcInfo.bit==64))||(dstInfo.isJustRegister&&(dstInfo.bit==64))) pushByte(reciever, INTEL_INSTR64_OperandSz_OVRD);
        pushByte(reciever, srcInfo.isJustRegister?INTEL_INSTR_MOV_RMv_REGv:INTEL_INSTR_MOV_RMv_Iv);// Specify if src is a register or an immutable
        const uint8_t RegOp = srcInfo.isJustRegister?srcInfo.registerRegOp:0;
        pushByte(reciever, dstInfo.ModRMByte|RegOp);
        if (dstInfo.requiresSIB) pushByte(reciever, dstInfo.SibByte);
        if (dstInfo.NumValNumBytes==4) pushDword(reciever, dstInfo.numValFourBytes, true);
        else if (dstInfo.NumValNumBytes==1) pushByte(reciever, dstInfo.numValOneByte);
        if (dstInfo.isVar) resolveVar(reciever,dstInfo.varName);
        if (srcInfo.isJustNumber) pushDword(reciever, srcInfo.numValFourBytes, true);
        if (debugInstructionOutput) {
            if (dstInfo.isIndirect&&!dstInfo.isVar) cout << "MOV " << ((srcInfo.bit==64)?"QWORD":"DWORD") << " PTR " << dstInfo.properString << ", " << srcInfo.properString << endl;
            else cout << "MOV " << dstInfo.properString << ", " << srcInfo.properString << endl;
        }
    } else {
        // ex: mov reg, ([eip/rip+num] or [reg+num] or [base+index*scale+num])
        if (bitMode==64 && srcInfo.bit==32) pushByte(reciever, INTEL_INSTR_AddrSz_OVRD);
        if (dstInfo.bit==64) pushByte(reciever, INTEL_INSTR64_OperandSz_OVRD);
        pushByte(reciever, INTEL_INSTR_MOV_REGv_RMv);
        pushByte(reciever, srcInfo.ModRMByte|dstInfo.registerRegOp);
        if (srcInfo.requiresSIB) pushByte(reciever, srcInfo.SibByte);
        if (srcInfo.NumValNumBytes==4) pushDword(reciever, srcInfo.numValFourBytes, true);
        else if (srcInfo.NumValNumBytes==1) pushByte(reciever, srcInfo.numValOneByte);
        if (srcInfo.isVar) resolveVar(reciever,srcInfo.varName);
        if (debugInstructionOutput) {
            if (!srcInfo.isVar) cout << "MOV " << dstInfo.properString << ", " << ((dstInfo.bit==64)?"QWORD":"DWORD") << " PTR " << srcInfo.properString << endl;
            else cout << "MOV " << dstInfo.properString << ", " << srcInfo.properString << endl;
        }
    }
}
template void MOV(std::ofstream& reciever, const char* dst, const char* src);
template void MOV(std::vector<uint8_t>& reciever, const char* dst, const char* src);
template void MOV(Elf64SegmentHandler*& reciever, const char* dst, const char* src);
template void MOV(Pe64SectionHandler*& reciever, const char* dst, const char* src);
#pragma endregion MOV

#pragma region RetRetf
template <typename T>
void RET(T& reciever) {
    pushByte(reciever, INTEL_INSTR_RET);
    if (debugInstructionOutput) cout << "RET" << endl;
}
template <typename T>
void RET(T& reciever,  const char* num) {
    if (bitMode==0) return;
    argumentType numInfo = parseArgument(num);
    if(numInfo.isError) { cout << "num: \"" << num << "\": error" << endl; return; }
    
    if (!numInfo.isJustNumber) { cout << "ret argument may only be a number" << endl; return; }
    if (numInfo.numValFourBytes >= 65536) { cout << "Number is too large to return." << endl; return; }

    pushByte(reciever, INTEL_INSTR_RET_I16);
    pushWord(reciever, (uint16_t)numInfo.numValFourBytes, true);
    if (debugInstructionOutput) cout << "RET " << numInfo.properString << endl;
}
template <typename T>
void RETF(T& reciever) {
    pushByte(reciever, INTEL_INSTR_RETF);
    if (debugInstructionOutput) cout << "RETF" << endl;
}
template <typename T>
void RETF(T& reciever,  const char* num) {
    if (bitMode==0) return;
    argumentType numInfo = parseArgument(num);
    if(numInfo.isError) { cout << "num: \"" << num << "\": error" << endl; return; }
    
    if (!numInfo.isJustNumber) { cout << "Retf argument may only be a number" << endl; return; }
    if (numInfo.numValFourBytes >= 65536) { cout << "Number is too large to return." << endl; return; }

    pushByte(reciever, INTEL_INSTR_RETF_I16);
    pushWord(reciever, (uint16_t)numInfo.numValFourBytes, true);
    if (debugInstructionOutput) cout << "RETF " << numInfo.properString << endl;
}
template void RET(std::ofstream& reciever);
template void RET(std::ofstream& reciever, const char* num);
template void RETF(std::ofstream& reciever);
template void RETF(std::ofstream& reciever, const char* num);
template void RET(std::vector<uint8_t>& reciever);
template void RET(std::vector<uint8_t>& reciever, const char* num);
template void RETF(std::vector<uint8_t>& reciever);
template void RETF(std::vector<uint8_t>& reciever, const char* num);
template void RET(Elf64SegmentHandler*& reciever);
template void RET(Elf64SegmentHandler*& reciever, const char* num);
template void RETF(Elf64SegmentHandler*& reciever);
template void RETF(Elf64SegmentHandler*& reciever, const char* num);
template void RET(Pe64SectionHandler*& reciever);
template void RET(Pe64SectionHandler*& reciever, const char* num);
template void RETF(Pe64SectionHandler*& reciever);
template void RETF(Pe64SectionHandler*& reciever, const char* num);
#pragma endregion RetRetf

#pragma region IntHlt
template <typename T>
void INT3(T& reciever) {
    if (bitMode==0) return;
    pushByte(reciever, INTEL_INSTR_INT3);
    if (debugInstructionOutput) cout << "INT3" << endl;
}
template <typename T>
void INT(T& reciever,  const char* num) {
    if (bitMode==0) return;
    argumentType numInfo = parseArgument(num);
    if(numInfo.isError) { cout << "num: \"" << num << "\": error" << endl; return; }
    
    if (!numInfo.isJustNumber) { cout << "int argument may only be a number" << endl; return; }
    if (numInfo.numValFourBytes >= 256) { cout << "Number is too large to return." << endl; return; }

    if (numInfo.numValFourBytes==3) {
        pushByte(reciever, INTEL_INSTR_INT3);
        if (debugInstructionOutput) cout << "INT 3" << endl;
    } else {
        pushByte(reciever, INTEL_INSTR_INT_Ib);
        pushByte(reciever, numInfo.numValFourBytes);
        if (debugInstructionOutput) cout << "INT " << numInfo.properString << endl;
    }
}
template void INT3(std::ofstream& reciever);
template void INT3(std::vector<uint8_t>& reciever);
template void INT3(Elf64SegmentHandler*& reciever);
template void INT3(Pe64SectionHandler*& reciever);
template void INT(std::ofstream& reciever, const char* num);
template void INT(std::vector<uint8_t>& reciever, const char* num);
template void INT(Elf64SegmentHandler*& reciever, const char* num);
template void INT(Pe64SectionHandler*& reciever, const char* num);

template <typename T>
void HLT(T& reciever) {
    if (bitMode==0) return;
    pushByte(reciever, INTEL_INSTR_HLT);
    if (debugInstructionOutput) cout << "HLT" << endl;
}
template void HLT(std::ofstream& reciever);
template void HLT(std::vector<uint8_t>& reciever);
template void HLT(Elf64SegmentHandler*& reciever);
template void HLT(Pe64SectionHandler*& reciever);
#pragma endregion IntHlt

#pragma region Test
template <typename T>
void TEST(T& reciever, const char* arg1, const char* arg2) {
    if (bitMode==0) return;
    argumentType arg1Info = parseArgument(arg1);
    if(arg1Info.isError) { cout << "arg1: \"" << arg1 << "\": error" << endl; return; }
    argumentType arg2Info = parseArgument(arg2);
    if(arg2Info.isError) { cout << "arg2: \"" << arg2 << "\": error" << endl; return; }

    if (arg1Info.isIndirect&&arg2Info.isIndirect) { cout << "both registers cannot be indirect." << endl; return; }
    if (arg1Info.isJustNumber) { cout << "arg1 cannot be just a number." << endl; return; }
    if (!arg2Info.isJustNumber) { cout << "arg2 can only be a number." << endl; return; }
    if ((arg1Info.bit!=0 && arg1Info.bit<32) || (arg2Info.bit!=0 && arg2Info.bit<32)) { cout << "invalid bit" << endl; return; }// dont have handling for these yet

    if (arg1Info.isJustRegister&&arg1Info.registerOffset==INTEL_REG_OFF_eAX) {
        // ex: test eAX/rAX, num
        if (!arg1Info.isIndirect && arg1Info.bit==64) pushByte(reciever, INTEL_INSTR64_OperandSz_OVRD);
        pushByte(reciever, INTEL_INSTR_TEST_eAX_Iv);
        pushDword(reciever, arg2Info.numValFourBytes, true);
        if (debugInstructionOutput) cout << "TEST " << arg1Info.properString << ", " << arg2Info.properString << endl;
    } else {
        // ex: test (reg or [eip/rip+num] or [reg+num] or [base+index*scale+num]), num
        if (arg1Info.isIndirect&&(bitMode==64)&&(arg1Info.bit==32)) pushByte(reciever, INTEL_INSTR_AddrSz_OVRD);
        if ((arg1Info.isJustRegister&&(arg1Info.bit==64))||(arg1Info.isIndirect&&(bitMode==64))) pushByte(reciever, INTEL_INSTR64_OperandSz_OVRD);
        pushByte(reciever, INTEL_INSTR_OP2v);// Specify if src is a register or an immutable
        pushByte(reciever, arg1Info.ModRMByte|INTEL_ModRM_OP2_TEST_RM_I);
        if (arg1Info.requiresSIB) pushByte(reciever, arg1Info.SibByte);
        if (arg1Info.NumValNumBytes==4) pushDword(reciever, arg1Info.numValFourBytes, true);
        else if (arg1Info.NumValNumBytes==1) pushByte(reciever, arg1Info.numValOneByte);
        if (arg1Info.isVar) resolveVar(reciever,arg1Info.varName);
        pushDword(reciever, arg2Info.numValFourBytes, true);
        if (debugInstructionOutput) {
            if (arg1Info.isIndirect) cout << "TEST " << ((bitMode==64)?"QWORD":"DWORD") << " PTR " << arg1Info.properString << ", " << arg2Info.properString << endl;
            else cout << "TEST " << arg1Info.properString << ", " << arg2Info.properString << endl;
        }
    }
}
template void TEST(std::ofstream& reciever, const char* arg1, const char* arg2);
template void TEST(std::vector<uint8_t>& reciever, const char* arg1, const char* arg2);
template void TEST(Elf64SegmentHandler*& reciever, const char* arg1, const char* arg2);
template void TEST(Pe64SectionHandler*& reciever, const char* arg1, const char* arg2);
#pragma endregion Test

#pragma region NotNeg
template <typename T>
void NotNeg(T& reciever, const char* arg, const char* instructionName, const uint8_t& op2Code=INTEL_ModRM_OP2_NOT_RM) {
    if (bitMode==0) return;
    argumentType argInfo = parseArgument(arg);
    if(argInfo.isError) { cout << "arg: \"" << arg << "\": error" << endl; return; }
    
    if (argInfo.isJustNumber) { cout << "arg cannot be just a number." << endl; return; }
    if ((argInfo.bit!=0)&&(argInfo.bit<32)) { cout << "invalid bit" << endl; return; }// dont have handling for these yet

    // ex: POP (reg or [eip/rip+num] or [reg+num] or [base+index*scale+num])
    if (argInfo.isIndirect&&(bitMode==64)&&(argInfo.bit==32)) pushByte(reciever, INTEL_INSTR_AddrSz_OVRD);
    if ((argInfo.isJustRegister&&(argInfo.bit==64))||(argInfo.isIndirect&&(bitMode==64))) pushByte(reciever, INTEL_INSTR64_OperandSz_OVRD);
    pushByte(reciever, INTEL_INSTR_OP2v);
    pushByte(reciever, argInfo.ModRMByte|op2Code);
    if (argInfo.requiresSIB) pushByte(reciever, argInfo.SibByte);
    if (argInfo.NumValNumBytes==4) pushDword(reciever, argInfo.numValFourBytes, true);// implies that it is indirect, because if it is just a number the function would have returned above
    else if (argInfo.NumValNumBytes==1) pushByte(reciever, argInfo.numValOneByte);
    if (argInfo.isVar) resolveVar(reciever,argInfo.varName);
    if (debugInstructionOutput) {
        if (argInfo.isIndirect) cout << instructionName << " " << ((bitMode==64)?"QWORD":"DWORD") << " PTR " << argInfo.properString << endl;
        else cout << instructionName << " " << argInfo.properString << endl;
    }
}
template <typename T>
void NOT(T& reciever, const char* arg) {
    NotNeg(reciever,arg,"NOT",INTEL_ModRM_OP2_NOT_RM);
}
template <typename T>
void NEG(T& reciever, const char* arg) {
    NotNeg(reciever,arg,"NEG",INTEL_ModRM_OP2_NEG_RM);
}
template void NOT(std::ofstream& reciever, const char* arg);
template void NEG(std::ofstream& reciever, const char* arg);
template void NOT(std::vector<uint8_t>& reciever, const char* arg);
template void NEG(std::vector<uint8_t>& reciever, const char* arg);
template void NOT(Elf64SegmentHandler*& reciever, const char* arg);
template void NEG(Elf64SegmentHandler*& reciever, const char* arg);
template void NOT(Pe64SectionHandler*& reciever, const char* arg);
template void NEG(Pe64SectionHandler*& reciever, const char* arg);
#pragma endregion NotNeg

#pragma region UmulMulUdivDiv
template <typename T>
void UmulMulUdivDiv(T& reciever, const char* arg, const char* instructionName, const uint8_t& op2Code=INTEL_ModRM_OP2_UMUL_eDX_eAX_RM) {
    if (bitMode==0) return;
    argumentType argInfo = parseArgument(arg);
    if(argInfo.isError) { cout << "arg: \"" << arg << "\": error" << endl; return; }
    
    if (argInfo.isJustNumber) { cout << "arg cannot be just a number." << endl; return; }
    if ((argInfo.bit!=0)&&(argInfo.bit<32)) { cout << "invalid bit" << endl; return; }// dont have handling for these yet

    // ex: POP (reg or [eip/rip+num] or [reg+num] or [base+index*scale+num])
    if (argInfo.isIndirect&&(bitMode==64)&&(argInfo.bit==32)) pushByte(reciever, INTEL_INSTR_AddrSz_OVRD);
    if ((argInfo.isJustRegister&&(argInfo.bit==64))||(argInfo.isIndirect&&(bitMode==64))) pushByte(reciever, INTEL_INSTR64_OperandSz_OVRD);
    pushByte(reciever, INTEL_INSTR_OP2v);
    pushByte(reciever, argInfo.ModRMByte|op2Code);
    if (argInfo.requiresSIB) pushByte(reciever, argInfo.SibByte);
    if (argInfo.NumValNumBytes==4) pushDword(reciever, argInfo.numValFourBytes, true);// implies that it is indirect, because if it is just a number the function would have returned above
    else if (argInfo.NumValNumBytes==1) pushByte(reciever, argInfo.numValOneByte);
    if (argInfo.isVar) resolveVar(reciever,argInfo.varName);
    if (debugInstructionOutput) {
        if (argInfo.isIndirect) cout << instructionName << " " << ((bitMode==64)?"QWORD":"DWORD") << " PTR " << argInfo.properString << endl;
        else cout << instructionName << " " << argInfo.properString << endl;
    }
}
template <typename T>
void UMUL(T& reciever, const char* arg) {
    UmulMulUdivDiv(reciever,arg,"MUL",INTEL_ModRM_OP2_UMUL_eDX_eAX_RM);
}
template <typename T>
void MUL(T& reciever, const char* arg) {
    UmulMulUdivDiv(reciever,arg,"IMUL",INTEL_ModRM_OP2_MUL_eDX_eAX_RM);
}
template <typename T>
void UDIV(T& reciever, const char* arg) {
    UmulMulUdivDiv(reciever,arg,"DIV",INTEL_ModRM_OP2_UDIV_eDX_eAX_RM);
}
template <typename T>
void DIV(T& reciever, const char* arg) {
    UmulMulUdivDiv(reciever,arg,"IDIV",INTEL_ModRM_OP2_DIV_eDX_eAX_RM);
}
template void UMUL(std::ofstream& reciever, const char* arg);
template void MUL(std::ofstream& reciever, const char* arg);
template void UDIV(std::ofstream& reciever, const char* arg);
template void DIV(std::ofstream& reciever, const char* arg);
template void UMUL(std::vector<uint8_t>& reciever, const char* arg);
template void MUL(std::vector<uint8_t>& reciever, const char* arg);
template void UDIV(std::vector<uint8_t>& reciever, const char* arg);
template void DIV(std::vector<uint8_t>& reciever, const char* arg);
template void UMUL(Elf64SegmentHandler*& reciever, const char* arg);
template void MUL(Elf64SegmentHandler*& reciever, const char* arg);
template void UDIV(Elf64SegmentHandler*& reciever, const char* arg);
template void DIV(Elf64SegmentHandler*& reciever, const char* arg);
template void UMUL(Pe64SectionHandler*& reciever, const char* arg);
template void MUL(Pe64SectionHandler*& reciever, const char* arg);
template void UDIV(Pe64SectionHandler*& reciever, const char* arg);
template void DIV(Pe64SectionHandler*& reciever, const char* arg);
#pragma endregion UmulMulUdivDiv

#pragma region flag stuff
// Carry flag
template <typename T>
void CLC(T& reciever) {
    if (bitMode==0) return;
    pushByte(reciever, INTEL_INSTR_CLR_CF);
    if (debugInstructionOutput) cout << "CLC" << endl;
}
template <typename T>
void STC(T& reciever) {
    if (bitMode==0) return;
    pushByte(reciever, INTEL_INSTR_SET_CF);
    if (debugInstructionOutput) cout << "STC" << endl;
}
// Interrupt  flag
template <typename T>
void CLI(T& reciever) {
    if (bitMode==0) return;
    pushByte(reciever, INTEL_INSTR_CLR_IF);
    if (debugInstructionOutput) cout << "CLI" << endl;
}
template <typename T>
void STI(T& reciever) {
    if (bitMode==0) return;
    pushByte(reciever, INTEL_INSTR_SET_IF);
    if (debugInstructionOutput) cout << "STI" << endl;
}
// Direction flag
template <typename T>
void CLD(T& reciever) {
    if (bitMode==0) return;
    pushByte(reciever, INTEL_INSTR_CLR_DF);
    if (debugInstructionOutput) cout << "CLD" << endl;
}
template <typename T>
void STD(T& reciever) {
    if (bitMode==0) return;
    pushByte(reciever, INTEL_INSTR_SET_DF);
    if (debugInstructionOutput) cout << "STD" << endl;
}
template void CLC(std::ofstream& reciever);
template void STC(std::ofstream& reciever);
template void CLI(std::ofstream& reciever);
template void STI(std::ofstream& reciever);
template void CLD(std::ofstream& reciever);
template void STD(std::ofstream& reciever);
template void CLC(std::vector<uint8_t>& reciever);
template void STC(std::vector<uint8_t>& reciever);
template void CLI(std::vector<uint8_t>& reciever);
template void STI(std::vector<uint8_t>& reciever);
template void CLD(std::vector<uint8_t>& reciever);
template void STD(std::vector<uint8_t>& reciever);
template void CLC(Elf64SegmentHandler*& reciever);
template void STC(Elf64SegmentHandler*& reciever);
template void CLI(Elf64SegmentHandler*& reciever);
template void STI(Elf64SegmentHandler*& reciever);
template void CLD(Elf64SegmentHandler*& reciever);
template void STD(Elf64SegmentHandler*& reciever);
template void CLC(Pe64SectionHandler*& reciever);
template void STC(Pe64SectionHandler*& reciever);
template void CLI(Pe64SectionHandler*& reciever);
template void STI(Pe64SectionHandler*& reciever);
template void CLD(Pe64SectionHandler*& reciever);
template void STD(Pe64SectionHandler*& reciever);
#pragma endregion flag stuff

#pragma endregion instructions
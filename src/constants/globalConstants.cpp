#include <string.h>
#include <iostream>
#include <algorithm>// for std::min

#include "globalConstants.h"
#include "intelConstants.h"

uint8_t bitMode = 0;

#include <iomanip>
#pragma region helper functions
void pushChars(std::ostream &stream, const uint8_t *chars, uint32_t len, const bool &LSB) {
    stream << std::hex << std::setfill('0') << std::setw(2);
    if (LSB)
        for (short i = 0; i < len; i++) stream << (int)chars[i] << " ";
    else
        for (short i = len - 1; i >= 0; i--) stream << (int)chars[i] << " ";
    stream << std::dec << std::setfill(' ');
}
void pushChars(std::ofstream &stream, const uint8_t *chars, uint32_t len, const bool &LSB) {
    if (LSB)
        for (short i = 0; i < len; i++) stream << chars[i];
    else
        for (short i = len - 1; i >= 0; i--) stream << chars[i];
}
void pushChars(std::ofstream &stream, std::vector<uint8_t> &vector) {
    for (size_t i = 0u; i < vector.size(); i++) stream << vector[i];
}
void pushChars(std::vector<uint8_t> &vector, const uint8_t *chars, uint32_t len, const bool &LSB) {
    if (LSB)
        for (char i = 0; i < len; i++) vector.push_back(chars[i]);
    else
        for (char i = len - 1; i >= 0; i--) vector.push_back(chars[i]);
}

void pushByte(std::ostream &stream, const uint8_t &byte) {
    pushChars(stream, &byte, 1, false);
}
void pushByte(std::ofstream &stream, const uint8_t &byte) {
    pushChars(stream, &byte, 1, false);
}
void pushByte(std::vector<uint8_t> &vector, const uint8_t &byte) {
    pushChars(vector, &byte, 1, false);
}

void pushHalfWord(std::ostream &stream, const uint16_t &halfword, const bool &LSB) {
    pushChars(stream, static_cast<const uint8_t *>(static_cast<const void *>(&halfword)), 2, LSB);
}
void pushHalfWord(std::ofstream &stream, const uint16_t &halfword, const bool &LSB) {
    pushChars(stream, static_cast<const uint8_t *>(static_cast<const void *>(&halfword)), 2, LSB);
}
void pushHalfWord(std::vector<uint8_t> &vector, const uint16_t &halfword, const bool &LSB) {
    pushChars(vector, static_cast<const uint8_t *>(static_cast<const void *>(&halfword)), 2, LSB);
}

void pushWord(std::ostream &stream, const uint32_t &word, const bool &LSB) {
    pushChars(stream, static_cast<const uint8_t *>(static_cast<const void *>(&word)), 4, LSB);
}
void pushWord(std::ofstream &stream, const uint32_t &word, const bool &LSB) {
    pushChars(stream, static_cast<const uint8_t *>(static_cast<const void *>(&word)), 4, LSB);
}
void pushWord(std::vector<uint8_t> &vector, const uint32_t &word, const bool &LSB) {
    pushChars(vector, static_cast<const uint8_t *>(static_cast<const void *>(&word)), 4, LSB);
}

void pushDword(std::ostream &stream, const uint64_t &dword, const bool &LSB) {
    pushChars(stream, static_cast<const uint8_t *>(static_cast<const void *>(&dword)), 8, LSB);
}
void pushDword(std::ofstream &stream, const uint64_t &dword, const bool &LSB) {
    pushChars(stream, static_cast<const uint8_t *>(static_cast<const void *>(&dword)), 8, LSB);
}
void pushDword(std::vector<uint8_t> &vector, const uint64_t &dword, const bool &LSB) {
    pushChars(vector, static_cast<const uint8_t *>(static_cast<const void *>(&dword)), 8, LSB);
}

void padBytes(std::ofstream &stream, const uint32_t &numBytes) {
    uint8_t C8[5]{0x00,0x00,0x00,0x00,0x00};
    for (int i = 0; i < numBytes; i++) {
        stream << C8[i%5];
        std::cout << C8[i%5];
    }
}

struct registerInfo {
    bool isValid=true;
    std::string reason="";
    unsigned char bit;
    uint8_t offset;
    uint8_t regOp;
    uint8_t RM;
    uint8_t index;
    uint8_t base;
};
registerInfo processReg(const char* reg, const size_t& len) {
    registerInfo info;
    if (len==3u && reg[0]=='r') {// all 64 bit registers are 3 characters long and start with r
        info.bit=64;
        if ((reg[1]=='A')&&(reg[2]=='X')) {
            info.offset=INTEL_REG_OFF_rAX;
        } else if ((reg[1]=='C')&&(reg[2]=='X')) {
            info.offset=INTEL_REG_OFF_rCX;
        } else if ((reg[1]=='D')&&(reg[2]=='X')) {
            info.offset=INTEL_REG_OFF_rDX;
        } else if ((reg[1]=='B')&&(reg[2]=='X')) {
            info.offset=INTEL_REG_OFF_rBX;
        } else if ((reg[1]=='S')&&(reg[2]=='P')) {
            info.offset=INTEL_REG_OFF_rSP;
        } else if ((reg[1]=='B')&&(reg[2]=='P')) {
            info.offset=INTEL_REG_OFF_rBP;
        } else if ((reg[1]=='S')&&(reg[2]=='I')) {
            info.offset=INTEL_REG_OFF_rSI;
        } else if ((reg[1]=='D')&&(reg[2]=='I')) {
            info.offset=INTEL_REG_OFF_rDI;
        } else { info.isValid=false; info.reason="\""+std::string(reg)+"\" is not a valid register."; return info; }
        if (bitMode<64) {
            info.isValid=false; info.reason="64 bit registers may only be used in 64 bit mode.";
        }
    } else if (len==3u && reg[0]=='e') {// all 32 bit registers are 3 characters long and start with e
        info.bit=32;
        if ((reg[1]=='A')&&(reg[2]=='X')) {
            info.offset=INTEL_REG_OFF_eAX;
        } else if ((reg[1]=='C')&&(reg[2]=='X')) {
            info.offset=INTEL_REG_OFF_eCX;
        } else if ((reg[1]=='D')&&(reg[2]=='X')) {
            info.offset=INTEL_REG_OFF_eDX;
        } else if ((reg[1]=='B')&&(reg[2]=='X')) {
            info.offset=INTEL_REG_OFF_eBX;
        } else if ((reg[1]=='S')&&(reg[2]=='P')) {
            info.offset=INTEL_REG_OFF_eSP;
        } else if ((reg[1]=='B')&&(reg[2]=='P')) {
            info.offset=INTEL_REG_OFF_eBP;
        } else if ((reg[1]=='S')&&(reg[2]=='I')) {
            info.offset=INTEL_REG_OFF_eSI;
        } else if ((reg[1]=='D')&&(reg[2]=='I')) {
            info.offset=INTEL_REG_OFF_eDI;
        } else { info.isValid=false; info.reason="\""+std::string(reg)+"\" is not a valid register."; return info; }
    } else if (len==2u && (reg[1]=='H' || reg[1]=='L')) {// all 8 bit registers are 2 characters long and end with either h or l
        info.bit=8u;
        if ((reg[0]=='A')) {
            info.offset=INTEL_REG_OFF_AL;
        } else if ((reg[0]=='C')) {
            info.offset=INTEL_REG_OFF_CL;
        } else if ((reg[0]=='D')) {
            info.offset=INTEL_REG_OFF_DL;
        } else if ((reg[0]=='B')) {
            info.offset=INTEL_REG_OFF_BL;
        } else { info.isValid=false; info.reason="\""+std::string(reg)+"\" is not a valid register."; return info; }
        if (reg[1]=='H') info.offset+=4;
    } else if (len==2u) {//most likely either a 16 bit rigister or invalid
        info.bit=16u;
        if ((reg[0]=='A')&&(reg[1]=='X')) {
            info.offset=INTEL_REG_OFF_AX;
        } else if ((reg[0]=='C')&&(reg[1]=='X')) {
            info.offset=INTEL_REG_OFF_CX;
        } else if ((reg[0]=='D')&&(reg[1]=='X')) {
            info.offset=INTEL_REG_OFF_DX;
        } else if ((reg[0]=='B')&&(reg[1]=='X')) {
            info.offset=INTEL_REG_OFF_BX;
        } else if ((reg[0]=='S')&&(reg[1]=='P')) {
            info.offset=INTEL_REG_OFF_SP;
        } else if ((reg[0]=='B')&&(reg[1]=='P')) {
            info.offset=INTEL_REG_OFF_BP;
        } else if ((reg[0]=='S')&&(reg[1]=='I')) {
            info.offset=INTEL_REG_OFF_SI;
        } else if ((reg[0]=='D')&&(reg[1]=='I')) {
            info.offset=INTEL_REG_OFF_DI;
        } else { info.isValid=false; info.reason="\""+std::string(reg)+"\" is not a valid register."; return info; }
    } else { info.isValid=false; info.reason="\""+std::string(reg)+"\" is not a valid register."; return info; }
    info.regOp=info.index=info.offset<<3;
    info.RM=info.base=info.offset;
    return info;
}

struct numberInfo {
    bool isValid=true;
    std::string reason="";
    unsigned long value;
    unsigned int len=0;
};
numberInfo processNum(const char* num, const size_t& len) {
    numberInfo info;
    std::string numStr = "";
    unsigned int numBase = 10;
    if (num[0]=='0'&&(num[1]=='b'||num[1]=='d'||num[1]=='x')) {// for numbers in the form of 0xb0001101, 0d25, or 0x19
        info.len+=2;// add 2 to length for the 0b/0d/0x
        // loop through string until it reaches 32 characters, or finds an ending character
        bool foundEnd = false;
        for (unsigned int i = 0; i <= std::min(31u,static_cast<unsigned int>(len-2u)); i++) {
            const char chr = std::tolower(num[i+2]);
            //std::cout << "char: '" << chr << "'" << std::endl;
            if (chr==']'||chr=='+'||chr=='\0') { foundEnd=true; break; }
            else if ((chr>='0' && chr<='9') || (chr>='a' && chr<='z')) {
                info.len++; numStr+=chr;
            } else if (chr=='_'||chr==',') { info.len++; continue; }//ignore underscores and commas
            else {info.reason="Invalid character '"+num[i+2]+std::string("'."); break;}// if there is a character that is not a number
        }
        if (!foundEnd) { if (info.reason.empty()) info.reason="Could not find end of Number"; info.isValid=false; return info; }
        // get number base based on the number abbreviation
        if (num[1]=='b') numBase=2;
        else if (num[1]=='x') numBase=16;
    } else {
        // loop through string until it reaches 32 characters, or finds an ending character
        bool foundEnd = false;
        for (unsigned int i = 0; i <= std::min(31u,static_cast<unsigned int>(len)); i++) {
            const char chr = std::tolower(num[i]);
            if (chr==']'||chr=='+'||chr=='\0') { foundEnd=true; break; }
            else if ((chr>='0' && chr<='9') || (chr>='a' && chr<='z')) {
                info.len++; numStr+=chr;
            } else if (chr=='_'||chr==',') { info.len++; continue; }//ignore underscores and commas
            else {info.reason="Invalid character '"+chr+std::string("'."); break;}// if there is a character that is not a number
        }
        if (!foundEnd) { if (info.reason.empty()) info.reason="Could not find end of Number"; info.isValid=false; return info; }
    }
    // parse number from the string
    char* p;
    info.value=strtol(numStr.c_str(),&p,numBase);// the character p represents the first character that "strtol" finds that is not valid, or '\0' if it did not find an invalid character
    if (*p!=0) { info.isValid=false; info.reason="Invalid character '"+*p+std::string("'."); return info; }
    return info;
}

struct instructionArgInfo { // isRegister and isNumber will only ever be true if isIndirect is true
    bool isValid=true;
    std::string reason="";
    unsigned int code=0;
    size_t strSize;
    bool isIndirect=false;
    unsigned char bit=0;
    bool hasReg1=false;
    std::string reg1Str="";
    uint8_t reg1Offset;
    uint8_t reg1RegOp;
    uint8_t reg1RM;
    uint8_t reg1Index;
    uint8_t reg1Base;
    bool hasReg2=false;
    std::string reg2Str="";
    uint8_t reg2Offset;
    uint8_t reg2RegOp;
    uint8_t reg2RM;
    uint8_t reg2Index;
    uint8_t reg2Base;
    uint8_t reg2Scale=1;
    bool hasNumber=false;
    unsigned long numValue=0u;
};
instructionArgInfo processArg(const char* arg) {
    instructionArgInfo info;
    info.strSize = std::string(arg).size();
    unsigned int cursor=0;
    if (arg[0]=='[') {
        info.isIndirect=true;
        cursor++;
        while (true) {
            if (arg[cursor]=='\0') break;
            if (arg[cursor]>='0' && arg[cursor]<='9') {
                // parse a number
                numberInfo numInfo = processNum(arg+cursor,info.strSize-cursor);
                if (!numInfo.isValid) { info.reason=numInfo.reason; info.isValid=false; return info;}
                info.hasNumber=true;
                info.numValue+=numInfo.value;//if there is more than one number added, they just get summed
                cursor+=numInfo.len;
                if (arg[cursor]=='+') {cursor++; continue; }
                else break;
            } else if (!info.hasReg1 || !info.hasReg2) {
                const char nextChar = std::tolower(arg[cursor]);
                size_t regLen = 0;
                if (nextChar=='e' || nextChar=='r') regLen=3u; // most likely eax,ecx,edx,ebx,esp,ebp,esi,edi, rax,rcx,rdx,rbx,rsp,rbp,rsi,or rdi
                else if (nextChar=='a' || nextChar=='c' || nextChar=='d' || nextChar=='b' || nextChar=='s' || nextChar=='b') regLen=2u;// most likely ax,cx,dx,bx,sp,bp,si,di, ah,ch,dh,bh,al,cl,dl,or bl
                else { info.reason="Invalid register \""+std::string(arg)+"\"."; info.isValid=false; return info; }
                std::string tmpStr;
                if (regLen==3u) {
                    tmpStr+=std::tolower(arg[cursor]); tmpStr+=std::toupper(arg[cursor+1]); tmpStr+=std::toupper(arg[cursor+2]); // I prefer capitalization like eAX, eCX, rAX, and rCX
                } else {// regLen==2u
                    tmpStr+=std::toupper(arg[cursor]); tmpStr+=std::toupper(arg[cursor+1]); // I prefer capitalization like AL, AH, AX, and CX
                }
                registerInfo regInfo = processReg(tmpStr.c_str(), regLen);
                if (!regInfo.isValid) { info.reason=regInfo.reason; info.isValid=false; return info; }
                if (regInfo.bit<32) { info.reason="8 and 16 bit registers are not valid for indirect addressing."; info.isValid=false; return info; }
                if (info.bit!=0 && regInfo.bit!=info.bit) { info.reason="\""+tmpStr+"\" operand size mis-match."; info.isValid=false; return info; }
                cursor+=regLen;

                bool hasMultiplier=arg[cursor]=='*';
                if (hasMultiplier) {
                    cursor++;
                    numberInfo numInfo = processNum(arg+cursor,info.strSize-cursor);
                    if (!numInfo.isValid || (numInfo.value!=1 && numInfo.value!=2 && numInfo.value!=4 && numInfo.value!=8)) { info.reason="Invalid multiplier for \""+tmpStr+"\"."; info.isValid=false; return info;}
                    cursor+=numInfo.len;
                    if (numInfo.value!=1) {
                        if (!info.hasReg2) {
                            info.hasReg2 = true;
                            info.bit=regInfo.bit;
                            info.reg2Str=tmpStr;
                            info.reg2Offset = regInfo.offset;
                            info.reg2RegOp = regInfo.regOp;
                            info.reg2RM = regInfo.RM;
                            info.reg2Index = regInfo.index;
                            info.reg2Base = regInfo.base;
                            info.reg2Scale = intToSIBScale(numInfo.value);
                        } else if (info.reg2Offset==regInfo.offset && info.reg2Scale==intToSIBScale(numInfo.value)) {// add together reg2 and the new register
                            info.reg2Scale = intToSIBScale(numInfo.value*2);
                        } else {
                            if (info.hasReg1) info.reason="Too many registers in address.";
                            else info.reason="More than one register cannot have a scale value.";
                            info.isValid=false; return info;
                        }
                        if (arg[cursor]=='+') {cursor++; continue; }
                        else break;
                    }
                    //if the scale is 1, it continues to the code below
                }
                if (!info.hasReg1) {
                    info.hasReg1 = true;
                    info.bit = regInfo.bit;
                    info.reg1Str = tmpStr;
                    info.reg1Offset = regInfo.offset;
                    info.reg1RegOp = regInfo.regOp;
                    info.reg1RM = regInfo.RM;
                    info.reg1Index = regInfo.index;
                    info.reg1Base = regInfo.base;
                } else if (!info.hasReg2) {
                    if (info.reg1Offset==regInfo.offset) {// remove reg1 and combine into reg2
                        info.hasReg1 = false;
                        info.reg1Str = "";
                        info.reg1Offset = 0;
                        info.reg1RegOp = 0;
                        info.reg1RM = 0;
                        info.reg1Index = 0;
                        info.reg1Base = 0;
                        info.hasReg2 = true;
                        info.reg2Str = tmpStr;
                        info.reg2Offset = regInfo.offset;
                        info.reg2RegOp = regInfo.regOp;
                        info.reg2RM = regInfo.RM;
                        info.reg2Index = regInfo.index;
                        info.reg2Base = regInfo.base;
                        info.reg2Scale = INTEL_SIB_Scale_2;
                    } else {
                        info.hasReg2 = true;
                        info.reg2Str = tmpStr;
                        info.reg2Offset = regInfo.offset;
                        info.reg2RegOp = regInfo.regOp;
                        info.reg2RM = regInfo.RM;
                        info.reg2Index = regInfo.index;
                        info.reg2Base = regInfo.base;
                        info.reg2Scale = INTEL_SIB_Scale_1;
                    }
                } else if (info.reg2Offset==regInfo.offset && info.reg2Scale==INTEL_SIB_Scale_1) {// add together reg2 and the new register
                    info.reg2Scale = INTEL_SIB_Scale_2;
                } else if (info.reg1Offset==regInfo.offset && info.reg2Scale==INTEL_SIB_Scale_1) {// set reg1 to the value in reg2, and add reg1 and the new register together into reg2
                    info.reg1Str = info.reg2Str;
                    info.reg1Offset = info.reg2Offset;
                    info.reg1RegOp = info.reg2RegOp;
                    info.reg1RM = info.reg2RM;
                    info.reg1Index = info.reg2Index;
                    info.reg1Base = info.reg2Base;
                    info.reg2Str = tmpStr;
                    info.reg2Offset = regInfo.offset;
                    info.reg2RegOp = regInfo.regOp;
                    info.reg2RM = regInfo.RM;
                    info.reg2Index = regInfo.index;
                    info.reg2Base = regInfo.base;
                    info.reg2Scale = INTEL_SIB_Scale_2;
                } else if (info.reg1Offset==info.reg2Offset && info.reg2Scale==INTEL_SIB_Scale_1) {//add reg1 and reg2 together into reg2 and set reg1 to the new register
                    info.reg2Scale = INTEL_SIB_Scale_2;
                    info.bit = regInfo.bit;
                    info.reg1Str = tmpStr;
                    info.reg1Offset = regInfo.offset;
                    info.reg1RegOp = regInfo.regOp;
                    info.reg1RM = regInfo.RM;
                    info.reg1Index = regInfo.index;
                    info.reg1Base = regInfo.base;
                } else { info.reason="Too many registers in address."; info.isValid=false; return info; }
                if (arg[cursor]=='+') {cursor++; continue; }
                else break;
            }
        }
        if (arg[cursor]!=']') { info.reason="Ending square bracket not found."; info.isValid=false; return info; }
        else if (arg[cursor+1]!='\0') { info.reason="Junk data found after argument."; info.isValid=false; return info; }
    } else if (arg[0]>='0' && arg[0]<='9') {
        // parse a number
        numberInfo numInfo = processNum(arg,info.strSize);
        if (!numInfo.isValid) { info.reason=numInfo.reason; info.isValid=false; return info;}
        cursor+=numInfo.len;
        if (arg[cursor]!='\0') { info.reason="Junk data found after argument."; info.isValid=false; return info; }
        info.hasNumber=true;
        info.numValue=numInfo.value;
    } else {// most likely either a register or invalid
        // check if it seems like it 
        const char firstChar = std::tolower(arg[0]);
        if (info.strSize<2||info.strSize>3) { info.reason="invalid value \""+std::string(arg)+std::string("\""); info.isValid=false; return info; }
        // parse a register
        registerInfo regInfo;
        if (info.strSize==3) {
            info.reg1Str="aaa";
            info.reg1Str[0]=std::tolower(arg[0]); info.reg1Str[1]=std::toupper(arg[1]); info.reg1Str[2]=std::toupper(arg[2]); // I prefer capitalization like eAX, eCX, rAX, and rCX
            regInfo = processReg(info.reg1Str.c_str(), 3);
            cursor=3u;
        } else {
            info.reg1Str="aa";
            info.reg1Str[0]=std::toupper(arg[0]); info.reg1Str[1]=std::toupper(arg[1]); // I prefer capitalization like AL, AH, and AX
            regInfo = processReg(info.reg1Str.c_str(), 2);
            cursor=2u;
        }
        if (!regInfo.isValid) { info.reason=regInfo.reason; info.isValid=false; return info; }
        if (arg[cursor]!='\0') { info.reason="junk data found after argument";info.isValid=false; return info;}
        info.hasReg1=true;
        info.bit=regInfo.bit;
        info.reg1Offset=regInfo.offset;
        info.reg1RegOp=regInfo.regOp;
        info.reg1RM=regInfo.RM;
    }
    return info;
}

namespace std {
    string to_string(const instructionArgInfo& arg) {
        std::string str = "";
        if (arg.isIndirect) {
            str+='[';
            if (arg.hasReg1) str+=arg.reg1Str;
            if (arg.hasReg2) {
                if (arg.hasReg1) str+='+';
                str+=arg.reg2Str;
                str+='*';
                str+=to_string(SIBScaleToInt(arg.reg2Scale));
            }
            if (arg.hasNumber) {
                if (arg.hasReg1||arg.hasReg2) str+='+';
                str+=to_string(arg.numValue);
            }
            str+=']';
        } else {
            if (arg.hasReg1) {
                str=arg.reg1Str;
            } else {// arg.hasNumber
                str=to_string(arg.numValue);
            }
        }
        return str;
    }
}
std::ostream& operator<<(std::ostream& o, const instructionArgInfo& info) {
    o << "{" << std::endl;
    if (!info.isValid) {
        o << "\tisValid: false" << std::endl;
        o << "\treason: \"" << info.reason << "\"" << std::endl;
        return o << std::endl << "}";
    } else o << "\tisValid: true" << std::endl;
    o << "\tstrSize: " << (int)info.strSize << "," << std::endl;
    o << "\tisIndirect: " << (info.isIndirect?"true,":"false,") << std::endl;
    if (info.hasReg1||info.hasReg2) o << "\tbit: " << (int)info.bit << "," << std::endl;
    if (info.hasReg1) {
        o << "\thasReg1: true" << std::endl;
        o << "\treg1Str: \"" << info.reg1Str << "\"," << std::endl;
        o << "\treg1Offset: " << (int)info.reg1Offset << "," << std::endl;
        o << "\treg1RegOp: " << (int)info.reg1RegOp << "," << std::endl;
        o << "\treg1RM: " << (int)info.reg1RM << "," << std::endl;
        o << "\treg1Index: " << (int)info.reg1Index << "," << std::endl;
        o << "\treg1Base: " << (int)info.reg1Base << "," << std::endl;
    } else o << "\thasReg1: false" << std::endl;
    if (info.hasReg2) {
        o << "\thasReg2: true" << std::endl;
        o << "\treg2Str: \"" << info.reg2Str << "\"," << std::endl;
        o << "\treg2Offset: " << (int)info.reg2Offset << "," << std::endl;
        o << "\treg2RegOp: " << (int)info.reg2RegOp << "," << std::endl;
        o << "\treg2RM: " << (int)info.reg2RM << "," << std::endl;
        o << "\treg2Index: " << (int)info.reg2Index << "," << std::endl;
        o << "\treg2Base: " << (int)info.reg2Base << "," << std::endl;
        o << "\treg2Scale: " << SIBScaleToInt(info.reg2Scale) << "," << std::endl;
    } else o << "\thasReg2: false" << std::endl;
    if (info.hasNumber) {
        o << "\thasNumber: true" << std::endl;
        o << "\tnumValue: " << info.numValue << "," << std::endl;
    } else o << "\thasNumber: false" << std::endl;
    return o << "}";
}

#pragma endregion helper functions

#pragma region instructions

// dst: destination
// src: source
// equivilent to dst = dst + src;
// uses codes 0x00 - 0x05 as well as general instructions 0x80 - 0x83 with modrm OP bits set to 0
// valid forms:
//      ADD reg reg
//      ADD reg num
//      ADD reg [reg] // reference the value in reg as a virtual memory address
//      ADD reg [num] // reference the value 25 as a virtual memory address
//      ADD [reg] reg
//      ADD [reg] num
//      ADD [reg] [reg] // reference the value in reg as a virtual memory address
//      ADD [reg] [num] // reference the value 25 as a virtual memory address
// where reg can be AL,CL,DL,BL,AH,CH,DH,BH, AX,CX,DX,BX,SP,BP,SI,DI, eAX,eCX,eDX,eBX,eSP,eBP,eSI,eDI, rAX,rCX,rDX,rBX,rSP,rBP,rSI, or rDI
// and num can be in the form of 25 (decimal,) 0d25 (also decimal,) 0x19 (hexi-decimal,) or 0b00011001 (binary)
template <typename T>
void ADD(T &receiver, const char* dst, const char* src) {
    if (bitMode==0) return;
    instructionArgInfo dstInfo = processArg(dst);
    if(!dstInfo.isValid) { std::cout << "dst: \"" << dst << "\": \"" << dstInfo.reason << "\"" << std::endl; return; }
    instructionArgInfo srcInfo = processArg(src);
    if(!srcInfo.isValid) { std::cout << "src: \"" << src << "\": \"" << srcInfo.reason << "\"" << std::endl; return; }

    if (dstInfo.isIndirect && srcInfo.isIndirect) { std::cout << "dst and src cannot both be indirect." << std::endl; return; }
    if (dstInfo.hasNumber && !dstInfo.isIndirect) { std::cout << "dst cannot be just a number." << std::endl; return; }
    if ((dstInfo.bit!=0 && dstInfo.bit<32) || (srcInfo.bit!=0 && srcInfo.bit<32)) return;// dont have handling for these yet
    if (!srcInfo.isIndirect) {
        if (srcInfo.hasNumber) {
            // src is just an immutable
            if (dstInfo.isIndirect) {
                if (dstInfo.hasReg2 || (dstInfo.hasNumber && !dstInfo.hasReg1)) {// will requre the SIB byte
                    if (dstInfo.hasReg1 && dstInfo.reg1Base==INTEL_SIB_Base_None) { std::cout << "Register \"rBP\" is an invalid address base" << std::endl; return; }
                    if (dstInfo.hasReg2 && dstInfo.reg2Index==INTEL_SIB_Index_None) { std::cout << "Register \"rSP\" is an invalid address base" << std::endl; return; }
                    // ex: add [reg+reg*scale+num], num
                    if (dstInfo.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
                    pushByte(receiver, INTEL_INSTR_OP1_RMv_Iv);
                    const uint8_t Mod = ((dstInfo.hasNumber && dstInfo.hasReg1)?INTEL_ModRM_MOD_4byteDisp:INTEL_ModRM_MOD_Address);// for some reason, specifically if you have a base in the [base+index*scale], and you want a displacement, you have to specify, just not any other time
                    const uint8_t RegOp = INTEL_ModRM_OP1_ADD_RM_I;
                    const uint8_t ScaleIndex = (dstInfo.hasReg2?(dstInfo.reg2Index|dstInfo.reg2Scale):(INTEL_SIB_Scale_None|INTEL_SIB_Index_None));
                    const uint8_t Base = (dstInfo.hasReg1?dstInfo.reg1Base:INTEL_SIB_Base_None);
                    pushByte(receiver, (Mod|RegOp|INTEL_ModRM_RM_NoDisplace));
                    pushByte(receiver, (ScaleIndex|Base));
                    if (dstInfo.hasNumber || !dstInfo.hasReg1) // displacement only needs to be added if it is desired. Except, for some reason... when there is not base of the formula [base+index*scale]
                        pushWord(receiver, dstInfo.hasNumber?dstInfo.numValue:0, true); // push either the displacement or 0 if there is not displacement
                    pushWord(receiver, srcInfo.numValue, true);
                    std::cout << "add dword ptr " << std::to_string(dstInfo) << ", " << std::to_string(srcInfo) << std::endl;
                } else {
                    if (dstInfo.hasNumber) { // dst is address with a register + a number
                        // ex: add [reg+num], num
                        if (dstInfo.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
                        pushByte(receiver, INTEL_INSTR_OP1_RMv_Iv);
                        pushByte(receiver, (INTEL_ModRM_MOD_4byteDisp|INTEL_ModRM_OP1_ADD_RM_I|dstInfo.reg1RM));
                        pushWord(receiver, dstInfo.numValue, true);
                        pushWord(receiver, srcInfo.numValue, true);
                        std::cout << "add dword ptr " << std::to_string(dstInfo) << ", " << std::to_string(srcInfo) << std::endl;
                    } else { // dst is address with just a register
                        // ex: add [reg], num
                        if (dstInfo.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
                        pushByte(receiver, INTEL_INSTR_OP1_RMv_Iv);
                        pushByte(receiver, (INTEL_ModRM_MOD_Address|INTEL_ModRM_OP1_ADD_RM_I|dstInfo.reg1RM));
                        pushWord(receiver, srcInfo.numValue, true);
                        std::cout << "add dword ptr " << std::to_string(dstInfo) << ", " << std::to_string(srcInfo) << std::endl;
                    }
                }
            } else {// dst cannot be a number, so dst is just a register
                if (dstInfo.reg1Offset==0) { // src is either eAX, or rAX
                    // ex: add eAX, num
                    if (srcInfo.bit==64) pushByte(receiver, INTEL_INSTR64_OperandSz_OVRD);
                    pushByte(receiver, INTEL_INSTR_ADD_eAX_Iv);
                    pushWord(receiver, srcInfo.numValue, true);
                    std::cout << "add " << std::to_string(dstInfo) << ", " << std::to_string(srcInfo) << std::endl;
                }
                // ex: add reg, num
                if (srcInfo.bit==64) pushByte(receiver, INTEL_INSTR64_OperandSz_OVRD);
                pushByte(receiver, INTEL_INSTR_OP1_RMv_Iv);
                pushByte(receiver, (INTEL_ModRM_MOD_Reg|INTEL_ModRM_OP1_ADD_RM_I|dstInfo.reg1RM));
                pushWord(receiver, srcInfo.numValue, true);
                std::cout << "add " << std::to_string(dstInfo) << ", " << std::to_string(srcInfo) << std::endl;
            }
        } else if (srcInfo.hasReg1) {
            // src is just a register
            if (dstInfo.isIndirect) {
                if (dstInfo.hasReg2 || (dstInfo.hasNumber && !dstInfo.hasReg1)) {// will requre the SIB byte
                    if (dstInfo.reg1Base==INTEL_SIB_Base_None) { std::cout << "Register \"" << dstInfo.reg1Str << "\" is an invalid address base" << std::endl; return; }
                    if (dstInfo.reg2Index==INTEL_SIB_Index_None) { std::cout << "Register \"" << dstInfo.reg2Str << "\" is an invalid address index" << std::endl; return; }
                    if (dstInfo.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
                    if (srcInfo.bit==64) pushByte(receiver, INTEL_INSTR64_OperandSz_OVRD);
                    pushByte(receiver, INTEL_INSTR_ADD_RMv_REGv);
                    const uint8_t Mod = ((dstInfo.hasNumber && dstInfo.hasReg1)?INTEL_ModRM_MOD_4byteDisp:INTEL_ModRM_MOD_Address);// for some reason, specifically if you have a base in the [base+index*scale], and you want a displacement, you have to specify, just not any other time
                    const uint8_t RegOp = srcInfo.reg1RegOp;
                    const uint8_t ScaleIndex = (dstInfo.hasReg2?(dstInfo.reg2Index|dstInfo.reg2Scale):(INTEL_SIB_Scale_None|INTEL_SIB_Index_None));
                    const uint8_t Base = (dstInfo.hasReg1?dstInfo.reg1Base:INTEL_SIB_Base_None);
                    pushByte(receiver, (Mod|RegOp|INTEL_ModRM_RM_NoDisplace));
                    pushByte(receiver, (ScaleIndex|Base));
                    if (dstInfo.hasNumber || !dstInfo.hasReg1) // displacement only needs to be added if it is desired. Except, for some reason... when there is not base of the formula [base+index*scale]
                        pushWord(receiver, dstInfo.hasNumber?dstInfo.numValue:0, true); // push either the displacement or 0 if there is not displacement
                    std::cout << "add " << ((srcInfo.bit==64)?"qword":"dword") << " ptr " << std::to_string(dstInfo) << ", " << std::to_string(srcInfo) << std::endl;
                } else {
                    if (dstInfo.hasNumber) { // dst is address with a register + a number
                        // ex: add [reg+num], reg
                        if (dstInfo.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
                        if (srcInfo.bit==64) pushByte(receiver, INTEL_INSTR64_OperandSz_OVRD);
                        pushByte(receiver, INTEL_INSTR_ADD_RMv_REGv);
                        pushByte(receiver, (INTEL_ModRM_MOD_4byteDisp|srcInfo.reg1RegOp|dstInfo.reg1RM));
                        pushWord(receiver, dstInfo.numValue, true);
                        std::cout << "add " << ((srcInfo.bit==64)?"qword":"dword") << " ptr " << std::to_string(dstInfo) << ", " << std::to_string(srcInfo) << std::endl;
                    } else { // dst is address with just a register
                        // ex: add [reg], reg
                        if (dstInfo.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
                        if (srcInfo.bit==64) pushByte(receiver, INTEL_INSTR64_OperandSz_OVRD);
                        pushByte(receiver, INTEL_INSTR_ADD_RMv_REGv);
                        pushByte(receiver, (INTEL_ModRM_MOD_Address|srcInfo.reg1RegOp|dstInfo.reg1RM));
                        std::cout << "add " << ((srcInfo.bit==64)?"qword":"dword") << " ptr " << std::to_string(dstInfo) << ", " << std::to_string(srcInfo) << std::endl;
                    }
                }
            } else {// dst cannot be a number, so dst is just a register
                // ex: add reg, reg
                if (srcInfo.bit==64) pushByte(receiver, INTEL_INSTR64_OperandSz_OVRD);
                pushByte(receiver, INTEL_INSTR_ADD_RMv_REGv);
                pushByte(receiver, (INTEL_ModRM_MOD_Reg|srcInfo.reg1RegOp|dstInfo.reg1RM));
                std::cout << "add " << std::to_string(dstInfo) << ", " << std::to_string(srcInfo) << std::endl;
            }
        }
    } else {
        // src is indirect, which means that dst is just a register. because both cant be indirect, and dst cant be justa number
        if (srcInfo.hasReg2 || (srcInfo.hasNumber && !srcInfo.hasReg1)) {// will requre the SIB byte
            if (srcInfo.reg1Base==INTEL_SIB_Base_None) { std::cout << "Register \"" << srcInfo.reg1Str << "\" is an invalid address base" << std::endl; return; }
            if (srcInfo.reg2Index==INTEL_SIB_Index_None) { std::cout << "Register \"" << srcInfo.reg2Str << "\" is an invalid address index" << std::endl; return; }
            if (srcInfo.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
            if (dstInfo.bit==64) pushByte(receiver, INTEL_INSTR64_OperandSz_OVRD);
            pushByte(receiver, INTEL_INSTR_ADD_REGv_RMv);
            const uint8_t Mod = ((srcInfo.hasNumber && srcInfo.hasReg1)?INTEL_ModRM_MOD_4byteDisp:INTEL_ModRM_MOD_Address);// for some reason, specifically if you have a base in the [base+index*scale], and you want a displacement, you have to specify, just not any other time
            const uint8_t RegOp = dstInfo.reg1RegOp;
            const uint8_t ScaleIndex = (srcInfo.hasReg2?(srcInfo.reg2Index|srcInfo.reg2Scale):(INTEL_SIB_Scale_None|INTEL_SIB_Index_None));
            const uint8_t Base = (srcInfo.hasReg1?srcInfo.reg1Base:INTEL_SIB_Base_None);
            pushByte(receiver, (Mod|RegOp|INTEL_ModRM_RM_NoDisplace));
            pushByte(receiver, (ScaleIndex|Base));
            if (srcInfo.hasNumber || !srcInfo.hasReg1) // displacement only needs to be added if it is desired. Except, for some reason... when there is not base of the formula [base+index*scale]
                pushWord(receiver, srcInfo.hasNumber?srcInfo.numValue:0, true); // push either the displacement or 0 if there is not displacement
            std::cout << "add " << std::to_string(dstInfo) << ", " << ((dstInfo.bit==64)?"qword":"dword") << " ptr " << std::to_string(srcInfo) << std::endl;
        } else {
            if (srcInfo.hasNumber) { // dst is address with a register + a number
                // ex: add reg, [reg+num]
                if (srcInfo.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
                if (dstInfo.bit==64) pushByte(receiver, INTEL_INSTR64_OperandSz_OVRD);
                pushByte(receiver, INTEL_INSTR_ADD_REGv_RMv);
                pushByte(receiver, (INTEL_ModRM_MOD_4byteDisp|dstInfo.reg1RegOp|srcInfo.reg1RM));
                pushWord(receiver, srcInfo.numValue, true);
                std::cout << "add " << std::to_string(dstInfo) << ", " << ((dstInfo.bit==64)?"qword":"dword") << " ptr " << std::to_string(srcInfo) << std::endl;
            } else { // dst is address with just a register
                // ex: add reg, [reg]
                if (srcInfo.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
                if (dstInfo.bit==64) pushByte(receiver, INTEL_INSTR64_OperandSz_OVRD);
                pushByte(receiver, INTEL_INSTR_ADD_REGv_RMv);
                pushByte(receiver, (INTEL_ModRM_MOD_Address|dstInfo.reg1RegOp|srcInfo.reg1RM));
                std::cout << "add " << std::to_string(dstInfo) << ", " << ((dstInfo.bit==64)?"qword":"dword") << " ptr " << std::to_string(srcInfo) << std::endl;
            }
        }
    }
}

template <typename T>
void OR(T &receiver, const char* dst, const char* src) {}
template <typename T>
void ADC(T &receiver, const char* dst, const char* src) {}
template <typename T>
void SBB(T &receiver, const char* dst, const char* src) {}
template <typename T>
void AND(T &receiver, const char* dst, const char* src) {}
template <typename T>
void SUB(T &receiver, const char* dst, const char* src) {}
template <typename T>
void XOR(T &receiver, const char* dst, const char* src) {}
template <typename T>
void CMP(T &receiver, const char* dst, const char* src) {}

/*#pragma region OR
//2 bytes
template <typename T>
void ORb(T &receiver, const char *reg1, const char *reg2) {
    pushByte(receiver, INTEL_INSTR_OR_RMb_REGb);
    pushByte(receiver, INTEL_ModRM_MOD_Reg|RegToModrmReg8(reg1)|RegToModrmRM8(reg2));
}
//2 bytes if reg is eAX, otherwise 3 bytes
template <typename T>
void ORb(T &receiver, const char *reg, const uint8_t &value) {
    uint8_t rm = RegToModrmRM8(reg);
    if (rm == 0) {// reg == eAX
        pushByte(receiver, INTEL_INSTR_OR_AL_Ib);
        pushByte(receiver, value);
    } else {
        pushByte(receiver, INTEL_INSTR_OP1_RMb_Ib);
        pushByte(receiver, INTEL_ModRM_MOD_Reg|INTEL_ModRM_OP1_OR_RM_I|rm);
        pushByte(receiver, value);
    }
}
//2 bytes
template <typename T>
void ORv(T &receiver, const char *reg1, const char *reg2) {
    pushByte(receiver, INTEL_INSTR_OR_RMv_REGv);
    pushByte(receiver, INTEL_ModRM_MOD_Reg|RegToModrmReg32(reg1)|RegToModrmRM32(reg2));
}
// 3,4,5 or 5 bytes
template <typename T>
void ORv(T &receiver, const char *reg, const uint32_t &value) {
    uint8_t rm = RegToModrmRM32(reg);
    if (value>255) {
        if (rm==0) {// reg == eAX and value is larger than a byte can hold
            pushByte(receiver, INTEL_INSTR_OR_eAX_Iv);
            pushWord(receiver, value, true);
        } else {// reg != eAX and value is larger than a byte can hold
            pushByte(receiver, INTEL_INSTR_OP1_RMv_Iv);
            pushByte(receiver, INTEL_ModRM_MOD_Reg|INTEL_ModRM_OP1_OR_RM_I|RegToModrmRM32(reg));
            pushWord(receiver, value, true);
        }
    } else {
        pushByte(receiver, INTEL_INSTR_OP1_RMv_Ib);
        pushByte(receiver, INTEL_ModRM_MOD_Reg|INTEL_ModRM_OP1_OR_RM_I|RegToModrmRM32(reg));
        pushByte(receiver, value);
    }
}
#pragma endregion OR*/


#pragma region template instatiations
//std::ofstream
template void ADD(std::ofstream& receiver, const char *dst, const char *src);
template void OR(std::ofstream& receiver, const char *dst, const char *src);
template void ADC(std::ofstream& receiver, const char *dst, const char *src);
template void SBB(std::ofstream& receiver, const char *dst, const char *src);
template void AND(std::ofstream& receiver, const char *dst, const char *src);
template void SUB(std::ofstream& receiver, const char *dst, const char *src);
template void XOR(std::ofstream& receiver, const char *dst, const char *src);
template void CMP(std::ofstream& receiver, const char *dst, const char *src);

/*template void ORb(std::ofstream &receiver, const char *reg1, const char *reg2);
template void ORb(std::ofstream &receiver, const char *reg, const uint8_t &value);
template void ORv(std::ofstream &receiver, const char *reg1, const char *reg2);
template void ORv(std::ofstream &receiver, const char *reg, const uint32_t &value);
template void ADCb(std::ofstream &receiver, const char *reg1, const char *reg2);
template void ADCb(std::ofstream &receiver, const char *reg, const uint8_t &value);
template void ADCv(std::ofstream &receiver, const char *reg1, const char *reg2);
template void ADCv(std::ofstream &receiver, const char *reg, const uint32_t &value);
template void SBBb(std::ofstream &receiver, const char *reg1, const char *reg2);
template void SBBb(std::ofstream &receiver, const char *reg, const uint8_t &value);
template void SBBv(std::ofstream &receiver, const char *reg1, const char *reg2);
template void SBBv(std::ofstream &receiver, const char *reg, const uint32_t &value);
template void ANDb(std::ofstream &receiver, const char *reg1, const char *reg2);
template void ANDb(std::ofstream &receiver, const char *reg, const uint8_t &value);
template void ANDv(std::ofstream &receiver, const char *reg1, const char *reg2);
template void ANDv(std::ofstream &receiver, const char *reg, const uint32_t &value);
template void SUBb(std::ofstream &receiver, const char *reg1, const char *reg2);
template void SUBb(std::ofstream &receiver, const char *reg, const uint8_t &value);
template void SUBv(std::ofstream &receiver, const char *reg1, const char *reg2);
template void SUBv(std::ofstream &receiver, const char *reg, const uint32_t &value);
template void XORb(std::ofstream &receiver, const char *reg1, const char *reg2);
template void XORb(std::ofstream &receiver, const char *reg, const uint8_t &value);
template void XORv(std::ofstream &receiver, const char *reg1, const char *reg2);
template void XORv(std::ofstream &receiver, const char *reg, const uint32_t &value);
template void CMPb(std::ofstream &receiver, const char *reg1, const char *reg2);
template void CMPb(std::ofstream &receiver, const char *reg, const uint8_t &value);
template void CMPv(std::ofstream &receiver, const char *reg1, const char *reg2);
template void CMPv(std::ofstream &receiver, const char *reg, const uint32_t &value);
template void INCb(std::ofstream &receiver, const char *reg);
template void INCv(std::ofstream &receiver, const char *reg);
template void DECb(std::ofstream &receiver, const char *reg);
template void DECv(std::ofstream &receiver, const char *reg);
template void CALL(std::ofstream &receiver, const uint32_t &value);
template void CALL(std::ofstream &receiver, const char *reg);
template void JMPb(std::ofstream &receiver, const uint8_t &value);
template void JMPv(std::ofstream &receiver, const uint32_t &value);
template void JMPv(std::ofstream &receiver, const char *reg);
template void PUSHb(std::ofstream &receiver, const uint8_t &value);
template void PUSHv(std::ofstream &receiver, const uint32_t &value);
template void PUSHv(std::ofstream &receiver, const char *reg);
template void POP(std::ofstream &receiver, const char *reg);
template void JOV(std::ofstream &receiver, const uint8_t &value);
template void JNOV(std::ofstream &receiver, const uint8_t &value);
template void JULT(std::ofstream &receiver, const uint8_t &value);
template void JUGE(std::ofstream &receiver, const uint8_t &value);
template void JET(std::ofstream &receiver, const uint8_t &value);
template void JNE(std::ofstream &receiver, const uint8_t &value);
template void JULE(std::ofstream &receiver, const uint8_t &value);
template void JUGT(std::ofstream &receiver, const uint8_t &value);
template void JS(std::ofstream &receiver, const uint8_t &value);
template void JNS(std::ofstream &receiver, const uint8_t &value);
template void JP(std::ofstream &receiver, const uint8_t &value);
template void JNP(std::ofstream &receiver, const uint8_t &value);
template void JLT(std::ofstream &receiver, const uint8_t &value);
template void JGE(std::ofstream &receiver, const uint8_t &value);
template void JLE(std::ofstream &receiver, const uint8_t &value);
template void JGT(std::ofstream &receiver, const uint8_t &value);
template void JCXZ(std::ofstream &receiver, const uint8_t &value);
template void NOP(std::ofstream &receiver);
template void XCHGb(std::ofstream &receiver, const char *reg1, const char *reg2);
template void XCHGv(std::ofstream &receiver, const char *reg1, const char *reg2);
template void MOVb(std::ofstream &receiver, const char *reg1, const char *reg2);
template void MOVv(std::ofstream &receiver, const char *reg1, const char *reg2);
template void MOVb(std::ofstream &receiver, const char *reg, const uint8_t &value);
template void MOVv(std::ofstream &receiver, const char *reg, const uint32_t &value);
template void RETN(std::ofstream &receiver, const uint16_t &value);
template void RETN(std::ofstream &receiver);
template void RETF(std::ofstream &receiver, const uint16_t &value);
template void RETF(std::ofstream &receiver);
template void INT(std::ofstream &receiver);
template void INT(std::ofstream &receiver, const uint8_t &value);
template void LOOPNE(std::ofstream &receiver, const uint8_t &value);
template void LOOPET(std::ofstream &receiver, const uint8_t &value);
template void LOOP(std::ofstream &receiver, const uint8_t &value);
template void HLT(std::ofstream &receiver);
template void TESTb(std::ofstream &receiver, const char *reg1, const uint8_t &value);
template void TESTv(std::ofstream &receiver, const char *reg1, const uint32_t &value);
template void NOTb(std::ofstream &receiver, const char *reg);
template void NOTv(std::ofstream &receiver, const char *reg);
template void NEGb(std::ofstream &receiver, const char *reg);
template void NEGv(std::ofstream &receiver, const char *reg);
template void CLC(std::ofstream &receiver);
template void STC(std::ofstream &receiver);
template void CLI(std::ofstream &receiver);
template void STI(std::ofstream &receiver);
template void CLD(std::ofstream &receiver);
template void STD(std::ofstream &receiver);*/

//std::vector<uint8_t>
template void ADD(std::vector<uint8_t>& receiver, const char *dst, const char *src);
template void OR(std::vector<uint8_t>& receiver, const char *dst, const char *src);
template void ADC(std::vector<uint8_t>& receiver, const char *dst, const char *src);
template void SBB(std::vector<uint8_t>& receiver, const char *dst, const char *src);
template void AND(std::vector<uint8_t>& receiver, const char *dst, const char *src);
template void SUB(std::vector<uint8_t>& receiver, const char *dst, const char *src);
template void XOR(std::vector<uint8_t>& receiver, const char *dst, const char *src);
template void CMP(std::vector<uint8_t>& receiver, const char *dst, const char *src);

/*template void ADD(std::vector<uint8_t> &receiver, const char *acc, const char *src);
template void ORb(std::vector<uint8_t> &receiver, const char *reg1, const char *reg2);
template void ORb(std::vector<uint8_t> &receiver, const char *reg, const uint8_t &value);
template void ORv(std::vector<uint8_t> &receiver, const char *reg1, const char *reg2);
template void ORv(std::vector<uint8_t> &receiver, const char *reg, const uint32_t &value);
template void ADCb(std::vector<uint8_t> &receiver, const char *reg1, const char *reg2);
template void ADCb(std::vector<uint8_t> &receiver, const char *reg, const uint8_t &value);
template void ADCv(std::vector<uint8_t> &receiver, const char *reg1, const char *reg2);
template void ADCv(std::vector<uint8_t> &receiver, const char *reg, const uint32_t &value);
template void SBBb(std::vector<uint8_t> &receiver, const char *reg1, const char *reg2);
template void SBBb(std::vector<uint8_t> &receiver, const char *reg, const uint8_t &value);
template void SBBv(std::vector<uint8_t> &receiver, const char *reg1, const char *reg2);
template void SBBv(std::vector<uint8_t> &receiver, const char *reg, const uint32_t &value);
template void ANDb(std::vector<uint8_t> &receiver, const char *reg1, const char *reg2);
template void ANDb(std::vector<uint8_t> &receiver, const char *reg, const uint8_t &value);
template void ANDv(std::vector<uint8_t> &receiver, const char *reg1, const char *reg2);
template void ANDv(std::vector<uint8_t> &receiver, const char *reg, const uint32_t &value);
template void SUBb(std::vector<uint8_t> &receiver, const char *reg1, const char *reg2);
template void SUBb(std::vector<uint8_t> &receiver, const char *reg, const uint8_t &value);
template void SUBv(std::vector<uint8_t> &receiver, const char *reg1, const char *reg2);
template void SUBv(std::vector<uint8_t> &receiver, const char *reg, const uint32_t &value);
template void XORb(std::vector<uint8_t> &receiver, const char *reg1, const char *reg2);
template void XORb(std::vector<uint8_t> &receiver, const char *reg, const uint8_t &value);
template void XORv(std::vector<uint8_t> &receiver, const char *reg1, const char *reg2);
template void XORv(std::vector<uint8_t> &receiver, const char *reg, const uint32_t &value);
template void CMPb(std::vector<uint8_t> &receiver, const char *reg1, const char *reg2);
template void CMPb(std::vector<uint8_t> &receiver, const char *reg, const uint8_t &value);
template void CMPv(std::vector<uint8_t> &receiver, const char *reg1, const char *reg2);
template void CMPv(std::vector<uint8_t> &receiver, const char *reg, const uint32_t &value);
template void INCb(std::vector<uint8_t> &receiver, const char *reg);
template void INCv(std::vector<uint8_t> &receiver, const char *reg);
template void DECb(std::vector<uint8_t> &receiver, const char *reg);
template void DECv(std::vector<uint8_t> &receiver, const char *reg);
template void CALL(std::vector<uint8_t> &receiver, const uint32_t &value);
template void CALL(std::vector<uint8_t> &receiver, const char *reg);
template void JMPb(std::vector<uint8_t> &receiver, const uint8_t &value);
template void JMPv(std::vector<uint8_t> &receiver, const uint32_t &value);
template void JMPv(std::vector<uint8_t> &receiver, const char *reg);
template void PUSHb(std::vector<uint8_t> &receiver, const uint8_t &value);
template void PUSHv(std::vector<uint8_t> &receiver, const uint32_t &value);
template void PUSHv(std::vector<uint8_t> &receiver, const char *reg);
template void POP(std::vector<uint8_t> &receiver, const char *reg);
template void JOV(std::vector<uint8_t> &receiver, const uint8_t &value);
template void JNOV(std::vector<uint8_t> &receiver, const uint8_t &value);
template void JULT(std::vector<uint8_t> &receiver, const uint8_t &value);
template void JUGE(std::vector<uint8_t> &receiver, const uint8_t &value);
template void JET(std::vector<uint8_t> &receiver, const uint8_t &value);
template void JNE(std::vector<uint8_t> &receiver, const uint8_t &value);
template void JULE(std::vector<uint8_t> &receiver, const uint8_t &value);
template void JUGT(std::vector<uint8_t> &receiver, const uint8_t &value);
template void JS(std::vector<uint8_t> &receiver, const uint8_t &value);
template void JNS(std::vector<uint8_t> &receiver, const uint8_t &value);
template void JP(std::vector<uint8_t> &receiver, const uint8_t &value);
template void JNP(std::vector<uint8_t> &receiver, const uint8_t &value);
template void JLT(std::vector<uint8_t> &receiver, const uint8_t &value);
template void JGE(std::vector<uint8_t> &receiver, const uint8_t &value);
template void JLE(std::vector<uint8_t> &receiver, const uint8_t &value);
template void JGT(std::vector<uint8_t> &receiver, const uint8_t &value);
template void JCXZ(std::vector<uint8_t> &receiver, const uint8_t &value);
template void NOP(std::vector<uint8_t> &receiver);
template void XCHGb(std::vector<uint8_t> &receiver, const char *reg1, const char *reg2);
template void XCHGv(std::vector<uint8_t> &receiver, const char *reg1, const char *reg2);
template void MOVb(std::vector<uint8_t> &receiver, const char *reg1, const char *reg2);
template void MOVv(std::vector<uint8_t> &receiver, const char *reg1, const char *reg2);
template void MOVb(std::vector<uint8_t> &receiver, const char *reg, const uint8_t &value);
template void MOVv(std::vector<uint8_t> &receiver, const char *reg, const uint32_t &value);
template void RETN(std::vector<uint8_t> &receiver, const uint16_t &value);
template void RETN(std::vector<uint8_t> &receiver);
template void RETF(std::vector<uint8_t> &receiver, const uint16_t &value);
template void RETF(std::vector<uint8_t> &receiver);
template void INT(std::vector<uint8_t> &receiver);
template void INT(std::vector<uint8_t> &receiver, const uint8_t &value);
template void LOOPNE(std::vector<uint8_t> &receiver, const uint8_t &value);
template void LOOPET(std::vector<uint8_t> &receiver, const uint8_t &value);
template void LOOP(std::vector<uint8_t> &receiver, const uint8_t &value);
template void HLT(std::vector<uint8_t> &receiver);
template void TESTb(std::vector<uint8_t> &receiver, const char *reg1, const uint8_t &value);
template void TESTv(std::vector<uint8_t> &receiver, const char *reg1, const uint32_t &value);
template void NOTb(std::vector<uint8_t> &receiver, const char *reg);
template void NOTv(std::vector<uint8_t> &receiver, const char *reg);
template void NEGb(std::vector<uint8_t> &receiver, const char *reg);
template void NEGv(std::vector<uint8_t> &receiver, const char *reg);
template void CLC(std::vector<uint8_t> &receiver);
template void STC(std::vector<uint8_t> &receiver);
template void CLI(std::vector<uint8_t> &receiver);
template void STI(std::vector<uint8_t> &receiver);
template void CLD(std::vector<uint8_t> &receiver);
template void STD(std::vector<uint8_t> &receiver);*/

//ElfSegmentHandler *
template void ADD(ElfSegmentHandler*& receiver, const char *dst, const char *src);
template void OR(ElfSegmentHandler*& receiver, const char *dst, const char *src);
template void ADC(ElfSegmentHandler*& receiver, const char *dst, const char *src);
template void SBB(ElfSegmentHandler*& receiver, const char *dst, const char *src);
template void AND(ElfSegmentHandler*& receiver, const char *dst, const char *src);
template void SUB(ElfSegmentHandler*& receiver, const char *dst, const char *src);
template void XOR(ElfSegmentHandler*& receiver, const char *dst, const char *src);
template void CMP(ElfSegmentHandler*& receiver, const char *dst, const char *src);

/*template void ADD(ElfSegmentHandler *&receiver, const char *acc, const char *src);
template void ORb(ElfSegmentHandler *&receiver, const char *reg1, const char *reg2);
template void ORb(ElfSegmentHandler *&receiver, const char *reg, const uint8_t &value);
template void ORv(ElfSegmentHandler *&receiver, const char *reg1, const char *reg2);
template void ORv(ElfSegmentHandler *&receiver, const char *reg, const uint32_t &value);
template void ADCb(ElfSegmentHandler *&receiver, const char *reg1, const char *reg2);
template void ADCb(ElfSegmentHandler *&receiver, const char *reg, const uint8_t &value);
template void ADCv(ElfSegmentHandler *&receiver, const char *reg1, const char *reg2);
template void ADCv(ElfSegmentHandler *&receiver, const char *reg, const uint32_t &value);
template void SBBb(ElfSegmentHandler *&receiver, const char *reg1, const char *reg2);
template void SBBb(ElfSegmentHandler *&receiver, const char *reg, const uint8_t &value);
template void SBBv(ElfSegmentHandler *&receiver, const char *reg1, const char *reg2);
template void SBBv(ElfSegmentHandler *&receiver, const char *reg, const uint32_t &value);
template void ANDb(ElfSegmentHandler *&receiver, const char *reg1, const char *reg2);
template void ANDb(ElfSegmentHandler *&receiver, const char *reg, const uint8_t &value);
template void ANDv(ElfSegmentHandler *&receiver, const char *reg1, const char *reg2);
template void ANDv(ElfSegmentHandler *&receiver, const char *reg, const uint32_t &value);
template void SUBb(ElfSegmentHandler *&receiver, const char *reg1, const char *reg2);
template void SUBb(ElfSegmentHandler *&receiver, const char *reg, const uint8_t &value);
template void SUBv(ElfSegmentHandler *&receiver, const char *reg1, const char *reg2);
template void SUBv(ElfSegmentHandler *&receiver, const char *reg, const uint32_t &value);
template void XORb(ElfSegmentHandler *&receiver, const char *reg1, const char *reg2);
template void XORb(ElfSegmentHandler *&receiver, const char *reg, const uint8_t &value);
template void XORv(ElfSegmentHandler *&receiver, const char *reg1, const char *reg2);
template void XORv(ElfSegmentHandler *&receiver, const char *reg, const uint32_t &value);
template void CMPb(ElfSegmentHandler *&receiver, const char *reg1, const char *reg2);
template void CMPb(ElfSegmentHandler *&receiver, const char *reg, const uint8_t &value);
template void CMPv(ElfSegmentHandler *&receiver, const char *reg1, const char *reg2);
template void CMPv(ElfSegmentHandler *&receiver, const char *reg, const uint32_t &value);
template void INCb(ElfSegmentHandler *&receiver, const char *reg);
template void INCv(ElfSegmentHandler *&receiver, const char *reg);
template void DECb(ElfSegmentHandler *&receiver, const char *reg);
template void DECv(ElfSegmentHandler *&receiver, const char *reg);
template void CALL(ElfSegmentHandler *&receiver, const uint32_t &value);
template void CALL(ElfSegmentHandler *&receiver, const char *reg);
template void JMPv(ElfSegmentHandler *&receiver, const uint32_t &value);
template void JMPv(ElfSegmentHandler *&receiver, const char *reg);
template void JMPb(ElfSegmentHandler *&receiver, const uint8_t &value);
template void PUSHb(ElfSegmentHandler *&receiver, const uint8_t &value);
template void PUSHv(ElfSegmentHandler *&receiver, const uint32_t &value);
template void PUSHv(ElfSegmentHandler *&receiver, const char *reg);
template void POP(ElfSegmentHandler *&receiver, const char *reg);
template void JOV(ElfSegmentHandler *&receiver, const uint8_t &value);
template void JNOV(ElfSegmentHandler *&receiver, const uint8_t &value);
template void JULT(ElfSegmentHandler *&receiver, const uint8_t &value);
template void JUGE(ElfSegmentHandler *&receiver, const uint8_t &value);
template void JET(ElfSegmentHandler *&receiver, const uint8_t &value);
template void JNE(ElfSegmentHandler *&receiver, const uint8_t &value);
template void JULE(ElfSegmentHandler *&receiver, const uint8_t &value);
template void JUGT(ElfSegmentHandler *&receiver, const uint8_t &value);
template void JS(ElfSegmentHandler *&receiver, const uint8_t &value);
template void JNS(ElfSegmentHandler *&receiver, const uint8_t &value);
template void JP(ElfSegmentHandler *&receiver, const uint8_t &value);
template void JNP(ElfSegmentHandler *&receiver, const uint8_t &value);
template void JLT(ElfSegmentHandler *&receiver, const uint8_t &value);
template void JGE(ElfSegmentHandler *&receiver, const uint8_t &value);
template void JLE(ElfSegmentHandler *&receiver, const uint8_t &value);
template void JGT(ElfSegmentHandler *&receiver, const uint8_t &value);
template void JCXZ(ElfSegmentHandler *&receiver, const uint8_t &value);
template void NOP(ElfSegmentHandler *&receiver);
template void XCHGb(ElfSegmentHandler *&receiver, const char *reg1, const char *reg2);
template void XCHGv(ElfSegmentHandler *&receiver, const char *reg1, const char *reg2);
template void MOVb(ElfSegmentHandler *&receiver, const char *reg1, const char *reg2);
template void MOVv(ElfSegmentHandler *&receiver, const char *reg1, const char *reg2);
template void MOVb(ElfSegmentHandler *&receiver, const char *reg, const uint8_t &value);
template void MOVv(ElfSegmentHandler *&receiver, const char *reg, const uint32_t &value);
template void RETN(ElfSegmentHandler *&receiver, const uint16_t &value);
template void RETN(ElfSegmentHandler *&receiver);
template void RETF(ElfSegmentHandler *&receiver, const uint16_t &value);
template void RETF(ElfSegmentHandler *&receiver);
template void INT(ElfSegmentHandler *&receiver);
template void INT(ElfSegmentHandler *&receiver, const uint8_t &value);
template void LOOPNE(ElfSegmentHandler *&receiver, const uint8_t &value);
template void LOOPET(ElfSegmentHandler *&receiver, const uint8_t &value);
template void LOOP(ElfSegmentHandler *&receiver, const uint8_t &value);
template void HLT(ElfSegmentHandler *&receiver);
template void TESTb(ElfSegmentHandler *&receiver, const char *reg1, const uint8_t &value);
template void TESTv(ElfSegmentHandler *&receiver, const char *reg1, const uint32_t &value);
template void NOTb(ElfSegmentHandler *&receiver, const char *reg);
template void NOTv(ElfSegmentHandler *&receiver, const char *reg);
template void NEGb(ElfSegmentHandler *&receiver, const char *reg);
template void NEGv(ElfSegmentHandler *&receiver, const char *reg);
template void CLC(ElfSegmentHandler *&receiver);
template void STC(ElfSegmentHandler *&receiver);
template void CLI(ElfSegmentHandler *&receiver);
template void STI(ElfSegmentHandler *&receiver);
template void CLD(ElfSegmentHandler *&receiver);
template void STD(ElfSegmentHandler *&receiver);*/

//Pe32SectionHandler *
template void ADD(Pe32SectionHandler*& receiver, const char *dst, const char *src);
template void OR(Pe32SectionHandler*& receiver, const char *dst, const char *src);
template void ADC(Pe32SectionHandler*& receiver, const char *dst, const char *src);
template void SBB(Pe32SectionHandler*& receiver, const char *dst, const char *src);
template void AND(Pe32SectionHandler*& receiver, const char *dst, const char *src);
template void SUB(Pe32SectionHandler*& receiver, const char *dst, const char *src);
template void XOR(Pe32SectionHandler*& receiver, const char *dst, const char *src);
template void CMP(Pe32SectionHandler*& receiver, const char *dst, const char *src);

/*template void ADD(Pe32SectionHandler *&receiver, const char *acc, const char *src);
template void ORb(Pe32SectionHandler *&receiver, const char *reg1, const char *reg2);
template void ORb(Pe32SectionHandler *&receiver, const char *reg, const uint8_t &value);
template void ORv(Pe32SectionHandler *&receiver, const char *reg1, const char *reg2);
template void ORv(Pe32SectionHandler *&receiver, const char *reg, const uint32_t &value);
template void ADCb(Pe32SectionHandler *&receiver, const char *reg1, const char *reg2);
template void ADCb(Pe32SectionHandler *&receiver, const char *reg, const uint8_t &value);
template void ADCv(Pe32SectionHandler *&receiver, const char *reg1, const char *reg2);
template void ADCv(Pe32SectionHandler *&receiver, const char *reg, const uint32_t &value);
template void SBBb(Pe32SectionHandler *&receiver, const char *reg1, const char *reg2);
template void SBBb(Pe32SectionHandler *&receiver, const char *reg, const uint8_t &value);
template void SBBv(Pe32SectionHandler *&receiver, const char *reg1, const char *reg2);
template void SBBv(Pe32SectionHandler *&receiver, const char *reg, const uint32_t &value);
template void ANDb(Pe32SectionHandler *&receiver, const char *reg1, const char *reg2);
template void ANDb(Pe32SectionHandler *&receiver, const char *reg, const uint8_t &value);
template void ANDv(Pe32SectionHandler *&receiver, const char *reg1, const char *reg2);
template void ANDv(Pe32SectionHandler *&receiver, const char *reg, const uint32_t &value);
template void SUBb(Pe32SectionHandler *&receiver, const char *reg1, const char *reg2);
template void SUBb(Pe32SectionHandler *&receiver, const char *reg, const uint8_t &value);
template void SUBv(Pe32SectionHandler *&receiver, const char *reg1, const char *reg2);
template void SUBv(Pe32SectionHandler *&receiver, const char *reg, const uint32_t &value);
template void XORb(Pe32SectionHandler *&receiver, const char *reg1, const char *reg2);
template void XORb(Pe32SectionHandler *&receiver, const char *reg, const uint8_t &value);
template void XORv(Pe32SectionHandler *&receiver, const char *reg1, const char *reg2);
template void XORv(Pe32SectionHandler *&receiver, const char *reg, const uint32_t &value);
template void CMPb(Pe32SectionHandler *&receiver, const char *reg1, const char *reg2);
template void CMPb(Pe32SectionHandler *&receiver, const char *reg, const uint8_t &value);
template void CMPv(Pe32SectionHandler *&receiver, const char *reg1, const char *reg2);
template void CMPv(Pe32SectionHandler *&receiver, const char *reg, const uint32_t &value);
template void INCb(Pe32SectionHandler *&receiver, const char *reg);
template void INCv(Pe32SectionHandler *&receiver, const char *reg);
template void DECb(Pe32SectionHandler *&receiver, const char *reg);
template void DECv(Pe32SectionHandler *&receiver, const char *reg);
template void CALL(Pe32SectionHandler *&receiver, const uint32_t &value);
template void CALL(Pe32SectionHandler *&receiver, const char *reg);
template void JMPv(Pe32SectionHandler *&receiver, const uint32_t &value);
template void JMPv(Pe32SectionHandler *&receiver, const char *reg);
template void JMPb(Pe32SectionHandler *&receiver, const uint8_t &value);
template void PUSHb(Pe32SectionHandler *&receiver, const uint8_t &value);
template void PUSHv(Pe32SectionHandler *&receiver, const uint32_t &value);
template void PUSHv(Pe32SectionHandler *&receiver, const char *reg);
template void POP(Pe32SectionHandler *&receiver, const char *reg);
template void JOV(Pe32SectionHandler *&receiver, const uint8_t &value);
template void JNOV(Pe32SectionHandler *&receiver, const uint8_t &value);
template void JULT(Pe32SectionHandler *&receiver, const uint8_t &value);
template void JUGE(Pe32SectionHandler *&receiver, const uint8_t &value);
template void JET(Pe32SectionHandler *&receiver, const uint8_t &value);
template void JNE(Pe32SectionHandler *&receiver, const uint8_t &value);
template void JULE(Pe32SectionHandler *&receiver, const uint8_t &value);
template void JUGT(Pe32SectionHandler *&receiver, const uint8_t &value);
template void JS(Pe32SectionHandler *&receiver, const uint8_t &value);
template void JNS(Pe32SectionHandler *&receiver, const uint8_t &value);
template void JP(Pe32SectionHandler *&receiver, const uint8_t &value);
template void JNP(Pe32SectionHandler *&receiver, const uint8_t &value);
template void JLT(Pe32SectionHandler *&receiver, const uint8_t &value);
template void JGE(Pe32SectionHandler *&receiver, const uint8_t &value);
template void JLE(Pe32SectionHandler *&receiver, const uint8_t &value);
template void JGT(Pe32SectionHandler *&receiver, const uint8_t &value);
template void JCXZ(Pe32SectionHandler *&receiver, const uint8_t &value);
template void NOP(Pe32SectionHandler *&receiver);
template void XCHGb(Pe32SectionHandler *&receiver, const char *reg1, const char *reg2);
template void XCHGv(Pe32SectionHandler *&receiver, const char *reg1, const char *reg2);
template void MOVb(Pe32SectionHandler *&receiver, const char *reg1, const char *reg2);
template void MOVv(Pe32SectionHandler *&receiver, const char *reg1, const char *reg2);
template void MOVb(Pe32SectionHandler *&receiver, const char *reg, const uint8_t &value);
template void MOVv(Pe32SectionHandler *&receiver, const char *reg, const uint32_t &value);
template void RETN(Pe32SectionHandler *&receiver, const uint16_t &value);
template void RETN(Pe32SectionHandler *&receiver);
template void RETF(Pe32SectionHandler *&receiver, const uint16_t &value);
template void RETF(Pe32SectionHandler *&receiver);
template void INT(Pe32SectionHandler *&receiver);
template void INT(Pe32SectionHandler *&receiver, const uint8_t &value);
template void LOOPNE(Pe32SectionHandler *&receiver, const uint8_t &value);
template void LOOPET(Pe32SectionHandler *&receiver, const uint8_t &value);
template void LOOP(Pe32SectionHandler *&receiver, const uint8_t &value);
template void HLT(Pe32SectionHandler *&receiver);
template void TESTb(Pe32SectionHandler *&receiver, const char *reg1, const uint8_t &value);
template void TESTv(Pe32SectionHandler *&receiver, const char *reg1, const uint32_t &value);
template void NOTb(Pe32SectionHandler *&receiver, const char *reg);
template void NOTv(Pe32SectionHandler *&receiver, const char *reg);
template void NEGb(Pe32SectionHandler *&receiver, const char *reg);
template void NEGv(Pe32SectionHandler *&receiver, const char *reg);
template void CLC(Pe32SectionHandler *&receiver);
template void STC(Pe32SectionHandler *&receiver);
template void CLI(Pe32SectionHandler *&receiver);
template void STI(Pe32SectionHandler *&receiver);
template void CLD(Pe32SectionHandler *&receiver);
template void STD(Pe32SectionHandler *&receiver);*/

//Pe64SectionHandler *
template void ADD(Pe64SectionHandler*& receiver, const char *dst, const char *src);
template void OR(Pe64SectionHandler*& receiver, const char *dst, const char *src);
template void ADC(Pe64SectionHandler*& receiver, const char *dst, const char *src);
template void SBB(Pe64SectionHandler*& receiver, const char *dst, const char *src);
template void AND(Pe64SectionHandler*& receiver, const char *dst, const char *src);
template void SUB(Pe64SectionHandler*& receiver, const char *dst, const char *src);
template void XOR(Pe64SectionHandler*& receiver, const char *dst, const char *src);
template void CMP(Pe64SectionHandler*& receiver, const char *dst, const char *src);

/*template void ADD(Pe64SectionHandler *&receiver, const char *acc, const char *src);
template void ORb(Pe64SectionHandler *&receiver, const char *reg1, const char *reg2);
template void ORb(Pe64SectionHandler *&receiver, const char *reg, const uint8_t &value);
template void ORv(Pe64SectionHandler *&receiver, const char *reg1, const char *reg2);
template void ORv(Pe64SectionHandler *&receiver, const char *reg, const uint64_t &value);
template void ADCb(Pe64SectionHandler *&receiver, const char *reg1, const char *reg2);
template void ADCb(Pe64SectionHandler *&receiver, const char *reg, const uint8_t &value);
template void ADCv(Pe64SectionHandler *&receiver, const char *reg1, const char *reg2);
template void ADCv(Pe64SectionHandler *&receiver, const char *reg, const uint64_t &value);
template void SBBb(Pe64SectionHandler *&receiver, const char *reg1, const char *reg2);
template void SBBb(Pe64SectionHandler *&receiver, const char *reg, const uint8_t &value);
template void SBBv(Pe64SectionHandler *&receiver, const char *reg1, const char *reg2);
template void SBBv(Pe64SectionHandler *&receiver, const char *reg, const uint64_t &value);
template void ANDb(Pe64SectionHandler *&receiver, const char *reg1, const char *reg2);
template void ANDb(Pe64SectionHandler *&receiver, const char *reg, const uint8_t &value);
template void ANDv(Pe64SectionHandler *&receiver, const char *reg1, const char *reg2);
template void ANDv(Pe64SectionHandler *&receiver, const char *reg, const uint64_t &value);
template void SUBb(Pe64SectionHandler *&receiver, const char *reg1, const char *reg2);
template void SUBb(Pe64SectionHandler *&receiver, const char *reg, const uint8_t &value);
template void SUBv(Pe64SectionHandler *&receiver, const char *reg1, const char *reg2);
template void SUBv(Pe64SectionHandler *&receiver, const char *reg, const uint64_t &value);
template void XORb(Pe64SectionHandler *&receiver, const char *reg1, const char *reg2);
template void XORb(Pe64SectionHandler *&receiver, const char *reg, const uint8_t &value);
template void XORv(Pe64SectionHandler *&receiver, const char *reg1, const char *reg2);
template void XORv(Pe64SectionHandler *&receiver, const char *reg, const uint64_t &value);
template void CMPb(Pe64SectionHandler *&receiver, const char *reg1, const char *reg2);
template void CMPb(Pe64SectionHandler *&receiver, const char *reg, const uint8_t &value);
template void CMPv(Pe64SectionHandler *&receiver, const char *reg1, const char *reg2);
template void CMPv(Pe64SectionHandler *&receiver, const char *reg, const uint64_t &value);
template void INCb(Pe64SectionHandler *&receiver, const char *reg);
template void INCv(Pe64SectionHandler *&receiver, const char *reg);
template void DECb(Pe64SectionHandler *&receiver, const char *reg);
template void DECv(Pe64SectionHandler *&receiver, const char *reg);
template void CALL(Pe64SectionHandler *&receiver, const uint32_t &value);
template void CALL(Pe64SectionHandler *&receiver, const char *reg);
template void JMPv(Pe64SectionHandler *&receiver, const uint64_t &value);
template void JMPv(Pe64SectionHandler *&receiver, const char *reg);
template void JMPb(Pe64SectionHandler *&receiver, const uint8_t &value);
template void PUSHb(Pe64SectionHandler *&receiver, const uint8_t &value);
template void PUSHv(Pe64SectionHandler *&receiver, const uint32_t &value);
template void PUSHv(Pe64SectionHandler *&receiver, const char *reg);
template void POP(Pe64SectionHandler *&receiver, const char *reg);
template void JOV(Pe64SectionHandler *&receiver, const uint8_t &value);
template void JNOV(Pe64SectionHandler *&receiver, const uint8_t &value);
template void JULT(Pe64SectionHandler *&receiver, const uint8_t &value);
template void JUGE(Pe64SectionHandler *&receiver, const uint8_t &value);
template void JET(Pe64SectionHandler *&receiver, const uint8_t &value);
template void JNE(Pe64SectionHandler *&receiver, const uint8_t &value);
template void JULE(Pe64SectionHandler *&receiver, const uint8_t &value);
template void JUGT(Pe64SectionHandler *&receiver, const uint8_t &value);
template void JS(Pe64SectionHandler *&receiver, const uint8_t &value);
template void JNS(Pe64SectionHandler *&receiver, const uint8_t &value);
template void JP(Pe64SectionHandler *&receiver, const uint8_t &value);
template void JNP(Pe64SectionHandler *&receiver, const uint8_t &value);
template void JLT(Pe64SectionHandler *&receiver, const uint8_t &value);
template void JGE(Pe64SectionHandler *&receiver, const uint8_t &value);
template void JLE(Pe64SectionHandler *&receiver, const uint8_t &value);
template void JGT(Pe64SectionHandler *&receiver, const uint8_t &value);
template void JCXZ(Pe64SectionHandler *&receiver, const uint8_t &value);
template void NOP(Pe64SectionHandler *&receiver);
template void XCHGb(Pe64SectionHandler *&receiver, const char *reg1, const char *reg2);
template void XCHGv(Pe64SectionHandler *&receiver, const char *reg1, const char *reg2);
template void MOVb(Pe64SectionHandler *&receiver, const char *reg1, const char *reg2);
template void MOVv(Pe64SectionHandler *&receiver, const char *reg1, const char *reg2);
template void MOVb(Pe64SectionHandler *&receiver, const char *reg, const uint8_t &value);
template void MOVv(Pe64SectionHandler *&receiver, const char *reg, const uint64_t &value);
template void RETN(Pe64SectionHandler *&receiver, const uint16_t &value);
template void RETN(Pe64SectionHandler *&receiver);
template void RETF(Pe64SectionHandler *&receiver, const uint16_t &value);
template void RETF(Pe64SectionHandler *&receiver);
template void INT(Pe64SectionHandler *&receiver);
template void INT(Pe64SectionHandler *&receiver, const uint8_t &value);
template void LOOPNE(Pe64SectionHandler *&receiver, const uint8_t &value);
template void LOOPET(Pe64SectionHandler *&receiver, const uint8_t &value);
template void LOOP(Pe64SectionHandler *&receiver, const uint8_t &value);
template void HLT(Pe64SectionHandler *&receiver);
template void TESTb(Pe64SectionHandler *&receiver, const char *reg1, const uint8_t &value);
template void TESTv(Pe64SectionHandler *&receiver, const char *reg1, const uint64_t &value);
template void NOTb(Pe64SectionHandler *&receiver, const char *reg);
template void NOTv(Pe64SectionHandler *&receiver, const char *reg);
template void NEGb(Pe64SectionHandler *&receiver, const char *reg);
template void NEGv(Pe64SectionHandler *&receiver, const char *reg);
template void CLC(Pe64SectionHandler *&receiver);
template void STC(Pe64SectionHandler *&receiver);
template void CLI(Pe64SectionHandler *&receiver);
template void STI(Pe64SectionHandler *&receiver);
template void CLD(Pe64SectionHandler *&receiver);
template void STD(Pe64SectionHandler *&receiver);*/
#pragma endregion template instatiations

#pragma endregion instructions
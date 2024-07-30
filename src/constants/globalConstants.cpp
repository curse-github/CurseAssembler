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
    }
}
void padBytes(std::vector<uint8_t> &vector, const uint32_t &numBytes) {
    uint8_t C8[5]{0x00,0x00,0x00,0x00,0x00};
    for (int i = 0; i < numBytes; i++) {
        vector.push_back(C8[i%5]);
    }
}

struct registerInfo {
    bool isValid=true;
    std::string reason="";
    unsigned char bit;
    bool isEip=false;
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
        } else if ((reg[1]=='I')&&(reg[2]=='P')) {
            info.offset=0;
            info.isEip=true;
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
        } else if ((reg[1]=='I')&&(reg[2]=='P')) {
            info.offset=0;
            info.isEip=true;
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
        } else if ((reg[0]=='I')&&(reg[1]=='P')) {
            info.offset=0;
            info.isEip=true;
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
            else {info.reason="1Invalid character '"+num[i+2]+std::string("'."); break;}// if there is a character that is not a number
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
            else {info.reason="1Invalid character '"+chr+std::string("'."); break;}// if there is a character that is not a number
        }
        if (!foundEnd) { if (info.reason.empty()) info.reason="Could not find end of Number"; info.isValid=false; return info; }
    }
    // parse number from the string
    char* p;
    info.value=strtol(numStr.c_str(),&p,numBase);// the character p represents the first character that "strtol" finds that is not valid, or '\0' if it did not find an invalid character
    if (*p!=0) { info.isValid=false; info.reason="2Invalid character '"+*p+std::string("'."); return info; }
    return info;
}

struct instructionArgInfo { // isRegister and isNumber will only ever be true if isIndirect is true
    bool isValid=true;
    std::string reason="";
    unsigned int code=0;
    size_t strSize;
    bool isIndirect=false;
    unsigned char bit=0;
    bool isEip=false;
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
const bool optimizeRegAddition=false;
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
                    if (regInfo.isEip) { info.reason="\""+tmpStr+"\" cannot be multiplied."; info.isValid=false; return info;}
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
                        } else if (optimizeRegAddition) {
                            if (info.reg2Offset==regInfo.offset && info.reg2Scale==intToSIBScale(numInfo.value)) {// add together reg2 and the new register
                                info.reg2Scale = intToSIBScale(numInfo.value*2);
                            } else {
                                if (info.hasReg1) info.reason="Too many registers in address.";
                                else info.reason="More than one register cannot have a scale value.";
                                info.isValid=false; return info;
                            }
                        }
                        if (arg[cursor]=='+') {cursor++; continue; }
                        else break;
                    }
                    //if the scale is 1, it continues to the code below
                }
                if (regInfo.isEip) {
                    if (info.hasReg1 || info.hasReg2) { info.reason="\""+tmpStr+"\" cannot be added to other registers."; info.isValid=false; return info;}
                    info.bit=regInfo.bit;
                    info.isEip=true;
                } else if (!info.hasReg1) {
                    info.hasReg1 = true;
                    info.bit = regInfo.bit;
                    info.reg1Str = tmpStr;
                    info.reg1Offset = regInfo.offset;
                    info.reg1RegOp = regInfo.regOp;
                    info.reg1RM = regInfo.RM;
                    info.reg1Index = regInfo.index;
                    info.reg1Base = regInfo.base;
                } else if (!info.hasReg2) {
                    if (optimizeRegAddition && info.reg1Offset==regInfo.offset) {// remove reg1 and combine into reg2
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
                } else if (optimizeRegAddition) {
                    if (info.reg2Offset==regInfo.offset && info.reg2Scale==INTEL_SIB_Scale_1) {// add together reg2 and the new register
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
                } else { info.reason="Too many registers in address."; info.isValid=false; return info; }
                if (arg[cursor]=='+') {cursor++; continue; }
                else break;
            } else { info.reason="Could not find end of string."; info.isValid=false; return info; }
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
        if (info.strSize<2||info.strSize>3) { info.reason="Invalid value \""+std::string(arg)+std::string("\""); info.isValid=false; return info; }
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
        if (arg[cursor]!='\0') { info.reason="Junk data found after argument";info.isValid=false; return info;}
        if (regInfo.isEip) { info.reason="\""+info.reg1Str+"\" is not valid when not indirect.";info.isValid=false; return info;}
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
            if (arg.isEip) {
                if (arg.bit==16) str+="IP";
                if (arg.bit==32) str+="eIP";
                if (arg.bit==64) str+="rIP";
            } else {
                if (arg.hasReg1) str+=arg.reg1Str;
                if (arg.hasReg2) {
                    if (arg.hasReg1) str+='+';
                    str+=arg.reg2Str;
                    str+='*';
                    str+=to_string(SIBScaleToInt(arg.reg2Scale));
                }
            }
            if (arg.hasNumber) {
                if (arg.isEip||arg.hasReg1||arg.hasReg2) str+='+';
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
    o << "\tisEip: " << (info.isEip?"true,":"false,") << std::endl;
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
//instruction template used for Add, Or, Adc, Sbb, And, Sub, Xor, Cmp
template <typename T>
void AddOrAdcSbbAndSubXorCmp(T &receiver, const char* dst, const char* src, const char* instructionName, const uint8_t& op1Code=INTEL_ModRM_OP1_ADD_RM_I, const uint8_t& RMv_REGv=INTEL_INSTR_ADD_RMv_REGv, const uint8_t& REGv_RMv=INTEL_INSTR_ADD_REGv_RMv, const uint8_t& eAX_Iv=INTEL_INSTR_ADD_eAX_Iv) {
    if (bitMode==0) return;
    instructionArgInfo dstInfo = processArg(dst);
    if(!dstInfo.isValid) { std::cout << "dst: \"" << dst << "\": \"" << dstInfo.reason << "\"" << std::endl; return; }
    instructionArgInfo srcInfo = processArg(src);
    if(!srcInfo.isValid) { std::cout << "src: \"" << src << "\": \"" << srcInfo.reason << "\"" << std::endl; return; }

    if (dstInfo.isIndirect && srcInfo.isIndirect) { std::cout << "dst and src cannot both be indirect." << std::endl; return; }
    if (dstInfo.hasNumber && !dstInfo.isIndirect) { std::cout << "dst cannot be just a number." << std::endl; return; }
    if ((dstInfo.bit!=0 && dstInfo.bit<32) || (srcInfo.bit!=0 && srcInfo.bit<32)) return;// dont have handling for these yet

    if (!srcInfo.isIndirect) {
        // Either specify the register that is the src, or the instruction code for the OP1 general instruction
        // This is done up here because it is used in all 4 out of 5 of the cases below and is the exact same value for each
        const uint8_t RegOp = srcInfo.hasReg1?srcInfo.reg1RegOp:op1Code;
        if (dstInfo.isIndirect) {
            if (dstInfo.isEip) {
                // ex: instr ([rip+0] or [rip+num]), (reg or num)
                if (bitMode==64 && dstInfo.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
                if (srcInfo.hasReg1 && (srcInfo.bit==64)) pushByte(receiver, INTEL_INSTR64_OperandSz_OVRD);
                pushByte(receiver, srcInfo.hasReg1?RMv_REGv:(srcInfo.numValue>=128?INTEL_INSTR_OP1_RMv_Iv:INTEL_INSTR_OP1_RMv_Ib));// Specify if src is a register or an immutable
                pushByte(receiver, (INTEL_ModRM_MOD_Address|RegOp|INTEL_ModRM_RM_DisplaceOnly));
                pushWord(receiver, dstInfo.hasNumber?dstInfo.numValue:0, true);
                if (srcInfo.hasNumber) {
                    if (srcInfo.numValue>=128) pushWord(receiver, srcInfo.numValue, true);
                    else pushByte(receiver, srcInfo.numValue);
                }
                std::cout << instructionName << " " << ((srcInfo.bit==64)?"qword":"dword") << " ptr " << std::to_string(dstInfo) << ", " << std::to_string(srcInfo) << std::endl;
            } else if (dstInfo.hasReg2 || (dstInfo.hasNumber && !dstInfo.hasReg1)) {// Will requre the SIB byte
                if (dstInfo.reg1Base==INTEL_SIB_Base_None) { std::cout << "Register \"" << dstInfo.reg1Str << "\" is an invalid address base" << std::endl; return; }
                if (dstInfo.reg2Index==INTEL_SIB_Index_None) { std::cout << "Register \"" << dstInfo.reg2Str << "\" is an invalid address index" << std::endl; return; }
                // ex: instr [reg+reg*scale+num], (reg or num)
                if (bitMode==64 && dstInfo.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
                if (srcInfo.hasReg1 && (srcInfo.bit==64)) pushByte(receiver, INTEL_INSTR64_OperandSz_OVRD);
                pushByte(receiver, srcInfo.hasReg1?RMv_REGv:(srcInfo.numValue>=128?INTEL_INSTR_OP1_RMv_Iv:INTEL_INSTR_OP1_RMv_Ib));// Specify if src is a register or an immutable
                const uint8_t Mod = ((dstInfo.hasNumber && dstInfo.hasReg1)?(dstInfo.numValue>=128?INTEL_ModRM_MOD_4byteDisp:INTEL_ModRM_MOD_1byteDisp):INTEL_ModRM_MOD_Address);// For some reason, specifically if you have a base in the [base+index*scale], and you want a displacement, you have to specify, just not any other time
                const uint8_t ScaleIndex = (dstInfo.hasReg2?(dstInfo.reg2Index|dstInfo.reg2Scale):(INTEL_SIB_Scale_None|INTEL_SIB_Index_None));
                const uint8_t Base = (dstInfo.hasReg1?dstInfo.reg1Base:INTEL_SIB_Base_None);
                pushByte(receiver, (Mod|RegOp|INTEL_ModRM_RM_NoDisplace));
                pushByte(receiver, (ScaleIndex|Base));
                if (dstInfo.hasNumber || !dstInfo.hasReg1) {// Displacement only needs to be added if it is desired. Except, for some reason... when there is not base of the formula [base+index*scale]
                    if (dstInfo.numValue>=128 || !dstInfo.hasReg1) pushWord(receiver, dstInfo.hasNumber?dstInfo.numValue:0, true);// Push either the displacement or 0 if there is not displacement
                    else pushByte(receiver, dstInfo.numValue);
                }
                if (srcInfo.hasNumber) {
                    if (srcInfo.numValue>=128) pushWord(receiver, srcInfo.numValue, true);
                    else pushByte(receiver, srcInfo.numValue);
                }
                std::cout << instructionName << " " << ((srcInfo.bit==64)?"qword":"dword") << " ptr " << std::to_string(dstInfo) << ", " << std::to_string(srcInfo) << std::endl;
            } else {
                // ex: instr ([reg+num] or [reg]), (reg or num)
                if (bitMode==64 && dstInfo.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
                if (srcInfo.hasReg1 && (srcInfo.bit==64)) pushByte(receiver, INTEL_INSTR64_OperandSz_OVRD);
                pushByte(receiver, srcInfo.hasReg1?RMv_REGv:(srcInfo.numValue>=128?INTEL_INSTR_OP1_RMv_Iv:INTEL_INSTR_OP1_RMv_Ib));
                const uint8_t Mod = dstInfo.hasNumber?(dstInfo.numValue>=128?INTEL_ModRM_MOD_4byteDisp:INTEL_ModRM_MOD_1byteDisp):INTEL_ModRM_MOD_Address;
                pushByte(receiver, (Mod|RegOp|dstInfo.reg1RM));
                if (dstInfo.hasNumber) {
                    if (dstInfo.numValue>=128) pushWord(receiver, dstInfo.numValue, true);
                    else pushByte(receiver, dstInfo.numValue);
                }
                if (srcInfo.hasNumber) {
                    if (srcInfo.numValue>=128) pushWord(receiver, srcInfo.numValue, true);
                    else pushByte(receiver, srcInfo.numValue);
                }
                std::cout << instructionName << " " << ((srcInfo.bit==64)?"qword":"dword") << " ptr " << std::to_string(dstInfo) << ", " << std::to_string(srcInfo) << std::endl;
            }
        } else if (dstInfo.reg1Offset==INTEL_REG_OFF_eAX && srcInfo.hasNumber && srcInfo.numValue>128) {
            if (dstInfo.bit==64) pushByte(receiver, INTEL_INSTR64_OperandSz_OVRD);
            pushByte(receiver, eAX_Iv);
            pushWord(receiver, srcInfo.numValue, true);
            std::cout << instructionName << " " << std::to_string(dstInfo) << ", " << std::to_string(srcInfo) << std::endl;
        } else {// dst cannot be a number, so dst is just a register
            // ex: instr reg, (reg or num)
            if (srcInfo.hasReg1 && (srcInfo.bit!=dstInfo.bit)) { std::cout << "Operand size mis-match." << std::endl; return; }
            if ((srcInfo.hasReg1 && (srcInfo.bit==64)) || (dstInfo.bit==64)) pushByte(receiver, INTEL_INSTR64_OperandSz_OVRD);
            pushByte(receiver, srcInfo.hasReg1?RMv_REGv:(srcInfo.numValue>=128?INTEL_INSTR_OP1_RMv_Iv:INTEL_INSTR_OP1_RMv_Ib));
            pushByte(receiver, (INTEL_ModRM_MOD_Reg|RegOp|dstInfo.reg1RM));
            if (srcInfo.hasNumber) {
                if (srcInfo.numValue>=128) pushWord(receiver, srcInfo.numValue, true);
                else pushByte(receiver, srcInfo.numValue);
            }
            std::cout << instructionName << " " << std::to_string(dstInfo) << ", " << std::to_string(srcInfo) << std::endl;
        }
    } else {
        // src is indirect, which means that dst is just a register. Because both cant be indirect, and dst cant be just a number
        if (srcInfo.isEip) {
            // ex: instr reg, ([rip+0] or [rip+num])
            if (bitMode==64 && srcInfo.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
            if (dstInfo.bit==64) pushByte(receiver, INTEL_INSTR64_OperandSz_OVRD);
            pushByte(receiver, REGv_RMv);
            pushByte(receiver, (INTEL_ModRM_MOD_Address|dstInfo.reg1RegOp|INTEL_ModRM_RM_DisplaceOnly));
            pushWord(receiver, srcInfo.hasNumber?srcInfo.numValue:0, true);
            if (dstInfo.hasNumber) {
                if (dstInfo.numValue>=128) pushWord(receiver, dstInfo.numValue, true);
                else pushByte(receiver, dstInfo.numValue);
            }
            std::cout << instructionName << " " << std::to_string(dstInfo) << ", " << ((dstInfo.bit==64)?"qword":"dword") << " ptr " << std::to_string(srcInfo) << std::endl;
        } else if (srcInfo.hasReg2 || (srcInfo.hasNumber && !srcInfo.hasReg1)) {// Will requre the SIB byte
            if (srcInfo.reg1Base==INTEL_SIB_Base_None) { std::cout << "Register \"" << srcInfo.reg1Str << "\" is an invalid address base" << std::endl; return; }
            if (srcInfo.reg2Index==INTEL_SIB_Index_None) { std::cout << "Register \"" << srcInfo.reg2Str << "\" is an invalid address index" << std::endl; return; }
            // ex: instr reg, [reg+reg*scale+num]
            if (bitMode==64 && srcInfo.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
            if (dstInfo.bit==64) pushByte(receiver, INTEL_INSTR64_OperandSz_OVRD);
            pushByte(receiver, REGv_RMv);
            const uint8_t Mod = ((srcInfo.hasNumber && srcInfo.hasReg1)?(srcInfo.numValue>=128?INTEL_ModRM_MOD_4byteDisp:INTEL_ModRM_MOD_1byteDisp):INTEL_ModRM_MOD_Address);// For some reason, specifically if you have a base in the [base+index*scale], and you want a displacement, you have to specify, just not any other time
            const uint8_t RegOp = dstInfo.reg1RegOp;
            const uint8_t ScaleIndex = (srcInfo.hasReg2?(srcInfo.reg2Index|srcInfo.reg2Scale):(INTEL_SIB_Scale_None|INTEL_SIB_Index_None));
            const uint8_t Base = (srcInfo.hasReg1?srcInfo.reg1Base:INTEL_SIB_Base_None);
            pushByte(receiver, (Mod|RegOp|INTEL_ModRM_RM_NoDisplace));
            pushByte(receiver, (ScaleIndex|Base));
            if (srcInfo.hasNumber || !srcInfo.hasReg1) {// Displacement only needs to be added if it is desired. Except, for some reason... when there is not base of the formula [base+index*scale]
                if (srcInfo.numValue>=128 || !srcInfo.hasReg1) pushWord(receiver, srcInfo.hasNumber?srcInfo.numValue:0, true);// Push either the displacement or 0 if there is not displacement
                else pushByte(receiver, srcInfo.numValue);
            }
            std::cout << instructionName << " " << std::to_string(dstInfo) << ", " << ((dstInfo.bit==64)?"qword":"dword") << " ptr " << std::to_string(srcInfo) << std::endl;
        } else {
            // ex: instr reg, ([reg+num] or [reg])
            if (bitMode==64 && srcInfo.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
            if (dstInfo.hasReg1 && (dstInfo.bit==64)) pushByte(receiver, INTEL_INSTR64_OperandSz_OVRD);
            pushByte(receiver, REGv_RMv);
            const uint8_t Mod = srcInfo.hasNumber?(srcInfo.numValue>=128?INTEL_ModRM_MOD_4byteDisp:INTEL_ModRM_MOD_1byteDisp):INTEL_ModRM_MOD_Address;
            pushByte(receiver, (Mod|dstInfo.reg1RegOp|srcInfo.reg1RM));
            if (srcInfo.hasNumber) {
                if (srcInfo.numValue>=128) pushWord(receiver, srcInfo.numValue, true);
                else pushByte(receiver, srcInfo.numValue);
            }
            std::cout << instructionName << " " << std::to_string(dstInfo) << ", " << ((dstInfo.bit==64)?"qword":"dword") << " ptr " << std::to_string(srcInfo) << std::endl;
        }
    }
}

template <typename T>
void ADD(T &receiver, const char* dst, const char* src) {
    AddOrAdcSbbAndSubXorCmp(receiver,dst,src,"ADD",INTEL_ModRM_OP1_ADD_RM_I,INTEL_INSTR_ADD_RMv_REGv,INTEL_INSTR_ADD_REGv_RMv,INTEL_INSTR_ADD_eAX_Iv);
}
template <typename T>
void OR(T &receiver, const char* dst, const char* src) {
    AddOrAdcSbbAndSubXorCmp(receiver,dst,src,"OR",INTEL_ModRM_OP1_OR_RM_I,INTEL_INSTR_OR_RMv_REGv,INTEL_INSTR_OR_REGv_RMv,INTEL_INSTR_OR_eAX_Iv);
}
template <typename T>
void ADC(T &receiver, const char* dst, const char* src) {
    AddOrAdcSbbAndSubXorCmp(receiver,dst,src,"ADC",INTEL_ModRM_OP1_ADC_RM_I,INTEL_INSTR_ADC_RMv_REGv,INTEL_INSTR_ADC_REGv_RMv,INTEL_INSTR_ADC_eAX_Iv);
}
template <typename T>
void SBB(T &receiver, const char* dst, const char* src) {
    AddOrAdcSbbAndSubXorCmp(receiver,dst,src,"SBB",INTEL_ModRM_OP1_SBB_RM_I,INTEL_INSTR_SBB_RMv_REGv,INTEL_INSTR_SBB_REGv_RMv,INTEL_INSTR_SBB_eAX_Iv);
}
template <typename T>
void AND(T &receiver, const char* dst, const char* src) {
    AddOrAdcSbbAndSubXorCmp(receiver,dst,src,"AND",INTEL_ModRM_OP1_AND_RM_I,INTEL_INSTR_AND_RMv_REGv,INTEL_INSTR_AND_REGv_RMv,INTEL_INSTR_AND_eAX_Iv);
}
template <typename T>
void SUB(T &receiver, const char* dst, const char* src) {
    AddOrAdcSbbAndSubXorCmp(receiver,dst,src,"SUB",INTEL_ModRM_OP1_SUB_RM_I,INTEL_INSTR_SUB_RMv_REGv,INTEL_INSTR_SUB_REGv_RMv,INTEL_INSTR_SUB_eAX_Iv);
}
template <typename T>
void XOR(T &receiver, const char* dst, const char* src) {
    AddOrAdcSbbAndSubXorCmp(receiver,dst,src,"XOR",INTEL_ModRM_OP1_XOR_RM_I,INTEL_INSTR_XOR_RMv_REGv,INTEL_INSTR_XOR_REGv_RMv,INTEL_INSTR_XOR_eAX_Iv);
}
template <typename T>
void CMP(T &receiver, const char* dst, const char* src) {
    AddOrAdcSbbAndSubXorCmp(receiver,dst,src,"CMP",INTEL_ModRM_OP1_CMP_RM_I,INTEL_INSTR_CMP_RMv_REGv,INTEL_INSTR_CMP_REGv_RMv,INTEL_INSTR_CMP_eAX_Iv);
}

template <typename T>
void IncDec(T &receiver, const char *arg, const char* instructionName, const uint8_t& op3Code=INTEL_ModRM_OP3_INC_RM, const uint8_t& offsetInstr=INTEL_INSTR32_INCpRv) {
    if (bitMode==0) return;
    instructionArgInfo argInfo = processArg(arg);
    if(!argInfo.isValid) { std::cout << "arg: \"" << arg << "\": \"" << argInfo.reason << "\"" << std::endl; return; }
    
    if (argInfo.hasNumber && !argInfo.isIndirect) { std::cout << "arg cannot be just a number." << std::endl; return; }
    if (argInfo.bit!=0 && argInfo.bit<32) return;// Dont have handling for these yet

    if (argInfo.isEip) {
        // ex: inc/dec ([rip+0] or [rip+num])
        if (bitMode==64 && argInfo.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
        pushByte(receiver, INTEL_INSTR_OP3v);
        pushByte(receiver, (INTEL_ModRM_MOD_Address|op3Code|INTEL_ModRM_RM_DisplaceOnly));
        pushWord(receiver, argInfo.hasNumber?argInfo.numValue:0, true);
        std::cout << instructionName << " " << (argInfo.isIndirect?"dword ptr ":"") << std::to_string(argInfo) << std::endl;
    } else if (bitMode==32 && !argInfo.isIndirect) {// Also implies that that it hasReg1
        // arg is just a register, and you can use the 32 bit instruction based on
        // ex: inc/dec eax thru edi
        pushByte(receiver, offsetInstr+argInfo.reg1Offset);
        std::cout << instructionName << " " << std::to_string(argInfo) << std::endl;
    } else if (argInfo.hasReg2 || (argInfo.hasNumber && !argInfo.hasReg1)) {// requires an SIB byte
        if (argInfo.reg1Base==INTEL_SIB_Base_None) { std::cout << "Register \"" << argInfo.reg1Str << "\" is an invalid address base" << std::endl; return; }
        if (argInfo.reg2Index==INTEL_SIB_Index_None) { std::cout << "Register \"" << argInfo.reg2Str << "\" is an invalid address index" << std::endl; return; }
        // ex: inc/dec [reg+reg*scale+num]
        if (bitMode==64 && argInfo.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
        pushByte(receiver, INTEL_INSTR_OP3v);
        const uint8_t Mod = ((argInfo.hasNumber && argInfo.hasReg1)?((argInfo.numValue>=128)?INTEL_ModRM_MOD_4byteDisp:INTEL_ModRM_MOD_1byteDisp):INTEL_ModRM_MOD_Address);// For some reason, specifically if you have a base in the [base+index*scale], and you want a displacement, you have to specify, just not any other time
        const uint8_t ScaleIndex = (argInfo.hasReg2?(argInfo.reg2Index|argInfo.reg2Scale):(INTEL_SIB_Scale_None|INTEL_SIB_Index_None));
        const uint8_t Base = (argInfo.hasReg1?argInfo.reg1Base:INTEL_SIB_Base_None);
        pushByte(receiver, (Mod|op3Code|INTEL_ModRM_RM_NoDisplace));
        pushByte(receiver, (ScaleIndex|Base));
        if (argInfo.hasNumber || !argInfo.hasReg1) {// Displacement only needs to be added if it is desired. Except, for some reason... when there is not base of the formula [base+index*scale]
            if (argInfo.numValue>=128 || !argInfo.hasReg1) pushWord(receiver, argInfo.hasNumber?argInfo.numValue:0, true); // Push either the displacement or 0 if there is not displacement.
            else pushByte(receiver, argInfo.numValue);
        }
        std::cout << instructionName << " dword ptr " << std::to_string(argInfo) << std::endl;
    } else { // arg is just a register, or indirect but does not require an SIB byte
        // ex: inc/dec (reg or [reg] or [reg+num])
        if (!argInfo.isIndirect && argInfo.bit==64) pushByte(receiver, INTEL_INSTR64_OperandSz_OVRD);
        if (argInfo.isIndirect && bitMode==64 && argInfo.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
        pushByte(receiver, INTEL_INSTR_OP3v);
        const uint8_t Mod = argInfo.isIndirect?(argInfo.hasNumber?((argInfo.numValue>=128)?INTEL_ModRM_MOD_4byteDisp:INTEL_ModRM_MOD_1byteDisp):INTEL_ModRM_MOD_Address):INTEL_ModRM_MOD_Reg;
        pushByte(receiver, ((Mod|op3Code|argInfo.reg1RM)));
        if (argInfo.hasNumber) {
            if (argInfo.numValue>=128) pushWord(receiver, argInfo.numValue, true);
            else pushByte(receiver, argInfo.numValue);
        }
        std::cout << instructionName << " " << (argInfo.isIndirect?"dword ptr ":"") << std::to_string(argInfo) << std::endl;
    }
}
template <typename T>
void INC(T &receiver, const char *arg) {
    IncDec(receiver,arg,"INC",INTEL_ModRM_OP3_INC_RM,INTEL_INSTR32_INCpRv);
}
template <typename T>
void DEC(T &receiver, const char *arg) {
    IncDec(receiver,arg,"DEC",INTEL_ModRM_OP3_DEC_RM,INTEL_INSTR32_DECpRv);
}


template <typename T>
void CallJmp(T &receiver, const char *arg, const char *instructionName, const uint8_t& op3Code=INTEL_ModRM_OP3_CALL_RM, const uint8_t& INSTR_Jv=INTEL_INSTR_CALL_Jv) {
    if (bitMode==0) return;
    instructionArgInfo argInfo = processArg(arg);
    if(!argInfo.isValid) { std::cout << "arg: \"" << arg << "\": \"" << argInfo.reason << "\"" << std::endl; return; }
    
    if (argInfo.hasReg1 && !argInfo.isIndirect) { std::cout << "arg cannot be just a register." << std::endl; return; }
    if (argInfo.bit!=0 && argInfo.bit<32) return;// Dont have handling for these yet

    if (!argInfo.isIndirect) {// arg is just a number
        // ex: call/jmp num
        std::cout << "Just a number as an argument is not currently supported for call." << std::endl;// return;
        if (argInfo.bit==64) pushByte(receiver, INTEL_INSTR64_OperandSz_OVRD);
        pushByte(receiver, INSTR_Jv);
        pushWord(receiver, argInfo.numValue, true);
        std::cout << instructionName << " " << std::to_string(argInfo) << std::endl;
    } else if (argInfo.isEip) {
        // ex: call/jmp ([rip+0] or [rip+num])
        if (bitMode==64 && argInfo.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
        pushByte(receiver, INTEL_INSTR_OP3v);
        pushByte(receiver, (INTEL_ModRM_MOD_Address|op3Code|INTEL_ModRM_RM_DisplaceOnly));
        pushWord(receiver, argInfo.hasNumber?argInfo.numValue:0, true);
        std::cout << instructionName << " " << ((bitMode==64)?"qword":"dword") << " ptr " << std::to_string(argInfo) << std::endl;
    } else if (argInfo.hasReg2 || (argInfo.hasNumber && !argInfo.hasReg1)) {// requires SIB byte
        if (argInfo.reg1Base==INTEL_SIB_Base_None) { std::cout << "Register \"" << argInfo.reg1Str << "\" is an invalid address base" << std::endl; return; }
        if (argInfo.reg2Index==INTEL_SIB_Index_None) { std::cout << "Register \"" << argInfo.reg2Str << "\" is an invalid address index" << std::endl; return; }
        // ex: call/jmp [reg+reg*scale+num]
        if (bitMode==64 && argInfo.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
        pushByte(receiver, INTEL_INSTR_OP3v);
        const uint8_t Mod = ((argInfo.hasNumber && argInfo.hasReg1)?((argInfo.numValue>=128)?INTEL_ModRM_MOD_4byteDisp:INTEL_ModRM_MOD_1byteDisp):INTEL_ModRM_MOD_Address);// For some reason, specifically if you have a base in the [base+index*scale], and you want a displacement, you have to specify, just not any other time
        const uint8_t ScaleIndex = (argInfo.hasReg2?(argInfo.reg2Index|argInfo.reg2Scale):(INTEL_SIB_Scale_None|INTEL_SIB_Index_None));
        const uint8_t Base = (argInfo.hasReg1?argInfo.reg1Base:INTEL_SIB_Base_None);
        pushByte(receiver, (Mod|op3Code|INTEL_ModRM_RM_NoDisplace));
        pushByte(receiver, (ScaleIndex|Base));
        if (argInfo.hasNumber || !argInfo.hasReg1) {// Displacement only needs to be added if it is desired. Except, for some reason... when there is not base of the formula [base+index*scale]
            if (argInfo.numValue>=128 || !argInfo.hasReg1) pushWord(receiver, argInfo.hasNumber?argInfo.numValue:0, true); // Push either the displacement or 0 if there is not displacement.
            else pushByte(receiver, argInfo.numValue);
        }
        std::cout << instructionName << " " << ((bitMode==64)?"qword":"dword") << " ptr " << std::to_string(argInfo) << std::endl;
    } else { // arg is indirect but does not require an SIB byte
        // ex: call/jmp ([num] or [reg] or [reg+num])
        if (bitMode==64 && argInfo.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
        pushByte(receiver, INTEL_INSTR_OP3v);
        const uint8_t Mod = argInfo.hasNumber?((argInfo.numValue>=128)?INTEL_ModRM_MOD_4byteDisp:INTEL_ModRM_MOD_1byteDisp):INTEL_ModRM_MOD_Address;
        pushByte(receiver, ((Mod|op3Code|argInfo.reg1RM)));
        if (argInfo.hasNumber) {
            if (argInfo.numValue>=128) pushWord(receiver, argInfo.numValue, true);
            else pushByte(receiver, argInfo.numValue);
        }
        std::cout << instructionName << " " << ((bitMode==64)?"qword":"dword") << " ptr " << std::to_string(argInfo) << std::endl;
    }
}
template <typename T>
void CALL(T &receiver, const char *arg) {
    CallJmp(receiver,arg,"CALL",INTEL_ModRM_OP3_CALL_RM,INTEL_INSTR_CALL_Jv);
}
/*template <typename T>
void CALLF(T &receiver, const char *arg) {
    if (bitMode==0) return;
    instructionArgInfo argInfo = processArg(arg);
    if(!argInfo.isValid) { std::cout << "arg: \"" << arg << "\": \"" << argInfo.reason << "\"" << std::endl; return; }
    
    if (argInfo.bit!=0 && argInfo.bit<32) return;// Dont have handling for these yet
}*/
template <typename T>
void JMP(T &receiver, const char *arg) {
    CallJmp(receiver,arg,"JMP",INTEL_ModRM_OP3_JMP_RM,INTEL_INSTR_JMP_Jv);
}
/*template <typename T>
void JMPF(T &receiver, const char *arg) {
    if (bitMode==0) return;
    instructionArgInfo argInfo = processArg(arg);
    if(!argInfo.isValid) { std::cout << "arg: \"" << arg << "\": \"" << argInfo.reason << "\"" << std::endl; return; }
    
    if (argInfo.bit!=0 && argInfo.bit<32) return;// Dont have handling for these yet
}*/

template <typename T>
void PUSH(T &receiver, const char *arg) {
    if (bitMode==0) return;
    instructionArgInfo argInfo = processArg(arg);
    if(!argInfo.isValid) { std::cout << "arg: \"" << arg << "\": \"" << argInfo.reason << "\"" << std::endl; return; }

    if (!argInfo.isIndirect && argInfo.bit!=0 && argInfo.bit!=bitMode) { std::cout << "PUSH arguments can only be " << bitMode << " bit." << std::endl; return; }
    if (argInfo.bit!=0 && argInfo.bit<32) return;// Dont have handling for these yet

    if (!argInfo.isIndirect) {
        if (argInfo.hasNumber) { // arg is just a number, and you can use the Iv unstruction
            // ex: push num
            if (argInfo.numValue>=128) {
                pushByte(receiver, INTEL_INSTR_PUSH_Iv);
                pushWord(receiver, argInfo.numValue, true);
            } else {
                pushByte(receiver, INTEL_INSTR_PUSH_Ib);
                pushByte(receiver, argInfo.numValue);
            }
            std::cout << "PUSH " << std::to_string(argInfo) << std::endl;
        } else { // arg is just a register, and you can use the offset based instruction
            // ex: push reg
            pushByte(receiver, INTEL_INSTR_PUSHpRv+argInfo.reg1Offset);
            std::cout << "PUSH " << std::to_string(argInfo) << std::endl;
        }
    } else if (argInfo.isEip) {
        // ex: push ([rip+0] or [rip+num])
        if (bitMode==64 && argInfo.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
        pushByte(receiver, INTEL_INSTR_OP3v);
        pushByte(receiver, (INTEL_ModRM_MOD_Address|INTEL_ModRM_OP3_PUSH_RM|INTEL_ModRM_RM_DisplaceOnly));
        pushWord(receiver, argInfo.hasNumber?argInfo.numValue:0, true);
        std::cout << "PUSH " << ((bitMode==64)?"qword":"dword") << " ptr " << std::to_string(argInfo) << std::endl;
    } else if (argInfo.hasReg2 || (argInfo.hasNumber && !argInfo.hasReg1)) {
        if (argInfo.reg1Base==INTEL_SIB_Base_None) { std::cout << "Register \"" << argInfo.reg1Str << "\" is an invalid address base" << std::endl; return; }
        if (argInfo.reg2Index==INTEL_SIB_Index_None) { std::cout << "Register \"" << argInfo.reg2Str << "\" is an invalid address index" << std::endl; return; }
        // ex: push [reg+reg*scale+num]
        if (bitMode==64 && argInfo.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
        pushByte(receiver, INTEL_INSTR_OP3v);
        const uint8_t Mod = ((argInfo.hasNumber && argInfo.hasReg1)?((argInfo.numValue>=128)?INTEL_ModRM_MOD_4byteDisp:INTEL_ModRM_MOD_1byteDisp):INTEL_ModRM_MOD_Address);// For some reason, specifically if you have a base in the [base+index*scale], and you want a displacement, you have to specify, just not any other time
        const uint8_t ScaleIndex = (argInfo.hasReg2?(argInfo.reg2Index|argInfo.reg2Scale):(INTEL_SIB_Scale_None|INTEL_SIB_Index_None));
        const uint8_t Base = (argInfo.hasReg1?argInfo.reg1Base:INTEL_SIB_Base_None);
        pushByte(receiver, (Mod|INTEL_ModRM_OP3_PUSH_RM|INTEL_ModRM_RM_NoDisplace));
        pushByte(receiver, (ScaleIndex|Base));
        if (argInfo.hasNumber || !argInfo.hasReg1) {// Displacement only needs to be added if it is desired. Except, for some reason... when there is not base of the formula [base+index*scale]
            if (argInfo.numValue>=128 || !argInfo.hasReg1) pushWord(receiver, argInfo.hasNumber?argInfo.numValue:0, true); // Push either the displacement or 0 if there is not displacement.
            else pushByte(receiver, argInfo.numValue);
        }
        std::cout << "PUSH " << ((bitMode==64)?"qword":"dword") << " ptr " << std::to_string(argInfo) << std::endl;
    } else { // arg is indirect but does not require an SIB byte
        // ex: push ([reg] or [reg+num])
        if (bitMode==64 && argInfo.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
        pushByte(receiver, INTEL_INSTR_OP3v);
        const uint8_t Mod = argInfo.hasNumber?((argInfo.numValue>=128)?INTEL_ModRM_MOD_4byteDisp:INTEL_ModRM_MOD_1byteDisp):INTEL_ModRM_MOD_Address;
        pushByte(receiver, ((Mod|INTEL_ModRM_OP3_PUSH_RM|argInfo.reg1RM)));
        if (argInfo.hasNumber)  {
            if (argInfo.numValue>=128) pushWord(receiver, argInfo.numValue, true);
            else pushByte(receiver, argInfo.numValue);
        }
        std::cout << "PUSH " << ((bitMode==64)?"qword":"dword") << " ptr " << std::to_string(argInfo) << std::endl;
    }
}
template <typename T>
void POP(T &receiver, const char *arg) {
    if (bitMode==0) return;
    instructionArgInfo argInfo = processArg(arg);
    if(!argInfo.isValid) { std::cout << "arg: \"" << arg << "\": \"" << argInfo.reason << "\"" << std::endl; return; }
    if (argInfo.hasNumber && !argInfo.isIndirect) { std::cout << "arg cannot be just a number." << std::endl; return; }
    
    if (!argInfo.isIndirect && argInfo.bit!=0 && argInfo.bit!=bitMode) { std::cout << "PUSH arguments can only be " << bitMode << " bit." << std::endl; return; }
    if (argInfo.bit!=0 && argInfo.bit<32) return;// Dont have handling for these yet

    if (!argInfo.isIndirect) { // arg is just a register, and you can use the offset based instruction
        // ex: pop reg
        pushByte(receiver, INTEL_INSTR_PUSHpRv+argInfo.reg1Offset);
        std::cout << "POP " << std::to_string(argInfo) << std::endl;
    } else if (argInfo.isEip) {
        // ex: pop ([rip+0] or [rip+num])
        if (bitMode==64 && argInfo.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
        pushByte(receiver, INTEL_INSTR_POP_RMv);
        pushByte(receiver, (INTEL_ModRM_MOD_Address|INTEL_ModRM_RM_DisplaceOnly));
        pushWord(receiver, argInfo.hasNumber?argInfo.numValue:0, true);
        std::cout << "POP " << ((bitMode==64)?"qword":"dword") << " ptr " << std::to_string(argInfo) << std::endl;
    } else if (argInfo.hasReg2 || (argInfo.hasNumber && !argInfo.hasReg1)) {
        if (argInfo.reg1Base==INTEL_SIB_Base_None) { std::cout << "Register \"" << argInfo.reg1Str << "\" is an invalid address base" << std::endl; return; }
        if (argInfo.reg2Index==INTEL_SIB_Index_None) { std::cout << "Register \"" << argInfo.reg2Str << "\" is an invalid address index" << std::endl; return; }
        // ex: pop [reg+reg*scale+num]
        if (bitMode==64 && argInfo.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
        pushByte(receiver, INTEL_INSTR_POP_RMv);
        const uint8_t Mod = ((argInfo.hasNumber && argInfo.hasReg1)?((argInfo.numValue>=128)?INTEL_ModRM_MOD_4byteDisp:INTEL_ModRM_MOD_1byteDisp):INTEL_ModRM_MOD_Address);// For some reason, specifically if you have a base in the [base+index*scale], and you want a displacement, you have to specify, just not any other time
        const uint8_t ScaleIndex = (argInfo.hasReg2?(argInfo.reg2Index|argInfo.reg2Scale):(INTEL_SIB_Scale_None|INTEL_SIB_Index_None));
        const uint8_t Base = (argInfo.hasReg1?argInfo.reg1Base:INTEL_SIB_Base_None);
        pushByte(receiver, (Mod|INTEL_ModRM_RM_NoDisplace));
        pushByte(receiver, (ScaleIndex|Base));
        if (argInfo.hasNumber || !argInfo.hasReg1) {// Displacement only needs to be added if it is desired. Except, for some reason... when there is not base of the formula [base+index*scale]
            if (argInfo.numValue>=128 || !argInfo.hasReg1) pushWord(receiver, argInfo.hasNumber?argInfo.numValue:0, true); // Push either the displacement or 0 if there is not displacement.
            else pushByte(receiver, argInfo.numValue);
        }
        std::cout << "POP " << ((bitMode==64)?"qword":"dword") << " ptr " << std::to_string(argInfo) << std::endl;
    } else { // arg is indirect but does not require an SIB byte
        // ex: pop ([reg] or [reg+num])
        if (bitMode==64 && argInfo.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
        pushByte(receiver, INTEL_INSTR_POP_RMv);
        const uint8_t Mod = argInfo.hasNumber?((argInfo.numValue>=128)?INTEL_ModRM_MOD_4byteDisp:INTEL_ModRM_MOD_1byteDisp):INTEL_ModRM_MOD_Address;
        pushByte(receiver, ((Mod|argInfo.reg1RM)));
        if (argInfo.hasNumber)  {
            if (argInfo.numValue>=128) pushWord(receiver, argInfo.numValue, true);
            else pushByte(receiver, argInfo.numValue);
        }
        std::cout << "POP " << ((bitMode==64)?"qword":"dword") << " ptr " << std::to_string(argInfo) << std::endl;
    }
}

/*#pragma region jump if instructions
template <typename T>
void JOV(T &receiver, const char *arg) {
    if (bitMode==0) return;
    instructionArgInfo argInfo = processArg(arg);
    if(!argInfo.isValid) { std::cout << "arg: \"" << arg << "\": \"" << argInfo.reason << "\"" << std::endl; return; }
    
    if(argInfo.isIndirect) { std::cout << "arg cannot be indirect" << std::endl; return; }
    if(argInfo.hasReg1) { std::cout << "arg cannot be a register" << std::endl; return; }
    if (argInfo.bit!=0 && argInfo.bit<32) return;// Dont have handling for these yet
}
template <typename T>
void JNOV(T &receiver, const char *arg) {
    if (bitMode==0) return;
    instructionArgInfo argInfo = processArg(arg);
    if(!argInfo.isValid) { std::cout << "arg: \"" << arg << "\": \"" << argInfo.reason << "\"" << std::endl; return; }
    
    if(argInfo.isIndirect) { std::cout << "arg cannot be indirect" << std::endl; return; }
    if(argInfo.hasReg1) { std::cout << "arg cannot be a register" << std::endl; return; }
    if (argInfo.bit!=0 && argInfo.bit<32) return;// Dont have handling for these yet
}
template <typename T>
void JULT(T &receiver, const char *arg) {
    if (bitMode==0) return;
    instructionArgInfo argInfo = processArg(arg);
    if(!argInfo.isValid) { std::cout << "arg: \"" << arg << "\": \"" << argInfo.reason << "\"" << std::endl; return; }
    
    if(argInfo.isIndirect) { std::cout << "arg cannot be indirect" << std::endl; return; }
    if(argInfo.hasReg1) { std::cout << "arg cannot be a register" << std::endl; return; }
    if (argInfo.bit!=0 && argInfo.bit<32) return;// Dont have handling for these yet
}
template <typename T>
void JUGE(T &receiver, const char *arg) {
    if (bitMode==0) return;
    instructionArgInfo argInfo = processArg(arg);
    if(!argInfo.isValid) { std::cout << "arg: \"" << arg << "\": \"" << argInfo.reason << "\"" << std::endl; return; }
    
    if(argInfo.isIndirect) { std::cout << "arg cannot be indirect" << std::endl; return; }
    if(argInfo.hasReg1) { std::cout << "arg cannot be a register" << std::endl; return; }
    if (argInfo.bit!=0 && argInfo.bit<32) return;// Dont have handling for these yet
}
template <typename T>
void JET(T &receiver, const char *arg) {
    if (bitMode==0) return;
    instructionArgInfo argInfo = processArg(arg);
    if(!argInfo.isValid) { std::cout << "arg: \"" << arg << "\": \"" << argInfo.reason << "\"" << std::endl; return; }
    
    if(argInfo.isIndirect) { std::cout << "arg cannot be indirect" << std::endl; return; }
    if(argInfo.hasReg1) { std::cout << "arg cannot be a register" << std::endl; return; }
    if (argInfo.bit!=0 && argInfo.bit<32) return;// Dont have handling for these yet
}
template <typename T>
void JNE(T &receiver, const char *arg) {
    if (bitMode==0) return;
    instructionArgInfo argInfo = processArg(arg);
    if(!argInfo.isValid) { std::cout << "arg: \"" << arg << "\": \"" << argInfo.reason << "\"" << std::endl; return; }
    
    if(argInfo.isIndirect) { std::cout << "arg cannot be indirect" << std::endl; return; }
    if(argInfo.hasReg1) { std::cout << "arg cannot be a register" << std::endl; return; }
    if (argInfo.bit!=0 && argInfo.bit<32) return;// Dont have handling for these yet
}
template <typename T>
void JULE(T &receiver, const char *arg) {
    if (bitMode==0) return;
    instructionArgInfo argInfo = processArg(arg);
    if(!argInfo.isValid) { std::cout << "arg: \"" << arg << "\": \"" << argInfo.reason << "\"" << std::endl; return; }
    
    if(argInfo.isIndirect) { std::cout << "arg cannot be indirect" << std::endl; return; }
    if(argInfo.hasReg1) { std::cout << "arg cannot be a register" << std::endl; return; }
    if (argInfo.bit!=0 && argInfo.bit<32) return;// Dont have handling for these yet
}
template <typename T>
void JUGT(T &receiver, const char *arg) {
    if (bitMode==0) return;
    instructionArgInfo argInfo = processArg(arg);
    if(!argInfo.isValid) { std::cout << "arg: \"" << arg << "\": \"" << argInfo.reason << "\"" << std::endl; return; }
    
    if(argInfo.isIndirect) { std::cout << "arg cannot be indirect" << std::endl; return; }
    if(argInfo.hasReg1) { std::cout << "arg cannot be a register" << std::endl; return; }
    if (argInfo.bit!=0 && argInfo.bit<32) return;// Dont have handling for these yet
}
template <typename T>
void JS(T &receiver, const char *arg) {
    if (bitMode==0) return;
    instructionArgInfo argInfo = processArg(arg);
    if(!argInfo.isValid) { std::cout << "arg: \"" << arg << "\": \"" << argInfo.reason << "\"" << std::endl; return; }
    
    if(argInfo.isIndirect) { std::cout << "arg cannot be indirect" << std::endl; return; }
    if(argInfo.hasReg1) { std::cout << "arg cannot be a register" << std::endl; return; }
    if (argInfo.bit!=0 && argInfo.bit<32) return;// Dont have handling for these yet
}
template <typename T>
void JNS(T &receiver, const char *arg) {
    if (bitMode==0) return;
    instructionArgInfo argInfo = processArg(arg);
    if(!argInfo.isValid) { std::cout << "arg: \"" << arg << "\": \"" << argInfo.reason << "\"" << std::endl; return; }
    
    if(argInfo.isIndirect) { std::cout << "arg cannot be indirect" << std::endl; return; }
    if(argInfo.hasReg1) { std::cout << "arg cannot be a register" << std::endl; return; }
    if (argInfo.bit!=0 && argInfo.bit<32) return;// Dont have handling for these yet
}
template <typename T>
void JP(T &receiver, const char *arg) {
    if (bitMode==0) return;
    instructionArgInfo argInfo = processArg(arg);
    if(!argInfo.isValid) { std::cout << "arg: \"" << arg << "\": \"" << argInfo.reason << "\"" << std::endl; return; }
    
    if(argInfo.isIndirect) { std::cout << "arg cannot be indirect" << std::endl; return; }
    if(argInfo.hasReg1) { std::cout << "arg cannot be a register" << std::endl; return; }
    if (argInfo.bit!=0 && argInfo.bit<32) return;// Dont have handling for these yet
};
template <typename T>
void JNP(T &receiver, const char *arg) {
    if (bitMode==0) return;
    instructionArgInfo argInfo = processArg(arg);
    if(!argInfo.isValid) { std::cout << "arg: \"" << arg << "\": \"" << argInfo.reason << "\"" << std::endl; return; }
    
    if(argInfo.isIndirect) { std::cout << "arg cannot be indirect" << std::endl; return; }
    if(argInfo.hasReg1) { std::cout << "arg cannot be a register" << std::endl; return; }
    if (argInfo.bit!=0 && argInfo.bit<32) return;// Dont have handling for these yet
}
template <typename T>
void JLT(T &receiver, const char *arg) {
    if (bitMode==0) return;
    instructionArgInfo argInfo = processArg(arg);
    if(!argInfo.isValid) { std::cout << "arg: \"" << arg << "\": \"" << argInfo.reason << "\"" << std::endl; return; }
    
    if(argInfo.isIndirect) { std::cout << "arg cannot be indirect" << std::endl; return; }
    if(argInfo.hasReg1) { std::cout << "arg cannot be a register" << std::endl; return; }
    if (argInfo.bit!=0 && argInfo.bit<32) return;// Dont have handling for these yet
}
template <typename T>
void JGE(T &receiver, const char *arg) {
    if (bitMode==0) return;
    instructionArgInfo argInfo = processArg(arg);
    if(!argInfo.isValid) { std::cout << "arg: \"" << arg << "\": \"" << argInfo.reason << "\"" << std::endl; return; }
    
    if(argInfo.isIndirect) { std::cout << "arg cannot be indirect" << std::endl; return; }
    if(argInfo.hasReg1) { std::cout << "arg cannot be a register" << std::endl; return; }
    if (argInfo.bit!=0 && argInfo.bit<32) return;// Dont have handling for these yet
}
template <typename T>
void JLE(T &receiver, const char *arg) {
    if (bitMode==0) return;
    instructionArgInfo argInfo = processArg(arg);
    if(!argInfo.isValid) { std::cout << "arg: \"" << arg << "\": \"" << argInfo.reason << "\"" << std::endl; return; }
    
    if(argInfo.isIndirect) { std::cout << "arg cannot be indirect" << std::endl; return; }
    if(argInfo.hasReg1) { std::cout << "arg cannot be a register" << std::endl; return; }
    if (argInfo.bit!=0 && argInfo.bit<32) return;// Dont have handling for these yet
}
template <typename T>
void JGT(T &receiver, const char *arg) {
    if (bitMode==0) return;
    instructionArgInfo argInfo = processArg(arg);
    if(!argInfo.isValid) { std::cout << "arg: \"" << arg << "\": \"" << argInfo.reason << "\"" << std::endl; return; }
    
    if(argInfo.isIndirect) { std::cout << "arg cannot be indirect" << std::endl; return; }
    if(argInfo.hasReg1) { std::cout << "arg cannot be a register" << std::endl; return; }
    if (argInfo.bit!=0 && argInfo.bit<32) return;// Dont have handling for these yet
}
template <typename T>
void JCXZ(T &receiver, const char *arg) {
    if (bitMode==0) return;
    instructionArgInfo argInfo = processArg(arg);
    if(!argInfo.isValid) { std::cout << "arg: \"" << arg << "\": \"" << argInfo.reason << "\"" << std::endl; return; }
    
    if(argInfo.isIndirect) { std::cout << "arg cannot be indirect" << std::endl; return; }
    if(argInfo.hasReg1) { std::cout << "arg cannot be a register" << std::endl; return; }
    if (argInfo.bit!=0 && argInfo.bit<32) return;// Dont have handling for these yet
}
#pragma endregion jump if instructions*/

template <typename T>
void NOP(T &receiver) {
    if (bitMode==0) return;
    pushByte(receiver, INTEL_INSTR_NOP);
    std::cout << "NOP" << std::endl;
}
template <typename T>
void NOP(T &receiver, const char *arg) {
    if (bitMode==0) return;
    instructionArgInfo argInfo = processArg(arg);
    if(!argInfo.isValid) { std::cout << "arg: \"" << arg << "\": \"" << argInfo.reason << "\"" << std::endl; return; }
    
    if (argInfo.hasNumber && !argInfo.isIndirect) { std::cout << "arg cannot be just a number." << std::endl; return; }
    if (argInfo.bit!=0 && argInfo.bit<32) return;// Dont have handling for these yet

    if (argInfo.isEip) {
        // ex: nop ([rip+0] or [rip+num])
        if (bitMode==64 && argInfo.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
        pushHalfWord(receiver, INTEL_INSTR_NOP_RMv, false);
        pushByte(receiver, (INTEL_ModRM_MOD_Address|INTEL_ModRM_RM_DisplaceOnly));
        pushWord(receiver, argInfo.hasNumber?argInfo.numValue:0, true);
        std::cout << "NOP dword ptr " << std::to_string(argInfo) << std::endl;
    } else if (argInfo.hasReg2 || (argInfo.hasNumber && !argInfo.hasReg1)) {
        if (argInfo.reg1Base==INTEL_SIB_Base_None) { std::cout << "Register \"" << argInfo.reg1Str << "\" is an invalid address base" << std::endl; return; }
        if (argInfo.reg2Index==INTEL_SIB_Index_None) { std::cout << "Register \"" << argInfo.reg2Str << "\" is an invalid address index" << std::endl; return; }
        // ex: nop [reg+reg*scale+num]
        if (bitMode==64 && argInfo.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
        pushHalfWord(receiver, INTEL_INSTR_NOP_RMv, false);
        const uint8_t Mod = ((argInfo.hasNumber && argInfo.hasReg1)?((argInfo.numValue>=128)?INTEL_ModRM_MOD_4byteDisp:INTEL_ModRM_MOD_1byteDisp):INTEL_ModRM_MOD_Address);// For some reason, specifically if you have a base in the [base+index*scale], and you want a displacement, you have to specify, just not any other time
        const uint8_t ScaleIndex = (argInfo.hasReg2?(argInfo.reg2Index|argInfo.reg2Scale):(INTEL_SIB_Scale_None|INTEL_SIB_Index_None));
        const uint8_t Base = (argInfo.hasReg1?argInfo.reg1Base:INTEL_SIB_Base_None);
        pushByte(receiver, (Mod|INTEL_ModRM_RM_NoDisplace));
        pushByte(receiver, (ScaleIndex|Base));  
        if (argInfo.hasNumber || !argInfo.hasReg1) {// Displacement only needs to be added if it is desired. Except, for some reason... when there is not base of the formula [base+index*scale]
            if (argInfo.numValue>=128 || !argInfo.hasReg1) pushWord(receiver, argInfo.hasNumber?argInfo.numValue:0, true); // Push either the displacement or 0 if there is not displacement.
            else pushByte(receiver, argInfo.numValue);
        }
        std::cout << "NOP dword ptr " << std::to_string(argInfo) << std::endl;
    } else { // arg is just a register, or is indirect but does not require an SIB byte
        // ex: nop ([reg] or [reg+num])
        if (!argInfo.isIndirect && argInfo.bit==64) pushByte(receiver, INTEL_INSTR64_OperandSz_OVRD);
        if (argInfo.isIndirect && bitMode==64 && argInfo.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
        pushHalfWord(receiver, INTEL_INSTR_NOP_RMv, false);
        const uint8_t Mod = argInfo.isIndirect?(argInfo.hasNumber?((argInfo.numValue>=128)?INTEL_ModRM_MOD_4byteDisp:INTEL_ModRM_MOD_1byteDisp):INTEL_ModRM_MOD_Address):INTEL_ModRM_MOD_Reg;
        pushByte(receiver, ((Mod|argInfo.reg1RM)));
        if (argInfo.hasNumber) {
            if (argInfo.numValue>=128) pushWord(receiver, argInfo.numValue, true);
            else pushByte(receiver, argInfo.numValue);
        }
        std::cout << "NOP " << (argInfo.isIndirect?"dword ptr ":"") << std::to_string(argInfo) << std::endl;
    }
}

template <typename T>
void XCHG(T &receiver, const char *reg1, const char *reg2) {
    if (bitMode==0) return;
    instructionArgInfo reg1Info = processArg(reg1);
    if(!reg1Info.isValid) { std::cout << "reg1: \"" << reg1 << "\": \"" << reg1Info.reason << "\"" << std::endl; return; }
    instructionArgInfo reg2Info = processArg(reg2);
    if(!reg2Info.isValid) { std::cout << "reg2: \"" << reg2 << "\": \"" << reg2Info.reason << "\"" << std::endl; return; }

    if (reg1Info.isIndirect && reg2Info.isIndirect) { std::cout << "both registers cannot be indirect." << std::endl; return; }
    if (reg1Info.hasNumber && !reg1Info.isIndirect) { std::cout << "reg1 cannot be just a number." << std::endl; return; }
    if (reg2Info.hasNumber && !reg2Info.isIndirect) { std::cout << "reg2 cannot be just a number." << std::endl; return; }
    if ((reg1Info.bit!=0 && reg1Info.bit<32) || (reg2Info.bit!=0 && reg2Info.bit<32)) return;// dont have handling for these yet

    if (reg1Info.isIndirect) {
        if (reg1Info.isEip) {
            // ex: xchg ([eip+0] or [eip+num]), reg
            if (bitMode==64 && reg1Info.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
            if (reg2Info.bit==64) pushByte(receiver, INTEL_INSTR64_OperandSz_OVRD);
            pushByte(receiver, INTEL_INSTR_XCHG_REGv_RMv);
            pushByte(receiver, INTEL_ModRM_MOD_Address|reg2Info.reg1RegOp|INTEL_ModRM_RM_DisplaceOnly);
            pushWord(receiver, reg1Info.hasNumber?reg1Info.numValue:0, true);
            std::cout << "XCHG " << ((reg2Info.bit==64)?"qword":"dword") << " ptr " << std::to_string(reg1Info) << ", " << std::to_string(reg2Info) << std::endl;
        } else if (reg1Info.hasReg2 || (reg1Info.hasNumber && !reg1Info.hasReg1)) {// will require a SIB byte
            if (reg1Info.reg1Base==INTEL_SIB_Base_None) { std::cout << "Register \"" << reg1Info.reg1Str << "\" is an invalid address base" << std::endl; return; }
            if (reg1Info.reg2Index==INTEL_SIB_Index_None) { std::cout << "Register \"" << reg1Info.reg2Str << "\" is an invalid address index" << std::endl; return; }
            // ex: xchg [reg+reg*scale+num], reg
            if (bitMode==64 && reg1Info.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
            if (reg2Info.bit==64) pushByte(receiver, INTEL_INSTR64_OperandSz_OVRD);
            pushByte(receiver, INTEL_INSTR_XCHG_REGv_RMv);
            const uint8_t Mod = ((reg1Info.hasNumber && reg1Info.hasReg1)?((reg1Info.numValue>=128)?INTEL_ModRM_MOD_4byteDisp:INTEL_ModRM_MOD_1byteDisp):INTEL_ModRM_MOD_Address);
            const uint8_t ScaleIndex = (reg1Info.hasReg2?(reg1Info.reg2Index|reg1Info.reg2Scale):(INTEL_SIB_Scale_None|INTEL_SIB_Index_None));
            const uint8_t Base = (reg1Info.hasReg1?reg1Info.reg1Base:INTEL_SIB_Base_None);
            pushByte(receiver, Mod|reg2Info.reg1RegOp|INTEL_ModRM_RM_NoDisplace);
            pushByte(receiver, ScaleIndex|Base);
            if (reg1Info.hasNumber || !reg1Info.hasReg1) {
                if (reg1Info.numValue>=128 || !reg1Info.hasReg1) pushWord(receiver, reg1Info.hasNumber?reg1Info.numValue:0, true);
                else pushByte(receiver, reg1Info.numValue);
            }
            std::cout << "XCHG " << ((reg2Info.bit==64)?"qword":"dword") << " ptr " << std::to_string(reg1Info) << ", " << std::to_string(reg2Info) << std::endl;
        } else {
            // ex: xchg ([reg] or [reg+num]), reg
            if (bitMode==64 && reg1Info.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
            if (reg2Info.bit==64) pushByte(receiver, INTEL_INSTR64_OperandSz_OVRD);
            pushByte(receiver, INTEL_INSTR_XCHG_REGv_RMv);
            const uint8_t Mod = (reg1Info.hasNumber?((reg1Info.numValue>=128)?INTEL_ModRM_MOD_4byteDisp:INTEL_ModRM_MOD_1byteDisp):INTEL_ModRM_MOD_Address);
            pushByte(receiver, Mod|reg2Info.reg1RegOp|reg1Info.reg1RM);
            if (reg1Info.hasNumber) {
                if (reg1Info.numValue>=128) pushWord(receiver, reg1Info.numValue, true);
                else pushByte(receiver, reg1Info.numValue);
            }
            std::cout << "XCHG " << ((reg2Info.bit==64)?"qword":"dword") << " ptr " << std::to_string(reg1Info) << ", " << std::to_string(reg2Info) << std::endl;
        }
    } else if (reg2Info.isIndirect) {
        if (reg2Info.isEip) {
            // ex: xchg reg, ([eip+0] or [eip+num])
            if (bitMode==64 && reg2Info.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
            if (reg1Info.bit==64) pushByte(receiver, INTEL_INSTR64_OperandSz_OVRD);
            pushByte(receiver, INTEL_INSTR_XCHG_REGv_RMv);
            pushByte(receiver, INTEL_ModRM_MOD_Address|reg1Info.reg1RegOp|INTEL_ModRM_RM_DisplaceOnly);
            pushWord(receiver, reg2Info.hasNumber?reg2Info.numValue:0, true);
            std::cout << "XCHG " << std::to_string(reg1Info) << ", " << ((reg1Info.bit==64)?"qword":"dword") << " ptr " << std::to_string(reg2Info) << std::endl;
        } else if (reg2Info.hasReg2 || (reg2Info.hasNumber && !reg2Info.hasReg1)) {// will require a SIB byte
            if (reg2Info.reg1Base==INTEL_SIB_Base_None) { std::cout << "Register \"" << reg2Info.reg1Str << "\" is an invalid address base" << std::endl; return; }
            if (reg2Info.reg2Index==INTEL_SIB_Index_None) { std::cout << "Register \"" << reg2Info.reg2Str << "\" is an invalid address index" << std::endl; return; }
            // ex: xchg reg, [reg+reg*scale+num]
            if (bitMode==64 && reg2Info.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
            if (reg1Info.bit==64) pushByte(receiver, INTEL_INSTR64_OperandSz_OVRD);
            pushByte(receiver, INTEL_INSTR_XCHG_REGv_RMv);
            const uint8_t Mod = ((reg2Info.hasNumber && reg2Info.hasReg1)?((reg2Info.numValue>=128)?INTEL_ModRM_MOD_4byteDisp:INTEL_ModRM_MOD_1byteDisp):INTEL_ModRM_MOD_Address);
            const uint8_t ScaleIndex = (reg2Info.hasReg2?(reg2Info.reg2Index|reg2Info.reg2Scale):(INTEL_SIB_Scale_None|INTEL_SIB_Index_None));
            const uint8_t Base = (reg2Info.hasReg1?reg2Info.reg1Base:INTEL_SIB_Base_None);
            pushByte(receiver, Mod|reg1Info.reg1RegOp|INTEL_ModRM_RM_NoDisplace);
            pushByte(receiver, ScaleIndex|Base);
            if (reg2Info.hasNumber || !reg2Info.hasReg1) {
                if (reg2Info.numValue>=128 || !reg2Info.hasReg1) pushWord(receiver, reg2Info.hasNumber?reg2Info.numValue:0, true);
                else pushByte(receiver, reg2Info.numValue);
            }
            std::cout << "XCHG " << std::to_string(reg1Info) << ", " << ((reg1Info.bit==64)?"qword":"dword") << " ptr " << std::to_string(reg2Info) << std::endl;
        } else {
            // ex: xchg reg, ([reg] or [reg+num])
            if (bitMode==64 && reg2Info.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
            if (reg1Info.bit==64) pushByte(receiver, INTEL_INSTR64_OperandSz_OVRD);
            pushByte(receiver, INTEL_INSTR_XCHG_REGv_RMv);
            const uint8_t Mod = (reg2Info.hasNumber?((reg2Info.numValue>=128)?INTEL_ModRM_MOD_4byteDisp:INTEL_ModRM_MOD_1byteDisp):INTEL_ModRM_MOD_Address);
            pushByte(receiver, Mod|reg1Info.reg1RegOp|reg2Info.reg1RM);
            if (reg2Info.hasNumber) {
                if (reg2Info.numValue>=128) pushWord(receiver, reg2Info.numValue, true);
                else pushByte(receiver, reg2Info.numValue);
            }
            std::cout << "XCHG " << std::to_string(reg1Info) << ", " << ((reg1Info.bit==64)?"qword":"dword") << " ptr " << std::to_string(reg2Info) << std::endl;
        }
    } else if (reg1Info.reg1Offset==INTEL_REG_OFF_eAX) {
        // ex: xchg eAX, reg
        if (reg1Info.bit!=reg2Info.bit) { std::cout << "Operand size mis-match." << std::endl; return; }
        if (reg1Info.bit==64 && (reg2Info.reg1Offset!=INTEL_REG_OFF_eAX)) pushByte(receiver, INTEL_INSTR64_OperandSz_OVRD);
        pushByte(receiver, INTEL_INSTR_XCHG_eAX_REGpRv+reg2Info.reg1Offset);
        if (reg2Info.reg1Offset==INTEL_REG_OFF_eAX) std::cout << "NOP" << std::endl;
        else std::cout << "XCHG " << std::to_string(reg1Info) << ", " << std::to_string(reg2Info) << std::endl;
    } else if (reg2Info.reg1Offset==INTEL_REG_OFF_eAX) {
        // ex: xchg reg, eAX
        if (reg1Info.bit!=reg2Info.bit) { std::cout << "Operand size mis-match." << std::endl; return; }
        if (reg1Info.bit==64 && (reg1Info.reg1Offset!=INTEL_REG_OFF_eAX)) pushByte(receiver, INTEL_INSTR64_OperandSz_OVRD);
        pushByte(receiver, INTEL_INSTR_XCHG_eAX_REGpRv+reg1Info.reg1Offset);
        if (reg1Info.reg1Offset==INTEL_REG_OFF_eAX) std::cout << "NOP" << std::endl;
        else std::cout << "XCHG " << std::to_string(reg1Info) << ", " << std::to_string(reg2Info) << std::endl;
    } else {
        // ex: xchg reg, reg
        if (reg1Info.bit!=reg2Info.bit) { std::cout << "Operand size mis-match." << std::endl; return; }
        if (reg1Info.bit==64) pushByte(receiver, INTEL_INSTR64_OperandSz_OVRD);
        pushByte(receiver, INTEL_INSTR_XCHG_REGv_RMv);
        pushByte(receiver, INTEL_ModRM_MOD_Reg|reg1Info.reg1RegOp|reg2Info.reg1RM);
        std::cout << "XCHG " << std::to_string(reg1Info) << ", " << std::to_string(reg2Info) << std::endl;
    }
}

template <typename T>
void MOV(T &receiver, const char *dst, const char *src) {
    if (bitMode==0) return;
    instructionArgInfo dstInfo = processArg(dst);
    if(!dstInfo.isValid) { std::cout << "dst: \"" << dst << "\": \"" << dstInfo.reason << "\"" << std::endl; return; }
    instructionArgInfo srcInfo = processArg(src);
    if(!srcInfo.isValid) { std::cout << "src: \"" << src << "\": \"" << srcInfo.reason << "\"" << std::endl; return; }

    if (dstInfo.isIndirect && srcInfo.isIndirect) { std::cout << "both registers cannot be indirect." << std::endl; return; }
    if (dstInfo.hasNumber && !dstInfo.isIndirect) { std::cout << "dst cannot be just a number." << std::endl; return; }
    if ((dstInfo.bit!=0 && dstInfo.bit<32) || (srcInfo.bit!=0 && srcInfo.bit<32)) return;// dont have handling for these yet

    if (dstInfo.isIndirect) {
        if (dstInfo.isEip) {
            // ex: mov ([eip+0] or [eip+num]), (reg or num)
            if (bitMode==64 && dstInfo.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
            if (srcInfo.bit==64) pushByte(receiver, INTEL_INSTR64_OperandSz_OVRD);
            pushByte(receiver, srcInfo.hasNumber?INTEL_INSTR_MOV_RMv_Iv:INTEL_INSTR_MOV_RMv_REGv);
            const uint8_t RegOp = srcInfo.hasReg1?srcInfo.reg1RegOp:0;
            pushByte(receiver, INTEL_ModRM_MOD_Address|RegOp|INTEL_ModRM_RM_DisplaceOnly);
            pushWord(receiver, dstInfo.hasNumber?dstInfo.numValue:0, true);
        } else if (dstInfo.hasReg2 || (dstInfo.hasNumber && !dstInfo.hasReg1)) {// will require a SIB byte
            if (dstInfo.reg1Base==INTEL_SIB_Base_None) { std::cout << "Register \"" << dstInfo.reg1Str << "\" is an invalid address base" << std::endl; return; }
            if (dstInfo.reg2Index==INTEL_SIB_Index_None) { std::cout << "Register \"" << dstInfo.reg2Str << "\" is an invalid address index" << std::endl; return; }
            // ex: mov [reg+reg*scale+num], (reg or num)
            if (bitMode==64 && dstInfo.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
            if (srcInfo.bit==64) pushByte(receiver, INTEL_INSTR64_OperandSz_OVRD);
            pushByte(receiver, srcInfo.hasNumber?INTEL_INSTR_MOV_RMv_Iv:INTEL_INSTR_MOV_RMv_REGv);
            const uint8_t Mod = ((dstInfo.hasNumber && dstInfo.hasReg1)?((dstInfo.numValue>=128)?INTEL_ModRM_MOD_4byteDisp:INTEL_ModRM_MOD_1byteDisp):INTEL_ModRM_MOD_Address);
            const uint8_t RegOp = srcInfo.hasReg1?srcInfo.reg1RegOp:0;
            const uint8_t ScaleIndex = (dstInfo.hasReg2?(dstInfo.reg2Index|dstInfo.reg2Scale):(INTEL_SIB_Scale_None|INTEL_SIB_Index_None));
            const uint8_t Base = (dstInfo.hasReg1?dstInfo.reg1Base:INTEL_SIB_Base_None);
            pushByte(receiver, Mod|RegOp|INTEL_ModRM_RM_NoDisplace);
            pushByte(receiver, ScaleIndex|Base);
            if (dstInfo.hasNumber || !dstInfo.hasReg1) {
                if (dstInfo.numValue>=128 || !dstInfo.hasReg1) pushWord(receiver, dstInfo.hasNumber?dstInfo.numValue:0, true);
                else pushByte(receiver, dstInfo.numValue);
            }
        } else {
            // ex: mov ([reg] or [reg+num]), (reg or num)
            if (bitMode==64 && dstInfo.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
            if (srcInfo.bit==64) pushByte(receiver, INTEL_INSTR64_OperandSz_OVRD);
            pushByte(receiver, srcInfo.hasNumber?INTEL_INSTR_MOV_RMv_Iv:INTEL_INSTR_MOV_RMv_REGv);
            const uint8_t Mod = (dstInfo.hasNumber?((dstInfo.numValue>=128)?INTEL_ModRM_MOD_4byteDisp:INTEL_ModRM_MOD_1byteDisp):INTEL_ModRM_MOD_Address);
            const uint8_t RegOp = srcInfo.hasReg1?srcInfo.reg1RegOp:0;
            pushByte(receiver, Mod|RegOp|dstInfo.reg1RM);
            if (dstInfo.hasNumber) {
                if (dstInfo.numValue>=128) pushWord(receiver, dstInfo.numValue, true);
                else pushByte(receiver, dstInfo.numValue);
            }
        }
        if (srcInfo.hasNumber) pushWord(receiver, srcInfo.numValue, true);
        std::cout << "MOV " << ((srcInfo.bit==64)?"qword":"dword") << " ptr " << std::to_string(dstInfo) << ", " << std::to_string(srcInfo) << std::endl;
    } else if (srcInfo.isIndirect) {
        if (srcInfo.isEip) {
            // ex: mov reg, ([eip+0] or [eip+num])
            if (bitMode==64 && srcInfo.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
            if (dstInfo.bit==64) pushByte(receiver, INTEL_INSTR64_OperandSz_OVRD);
            pushByte(receiver, INTEL_INSTR_MOV_REGv_RMv);
            pushByte(receiver, INTEL_ModRM_MOD_Address|dstInfo.reg1RegOp|INTEL_ModRM_RM_DisplaceOnly);
            pushWord(receiver, srcInfo.hasNumber?srcInfo.numValue:0, true);
        } else if (srcInfo.hasReg2 || (srcInfo.hasNumber && !srcInfo.hasReg1)) {// will require a SIB byte
            if (srcInfo.reg1Base==INTEL_SIB_Base_None) { std::cout << "Register \"" << srcInfo.reg1Str << "\" is an invalid address base" << std::endl; return; }
            if (srcInfo.reg2Index==INTEL_SIB_Index_None) { std::cout << "Register \"" << srcInfo.reg2Str << "\" is an invalid address index" << std::endl; return; }
            // ex: mov reg, [reg+reg*scale+num]
            if (bitMode==64 && srcInfo.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
            if (dstInfo.bit==64) pushByte(receiver, INTEL_INSTR64_OperandSz_OVRD);
            pushByte(receiver, INTEL_INSTR_MOV_REGv_RMv);
            const uint8_t Mod = ((srcInfo.hasNumber && srcInfo.hasReg1)?((srcInfo.numValue>=128)?INTEL_ModRM_MOD_4byteDisp:INTEL_ModRM_MOD_1byteDisp):INTEL_ModRM_MOD_Address);
            const uint8_t ScaleIndex = (srcInfo.hasReg2?(srcInfo.reg2Index|srcInfo.reg2Scale):(INTEL_SIB_Scale_None|INTEL_SIB_Index_None));
            const uint8_t Base = (srcInfo.hasReg1?srcInfo.reg1Base:INTEL_SIB_Base_None);
            pushByte(receiver, Mod|dstInfo.reg1RegOp|INTEL_ModRM_RM_NoDisplace);
            pushByte(receiver, ScaleIndex|Base);
            if (srcInfo.hasNumber || !srcInfo.hasReg1) {
                if (srcInfo.numValue>=128 || !srcInfo.hasReg1) pushWord(receiver, srcInfo.hasNumber?srcInfo.numValue:0, true);
                else pushByte(receiver, srcInfo.numValue);
            }
        } else {
            // ex: mov reg, ([reg] or [reg+num])
            if (bitMode==64 && srcInfo.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
            if (dstInfo.bit==64) pushByte(receiver, INTEL_INSTR64_OperandSz_OVRD);
            pushByte(receiver, INTEL_INSTR_MOV_REGv_RMv);
            const uint8_t Mod = (srcInfo.hasNumber?((srcInfo.numValue>=128)?INTEL_ModRM_MOD_4byteDisp:INTEL_ModRM_MOD_1byteDisp):INTEL_ModRM_MOD_Address);
            pushByte(receiver, Mod|dstInfo.reg1RegOp|srcInfo.reg1RM);
            if (srcInfo.hasNumber) {
                if (srcInfo.numValue>=128) pushWord(receiver, srcInfo.numValue, true);
                else pushByte(receiver, srcInfo.numValue);
            }
        }
        std::cout << "MOV " << std::to_string(dstInfo) << ", " << ((dstInfo.bit==64)?"qword":"dword") << " ptr " << std::to_string(srcInfo) << std::endl;
    } else if (srcInfo.hasNumber && dstInfo.bit<64) {
        // ex: mov reg, num
        if (dstInfo.bit==64) pushByte(receiver, INTEL_INSTR64_OperandSz_OVRD);
        pushByte(receiver, INTEL_INSTR_MOV_REGpRv_Iv+dstInfo.reg1Offset);
        pushWord(receiver, srcInfo.numValue, true);
        std::cout << "MOV " << std::to_string(dstInfo) << ", " << std::to_string(srcInfo) << std::endl;
    } else {
        // ex: mov reg, (reg or num)
        if (srcInfo.bit!=0 && dstInfo.bit!=srcInfo.bit) { std::cout << "Operand size mis-match." << std::endl; return; }
        if (dstInfo.bit==64) pushByte(receiver, INTEL_INSTR64_OperandSz_OVRD);
        pushByte(receiver, srcInfo.hasNumber?INTEL_INSTR_MOV_RMv_Iv:INTEL_INSTR_MOV_RMv_REGv);
        const uint8_t RegOp = srcInfo.hasReg1?srcInfo.reg1RegOp:0;
        pushByte(receiver, INTEL_ModRM_MOD_Reg|RegOp|dstInfo.reg1RM);
        if (srcInfo.hasNumber) pushWord(receiver, srcInfo.numValue, true);
        std::cout << "MOV " << std::to_string(dstInfo) << ", " << std::to_string(srcInfo) << std::endl;
    }
}

template <typename T>
void RET(T &receiver,  const char *num) {
    if (bitMode==0) return;
    instructionArgInfo numInfo = processArg(num);
    if(!numInfo.isValid) { std::cout << "num: \"" << num << "\": \"" << numInfo.reason << "\"" << std::endl; return; }
    
    if (numInfo.isIndirect || numInfo.hasReg1) { std::cout << "ret argument may only be a number" << std::endl; return; }
    if (numInfo.numValue >= 65536) { std::cout << "Number is too large to return." << std::endl; return; }
    pushByte(receiver, INTEL_INSTR_RET_I16);
    pushHalfWord(receiver, numInfo.numValue, true);
    std::cout << "RET " << std::to_string(numInfo) << std::endl;
}
template <typename T>
void RET(T &receiver) {
    pushByte(receiver, INTEL_INSTR_RET);
    std::cout << "RET" << std::endl;
}
template <typename T>
void RETF(T &receiver,  const char *num) {
    if (bitMode==0) return;
    instructionArgInfo numInfo = processArg(num);
    if(!numInfo.isValid) { std::cout << "num: \"" << num << "\": \"" << numInfo.reason << "\"" << std::endl; return; }
    
    if (numInfo.isIndirect || numInfo.hasReg1) { std::cout << "Retf argument may only be a number" << std::endl; return; }
    if (numInfo.numValue >= 65536) { std::cout << "Number is too large to return." << std::endl; return; }
    pushByte(receiver, INTEL_INSTR_RETF_I16);
    pushHalfWord(receiver, numInfo.numValue, true);
    std::cout << "RETF " << std::to_string(numInfo) << std::endl;
}
template <typename T>
void RETF(T &receiver) {
    pushByte(receiver, INTEL_INSTR_RETF);
    std::cout << "RETF" << std::endl;
}
template <typename T>
void INT(T &receiver,  const char *num) {
    if (bitMode==0) return;
    instructionArgInfo numInfo = processArg(num);
    if(!numInfo.isValid) { std::cout << "num: \"" << num << "\": \"" << numInfo.reason << "\"" << std::endl; return; }
    
    if (numInfo.isIndirect || numInfo.hasReg1) { std::cout << "Ret argument may only be a number." << std::endl; return; }
    if (numInfo.numValue >= 256) { std::cout << "Number is too large for interrupt." << std::endl; return; }
    if (numInfo.numValue==3) {
        pushByte(receiver, INTEL_INSTR_INT3);
        std::cout << "INT3" << std::endl;
    } else {
        pushByte(receiver, INTEL_INSTR_INT_Ib);
        pushByte(receiver, numInfo.numValue);
        std::cout << "INT " << std::to_string(numInfo) << std::endl;
    }
}

template <typename T>
void HLT(T &receiver) {
    if (bitMode==0) return;
    pushByte(receiver, INTEL_INSTR_HLT);
    std::cout << "HLT" << std::endl;
}

template <typename T>
void TEST(T &receiver, const char *arg1, const char *arg2) {
    if (bitMode==0) return;
    instructionArgInfo arg1Info = processArg(arg1);
    if(!arg1Info.isValid) { std::cout << "arg1: \"" << arg1 << "\": \"" << arg1Info.reason << "\"" << std::endl; return; }
    instructionArgInfo arg2Info = processArg(arg2);
    if(!arg2Info.isValid) { std::cout << "arg2: \"" << arg2 << "\": \"" << arg2Info.reason << "\"" << std::endl; return; }

    if (arg1Info.isIndirect && arg2Info.isIndirect) { std::cout << "both registers cannot be indirect." << std::endl; return; }
    if (arg1Info.hasNumber && !arg1Info.isIndirect) { std::cout << "dst cannot be just a number." << std::endl; return; }
    if (arg2Info.isIndirect || arg2Info.hasReg1) { std::cout << "arg2 can only be a number." << std::endl; return; }
    if ((arg1Info.bit!=0 && arg1Info.bit<32) || (arg2Info.bit!=0 && arg2Info.bit<32)) return;// dont have handling for these yet
    
    if (arg1Info.isEip) {
        if (bitMode==64 && arg1Info.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
        pushByte(receiver, INTEL_INSTR_OP2v);
        pushByte(receiver, (INTEL_ModRM_MOD_Address|INTEL_ModRM_OP2_TEST_RM_I|INTEL_ModRM_RM_DisplaceOnly));
        pushWord(receiver, arg1Info.hasNumber?arg1Info.numValue:0, true);
        pushWord(receiver, arg2Info.numValue, true);
        std::cout << "TEST dword ptr " << std::to_string(arg1Info) << ", " << std::to_string(arg2Info) << std::endl;
    } else if (arg1Info.hasReg2 || (arg1Info.hasNumber && ! arg1Info.hasReg1)) {// requires a SIB byte
        if (arg1Info.reg1Base==INTEL_SIB_Base_None) { std::cout << "Register \"" << arg1Info.reg1Str << "\" is an invalid address base" << std::endl; return; }
        if (arg1Info.reg2Index==INTEL_SIB_Index_None) { std::cout << "Register \"" << arg1Info.reg2Str << "\" is an invalid address index" << std::endl; return; }
        // ex: not [reg+reg*scale+num]
        if (bitMode==64 && arg1Info.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
        pushByte(receiver, INTEL_INSTR_OP2v);
        const uint8_t Mod = ((arg1Info.hasNumber && arg1Info.hasReg1)?((arg1Info.numValue>=128)?INTEL_ModRM_MOD_4byteDisp:INTEL_ModRM_MOD_1byteDisp):INTEL_ModRM_MOD_Address);// For some reason, specifically if you have a base in the [base+index*scale], and you want a displacement, you have to specify, just not any other time
        const uint8_t ScaleIndex = (arg1Info.hasReg2?(arg1Info.reg2Index|arg1Info.reg2Scale):(INTEL_SIB_Scale_None|INTEL_SIB_Index_None));
        const uint8_t Base = (arg1Info.hasReg1?arg1Info.reg1Base:INTEL_SIB_Base_None);
        pushByte(receiver, (Mod|INTEL_ModRM_OP2_TEST_RM_I|INTEL_ModRM_RM_NoDisplace));
        pushByte(receiver, (ScaleIndex|Base));
        if (arg1Info.hasNumber || !arg1Info.hasReg1) {// Displacement only needs to be added if it is desired. Except, for some reason... when there is not base of the formula [base+index*scale]
            if (arg1Info.numValue>=128 || !arg1Info.hasReg1) pushWord(receiver, arg1Info.hasNumber?arg1Info.numValue:0, true); // Push either the displacement or 0 if there is not displacement.
            else pushByte(receiver, arg1Info.numValue);
        }
        pushWord(receiver, arg2Info.numValue, true);
        std::cout << "TEST dword ptr " << std::to_string(arg1Info) << ", " << std::to_string(arg2Info) << std::endl;
    } else if (!arg1Info.isIndirect && arg1Info.reg1Offset==INTEL_REG_OFF_eAX) {
        if (!arg1Info.isIndirect && arg1Info.bit==64) pushByte(receiver, INTEL_INSTR64_OperandSz_OVRD);
        pushByte(receiver, INTEL_INSTR_TEST_eAX_Iv);
        pushWord(receiver, arg2Info.numValue, true);
        std::cout << "TEST " << std::to_string(arg1Info) << ", " << std::to_string(arg2Info) << std::endl;
    } else {
        // ex: not (reg or [reg] or [reg+num])
        if (!arg1Info.isIndirect && arg1Info.bit==64) pushByte(receiver, INTEL_INSTR64_OperandSz_OVRD);
        if (arg1Info.isIndirect && bitMode==64 && arg1Info.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
        pushByte(receiver, INTEL_INSTR_OP2v);
        const uint8_t Mod = arg1Info.isIndirect?(arg1Info.hasNumber?((arg1Info.numValue>=128)?INTEL_ModRM_MOD_4byteDisp:INTEL_ModRM_MOD_1byteDisp):INTEL_ModRM_MOD_Address):INTEL_ModRM_MOD_Reg;
        pushByte(receiver, ((Mod|INTEL_ModRM_OP2_TEST_RM_I|arg1Info.reg1RM)));
        if (arg1Info.hasNumber) {
            if (arg1Info.numValue>=128) pushWord(receiver, arg1Info.numValue, true);
            else pushByte(receiver, arg1Info.numValue);
        }
        pushWord(receiver, arg2Info.numValue, true);
        std::cout << "TEST " << (arg1Info.isIndirect?"dword ptr ":"") << std::to_string(arg1Info) << ", " << std::to_string(arg2Info) << std::endl;
    }
}

template <typename T>
void NotNeg(T &receiver, const char *arg, const char *instructionName, const uint8_t& op2Code=INTEL_ModRM_OP2_NOT_RM) {
    if (bitMode==0) return;
    instructionArgInfo argInfo = processArg(arg);
    if(!argInfo.isValid) { std::cout << "arg: \"" << arg << "\": \"" << argInfo.reason << "\"" << std::endl; return; }
    
    if (argInfo.hasNumber && !argInfo.isIndirect) { std::cout << "arg cannot be just a number." << std::endl; return; }
    if (argInfo.bit!=0 && argInfo.bit<32) return;// Dont have handling for these yet

    if (argInfo.isEip) {
        // ex: not/neg ([eip+0] or [eip+num])
        if (bitMode==64 && argInfo.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
        pushByte(receiver, INTEL_INSTR_OP2v);
        pushByte(receiver, (INTEL_ModRM_MOD_Address|op2Code|INTEL_ModRM_RM_DisplaceOnly));
        pushWord(receiver, argInfo.hasNumber?argInfo.numValue:0, true);
        std::cout << instructionName << " dword ptr " << std::to_string(argInfo) << std::endl;
    } else if (argInfo.hasReg2 || (argInfo.hasNumber && ! argInfo.hasReg1)) {// requires a SIB byte
        if (argInfo.reg1Base==INTEL_SIB_Base_None) { std::cout << "Register \"" << argInfo.reg1Str << "\" is an invalid address base" << std::endl; return; }
        if (argInfo.reg2Index==INTEL_SIB_Index_None) { std::cout << "Register \"" << argInfo.reg2Str << "\" is an invalid address index" << std::endl; return; }
        // ex: not/neg [reg+reg*scale+num]
        if (bitMode==64 && argInfo.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
        pushByte(receiver, INTEL_INSTR_OP2v);
        const uint8_t Mod = ((argInfo.hasNumber && argInfo.hasReg1)?((argInfo.numValue>=128)?INTEL_ModRM_MOD_4byteDisp:INTEL_ModRM_MOD_1byteDisp):INTEL_ModRM_MOD_Address);// For some reason, specifically if you have a base in the [base+index*scale], and you want a displacement, you have to specify, just not any other time
        const uint8_t ScaleIndex = (argInfo.hasReg2?(argInfo.reg2Index|argInfo.reg2Scale):(INTEL_SIB_Scale_None|INTEL_SIB_Index_None));
        const uint8_t Base = (argInfo.hasReg1?argInfo.reg1Base:INTEL_SIB_Base_None);
        pushByte(receiver, (Mod|op2Code|INTEL_ModRM_RM_NoDisplace));
        pushByte(receiver, (ScaleIndex|Base));
        if (argInfo.hasNumber || !argInfo.hasReg1) { // Displacement only needs to be added if it is desired. Except, for some reason... when there is not base of the formula [base+index*scale]
            if (argInfo.numValue>=128 || !argInfo.hasReg1) pushWord(receiver, argInfo.hasNumber?argInfo.numValue:0, true); // Push either the displacement or 0 if there is no displacement.
            else pushByte(receiver, argInfo.hasNumber?argInfo.numValue:0);
        }
        std::cout << instructionName << " dword ptr " << std::to_string(argInfo) << std::endl;
    } else {
        // ex: not/neg (reg or [reg] or [reg+num])
        if (!argInfo.isIndirect && argInfo.bit==64) pushByte(receiver, INTEL_INSTR64_OperandSz_OVRD);
        if (argInfo.isIndirect && bitMode==64 && argInfo.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
        pushByte(receiver, INTEL_INSTR_OP2v);
        const uint8_t Mod = argInfo.isIndirect?(argInfo.hasNumber?((argInfo.numValue>=128)?INTEL_ModRM_MOD_4byteDisp:INTEL_ModRM_MOD_1byteDisp):INTEL_ModRM_MOD_Address):INTEL_ModRM_MOD_Reg;
        pushByte(receiver, ((Mod|op2Code|argInfo.reg1RM)));
        if (argInfo.hasNumber) {
            if (argInfo.numValue>=128) pushWord(receiver, argInfo.hasNumber?argInfo.numValue:0, true);
            else pushByte(receiver, argInfo.hasNumber?argInfo.numValue:0);
        }
        std::cout << instructionName << " " << (argInfo.isIndirect?"dword ptr ":"") << std::to_string(argInfo) << std::endl;
    }
}
template <typename T>
void NOT(T &receiver, const char *arg) {
    NotNeg(receiver,arg,"NOT",INTEL_ModRM_OP2_NOT_RM);
}
template <typename T>
void NEG(T &receiver, const char *arg) {
    NotNeg(receiver,arg,"NEG",INTEL_ModRM_OP2_NEG_RM);
}

template <typename T>
void MulImulDivIdiv(T &receiver, const char *arg, const char *instructionName, const uint8_t& op2Code=INTEL_ModRM_OP2_UMUL_eDX_eAX_RM) {
    if (bitMode==0) return;
    instructionArgInfo argInfo = processArg(arg);
    if(!argInfo.isValid) { std::cout << "arg: \"" << arg << "\": \"" << argInfo.reason << "\"" << std::endl; return; }
    
    if (argInfo.hasNumber && !argInfo.isIndirect) { std::cout << "arg cannot be just a number." << std::endl; return; }
    if (argInfo.bit!=0 && argInfo.bit<32) return;// Dont have handling for these yet
    
    if (argInfo.isEip) {
        // ex: instr ([eip+0] or [eip+num])
        if (bitMode==64 && argInfo.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
        pushByte(receiver, INTEL_INSTR_OP2v);
        pushByte(receiver, (INTEL_ModRM_MOD_Address|op2Code|INTEL_ModRM_RM_DisplaceOnly));
        pushWord(receiver, argInfo.hasNumber?argInfo.numValue:0, true);
        std::cout << instructionName << " dword ptr " << std::to_string(argInfo) << std::endl;
    } else if (argInfo.hasReg2 || (argInfo.hasNumber && ! argInfo.hasReg1)) {// requires a SIB byte
        if (argInfo.reg1Base==INTEL_SIB_Base_None) { std::cout << "Register \"" << argInfo.reg1Str << "\" is an invalid address base" << std::endl; return; }
        if (argInfo.reg2Index==INTEL_SIB_Index_None) { std::cout << "Register \"" << argInfo.reg2Str << "\" is an invalid address index" << std::endl; return; }
        // ex: instr [reg+reg*scale+num]
        if (bitMode==64 && argInfo.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
        pushByte(receiver, INTEL_INSTR_OP2v);
        const uint8_t Mod = ((argInfo.hasNumber && argInfo.hasReg1)?((argInfo.numValue>=128)?INTEL_ModRM_MOD_4byteDisp:INTEL_ModRM_MOD_1byteDisp):INTEL_ModRM_MOD_Address);// For some reason, specifically if you have a base in the [base+index*scale], and you want a displacement, you have to specify, just not any other time
        const uint8_t ScaleIndex = (argInfo.hasReg2?(argInfo.reg2Index|argInfo.reg2Scale):(INTEL_SIB_Scale_None|INTEL_SIB_Index_None));
        const uint8_t Base = (argInfo.hasReg1?argInfo.reg1Base:INTEL_SIB_Base_None);
        pushByte(receiver, (Mod|op2Code|INTEL_ModRM_RM_NoDisplace));
        pushByte(receiver, (ScaleIndex|Base));
        if (argInfo.hasNumber || !argInfo.hasReg1) {// Displacement only needs to be added if it is desired. Except, for some reason... when there is not base of the formula [base+index*scale]
            if (argInfo.numValue>=128 || !argInfo.hasReg1) pushWord(receiver, argInfo.hasNumber?argInfo.numValue:0, true); // Push either the displacement or 0 if there is not displacement.
            else pushByte(receiver, argInfo.numValue);
        }
        std::cout << instructionName << " dword ptr " << std::to_string(argInfo) << std::endl;
    } else {
        // ex: instr (reg or [reg] or [reg+num])
        if (!argInfo.isIndirect && argInfo.bit==64) pushByte(receiver, INTEL_INSTR64_OperandSz_OVRD);
        if (argInfo.isIndirect && bitMode==64 && argInfo.bit==32) pushByte(receiver, INTEL_INSTR_AddrSz_OVRD);
        pushByte(receiver, INTEL_INSTR_OP2v);
        const uint8_t Mod = argInfo.isIndirect?(argInfo.hasNumber?((argInfo.numValue>=128)?INTEL_ModRM_MOD_4byteDisp:INTEL_ModRM_MOD_1byteDisp):INTEL_ModRM_MOD_Address):INTEL_ModRM_MOD_Reg;
        pushByte(receiver, ((Mod|op2Code|argInfo.reg1RM)));
        if (argInfo.hasNumber) {
            if (argInfo.numValue>=128) pushWord(receiver, argInfo.numValue, true);
            else pushByte(receiver, argInfo.numValue);
        }
        std::cout << instructionName << " " << (argInfo.isIndirect?"dword ptr ":"") << std::to_string(argInfo) << std::endl;
    }
}
template <typename T>
void UMUL(T &receiver, const char *arg) {
    MulImulDivIdiv(receiver,arg,"MUL",INTEL_ModRM_OP2_UMUL_eDX_eAX_RM);
}
template <typename T>
void MUL(T &receiver, const char *arg) {
    MulImulDivIdiv(receiver,arg,"IMUL",INTEL_ModRM_OP2_MUL_eDX_eAX_RM);
}
template <typename T>
void UDIV(T &receiver, const char *arg) {
    MulImulDivIdiv(receiver,arg,"DIV",INTEL_ModRM_OP2_UDIV_eDX_eAX_RM);
}
template <typename T>
void DIV(T &receiver, const char *arg) {
    MulImulDivIdiv(receiver,arg,"IDIV",INTEL_ModRM_OP2_DIV_eDX_eAX_RM);
}

template <typename T>
void CLC(T &receiver) {
    if (bitMode==0) return;
    pushByte(receiver, INTEL_INSTR_CLR_CF);
    std::cout << "CLC" << std::endl;
}
template <typename T>
void STC(T &receiver) {
    if (bitMode==0) return;
    pushByte(receiver, INTEL_INSTR_SET_CF);
    std::cout << "STC" << std::endl;
}
template <typename T>
void CLI(T &receiver) {
    if (bitMode==0) return;
    pushByte(receiver, INTEL_INSTR_CLR_IF);
    std::cout << "CLI" << std::endl;
}
template <typename T>
void STI(T &receiver) {
    if (bitMode==0) return;
    pushByte(receiver, INTEL_INSTR_SET_IF);
    std::cout << "STI" << std::endl;
}
template <typename T>
void CLD(T &receiver) {
    if (bitMode==0) return;
    pushByte(receiver, INTEL_INSTR_CLR_DF);
    std::cout << "CLD" << std::endl;
}
template <typename T>
void STD(T &receiver) {
    if (bitMode==0) return;
    pushByte(receiver, INTEL_INSTR_SET_DF);
    std::cout << "STD" << std::endl;
}

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
template void INC(std::ofstream& receiver, const char *arg);
template void DEC(std::ofstream& receiver, const char *arg);
template void CALL(std::ofstream& receiver, const char *arg);
//template void CALLF(std::ofstream& receiver, const char *arg);
template void JMP(std::ofstream& receiver, const char *arg);
//template void JMPF(std::ofstream& receiver, const char *arg);
template void PUSH(std::ofstream& receiver, const char *arg);
template void POP(std::ofstream& receiver, const char *arg);
/*template void JOV(std::ofstream& receiver, const char *arg);
template void JNOV(std::ofstream& receiver, const char *arg);
template void JULT(std::ofstream& receiver, const char *arg);
template void JUGE(std::ofstream& receiver, const char *arg);
template void JET(std::ofstream& receiver, const char *arg);
template void JNE(std::ofstream& receiver, const char *arg);
template void JULE(std::ofstream& receiver, const char *arg);
template void JUGT(std::ofstream& receiver, const char *arg);
template void JS(std::ofstream& receiver, const char *arg);
template void JNS(std::ofstream& receiver, const char *arg);
template void JP(std::ofstream& receiver, const char *arg);
template void JNP(std::ofstream& receiver, const char *arg);
template void JLT(std::ofstream& receiver, const char *arg);
template void JGE(std::ofstream& receiver, const char *arg);
template void JLE(std::ofstream& receiver, const char *arg);
template void JGT(std::ofstream& receiver, const char *arg);
template void JCXZ(std::ofstream& receiver, const char *arg);*/
template void NOP(std::ofstream& receiver);
template void NOP(std::ofstream& receiver, const char *arg);
template void XCHG(std::ofstream& receiver, const char *reg1, const char *reg2);
template void MOV(std::ofstream& receiver, const char *dst, const char *src);
template void RET(std::ofstream& receiver, const char *num);
template void RET(std::ofstream& receiver);
template void RETF(std::ofstream& receiver, const char *num);
template void RETF(std::ofstream& receiver);
template void INT(std::ofstream& receiver, const char *num);\
template void HLT(std::ofstream& receiver);
template void TEST(std::ofstream& receiver, const char *arg1, const char *arg2);
template void NOT(std::ofstream& receiver, const char *arg);
template void NEG(std::ofstream& receiver, const char *arg);
template void UMUL(std::ofstream& receiver, const char *arg);
template void MUL(std::ofstream& receiver, const char *arg);
template void UDIV(std::ofstream& receiver, const char *arg);
template void DIV(std::ofstream& receiver, const char *arg);
template void CLC(std::ofstream& receiver);
template void STC(std::ofstream& receiver);
template void CLI(std::ofstream& receiver);
template void STI(std::ofstream& receiver);
template void CLD(std::ofstream& receiver);
template void STD(std::ofstream& receiver);

//std::vector<uint8_t>
template void ADD(std::vector<uint8_t>& receiver, const char *dst, const char *src);
template void OR(std::vector<uint8_t>& receiver, const char *dst, const char *src);
template void ADC(std::vector<uint8_t>& receiver, const char *dst, const char *src);
template void SBB(std::vector<uint8_t>& receiver, const char *dst, const char *src);
template void AND(std::vector<uint8_t>& receiver, const char *dst, const char *src);
template void SUB(std::vector<uint8_t>& receiver, const char *dst, const char *src);
template void XOR(std::vector<uint8_t>& receiver, const char *dst, const char *src);
template void CMP(std::vector<uint8_t>& receiver, const char *dst, const char *src);
template void INC(std::vector<uint8_t>& receiver, const char *arg);
template void DEC(std::vector<uint8_t>& receiver, const char *arg);
template void CALL(std::vector<uint8_t>& receiver, const char *arg);
//template void CALLF(std::vector<uint8_t>& receiver, const char *arg);
template void JMP(std::vector<uint8_t>& receiver, const char *arg);
//template void JMPF(std::vector<uint8_t>& receiver, const char *arg);
template void PUSH(std::vector<uint8_t>& receiver, const char *arg);
template void POP(std::vector<uint8_t>& receiver, const char *arg);
/*template void JOV(std::vector<uint8_t>& receiver, const char *arg);
template void JNOV(std::vector<uint8_t>& receiver, const char *arg);
template void JULT(std::vector<uint8_t>& receiver, const char *arg);
template void JUGE(std::vector<uint8_t>& receiver, const char *arg);
template void JET(std::vector<uint8_t>& receiver, const char *arg);
template void JNE(std::vector<uint8_t>& receiver, const char *arg);
template void JULE(std::vector<uint8_t>& receiver, const char *arg);
template void JUGT(std::vector<uint8_t>& receiver, const char *arg);
template void JS(std::vector<uint8_t>& receiver, const char *arg);
template void JNS(std::vector<uint8_t>& receiver, const char *arg);
template void JP(std::vector<uint8_t>& receiver, const char *arg);
template void JNP(std::vector<uint8_t>& receiver, const char *arg);
template void JLT(std::vector<uint8_t>& receiver, const char *arg);
template void JGE(std::vector<uint8_t>& receiver, const char *arg);
template void JLE(std::vector<uint8_t>& receiver, const char *arg);
template void JGT(std::vector<uint8_t>& receiver, const char *arg);
template void JCXZ(std::vector<uint8_t>& receiver, const char *arg);*/
template void NOP(std::vector<uint8_t>& receiver);
template void NOP(std::vector<uint8_t>& receiver, const char *arg);
template void XCHG(std::vector<uint8_t>& receiver, const char *reg1, const char *reg2);
template void MOV(std::vector<uint8_t>& receiver, const char *dst, const char *src);
template void RET(std::vector<uint8_t>& receiver, const char *num);
template void RET(std::vector<uint8_t>& receiver);
template void RETF(std::vector<uint8_t>& receiver, const char *num);
template void RETF(std::vector<uint8_t>& receiver);
template void INT(std::vector<uint8_t>& receiver, const char *num);
template void HLT(std::vector<uint8_t>& receiver);
template void TEST(std::vector<uint8_t>& receiver, const char *arg1, const char *arg2);
template void NOT(std::vector<uint8_t>& receiver, const char *arg);
template void NEG(std::vector<uint8_t>& receiver, const char *arg);
template void UMUL(std::vector<uint8_t>& receiver, const char *arg);
template void MUL(std::vector<uint8_t>& receiver, const char *arg);
template void UDIV(std::vector<uint8_t>& receiver, const char *arg);
template void DIV(std::vector<uint8_t>& receiver, const char *arg);
template void CLC(std::vector<uint8_t>& receiver);
template void STC(std::vector<uint8_t>& receiver);
template void CLI(std::vector<uint8_t>& receiver);
template void STI(std::vector<uint8_t>& receiver);
template void CLD(std::vector<uint8_t>& receiver);
template void STD(std::vector<uint8_t>& receiver);

//ElfSegmentHandler *
template void ADD(ElfSegmentHandler*& receiver, const char *dst, const char *src);
template void OR(ElfSegmentHandler*& receiver, const char *dst, const char *src);
template void ADC(ElfSegmentHandler*& receiver, const char *dst, const char *src);
template void SBB(ElfSegmentHandler*& receiver, const char *dst, const char *src);
template void AND(ElfSegmentHandler*& receiver, const char *dst, const char *src);
template void SUB(ElfSegmentHandler*& receiver, const char *dst, const char *src);
template void XOR(ElfSegmentHandler*& receiver, const char *dst, const char *src);
template void CMP(ElfSegmentHandler*& receiver, const char *dst, const char *src);
template void INC(ElfSegmentHandler*& receiver, const char *arg);
template void DEC(ElfSegmentHandler*& receiver, const char *arg);
template void CALL(ElfSegmentHandler*& receiver, const char *arg);
//template void CALLF(ElfSegmentHandler*& receiver, const char *arg);
template void JMP(ElfSegmentHandler*& receiver, const char *arg);
//template void JMPF(ElfSegmentHandler*& receiver, const char *arg);
template void PUSH(ElfSegmentHandler*& receiver, const char *arg);
template void POP(ElfSegmentHandler*& receiver, const char *arg);
/*template void JOV(ElfSegmentHandler*& receiver, const char *arg);
template void JNOV(ElfSegmentHandler*& receiver, const char *arg);
template void JULT(ElfSegmentHandler*& receiver, const char *arg);
template void JUGE(ElfSegmentHandler*& receiver, const char *arg);
template void JET(ElfSegmentHandler*& receiver, const char *arg);
template void JNE(ElfSegmentHandler*& receiver, const char *arg);
template void JULE(ElfSegmentHandler*& receiver, const char *arg);
template void JUGT(ElfSegmentHandler*& receiver, const char *arg);
template void JS(ElfSegmentHandler*& receiver, const char *arg);
template void JNS(ElfSegmentHandler*& receiver, const char *arg);
template void JP(ElfSegmentHandler*& receiver, const char *arg);
template void JNP(ElfSegmentHandler*& receiver, const char *arg);
template void JLT(ElfSegmentHandler*& receiver, const char *arg);
template void JGE(ElfSegmentHandler*& receiver, const char *arg);
template void JLE(ElfSegmentHandler*& receiver, const char *arg);
template void JGT(ElfSegmentHandler*& receiver, const char *arg);
template void JCXZ(ElfSegmentHandler*& receiver, const char *arg);*/
template void NOP(ElfSegmentHandler*& receiver);
template void NOP(ElfSegmentHandler*& receiver, const char *arg);
template void XCHG(ElfSegmentHandler*& receiver, const char *reg1, const char *reg2);
template void MOV(ElfSegmentHandler*& receiver, const char *dst, const char *src);
template void RET(ElfSegmentHandler*& receiver, const char *num);
template void RET(ElfSegmentHandler*& receiver);
template void RETF(ElfSegmentHandler*& receiver, const char *num);
template void RETF(ElfSegmentHandler*& receiver);
template void INT(ElfSegmentHandler*& receiver, const char *num);
template void HLT(ElfSegmentHandler*& receiver);
template void TEST(ElfSegmentHandler*& receiver, const char *arg1, const char *arg2);
template void NOT(ElfSegmentHandler*& receiver, const char *arg);
template void NEG(ElfSegmentHandler*& receiver, const char *arg);
template void UMUL(ElfSegmentHandler*& receiver, const char *arg);
template void MUL(ElfSegmentHandler*& receiver, const char *arg);
template void UDIV(ElfSegmentHandler*& receiver, const char *arg);
template void DIV(ElfSegmentHandler*& receiver, const char *arg);
template void CLC(ElfSegmentHandler*& receiver);
template void STC(ElfSegmentHandler*& receiver);
template void CLI(ElfSegmentHandler*& receiver);
template void STI(ElfSegmentHandler*& receiver);
template void CLD(ElfSegmentHandler*& receiver);
template void STD(ElfSegmentHandler*& receiver);

//Pe32SectionHandler *
template void ADD(Pe32SectionHandler*& receiver, const char *dst, const char *src);
template void OR(Pe32SectionHandler*& receiver, const char *dst, const char *src);
template void ADC(Pe32SectionHandler*& receiver, const char *dst, const char *src);
template void SBB(Pe32SectionHandler*& receiver, const char *dst, const char *src);
template void AND(Pe32SectionHandler*& receiver, const char *dst, const char *src);
template void SUB(Pe32SectionHandler*& receiver, const char *dst, const char *src);
template void XOR(Pe32SectionHandler*& receiver, const char *dst, const char *src);
template void CMP(Pe32SectionHandler*& receiver, const char *dst, const char *src);
template void INC(Pe32SectionHandler*& receiver, const char *arg);
template void DEC(Pe32SectionHandler*& receiver, const char *arg);
template void CALL(Pe32SectionHandler*& receiver, const char *arg);
//template void CALLF(Pe32SectionHandler*& receiver, const char *arg);
template void JMP(Pe32SectionHandler*& receiver, const char *arg);
//template void JMPF(Pe32SectionHandler*& receiver, const char *arg);
template void PUSH(Pe32SectionHandler*& receiver, const char *arg);
template void POP(Pe32SectionHandler*& receiver, const char *arg);
/*template void JOV(Pe32SectionHandler*& receiver, const char *arg);
template void JNOV(Pe32SectionHandler*& receiver, const char *arg);
template void JULT(Pe32SectionHandler*& receiver, const char *arg);
template void JUGE(Pe32SectionHandler*& receiver, const char *arg);
template void JET(Pe32SectionHandler*& receiver, const char *arg);
template void JNE(Pe32SectionHandler*& receiver, const char *arg);
template void JULE(Pe32SectionHandler*& receiver, const char *arg);
template void JUGT(Pe32SectionHandler*& receiver, const char *arg);
template void JS(Pe32SectionHandler*& receiver, const char *arg);
template void JNS(Pe32SectionHandler*& receiver, const char *arg);
template void JP(Pe32SectionHandler*& receiver, const char *arg);
template void JNP(Pe32SectionHandler*& receiver, const char *arg);
template void JLT(Pe32SectionHandler*& receiver, const char *arg);
template void JGE(Pe32SectionHandler*& receiver, const char *arg);
template void JLE(Pe32SectionHandler*& receiver, const char *arg);
template void JGT(Pe32SectionHandler*& receiver, const char *arg);
template void JCXZ(Pe32SectionHandler*& receiver, const char *arg);*/
template void NOP(Pe32SectionHandler*& receiver);
template void NOP(Pe32SectionHandler*& receiver, const char *arg);
template void XCHG(Pe32SectionHandler*& receiver, const char *reg1, const char *reg2);
template void MOV(Pe32SectionHandler*& receiver, const char *dst, const char *src);
template void RET(Pe32SectionHandler*& receiver, const char *num);
template void RET(Pe32SectionHandler*& receiver);
template void RETF(Pe32SectionHandler*& receiver, const char *num);
template void RETF(Pe32SectionHandler*& receiver);
template void INT(Pe32SectionHandler*& receiver, const char *num);
template void HLT(Pe32SectionHandler*& receiver);
template void TEST(Pe32SectionHandler*& receiver, const char *arg1, const char *arg2);
template void NOT(Pe32SectionHandler*& receiver, const char *arg);
template void NEG(Pe32SectionHandler*& receiver, const char *arg);
template void UMUL(Pe32SectionHandler*& receiver, const char *arg);
template void MUL(Pe32SectionHandler*& receiver, const char *arg);
template void UDIV(Pe32SectionHandler*& receiver, const char *arg);
template void DIV(Pe32SectionHandler*& receiver, const char *arg);
template void CLC(Pe32SectionHandler*& receiver);
template void STC(Pe32SectionHandler*& receiver);
template void CLI(Pe32SectionHandler*& receiver);
template void STI(Pe32SectionHandler*& receiver);
template void CLD(Pe32SectionHandler*& receiver);
template void STD(Pe32SectionHandler*& receiver);

//Pe64SectionHandler *
template void ADD(Pe64SectionHandler*& receiver, const char *dst, const char *src);
template void OR(Pe64SectionHandler*& receiver, const char *dst, const char *src);
template void ADC(Pe64SectionHandler*& receiver, const char *dst, const char *src);
template void SBB(Pe64SectionHandler*& receiver, const char *dst, const char *src);
template void AND(Pe64SectionHandler*& receiver, const char *dst, const char *src);
template void SUB(Pe64SectionHandler*& receiver, const char *dst, const char *src);
template void XOR(Pe64SectionHandler*& receiver, const char *dst, const char *src);
template void CMP(Pe64SectionHandler*& receiver, const char *dst, const char *src);
template void INC(Pe64SectionHandler*& receiver, const char *arg);
template void DEC(Pe64SectionHandler*& receiver, const char *arg);
template void CALL(Pe64SectionHandler*& receiver, const char *arg);
//template void CALLF(Pe64SectionHandler*& receiver, const char *arg);
template void JMP(Pe64SectionHandler*& receiver, const char *arg);
//template void JMPF(Pe64SectionHandler*& receiver, const char *arg);
template void PUSH(Pe64SectionHandler*& receiver, const char *arg);
template void POP(Pe64SectionHandler*& receiver, const char *arg);
/*template void JOV(Pe64SectionHandler*& receiver, const char *arg);
template void JNOV(Pe64SectionHandler*& receiver, const char *arg);
template void JULT(Pe64SectionHandler*& receiver, const char *arg);
template void JUGE(Pe64SectionHandler*& receiver, const char *arg);
template void JET(Pe64SectionHandler*& receiver, const char *arg);
template void JNE(Pe64SectionHandler*& receiver, const char *arg);
template void JULE(Pe64SectionHandler*& receiver, const char *arg);
template void JUGT(Pe64SectionHandler*& receiver, const char *arg);
template void JS(Pe64SectionHandler*& receiver, const char *arg);
template void JNS(Pe64SectionHandler*& receiver, const char *arg);
template void JP(Pe64SectionHandler*& receiver, const char *arg);
template void JNP(Pe64SectionHandler*& receiver, const char *arg);
template void JLT(Pe64SectionHandler*& receiver, const char *arg);
template void JGE(Pe64SectionHandler*& receiver, const char *arg);
template void JLE(Pe64SectionHandler*& receiver, const char *arg);
template void JGT(Pe64SectionHandler*& receiver, const char *arg);
template void JCXZ(Pe64SectionHandler*& receiver, const char *arg);*/
template void NOP(Pe64SectionHandler*& receiver);
template void NOP(Pe64SectionHandler*& receiver, const char *arg);
template void XCHG(Pe64SectionHandler*& receiver, const char *reg1, const char *reg2);
template void MOV(Pe64SectionHandler*& receiver, const char *dst, const char *src);
template void RET(Pe64SectionHandler*& receiver, const char *num);
template void RET(Pe64SectionHandler*& receiver);
template void RETF(Pe64SectionHandler*& receiver, const char *num);
template void RETF(Pe64SectionHandler*& receiver);
template void INT(Pe64SectionHandler*& receiver, const char *num);
template void HLT(Pe64SectionHandler*& receiver);
template void TEST(Pe64SectionHandler*& receiver, const char *arg1, const char *arg2);
template void NOT(Pe64SectionHandler*& receiver, const char *arg);
template void NEG(Pe64SectionHandler*& receiver, const char *arg);
template void UMUL(Pe64SectionHandler*& receiver, const char *arg);
template void MUL(Pe64SectionHandler*& receiver, const char *arg);
template void UDIV(Pe64SectionHandler*& receiver, const char *arg);
template void DIV(Pe64SectionHandler*& receiver, const char *arg);
template void CLC(Pe64SectionHandler*& receiver);
template void STC(Pe64SectionHandler*& receiver);
template void CLI(Pe64SectionHandler*& receiver);
template void STI(Pe64SectionHandler*& receiver);
template void CLD(Pe64SectionHandler*& receiver);
template void STD(Pe64SectionHandler*& receiver);

#pragma endregion template instatiations

#pragma endregion instructions
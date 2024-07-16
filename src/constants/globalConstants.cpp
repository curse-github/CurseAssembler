#include <string.h>
#include <iostream>

#include "globalConstants.h"
#include "intelConstants.h"

#pragma region helper functions
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

void pushByte(std::ofstream &stream, const uint8_t &byte) {
    pushChars(stream, &byte, 1, false);
}
void pushByte(std::vector<uint8_t> &vector, const uint8_t &byte) {
    pushChars(vector, &byte, 1, false);
}

void pushHalfWord(std::ofstream &stream, const uint16_t &halfword, const bool &LSB) {
    pushChars(stream, static_cast<const uint8_t *>(static_cast<const void *>(&halfword)), 2, LSB);
}
void pushHalfWord(std::vector<uint8_t> &vector, const uint16_t &halfword, const bool &LSB) {
    pushChars(vector, static_cast<const uint8_t *>(static_cast<const void *>(&halfword)), 2, LSB);
}

void pushWord(std::ofstream &stream, const uint32_t &word, const bool &LSB) {
    pushChars(stream, static_cast<const uint8_t *>(static_cast<const void *>(&word)), 4, LSB);
}
void pushWord(std::vector<uint8_t> &vector, const uint32_t &word, const bool &LSB) {
    pushChars(vector, static_cast<const uint8_t *>(static_cast<const void *>(&word)), 4, LSB);
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
    unsigned char bit;
    uint8_t offset;
    uint8_t regOp;
    uint8_t RM;
};
registerInfo processReg(const char* reg, const size_t& len) {
    registerInfo info;
    if (len==3u && reg[0]=='r') {// all 64 bit registers are 3 characters long and start with r
        info.bit=64u;
        if ((reg[1]=='a')&&(reg[2]=='x')) {
            info.offset=INTEL_REG_OFF_rAX;
        } else if ((reg[1]=='c')&&(reg[2]=='x')) {
            info.offset=INTEL_REG_OFF_rCX;
        } else if ((reg[1]=='d')&&(reg[2]=='x')) {
            info.offset=INTEL_REG_OFF_rDX;
        } else if ((reg[1]=='b')&&(reg[2]=='x')) {
            info.offset=INTEL_REG_OFF_rBX;
        } else if ((reg[1]=='s')&&(reg[2]=='p')) {
            info.offset=INTEL_REG_OFF_rSP;
        } else if ((reg[1]=='b')&&(reg[2]=='p')) {
            info.offset=INTEL_REG_OFF_rBP;
        } else if ((reg[1]=='s')&&(reg[2]=='i')) {
            info.offset=INTEL_REG_OFF_rSI;
        } else if ((reg[1]=='d')&&(reg[2]=='i')) {
            info.offset=INTEL_REG_OFF_rDI;
        } else { info.isValid=false; return info; }
    } else if (len==3u && reg[0]=='e') {// all 32 bit registers are 3 characters long and start with e
        info.bit=32u;
        if ((reg[1]=='a')&&(reg[2]=='x')) {
            info.offset=INTEL_REG_OFF_eAX;
        } else if ((reg[1]=='c')&&(reg[2]=='x')) {
            info.offset=INTEL_REG_OFF_eCX;
        } else if ((reg[1]=='d')&&(reg[2]=='x')) {
            info.offset=INTEL_REG_OFF_eDX;
        } else if ((reg[1]=='b')&&(reg[2]=='x')) {
            info.offset=INTEL_REG_OFF_eBX;
        } else if ((reg[1]=='s')&&(reg[2]=='p')) {
            info.offset=INTEL_REG_OFF_eSP;
        } else if ((reg[1]=='b')&&(reg[2]=='p')) {
            info.offset=INTEL_REG_OFF_eBP;
        } else if ((reg[1]=='s')&&(reg[2]=='i')) {
            info.offset=INTEL_REG_OFF_eSI;
        } else if ((reg[1]=='d')&&(reg[2]=='i')) {
            info.offset=INTEL_REG_OFF_eDI;
        } else { info.isValid=false; return info; }
    } else if (len==2u && (reg[1]=='h' || reg[1]=='l')) {// all 8 bit registers are 2 characters long and end with either h or l
        info.bit=8u;
        if ((reg[0]=='a')) {
            info.offset=INTEL_REG_OFF_AL;
        } else if ((reg[0]=='c')) {
            info.offset=INTEL_REG_OFF_CL;
        } else if ((reg[0]=='d')) {
            info.offset=INTEL_REG_OFF_DL;
        } else if ((reg[0]=='b')) {
            info.offset=INTEL_REG_OFF_BL;
        } else { info.isValid=false; return info; }
        if (reg[1]=='h') info.offset+=4;
    } else {//most likely either a 16 bit rigister or invalid
        info.bit=16u;
        if ((reg[0]=='a')&&(reg[1]=='x')) {
            info.offset=INTEL_REG_OFF_AX;
        } else if ((reg[0]=='c')&&(reg[1]=='x')) {
            info.offset=INTEL_REG_OFF_CX;
        } else if ((reg[0]=='d')&&(reg[1]=='x')) {
            info.offset=INTEL_REG_OFF_DX;
        } else if ((reg[0]=='b')&&(reg[1]=='x')) {
            info.offset=INTEL_REG_OFF_BX;
        } else if ((reg[0]=='s')&&(reg[1]=='p')) {
            info.offset=INTEL_REG_OFF_SP;
        } else if ((reg[0]=='b')&&(reg[1]=='p')) {
            info.offset=INTEL_REG_OFF_BP;
        } else if ((reg[0]=='s')&&(reg[1]=='i')) {
            info.offset=INTEL_REG_OFF_SI;
        } else if ((reg[0]=='d')&&(reg[1]=='i')) {
            info.offset=INTEL_REG_OFF_DI;
        } else { info.isValid=false; return info; }
    }
    info.regOp=info.offset<<3;
    info.RM=info.offset;
    return info;
}

struct numberInfo {
    bool isValid=true;
    unsigned long value;
};

numberInfo processNum(const char* num, const size_t& len) {
    numberInfo info;
    std::string numStr = "";
    unsigned int numBase = 10;
    if (num[0]=='0'&&(num[1]=='b'||num[1]=='d'||num[1]=='x')) {// for numbers in the form of 0xb0001101, 0d25, or 0x19
        // loop through string until it reaches 20 characters, or finds the ending square bracket
        bool foundEnd = false;
        for (unsigned int i = 0; i < 20; i++) {
            if (num[i+2]==']'||num[i+2]=='\0') {foundEnd=((len-2-i)<=1); break; }// found ending square bracket, however... if the end of the square bracket is not the end of the argument, it is invalid
            else if (num[i+2]>='0' && num[i+2]<='9') {
                numStr+=num[i+2];
            } else break;// if there is a character that is not a 
        }
        if (!foundEnd) { info.isValid=false; return info; }
        // parse the number out of the string, in the correct base based on the number abbreviation
        if (num[1]=='b') numBase=2;
        else if (num[1]=='x') numBase=16;
    } else {
        // loop through string until it reaches 20 characters, or finds the ending square bracket
        bool foundEnd = false;
        for (unsigned int i = 0; i < 20; i++) {
            if (num[i]==']'||num[i]=='\0') {foundEnd=((len-i)<=1); break; }// found ending square bracket, however... if the end of the square bracket is not the end of the argument, it is invalid
            else if (num[i]>='0' && num[i]<='9') {
                numStr+=num[i];
            } else break;// if there is a character that is not a 
        }
        if (!foundEnd) { info.isValid=false; return info; }
    }
    char* p;
    info.value=strtol(numStr.c_str(),&p,numBase);// the character p represents the first character that "strtol" finds that is not valid, or '\0' if it did not find an invalid character
    if (*p!=0) { info.isValid=false; return info; }
    return info;
}

struct instructionArgInfo { // isRegister and isNumber will only ever be true if isIndirect is true
    bool isValid=true;
    bool isIndirect=false;
    size_t strSize;
    unsigned char bit=0; // for a number, this is irrelavent
    bool hasRegister=false;
    uint8_t regOffset;
    uint8_t regRegOp;
    uint8_t regRM;
    bool hasNumber=false;
    unsigned long numValue;
};
instructionArgInfo processArg(const char* arg) {
    instructionArgInfo info;
    info.strSize = std::string(arg).size();
    if (arg[0]=='[') {
        info.isIndirect=true;
        if (arg[0]>='0' && arg[0]<='9') {
            // parse a number
            numberInfo numInfo = processNum(arg,info.strSize);
            if (!numInfo.isValid) { info.isValid=false; return info;}
            info.hasNumber=true;
            info.numValue=numInfo.value;
        } else {// most likely either a register or invalid
            // check if it seems like it
            size_t regLen = 0;
            const char firstChar = std::tolower(arg[1]);
            if (firstChar=='e' || firstChar=='r') regLen=3; // most likely eax,ecx,edx,ebx,esp,ebp,esi,edi, rax,rcx,rdx,rbx,rsp,rbp,rsi,or rdi
            else if (firstChar=='a' || firstChar=='c' || firstChar=='d' || firstChar=='b' || firstChar=='s' || firstChar=='b') regLen=2;// most likely ax,cx,dx,bx,sp,bp,si,di, ah,ch,dh,bh,al,cl,dl,or bl
            else { info.isValid=false; return info; }
            // parse a register
            if (arg[regLen+1]=='+') {
                info.hasNumber=true;
            } else if (arg[regLen+1]!=']') { info.isValid=false; return info; }
            std::string regRef = ((regLen==3)?"aaa":"aa");
            regRef[0]=std::tolower(arg[1]); regRef[1]=std::tolower(arg[2]); if (regLen==3) regRef[2]=std::tolower(arg[3]);// copy first 2 or 3 characters into "regRef"
            registerInfo regInfo = processReg(regRef.c_str(), regLen);
            if (!regInfo.isValid) { info.isValid=false; return info; }
            info.hasRegister = true;
            info.bit=regInfo.bit;
            info.regOffset = regInfo.offset;
            info.regRegOp = regInfo.regOp;
            info.regRM = regInfo.RM;
            if (info.hasNumber) {
                size_t numberStartingIndex = 2u+regLen;// plus 2, one for square bracket, and one for the plus sign
                numberInfo numInfo = processNum(arg+numberStartingIndex,info.strSize-numberStartingIndex);
                if (!numInfo.isValid) { info.isValid=false; return info;}
                info.numValue=numInfo.value;
            }
        }
    } else if (arg[0]>='0' && arg[0]<='9') {
        // parse a number
        numberInfo numInfo = processNum(arg,info.strSize);
        if (!numInfo.isValid) { info.isValid=false; return info;}
        info.hasNumber=true;
        info.numValue=numInfo.value;
    } else {// most likely either a register or invalid
        // check if it seems like it 
        const char firstChar = std::tolower(arg[0]);
        if (info.strSize<2||info.strSize>3) { info.isValid=false; return info; }
        // parse a register
        std::string regRef=((info.strSize==3)?"aaa":"aa");
        regRef[0]=std::tolower(arg[0]); regRef[1]=std::tolower(arg[1]); if (info.strSize==3) regRef[2]=std::tolower(arg[2]);// copy first 2 or 3 characters into "regRef"
        registerInfo regInfo = processReg(regRef.c_str(), regRef.size());
        if (!regInfo.isValid) { info.isValid=false; return info; }
        info.hasRegister=true;
        info.bit=regInfo.bit;
        info.regOffset=regInfo.offset;
        info.regRegOp=regInfo.regOp;
        info.regRM=regInfo.RM;
    }
    return info;
}

std::ostream& operator<<(std::ostream& o, const instructionArgInfo& info) {
    o << "{" << std::endl;
    o << "\tisValid: " << (info.isValid?"true":"false");
    if (!info.isValid) return o << std::endl << "}";
    else o << "," << std::endl;
    o << "\tisIndirect: " << (info.isIndirect?"true,":"false,") << std::endl;
    o << "\tstrSize: " << (int)info.strSize << "," << std::endl;
    o << "\tbit: " << (int)info.bit << "," << std::endl;
    o << "\thasRegister: " << (info.hasRegister?"true,":"false,") << std::endl;
    o << "\tregOffset: " << (int)info.regOffset << "," << std::endl;
    o << "\tregRegOp: " << (int)info.regRegOp << "," << std::endl;
    o << "\tregRM: " << (int)info.regRM << "," << std::endl;
    o << "\thasNumber: " << (info.hasNumber?"true,":"false,") << std::endl;
    o << "\tnumValue: " << info.numValue << "," << std::endl;
    return o << "}";
}

#pragma endregion helper functions

#pragma region instructions

#pragma region ADD
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
    instructionArgInfo dstInfo = processArg(dst);
    instructionArgInfo srcInfo = processArg(src);
    std::cout << "dst: " << dstInfo << std::endl;
    std::cout << "src: " << srcInfo << std::endl;
}
#pragma endregion ADD


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
template void ADD(std::ofstream &receiver, const char *acc, const char *src);
template void ORb(std::ofstream &receiver, const char *reg1, const char *reg2);
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
template void STD(std::ofstream &receiver);

//std::vector<uint8_t>
template void ADD(std::vector<uint8_t> &receiver, const char *acc, const char *src);
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
template void STD(std::vector<uint8_t> &receiver);

//ElfSegmentHandler *
template void ADD(ElfSegmentHandler *&receiver, const char *acc, const char *src);
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
template void STD(ElfSegmentHandler *&receiver);

//Pe32SectionHandler *
template void ADD(Pe32SectionHandler *&receiver, const char *acc, const char *src);
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
template void STD(Pe32SectionHandler *&receiver);

//Pe64SectionHandler *
template void ADD(Pe64SectionHandler *&receiver, const char *acc, const char *src);
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
template void STD(Pe64SectionHandler *&receiver);
#pragma endregion template instatiations

#pragma endregion instructions
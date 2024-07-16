#include <cstdint>
#include <string>
#include <stdlib.h>

#pragma region stuff
//64 bit
#define INTEL_REG_OFF_rAX 0
#define INTEL_REG_OFF_rCX 1
#define INTEL_REG_OFF_rDX 2
#define INTEL_REG_OFF_rBX 3
#define INTEL_REG_OFF_rSP 4
#define INTEL_REG_OFF_rBP 5
#define INTEL_REG_OFF_rSI 6
#define INTEL_REG_OFF_rDI 7
//32 bit
#define INTEL_REG_OFF_eAX 0
#define INTEL_REG_OFF_eCX 1
#define INTEL_REG_OFF_eDX 2
#define INTEL_REG_OFF_eBX 3
#define INTEL_REG_OFF_eSP 4
#define INTEL_REG_OFF_eBP 5
#define INTEL_REG_OFF_eSI 6
#define INTEL_REG_OFF_eDI 7
//16 bit
#define INTEL_REG_OFF_AX 0
#define INTEL_REG_OFF_CX 1
#define INTEL_REG_OFF_DX 2
#define INTEL_REG_OFF_BX 3
#define INTEL_REG_OFF_SP 4
#define INTEL_REG_OFF_BP 5
#define INTEL_REG_OFF_SI 6
#define INTEL_REG_OFF_DI 7
//8 bit
#define INTEL_REG_OFF_AL 0
#define INTEL_REG_OFF_CL 1
#define INTEL_REG_OFF_DL 2
#define INTEL_REG_OFF_BL 3
#define INTEL_REG_OFF_AH 4
#define INTEL_REG_OFF_CH 5
#define INTEL_REG_OFF_DH 6
#define INTEL_REG_OFF_BH 7
#pragma endregion stuff

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

#include <iostream>
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

int main(int argc, char** argv) {
    if (argc<2) return 1;
    std::cout << "\"" << argv[1] << "\":" << std::endl;
    std::cout << processArg(argv[1]) << std::endl;
    return 0;
}
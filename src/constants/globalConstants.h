#ifndef _GLOBAL_CONSTANTS_H
#define _GLOBAL_CONSTANTS_H

#include <fstream>
#include <vector>

const unsigned int VirtAddr32 = 0x00400000;
const unsigned int Align32 = 0x00000200;
const unsigned long VirtAddr64 = 0x0000000000400000;
const unsigned long Align64 = 0x0000000000000200;

void pushChars(std::ofstream &stream, const unsigned char *chars, unsigned int len, const bool &LSB);
void pushChars(std::vector<unsigned char> &vector, const unsigned char *chars, unsigned int len, const bool &LSB);
void pushChars(std::ofstream &stream, std::vector<unsigned char> &vector);

void pushByte(std::ofstream &stream, const unsigned char &byte);
void pushByte(std::vector<unsigned char> &vector, const unsigned char &byte);

void pushHalfWord(std::ofstream &stream, const unsigned short &halfword, const bool &LSB);
void pushHalfWord(std::vector<unsigned char> &vector, const unsigned short &halfword, const bool &LSB);

void pushWord(std::ofstream &stream, const unsigned int &word, const bool &LSB);
void pushWord(std::vector<unsigned char> &vector, const unsigned int &word, const bool &LSB);

void pushDword(std::ofstream &stream, const unsigned long &dword, const bool &LSB);
void pushDword(std::vector<unsigned char> &vector, const unsigned long &dword, const bool &LSB);

void padBytes(std::ofstream &stream, const unsigned int &numBytes);

//32 bit reg helpers, for registers eAX, eCX, eDX, eBX, eSP, eBP, eSI, and eDI
unsigned char RegToOffset32(const char *reg);
unsigned char RegToModrmReg32(const char *reg);
unsigned char RegToModrmRM32(const char *reg);
//16 bit reg helpers, for registers AX, CX, DX, BX, SP, BP, SI, and DI
unsigned char RegToOffset16(const char *reg);
unsigned char RegToModrmReg16(const char *reg);
unsigned char RegToModrmRM16(const char *reg);
//8 bit reg helpers, for registers AL, CL, DL, BL, AH, CH, DH, and BH
unsigned char RegToOffset8(const char *reg);
unsigned char RegToModrmReg8(const char *reg);
unsigned char RegToModrmRM8(const char *reg);

// uses codes 0x00 - 0x05 as well as general instructions 0x80 - 0x83 with modrm OP bits set to 0
template<typename T>
void ADDb(const T &receiver, const char *&reg1, const char *&reg2);
template<typename T>
void ADDb(const T &receiver, const char *&reg, const unsigned char &value);
template<typename T>
void ADDv(const T &receiver, const char *&reg1, const char *&reg2);
template<typename T>
void ADDv(const T &receiver, const char *&reg, const unsigned int &value);

// uses codes 0x08 - 0x0D as well as general instructions 0x80 - 0x83 with modrm OP bits set to 1
template<typename T>
void ORb(const T &receiver, const char *&reg1, const char *&reg2);
template<typename T>
void ORb(const T &receiver, const char *&reg, const unsigned char &value);
template<typename T>
void ORv(const T &receiver, const char *&reg1, const char *&reg2);
template<typename T>
void ORv(const T &receiver, const char *&reg, const unsigned int &value);

// uses codes 0x10 - 0x15 as well as general instructions 0x80 - 0x83 with modrm OP bits set to 2
template<typename T>
void ADCb(const T &receiver, const char *&reg1, const char *&reg2);
template<typename T>
void ADCb(const T &receiver, const char *&reg, const unsigned char &value);
template<typename T>
void ADCv(const T &receiver, const char *&reg1, const char *&reg2);
template<typename T>
void ADCv(const T &receiver, const char *&reg, const unsigned int &value);

// uses codes 0x18 - 0x1D as well as general instructions 0x80 - 0x83 with modrm OP bits set to 3
template<typename T>
void SBBb(const T &receiver, const char *&reg1, const char *&reg2);
template<typename T>
void SBBb(const T &receiver, const char *&reg, const unsigned char &value);
template<typename T>
void SBBv(const T &receiver, const char *&reg1, const char *&reg2);
template<typename T>
void SBBv(const T &receiver, const char *&reg, const unsigned int &value);

// uses codes 0x20 - 0x25 as well as general instructions 0x80 - 0x83 with modrm OP bits set to 4
template<typename T>
void ANDb(const T &receiver, const char *&reg1, const char *&reg2);
template<typename T>
void ANDb(const T &receiver, const char *&reg, const unsigned char &value);
template<typename T>
void ANDv(const T &receiver, const char *&reg1, const char *&reg2);
template<typename T>
void ANDv(const T &receiver, const char *&reg, const unsigned int &value);

// uses codes 0x28 - 0x2D as well as general instructions 0x80 - 0x83 with modrm OP bits set to 5
template<typename T>
void SUBb(const T &receiver, const char *&reg1, const char *&reg2);
template<typename T>
void SUBb(const T &receiver, const char *&reg, const unsigned char &value);
template<typename T>
void SUBv(const T &receiver, const char *&reg1, const char *&reg2);
template<typename T>
void SUBv(const T &receiver, const char *&reg, const unsigned int &value);

// uses codes 0x30 - 0x35 as well as general instructions 0x80 - 0x83 with modrm OP bits set to 6
template<typename T>
void XORb(const T &receiver, const char *&reg1, const char *&reg2);
template<typename T>
void XORb(const T &receiver, const char *&reg, const unsigned char &value);
template<typename T>
void XORv(const T &receiver, const char *&reg1, const char *&reg2);
template<typename T>
void XORv(const T &receiver, const char *&reg, const unsigned int &value);

// uses codes 0x38 - 0x3D as well as general instructions 0x80 - 0x83 with modrm OP bits set to 7
template<typename T>
void CMPb(const T &receiver, const char *&reg1, const char *&reg2);
template<typename T>
void CMPb(const T &receiver, const char *&reg, const unsigned char &value);
template<typename T>
void CMPv(const T &receiver, const char *&reg1, const char *&reg2);
template<typename T>
void CMPv(const T &receiver, const char *&reg, const unsigned int &value);

//uses codes 0x40 - 0x47 as well as general instructions 0xFE and 0xFF with modrm OP bits set to 0 for indirect
template <typename T>
void INCb(const T &receiver, const char *&reg);
template <typename T>
void INCv(const T &receiver, const char *&reg);

//uses codes 0x48 - 0x4F as well as general instructions 0xFE and 0xFF with modrm OP bits set to 1 for indirect
template <typename T>
void DECb(const T &receiver, const char *&reg);
template <typename T>
void DECv(const T &receiver, const char *&reg);

//uses code 0xE8
template <typename T>
void CALL(const T &receiver, const unsigned int &value);//relative
//uses general instruction 0xFF with modrm OP bits set to 2 for indirect addressing
template <typename T>
void CALL(const T &receiver, const char *&reg);
//uses code 0x9A as well as the general instruction 0xFF with modrm OP bits set to 3 for indirect
// takes 5 bytes
template <typename T>
void CALLF(const T &receiver, const unsigned long &value){};

//uses code 0xE9
template <typename T>
void JMPv(const T &receiver, const unsigned int &value);
//uses general instruction 0xFF with modrm OP bits set to 4 for indirect addressing
template <typename T>
void JMPv(const T &receiver, const char *&value);
//uses code 0xEB
template <typename T>
void JMPb(const T &receiver, const unsigned char &value);
//uses code 0xEA as well as the general instruction 0xFF with modrm OP bits set to 5 for indirect
//    0xEA takes A whereas 0xFF takes m16
// takes 5 bytes
template <typename T>
void JMPF(const T &receiver, const unsigned long &value){};

//uses codes 0x50 - 0x57 as well as the general instruction 0xFF with modrm OP bits set to 6 for indirect
template <typename T>
void PUSHv(const T &receiver, const char *&reg);
//uses code 0x68
template <typename T>
void PUSHv(const T &receiver, const unsigned int &value);
//uses code 0x6A
template <typename T>
void PUSHb(const T &receiver, const unsigned char &value);

//uses codes 0x58 - 0x5F and possibly 0x8F
template <typename T>
void POP(const T &receiver, const char *&reg);

#pragma region jump if instructions

//uses code 0x70
template <typename T>
void JOV(const T &receiver, const unsigned char &value);
//uses code 0x71
template <typename T>
void JNOV(const T &receiver, const unsigned char &value);
//uses code 0x72
template <typename T>
void JULT(const T &receiver, const unsigned char &value);
//uses code 0x73
template <typename T>
void JUGT(const T &receiver, const unsigned char &value);
//uses code 0x74
template <typename T>
void JET(const T &receiver, const unsigned char &value);
//uses code 0x75
template <typename T>
void JNE(const T &receiver, const unsigned char &value);
//uses code 0x76
template <typename T>
void JULE(const T &receiver, const unsigned char &value);
//uses code 0x77
template <typename T>
void JUGT(const T &receiver, const unsigned char &value);
//uses code 0x78
template <typename T>
void JS(const T &receiver, const unsigned char &value);
//uses code 0x79
template <typename T>
void JNS(const T &receiver, const unsigned char &value);
//uses code 0x7A
template <typename T>
void JP(const T &receiver, const unsigned char &value);
//uses code 0x7B
template <typename T>
void JNP(const T &receiver, const unsigned char &value);
//uses code 0x7C
template <typename T>
void JLT(const T &receiver, const unsigned char &value);
//uses code 0x7D
template <typename T>
void JGE(const T &receiver, const unsigned char &value);
//uses code 0x7E
template <typename T>
void JLE(const T &receiver, const unsigned char &value);
//uses code 0x7F
template <typename T>
void JGT(const T &receiver, const unsigned char &value);

//uses code 0xE3
template <typename T>
void JCXZ(const T &receiver, const unsigned char &value);

#pragma endregion jump if instructions

template <typename T>
void NOP(const T &receiver);
//uses code 0x86
template <typename T>
void XCHGb(const T &receiver, const char *&reg1, const char *&reg2);
//uses codes 0x90 - 0x97 or possibly 0x90
template <typename T>
void XCHGv(const T &receiver, const char *&reg1, const char *&reg2);

//uses codes 0x88 or 0x8A
template <typename T>
void MOVb(const T &receiver, const char *&reg1, const char *&reg2);
//uses codes 0x89 or 0x8B
template <typename T>
void MOVv(const T &receiver, const char *&reg1, const char *&reg2);
//uses codes 0xB0 - 0xB7 and possibly 0xC6
template <typename T>
void MOVb(const T &receiver, const char *&reg, const unsigned char &value);
//uses codes 0xB8 - 0xBF and possibly 0xC7
template <typename T>
void MOVv(const T &receiver, const char *&reg, const unsigned int &value);

//uses code 0xCA
template <typename T>
void RETF16(const T &receiver, const unsigned short &value);
//uses code 0xCB
template <typename T>
void RETF(const T &receiver);

//uses code 0xCC
template <typename T>
void INT(const T &receiver);
//uses code 0xCD
template <typename T>
void INT(const T &receiver, const unsigned char &value);

//uses code 0xE0
template <typename T>
void LOOPNZ(const T &receiver, const unsigned char &value);
//uses code 0xE1
template <typename T>
void LOOPZ(const T &receiver, const unsigned char &value);
//uses code 0xE2
template <typename T>
void LOOP(const T &receiver, const unsigned char &value);

//uses code 0xF4
template <typename T>
void HLT(const T &receiver);

//TEST,NOT,NEG,MUL,IMUL,DIV,and IDIV instructions

//uses general instruction 0xF6 with modrm OP bits set to 0
template <typename T>
void TESTb(const T &receiver, const char *&reg1, const unsigned char &value);
//uses general instruction 0xF6 with modrm OP bits set to 2
template <typename T>
void NOTb(const T &receiver, const char *&reg);
//uses general instruction 0xF6 with modrm OP bits set to 3
template <typename T>
void NEGb(const T &receiver, const char *&reg);

//instructions take 4 or more arguments? im confused
/*
//uses general instruction 0xF6 with modrm OP bits set to 4
template <typename T>
void UMULb(const T &receiver);
//uses general instruction 0xF6 with modrm OP bits set to 5
template <typename T>
void MULb(const T &receiver);
//uses general instruction 0xF6 with modrm OP bits set to 6
template <typename T>
void UDIVb(const T &receiver);
//uses general instruction 0xF6 with modrm OP bits set to 7
template <typename T>
void DIVb(const T &receiver);
*/

// these uses general instruction 0xF7 with modrm OP bits set to 0
template <typename T>
void TESTv(const T &receiver, const char *&reg1, const unsigned int &value);
//uses general instruction 0xF7 with modrm OP bits set to 2
template <typename T>
void NOTv(const T &receiver, const char *&reg);
//uses general instruction 0xF7 with modrm OP bits set to 3
template <typename T>
void NEGv(const T &receiver, const char *&reg);

// these instructions take 3 arguments? im confused
/*
//uses general instruction 0xF7 with modrm OP bits set to 4
template <typename T>
void UMULv(const T &receiver);
//uses general instruction 0xF7 with modrm OP bits set to 5
template <typename T>
void MULv(const T &receiver);
//uses general instruction 0xF7 with modrm OP bits set to 6
template <typename T>
void UDIVv(const T &receiver);
//uses general instruction 0xF7 with modrm OP bits set to 7
template <typename T>
void DIVv(const T &receiver);
*/

#define INTEL_ModRM_OP2_NOT 0b00010000// 2 lockable
#define INTEL_ModRM_OP2_NEG 0b00011000// 3 lockable
#define INTEL_ModRM_OP2_UMUL 0b00100000// 4
#define INTEL_ModRM_OP2_MUL 0b00101000// 5
#define INTEL_ModRM_OP2_UDIV 0b00110000// 6
#define INTEL_ModRM_OP2_DIV 0b00111000// 7

//uses code 0xF8
template <typename T>
void CLC(const T &receiver);
//uses code 0xF9
template <typename T>
void STC(const T &receiver);
//uses code 0xFA
template <typename T>
void CLI(const T &receiver);
//uses code 0xFB
template <typename T>
void STI(const T &receiver);
//uses code 0xFC
template <typename T>
void CLD(const T &receiver);
//uses code 0xFD
template <typename T>
void STD(const T &receiver);

#endif  // _GLOBAL_CONSTANTS_H
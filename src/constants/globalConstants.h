#ifndef _GLOBAL_CONSTANTS_H
#define _GLOBAL_CONSTANTS_H

#include <fstream>
#include <vector>
#include <cstdint>

const uint32_t VirtAddr32 = 0x00400000;
const uint32_t Align32 = 0x00000200;
const uint64_t VirtAddr64 = 0x0000000000400000;
const uint64_t Align64 = 0x0000000000000200;

void pushChars(std::ofstream &stream, const uint8_t *chars, uint32_t len, const bool &LSB);
void pushChars(std::vector<uint8_t> &vector, const uint8_t *chars, uint32_t len, const bool &LSB);
void pushChars(std::ofstream &stream, std::vector<uint8_t> &vector);

void pushByte(std::ofstream &stream, const uint8_t &byte);
void pushByte(std::vector<uint8_t> &vector, const uint8_t &byte);

void pushHalfWord(std::ofstream &stream, const uint16_t &halfword, const bool &LSB);
void pushHalfWord(std::vector<uint8_t> &vector, const uint16_t &halfword, const bool &LSB);

void pushWord(std::ofstream &stream, const uint32_t &word, const bool &LSB);
void pushWord(std::vector<uint8_t> &vector, const uint32_t &word, const bool &LSB);

void pushDword(std::ofstream &stream, const uint64_t &dword, const bool &LSB);
void pushDword(std::vector<uint8_t> &vector, const uint64_t &dword, const bool &LSB);

void padBytes(std::ofstream &stream, const uint32_t &numBytes);

#include "ELF.h"
#include "PE.h"

//32 bit reg helpers, for registers eAX, eCX, eDX, eBX, eSP, eBP, eSI, and eDI
uint8_t RegToOffset32(const char *reg);
uint8_t RegToModrmReg32(const char *reg);
uint8_t RegToModrmRM32(const char *reg);
//16 bit reg helpers, for registers AX, CX, DX, BX, SP, BP, SI, and DI
uint8_t RegToOffset16(const char *reg);
uint8_t RegToModrmReg16(const char *reg);
uint8_t RegToModrmRM16(const char *reg);
//8 bit reg helpers, for registers AL, CL, DL, BL, AH, CH, DH, and BH
uint8_t RegToOffset8(const char *reg);
uint8_t RegToModrmReg8(const char *reg);
uint8_t RegToModrmRM8(const char *reg);

// uses codes 0x00 - 0x05 as well as general instructions 0x80 - 0x83 with modrm OP bits set to 0
template<typename T>
void ADDb(T &receiver, const char *reg1, const char *reg2);
template<typename T>
void ADDb(T &receiver, const char *reg, const uint8_t &value);
template<typename T>
void ADDv(T &receiver, const char *reg1, const char *reg2);
template<typename T>
void ADDv(T &receiver, const char *reg, const uint32_t &value);

// uses codes 0x08 - 0x0D as well as general instructions 0x80 - 0x83 with modrm OP bits set to 1
template<typename T>
void ORb(T &receiver, const char *reg1, const char *reg2);
template<typename T>
void ORb(T &receiver, const char *reg, const uint8_t &value);
template<typename T>
void ORv(T &receiver, const char *reg1, const char *reg2);
template<typename T>
void ORv(T &receiver, const char *reg, const uint32_t &value);

// uses codes 0x10 - 0x15 as well as general instructions 0x80 - 0x83 with modrm OP bits set to 2
template<typename T>
void ADCb(T &receiver, const char *reg1, const char *reg2);
template<typename T>
void ADCb(T &receiver, const char *reg, const uint8_t &value);
template<typename T>
void ADCv(T &receiver, const char *reg1, const char *reg2);
template<typename T>
void ADCv(T &receiver, const char *reg, const uint32_t &value);

// uses codes 0x18 - 0x1D as well as general instructions 0x80 - 0x83 with modrm OP bits set to 3
template<typename T>
void SBBb(T &receiver, const char *reg1, const char *reg2);
template<typename T>
void SBBb(T &receiver, const char *reg, const uint8_t &value);
template<typename T>
void SBBv(T &receiver, const char *reg1, const char *reg2);
template<typename T>
void SBBv(T &receiver, const char *reg, const uint32_t &value);

// uses codes 0x20 - 0x25 as well as general instructions 0x80 - 0x83 with modrm OP bits set to 4
template<typename T>
void ANDb(T &receiver, const char *reg1, const char *reg2);
template<typename T>
void ANDb(T &receiver, const char *reg, const uint8_t &value);
template<typename T>
void ANDv(T &receiver, const char *reg1, const char *reg2);
template<typename T>
void ANDv(T &receiver, const char *reg, const uint32_t &value);

// uses codes 0x28 - 0x2D as well as general instructions 0x80 - 0x83 with modrm OP bits set to 5
template<typename T>
void SUBb(T &receiver, const char *reg1, const char *reg2);
template<typename T>
void SUBb(T &receiver, const char *reg, const uint8_t &value);
template<typename T>
void SUBv(T &receiver, const char *reg1, const char *reg2);
template<typename T>
void SUBv(T &receiver, const char *reg, const uint32_t &value);

// uses codes 0x30 - 0x35 as well as general instructions 0x80 - 0x83 with modrm OP bits set to 6
template<typename T>
void XORb(T &receiver, const char *reg1, const char *reg2);
template<typename T>
void XORb(T &receiver, const char *reg, const uint8_t &value);
template<typename T>
void XORv(T &receiver, const char *reg1, const char *reg2);
template<typename T>
void XORv(T &receiver, const char *reg, const uint32_t &value);

// uses codes 0x38 - 0x3D as well as general instructions 0x80 - 0x83 with modrm OP bits set to 7
template<typename T>
void CMPb(T &receiver, const char *reg1, const char *reg2);
template<typename T>
void CMPb(T &receiver, const char *reg, const uint8_t &value);
template<typename T>
void CMPv(T &receiver, const char *reg1, const char *reg2);
template<typename T>
void CMPv(T &receiver, const char *reg, const uint32_t &value);

//uses codes 0x40 - 0x47 as well as general instructions 0xFE and 0xFF with modrm OP bits set to 0 for indirect
template <typename T>
void INCb(T &receiver, const char *reg);
template <typename T>
void INCv(T &receiver, const char *reg);

//uses codes 0x48 - 0x4F as well as general instructions 0xFE and 0xFF with modrm OP bits set to 1 for indirect
template <typename T>
void DECb(T &receiver, const char *reg);
template <typename T>
void DECv(T &receiver, const char *reg);

//uses code 0xE8
template <typename T>
void CALL(T &receiver, const uint32_t &value);//relative
//uses general instruction 0xFF with modrm OP bits set to 2 for indirect addressing
template <typename T>
void CALL(T &receiver, const char *reg);

//uses code 0x9A
// takes 5 bytes
template <typename T>
void CALLF(T &receiver, const uint64_t &value){};
//uses general instruction 0xFF with modrm OP bits set to 3 for indirect
template <typename T>
void CALLF(T &receiver, const char *reg){};

//uses code 0xEB
template <typename T>
void JMPb(T &receiver, const uint8_t &value);
//uses code 0xE9
template <typename T>
void JMPv(T &receiver, const uint32_t &value);
//uses general instruction 0xFF with modrm OP bits set to 4 for indirect addressing
template <typename T>
void JMPv(T &receiver, const char *reg);

//uses code 0xEA
// takes 5 bytes
template <typename T>
void JMPF(T &receiver, const uint64_t &value){};
//uses general instruction 0xFF with modrm OP bits set to 5 for indirect
template <typename T>
void JMPF(T &receiver, const char *reg){};

//uses code 0x6A
template <typename T>
void PUSHb(T &receiver, const uint8_t &value);
//uses code 0x68
template <typename T>
void PUSHv(T &receiver, const uint32_t &value);
//uses codes 0x50 - 0x57 as well as the general instruction 0xFF with modrm OP bits set to 6 for indirect
template <typename T>
void PUSHv(T &receiver, const char *reg);

//uses codes 0x58 - 0x5F and possibly 0x8F
template <typename T>
void POP(T &receiver, const char *reg);

#pragma region jump if instructions

//uses code 0x70
template <typename T>
void JOV(T &receiver, const uint8_t &value);
//uses code 0x71
template <typename T>
void JNOV(T &receiver, const uint8_t &value);
//uses code 0x72
template <typename T>
void JULT(T &receiver, const uint8_t &value);
//uses code 0x73
template <typename T>
void JUGE(T &receiver, const uint8_t &value);
//uses code 0x74
template <typename T>
void JET(T &receiver, const uint8_t &value);
//uses code 0x75
template <typename T>
void JNE(T &receiver, const uint8_t &value);
//uses code 0x76
template <typename T>
void JULE(T &receiver, const uint8_t &value);
//uses code 0x77
template <typename T>
void JUGT(T &receiver, const uint8_t &value);
//uses code 0x78
template <typename T>
void JS(T &receiver, const uint8_t &value);
//uses code 0x79
template <typename T>
void JNS(T &receiver, const uint8_t &value);
//uses code 0x7A
template <typename T>
void JP(T &receiver, const uint8_t &value);
//uses code 0x7B
template <typename T>
void JNP(T &receiver, const uint8_t &value);
//uses code 0x7C
template <typename T>
void JLT(T &receiver, const uint8_t &value);
//uses code 0x7D
template <typename T>
void JGE(T &receiver, const uint8_t &value);
//uses code 0x7E
template <typename T>
void JLE(T &receiver, const uint8_t &value);
//uses code 0x7F
template <typename T>
void JGT(T &receiver, const uint8_t &value);

//uses code 0xE3
template <typename T>
void JCXZ(T &receiver, const uint8_t &value);

#pragma endregion jump if instructions

template <typename T>
void NOP(T &receiver);
//uses code 0x86
template <typename T>
void XCHGb(T &receiver, const char *reg1, const char *reg2);
//uses codes 0x90 - 0x97 or possibly 0x90
template <typename T>
void XCHGv(T &receiver, const char *reg1, const char *reg2);

//uses codes 0x88 or 0x8A
template <typename T>
void MOVb(T &receiver, const char *reg1, const char *reg2);
//uses codes 0x89 or 0x8B
template <typename T>
void MOVv(T &receiver, const char *reg1, const char *reg2);
//uses codes 0xB0 - 0xB7 and possibly 0xC6
template <typename T>
void MOVb(T &receiver, const char *reg, const uint8_t &value);
//uses codes 0xB8 - 0xBF and possibly 0xC7
template <typename T>
void MOVv(T &receiver, const char *reg, const uint32_t &value);

//uses code 0xC2
template <typename T>
void RETN(T &receiver, const uint16_t &value);
//uses code 0xC3
template <typename T>
void RETN(T &receiver);

//uses code 0xCA
template <typename T>
void RETF(T &receiver, const uint16_t &value);
//uses code 0xCB
template <typename T>
void RETF(T &receiver);

//uses code 0xCC
template <typename T>
void INT(T &receiver);
//uses code 0xCD
template <typename T>
void INT(T &receiver, const uint8_t &value);

//uses code 0xE0
template <typename T>
void LOOPNE(T &receiver, const uint8_t &value);
//uses code 0xE1
template <typename T>
void LOOPET(T &receiver, const uint8_t &value);
//uses code 0xE2
template <typename T>
void LOOP(T &receiver, const uint8_t &value);

//uses code 0xF4
template <typename T>
void HLT(T &receiver);

//TEST,NOT,NEG,MUL,IMUL,DIV,and IDIV instructions

//uses general instruction 0xF6 with modrm OP bits set to 0
template <typename T>
void TESTb(T &receiver, const char *reg1, const uint8_t &value);
// these uses general instruction 0xF7 with modrm OP bits set to 0
template <typename T>
void TESTv(T &receiver, const char *reg1, const uint32_t &value);
//uses general instruction 0xF6 with modrm OP bits set to 2
template <typename T>
void NOTb(T &receiver, const char *reg);
//uses general instruction 0xF7 with modrm OP bits set to 2
template <typename T>
void NOTv(T &receiver, const char *reg);
//uses general instruction 0xF6 with modrm OP bits set to 3
template <typename T>
void NEGb(T &receiver, const char *reg);
//uses general instruction 0xF7 with modrm OP bits set to 3
template <typename T>
void NEGv(T &receiver, const char *reg);

//instructions take 4 or more arguments? im confused
/*
//uses general instruction 0xF6 with modrm OP bits set to 4
template <typename T>
void UMULb(T &receiver);
//uses general instruction 0xF7 with modrm OP bits set to 4
template <typename T>
void UMULv(T &receiver);
//uses general instruction 0xF6 with modrm OP bits set to 5
template <typename T>
void MULb(T &receiver);
//uses general instruction 0xF7 with modrm OP bits set to 5
template <typename T>
void MULv(T &receiver);
//uses general instruction 0xF6 with modrm OP bits set to 6
template <typename T>
void UDIVb(T &receiver);
//uses general instruction 0xF7 with modrm OP bits set to 6
template <typename T>
void UDIVv(T &receiver);
//uses general instruction 0xF6 with modrm OP bits set to 7
template <typename T>
void DIVb(T &receiver);
//uses general instruction 0xF7 with modrm OP bits set to 7
template <typename T>
void DIVv(T &receiver);
*/


//uses code 0xF8
template <typename T>
void CLC(T &receiver);
//uses code 0xF9
template <typename T>
void STC(T &receiver);
//uses code 0xFA
template <typename T>
void CLI(T &receiver);
//uses code 0xFB
template <typename T>
void STI(T &receiver);
//uses code 0xFC
template <typename T>
void CLD(T &receiver);
//uses code 0xFD
template <typename T>
void STD(T &receiver);

#endif  // _GLOBAL_CONSTANTS_H
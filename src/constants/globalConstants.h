#ifndef _GLOBAL_CONSTANTS_H
#define _GLOBAL_CONSTANTS_H

#include <fstream>
#include <vector>
#include <cstdint>

const uint32_t VirtAddr32 = 0x00400000;
const uint32_t Align32 = 0x00000200;
const uint64_t VirtAddr64 = 0x0000000000400000;
const uint64_t Align64 = 0x0000000000000200;

#define SECTION_ALIGN 0x1000u
#define FILE_ALIGN 0x200u

extern uint8_t bitMode;

void pushChars(std::ostream &stream, const uint8_t *chars, uint32_t len, const bool &LSB);
void pushChars(std::ofstream &stream, const uint8_t *chars, uint32_t len, const bool &LSB);
void pushChars(std::vector<uint8_t> &vector, const uint8_t *chars, uint32_t len, const bool &LSB);
void pushChars(std::ofstream &stream, std::vector<uint8_t> &vector);

void pushByte(std::ostream &stream, const uint8_t &byte);
void pushByte(std::ofstream &stream, const uint8_t &byte);
void pushByte(std::vector<uint8_t> &vector, const uint8_t &byte);

void pushHalfWord(std::ostream &stream, const uint16_t &halfword, const bool &LSB);
void pushHalfWord(std::ofstream &stream, const uint16_t &halfword, const bool &LSB);
void pushHalfWord(std::vector<uint8_t> &vector, const uint16_t &halfword, const bool &LSB);

void pushWord(std::ostream &stream, const uint32_t &word, const bool &LSB);
void pushWord(std::ofstream &stream, const uint32_t &word, const bool &LSB);
void pushWord(std::vector<uint8_t> &vector, const uint32_t &word, const bool &LSB);

void pushDword(std::ostream &stream, const uint64_t &dword, const bool &LSB);
void pushDword(std::ofstream &stream, const uint64_t &dword, const bool &LSB);
void pushDword(std::vector<uint8_t> &vector, const uint64_t &dword, const bool &LSB);

void padBytes(std::ofstream &stream, const uint32_t &numBytes);

#include "ELF.h"
#include "PE.h"

template<typename T>
void ADD(T &receiver, const char* dst, const char* src);
template <typename T>
void OR(T &receiver, const char* dst, const char* src);
template <typename T>
void ADC(T &receiver, const char* dst, const char* src);
template <typename T>
void SBB(T &receiver, const char* dst, const char* src);
template <typename T>
void AND(T &receiver, const char* dst, const char* src);
template <typename T>
void SUB(T &receiver, const char* dst, const char* src);
template <typename T>
void XOR(T &receiver, const char* dst, const char* src);
template <typename T>
void CMP(T &receiver, const char* dst, const char* src);
template <typename T>
void INC(T &receiver, const char *arg);
template <typename T>
void DEC(T &receiver, const char *arg);
template <typename T>
void CALL(T &receiver, const char *arg);
/*template <typename T>
void CALLF(T &receiver, const char *arg);*/
template <typename T>
void JMP(T &receiver, const char *arg);
/*template <typename T>
void JMPF(T &receiver, const char *arg);*/
template <typename T>
void PUSH(T &receiver, const char *arg);
template <typename T>
void POP(T &receiver, const char *arg);

/*#pragma region jump if instructions
template <typename T>
void JOV(T &receiver, const char *arg);
template <typename T>
void JNOV(T &receiver, const char *arg);
template <typename T>
void JULT(T &receiver, const char *arg);
template <typename T>
void JUGE(T &receiver, const char *arg);
template <typename T>
void JET(T &receiver, const char *arg);
template <typename T>
void JNE(T &receiver, const char *arg);
template <typename T>
void JULE(T &receiver, const char *arg);
template <typename T>
void JUGT(T &receiver, const char *arg);
template <typename T>
void JS(T &receiver, const char *arg);
template <typename T>
void JNS(T &receiver, const char *arg);
template <typename T>
void JP(T &receiver, const char *arg);
template <typename T>
void JNP(T &receiver, const char *arg);
template <typename T>
void JLT(T &receiver, const char *arg);
template <typename T>
void JGE(T &receiver, const char *arg);
template <typename T>
void JLE(T &receiver, const char *arg);
template <typename T>
void JGT(T &receiver, const char *arg);
template <typename T>
void JCXZ(T &receiver, const char *arg);
#pragma endregion jump if instructions*/

template <typename T>
void NOP(T &receiver);
template <typename T>
void XCHG(T &receiver, const char *reg1, const char *reg2);
template <typename T>
void MOV(T &receiver, const char *dst, const char *src);
template <typename T>
void RET(T &receiver, const char *num);
template <typename T>
void RET(T &receiver);
template <typename T>
void RETF(T &receiver, const char *num);
template <typename T>
void RETF(T &receiver);
template <typename T>
void INT(T &receiver, const char *num);

/*//uses code 0xE0
template <typename T>
void LOOPNE(T &receiver, const uint8_t &value);
//uses code 0xE1
template <typename T>
void LOOPET(T &receiver, const uint8_t &value);
//uses code 0xE2
template <typename T>
void LOOP(T &receiver, const uint8_t &value);*/

template <typename T>
void HLT(T &receiver);

//TEST,NOT,NEG,MUL,IMUL,DIV,and IDIV instructions

/*template <typename T>
void TEST(T &receiver, const char *arg1, const uint8_t &arg2);*/
template <typename T>
void NOT(T &receiver, const char *arg);
template <typename T>
void NEG(T &receiver, const char *arg);

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

template <typename T>
void CLC(T &receiver);
template <typename T>
void STC(T &receiver);
template <typename T>
void CLI(T &receiver);
template <typename T>
void STI(T &receiver);
template <typename T>
void CLD(T &receiver);
template <typename T>
void STD(T &receiver);

#endif  // _GLOBAL_CONSTANTS_H
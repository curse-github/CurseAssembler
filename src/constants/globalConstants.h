#ifndef _GLOBAL_CONSTANTS_H
#define _GLOBAL_CONSTANTS_H

#include <fstream>
#include <vector>
#include <cstdint>
#include <iomanip>

const uint32_t VirtAddr32 = 0x00400000;
const uint64_t VirtAddr64 = 0x0000000140000000;

#define SECTION_ALIGN 0x1000u
#define FILE_ALIGN 0x200u

//utility func
uint32_t roundToAlign(const uint32_t &value, const uint32_t &align);

extern uint8_t bitMode;

#pragma region void pushChars(T&, const uint8_t*, const size_t&, const bool&)
template <typename T>
void pushChars(T& reciever, const uint8_t* chars, const size_t& len, const bool& LSB);
template <>
void pushChars(std::ostream& reciever, const uint8_t* chars, const size_t& len, const bool& LSB);
template <>
void pushChars(std::ofstream& reciever, const uint8_t* chars, const size_t& len, const bool& LSB);
template <>
void pushChars(std::vector<uint8_t>& reciever, const uint8_t* chars, const size_t& len, const bool& LSB);
#pragma endregion void pushChars(T&, const uint8_t*, const size_t&, const bool&)
template <typename T>
void pushChars(T& reciever, const std::vector<uint8_t>& vector);
void setCharsAt(std::vector<uint8_t>& vector, const size_t& index, const uint8_t* chars, const size_t& len, const bool& LSB);

template <typename T>
void pushByte (T& reciever, const uint8_t& byte);
template <typename T>
void pushWord (T& reciever, const uint16_t& word, const bool& LSB);
template <typename T>
void pushDword(T& reciever, const uint32_t& dword, const bool& LSB);
template <typename T>
void pushQword(T& reciever, const uint64_t& qword, const bool& LSB);
void setByteAt (std::vector<uint8_t>& vector, const size_t& index, const uint8_t& byte);
void setWordAt (std::vector<uint8_t>& vector, const size_t& index, const uint16_t& word, const bool& LSB);
void setDwordAt(std::vector<uint8_t>& vector, const size_t& index, const uint32_t& dword, const bool& LSB);
void setQwordAt(std::vector<uint8_t>& vector, const size_t& index, const uint64_t& qword, const bool& LSB);

void padBytes(std::ofstream& stream, const size_t& numBytes);
void padBytes(std::vector<uint8_t>& vector, const size_t& numBytes);

#include "ELF.h"
#include "PE.h"


extern bool debugInstructionOutput;
template <typename T>
void resolveVar(T& receiver, const std::string& varName);

template<typename T>
void ADD(T& receiver, const char* dst, const char* src);
template <typename T>
void OR(T& receiver, const char* dst, const char* src);
template <typename T>
void ADC(T& receiver, const char* dst, const char* src);
template <typename T>
void SBB(T& receiver, const char* dst, const char* src);
template <typename T>
void AND(T& receiver, const char* dst, const char* src);
template <typename T>
void SUB(T& receiver, const char* dst, const char* src);
template <typename T>
void XOR(T& receiver, const char* dst, const char* src);
template <typename T>
void CMP(T& receiver, const char* dst, const char* src);
template <typename T>
void INC(T& receiver, const char* arg);
template <typename T>
void DEC(T& receiver, const char* arg);
template <typename T>
void CALL(T& receiver, const char* arg);
/*template <typename T>
void CALLF(T& receiver, const char* arg);*/
template <typename T>
void JMP(T& receiver, const char* arg);
/*template <typename T>
void JMPF(T& receiver, const char* arg);*/
template <typename T>
void PUSH(T& receiver, const char* arg);
template <typename T>
void POP(T& receiver, const char* arg);

/*#pragma region jump if instructions
template <typename T>
void JOV(T& receiver, const char* arg);
template <typename T>
void JNOV(T& receiver, const char* arg);
template <typename T>
void JULT(T& receiver, const char* arg);
template <typename T>
void JUGE(T& receiver, const char* arg);
template <typename T>
void JET(T& receiver, const char* arg);
template <typename T>
void JNE(T& receiver, const char* arg);
template <typename T>
void JULE(T& receiver, const char* arg);
template <typename T>
void JUGT(T& receiver, const char* arg);
template <typename T>
void JS(T& receiver, const char* arg);
template <typename T>
void JNS(T& receiver, const char* arg);
template <typename T>
void JP(T& receiver, const char* arg);
template <typename T>
void JNP(T& receiver, const char* arg);
template <typename T>
void JLT(T& receiver, const char* arg);
template <typename T>
void JGE(T& receiver, const char* arg);
template <typename T>
void JLE(T& receiver, const char* arg);
template <typename T>
void JGT(T& receiver, const char* arg);
template <typename T>
void JCXZ(T& receiver, const char* arg);
#pragma endregion jump if instructions*/

template <typename T>
void NOP(T& receiver);
template <typename T>
void NOP(T& receiver, const char* arg);
template <typename T>
void XCHG(T& receiver, const char* arg1, const char* arg2);
template <typename T>
void MOV(T& receiver, const char* dst, const char* src);
template <typename T>
void RET(T& receiver, const char* num);
template <typename T>
void RET(T& receiver);
template <typename T>
void RETF(T& receiver, const char* num);
template <typename T>
void RETF(T& receiver);
template <typename T>
void INT3(T& receiver);
template <typename T>
void INT(T& receiver, const char* num);

/*//uses code 0xE0
template <typename T>
void LOOPNE(T& receiver, const uint8_t& value);
//uses code 0xE1
template <typename T>
void LOOPET(T& receiver, const uint8_t& value);
//uses code 0xE2
template <typename T>
void LOOP(T& receiver, const uint8_t& value);*/

template <typename T>
void HLT(T& receiver);
template <typename T>
void TEST(T& receiver, const char* arg1, const char* arg2);
template <typename T>
void NOT(T& receiver, const char* arg);
template <typename T>
void NEG(T& receiver, const char* arg);

template <typename T>
void UMUL(T& receiver, const char* arg);
template <typename T>
void MUL(T& receiver, const char* arg);
template <typename T>
void UDIV(T& receiver, const char* arg);
template <typename T>
void DIV(T& receiver, const char* arg);

template <typename T>
void CLC(T& receiver);
template <typename T>
void STC(T& receiver);
template <typename T>
void CLI(T& receiver);
template <typename T>
void STI(T& receiver);
template <typename T>
void CLD(T& receiver);
template <typename T>
void STD(T& receiver);

#endif  // _GLOBAL_CONSTANTS_H
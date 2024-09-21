#ifndef _GLOBAL_CONSTANTS_H
#define _GLOBAL_CONSTANTS_H

#include <cstdint>// for uint8_t, uint16_t, uint32_t, and uint64_t
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>

#include <vector>
#include <iomanip>
#include <algorithm>// for std::min and std::max
#include <filesystem>

#include "intelConstants.h"

#define SECTION_ALIGN 0x1000u
#define FILE_ALIGN 0x200u

extern uint32_t VirtAddr32;
extern uint64_t VirtAddr64;
extern uint8_t bitMode;

//utility func
uint32_t roundToAlign(const uint32_t &value, const uint32_t &align);
template<typename T>
std::string intToHex(const T& i, const char *prefix="0x");
template<>
std::string intToHex(const int8_t& i, const char *prefix);
template<>
std::string intToHex(const uint8_t& i, const char *prefix);
template<>
std::string intToHex(const int16_t& i, const char *prefix);
template<>
std::string intToHex(const uint16_t& i, const char *prefix);
template<>
std::string intToHex(const int32_t& i, const char *prefix);
template<>
std::string intToHex(const uint32_t& i, const char *prefix);
template<>
std::string intToHex(const int64_t& i, const char *prefix);
template<>
std::string intToHex(const uint64_t& i, const char *prefix);
void padBytes(std::ofstream& stream, const size_t& numBytes);
void padBytes(std::vector<uint8_t>& vector, const size_t& numBytes);
char toLower(char chr);
char toUpper(char chr);
void trashBytes(std::ifstream& stream, uint32_t& count, const uint32_t& num);

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

std::vector<uint8_t> readBytes(std::ifstream& stream, uint32_t& count, const size_t& num);
char readChar(std::ifstream& stream, uint32_t& count);
uint8_t readByte(std::ifstream& stream, uint32_t& count);
uint16_t readWord(std::ifstream& stream, uint32_t& count);
uint32_t readDword(std::ifstream& stream, uint32_t& count);
uint32_t readQword(std::ifstream& stream, uint32_t& count);
std::vector<uint8_t> readBytesAt(std::vector<uint8_t>& vec, const size_t& at, const size_t& num);
char readCharAt(std::vector<uint8_t>& vec, const size_t& at);
uint8_t readByteAt(std::vector<uint8_t>& vec, const size_t& at);
uint16_t readWordAt(std::vector<uint8_t>& vec, const size_t& at);
uint32_t readDwordAt(std::vector<uint8_t>& vec, const size_t& at);
uint32_t readQwordAt(std::vector<uint8_t>& vec, const size_t& at);
std::string readStringAt(std::vector<uint8_t>& vec, const size_t& at);

#include "ELF.h"
#include "PE.h"

extern bool debugInstructionOutput;
template <typename T>
void resolveVar(T& reciever, const std::string& varName);

template<typename T>
void ADD(T& reciever, const char* dst, const char* src);
template <typename T>
void OR(T& reciever, const char* dst, const char* src);
template <typename T>
void ADC(T& reciever, const char* dst, const char* src);
template <typename T>
void SBB(T& reciever, const char* dst, const char* src);
template <typename T>
void AND(T& reciever, const char* dst, const char* src);
template <typename T>
void SUB(T& reciever, const char* dst, const char* src);
template <typename T>
void XOR(T& reciever, const char* dst, const char* src);
template <typename T>
void CMP(T& reciever, const char* dst, const char* src);
template <typename T>
void INC(T& reciever, const char* arg);
template <typename T>
void DEC(T& reciever, const char* arg);
template <typename T>
void CALL(T& reciever, const char* arg);
/*template <typename T>
void CALLF(T& reciever, const char* arg);*/
template <typename T>
void JMP(T& reciever, const char* arg);
/*template <typename T>
void JMPF(T& reciever, const char* arg);*/
template <typename T>
void PUSH(T& reciever, const char* arg);
template <typename T>
void POP(T& reciever, const char* arg);

/*#pragma region jump if instructions
template <typename T>
void JOV(T& reciever, const char* arg);
template <typename T>
void JNOV(T& reciever, const char* arg);
template <typename T>
void JULT(T& reciever, const char* arg);
template <typename T>
void JUGE(T& reciever, const char* arg);
template <typename T>
void JET(T& reciever, const char* arg);
template <typename T>
void JNE(T& reciever, const char* arg);
template <typename T>
void JULE(T& reciever, const char* arg);
template <typename T>
void JUGT(T& reciever, const char* arg);
template <typename T>
void JS(T& reciever, const char* arg);
template <typename T>
void JNS(T& reciever, const char* arg);
template <typename T>
void JP(T& reciever, const char* arg);
template <typename T>
void JNP(T& reciever, const char* arg);
template <typename T>
void JLT(T& reciever, const char* arg);
template <typename T>
void JGE(T& reciever, const char* arg);
template <typename T>
void JLE(T& reciever, const char* arg);
template <typename T>
void JGT(T& reciever, const char* arg);
template <typename T>
void JCXZ(T& reciever, const char* arg);
#pragma endregion jump if instructions*/

template <typename T>
void LEA(T& reciever, const char* dst, const char* src);

template <typename T>
void NOP(T& reciever);
template <typename T>
void NOP(T& reciever, const char* arg);
template <typename T>
void XCHG(T& reciever, const char* arg1, const char* arg2);
template <typename T>
void MOV(T& reciever, const char* dst, const char* src);
template <typename T>
void RET(T& reciever, const char* num);
template <typename T>
void RET(T& reciever);
template <typename T>
void RETF(T& reciever, const char* num);
template <typename T>
void RETF(T& reciever);
template <typename T>
void INT3(T& reciever);
template <typename T>
void INT(T& reciever, const char* num);

/*//uses code 0xE0
template <typename T>
void LOOPNE(T& reciever, const uint8_t& value);
//uses code 0xE1
template <typename T>
void LOOPET(T& reciever, const uint8_t& value);
//uses code 0xE2
template <typename T>
void LOOP(T& reciever, const uint8_t& value);*/

template <typename T>
void HLT(T& reciever);
template <typename T>
void TEST(T& reciever, const char* arg1, const char* arg2);
template <typename T>
void NOT(T& reciever, const char* arg);
template <typename T>
void NEG(T& reciever, const char* arg);

template <typename T>
void UMUL(T& reciever, const char* arg);
template <typename T>
void MUL(T& reciever, const char* arg);
template <typename T>
void UDIV(T& reciever, const char* arg);
template <typename T>
void DIV(T& reciever, const char* arg);

template <typename T>
void CLC(T& reciever);
template <typename T>
void STC(T& reciever);
template <typename T>
void CLI(T& reciever);
template <typename T>
void STI(T& reciever);
template <typename T>
void CLD(T& reciever);
template <typename T>
void STD(T& reciever);

#endif  // _GLOBAL_CONSTANTS_H
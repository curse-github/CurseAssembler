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

void INC(std::vector<unsigned char> &vector, const char *reg);
void DEC(std::vector<unsigned char> &vector, const char *reg);
void PUSH(std::vector<unsigned char> &vector, const char *reg);
void PUSH(std::vector<unsigned char> &vector, unsigned int value, const bool &LSB);
void POP(std::vector<unsigned char> &vector, const char *reg);
void NOP(std::vector<unsigned char> &vector);
void XCHG_eAX(std::vector<unsigned char> &vector, const char *reg);
void MOVeaxAddr32(std::vector<unsigned char> &vector, const unsigned int &addr, const bool &LSB);
void MOVeaxAddr64(std::vector<unsigned char> &vector, const unsigned long &addr, const bool &LSB);
void MOVaddrEax32(std::vector<unsigned char> &vector, const unsigned int &addr, const bool &LSB);
void MOVaddrEax64(std::vector<unsigned char> &vector, const unsigned long &addr, const bool &LSB);
void MOV32(std::vector<unsigned char> &vector, const char *reg, const unsigned int &value, const bool &LSB);
void MOV8_low(std::vector<unsigned char> &vector, const char *reg, const unsigned char &value);
void MOV8_high(std::vector<unsigned char> &vector, const char *reg, const unsigned char &value);
void INT(std::vector<unsigned char> &vector, unsigned char value);
void SYSCALL(std::vector<unsigned char> &vector, const bool &isLinux);
void JMP32(std::vector<unsigned char> &vector, unsigned int value, const bool &LSB);
void JMP64(std::vector<unsigned char> &vector, unsigned long value, const bool &LSB);
void JMPoffset(std::vector<unsigned char> &vector, unsigned char value);

#endif  // _GLOBAL_CONSTANTS_H
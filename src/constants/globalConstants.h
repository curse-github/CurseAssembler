#ifndef _GLOBAL_H
#define _GLOBAL_H

#include <fstream>
#include <iostream>
#include <vector>

void pushChars(std::ofstream &stream, const unsigned char *chars, unsigned int len, const bool &LSB);
void pushChars(std::vector<unsigned char> &vector, const unsigned char *chars, unsigned int len, const bool &LSB);
void pushChars(std::ofstream &stream, std::vector<unsigned char> &vector);

void pushByte(std::ofstream &stream, const unsigned char &byte, const bool &LSB);
void pushByte(std::vector<unsigned char> &vector, const unsigned char &byte, const bool &LSB);

void pushHalfWord(std::ofstream &stream, const unsigned short &halfword, const bool &LSB);
void pushHalfWord(std::vector<unsigned char> &vector, const unsigned short &halfword, const bool &LSB);

void pushWord(std::ofstream &stream, const unsigned int &word, const bool &LSB);
void pushWord(std::vector<unsigned char> &vector, const unsigned int &word, const bool &LSB);

void pushDword(std::ofstream &stream, const unsigned long &dword, const bool &LSB);
void pushDword(std::vector<unsigned char> &vector, const unsigned long &dword, const bool &LSB);

void INC(std::vector<unsigned char> &vector, const char* reg);
void DEC(std::vector<unsigned char> &vector, const char* reg);
void PUSH(std::vector<unsigned char> &vector, const char* reg);
void PUSH(std::vector<unsigned char> &vector, unsigned int value, const bool &LSB);
void POP(std::vector<unsigned char> &vector, const char* reg);
void NOP(std::vector<unsigned char> &vector);
void XCHG_eAX(std::vector<unsigned char> &vector, const char* reg);
void MOVaddr32(std::vector<unsigned char> &vector, const unsigned int &addr, const bool &LSB);
void MOVaddr64(std::vector<unsigned char> &vector, const unsigned long &addr, const bool &LSB);
void MOV32(std::vector<unsigned char> &vector, const char* reg, const unsigned int &value, const bool &LSB);
void MOV8_low(std::vector<unsigned char> &vector, const char* reg, const unsigned char &value);
void MOV8_high(std::vector<unsigned char> &vector, const char* reg, const unsigned char &value);
void INT(std::vector<unsigned char> &vector, unsigned char value);
void SYSCALL(std::vector<unsigned char> &vector, const bool &isLinux);
void JMP32(std::vector<unsigned char> &vector, unsigned int value, const bool &LSB);
void JMP64(std::vector<unsigned char> &vector, unsigned long value, const bool &LSB);
void JMPoffset(std::vector<unsigned char> &vector, unsigned char value);

#endif  // _GLOBAL_H
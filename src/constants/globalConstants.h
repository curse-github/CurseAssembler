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

template<typename T>
void ADD(const T &receiver) {
    
}

#endif  // _GLOBAL_CONSTANTS_H
#include <string.h>

#include "globalConstants.h"
#include "intelConstants.h"

#pragma region helper functions
void pushChars(std::ofstream &stream, const unsigned char *chars, unsigned int len, const bool &LSB) {
    if (LSB)
        for (short i = 0; i < len; i++) stream << chars[i];
    else
        for (short i = len - 1; i >= 0; i--) stream << chars[i];
}
void pushChars(std::ofstream &stream, std::vector<unsigned char> &vector) {
    for (size_t i = 0u; i < vector.size(); i++) stream << vector[i];
}
void pushChars(std::vector<unsigned char> &vector, const unsigned char *chars, unsigned int len, const bool &LSB) {
    if (LSB)
        for (char i = 0; i < len; i++) vector.push_back(chars[i]);
    else
        for (char i = len - 1; i >= 0; i--) vector.push_back(chars[i]);
}

void pushByte(std::ofstream &stream, const unsigned char &byte, const bool &LSB) {
    pushChars(stream, &byte, 1, LSB);
}
void pushByte(std::vector<unsigned char> &vector, const unsigned char &byte, const bool &LSB) {
    pushChars(vector, &byte, 1, LSB);
}

void pushHalfWord(std::ofstream &stream, const unsigned short &halfword, const bool &LSB) {
    pushChars(stream, static_cast<const unsigned char *>(static_cast<const void *>(&halfword)), sizeof(halfword), LSB);
}
void pushHalfWord(std::vector<unsigned char> &vector, const unsigned short &halfword, const bool &LSB) {
    pushChars(vector, static_cast<const unsigned char *>(static_cast<const void *>(&halfword)), sizeof(halfword), LSB);
}

void pushWord(std::ofstream &stream, const unsigned int &word, const bool &LSB) {
    pushChars(stream, static_cast<const unsigned char *>(static_cast<const void *>(&word)), sizeof(word), LSB);
}
void pushWord(std::vector<unsigned char> &vector, const unsigned int &word, const bool &LSB) {
    pushChars(vector, static_cast<const unsigned char *>(static_cast<const void *>(&word)), sizeof(word), LSB);
}

void pushDword(std::ofstream &stream, const unsigned long &dword, const bool &LSB) {
    pushChars(stream, static_cast<const unsigned char *>(static_cast<const void *>(&dword)), sizeof(dword), LSB);
}
void pushDword(std::vector<unsigned char> &vector, const unsigned long &dword, const bool &LSB) {
    pushChars(vector, static_cast<const unsigned char *>(static_cast<const void *>(&dword)), sizeof(dword), LSB);
}
#pragma endregion// helper functions

#pragma region instructions

unsigned char RegToOffset(const char *reg) {
    unsigned char offset = 0;
    if (strcmp(reg, "eAX") == 0)
        offset = INTEL_REG_OFF_A;
    else if (strcmp(reg, "eCX") == 0)
        offset = INTEL_REG_OFF_C;
    else if (strcmp(reg, "eDX") == 0)
        offset = INTEL_REG_OFF_D;
    else if (strcmp(reg, "eBX") == 0)
        offset = INTEL_REG_OFF_B;
    else {
        std::cout << "Invalid register." << std::endl;
        return 150;
    }
    return offset;
}
void INC(std::vector<unsigned char> &vector, const char *reg) {
    pushByte(vector, INTEL_INSTR_INC_REG + RegToOffset(reg), false);
}
void DEC(std::vector<unsigned char> &vector, const char *reg) {
    pushByte(vector, INTEL_INSTR_DEC_REG + RegToOffset(reg), false);
}
void PUSH(std::vector<unsigned char> &vector, const char *reg) {
    pushByte(vector, INTEL_INSTR_PUSH_REG + RegToOffset(reg), false);
}
void PUSH(std::vector<unsigned char> &vector, unsigned int value, const bool &LSB) {
    pushByte(vector, INTEL_INSTR_PUSH_Iv, false);
    pushWord(vector, value, LSB);
}
void POP(std::vector<unsigned char> &vector, const char *reg) {
    pushByte(vector, INTEL_INSTR_POP_REG + RegToOffset(reg), false);
}
void NOP(std::vector<unsigned char> &vector) {
    pushByte(vector, INTEL_INSTR_NOP, false);
}
void XCHG_eAX(std::vector<unsigned char> &vector, const char *reg) {
    pushByte(vector, INTEL_INSTR_XCHG_eAX_REG + RegToOffset(reg), false);
}

void MOVaddr32(std::vector<unsigned char> &vector, const unsigned int &addr, const bool &LSB) {
    pushByte(vector, INTEL_INSTR_MOV_REG_eAX_Ov, false);
    pushWord(vector, addr, LSB);
}
void MOVaddr64(std::vector<unsigned char> &vector, const unsigned long &addr, const bool &LSB) {
    pushByte(vector, INTEL_INSTR_MOV_REG_eAX_Ov, false);
    pushDword(vector, addr, LSB);
}
void MOV32(std::vector<unsigned char> &vector, const char *reg, const unsigned int &value, const bool &LSB) {
    pushByte(vector, INTEL_INSTR_MOV_REG_Iv + RegToOffset(reg), false);
    pushWord(vector, value, LSB);
}
void MOV8_low(std::vector<unsigned char> &vector, const char *reg, const unsigned char &value) {
    pushByte(vector, INTEL_INSTR_MOV_REG_Ib_Low + RegToOffset(reg), false);
    pushByte(vector, value, false);
}
void MOV8_high(std::vector<unsigned char> &vector, const char *reg, const unsigned char &value) {
    pushByte(vector, INTEL_INSTR_MOV_REG_Ib_Low + RegToOffset(reg), false);
    pushByte(vector, value, false);
}
void INT(std::vector<unsigned char> &vector, unsigned char value) {
    pushByte(vector, INTEL_INSTR_INT_Ib, false);
    pushByte(vector, value, false);
}
void SYSCALL(std::vector<unsigned char> &vector, const bool &isLinux) {
    if (isLinux) {
        INT(vector,0x80);
    } else {
        
    }
}
void JMP32(std::vector<unsigned char> &vector, unsigned int value, const bool &LSB) {
    pushByte(vector, INTEL_INSTR_JMP_Ap, false);
    pushWord(vector, value, LSB);
}
void JMP64(std::vector<unsigned char> &vector, unsigned long value, const bool &LSB) {
    pushByte(vector, INTEL_INSTR_JMP_Ap, false);
    pushDword(vector, value, LSB);
}
void JMPoffset(std::vector<unsigned char> &vector, unsigned char value) {
    pushByte(vector, INTEL_INSTR_JMP_Jb, false);
    pushByte(vector, value, false);
}

#pragma endregion// instructions
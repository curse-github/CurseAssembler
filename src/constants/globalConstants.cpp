#include <string.h>
#include <iostream>

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

void pushByte(std::ofstream &stream, const unsigned char &byte) {
    pushChars(stream, &byte, 1, false);
}
void pushByte(std::vector<unsigned char> &vector, const unsigned char &byte) {
    pushChars(vector, &byte, 1, false);
}

void pushHalfWord(std::ofstream &stream, const unsigned short &halfword, const bool &LSB) {
    pushChars(stream, static_cast<const unsigned char *>(static_cast<const void *>(&halfword)), 2, LSB);
}
void pushHalfWord(std::vector<unsigned char> &vector, const unsigned short &halfword, const bool &LSB) {
    pushChars(vector, static_cast<const unsigned char *>(static_cast<const void *>(&halfword)), 2, LSB);
}

void pushWord(std::ofstream &stream, const unsigned int &word, const bool &LSB) {
    pushChars(stream, static_cast<const unsigned char *>(static_cast<const void *>(&word)), 4, LSB);
}
void pushWord(std::vector<unsigned char> &vector, const unsigned int &word, const bool &LSB) {
    pushChars(vector, static_cast<const unsigned char *>(static_cast<const void *>(&word)), 4, LSB);
}

void pushDword(std::ofstream &stream, const unsigned long &dword, const bool &LSB) {
    pushChars(stream, static_cast<const unsigned char *>(static_cast<const void *>(&dword)), 8, LSB);
}
void pushDword(std::vector<unsigned char> &vector, const unsigned long &dword, const bool &LSB) {
    pushChars(vector, static_cast<const unsigned char *>(static_cast<const void *>(&dword)), 8, LSB);
}

void padBytes(std::ofstream &stream, const unsigned int &numBytes) {
    unsigned char C8[5]{0x00,0x00,0x00,0x00,0x00};
    for (int i = 0; i < numBytes; i++)
    {
        stream << C8[i%5];
        std::cout << C8[i%5];
    }
    
}
#pragma endregion  // helper functions

#pragma region instructions
unsigned char RegToOffset(const char *reg) {
    unsigned char offset = 0;
    if (strcmp(reg, "eAX") == 0)
        offset = INTEL_REG_OFF_eAX;
    else if (strcmp(reg, "eCX") == 0)
        offset = INTEL_REG_OFF_eCX;
    else if (strcmp(reg, "eDX") == 0)
        offset = INTEL_REG_OFF_eDX;
    else if (strcmp(reg, "eBX") == 0)
        offset = INTEL_REG_OFF_eBX;
    else if (strcmp(reg, "eSP") == 0)
        offset = INTEL_REG_OFF_eSP;
    else if (strcmp(reg, "eBP") == 0)
        offset = INTEL_REG_OFF_eBP;
    else if (strcmp(reg, "eSI") == 0)
        offset = INTEL_REG_OFF_eSI;
    else if (strcmp(reg, "eDI") == 0)
        offset = INTEL_REG_OFF_eDI;
    else {
        std::cout << "Invalid register: \"" << reg << "\"." << std::endl;
        return 150;
    }
    return offset;
}
unsigned char RegToModRMreg(const char *reg) {
    unsigned char offset = 0;
    if (strcmp(reg, "eAX") == 0)
        offset = INTEL_ModRM_REG_eAX;
    else if (strcmp(reg, "eCX") == 0)
        offset = INTEL_ModRM_REG_eCX;
    else if (strcmp(reg, "eDX") == 0)
        offset = INTEL_ModRM_REG_eDX;
    else if (strcmp(reg, "eBX") == 0)
        offset = INTEL_ModRM_REG_eBX;
    else if (strcmp(reg, "eSP") == 0)
        offset = INTEL_ModRM_REG_eSP;
    else if (strcmp(reg, "eBP") == 0)
        offset = INTEL_ModRM_REG_eBP;
    else if (strcmp(reg, "eSI") == 0)
        offset = INTEL_ModRM_REG_eSI;
    else if (strcmp(reg, "eDI") == 0)
        offset = INTEL_ModRM_REG_eDI;
    else {
        std::cout << "Invalid register." << std::endl;
        return 150;
    }
    return offset;
}
unsigned char RegToModRMrm(const char *reg) {
    unsigned char offset = 0;
    if (strcmp(reg, "eAX") == 0)
        offset = INTEL_ModRM_RM_eAX;
    else if (strcmp(reg, "eCX") == 0)
        offset = INTEL_ModRM_RM_eCX;
    else if (strcmp(reg, "eDX") == 0)
        offset = INTEL_ModRM_RM_eDX;
    else if (strcmp(reg, "eBX") == 0)
        offset = INTEL_ModRM_RM_eBX;
    else if (strcmp(reg, "eSP") == 0)
        offset = INTEL_ModRM_RM_eSP;
    else if (strcmp(reg, "eBP") == 0)
        offset = INTEL_ModRM_RM_eBP;
    else if (strcmp(reg, "eSI") == 0)
        offset = INTEL_ModRM_RM_eSI;
    else if (strcmp(reg, "eDI") == 0)
        offset = INTEL_ModRM_RM_eDI;
    else {
        std::cout << "Invalid register." << std::endl;
        return 150;
    }
    return offset;
}


#pragma endregion  // instructions
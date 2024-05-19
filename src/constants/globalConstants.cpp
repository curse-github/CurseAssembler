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
    for (int i = 0; i < numBytes; i++) {
        stream << C8[i%5];
        std::cout << C8[i%5];
    }
    
}

#pragma region 32 bit reg helpers
unsigned char RegToOffset32(const char *reg) {
    unsigned char offset = 0;
    if (strcmp(reg, "eAX") == 0) offset = INTEL_REG_OFF_eAX;
    else if (strcmp(reg, "eCX") == 0) offset = INTEL_REG_OFF_eCX;
    else if (strcmp(reg, "eDX") == 0) offset = INTEL_REG_OFF_eDX;
    else if (strcmp(reg, "eBX") == 0) offset = INTEL_REG_OFF_eBX;
    else if (strcmp(reg, "eSP") == 0) offset = INTEL_REG_OFF_eSP;
    else if (strcmp(reg, "eBP") == 0) offset = INTEL_REG_OFF_eBP;
    else if (strcmp(reg, "eSI") == 0) offset = INTEL_REG_OFF_eSI;
    else if (strcmp(reg, "eDI") == 0) offset = INTEL_REG_OFF_eDI;
    else {
        std::cout << "Invalid register: \"" << reg << "\"." << std::endl;
        return 150;
    }
    return offset;
}
unsigned char RegToModrmReg32(const char *reg) {
    unsigned char offset = 0;
    if (strcmp(reg, "eAX") == 0) offset = INTEL_ModRM_REG_eAX;
    else if (strcmp(reg, "eCX") == 0) offset = INTEL_ModRM_REG_eCX;
    else if (strcmp(reg, "eDX") == 0) offset = INTEL_ModRM_REG_eDX;
    else if (strcmp(reg, "eBX") == 0) offset = INTEL_ModRM_REG_eBX;
    else if (strcmp(reg, "eSP") == 0) offset = INTEL_ModRM_REG_eSP;
    else if (strcmp(reg, "eBP") == 0) offset = INTEL_ModRM_REG_eBP;
    else if (strcmp(reg, "eSI") == 0) offset = INTEL_ModRM_REG_eSI;
    else if (strcmp(reg, "eDI") == 0) offset = INTEL_ModRM_REG_eDI;
    else {
        std::cout << "Invalid register." << std::endl;
        return 150;
    }
    return offset;
}
unsigned char RegToModrmRM32(const char *reg) {
    unsigned char offset = 0;
    if (strcmp(reg, "eAX") == 0) offset = INTEL_ModRM_RM_eAX;
    else if (strcmp(reg, "eCX") == 0) offset = INTEL_ModRM_RM_eCX;
    else if (strcmp(reg, "eDX") == 0) offset = INTEL_ModRM_RM_eDX;
    else if (strcmp(reg, "eBX") == 0) offset = INTEL_ModRM_RM_eBX;
    else if (strcmp(reg, "eSP") == 0) offset = INTEL_ModRM_RM_eSP;
    else if (strcmp(reg, "eBP") == 0) offset = INTEL_ModRM_RM_eBP;
    else if (strcmp(reg, "eSI") == 0) offset = INTEL_ModRM_RM_eSI;
    else if (strcmp(reg, "eDI") == 0) offset = INTEL_ModRM_RM_eDI;
    else {
        std::cout << "Invalid register." << std::endl;
        return 150;
    }
    return offset;
}
#pragma endregion

#pragma region 16 bit reg helpers
unsigned char RegToOffset16(const char *reg) {
    unsigned char offset = 0;
    if (strcmp(reg, "AX") == 0) offset = INTEL_REG_OFF_AX;
    else if (strcmp(reg, "CX") == 0) offset = INTEL_REG_OFF_CX;
    else if (strcmp(reg, "DX") == 0) offset = INTEL_REG_OFF_DX;
    else if (strcmp(reg, "BX") == 0) offset = INTEL_REG_OFF_BX;
    else if (strcmp(reg, "SP") == 0) offset = INTEL_REG_OFF_SP;
    else if (strcmp(reg, "BP") == 0) offset = INTEL_REG_OFF_BP;
    else if (strcmp(reg, "SI") == 0) offset = INTEL_REG_OFF_SI;
    else if (strcmp(reg, "DI") == 0) offset = INTEL_REG_OFF_DI;
    else {
        std::cout << "Invalid register: \"" << reg << "\"." << std::endl;
        return 150;
    }
    return offset;
}
unsigned char RegToModrmReg16(const char *reg) {
    unsigned char offset = 0;
    if (strcmp(reg, "AX") == 0) offset = INTEL_ModRM_REG_AX;
    else if (strcmp(reg, "CX") == 0) offset = INTEL_ModRM_REG_CX;
    else if (strcmp(reg, "DX") == 0) offset = INTEL_ModRM_REG_DX;
    else if (strcmp(reg, "BX") == 0) offset = INTEL_ModRM_REG_BX;
    else if (strcmp(reg, "SP") == 0) offset = INTEL_ModRM_REG_SP;
    else if (strcmp(reg, "BP") == 0) offset = INTEL_ModRM_REG_BP;
    else if (strcmp(reg, "SI") == 0) offset = INTEL_ModRM_REG_SI;
    else if (strcmp(reg, "DI") == 0) offset = INTEL_ModRM_REG_DI;
    else {
        std::cout << "Invalid register." << std::endl;
        return 150;
    }
    return offset;
}
unsigned char RegToModrmRM16(const char *reg) {
    unsigned char offset = 0;
    if (strcmp(reg, "AX") == 0) offset = INTEL_ModRM_RM_AX;
    else if (strcmp(reg, "CX") == 0) offset = INTEL_ModRM_RM_CX;
    else if (strcmp(reg, "DX") == 0) offset = INTEL_ModRM_RM_DX;
    else if (strcmp(reg, "BX") == 0) offset = INTEL_ModRM_RM_BX;
    else if (strcmp(reg, "SP") == 0) offset = INTEL_ModRM_RM_SP;
    else if (strcmp(reg, "BP") == 0) offset = INTEL_ModRM_RM_BP;
    else if (strcmp(reg, "SI") == 0) offset = INTEL_ModRM_RM_SI;
    else if (strcmp(reg, "DI") == 0) offset = INTEL_ModRM_RM_DI;
    else {
        std::cout << "Invalid register." << std::endl;
        return 150;
    }
    return offset;
}
#pragma endregion

#pragma region 8 bit reg helpers
unsigned char RegToOffset8(const char *reg) {
    unsigned char offset = 0;
    if (strcmp(reg, "AL") == 0) offset = INTEL_REG_OFF_AL;
    else if (strcmp(reg, "CL") == 0) offset = INTEL_REG_OFF_CL;
    else if (strcmp(reg, "DL") == 0) offset = INTEL_REG_OFF_DL;
    else if (strcmp(reg, "BL") == 0) offset = INTEL_REG_OFF_BL;
    else if (strcmp(reg, "AH") == 0) offset = INTEL_REG_OFF_AH;
    else if (strcmp(reg, "CH") == 0) offset = INTEL_REG_OFF_CH;
    else if (strcmp(reg, "DH") == 0) offset = INTEL_REG_OFF_DH;
    else if (strcmp(reg, "BH") == 0) offset = INTEL_REG_OFF_BH;
    else {
        std::cout << "Invalid register: \"" << reg << "\"." << std::endl;
        return 150;
    }
    return offset;
}
unsigned char RegToModrmReg8(const char *reg) {
    unsigned char offset = 0;
    if (strcmp(reg, "AL") == 0) offset = INTEL_ModRM_REG_AL;
    else if (strcmp(reg, "CL") == 0) offset = INTEL_ModRM_REG_CL;
    else if (strcmp(reg, "DL") == 0) offset = INTEL_ModRM_REG_DL;
    else if (strcmp(reg, "BL") == 0) offset = INTEL_ModRM_REG_BL;
    else if (strcmp(reg, "AH") == 0) offset = INTEL_ModRM_REG_AH;
    else if (strcmp(reg, "CH") == 0) offset = INTEL_ModRM_REG_CH;
    else if (strcmp(reg, "DH") == 0) offset = INTEL_ModRM_REG_DH;
    else if (strcmp(reg, "BH") == 0) offset = INTEL_ModRM_REG_BH;
    else {
        std::cout << "Invalid register." << std::endl;
        return 150;
    }
    return offset;
}
unsigned char RegToModrmRM8(const char *reg) {
    unsigned char offset = 0;
    if (strcmp(reg, "AL") == 0) offset = INTEL_ModRM_RM_AL;
    else if (strcmp(reg, "CL") == 0) offset = INTEL_ModRM_RM_CL;
    else if (strcmp(reg, "DL") == 0) offset = INTEL_ModRM_RM_DL;
    else if (strcmp(reg, "BL") == 0) offset = INTEL_ModRM_RM_BL;
    else if (strcmp(reg, "AH") == 0) offset = INTEL_ModRM_RM_AH;
    else if (strcmp(reg, "CH") == 0) offset = INTEL_ModRM_RM_CH;
    else if (strcmp(reg, "DH") == 0) offset = INTEL_ModRM_RM_DH;
    else if (strcmp(reg, "BH") == 0) offset = INTEL_ModRM_RM_BH;
    else {
        std::cout << "Invalid register." << std::endl;
        return 150;
    }
    return offset;
}
#pragma endregion

#pragma endregion  // helper functions

#pragma region instructions

#pragma endregion  // instructions
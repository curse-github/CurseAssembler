#include <string.h>
#include <iostream>

#include "globalConstants.h"
#include "intelConstants.h"

#pragma region helper functions
void pushChars(std::ofstream &stream, const uint8_t *chars, uint32_t len, const bool &LSB) {
    if (LSB)
        for (short i = 0; i < len; i++) stream << chars[i];
    else
        for (short i = len - 1; i >= 0; i--) stream << chars[i];
}
void pushChars(std::ofstream &stream, std::vector<uint8_t> &vector) {
    for (size_t i = 0u; i < vector.size(); i++) stream << vector[i];
}
void pushChars(std::vector<uint8_t> &vector, const uint8_t *chars, uint32_t len, const bool &LSB) {
    if (LSB)
        for (char i = 0; i < len; i++) vector.push_back(chars[i]);
    else
        for (char i = len - 1; i >= 0; i--) vector.push_back(chars[i]);
}

void pushByte(std::ofstream &stream, const uint8_t &byte) {
    pushChars(stream, &byte, 1, false);
}
void pushByte(std::vector<uint8_t> &vector, const uint8_t &byte) {
    pushChars(vector, &byte, 1, false);
}

void pushHalfWord(std::ofstream &stream, const uint16_t &halfword, const bool &LSB) {
    pushChars(stream, static_cast<const uint8_t *>(static_cast<const void *>(&halfword)), 2, LSB);
}
void pushHalfWord(std::vector<uint8_t> &vector, const uint16_t &halfword, const bool &LSB) {
    pushChars(vector, static_cast<const uint8_t *>(static_cast<const void *>(&halfword)), 2, LSB);
}

void pushWord(std::ofstream &stream, const uint32_t &word, const bool &LSB) {
    pushChars(stream, static_cast<const uint8_t *>(static_cast<const void *>(&word)), 4, LSB);
}
void pushWord(std::vector<uint8_t> &vector, const uint32_t &word, const bool &LSB) {
    pushChars(vector, static_cast<const uint8_t *>(static_cast<const void *>(&word)), 4, LSB);
}

void pushDword(std::ofstream &stream, const uint64_t &dword, const bool &LSB) {
    pushChars(stream, static_cast<const uint8_t *>(static_cast<const void *>(&dword)), 8, LSB);
}
void pushDword(std::vector<uint8_t> &vector, const uint64_t &dword, const bool &LSB) {
    pushChars(vector, static_cast<const uint8_t *>(static_cast<const void *>(&dword)), 8, LSB);
}

void padBytes(std::ofstream &stream, const uint32_t &numBytes) {
    uint8_t C8[5]{0x00,0x00,0x00,0x00,0x00};
    for (int i = 0; i < numBytes; i++) {
        stream << C8[i%5];
        std::cout << C8[i%5];
    }
    
}

#pragma region 32 bit reg helpers
uint8_t RegToOffset32(const char *reg) {
    uint8_t offset = 0;
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
uint8_t RegToModrmReg32(const char *reg) {
    uint8_t offset = 0;
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
uint8_t RegToModrmRM32(const char *reg) {
    uint8_t offset = 0;
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
uint8_t RegToOffset16(const char *reg) {
    uint8_t offset = 0;
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
uint8_t RegToModrmReg16(const char *reg) {
    uint8_t offset = 0;
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
uint8_t RegToModrmRM16(const char *reg) {
    uint8_t offset = 0;
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
uint8_t RegToOffset8(const char *reg) {
    uint8_t offset = 0;
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
uint8_t RegToModrmReg8(const char *reg) {
    uint8_t offset = 0;
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
uint8_t RegToModrmRM8(const char *reg) {
    uint8_t offset = 0;
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

#pragma endregion helper functions

#pragma region instructions

#pragma region ADD
//2 bytes
template <typename T>
void ADDb(T &receiver, const char *reg1, const char *reg2) {
    pushByte(receiver, INTEL_INSTR_ADD_RMb_REGb);
    pushByte(receiver, INTEL_ModRM_MOD_Reg|RegToModrmReg8(reg1)|RegToModrmRM8(reg2));
}
//2 bytes if reg is eAX, otherwise 3 bytes
template <typename T>
void ADDb(T &receiver, const char *reg, const uint8_t &value) {
    uint8_t rm = RegToModrmRM8(reg);
    if (rm == 0) {// reg == eAX
        pushByte(receiver, INTEL_INSTR_ADD_AL_Ib);
        pushByte(receiver, value);
    } else {
        pushByte(receiver, INTEL_INSTR_OP1_RMb_Ib);
        pushByte(receiver, INTEL_ModRM_MOD_Reg|INTEL_ModRM_OP1_ADD_RM_I|rm);
        pushByte(receiver, value);
    }
}
//2 bytes
template <typename T>
void ADDv(T &receiver, const char *reg1, const char *reg2) {
    pushByte(receiver, INTEL_INSTR_ADD_RMv_REGv);
    pushByte(receiver, INTEL_ModRM_MOD_Reg|RegToModrmReg32(reg1)|RegToModrmRM32(reg2));
}
// 3,4,5 or 5 bytes
template <typename T>
void ADDv(T &receiver, const char *reg, const uint32_t &value) {
    uint8_t rm = RegToModrmRM32(reg);
    if (value>255) {
        if (rm==0) {// reg == eAX and value is larger than a byte can hold
            pushByte(receiver, INTEL_INSTR_ADD_eAX_Iv);
            pushWord(receiver, value, true);
        } else {// reg != eAX and value is larger than a byte can hold
            pushByte(receiver, INTEL_INSTR_OP1_RMv_Iv);
            pushByte(receiver, INTEL_ModRM_MOD_Reg|INTEL_ModRM_OP1_ADD_RM_I|RegToModrmRM32(reg));
            pushWord(receiver, value, true);
        }
    } else {
        pushByte(receiver, INTEL_INSTR_OP1_RMv_Ib);
        pushByte(receiver, INTEL_ModRM_MOD_Reg|INTEL_ModRM_OP1_ADD_RM_I|RegToModrmRM32(reg));
        pushByte(receiver, value);
    }
}
#pragma endregion ADD

#pragma region OR
//2 bytes
template <typename T>
void ORb(T &receiver, const char *reg1, const char *reg2) {
    pushByte(receiver, INTEL_INSTR_OR_RMb_REGb);
    pushByte(receiver, INTEL_ModRM_MOD_Reg|RegToModrmReg8(reg1)|RegToModrmRM8(reg2));
}
//2 bytes if reg is eAX, otherwise 3 bytes
template <typename T>
void ORb(T &receiver, const char *reg, const uint8_t &value) {
    uint8_t rm = RegToModrmRM8(reg);
    if (rm == 0) {// reg == eAX
        pushByte(receiver, INTEL_INSTR_OR_AL_Ib);
        pushByte(receiver, value);
    } else {
        pushByte(receiver, INTEL_INSTR_OP1_RMb_Ib);
        pushByte(receiver, INTEL_ModRM_MOD_Reg|INTEL_ModRM_OP1_OR_RM_I|rm);
        pushByte(receiver, value);
    }
}
//2 bytes
template <typename T>
void ORv(T &receiver, const char *reg1, const char *reg2) {
    pushByte(receiver, INTEL_INSTR_OR_RMv_REGv);
    pushByte(receiver, INTEL_ModRM_MOD_Reg|RegToModrmReg32(reg1)|RegToModrmRM32(reg2));
}
// 3,4,5 or 5 bytes
template <typename T>
void ORv(T &receiver, const char *reg, const uint32_t &value) {
    uint8_t rm = RegToModrmRM32(reg);
    if (value>255) {
        if (rm==0) {// reg == eAX and value is larger than a byte can hold
            pushByte(receiver, INTEL_INSTR_OR_eAX_Iv);
            pushWord(receiver, value, true);
        } else {// reg != eAX and value is larger than a byte can hold
            pushByte(receiver, INTEL_INSTR_OP1_RMv_Iv);
            pushByte(receiver, INTEL_ModRM_MOD_Reg|INTEL_ModRM_OP1_OR_RM_I|RegToModrmRM32(reg));
            pushWord(receiver, value, true);
        }
    } else {
        pushByte(receiver, INTEL_INSTR_OP1_RMv_Ib);
        pushByte(receiver, INTEL_ModRM_MOD_Reg|INTEL_ModRM_OP1_OR_RM_I|RegToModrmRM32(reg));
        pushByte(receiver, value);
    }
}
#pragma endregion OR

#pragma region ADC
//2 bytes
template <typename T>
void ADCb(T &receiver, const char *reg1, const char *reg2) {
    pushByte(receiver, INTEL_INSTR_ADC_RMb_REGb);
    pushByte(receiver, INTEL_ModRM_MOD_Reg|RegToModrmReg8(reg1)|RegToModrmRM8(reg2));
}
//2 bytes if reg is eAX, otherwise 3 bytes
template <typename T>
void ADCb(T &receiver, const char *reg, const uint8_t &value) {
    uint8_t rm = RegToModrmRM8(reg);
    if (rm == 0) {// reg == eAX
        pushByte(receiver, INTEL_INSTR_ADC_AL_Ib);
        pushByte(receiver, value);
    } else {
        pushByte(receiver, INTEL_INSTR_OP1_RMb_Ib);
        pushByte(receiver, INTEL_ModRM_MOD_Reg|INTEL_ModRM_OP1_ADC_RM_I|rm);
        pushByte(receiver, value);
    }
}
//2 bytes
template <typename T>
void ADCv(T &receiver, const char *reg1, const char *reg2) {
    pushByte(receiver, INTEL_INSTR_ADC_RMv_REGv);
    pushByte(receiver, INTEL_ModRM_MOD_Reg|RegToModrmReg32(reg1)|RegToModrmRM32(reg2));
}
// 3,4,5 or 5 bytes
template <typename T>
void ADCv(T &receiver, const char *reg, const uint32_t &value) {
    uint8_t rm = RegToModrmRM32(reg);
    if (value>255) {
        if (rm==0) {// reg == eAX and value is larger than a byte can hold
            pushByte(receiver, INTEL_INSTR_ADC_eAX_Iv);
            pushWord(receiver, value, true);
        } else {// reg != eAX and value is larger than a byte can hold
            pushByte(receiver, INTEL_INSTR_OP1_RMv_Iv);
            pushByte(receiver, INTEL_ModRM_MOD_Reg|INTEL_ModRM_OP1_ADC_RM_I|RegToModrmRM32(reg));
            pushWord(receiver, value, true);
        }
    } else {
        pushByte(receiver, INTEL_INSTR_OP1_RMv_Ib);
        pushByte(receiver, INTEL_ModRM_MOD_Reg|INTEL_ModRM_OP1_ADC_RM_I|RegToModrmRM32(reg));
        pushByte(receiver, value);
    }
}
#pragma endregion ADC

#pragma region SBB
//2 bytes
template <typename T>
void SBBb(T &receiver, const char *reg1, const char *reg2) {
    pushByte(receiver, INTEL_INSTR_SBB_RMb_REGb);
    pushByte(receiver, INTEL_ModRM_MOD_Reg|RegToModrmReg8(reg1)|RegToModrmRM8(reg2));
}
//2 bytes if reg is eAX, otherwise 3 bytes
template <typename T>
void SBBb(T &receiver, const char *reg, const uint8_t &value) {
    uint8_t rm = RegToModrmRM8(reg);
    if (rm == 0) {// reg == eAX
        pushByte(receiver, INTEL_INSTR_SBB_AL_Ib);
        pushByte(receiver, value);
    } else {
        pushByte(receiver, INTEL_INSTR_OP1_RMb_Ib);
        pushByte(receiver, INTEL_ModRM_MOD_Reg|INTEL_ModRM_OP1_SBB_RM_I|rm);
        pushByte(receiver, value);
    }
}
//2 bytes
template <typename T>
void SBBv(T &receiver, const char *reg1, const char *reg2) {
    pushByte(receiver, INTEL_INSTR_SBB_RMv_REGv);
    pushByte(receiver, INTEL_ModRM_MOD_Reg|RegToModrmReg32(reg1)|RegToModrmRM32(reg2));
}
// 3,4,5 or 5 bytes
template <typename T>
void SBBv(T &receiver, const char *reg, const uint32_t &value) {
    uint8_t rm = RegToModrmRM32(reg);
    if (value>255) {
        if (rm==0) {// reg == eAX and value is larger than a byte can hold
            pushByte(receiver, INTEL_INSTR_SBB_eAX_Iv);
            pushWord(receiver, value, true);
        } else {// reg != eAX and value is larger than a byte can hold
            pushByte(receiver, INTEL_INSTR_OP1_RMv_Iv);
            pushByte(receiver, INTEL_ModRM_MOD_Reg|INTEL_ModRM_OP1_SBB_RM_I|RegToModrmRM32(reg));
            pushWord(receiver, value, true);
        }
    } else {
        pushByte(receiver, INTEL_INSTR_OP1_RMv_Ib);
        pushByte(receiver, INTEL_ModRM_MOD_Reg|INTEL_ModRM_OP1_SBB_RM_I|RegToModrmRM32(reg));
        pushByte(receiver, value);
    }
}
#pragma endregion SBB

#pragma region AND
//2 bytes
template <typename T>
void ANDb(T &receiver, const char *reg1, const char *reg2) {
    pushByte(receiver, INTEL_INSTR_AND_RMb_REGb);
    pushByte(receiver, INTEL_ModRM_MOD_Reg|RegToModrmReg8(reg1)|RegToModrmRM8(reg2));
}
//2 bytes if reg is eAX, otherwise 3 bytes
template <typename T>
void ANDb(T &receiver, const char *reg, const uint8_t &value) {
    uint8_t rm = RegToModrmRM8(reg);
    if (rm == 0) {// reg == eAX
        pushByte(receiver, INTEL_INSTR_AND_AL_Ib);
        pushByte(receiver, value);
    } else {
        pushByte(receiver, INTEL_INSTR_OP1_RMb_Ib);
        pushByte(receiver, INTEL_ModRM_MOD_Reg|INTEL_ModRM_OP1_AND_RM_I|rm);
        pushByte(receiver, value);
    }
}
//2 bytes
template <typename T>
void ANDv(T &receiver, const char *reg1, const char *reg2) {
    pushByte(receiver, INTEL_INSTR_AND_RMv_REGv);
    pushByte(receiver, INTEL_ModRM_MOD_Reg|RegToModrmReg32(reg1)|RegToModrmRM32(reg2));
}
// 3,4,5 or 5 bytes
template <typename T>
void ANDv(T &receiver, const char *reg, const uint32_t &value) {
    uint8_t rm = RegToModrmRM32(reg);
    if (value>255) {
        if (rm==0) {// reg == eAX and value is larger than a byte can hold
            pushByte(receiver, INTEL_INSTR_AND_eAX_Iv);
            pushWord(receiver, value, true);
        } else {// reg != eAX and value is larger than a byte can hold
            pushByte(receiver, INTEL_INSTR_OP1_RMv_Iv);
            pushByte(receiver, INTEL_ModRM_MOD_Reg|INTEL_ModRM_OP1_AND_RM_I|RegToModrmRM32(reg));
            pushWord(receiver, value, true);
        }
    } else {
        pushByte(receiver, INTEL_INSTR_OP1_RMv_Ib);
        pushByte(receiver, INTEL_ModRM_MOD_Reg|INTEL_ModRM_OP1_AND_RM_I|RegToModrmRM32(reg));
        pushByte(receiver, value);
    }
}
#pragma endregion AND

#pragma region SUB
//2 bytes
template <typename T>
void SUBb(T &receiver, const char *reg1, const char *reg2) {
    pushByte(receiver, INTEL_INSTR_SUB_RMb_REGb);
    pushByte(receiver, INTEL_ModRM_MOD_Reg|RegToModrmReg8(reg1)|RegToModrmRM8(reg2));
}
//2 bytes if reg is eAX, otherwise 3 bytes
template <typename T>
void SUBb(T &receiver, const char *reg, const uint8_t &value) {
    uint8_t rm = RegToModrmRM8(reg);
    if (rm == 0) {// reg == eAX
        pushByte(receiver, INTEL_INSTR_SUB_AL_Ib);
        pushByte(receiver, value);
    } else {
        pushByte(receiver, INTEL_INSTR_OP1_RMb_Ib);
        pushByte(receiver, INTEL_ModRM_MOD_Reg|INTEL_ModRM_OP1_SUB_RM_I|rm);
        pushByte(receiver, value);
    }
}
//2 bytes
template <typename T>
void SUBv(T &receiver, const char *reg1, const char *reg2) {
    pushByte(receiver, INTEL_INSTR_SUB_RMv_REGv);
    pushByte(receiver, INTEL_ModRM_MOD_Reg|RegToModrmReg32(reg1)|RegToModrmRM32(reg2));
}
// 3,4,5 or 5 bytes
template <typename T>
void SUBv(T &receiver, const char *reg, const uint32_t &value) {
    uint8_t rm = RegToModrmRM32(reg);
    if (value>255) {
        if (rm==0) {// reg == eAX and value is larger than a byte can hold
            pushByte(receiver, INTEL_INSTR_SUB_eAX_Iv);
            pushWord(receiver, value, true);
        } else {// reg != eAX and value is larger than a byte can hold
            pushByte(receiver, INTEL_INSTR_OP1_RMv_Iv);
            pushByte(receiver, INTEL_ModRM_MOD_Reg|INTEL_ModRM_OP1_SUB_RM_I|RegToModrmRM32(reg));
            pushWord(receiver, value, true);
        }
    } else {
        pushByte(receiver, INTEL_INSTR_OP1_RMv_Ib);
        pushByte(receiver, INTEL_ModRM_MOD_Reg|INTEL_ModRM_OP1_SUB_RM_I|RegToModrmRM32(reg));
        pushByte(receiver, value);
    }
}
#pragma endregion SUB

#pragma region XOR
//2 bytes
template <typename T>
void XORb(T &receiver, const char *reg1, const char *reg2) {
    pushByte(receiver, INTEL_INSTR_XOR_RMb_REGb);
    pushByte(receiver, INTEL_ModRM_MOD_Reg|RegToModrmReg8(reg1)|RegToModrmRM8(reg2));
}
//2 bytes if reg is eAX, otherwise 3 bytes
template <typename T>
void XORb(T &receiver, const char *reg, const uint8_t &value) {
    uint8_t rm = RegToModrmRM8(reg);
    if (rm == 0) {// reg == eAX
        pushByte(receiver, INTEL_INSTR_XOR_AL_Ib);
        pushByte(receiver, value);
    } else {
        pushByte(receiver, INTEL_INSTR_OP1_RMb_Ib);
        pushByte(receiver, INTEL_ModRM_MOD_Reg|INTEL_ModRM_OP1_XOR_RM_I|rm);
        pushByte(receiver, value);
    }
}
//2 bytes
template <typename T>
void XORv(T &receiver, const char *reg1, const char *reg2) {
    pushByte(receiver, INTEL_INSTR_XOR_RMv_REGv);
    pushByte(receiver, INTEL_ModRM_MOD_Reg|RegToModrmReg32(reg1)|RegToModrmRM32(reg2));
}
// 3,4,5 or 5 bytes
template <typename T>
void XORv(T &receiver, const char *reg, const uint32_t &value) {
    uint8_t rm = RegToModrmRM32(reg);
    if (value>255) {
        if (rm==0) {// reg == eAX and value is larger than a byte can hold
            pushByte(receiver, INTEL_INSTR_XOR_eAX_Iv);
            pushWord(receiver, value, true);
        } else {// reg != eAX and value is larger than a byte can hold
            pushByte(receiver, INTEL_INSTR_OP1_RMv_Iv);
            pushByte(receiver, INTEL_ModRM_MOD_Reg|INTEL_ModRM_OP1_XOR_RM_I|RegToModrmRM32(reg));
            pushWord(receiver, value, true);
        }
    } else {
        pushByte(receiver, INTEL_INSTR_OP1_RMv_Ib);
        pushByte(receiver, INTEL_ModRM_MOD_Reg|INTEL_ModRM_OP1_XOR_RM_I|RegToModrmRM32(reg));
        pushByte(receiver, value);
    }
}
#pragma endregion XOR

#pragma region CMP
//2 bytes
template <typename T>
void CMPb(T &receiver, const char *reg1, const char *reg2) {
    pushByte(receiver, INTEL_INSTR_CMP_RMb_REGb);
    pushByte(receiver, INTEL_ModRM_MOD_Reg|RegToModrmReg8(reg1)|RegToModrmRM8(reg2));
}
//2 bytes if reg is eAX, otherwise 3 bytes
template <typename T>
void CMPb(T &receiver, const char *reg, const uint8_t &value) {
    uint8_t rm = RegToModrmRM8(reg);
    if (rm == 0) {// reg == eAX
        pushByte(receiver, INTEL_INSTR_CMP_AL_Ib);
        pushByte(receiver, value);
    } else {
        pushByte(receiver, INTEL_INSTR_OP1_RMb_Ib);
        pushByte(receiver, INTEL_ModRM_MOD_Reg|INTEL_ModRM_OP1_CMP_RM_I|rm);
        pushByte(receiver, value);
    }
}
//2 bytes
template <typename T>
void CMPv(T &receiver, const char *reg1, const char *reg2) {
    pushByte(receiver, INTEL_INSTR_CMP_RMv_REGv);
    pushByte(receiver, INTEL_ModRM_MOD_Reg|RegToModrmReg32(reg1)|RegToModrmRM32(reg2));
}
// 3,4,5 or 5 bytes
template <typename T>
void CMPv(T &receiver, const char *reg, const uint32_t &value) {
    uint8_t rm = RegToModrmRM32(reg);
    if (value>255) {
        if (rm==0) {// reg == eAX and value is larger than a byte can hold
            pushByte(receiver, INTEL_INSTR_CMP_eAX_Iv);
            pushWord(receiver, value, true);
        } else {// reg != eAX and value is larger than a byte can hold
            pushByte(receiver, INTEL_INSTR_OP1_RMv_Iv);
            pushByte(receiver, INTEL_ModRM_MOD_Reg|INTEL_ModRM_OP1_CMP_RM_I|RegToModrmRM32(reg));
            pushWord(receiver, value, true);
        }
    } else {
        pushByte(receiver, INTEL_INSTR_OP1_RMv_Ib);
        pushByte(receiver, INTEL_ModRM_MOD_Reg|INTEL_ModRM_OP1_CMP_RM_I|RegToModrmRM32(reg));
        pushByte(receiver, value);
    }
}
#pragma endregion CMP

#pragma region INC
//has indirect version
//2 bytes
template <typename T>
void INCb(T &receiver, const char *reg) {
    pushByte(receiver, INTEL_INSTR_OP3b);
    pushByte(receiver, INTEL_ModRM_MOD_Reg|INTEL_ModRM_OP3_INC_RM|RegToModrmRM8(reg));
}
//has indirect version
//1 byte
template <typename T>
void INCv(T &receiver, const char *reg) {
    pushByte(receiver, INTEL_INSTR32_INCpRv+RegToOffset32(reg));
}
#pragma endregion INC

#pragma region DEC
//has indirect version
//2 bytes
template <typename T>
void DECb(T &receiver, const char *reg) {
    pushByte(receiver, INTEL_INSTR_OP3b);
    pushByte(receiver, INTEL_ModRM_MOD_Reg|INTEL_ModRM_OP3_DEC_RM|RegToModrmRM8(reg));
}
//has indirect version
//1 byte
template <typename T>
void DECv(T &receiver, const char *reg) {
    pushByte(receiver, INTEL_INSTR32_DECpRv+RegToOffset32(reg));
}
#pragma endregion DEC

#pragma region CALL
//5 bytes
template <typename T>
void CALL(T &receiver, const uint32_t &value) {
    pushByte(receiver, INTEL_INSTR_CALL_Jv);
    pushWord(receiver, value, true);
}
//2 bytes
template <typename T>
void CALL(T &receiver, const char *reg) {
    pushByte(receiver, INTEL_INSTR_OP3v);
    pushByte(receiver, INTEL_ModRM_MOD_Reg|INTEL_ModRM_OP3_CALL_RM|RegToModrmRM32(reg));
}
#pragma endregion CALL

#pragma region JMP
//2 bytes
template <typename T>
void JMPb(T &receiver, const uint8_t &value) {
    pushByte(receiver, INTEL_INSTR_JMP_Jb);
    pushByte(receiver, value);
}
//5 bytes
template <typename T>
void JMPv(T &receiver, const uint32_t &value) {
    pushByte(receiver, INTEL_INSTR_JMP_Jv);
    pushWord(receiver, value, true);
}
//2 bytes
template <typename T>
void JMPv(T &receiver, const char *reg) {
    pushByte(receiver, INTEL_INSTR_OP3v);
    pushByte(receiver, INTEL_ModRM_MOD_Reg|INTEL_ModRM_OP3_JMP_RM|RegToModrmRM32(reg));
}
#pragma endregion JMP

#pragma region PUSH
//2 bytes
template <typename T>
void PUSHb(T &receiver, const uint8_t &value) {
    pushByte(receiver, INTEL_INSTR_PUSH_Ib);
    pushByte(receiver, value);
}
//5 bytes
template <typename T>
void PUSHv(T &receiver, const uint32_t &value) {
    pushByte(receiver, INTEL_INSTR_PUSH_Iv);
    pushWord(receiver, value, true);
}
//1 byte
template <typename T>
void PUSHv(T &receiver, const char *reg) {
    pushByte(receiver, INTEL_INSTR32_PUSHpRv+RegToOffset32(reg));
}
#pragma endregion PUSH

//1 byte
template <typename T>
void POP(T &receiver, const char *reg) {
    pushByte(receiver, INTEL_INSTR32_POPpRv+RegToOffset32(reg));
}

#pragma region J
template <typename T>
void JOV(T &receiver, const uint8_t &value) {
    pushByte(receiver,INTEL_INSTR_JIF_Ov);
    pushByte(receiver,value);
}
template <typename T>
void JNOV(T &receiver, const uint8_t &value) {
    pushByte(receiver,INTEL_INSTR_JIF_NOv);
    pushByte(receiver,value);
}
template <typename T>
void JULT(T &receiver, const uint8_t &value) {
    pushByte(receiver,INTEL_INSTR_JIF_uLT);
    pushByte(receiver,value);
}
template <typename T>
void JUGE(T &receiver, const uint8_t &value) {
    pushByte(receiver,INTEL_INSTR_JIF_uGT);
    pushByte(receiver,value);
}
template <typename T>
void JET(T &receiver, const uint8_t &value) {
    pushByte(receiver,INTEL_INSTR_JIF_ET);
    pushByte(receiver,value);
}
template <typename T>
void JNE(T &receiver, const uint8_t &value) {
    pushByte(receiver,INTEL_INSTR_JIF_NE);
    pushByte(receiver,value);
}
template <typename T>
void JULE(T &receiver, const uint8_t &value) {
    pushByte(receiver,INTEL_INSTR_JIF_uLE);
    pushByte(receiver,value);
}
template <typename T>
void JUGT(T &receiver, const uint8_t &value) {
    pushByte(receiver,INTEL_INSTR_JIF_uGT);
    pushByte(receiver,value);
}
template <typename T>
void JS(T &receiver, const uint8_t &value) {
    pushByte(receiver,INTEL_INSTR_JIF_Sign);
    pushByte(receiver,value);
}
template <typename T>
void JNS(T &receiver, const uint8_t &value) {
    pushByte(receiver,INTEL_INSTR_JIF_NSign);
    pushByte(receiver,value);
}
template <typename T>
void JP(T &receiver, const uint8_t &value) {
    pushByte(receiver,INTEL_INSTR_JIF_Parity);
    pushByte(receiver,value);
}
template <typename T>
void JNP(T &receiver, const uint8_t &value) {
    pushByte(receiver,INTEL_INSTR_JIF_NParity);
    pushByte(receiver,value);
}
template <typename T>
void JLT(T &receiver, const uint8_t &value) {
    pushByte(receiver,INTEL_INSTR_JIF_LT);
    pushByte(receiver,value);
}
template <typename T>
void JGE(T &receiver, const uint8_t &value) {
    pushByte(receiver,INTEL_INSTR_JIF_GE);
    pushByte(receiver,value);
}
template <typename T>
void JLE(T &receiver, const uint8_t &value) {
    pushByte(receiver,INTEL_INSTR_JIF_LE);
    pushByte(receiver,value);
}
template <typename T>
void JGT(T &receiver, const uint8_t &value) {
    pushByte(receiver,INTEL_INSTR_JIF_GT);
    pushByte(receiver,value);
}
template <typename T>
void JCXZ(T &receiver, const uint8_t &value) {
    pushByte(receiver,INTEL_INSTR_JMP_eCXeq0_Jb);
    pushByte(receiver,value);
}
#pragma endregion J

//1 byte
template <typename T>
void NOP(T &receiver) {
    pushByte(receiver, INTEL_INSTR_NOP);
}

#pragma region XCHG
//2 bytes
template <typename T>
void XCHGb(T &receiver, const char *reg1, const char *reg2) {
    pushByte(receiver, INTEL_INSTR_XCHG_REGb_RMb);
    pushByte(receiver, INTEL_ModRM_MOD_Reg|RegToModrmReg8(reg1)|RegToModrmRM8(reg2));
}
//1 byte if either reg is eAX, otherwise 2 bytes
template <typename T>
void XCHGv(T &receiver, const char *reg1, const char *reg2) {
    uint8_t reg = RegToModrmReg32(reg1);
    if (reg==0) {// reg1 == eAX
        pushByte(receiver, INTEL_INSTR_XCHG_eAX_REGpRv+RegToOffset32(reg2));// might be NOP lol
    } else {
        uint8_t rm = RegToModrmRM32(reg2);
        if (rm==0) {// reg2 == eAX
            pushByte(receiver, INTEL_INSTR_XCHG_eAX_REGpRv+RegToOffset32(reg1));
        } else {
            pushByte(receiver, INTEL_INSTR_XCHG_REGv_RMv);
            pushByte(receiver, INTEL_ModRM_MOD_Reg|reg|rm);
        }
    }
}
#pragma endregion XCHG

#pragma region MOV
//2 bytes
template <typename T>
void MOVb(T &receiver, const char *reg1, const char *reg2) {
    pushByte(receiver, INTEL_INSTR_MOV_RMb_REGb);
    pushByte(receiver, INTEL_ModRM_MOD_Reg|RegToModrmReg8(reg1)|RegToModrmRM8(reg2));
}
//2 bytes
template <typename T>
void MOVv(T &receiver, const char *reg1, const char *reg2) {
    pushByte(receiver, INTEL_INSTR_MOV_RMv_REGv);
    pushByte(receiver, INTEL_ModRM_MOD_Reg|RegToModrmReg32(reg1)|RegToModrmRM32(reg2));
}
//1 byte
template <typename T>
void MOVb(T &receiver, const char *reg, const uint8_t &value) {
    pushByte(receiver, INTEL_INSTR_MOV_REGpRb_Ib+RegToOffset8(reg));
    pushByte(receiver,value);
}
//1 byte
template <typename T>
void MOVv(T &receiver, const char *reg, const uint32_t &value) {
    pushByte(receiver, INTEL_INSTR_MOV_REGpRv_Iv+RegToOffset32(reg));
    pushWord(receiver,value,true);
}
#pragma endregion MOV

#pragma region RET
//3 bytes
template <typename T>
void RETN(T &receiver, const uint16_t &value) {
    pushByte(receiver, INTEL_INSTR_RETN_I16);
    pushHalfWord(receiver, value, true);
}
//1 byte
template <typename T>
void RETN(T &receiver) {
    pushByte(receiver, INTEL_INSTR_RETN);
}
//3 bytes
template <typename T>
void RETF(T &receiver, const uint16_t &value) {
    pushByte(receiver, INTEL_INSTR_RETF_I16);
    pushHalfWord(receiver, value, true);
}
//1 byte
template <typename T>
void RETF(T &receiver) {
    pushByte(receiver, INTEL_INSTR_RETF);
}
#pragma endregion RET

#pragma region INT
//1 byte
template <typename T>
void INT(T &receiver) {
    pushByte(receiver, INTEL_INSTR_INT);
}
//2 bytes
template <typename T>
void INT(T &receiver, const uint8_t &value) {
    pushByte(receiver, INTEL_INSTR_INT_Ib);
    pushByte(receiver, value);
}
#pragma endregion INT

#pragma region LOOP
//2 bytes
template <typename T>
void LOOPNE(T &receiver, const uint8_t &value) {
    pushByte(receiver, INTEL_INSTR_LOOPn0_eCX_Jb);
    pushByte(receiver, value);
}
//2 bytes
template <typename T>
void LOOPET(T &receiver, const uint8_t &value) {
    pushByte(receiver, INTEL_INSTR_LOOP0_eCX_Jb);
    pushByte(receiver, value);
}
//2 bytes
template <typename T>
void LOOP(T &receiver, const uint8_t &value) {
    pushByte(receiver, INTEL_INSTR_LOOP_eCX_Jb);
    pushByte(receiver, value);
}
#pragma endregion LOOP

//1 byte
template <typename T>
void HLT(T &receiver) {
    pushByte(receiver, INTEL_INSTR_HLT);
}

#pragma region OP2
//3 bytes
template <typename T>
void TESTb(T &receiver, const char *reg, const uint8_t &value) {
    pushByte(receiver, INTEL_INSTR_OP2b);
    pushByte(receiver, INTEL_ModRM_MOD_Reg|INTEL_ModRM_OP2_TEST_RM_I|RegToModrmRM8(reg));
    pushByte(receiver, value);
}
//6 bytes
template <typename T>
void TESTv(T &receiver, const char *reg, const uint32_t &value) {
    pushByte(receiver, INTEL_INSTR_OP2v);
    pushByte(receiver, INTEL_ModRM_MOD_Reg|INTEL_ModRM_OP2_TEST_RM_I|RegToModrmRM32(reg));
    pushWord(receiver, value, true);
}
//2 bytes
template <typename T>
void NOTb(T &receiver, const char *reg) {
    pushByte(receiver, INTEL_INSTR_OP2b);
    pushByte(receiver, INTEL_ModRM_MOD_Reg|INTEL_ModRM_OP2_NOT_RM|RegToModrmRM8(reg));
}
//2 bytes
template <typename T>
void NOTv(T &receiver, const char *reg) {
    pushByte(receiver, INTEL_INSTR_OP2v);
    pushByte(receiver, INTEL_ModRM_MOD_Reg|INTEL_ModRM_OP2_NOT_RM|RegToModrmRM32(reg));
}
//2 bytes
template <typename T>
void NEGb(T &receiver, const char *reg) {
    pushByte(receiver, INTEL_INSTR_OP2b);
    pushByte(receiver, INTEL_ModRM_MOD_Reg|INTEL_ModRM_OP2_NEG_RM|RegToModrmRM8(reg));
}
//2 bytes
template <typename T>
void NEGv(T &receiver, const char *reg) {
    pushByte(receiver, INTEL_INSTR_OP2v);
    pushByte(receiver, INTEL_ModRM_MOD_Reg|INTEL_ModRM_OP2_NEG_RM|RegToModrmRM32(reg));
}
#pragma endregion OP2

#pragma region clr/set flag instructions
//1 byte
template <typename T>
void CLC(T &receiver) {
    pushByte(receiver, INTEL_INSTR_CLR_CF);
}
//1 byte
template <typename T>
void STC(T &receiver) {
    pushByte(receiver, INTEL_INSTR_SET_CF);
}
//1 byte
template <typename T>
void CLI(T &receiver) {
    pushByte(receiver, INTEL_INSTR_CLR_IF);
}
//1 byte
template <typename T>
void STI(T &receiver) {
    pushByte(receiver, INTEL_INSTR_SET_IF);
}
//1 byte
template <typename T>
void CLD(T &receiver) {
    pushByte(receiver, INTEL_INSTR_CLR_DF);
}
//1 byte
template <typename T>
void STD(T &receiver) {
    pushByte(receiver, INTEL_INSTR_SET_DF);
}
#pragma endregion clr/set flag instructions

#pragma region instatiations
//std::ofstream
template void ADDb(std::ofstream &receiver, const char *reg1, const char *reg2);
template void ADDb(std::ofstream &receiver, const char *reg, const uint8_t &value);
template void ADDv(std::ofstream &receiver, const char *reg1, const char *reg2);
template void ADDv(std::ofstream &receiver, const char *reg, const uint32_t &value);
template void ORb(std::ofstream &receiver, const char *reg1, const char *reg2);
template void ORb(std::ofstream &receiver, const char *reg, const uint8_t &value);
template void ORv(std::ofstream &receiver, const char *reg1, const char *reg2);
template void ORv(std::ofstream &receiver, const char *reg, const uint32_t &value);
template void ADCb(std::ofstream &receiver, const char *reg1, const char *reg2);
template void ADCb(std::ofstream &receiver, const char *reg, const uint8_t &value);
template void ADCv(std::ofstream &receiver, const char *reg1, const char *reg2);
template void ADCv(std::ofstream &receiver, const char *reg, const uint32_t &value);
template void SBBb(std::ofstream &receiver, const char *reg1, const char *reg2);
template void SBBb(std::ofstream &receiver, const char *reg, const uint8_t &value);
template void SBBv(std::ofstream &receiver, const char *reg1, const char *reg2);
template void SBBv(std::ofstream &receiver, const char *reg, const uint32_t &value);
template void ANDb(std::ofstream &receiver, const char *reg1, const char *reg2);
template void ANDb(std::ofstream &receiver, const char *reg, const uint8_t &value);
template void ANDv(std::ofstream &receiver, const char *reg1, const char *reg2);
template void ANDv(std::ofstream &receiver, const char *reg, const uint32_t &value);
template void SUBb(std::ofstream &receiver, const char *reg1, const char *reg2);
template void SUBb(std::ofstream &receiver, const char *reg, const uint8_t &value);
template void SUBv(std::ofstream &receiver, const char *reg1, const char *reg2);
template void SUBv(std::ofstream &receiver, const char *reg, const uint32_t &value);
template void XORb(std::ofstream &receiver, const char *reg1, const char *reg2);
template void XORb(std::ofstream &receiver, const char *reg, const uint8_t &value);
template void XORv(std::ofstream &receiver, const char *reg1, const char *reg2);
template void XORv(std::ofstream &receiver, const char *reg, const uint32_t &value);
template void CMPb(std::ofstream &receiver, const char *reg1, const char *reg2);
template void CMPb(std::ofstream &receiver, const char *reg, const uint8_t &value);
template void CMPv(std::ofstream &receiver, const char *reg1, const char *reg2);
template void CMPv(std::ofstream &receiver, const char *reg, const uint32_t &value);
template void INCb(std::ofstream &receiver, const char *reg);
template void INCv(std::ofstream &receiver, const char *reg);
template void DECb(std::ofstream &receiver, const char *reg);
template void DECv(std::ofstream &receiver, const char *reg);
template void CALL(std::ofstream &receiver, const uint32_t &value);
template void CALL(std::ofstream &receiver, const char *reg);
template void JMPb(std::ofstream &receiver, const uint8_t &value);
template void JMPv(std::ofstream &receiver, const uint32_t &value);
template void JMPv(std::ofstream &receiver, const char *reg);
template void PUSHb(std::ofstream &receiver, const uint8_t &value);
template void PUSHv(std::ofstream &receiver, const uint32_t &value);
template void PUSHv(std::ofstream &receiver, const char *reg);
template void POP(std::ofstream &receiver, const char *reg);
template void JOV(std::ofstream &receiver, const uint8_t &value);
template void JNOV(std::ofstream &receiver, const uint8_t &value);
template void JULT(std::ofstream &receiver, const uint8_t &value);
template void JUGE(std::ofstream &receiver, const uint8_t &value);
template void JET(std::ofstream &receiver, const uint8_t &value);
template void JNE(std::ofstream &receiver, const uint8_t &value);
template void JULE(std::ofstream &receiver, const uint8_t &value);
template void JUGT(std::ofstream &receiver, const uint8_t &value);
template void JS(std::ofstream &receiver, const uint8_t &value);
template void JNS(std::ofstream &receiver, const uint8_t &value);
template void JP(std::ofstream &receiver, const uint8_t &value);
template void JNP(std::ofstream &receiver, const uint8_t &value);
template void JLT(std::ofstream &receiver, const uint8_t &value);
template void JGE(std::ofstream &receiver, const uint8_t &value);
template void JLE(std::ofstream &receiver, const uint8_t &value);
template void JGT(std::ofstream &receiver, const uint8_t &value);
template void JCXZ(std::ofstream &receiver, const uint8_t &value);
template void NOP(std::ofstream &receiver);
template void XCHGb(std::ofstream &receiver, const char *reg1, const char *reg2);
template void XCHGv(std::ofstream &receiver, const char *reg1, const char *reg2);
template void MOVb(std::ofstream &receiver, const char *reg1, const char *reg2);
template void MOVv(std::ofstream &receiver, const char *reg1, const char *reg2);
template void MOVb(std::ofstream &receiver, const char *reg, const uint8_t &value);
template void MOVv(std::ofstream &receiver, const char *reg, const uint32_t &value);
template void RETN(std::ofstream &receiver, const uint16_t &value);
template void RETN(std::ofstream &receiver);
template void RETF(std::ofstream &receiver, const uint16_t &value);
template void RETF(std::ofstream &receiver);
template void INT(std::ofstream &receiver);
template void INT(std::ofstream &receiver, const uint8_t &value);
template void LOOPNE(std::ofstream &receiver, const uint8_t &value);
template void LOOPET(std::ofstream &receiver, const uint8_t &value);
template void LOOP(std::ofstream &receiver, const uint8_t &value);
template void HLT(std::ofstream &receiver);
template void TESTb(std::ofstream &receiver, const char *reg1, const uint8_t &value);
template void TESTv(std::ofstream &receiver, const char *reg1, const uint32_t &value);
template void NOTb(std::ofstream &receiver, const char *reg);
template void NOTv(std::ofstream &receiver, const char *reg);
template void NEGb(std::ofstream &receiver, const char *reg);
template void NEGv(std::ofstream &receiver, const char *reg);
template void CLC(std::ofstream &receiver);
template void STC(std::ofstream &receiver);
template void CLI(std::ofstream &receiver);
template void STI(std::ofstream &receiver);
template void CLD(std::ofstream &receiver);
template void STD(std::ofstream &receiver);

//std::vector<uint8_t>
template void ADDb(std::vector<uint8_t> &receiver, const char *reg1, const char *reg2);
template void ADDb(std::vector<uint8_t> &receiver, const char *reg, const uint8_t &value);
template void ADDv(std::vector<uint8_t> &receiver, const char *reg1, const char *reg2);
template void ADDv(std::vector<uint8_t> &receiver, const char *reg, const uint32_t &value);
template void ORb(std::vector<uint8_t> &receiver, const char *reg1, const char *reg2);
template void ORb(std::vector<uint8_t> &receiver, const char *reg, const uint8_t &value);
template void ORv(std::vector<uint8_t> &receiver, const char *reg1, const char *reg2);
template void ORv(std::vector<uint8_t> &receiver, const char *reg, const uint32_t &value);
template void ADCb(std::vector<uint8_t> &receiver, const char *reg1, const char *reg2);
template void ADCb(std::vector<uint8_t> &receiver, const char *reg, const uint8_t &value);
template void ADCv(std::vector<uint8_t> &receiver, const char *reg1, const char *reg2);
template void ADCv(std::vector<uint8_t> &receiver, const char *reg, const uint32_t &value);
template void SBBb(std::vector<uint8_t> &receiver, const char *reg1, const char *reg2);
template void SBBb(std::vector<uint8_t> &receiver, const char *reg, const uint8_t &value);
template void SBBv(std::vector<uint8_t> &receiver, const char *reg1, const char *reg2);
template void SBBv(std::vector<uint8_t> &receiver, const char *reg, const uint32_t &value);
template void ANDb(std::vector<uint8_t> &receiver, const char *reg1, const char *reg2);
template void ANDb(std::vector<uint8_t> &receiver, const char *reg, const uint8_t &value);
template void ANDv(std::vector<uint8_t> &receiver, const char *reg1, const char *reg2);
template void ANDv(std::vector<uint8_t> &receiver, const char *reg, const uint32_t &value);
template void SUBb(std::vector<uint8_t> &receiver, const char *reg1, const char *reg2);
template void SUBb(std::vector<uint8_t> &receiver, const char *reg, const uint8_t &value);
template void SUBv(std::vector<uint8_t> &receiver, const char *reg1, const char *reg2);
template void SUBv(std::vector<uint8_t> &receiver, const char *reg, const uint32_t &value);
template void XORb(std::vector<uint8_t> &receiver, const char *reg1, const char *reg2);
template void XORb(std::vector<uint8_t> &receiver, const char *reg, const uint8_t &value);
template void XORv(std::vector<uint8_t> &receiver, const char *reg1, const char *reg2);
template void XORv(std::vector<uint8_t> &receiver, const char *reg, const uint32_t &value);
template void CMPb(std::vector<uint8_t> &receiver, const char *reg1, const char *reg2);
template void CMPb(std::vector<uint8_t> &receiver, const char *reg, const uint8_t &value);
template void CMPv(std::vector<uint8_t> &receiver, const char *reg1, const char *reg2);
template void CMPv(std::vector<uint8_t> &receiver, const char *reg, const uint32_t &value);
template void INCb(std::vector<uint8_t> &receiver, const char *reg);
template void INCv(std::vector<uint8_t> &receiver, const char *reg);
template void DECb(std::vector<uint8_t> &receiver, const char *reg);
template void DECv(std::vector<uint8_t> &receiver, const char *reg);
template void CALL(std::vector<uint8_t> &receiver, const uint32_t &value);
template void CALL(std::vector<uint8_t> &receiver, const char *reg);
template void JMPb(std::vector<uint8_t> &receiver, const uint8_t &value);
template void JMPv(std::vector<uint8_t> &receiver, const uint32_t &value);
template void JMPv(std::vector<uint8_t> &receiver, const char *reg);
template void PUSHb(std::vector<uint8_t> &receiver, const uint8_t &value);
template void PUSHv(std::vector<uint8_t> &receiver, const uint32_t &value);
template void PUSHv(std::vector<uint8_t> &receiver, const char *reg);
template void POP(std::vector<uint8_t> &receiver, const char *reg);
template void JOV(std::vector<uint8_t> &receiver, const uint8_t &value);
template void JNOV(std::vector<uint8_t> &receiver, const uint8_t &value);
template void JULT(std::vector<uint8_t> &receiver, const uint8_t &value);
template void JUGE(std::vector<uint8_t> &receiver, const uint8_t &value);
template void JET(std::vector<uint8_t> &receiver, const uint8_t &value);
template void JNE(std::vector<uint8_t> &receiver, const uint8_t &value);
template void JULE(std::vector<uint8_t> &receiver, const uint8_t &value);
template void JUGT(std::vector<uint8_t> &receiver, const uint8_t &value);
template void JS(std::vector<uint8_t> &receiver, const uint8_t &value);
template void JNS(std::vector<uint8_t> &receiver, const uint8_t &value);
template void JP(std::vector<uint8_t> &receiver, const uint8_t &value);
template void JNP(std::vector<uint8_t> &receiver, const uint8_t &value);
template void JLT(std::vector<uint8_t> &receiver, const uint8_t &value);
template void JGE(std::vector<uint8_t> &receiver, const uint8_t &value);
template void JLE(std::vector<uint8_t> &receiver, const uint8_t &value);
template void JGT(std::vector<uint8_t> &receiver, const uint8_t &value);
template void JCXZ(std::vector<uint8_t> &receiver, const uint8_t &value);
template void NOP(std::vector<uint8_t> &receiver);
template void XCHGb(std::vector<uint8_t> &receiver, const char *reg1, const char *reg2);
template void XCHGv(std::vector<uint8_t> &receiver, const char *reg1, const char *reg2);
template void MOVb(std::vector<uint8_t> &receiver, const char *reg1, const char *reg2);
template void MOVv(std::vector<uint8_t> &receiver, const char *reg1, const char *reg2);
template void MOVb(std::vector<uint8_t> &receiver, const char *reg, const uint8_t &value);
template void MOVv(std::vector<uint8_t> &receiver, const char *reg, const uint32_t &value);
template void RETN(std::vector<uint8_t> &receiver, const uint16_t &value);
template void RETN(std::vector<uint8_t> &receiver);
template void RETF(std::vector<uint8_t> &receiver, const uint16_t &value);
template void RETF(std::vector<uint8_t> &receiver);
template void INT(std::vector<uint8_t> &receiver);
template void INT(std::vector<uint8_t> &receiver, const uint8_t &value);
template void LOOPNE(std::vector<uint8_t> &receiver, const uint8_t &value);
template void LOOPET(std::vector<uint8_t> &receiver, const uint8_t &value);
template void LOOP(std::vector<uint8_t> &receiver, const uint8_t &value);
template void HLT(std::vector<uint8_t> &receiver);
template void TESTb(std::vector<uint8_t> &receiver, const char *reg1, const uint8_t &value);
template void TESTv(std::vector<uint8_t> &receiver, const char *reg1, const uint32_t &value);
template void NOTb(std::vector<uint8_t> &receiver, const char *reg);
template void NOTv(std::vector<uint8_t> &receiver, const char *reg);
template void NEGb(std::vector<uint8_t> &receiver, const char *reg);
template void NEGv(std::vector<uint8_t> &receiver, const char *reg);
template void CLC(std::vector<uint8_t> &receiver);
template void STC(std::vector<uint8_t> &receiver);
template void CLI(std::vector<uint8_t> &receiver);
template void STI(std::vector<uint8_t> &receiver);
template void CLD(std::vector<uint8_t> &receiver);
template void STD(std::vector<uint8_t> &receiver);

//ElfSegmentHandler *
template void ADDb(ElfSegmentHandler *&receiver, const char *reg1, const char *reg2);
template void ADDb(ElfSegmentHandler *&receiver, const char *reg, const uint8_t &value);
template void ADDv(ElfSegmentHandler *&receiver, const char *reg1, const char *reg2);
template void ADDv(ElfSegmentHandler *&receiver, const char *reg, const uint32_t &value);
template void ORb(ElfSegmentHandler *&receiver, const char *reg1, const char *reg2);
template void ORb(ElfSegmentHandler *&receiver, const char *reg, const uint8_t &value);
template void ORv(ElfSegmentHandler *&receiver, const char *reg1, const char *reg2);
template void ORv(ElfSegmentHandler *&receiver, const char *reg, const uint32_t &value);
template void ADCb(ElfSegmentHandler *&receiver, const char *reg1, const char *reg2);
template void ADCb(ElfSegmentHandler *&receiver, const char *reg, const uint8_t &value);
template void ADCv(ElfSegmentHandler *&receiver, const char *reg1, const char *reg2);
template void ADCv(ElfSegmentHandler *&receiver, const char *reg, const uint32_t &value);
template void SBBb(ElfSegmentHandler *&receiver, const char *reg1, const char *reg2);
template void SBBb(ElfSegmentHandler *&receiver, const char *reg, const uint8_t &value);
template void SBBv(ElfSegmentHandler *&receiver, const char *reg1, const char *reg2);
template void SBBv(ElfSegmentHandler *&receiver, const char *reg, const uint32_t &value);
template void ANDb(ElfSegmentHandler *&receiver, const char *reg1, const char *reg2);
template void ANDb(ElfSegmentHandler *&receiver, const char *reg, const uint8_t &value);
template void ANDv(ElfSegmentHandler *&receiver, const char *reg1, const char *reg2);
template void ANDv(ElfSegmentHandler *&receiver, const char *reg, const uint32_t &value);
template void SUBb(ElfSegmentHandler *&receiver, const char *reg1, const char *reg2);
template void SUBb(ElfSegmentHandler *&receiver, const char *reg, const uint8_t &value);
template void SUBv(ElfSegmentHandler *&receiver, const char *reg1, const char *reg2);
template void SUBv(ElfSegmentHandler *&receiver, const char *reg, const uint32_t &value);
template void XORb(ElfSegmentHandler *&receiver, const char *reg1, const char *reg2);
template void XORb(ElfSegmentHandler *&receiver, const char *reg, const uint8_t &value);
template void XORv(ElfSegmentHandler *&receiver, const char *reg1, const char *reg2);
template void XORv(ElfSegmentHandler *&receiver, const char *reg, const uint32_t &value);
template void CMPb(ElfSegmentHandler *&receiver, const char *reg1, const char *reg2);
template void CMPb(ElfSegmentHandler *&receiver, const char *reg, const uint8_t &value);
template void CMPv(ElfSegmentHandler *&receiver, const char *reg1, const char *reg2);
template void CMPv(ElfSegmentHandler *&receiver, const char *reg, const uint32_t &value);
template void INCb(ElfSegmentHandler *&receiver, const char *reg);
template void INCv(ElfSegmentHandler *&receiver, const char *reg);
template void DECb(ElfSegmentHandler *&receiver, const char *reg);
template void DECv(ElfSegmentHandler *&receiver, const char *reg);
template void CALL(ElfSegmentHandler *&receiver, const uint32_t &value);
template void CALL(ElfSegmentHandler *&receiver, const char *reg);
template void JMPv(ElfSegmentHandler *&receiver, const uint32_t &value);
template void JMPv(ElfSegmentHandler *&receiver, const char *reg);
template void JMPb(ElfSegmentHandler *&receiver, const uint8_t &value);
template void PUSHb(ElfSegmentHandler *&receiver, const uint8_t &value);
template void PUSHv(ElfSegmentHandler *&receiver, const uint32_t &value);
template void PUSHv(ElfSegmentHandler *&receiver, const char *reg);
template void POP(ElfSegmentHandler *&receiver, const char *reg);
template void JOV(ElfSegmentHandler *&receiver, const uint8_t &value);
template void JNOV(ElfSegmentHandler *&receiver, const uint8_t &value);
template void JULT(ElfSegmentHandler *&receiver, const uint8_t &value);
template void JUGE(ElfSegmentHandler *&receiver, const uint8_t &value);
template void JET(ElfSegmentHandler *&receiver, const uint8_t &value);
template void JNE(ElfSegmentHandler *&receiver, const uint8_t &value);
template void JULE(ElfSegmentHandler *&receiver, const uint8_t &value);
template void JUGT(ElfSegmentHandler *&receiver, const uint8_t &value);
template void JS(ElfSegmentHandler *&receiver, const uint8_t &value);
template void JNS(ElfSegmentHandler *&receiver, const uint8_t &value);
template void JP(ElfSegmentHandler *&receiver, const uint8_t &value);
template void JNP(ElfSegmentHandler *&receiver, const uint8_t &value);
template void JLT(ElfSegmentHandler *&receiver, const uint8_t &value);
template void JGE(ElfSegmentHandler *&receiver, const uint8_t &value);
template void JLE(ElfSegmentHandler *&receiver, const uint8_t &value);
template void JGT(ElfSegmentHandler *&receiver, const uint8_t &value);
template void JCXZ(ElfSegmentHandler *&receiver, const uint8_t &value);
template void NOP(ElfSegmentHandler *&receiver);
template void XCHGb(ElfSegmentHandler *&receiver, const char *reg1, const char *reg2);
template void XCHGv(ElfSegmentHandler *&receiver, const char *reg1, const char *reg2);
template void MOVb(ElfSegmentHandler *&receiver, const char *reg1, const char *reg2);
template void MOVv(ElfSegmentHandler *&receiver, const char *reg1, const char *reg2);
template void MOVb(ElfSegmentHandler *&receiver, const char *reg, const uint8_t &value);
template void MOVv(ElfSegmentHandler *&receiver, const char *reg, const uint32_t &value);
template void RETN(ElfSegmentHandler *&receiver, const uint16_t &value);
template void RETN(ElfSegmentHandler *&receiver);
template void RETF(ElfSegmentHandler *&receiver, const uint16_t &value);
template void RETF(ElfSegmentHandler *&receiver);
template void INT(ElfSegmentHandler *&receiver);
template void INT(ElfSegmentHandler *&receiver, const uint8_t &value);
template void LOOPNE(ElfSegmentHandler *&receiver, const uint8_t &value);
template void LOOPET(ElfSegmentHandler *&receiver, const uint8_t &value);
template void LOOP(ElfSegmentHandler *&receiver, const uint8_t &value);
template void HLT(ElfSegmentHandler *&receiver);
template void TESTb(ElfSegmentHandler *&receiver, const char *reg1, const uint8_t &value);
template void TESTv(ElfSegmentHandler *&receiver, const char *reg1, const uint32_t &value);
template void NOTb(ElfSegmentHandler *&receiver, const char *reg);
template void NOTv(ElfSegmentHandler *&receiver, const char *reg);
template void NEGb(ElfSegmentHandler *&receiver, const char *reg);
template void NEGv(ElfSegmentHandler *&receiver, const char *reg);
template void CLC(ElfSegmentHandler *&receiver);
template void STC(ElfSegmentHandler *&receiver);
template void CLI(ElfSegmentHandler *&receiver);
template void STI(ElfSegmentHandler *&receiver);
template void CLD(ElfSegmentHandler *&receiver);
template void STD(ElfSegmentHandler *&receiver);

//Pe32SectionHandler *
template void ADDb(Pe32SectionHandler *&receiver, const char *reg1, const char *reg2);
template void ADDb(Pe32SectionHandler *&receiver, const char *reg, const uint8_t &value);
template void ADDv(Pe32SectionHandler *&receiver, const char *reg1, const char *reg2);
template void ADDv(Pe32SectionHandler *&receiver, const char *reg, const uint32_t &value);
template void ORb(Pe32SectionHandler *&receiver, const char *reg1, const char *reg2);
template void ORb(Pe32SectionHandler *&receiver, const char *reg, const uint8_t &value);
template void ORv(Pe32SectionHandler *&receiver, const char *reg1, const char *reg2);
template void ORv(Pe32SectionHandler *&receiver, const char *reg, const uint32_t &value);
template void ADCb(Pe32SectionHandler *&receiver, const char *reg1, const char *reg2);
template void ADCb(Pe32SectionHandler *&receiver, const char *reg, const uint8_t &value);
template void ADCv(Pe32SectionHandler *&receiver, const char *reg1, const char *reg2);
template void ADCv(Pe32SectionHandler *&receiver, const char *reg, const uint32_t &value);
template void SBBb(Pe32SectionHandler *&receiver, const char *reg1, const char *reg2);
template void SBBb(Pe32SectionHandler *&receiver, const char *reg, const uint8_t &value);
template void SBBv(Pe32SectionHandler *&receiver, const char *reg1, const char *reg2);
template void SBBv(Pe32SectionHandler *&receiver, const char *reg, const uint32_t &value);
template void ANDb(Pe32SectionHandler *&receiver, const char *reg1, const char *reg2);
template void ANDb(Pe32SectionHandler *&receiver, const char *reg, const uint8_t &value);
template void ANDv(Pe32SectionHandler *&receiver, const char *reg1, const char *reg2);
template void ANDv(Pe32SectionHandler *&receiver, const char *reg, const uint32_t &value);
template void SUBb(Pe32SectionHandler *&receiver, const char *reg1, const char *reg2);
template void SUBb(Pe32SectionHandler *&receiver, const char *reg, const uint8_t &value);
template void SUBv(Pe32SectionHandler *&receiver, const char *reg1, const char *reg2);
template void SUBv(Pe32SectionHandler *&receiver, const char *reg, const uint32_t &value);
template void XORb(Pe32SectionHandler *&receiver, const char *reg1, const char *reg2);
template void XORb(Pe32SectionHandler *&receiver, const char *reg, const uint8_t &value);
template void XORv(Pe32SectionHandler *&receiver, const char *reg1, const char *reg2);
template void XORv(Pe32SectionHandler *&receiver, const char *reg, const uint32_t &value);
template void CMPb(Pe32SectionHandler *&receiver, const char *reg1, const char *reg2);
template void CMPb(Pe32SectionHandler *&receiver, const char *reg, const uint8_t &value);
template void CMPv(Pe32SectionHandler *&receiver, const char *reg1, const char *reg2);
template void CMPv(Pe32SectionHandler *&receiver, const char *reg, const uint32_t &value);
template void INCb(Pe32SectionHandler *&receiver, const char *reg);
template void INCv(Pe32SectionHandler *&receiver, const char *reg);
template void DECb(Pe32SectionHandler *&receiver, const char *reg);
template void DECv(Pe32SectionHandler *&receiver, const char *reg);
template void CALL(Pe32SectionHandler *&receiver, const uint32_t &value);
template void CALL(Pe32SectionHandler *&receiver, const char *reg);
template void JMPv(Pe32SectionHandler *&receiver, const uint32_t &value);
template void JMPv(Pe32SectionHandler *&receiver, const char *reg);
template void JMPb(Pe32SectionHandler *&receiver, const uint8_t &value);
template void PUSHb(Pe32SectionHandler *&receiver, const uint8_t &value);
template void PUSHv(Pe32SectionHandler *&receiver, const uint32_t &value);
template void PUSHv(Pe32SectionHandler *&receiver, const char *reg);
template void POP(Pe32SectionHandler *&receiver, const char *reg);
template void JOV(Pe32SectionHandler *&receiver, const uint8_t &value);
template void JNOV(Pe32SectionHandler *&receiver, const uint8_t &value);
template void JULT(Pe32SectionHandler *&receiver, const uint8_t &value);
template void JUGE(Pe32SectionHandler *&receiver, const uint8_t &value);
template void JET(Pe32SectionHandler *&receiver, const uint8_t &value);
template void JNE(Pe32SectionHandler *&receiver, const uint8_t &value);
template void JULE(Pe32SectionHandler *&receiver, const uint8_t &value);
template void JUGT(Pe32SectionHandler *&receiver, const uint8_t &value);
template void JS(Pe32SectionHandler *&receiver, const uint8_t &value);
template void JNS(Pe32SectionHandler *&receiver, const uint8_t &value);
template void JP(Pe32SectionHandler *&receiver, const uint8_t &value);
template void JNP(Pe32SectionHandler *&receiver, const uint8_t &value);
template void JLT(Pe32SectionHandler *&receiver, const uint8_t &value);
template void JGE(Pe32SectionHandler *&receiver, const uint8_t &value);
template void JLE(Pe32SectionHandler *&receiver, const uint8_t &value);
template void JGT(Pe32SectionHandler *&receiver, const uint8_t &value);
template void JCXZ(Pe32SectionHandler *&receiver, const uint8_t &value);
template void NOP(Pe32SectionHandler *&receiver);
template void XCHGb(Pe32SectionHandler *&receiver, const char *reg1, const char *reg2);
template void XCHGv(Pe32SectionHandler *&receiver, const char *reg1, const char *reg2);
template void MOVb(Pe32SectionHandler *&receiver, const char *reg1, const char *reg2);
template void MOVv(Pe32SectionHandler *&receiver, const char *reg1, const char *reg2);
template void MOVb(Pe32SectionHandler *&receiver, const char *reg, const uint8_t &value);
template void MOVv(Pe32SectionHandler *&receiver, const char *reg, const uint32_t &value);
template void RETN(Pe32SectionHandler *&receiver, const uint16_t &value);
template void RETN(Pe32SectionHandler *&receiver);
template void RETF(Pe32SectionHandler *&receiver, const uint16_t &value);
template void RETF(Pe32SectionHandler *&receiver);
template void INT(Pe32SectionHandler *&receiver);
template void INT(Pe32SectionHandler *&receiver, const uint8_t &value);
template void LOOPNE(Pe32SectionHandler *&receiver, const uint8_t &value);
template void LOOPET(Pe32SectionHandler *&receiver, const uint8_t &value);
template void LOOP(Pe32SectionHandler *&receiver, const uint8_t &value);
template void HLT(Pe32SectionHandler *&receiver);
template void TESTb(Pe32SectionHandler *&receiver, const char *reg1, const uint8_t &value);
template void TESTv(Pe32SectionHandler *&receiver, const char *reg1, const uint32_t &value);
template void NOTb(Pe32SectionHandler *&receiver, const char *reg);
template void NOTv(Pe32SectionHandler *&receiver, const char *reg);
template void NEGb(Pe32SectionHandler *&receiver, const char *reg);
template void NEGv(Pe32SectionHandler *&receiver, const char *reg);
template void CLC(Pe32SectionHandler *&receiver);
template void STC(Pe32SectionHandler *&receiver);
template void CLI(Pe32SectionHandler *&receiver);
template void STI(Pe32SectionHandler *&receiver);
template void CLD(Pe32SectionHandler *&receiver);
template void STD(Pe32SectionHandler *&receiver);
#pragma endregion instatiations

#pragma endregion instructions
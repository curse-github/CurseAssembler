#ifndef _INTEL_CONSTANTS_H
#define _INTEL_CONSTANTS_H

//mod rm values
#define INTEL_ModRM_MOD_Address 0b00000000
#define INTEL_ModRM_MOD_1byteDisplacement 0b01000000
#define INTEL_ModRM_MOD_4byteDisplacement 0b10000000
#define INTEL_ModRM_MOD_Reg 0b11000000

#define INTEL_ModRM_REG_eAX 0b00000000
#define INTEL_ModRM_REG_eCX 0b00001000
#define INTEL_ModRM_REG_eDX 0b00010000
#define INTEL_ModRM_REG_eBX 0b00011000
#define INTEL_ModRM_REG_eSP 0b00100000
#define INTEL_ModRM_REG_eBP 0b00101000
#define INTEL_ModRM_REG_eSI 0b00110000
#define INTEL_ModRM_REG_eDI 0b00111000

#define INTEL_ModRM_RM_eAX 0b00000000
#define INTEL_ModRM_RM_eCX 0b00000001
#define INTEL_ModRM_RM_eDX 0b00000010
#define INTEL_ModRM_RM_eBX 0b00000011
#define INTEL_ModRM_RM_eSP 0b00000100
#define INTEL_ModRM_RM_eBP 0b00000101
#define INTEL_ModRM_RM_eSI 0b00000110
#define INTEL_ModRM_RM_eDI 0b00000111

#define INTEL_ModRM_RM_NoDisplace 0b00000100
#define INTEL_ModRM_RM_DisplaceOnly 0b00000101

// instruction offsets
#define INTEL_REG_OFF_AL 0
#define INTEL_REG_OFF_CL 1
#define INTEL_REG_OFF_DL 2
#define INTEL_REG_OFF_BL 3
#define INTEL_REG_OFF_AH 4
#define INTEL_REG_OFF_CH 5
#define INTEL_REG_OFF_DH 6
#define INTEL_REG_OFF_BH 7

#define INTEL_REG_OFF_eAX 0
#define INTEL_REG_OFF_eCX 1
#define INTEL_REG_OFF_eDX 2
#define INTEL_REG_OFF_eBX 3
#define INTEL_REG_OFF_eSP 4
#define INTEL_REG_OFF_eBP 5
#define INTEL_REG_OFF_eSI 6
#define INTEL_REG_OFF_eDI 7

#define INTEL_INSTR_ADD_eAX_Ib 0x04
#define INTEL_INSTR_ADD_eAX_Iv 0x05
#define INTEL_INSTR_SUB_eAX_Ib 0x2C
#define INTEL_INSTR_SUB_eAX_Iv 0x2D
#define INTEL_INSTR_INC_REG 0x40   // eAX + offset
#define INTEL_INSTR_DEC_REG 0x48   // eAX + offset
#define INTEL_INSTR_PUSH_REG 0x50  // eAX + offset
#define INTEL_INSTR_PUSH_Iv 0x68
#define INTEL_INSTR_POP_REG 0x58  // eAX + offset
#define INTEL_INSTR_ADD_REG_Ib 0x80  // requires modrm byte, rm + Ib
#define INTEL_INSTR_ADD_REG_Iv 0x81  // requires modrm byte, rm + Iv
#define INTEL_INSTR_SUB_REG_Ib 0x82  // requires modrm byte, rm - Ib
#define INTEL_INSTR_SUB_REG_Iv 0x83  // requires modrm byte, rm - Iv
#define INTEL_INSTR_NOP 0x90
#define INTEL_INSTR_MOV_REG_REG 0x89  // requires modrm byte, rm = reg
#define INTEL_INSTR_XCHG_eAX_REG 0x90  // eAX + offset
#define INTEL_INSTR_MOV_REG_eAX_Ov 0xA1
#define INTEL_INSTR_MOV_REG_Ov_eAX 0xA3
#define INTEL_INSTR_MOV_REG_Iv 0xB8       // eAX + offset
#define INTEL_INSTR_MOV_REG_Ib_Low 0xB0   // eAX + offset
#define INTEL_INSTR_MOV_REG_Ib_High 0xB4  // eAX + offset
#define INTEL_INSTR_INT_Ib 0xCD
#define INTEL_INSTR_JMP_Ap 0xEA
#define INTEL_INSTR_JMP_Jb 0xEB
// #define INTEL_INSTR_ 0x00


#endif// _INTEL_CONSTANTS_H
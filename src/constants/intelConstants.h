#define INTEL_ModRM_REG_eAX 0b00000000
#define INTEL_ModRM_REG_eCX 0b00001000
#define INTEL_ModRM_REG_eDX 0b00010000
#define INTEL_ModRM_REG_eBX 0b00011000

#define INTEL_ModRM_Mod_NoDisplace 0b00000100
#define INTEL_ModRM_Mod_DisplaceOnly 0b00000101
#define INTEL_ModRM_Mod_1byteDisplacement 0b01000000
#define INTEL_ModRM_Mod_4byteDisplacement 0b10000000
#define INTEL_ModRM_Mod_Reg 0b11000000

#pragma region instructions

#define INTEL_REG_OFF_A 0
#define INTEL_REG_OFF_C 1
#define INTEL_REG_OFF_D 2
#define INTEL_REG_OFF_B 3

#define INTEL_INSTR_INC_REG 0x40// + reg offset
#define INTEL_INSTR_DEC_REG 0x48// + reg offset
#define INTEL_INSTR_PUSH_REG 0x50// + reg offset
#define INTEL_INSTR_PUSH_Iv 0x68
#define INTEL_INSTR_POP_REG 0x58// + reg offset
#define INTEL_INSTR_NOP 0x90
#define INTEL_INSTR_XCHG_eAX_REG 0x90// + reg offset
#define INTEL_INSTR_MOV_REG_eAX_Ov 0xA1
#define INTEL_INSTR_MOV_REG_Iv 0xB8// + reg offset
#define INTEL_INSTR_MOV_REG_Ib_Low 0xB0// + reg offset
#define INTEL_INSTR_MOV_REG_Ib_High 0xB4// + reg offset
#define INTEL_INSTR_INT_Ib 0xCD
#define INTEL_INSTR_JMP_Ap 0xEA
#define INTEL_INSTR_JMP_Jb 0xEB
//#define INTEL_INSTR_ 0x00

#pragma endregion// instructions
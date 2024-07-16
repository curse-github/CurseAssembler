#ifndef _INTEL_CONSTANTS_H
#define _INTEL_CONSTANTS_H

// mod bits
#define INTEL_ModRM_MOD_Address 0b00000000
#define INTEL_ModRM_MOD_1byteDisplacement 0b01000000
#define INTEL_ModRM_MOD_4byteDisplacement 0b10000000
#define INTEL_ModRM_MOD_Reg 0b11000000

// instruction REG bits
//64 bit
#define INTEL_ModRM_REG_rAX 0b00000000
#define INTEL_ModRM_REG_rCX 0b00001000
#define INTEL_ModRM_REG_rDX 0b00010000
#define INTEL_ModRM_REG_rBX 0b00011000
#define INTEL_ModRM_REG_rSP 0b00100000
#define INTEL_ModRM_REG_rBP 0b00101000
#define INTEL_ModRM_REG_rSI 0b00110000
#define INTEL_ModRM_REG_rDI 0b00111000
//32 bit
#define INTEL_ModRM_REG_eAX 0b00000000
#define INTEL_ModRM_REG_eCX 0b00001000
#define INTEL_ModRM_REG_eDX 0b00010000
#define INTEL_ModRM_REG_eBX 0b00011000
#define INTEL_ModRM_REG_eSP 0b00100000
#define INTEL_ModRM_REG_eBP 0b00101000
#define INTEL_ModRM_REG_eSI 0b00110000
#define INTEL_ModRM_REG_eDI 0b00111000
//16 bit
#define INTEL_ModRM_REG_AX 0b00000000
#define INTEL_ModRM_REG_CX 0b00001000
#define INTEL_ModRM_REG_DX 0b00010000
#define INTEL_ModRM_REG_BX 0b00011000
#define INTEL_ModRM_REG_SP 0b00100000
#define INTEL_ModRM_REG_BP 0b00101000
#define INTEL_ModRM_REG_SI 0b00110000
#define INTEL_ModRM_REG_DI 0b00111000
//8 bit
#define INTEL_ModRM_REG_AL 0b00000000
#define INTEL_ModRM_REG_CL 0b00001000
#define INTEL_ModRM_REG_DL 0b00010000
#define INTEL_ModRM_REG_BL 0b00011000
#define INTEL_ModRM_REG_AH 0b00100000
#define INTEL_ModRM_REG_CH 0b00101000
#define INTEL_ModRM_REG_DH 0b00110000
#define INTEL_ModRM_REG_BH 0b00111000

#pragma region mod R/M opcodes
#define INTEL_ModRM_OP1_ADD_RM_I 0b00000000// 0
#define INTEL_ModRM_OP1_OR_RM_I 0b00001000// 1
#define INTEL_ModRM_OP1_ADC_RM_I 0b00010000// 2
#define INTEL_ModRM_OP1_SBB_RM_I 0b00011000// 3
#define INTEL_ModRM_OP1_AND_RM_I 0b00100000// 4
#define INTEL_ModRM_OP1_SUB_RM_I 0b00101000// 5
#define INTEL_ModRM_OP1_XOR_RM_I 0b00110000// 6
#define INTEL_ModRM_OP1_CMP_RM_I 0b00111000// 7

#define INTEL_ModRM_OP2_TEST_RM_I 0b00000000// 2
#define INTEL_ModRM_OP2_NOT_RM 0b00010000// 2 lockable
#define INTEL_ModRM_OP2_NEG_RM 0b00011000// 3 lockable
//#define INTEL_ModRM_OP2_UMUL 0b00100000// 4
//#define INTEL_ModRM_OP2_MUL 0b00101000// 5
//#define INTEL_ModRM_OP2_UDIV 0b00110000// 6
//#define INTEL_ModRM_OP2_DIV 0b00111000// 7

#define INTEL_ModRM_OP3_INC_RM 0b00000000// 0
#define INTEL_ModRM_OP3_DEC_RM 0b00001000// 1
#define INTEL_ModRM_OP3_CALL_RM 0b00010000// 2
#define INTEL_ModRM_OP3_CALLF_m 0b00011000// 3
#define INTEL_ModRM_OP3_JMP_RM 0b00100000// 4
#define INTEL_ModRM_OP3_JMPF_m 0b00101000// 5
#define INTEL_ModRM_OP3_PUSH_RM 0b00110000// 6
#pragma endregion region mod R/M opcodes

// instruction RM bits
//32 bit
#define INTEL_ModRM_RM_eAX 0b00000000
#define INTEL_ModRM_RM_eCX 0b00000001
#define INTEL_ModRM_RM_eDX 0b00000010
#define INTEL_ModRM_RM_eBX 0b00000011
#define INTEL_ModRM_RM_eSP 0b00000100
#define INTEL_ModRM_RM_eBP 0b00000101
#define INTEL_ModRM_RM_eSI 0b00000110
#define INTEL_ModRM_RM_eDI 0b00000111
//16 bit
#define INTEL_ModRM_RM_AX 0b00000000
#define INTEL_ModRM_RM_CX 0b00000001
#define INTEL_ModRM_RM_DX 0b00000010
#define INTEL_ModRM_RM_BX 0b00000011
#define INTEL_ModRM_RM_SP 0b00000100
#define INTEL_ModRM_RM_BP 0b00000101
#define INTEL_ModRM_RM_SI 0b00000110
#define INTEL_ModRM_RM_DI 0b00000111
//8 bit
#define INTEL_ModRM_RM_AL 0b00000000
#define INTEL_ModRM_RM_CL 0b00000001
#define INTEL_ModRM_RM_DL 0b00000010
#define INTEL_ModRM_RM_BL 0b00000011
#define INTEL_ModRM_RM_AH 0b00000100
#define INTEL_ModRM_RM_CH 0b00000101
#define INTEL_ModRM_RM_DH 0b00000110
#define INTEL_ModRM_RM_BH 0b00000111

#define INTEL_ModRM_RM_NoDisplace 0b00000100
#define INTEL_ModRM_RM_DisplaceOnly 0b00000101

// instruction offsets
//64 bit
#define INTEL_REG_OFF_rAX 0
#define INTEL_REG_OFF_rCX 1
#define INTEL_REG_OFF_rDX 2
#define INTEL_REG_OFF_rBX 3
#define INTEL_REG_OFF_rSP 4
#define INTEL_REG_OFF_rBP 5
#define INTEL_REG_OFF_rSI 6
#define INTEL_REG_OFF_rDI 7
//32 bit
#define INTEL_REG_OFF_eAX 0
#define INTEL_REG_OFF_eCX 1
#define INTEL_REG_OFF_eDX 2
#define INTEL_REG_OFF_eBX 3
#define INTEL_REG_OFF_eSP 4
#define INTEL_REG_OFF_eBP 5
#define INTEL_REG_OFF_eSI 6
#define INTEL_REG_OFF_eDI 7
//16 bit
#define INTEL_REG_OFF_AX 0
#define INTEL_REG_OFF_CX 1
#define INTEL_REG_OFF_DX 2
#define INTEL_REG_OFF_BX 3
#define INTEL_REG_OFF_SP 4
#define INTEL_REG_OFF_BP 5
#define INTEL_REG_OFF_SI 6
#define INTEL_REG_OFF_DI 7
//8 bit
#define INTEL_REG_OFF_AL 0
#define INTEL_REG_OFF_CL 1
#define INTEL_REG_OFF_DL 2
#define INTEL_REG_OFF_BL 3
#define INTEL_REG_OFF_AH 4
#define INTEL_REG_OFF_CH 5
#define INTEL_REG_OFF_DH 6
#define INTEL_REG_OFF_BH 7


#define INTEL_INSTR_ADD_RMb_REGb 0x00// lockable
#define INTEL_INSTR_ADD_RMv_REGv 0x01// lockable
#define INTEL_INSTR_ADD_REGb_RMb 0x02
#define INTEL_INSTR_ADD_REGv_RMv 0x03
#define INTEL_INSTR_ADD_AL_Ib 0x04
#define INTEL_INSTR_ADD_eAX_Iv 0x05
//#define INTEL_INSTR_PUSH_Erg 0x06
//#define INTEL_INSTR_POP_Erg 0x07
#define INTEL_INSTR_OR_RMb_REGb 0x08// lockable
#define INTEL_INSTR_OR_RMv_REGv 0x09// lockable
#define INTEL_INSTR_OR_REGb_RMb 0x0A
#define INTEL_INSTR_OR_REGv_RMv 0x0B
#define INTEL_INSTR_OR_AL_Ib 0x0C
#define INTEL_INSTR_OR_eAX_Iv 0x0D
//#define INTEL_INSTR_PUSH_Crg 0x0E
//#define INTEL_INSTR_ 0x0F
#define INTEL_INSTR_ADC_RMb_REGb 0x10// lockable
#define INTEL_INSTR_ADC_RMv_REGv 0x11// lockable
#define INTEL_INSTR_ADC_REGb_RMb 0x12
#define INTEL_INSTR_ADC_REGv_RMv 0x13
#define INTEL_INSTR_ADC_AL_Ib 0x14
#define INTEL_INSTR_ADC_eAX_Iv 0x15
//#define INTEL_INSTR_PUSH_Srg 0x16
//#define INTEL_INSTR_POP_Srg 0x17
#define INTEL_INSTR_SBB_RMb_REGb 0x18// lockable
#define INTEL_INSTR_SBB_RMv_REGv 0x19// lockable
#define INTEL_INSTR_SBB_REGb_RMb 0x1A
#define INTEL_INSTR_SBB_REGv_RMv 0x1B
#define INTEL_INSTR_SBB_AL_Ib 0x1C
#define INTEL_INSTR_SBB_eAX_Iv 0x1D
//#define INTEL_INSTR_PUSH_Drg 0x1E
//#define INTEL_INSTR_PUSH_Drg 0x1F
#define INTEL_INSTR_AND_RMb_REGb 0x20// lockable
#define INTEL_INSTR_AND_RMv_REGv 0x21// lockable
#define INTEL_INSTR_AND_REGb_RMb 0x22
#define INTEL_INSTR_AND_REGv_RMv 0x23
#define INTEL_INSTR_AND_AL_Ib 0x24
#define INTEL_INSTR_AND_eAX_Iv 0x25
#define INTEL_INSTR_ES_OVRD 0x26
//#define INTEL_INSTR_ 0x27
#define INTEL_INSTR_SUB_RMb_REGb 0x28// lockable
#define INTEL_INSTR_SUB_RMv_REGv 0x29// lockable
#define INTEL_INSTR_SUB_REGb_RMb 0x2A
#define INTEL_INSTR_SUB_REGv_RMv 0x2B
#define INTEL_INSTR_SUB_AL_Ib 0x2C
#define INTEL_INSTR_SUB_eAX_Iv 0x2D
#define INTEL_INSTR_CS_OVRD 0x2E
//#define INTEL_INSTR_ 0x2F
#define INTEL_INSTR_XOR_RMb_REGb 0x30// lockable
#define INTEL_INSTR_XOR_RMv_REGv 0x31// lockable
#define INTEL_INSTR_XOR_REGb_RMb 0x32
#define INTEL_INSTR_XOR_REGv_RMv 0x33
#define INTEL_INSTR_XOR_AL_Ib 0x34
#define INTEL_INSTR_XOR_eAX_Iv 0x35
#define INTEL_INSTR_SS_OVRD 0x36
//#define INTEL_INSTR_ 0x37
#define INTEL_INSTR_CMP_RMb_REGb 0x38
#define INTEL_INSTR_CMP_RMv_REGv 0x39
#define INTEL_INSTR_CMP_REGb_RMv 0x3A
#define INTEL_INSTR_CMP_REGv_RMv 0x3B
#define INTEL_INSTR_CMP_AL_Ib 0x3C
#define INTEL_INSTR_CMP_eAX_Iv 0x3D
#define INTEL_INSTR_DS_OVRD 0x3E
//#define INTEL_INSTR_ 0x3F
#define INTEL_INSTR32_INCpRv 0x40
//0x41-0x47 are offsets of INC+r
#define INTEL_INSTR32_DECpRv 0x48
//0x49-0x4F are offsets of DEC+r
#define INTEL_INSTR_PUSHpRv 0x50
//0x51-0x57 are offsets of PUSH+r
#define INTEL_INSTR_POPpRv 0x58
//0x59-0x5F are offsets of POP+r
//#define INTEL_INSTR_ 0x60
//#define INTEL_INSTR_ 0x61
//#define INTEL_INSTR_ 0x62
//#define INTEL_INSTR_ 0x63
#define INTEL_INSTR_FS_OVRD 0x64
#define INTEL_INSTR_GS_OVRD 0x65
#define INTEL_INSTR_OperandSz_OVRD 0x66
#define INTEL_INSTR_AddrSz_OVRD 0x67
#define INTEL_INSTR_PUSH_Iv 0x68
//#define INTEL_INSTR_ 0x69
#define INTEL_INSTR_PUSH_Ib 0x6A
//#define INTEL_INSTR_ 0x6B
//0x6C - 0x6F IO functions?
#define INTEL_INSTR_JIF_Ov 0x70
#define INTEL_INSTR_JIF_NOv 0x71
#define INTEL_INSTR_JIF_uLT 0x72// unsigned less than
#define INTEL_INSTR_JIF_uGE 0x73// unsigned geater than or equal
#define INTEL_INSTR_JIF_ET 0x74
#define INTEL_INSTR_JIF_NE 0x75
#define INTEL_INSTR_JIF_uLE 0x76// unsigned less than or equal
#define INTEL_INSTR_JIF_uGT 0x77// unsigned greater than
#define INTEL_INSTR_JIF_Sign 0x78
#define INTEL_INSTR_JIF_NSign 0x79
#define INTEL_INSTR_JIF_Parity 0x7A
#define INTEL_INSTR_JIF_NParity 0x7B
#define INTEL_INSTR_JIF_LT 0x7C// less than
#define INTEL_INSTR_JIF_GE 0x7D// geater than or equal
#define INTEL_INSTR_JIF_LE 0x7E// less than or equal
#define INTEL_INSTR_JIF_GT 0x7F// greater than
#define INTEL_INSTR_OP1_RMb_Ib 0x80
#define INTEL_INSTR_OP1_RMv_Iv 0x81
//#define INTEL_INSTR_OP1_RMb_Ib 0x82
#define INTEL_INSTR_OP1_RMv_Ib 0x83
//#define INTEL_INSTR_ 0x84
//#define INTEL_INSTR_ 0x85
#define INTEL_INSTR_XCHG_REGb_RMb 0x86// lockable
#define INTEL_INSTR_XCHG_REGv_RMv 0x87// lockable
#define INTEL_INSTR_MOV_RMb_REGb 0x88
#define INTEL_INSTR_MOV_RMv_REGv 0x89
#define INTEL_INSTR_MOV_REGb_RMb 0x8A
#define INTEL_INSTR_MOV_REGv_RMv 0x8B
//#define INTEL_INSTR_ 0x8C
//#define INTEL_INSTR_ 0x8D
//#define INTEL_INSTR_ 0x8E
#define INTEL_INSTR_POP_RMv 0x8F
#define INTEL_INSTR_NOP 0x90
#define INTEL_INSTR_XCHG_eAX_REGpRv 0x90
///0x91-0x97 are offsets of XCHG eAX REG+r
//#define INTEL_INSTR_ 0x98
//#define INTEL_INSTR_ 0x99
#define INTEL_INSTR32_CALLF_Av 0x9A// call far
//#define INTEL_INSTR_ 0x9B
// 0x9C - 0x9F push and pop flag instructions
//0xA0 - 0xA3 are moffs MOV instructions
// 0xA4 - 0xAF are string instructions
#define INTEL_INSTR_MOV_REGpRb_Ib 0xB0
///0xB1-0xB7 are offsets of MOV REG+r Ib
#define INTEL_INSTR_MOV_REGpRv_Iv 0xB8
///0xB1-0xB7 are offsets of MOV REG+r Iv
//#define INTEL_INSTR_ 0xC1
#define INTEL_INSTR_RETN_I16 0xC2
#define INTEL_INSTR_RETN 0xC3
//#define INTEL_INSTR_ 0xC4
//#define INTEL_INSTR_ 0xC5
#define INTEL_INSTR_MOV_RMb_Ib 0xC6
#define INTEL_INSTR_MOV_RMv_Iv 0xC7
//#define INTEL_INSTR_ 0xC8// enter
//#define INTEL_INSTR_ 0xC9// leave
#define INTEL_INSTR_RETF_I16 0xCA
#define INTEL_INSTR_RETF 0xCB
#define INTEL_INSTR_INT 0xCC
#define INTEL_INSTR_INT_Ib 0xCD
//#define INTEL_INSTR_ 0xCE
//#define INTEL_INSTR_ 0xCF
// 0xD0 - 0xD3 are matrix instructions?
//#define INTEL_INSTR_ 0xD4
//#define INTEL_INSTR_ 0xD5
// 0xD4 and 0xD5 are adjust instructions
//#define INTEL_INSTR_ 0xD6
//#define INTEL_INSTR_ 0xD7
/*#pragma region float instructions
#define INTEL_INSTR_ 0xD8// float instructions
    0: ADD,
    1: MULL,
    2: CMP,
    3: CMP and pop,
    4: SUB,
    5: SUBR,
    6: DIV,
    7: DIVR
#define INTEL_INSTR_ 0xD9// float instructions
    0: load,
    1: XCHG,
    2: store,
    3: store and pop,
    4: load env,
        E0: change sign,
        E1: abs,
        E4: test,
        E5: examine,
    5: load control word,
        E8: load +1.0
        E9: load log2(10)
        EA: load log2(e)
        EB: load pi
        EC: load log10(2)
        EF: load ln(2)
        EE: load +0.0
    6: store env,
        F0: compute 2^x+1
        F1: compute y*log2(x) and pop
        F2: partial tan
        F3: partial arctan and pop
        F4: extract operand and significand
        F5: IEEE partial remainder
    7: store control word
        F8: partial remainder
        F9: compute y*log2(x+1) and pop
        FA: compute sqrt(x)
        FB: compute sin/cos (x)
        FC: compute round(x)
        FD: compute scale(x,y)
        FE: compute sin(x)
        FF: compute cos(x)
#define INTEL_INSTR_ 0xDA// float
    0: IADD
    1: IMUL
    2: ICMP
    3: ICMP and pop
    4: ISUB
    5: ISUBR
    6: IDIV
    7: IDIVR
#define INTEL_INSTR_ 0xDB// float 
#define INTEL_INSTR_ 0xDC// float
#define INTEL_INSTR_ 0xDD// float 
#define INTEL_INSTR_ 0xDE// float 
#define INTEL_INSTR_ 0xDF// float 
#pragma endregion float instructions*/
#define INTEL_INSTR_LOOPn0_eCX_Jb 0xE0
#define INTEL_INSTR_LOOP0_eCX_Jb 0xE1
#define INTEL_INSTR_LOOP_eCX_Jb 0xE2
#define INTEL_INSTR_JMP_eCXeq0_Jb 0xE3
//0xE4 - 0xE7 are input/output instructions
#define INTEL_INSTR_CALL_Jv 0xE8// call rel
#define INTEL_INSTR_JMP_Jv 0xE9//  jump rel
#define INTEL_INSTR_JMPF_Av 0xEA// jump far
#define INTEL_INSTR_JMP_Jb 0xEB//  jump rel near
//0xEC - 0xEF are input/output instructions
//#define INTEL_INSTR_LOCK 0xF0
//#define INTEL_INSTR_ 0xF1
// 0xF2 and 0xF3 are flags
#define INTEL_INSTR_HLT 0xF4
//#define INTEL_INSTR_CMC 0xF5
#define INTEL_INSTR_OP2b 0xF6
#define INTEL_INSTR_OP2v 0xF7
#define INTEL_INSTR_CLR_CF 0xF8
#define INTEL_INSTR_SET_CF 0xF9
#define INTEL_INSTR_CLR_IF 0xFA
#define INTEL_INSTR_SET_IF 0xFB
#define INTEL_INSTR_CLR_DF 0xFC
#define INTEL_INSTR_SET_DF 0xFD
#define INTEL_INSTR_OP3b 0xFE
#define INTEL_INSTR_OP3v 0xFF
// #define INTEL_INSTR_ 0x00


#endif// _INTEL_CONSTANTS_H
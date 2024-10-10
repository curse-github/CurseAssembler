#ifndef _ELF_CONSTANTS_H
#define _ELF_CONSTANTS_H

#define ELF_ARCHITECTURE_INV 0x00u
#define ELF_ARCHITECTURE_32 0x01u
#define ELF_ARCHITECTURE_64 0x02u

#define ELF_ENCODING_INV 0x00u
#define ELF_ENCODING_LSB 0x01u
#define ELF_ENCODING_MSB 0x02u

#define ELF_TYPE_INV 0x0000u
#define ELF_TYPE_REL 0x0001u
#define ELF_TYPE_OBJ 0x0001u
#define ELF_TYPE_EXE 0x0002u
#define ELF_TYPE_SO 0x0003u
#define ELF_TYPE_DLL 0x0003u
#define ELF_TYPE_CORE 0x0004u

#define ELF_OS_INV 0x00u
#define ELF_OS_NONE 0x00u
#define ELF_OS_HPUX 0x01u
#define ELF_OS_NETBSD 0x02u
#define ELF_OS_LINUX 0x03u

#define ELF_MACHINE_INV 0x0000u
#define ELF_MACHINE_NONE 0x0000u
#define ELF_MACHINE_x86 0x0003u
#define ELF_MACHINE_MIPS 0x0008u
#define ELF_MACHINE_ARM 0x0028u
#define ELF_MACHINE_AMD64 0x003eu
#define ELF_MACHINE_ARM8 0x00b7u
#define ELF_MACHINE_RISCV 0x00f3u

#define ELF_SEGMENT_TYPE_INV 0x00000000u
#define ELF_SEGMENT_TYPE_LOAD 0x00000001u
#define ELF_SEGMENT_TYPE_DYN 0x00000002u
#define ELF_SEGMENT_TYPE_INTP 0x00000003u
#define ELF_SEGMENT_TYPE_NOTE 0x00000004u
#define ELF_SEGMENT_TYPE_SHLIB 0x00000005u  // should not be used.
#define ELF_SEGMENT_TYPE_PHDR 0x00000006u
#define ELF_SEGMENT_TYPE_TLS 0x00000007u

#define ELF_SEGMENT_FLAG_EXECUTE 0x0000001u
#define ELF_SEGMENT_FLAG_WRITE 0x0000002u
#define ELF_SEGMENT_FLAG_READ 0x0000004u

#define ELF_SECTION_TYPE_NULL 0x0000000u
#define ELF_SECTION_TYPE_PROGBITS 0x0000001u
#define ELF_SECTION_TYPE_SYMTAB 0x0000002u
#define ELF_SECTION_TYPE_STRTAB 0x0000003u
#define ELF_SECTION_TYPE_RELA 0x0000004u
#define ELF_SECTION_TYPE_HASH 0x0000005u
#define ELF_SECTION_TYPE_DYNAMIC 0x0000006u
#define ELF_SECTION_TYPE_NOTE 0x0000007u
#define ELF_SECTION_TYPE_NOBITS 0x0000008u
#define ELF_SECTION_TYPE_REL 0x0000009u
//#define ELF_SECTION_TYPE_SHLIB 0x000000Au
#define ELF_SECTION_TYPE_DYNSYM 0x000000Bu
#define ELF_SECTION_TYPE_INIT_ARRAY 0x000000Eu
#define ELF_SECTION_TYPE_FINI_ARRAY 0x000000Fu
#define ELF_SECTION_TYPE_PREINIT_ARRAY 0x0000010u
#define ELF_SECTION_TYPE_GROUP 0x0000011u
#define ELF_SECTION_TYPE_SYMTAB_SHNDX 0x0000012u
#define ELF_SECTION_TYPE_NUM 0x0000013u

#define ELF_SECTION_FLAG_WRITE 0x0000001u
#define ELF_SECTION_FLAG_ALLOC 0x0000002u
#define ELF_SECTION_FLAG_EXEC 0x0000004u
#define ELF_SECTION_FLAG_MERGE 0x0000010u
#define ELF_SECTION_FLAG_STRINGS 0x0000020u
#define ELF_SECTION_FLAG_INFO_LINK 0x0000040u
#define ELF_SECTION_FLAG_LINK_ORDER 0x0000080u
#define ELF_SECTION_FLAG_GROUP 0x0000200u
#define ELF_SECTION_FLAG_TLS 0x0000400u

#define ELF_SYMBOL_TYPE_NOTYPE 0x00u
#define ELF_SYMBOL_TYPE_OBJECT 0x01u
#define ELF_SYMBOL_TYPE_FUNC 0x02u
#define ELF_SYMBOL_TYPE_SECTION 0x03u// symbol points to section
#define ELF_SYMBOL_TYPE_FILE 0x04u// refers to file
#define ELF_SYMBOL_TYPE_COMMON 0x05u
#define ELF_SYMBOL_TYPE_TLS 0x06u
//#define ELF_SYMBOL_TYPE_GNU_IFUNC 0x000000au

#define ELF_SYMBOL_BINDING_LOCAL 0x00u
#define ELF_SYMBOL_BINDING_GLOBAL 0x01u
#define ELF_SYMBOL_BINDING_WEAK 0x02u
//#define ELF_SYMBOL_BINDING_GNU_UNIQUE 0x0au

// https://docs.oracle.com/cd/E23824_01/html/819-0690/chapter6-42444.html
#define ELF_DYN_TAG_NULL            0x0000000000000000u// none
#define ELF_DYN_TAG_NEEDED          0x0000000000000001u// val
#define ELF_DYN_TAG_PLTRELSZ        0x0000000000000002u// val
#define ELF_DYN_TAG_PLTGOT          0x0000000000000003u// ptr to global offset table
#define ELF_DYN_TAG_HASH            0x0000000000000004u// ptr
#define ELF_DYN_TAG_STRTAB          0x0000000000000005u// ptr to string table
#define ELF_DYN_TAG_SYMTAB          0x0000000000000006u// ptr to symbol table
#define ELF_DYN_TAG_RELA            0x0000000000000007u// ptr
#define ELF_DYN_TAG_RELASZ          0x0000000000000008u// val
#define ELF_DYN_TAG_RELAENT         0x0000000000000009u// val
#define ELF_DYN_TAG_STRSZ           0x000000000000000au// val
#define ELF_DYN_TAG_SYMENT          0x000000000000000bu// val
//#define ELF_DYN_TAG_INIT            0x000000000000000cu
//#define ELF_DYN_TAG_FINI            0x000000000000000du
//#define ELF_DYN_TAG_SONAME          0x000000000000000eu
#define ELF_DYN_TAG_RPATH           0x000000000000000fu// ptr to string
#define ELF_DYN_TAG_SYMBOLIC        0x0000000000000010u// none
#define ELF_DYN_TAG_REL             0x0000000000000011u// ptr to relocation table
#define ELF_DYN_TAG_RELSZ           0x0000000000000012u// val
#define ELF_DYN_TAG_RELENT          0x0000000000000013u// val
#define ELF_DYN_TAG_PLTREL          0x0000000000000014u// val
#define ELF_DYN_TAG_DEBUG           0x0000000000000015u// ptr
#define ELF_DYN_TAG_TEXTREL         0x0000000000000016u// none
#define ELF_DYN_TAG_JMPREL          0x0000000000000017u// ptr, requires the presence of the PLTRELSZ and PLTREL elements
//#define ELF_DYN_TAG_BIND_NOW        0x0000000000000018u
//#define ELF_DYN_TAG_INIT_ARRAY      0x0000000000000019u
//#define ELF_DYN_TAG_FINI_ARRAY      0x000000000000001au
//#define ELF_DYN_TAG_INIT_ARRAYSZ    0x000000000000001bu
//#define ELF_DYN_TAG_FINI_ARRAYSZ    0x000000000000001cu
#define ELF_DYN_TAG_RUNPATH         0x000000000000001du// ptr to string
#define ELF_DYN_TAG_FLAGS           0x000000000000001eu// val
#define ELF_DYN_TAG_ENCODING        0x000000000000001fu// ?
//#define ELF_DYN_TAG_PREINIT_ARRAY   0x0000000000000020u
//#define ELF_DYN_TAG_PREINIT_ARRAYSZ 0x0000000000000021u
//#define ELF_DYN_TAG_MAXPOSTAGS      0x0000000000000022u
// 0x6000000d - 0x6000001f are operating system tags
#define ELF_DYN_TAG_DT_VALRNGLO     0x000000006ffffd00u// ?, value range low
#define ELF_DYN_TAG_CHECKSUM        0x000000006ffffdf8u// val
#define ELF_DYN_TAG_PLTPADSZ        0x000000006ffffdf9u// val
#define ELF_DYN_TAG_MOVEENT         0x000000006ffffdfau// val
#define ELF_DYN_TAG_MOVESZ          0x000000006ffffdfbu// val
//#define ELF_DYN_TAG_POSFLAG_1       0x000000006ffffdfdu
#define ELF_DYN_TAG_SYMINSZ         0x000000006ffffdfeu// val, symbol info size
#define ELF_DYN_TAG_SYMINENT        0x000000006ffffdffu// val, symbol info entry
#define ELF_DYN_TAG_VALRNGHI        0x000000006ffffdffu// ?, value range high
#define ELF_DYN_TAG_ADDRRNGLO       0x000000006ffffe00u// ?, address range low
#define ELF_DYN_TAG_CONFIG          0x000000006ffffefau// ptr
#define ELF_DYN_TAG_DEPAUDIT        0x000000006ffffefbu// ptr
#define ELF_DYN_TAG_AUDIT           0x000000006ffffefcu// ptr
#define ELF_DYN_TAG_PLTPAD          0x000000006ffffefdu// ptr
#define ELF_DYN_TAG_MOVETAB         0x000000006ffffefeu// ptr
#define ELF_DYN_TAG_SYMINFO         0x000000006ffffeffu// ptr
#define ELF_DYN_TAG_ADDRRNGHI       0x000000006ffffeffu// ?, address range high
#define ELF_DYN_TAG_RELACOUNT       0x000000006ffffff9u// val
#define ELF_DYN_TAG_RELCOUNT        0x000000006ffffffau// val
#define ELF_DYN_TAG_FLAGS_1         0x000000006ffffffbu// val
#define ELF_DYN_TAG_VERDEF          0x000000006ffffffcu// ptr
#define ELF_DYN_TAG_VERDEFNUM       0x000000006ffffffdu// val
#define ELF_DYN_TAG_VERNEED         0x000000006ffffffeu// ptr
#define ELF_DYN_TAG_VERNEEDNUM      0x000000006fffffffu// val

#define ELF_DYN_F_ORIGIN     0x0000000000000001
#define ELF_DYN_F_SYMBOLIC   0x0000000000000002
#define ELF_DYN_F_TEXTREL    0x0000000000000004
#define ELF_DYN_F_BIND_NOW   0x0000000000000008
#define ELF_DYN_F_STATIC_TLS 0x0000000000000010

#define ELF_DYN_F1_NOW      0x0000000000000001
#define ELF_DYN_F1_NODELETE 0x0000000000000008
#define ELF_DYN_F1_NODEFLIB 0x0000000000000800
#define ELF_DYN_F1_EDITED   0x0000000000200000
#define ELF_DYN_F1_SYMINTPOS 0x800000

//https://docs.oracle.com/cd/E23824_01/html/819-0690/chapter6-54839.html#scrolltoc
#define ELF64_REL_SYM(info)       ((info)>>32)
#define ELF64_REL_TYPE(info)      ((uint32_t)(info))
#define ELF64_REL_INFO(sym, type) (((uint64_t)(sym)<<32)+ (uint64_t)(type))
// A: r_addend from elf64Rela type
// B: base address
// G: the offset into the global offset table of the address for the given symbol
// GOT: the offset of the global offset table
// L: the section offset/address of the PLT entr for the given symbol
// P: section offset/address of storage unit? relates to r_offset in elf64Rela or elf64Rel structs
// S: value of the given symbol
// Z: size of the given symbol
#define ELF64_REL_TYPE_AMD64_NONE      0x00000000
#define ELF64_REL_TYPE_AMD64_64        0x00000001// S + A
#define ELF64_REL_TYPE_AMD64_PC32      0x00000002// S + A - P
#define ELF64_REL_TYPE_AMD64_GOT32     0x00000003// G + A
#define ELF64_REL_TYPE_AMD64_PLT32     0x00000004// L + A - P
//#define ELF64_REL_TYPE_AMD64_COPY      0x00000005// some special thing
#define ELF64_REL_TYPE_AMD64_GLOB_DAT  0x00000006// S
#define ELF64_REL_TYPE_AMD64_JUMP_SLOT 0x00000007// S
#define ELF64_REL_TYPE_AMD64_RELATIVE  0x00000008// B + A
//#define ELF64_REL_TYPE_AMD64_GOTPCREL  0x00000009// G + GOT + A - P, plus some weird nuance, idk
#define ELF64_REL_TYPE_AMD64_32        0x0000000a// S + A    , gets truncated to 32 bits
#define ELF64_REL_TYPE_AMD64_32s       0x0000000b// S + A    , gets truncated to 32 bits
//#define ELF64_REL_TYPE_AMD64_16        0x0000000c// S + A    , shouldnt be used, in theory would be truncated to 16 bits
//#define ELF64_REL_TYPE_AMD64_PC16      0x0000000d// S + A - P, shouldnt be used, in theory would be truncated to 16 bits
//#define ELF64_REL_TYPE_AMD64_8         0x0000000e// S + A    , shouldnt be used, in theory would be truncated to 8 bits
//#define ELF64_REL_TYPE_AMD64_PC8       0x0000000f// S + A - P, shouldnt be used, in theory would be truncated to 8 bits
#define ELF64_REL_TYPE_AMD64_PC64      0x00000018// S + A - P
#define ELF64_REL_TYPE_AMD64_GOTOFF64  0x00000019// S + A + GOT
#define ELF64_REL_TYPE_AMD64_GOTPC32   0x0000001a// GOT + A + P
#define ELF64_REL_TYPE_AMD64_SIZE32    0x00000020// Z + A
#define ELF64_REL_TYPE_AMD64_SIZE64    0x00000021// Z + A


#define LINUX_SYSCALL_EXIT 0x00000001u
#define LINUX_SYSCALL_FORK 0x00000002u
#define LINUX_SYSCALL_READ 0x00000003u
#define LINUX_SYSCALL_WRITE 0x00000004u
#define LINUX_SYSCALL_OPEN 0x00000005u
#define LINUX_SYSCALL_CLOSE 0x00000006u
//exit: ebx holds exit code
// fork: ebx holds pt_regs
// read: ebx holds uint, ecx holds char *, edx holds size
// write: ebx holds uint, ecx holds const char *, edx holds size
// open: ebx holds const char *, ecx holds int, edx holds int
// close: ebx holds uint
#endif// _ELF_CONSTANTS_H
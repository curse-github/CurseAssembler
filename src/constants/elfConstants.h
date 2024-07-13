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
#define ELF_SEGMENT_TYPE_NT 0x00000004u
#define ELF_SEGMENT_TYPE_SHLIB 0x00000005u  // should not be used.
#define ELF_SEGMENT_TYPE_PHDR 0x00000006u
#define ELF_SEGMENT_TYPE_TLS 0x00000007u

#define ELF_SEGMENT_FLAG_EXECUTE 0x0000001u
#define ELF_SEGMENT_FLAG_WRITE 0x0000002u
#define ELF_SEGMENT_FLAG_READ 0x0000004u

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
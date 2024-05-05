#ifndef _PE_CONSTANTS_H
#define _PE_CONSTANTS_H

#define IMAGE_FILE_MAGIC32 0x010b
#define IMAGE_FILE_MAGIC64 0x020b

// learn.microsoft.com/en-us/windows/win32/debug/pe-format#machine-types
#define IMAGE_FILE_MACHINE_UNKNOWN 0x0  // usually assumed to apply to any platform
#define IMAGE_FILE_MACHINE_AMD64 0x8664
#define IMAGE_FILE_MACHINE_I386 0x014c   // Intel 386 or later processors and compatible processors
#define IMAGE_FILE_MACHINE_ARM 0x1c0     // little endian
#define IMAGE_FILE_MACHINE_ARM64 0xaa64  // little endian
#define IMAGE_FILE_MACHINE_RISKV32 0x5032
#define IMAGE_FILE_MACHINE_RISKV64 0x5064
#define IMAGE_FILE_MACHINE_RISKV128 0x5128

// learn.microsoft.com/en-us/windows/win32/debug/pe-format#characteristics
#define IMAGE_FILE_CHARACTERISTIC_RELOCS_STRIPPED 0x0001
#define IMAGE_FILE_CHARACTERISTIC_EXECUTABLE_IMAGE 0x0002
#define IMAGE_FILE_CHARACTERISTIC_32BIT_MACHINE 0x0100
#define IMAGE_FILE_CHARACTERISTIC_DEBUG_STRIPPED 0x0200
#define IMAGE_FILE_CHARACTERISTIC_SYSTEM 0x1000
#define IMAGE_FILE_CHARACTERISTIC_DLL 0x2000
#define IMAGE_FILE_CHARACTERISTIC_BYTES_REVERSED 0x8000  // means it is MSB or Big endian. this flag is deprecated.

// learn.microsoft.com/en-us/windows/win32/debug/pe-format#optional-header-image-only
#define IMAGE_SUBSYSTEM_UNKNOWN 0
#define IMAGE_SUBSYSTEM_NATIVE 1       // device drivers and natvie Windows processes
#define IMAGE_SUBSYSTEM_WINDOWS_GUI 2  // Windows graphical user interface
#define IMAGE_SUBSYSTEM_WINDOWS_CUI 3  // windows character subsystem
#define IMAGE_SUBSYSTEM_OS2_CUI 5
#define IMAGE_SUBSYSTEM_POSIX_CUI 7
#define IMAGE_SUBSYSTEM_NATIVE_WINDOWS 8
#define IMAGE_SUBSYSTEM_WINDOWS_CE_GUI 9
#define IMAGE_SUBSYSTEM_EFI_APPLICATION 10  // Extensible Firmware Interface (EFI) application
#define IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER 11
#define IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER 12
#define IMAGE_SUBSYSTEM_EFI_ROM 13
#define IMAGE_SUBSYSTEM_XBOX 14
#define IMAGE_SUBSYSTEM_WINDOWS_BOOT_APPLICATION 16

// learn.microsoft.com/en-us/windows/win32/debug/pe-format DLL Characteristics

// https://learn.microsoft.com/en-us/windows/win32/debug/pe-format#section-flags
// section flags or characteristics
#define IMAGE_SCN_CNT_CODE 0x00000020
#define IMAGE_SCN_CNT_INITIALIZED_DATA 0x00000040// contains initialized data
#define IMAGE_SCN_CNT_UNINITIALIZED_DATA 0x00000080// contains uninitialized data
#define IMAGE_SCN_ALIGN_1BYTES 0x00100000
#define IMAGE_SCN_ALIGN_2BYTES 0x00200000
#define IMAGE_SCN_ALIGN_4BYTES 0x00300000
#define IMAGE_SCN_ALIGN_8BYTES 0x00400000
#define IMAGE_SCN_ALIGN_16BYTES 0x00500000
#define IMAGE_SCN_ALIGN_32BYTES 0x00600000
#define IMAGE_SCN_ALIGN_64BYTES 0x00700000
#define IMAGE_SCN_ALIGN_128BYTES 0x00800000
#define IMAGE_SCN_ALIGN_256BYTES 0x00900000
#define IMAGE_SCN_ALIGN_512BYTES 0x00A00000
#define IMAGE_SCN_ALIGN_1024BYTES 0x00B00000
#define IMAGE_SCN_ALIGN_2048BYTES 0x00C00000
#define IMAGE_SCN_ALIGN_4096BYTES 0x00D00000
#define IMAGE_SCN_ALIGN_8192BYTES 0x00E00000
#define IMAGE_SCN_MEM_NOT_CACHED 0x04000000
#define IMAGE_SCN_MEM_EXECUTE 0x20000000
#define IMAGE_SCN_MEM_READ 0x40000000
#define IMAGE_SCN_MEM_WRITE 0x80000000

#endif  // _PE_CONSTANTS_H
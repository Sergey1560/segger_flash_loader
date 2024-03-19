#ifndef SEGGER_FLASH_OS_H
#define SEGGER_FLASH_OS_H

typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned long U32;

typedef signed char I8;
typedef signed short I16;
typedef signed long I32;

#define ONCHIP     (1)             // On-chip Flash Memory

#define MAX_NUM_SECTORS (512)      // Max. number of sectors, must not be modified.
#define ALGO_VERSION    (0x0101)   // Algo version, must not be modified.

struct SECTOR_INFO  {
  U32 SectorSize;       // Sector Size in bytes
  U32 SectorStartAddr;  // Start address of the sector area (relative to the "BaseAddr" of the flash)
};

struct FlashDevice  {
   U16 AlgoVer;       // Algo version number
   U8  Name[128];     // Flash device name
   U16 Type;          // Flash device type
   U32 BaseAddr;      // Flash base address
   U32 TotalSize;     // Total flash device size in Bytes (256 KB)
   U32 PageSize;      // Page Size (number of bytes that will be passed to ProgramPage(). MinAlig is 8 byte
   U32 Reserved;      // Reserved, should be 0
   U8  ErasedVal;     // Flash erased value
   U32 TimeoutProg;   // Program page timeout in ms
   U32 TimeoutErase;  // Erase sector timeout in ms
   struct SECTOR_INFO SectorInfo[MAX_NUM_SECTORS]; // Flash sector layout definition
};

#endif
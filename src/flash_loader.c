#include "flash_loader.h"
#include "w25q.h"
#include "rcc.h"
#include "log.h"

struct FlashDevice const FlashDevice DevDescr =  {
  ALGO_VERSION,              // Algo version
  "F407 W25Q", // Flash device name
  ONCHIP,                    // Flash device type
  0x90000000,                // Flash base address
  0x400000,                  // Total flash device size in Bytes
  256,                       // Page Size (number of bytes that will be passed to ProgramPage(). May be multiple of min alignment in order to reduce overhead for calling ProgramPage multiple times
  0,                         // Reserved, should be 0
  0xFF,                      // Flash erased value
  100,                       // Program page timeout in ms
  6000,                      // Erase sector timeout in ms
  //
  // Flash sector layout definition
  //
  {
  {0x00000400, 0x00000000},   // 4096 *  4 KB =  16 MB
  {0xFFFFFFFF, 0xFFFFFFFF}    // Indicates the end of the flash sector layout. Must be present.
  }
};



int PrgCode SEGGER_FL_Prepare(U32 PreparePara0, U32 PreparePara1, U32 PreparePara2){
    int ret_value = -1;
    uint16_t flash_id;

	dwt_init();
	RCC_init();

    DEBUG("=>Prepare call");

    w25q_init();
    flash_id =  w25q_read_id();
    
    DEBUG("Flash ID 0x%0X",flash_id);

    if(flash_id == 0xEF14){
        ret_value = 0;
    }

    return ret_value;
};


int PrgCode SEGGER_FL_Restore(U32 RestorePara0, U32 RestorePara1, U32 RestorePara2){

    DEBUG("=>Restore call");
    return 0;
};


int PrgCode SEGGER_FL_Program(U32 DestAddr, U32 NumBytes, U8 *pSrcBuff){
    uint32_t addr=0;

    DEBUG("=>Program call addr:0x%0X %d bytes",DestAddr,NumBytes);

    if(DestAddr >= FlashDevice.BaseAddr){
        addr = DestAddr - FlashDevice.BaseAddr;
    }else{
        addr = DestAddr;
    }
    
    w25q_write(pSrcBuff,addr,NumBytes);
    return 0;
};

int PrgCode SEGGER_FL_Erase(U32 SectorAddr, U32 SectorIndex, U32 NumSectors){
    uint32_t addr=0;

    DEBUG("=>Erase call addr:0x%0X  index:%d sectors:%d bytes",SectorAddr,SectorIndex,NumSectors);

    if(SectorAddr >= FlashDevice.BaseAddr){
        addr = SectorAddr - FlashDevice.BaseAddr;
    }

    for(uint32_t i=SectorIndex; i<(SectorIndex+NumSectors); i++){
        w25q_erase(addr+(i*FlashDevice.SectorInfo->SectorSize),W25Q_ERASE_SECTOR);
    }
    return 0;

};

int PrgCode SEGGER_FL_EraseChip(void){

    DEBUG("=>Erase chip call");

    w25q_erase(0,W25Q_ERASE_CHIP);
    return 0;

};

int PrgCode SEGGER_FL_Read(U32 Addr, U32 NumBytes, U8 *pDestBuff){
    uint32_t addr=0;

    DEBUG("=>Read call addr: 0x%0X %d bytes",Addr,NumBytes);

    if(Addr >= FlashDevice.BaseAddr){
        addr = Addr - FlashDevice.BaseAddr;
    }

    w25q_read(pDestBuff,addr,NumBytes);

    return NumBytes;
}
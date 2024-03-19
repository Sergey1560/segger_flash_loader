#include "w25q.h"
static void w25q_page_write(uint8_t* data, uint32_t addr, uint16_t count);
static void w25q_wait(void);
static void w25q_write_en(void);

void w25q_init(void){
    spi1_init();
}


uint16_t w25q_read_id(void){
  uint16_t id;
  
  SPI1_CS_LOW;
  spi1_send(W25X_ManufactDeviceID,1);
  spi1_send(0x00,1);
  spi1_send(0x00,1);
  spi1_send(0x00,1);
  
  id = spi1_send(0xFF,1) << 8;
  id |= spi1_send(0xFF,1);

  SPI1_CS_HI;
  return id;
}

void w25q_read(uint8_t* data, uint32_t addr, uint32_t count) {
  SPI1_CS_LOW;

  spi1_send(W25X_ReadData,1);

  spi1_send((addr & 0xFF0000) >> 16,1);
  spi1_send((addr & 0xFF00) >> 8,1);
  spi1_send(addr & 0xFF,1);

  while (count--) { 
      *data = spi1_send(0xFF,1);
      data++;
    }

  SPI1_CS_HI;
}

void w25q_write(uint8_t* data, uint32_t addr, uint32_t count){

  while(count){
    if(count > W25Q_PageSize){
      w25q_page_write(data,addr,W25Q_PageSize);
      count -= W25Q_PageSize;
      data += W25Q_PageSize;
      addr += W25Q_PageSize;
    }else{
      w25q_page_write(data,addr,count);
      count = 0;
    }
  }

}

/*
Удаляет блок указанного размера, который включает байт с указанным адресом.
*/
void w25q_erase(uint32_t addr, enum w25q_erase_modes mode){

  w25q_write_en();
  
  SPI1_CS_LOW;

  spi1_send(mode,1);

  if(mode != W25Q_ERASE_CHIP){
    spi1_send((addr & 0xFF0000) >> 16,0);
    spi1_send((addr & 0xFF00) >> 8,0);
    spi1_send(addr & 0xFF,0);
  }

  SPI1_CS_HI;

  w25q_wait();
}

static void w25q_page_write(uint8_t* data, uint32_t addr, uint16_t count) {

  w25q_write_en();

  if(count > W25Q_PageSize){
    count = W25Q_PageSize;
  }

  if(count == W25Q_PageSize){
    addr &= 0xFFFF00;
  }

  SPI1_CS_LOW;
  spi1_send(W25X_PageProgram,0);
  spi1_send((addr & 0xFF0000) >> 16,0);
  spi1_send((addr & 0xFF00) >> 8,0);
  spi1_send(addr & 0xFF,0);


  while (count--) {
    spi1_send(*data,0);
    data++;
  }

  SPI1_CS_HI;
  w25q_wait();
}


static void w25q_write_en(void){
  uint8_t status;

  SPI1_CS_LOW;
  spi1_send(W25X_WriteEnable,1);
  SPI1_CS_HI;

  SPI1_CS_LOW;
  spi1_send(W25X_ReadStatusReg,1);

  do
    status = spi1_send(0xFF,0);
  while ((status & 0x02) == 0);
  
  SPI1_CS_HI;
}

static void w25q_wait(void){
  uint8_t status = 0;

  SPI1_CS_LOW;
  spi1_send(W25X_ReadStatusReg,1);

  do
    status = spi1_send(0xFF,0);
  while (status & 0x01);

  SPI1_CS_HI;
}
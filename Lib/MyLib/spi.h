#ifndef SPI_H
#define SPI_H
#include "stdint.h"
#include "stm32f4xx.h"

#define SPI1_CS_HI     GPIOA->BSRR = GPIO_BSRR_BS15
#define SPI1_CS_LOW    GPIOA->BSRR = GPIO_BSRR_BR15
#define SPI1_DR_8b     (*(__IO uint8_t *)((uint32_t)SPI1+0x0C))


void spi1_init(void);
void spi1_gpio_init(void);
uint8_t spi1_send(uint8_t data, uint8_t read);

#endif
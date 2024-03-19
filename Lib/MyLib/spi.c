#include "spi.h"


uint8_t spi1_send(uint8_t data, uint8_t read){
    uint8_t ret=0;

    (void)SPI1->DR;
    
    while((SPI1->SR & SPI_SR_TXE) == 0){__NOP();};
    SPI1->DR = data;
    while((SPI1->SR & SPI_SR_RXNE) == 0){__NOP();};
    ret=SPI1_DR_8b;

    return ret;
}


void spi1_init(void){

    spi1_gpio_init();

    //84Mhz
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    SPI1->CR1 = 0;
    SPI1->CR1 = SPI_CR1_SSI|SPI_CR1_SSM |\
                SPI_CR1_CPHA|SPI_CR1_CPOL |\
                SPI_CR1_MSTR |\
                (5 << SPI_CR1_BR_Pos);

	SPI1->CR1 |= SPI_CR1_SPE;
}

/*
PB3 SCK
PB5 MOSI
PB4 MISO
PA15 CS
*/
void spi1_gpio_init(void){

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN|RCC_AHB1ENR_GPIOAEN;

    //PB3,PB4,PB5
    GPIOB->MODER &= ~(GPIO_MODER_MODE3|GPIO_MODER_MODE4|GPIO_MODER_MODE5);
    GPIOB->MODER |= (GPIO_MODER_MODE3_1|GPIO_MODER_MODE4_1|GPIO_MODER_MODE5_1);

    GPIOB->OTYPER &= ~(GPIO_OTYPER_OT3|GPIO_OTYPER_OT4|GPIO_OTYPER_OT5);

    GPIOB->OSPEEDR |= (3 << GPIO_OSPEEDR_OSPEED3_Pos)| (3 << GPIO_OSPEEDR_OSPEED4_Pos)|(3 << GPIO_OSPEEDR_OSPEED5_Pos);

    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD3|GPIO_PUPDR_PUPD4|GPIO_PUPDR_PUPD5);

    GPIOB->AFR[0] &= ~(GPIO_AFRL_AFRL3|GPIO_AFRL_AFRL4|GPIO_AFRL_AFRL5);
    GPIOB->AFR[0] |= (5 << GPIO_AFRL_AFSEL3_Pos)|(5 << GPIO_AFRL_AFSEL4_Pos)|(5 << GPIO_AFRL_AFSEL5_Pos);

    //PA15 CS
    GPIOA->MODER &= ~GPIO_MODER_MODE15;
  	GPIOA->MODER |= GPIO_MODER_MODE15_0;
	
    GPIOA->OSPEEDR |= (3 << GPIO_OSPEEDR_OSPEED15_Pos);
	
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD15;

    SPI1_CS_HI;
}   
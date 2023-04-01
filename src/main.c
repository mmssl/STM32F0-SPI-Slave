#include "stm32f0xx_conf.h"

/*Private functions decleration*/
void SPI1_SlaveConfig (void);
void GPIO_config (void); 
void SPI1_Receive (volatile uint8_t *data, volatile int size);
/*Private variables*/
volatile uint8_t data[3];


void SPI1_SlaveConfig (void)
{
	RCC->APB2ENR |= (1<<12);
	//CR1 configurations
	
	//SPI1->CR1 |= (3<<3); baud rate settings not recuired for slave mode
	SPI1->CR1 |=  (1<<1) | (1<<0); // cpol = 1, cpha = 1
	SPI1->CR1 &= ~(1<<10); // full-duplex mode
	SPI1->CR1 &= ~(1<<7);  // MSB first
	SPI1->CR1 &= ~(1<<14); // output disabled receive only
	SPI1->CR1 |=  (1<<15); // bidirectional mode
	SPI1->CR1 &= ~(1<<8) & ~(1<<9); // hardware nss active
	SPI1->CR1 &= ~(1<<2); // slave mode

	//CR2 configuration
	SPI1->CR2 |=  (7<<8); // 8-bit data
	SPI1->CR2 &= ~(1<<2); // master mode disabled
}

void GPIO_config (void)
{
	RCC->AHBENR    |=   (1<<17); // GPIOA clock enabled
	GPIOA->MODER   &= ~(0xffffffff);
	GPIOA->MODER   |=  (2<<8) | (2<<10) | (2<<12) | (2<<14); // alternate function mode for PA4-5-6-7
	GPIOA->OSPEEDR |=  (3<<8) | (3<<10) | (3<<12) | (3<<14); // high speed to all pins
	GPIOA->AFR[0]  |=  (5<<16) | (5<<20) | (5<<24) | (5<<28); // alternate functions selection 
}

void SPI1_Receive (volatile uint8_t *data, volatile int size)
{
	while (size)
  {
    while(!((SPI1->SR)&(1<<7))) {} // wait to BSY bit is reset
    SPI1->DR = 0; // send dummy data
    while(!((SPI1->SR)&(1<<0))) {} // wait for rxne bit to set
	*data++ = (SPI1->DR);
	size--;
  }	
}

int main (void)
{
	SystemInit();
	SPI1_SlaveConfig();
	GPIO_config();

	SPI1->CR1 |= (1<<6); // enable spi
	SPI1_Receive(data, 3);
	SPI1->CR1 &= ~(1<<6); // disable spi

	while (1)
	{

	}
	return 0;
}

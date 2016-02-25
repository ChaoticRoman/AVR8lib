#include "spi.h"

#include <avr/io.h>

void SPI_MasterInit(void)
{
  /* Set MOSI and SCK output*/
  DDRB |= 0b10100000;
  /* MISO input*/
  DDRB &= 0b10111111;
  
  /* Enable SPI, Master, set clock rate fck/4 */
  SPCR = 0b01010000;
}

char SPI(char cData) {
  char recieved;

  SPDR = cData; // send
  while(!(SPSR & (1<<SPIF))) ;  // Wait for transmission complete
  recieved = SPDR;

  return recieved;
}

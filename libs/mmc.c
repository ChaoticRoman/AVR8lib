#include "mmc.h"

#include "spi.h"

#include <avr/io.h>
#include <inttypes.h>

char MMC_command(char cmd, uint16_t ArgH, uint16_t ArgL, char crc ) {
                  //command name, 4B args, CRC
   SPI(0xFF);
   SPI(cmd);
   SPI( (uint8_t)(ArgH >> 8) );
   SPI( (uint8_t)ArgH );
   SPI( (uint8_t)(ArgL >> 8) );
   SPI( (uint8_t)ArgL );
   SPI(crc);
   SPI(0xFF);
   return SPI(0xFF);            // Returns the last byte received
}


int MMC_init(void) {
  SPI_MasterInit();             // Init SPI
  SPCR |=0b00000010;            // SPI 640 kHz
  MMC_CS_DDR |=1 << MMC_CS_PIN;            // CS
  MMC_CS_PORT|=1 << MMC_CS_PIN;            // CS high=disable MC

  char i;
  for(i=0; i < 10; i++) 
    SPI(0xFF);                  // Send 10 * 8 = 80 clock pulses

  MMC_CS_PORT&=~(1 << MMC_CS_PIN);         //enable CS
  for(i=0; i < 2; i++) 
    SPI(0xFF);                  // Send 2 * 8 = 16 clock pulses

  if (MMC_command(0x40,0,0,0x95)!=1)   // CMD0 go idle, (1 idle)
    return 1;

  while (MMC_command(0x41,0,0,0xFF) !=0) ; // CMD1 init (1 busy, 0 ready)

  SPCR &=0b11111100;            // SPI 2.5 MHz

  if (MMC_command(0x50,0,512,0xFF)!=0)  // CMD16 set blocklen 512 (0 success)
    return 2;

  return 0;
}


int MMC_write(uint16_t sector, char * data) { 
  uint16_t i;
  uint32_t addres=sector<<9;
  char c;

  c = MMC_command(0x58,(uint16_t)(addres>>16),(uint16_t)addres,0xFF);
  if (c!=0) {//CMD24, write block
/*    USART_Transmit_byte(c);
    USART_Transmit('\n');*/
    return 1;
  }

  SPI(0xFF);             //card response R1
  SPI(0xFF);

  SPI(0xFE);             // Start block data token

  for (i=0;i<BUFF_LEN;i++)    // write ram sectors to MMC
    SPI(data[i]);
  for (i=BUFF_LEN;i<512;i++)    // write ram sectors to MMC
    SPI(0);

  SPI(0xFF);             //16-bit CRC
  SPI(0xFF);

  c = SPI(0xFF);        // Data response
  c &= 0b00011111;      // 0bxxx0???1, ???=010 -> OK
  if (c != 0b00000101)
    return 1;

// ATT: typecast (char)0xFE is a must!
  while(SPI(0xFF) != (char)0xFF) ;  // wait until MMC is not busy anymore
  return 0;
}


int MMC_read(uint16_t sector, char * target) {
  int i;
  char c;
  uint32_t addres=sector<<9;

  c=MMC_command(0x51,(uint16_t)(addres>>16),(uint16_t)addres,0xFF);  // CMD17 read single block
  if (c != 0) {
/*    USART_Transmit_byte(c);
    USART_Transmit('\n');*/
    return 1; //error
  }

// ATT: typecast (char)0xFE is a must!
    while(SPI(0xFF) != (char)0xFE) ; // wait for transmission start
//  USART_Transmit_string("Transmission start...\n ");

    for(i=0; i < BUFF_LEN; i++) // data recieve
      target[i] = SPI(0xFF);

    for(i=BUFF_LEN; i<512 ; i++) // dummy data recieve
      SPI(0xFF);
    
    SPI(0xFF); //2B CRC
    SPI(0xFF);
    return 0;
}

void MMC_shutdown() {
  MMC_CS_PORT|=1 << MMC_CS_PIN;            // CS high=disable MC
}


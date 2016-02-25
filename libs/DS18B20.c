/*
** ===================================================================
** Name:		   : DS18B20.c
** Describe:       : Source file for basic control temperature sensor Dallas DS18B20
** Author:         : Unknown,
**                 : rewrote and remade Vít Svadbík (vitsvadbik@email.cz) 
**		   		   : BuB, March 2011; pak Roman Pavelka Jul 2011
** ===================================================================
*/

#include "DS18B20.h"

////////////////// initilization / reset //////////////////

void set_therm(char therm_num) {
  THERM_DQ=therm_num;  
}

uint8_t therm_reset(void)
{	
uint8_t i;	//uint8_t - data type defined in stdint.h - 8-bit signed type

//Pull line low and wait for 480uS
_delay_us(1000); // for for standstill bus
THERM_LOW(); 
THERM_OUTPUT_MODE();	
_delay_us(480);

//Release line and wait for 60uS
THERM_HIGH(); 
THERM_INPUT_MODE();		
_delay_us(70);

//Store line value and wait until the completion of 480uS period
i=(THERM_PIN & (1<<THERM_DQ));	// THERM_PIN = receive level
_delay_us(410);
THERM_LOW(); 
//Return the value read from the presence pulse (0=OK, 1=WRONG)
return i;
}

////////////////// write bit to DS18B20 //////////////////

void therm_write_bit(uint8_t bit)
{
_delay_us(1);
//Pull line low for 1uS
THERM_LOW();
THERM_OUTPUT_MODE();
_delay_us(6);

//If we want to write 1, release the line (if not will keep low)
if(bit) THERM_INPUT_MODE(); //write log. 1

//Wait for 60uS (66uS) and release the line
_delay_us(60);		//write log.0
THERM_INPUT_MODE();
}

////////////////// write byte to DS18B20 //////////////////

void therm_write_byte(uint8_t byte)
{
uint8_t i=8;

 while(i--){		//running for 7 bite 
 //Write actual bit and shift one position right to make the next bit ready
 therm_write_bit(byte&1);
 byte>>=1;
 }
}

////////////////// read bit from DS18B20 //////////////////

uint8_t therm_read_bit(void)
{
uint8_t bit=0;

//Pull line low for 1uS
THERM_LOW();
THERM_OUTPUT_MODE();
_delay_us(1);

//Release line and wait for 14uS
THERM_INPUT_MODE();
_delay_us(14);

//Read line value
if(THERM_PIN&(1<<THERM_DQ)) bit=1;	//if receive high - bit = 1

//Wait for 45uS to end and return read value
_delay_us(45);
return bit;
}

////////////////// read byte from DS18B20 //////////////////
uint8_t therm_read_byte(void)
{
uint8_t i=8, n=0;

 while(i--){	//running for 7 bite 
 //Shift one position right and store read value
 n>>=1;		
 n|=(therm_read_bit()<<7);
 }
return n;
}


uint16_t DS_get(char num) {
    uint8_t temperature[2];

    //Reset, skip ROM and start temperature conversion
    set_therm(num);
    if (therm_reset()) { //ERROR
        return 0;
    } else {
	therm_write_byte(SKIP_ROM);
	therm_write_byte(CONVERT_T);

	//Wait until conversion is complete
	while(!therm_read_bit()); //DS18B20 send 1 for conversion is done

	//Reset, skip ROM and send command to read Scratchpad
	therm_reset();
	therm_write_byte(SKIP_ROM);
	therm_write_byte(READ_SCRAT);

	//Read Scratchpad (only 2 first bytes)
	temperature[0]=therm_read_byte();               //low byte
	temperature[1]=therm_read_byte();               //high byte
    }
//therm_reset();

//conversion 2 byte to digit and decimal
    uint8_t digit;
    uint16_t decimal;

    digit = (temperature[1]<<4)&0xf0;               //form 2 bytes to byte for conversion
    digit |= (temperature[0]&0xf0)>>4;

    decimal = (temperature[0]&0xf)*625/100;   //only 2 decimal place (1000 - one decimal place)*/

    //uint8_t result;
    //result = (temperature[1]<<4)|(temperature[0]>>4); //
    	return (digit<<8)|(uint8_t)decimal;
}


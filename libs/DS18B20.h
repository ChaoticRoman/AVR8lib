/*
** ===================================================================
** Name:		   : DS18B20.h
** Describe:       : Header file for basic control temperature sensor Dallas DS18B20
** Author:         : Unknown,
**                 : rewrote and remade Vít Svadbík (vitsvadbik@email.cz) 
**		   		   : BuB, March 2011
** ===================================================================
*/

#ifndef DS18B20
#define DS18B20
#endif

#include <avr/io.h>	

#ifndef F_CPU
 #define F_CPU 16000000UL
#endif

#include <util/delay.h>
#include <stdint.h>


//defined command - datasheet DS18B20
#define CONVERT_T	0x44
#define READ_SCRAT	0xbe
#define WRITE_SCRAT 0x4e
#define COPY_SCRAT	0x48
#define RECALL_E2	0xb8
#define READ_PWR	0xb4
#define SEARCH_ROM	0xf0
#define READ_ROM	0x33
#define MATCH_ROM	0x55
#define SKIP_ROM 	0xcc
#define ALARM_SRCH	0xec


// Thermometer conections
#define THERM_PORT	PORTD
#define THERM_DDR	DDRD
#define THERM_PIN	PIND
//#define THERM_DQ	PC5		//Conect data from DS18B20
char THERM_DQ;

// Useful setting
#define THERM_INPUT_MODE() THERM_DDR&=~(1<<THERM_DQ) //DDR set as input
#define THERM_OUTPUT_MODE() THERM_DDR|=(1<<THERM_DQ) //DDR set as output
#define THERM_LOW() THERM_PORT&=~(1<<THERM_DQ)		 //out is low (log. 0)
#define THERM_HIGH() THERM_PORT|=(1<<THERM_DQ)		 //out is high(log. 1)

//Functions

// initilization / reset 
uint8_t therm_reset(void);
void set_therm(char therm_num);

// write bit to DS18B20 
void therm_write_bit(uint8_t bit);

// write byte to DS18B20 - paste command, defined upper
void therm_write_byte(uint8_t byte);

// read bit from DS18B20 
uint8_t therm_read_bit(void);

// read byte from DS18B20 - return receive value
uint8_t therm_read_byte(void);


uint16_t DS_get(char num);

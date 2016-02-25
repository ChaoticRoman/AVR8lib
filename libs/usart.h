#ifndef USART_H
#define USART_H

//Inits:
void USART_Init( unsigned int ubrr);	// 8b, no parity, 1 stop
					// asynchronous, RX interrupt

// Busy loops:
void USART_Transmit( unsigned char data ); // send char (1B)
void USART_Transmit_string( char *data ); // send string
void USART_Transmit_num( unsigned char data ); // send ascii
void USART_Transmit_longnum( signed long data ); // send ascii
void USART_Transmit_num_padded( unsigned long data, unsigned char pads ); // send ascii
//void USART_Transmit_float( float data );
void USART_Transmit_byte( char data ); //send in ascii hex reprezentation

#endif

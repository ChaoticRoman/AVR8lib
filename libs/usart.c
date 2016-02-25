#include "usart.h"

#include <avr/io.h>


void USART_Init( unsigned int ubrr)

{
  /* Set baud rate */
  UBRRH = (unsigned char)(ubrr>>8);
  UBRRL = (unsigned char)ubrr;

  /* flush transmitt register, Double speed */
  UCSRA = (1<<UDRE)|(1 << U2X);

  /* enable receiver interrupt and transmitter, pins forced */
  UCSRB = (1<<RXCIE)|(1<<RXEN)|(1<<TXEN);

  /* Asynchronous, no parity, Set frame format: 8data, 1stop bit */
  UCSRC = (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);
}


void USART_Transmit( unsigned char data )
{
  while ( !( UCSRA & (1<<UDRE)) );  /* Wait for empty transmit buffer */

  UDR = data;  /* Put data into buffer, sends the data */
}


void USART_Transmit_string( char *data )
{
  while ( *data != 0 ) {
    USART_Transmit(*data);
    data++;
  }
}


void USART_Transmit_longnum(signed long data ) {
  unsigned char digits[10];
	signed char i;

  if (data==0) 
  USART_Transmit('0');  
  else { // PROZKOUMAT!

  if (data<0) {
    USART_Transmit('-');
    data*=-1;
  }

  for (i=0;i<10;i++) {
    digits[i]=data%10;
    data=data/10;
  }

  i=9;
  while (digits[i]==0) i--;
  while (i>0) {
    USART_Transmit(digits[i]+48);
    i--;
  }
  USART_Transmit(digits[0]+48);  

 }
}


void USART_Transmit_num(unsigned char data ) {
  unsigned char a,b,c;

  c=data%10;
  data=data/10;
  b=data%10;
  data=data/10;
  a=data%10;

  if (a>0) USART_Transmit(a+48);
  if ((a>0) || (b>0)) USART_Transmit(b+48);
  USART_Transmit(c+48);  
}


void USART_Transmit_num_padded(unsigned long data, unsigned char pads ) {
  unsigned char digits[10];
	signed char i;

//   if (data==0) 
//   USART_Transmit('0');  
//   else { // PROZKOUMAT!
/*
  if (data<0) {
    USART_Transmit('-');
    data*=-1;
  }*/

  for (i=0;i<pads;i++) {
    digits[i]=data%10;
    data=data/10;
  }
  
  for (i=pads-1;i>-1;i--) {
    USART_Transmit(digits[i]+48);
  }
/*
 }*/
}


/*void USART_Transmit_float( float data ) {
  int a,b,c;

  c=((int)data)%10;
  b=((int)(data/10))%10;
  a=((int)(data/100))%10;

  USART_Transmit(a+48);
  USART_Transmit(b+48);
  USART_Transmit(c+48);
}*/


void USART_Transmit_byte( char data ) {
  unsigned char i;
  i=0b10000000;
  while (i) {
    USART_Transmit( ( (i&data)&&1 ) + 48 );
    i>>=1;
  }
}

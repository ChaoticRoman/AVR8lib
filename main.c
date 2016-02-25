// ************************
//    Project name, 2014
// ************************

#include "defines.h"

#include <util/delay.h>

#include "libs/T6963C.h"
#include "libs/graphic.h"
#include "libs/usart.h"
#include "libs/DS18B20.h"


#define BL_OUTPUT sbi(DDRB, 3)
#define BL_ON sbi(PORTB, 3)
#define BL_OFF cbi(PORTB, 3)

#define LED_OUTPUT sbi(DDRD, 7)
#define LED_ON sbi(PORTD, 7)
#define LED_OFF cbi(PORTD, 7)
#define BLINK LED_ON;_delay_ms(100);LED_OFF
#define BLINK00 BLINK;_delay_ms(100)
#define BLINK3 BLINK00;BLINK00;BLINK00

#define DS_OUTPUT sbi(DDRD, 6)
#define DS_ON sbi(PORTD, 6)
#define DS_OFF cbi(PORTD, 6)


volatile uint8_t data, data_flag;


ISR(USART_RXC_vect) { // Prepsat bezpecne: detekci neplatnych znaku, timeout, potvrzeni na zaver
  data=UDR;
  data_flag = 1;
}


int main(void)
{
    BL_OUTPUT;
//    DS_OUTPUT;
    BL_ON;

    GLCD_Initalize(); // Initalize LCD
    GLCD_ClearText(); // Clear text area
    GLCD_ClearCG(); // Clear character generator area
    GLCD_ClearGraphic(); // Clear graphic area

    GLCD_TextGoTo(0,0);// set text coordinates
    GLCD_WriteString("Hello"); // write text
    GLCD_Circle(32,32,20); // draw circle
    GLCD_Rectangle(8,8,24, 48); // draw rectangle
//    GLCD_FillRectangle(0, 0, 96, 8);

    USART_Init(MYUBRR);
    USART_Transmit_string("Hi\n");

    LED_OUTPUT;
    BLINK3;
    GLCD_ClearText();
    GLCD_TextGoTo(0,0);// set text coordinates

    ADCSRA = 0b10000111;

    data_flag=0;


    sei();
    uint16_t T = 0;
    uint8_t i;
    while(1)
    {
      if (data_flag == 1)
      {
          //BLINK00; // Heh nasty error when it is here, if 'g' followed by newline comes, because it rewrites data here :D
          if (data == 't')
          {
              T = DS_get(6);
              GLCD_TextGoTo(0,0);

              i = (uint8_t)(T>>8);
              USART_Transmit_num(i);
              GLCD_WriteChar(i/10 + '0');
              GLCD_WriteChar(i%10 + '0');

              USART_Transmit('.');
              GLCD_WriteChar('.');

              i = (uint8_t)(T);
              USART_Transmit_num(i);
              GLCD_WriteChar(i/10 + '0');
              GLCD_WriteChar(i%10 + '0');

              USART_Transmit('C');
              GLCD_WriteChar('C');

              USART_Transmit('\n');
          }

          if (data == 'g')
          {
              ADMUX = 0x00;
              sbi(ADCSRA, ADSC);
              while (qbi(ADCSRA, ADSC))
                  ;
              uint16_t val = ADC;
              GLCD_TextGoTo(0,0);

          }
          data_flag=0;
      }
    }


    return 0;
}


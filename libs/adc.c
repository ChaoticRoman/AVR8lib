#include "adc.h"

#include <avr/io.h>>

inline void AD_single_conversion() {
  ADCSRA |= (1 << ADSC);  // Start A2D Conversions 
}

void AD_Init_Interrupt() {
  ADMUX = 0b01000101; // PORTC_5,Vref=V_ACC,capacitor on AVREF

  ADCSRA = (0b00000111); // 128 Prescaling
  ADCSRA |= (1 << ADFR);  // Set ADC to Free-Running Mode
  ADCSRA |= (1 << ADIE);  // enable interrupt
//  sei();
  ADCSRA |= (1 << ADEN);  // Enable ADC
  ADCSRA |= (1 << ADSC);  // Start A2D Conversions 
}


void AD_Init() {
  ADMUX = 0b01000000; // PORTC_0,Vref=Vacc with cap. on ref.

  ADCSRA |= (0b00000111); // 128 Prescaling
  //ADCSRA |= (1 << ADFR);  // Set ADC to Free-Running Mode
  ADCSRA |= (1 << ADEN);  // Enable ADC
  ADCSRA |= (1 << ADSC);  // Start A2D Conversions
}


unsigned char AD_Read() {
  char val;
  val=(ADCL>>2); // low must be read first!! see datasheet...
  val|=(ADCH<<6);
  return val;
}

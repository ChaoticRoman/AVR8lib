#include "timer.h"

#include <avr/io.h>

void Timer0_Init_Interrupt() {
  TIMSK |= 0b0000001; // Enable overflow interrupt tim0                                                                                   
  TCCR0=0b00000010; //8 prescale,start timer
}

void Timer1_Init_Interrupt() {
  TIMSK |= 0b00000100; // Enable overflow interrupt tim1
  TCCR1B=0b00000010; //8 prescale,start timer
}


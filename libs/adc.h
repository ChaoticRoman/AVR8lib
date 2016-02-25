#ifndef ADC_H
#define ADC_H

inline void AD_single_conversion();
void AD_Init_Interrupt();
void AD_Init();
unsigned char AD_Read();

#endif
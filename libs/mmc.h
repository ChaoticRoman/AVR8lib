#ifndef MMC_H
#define MMC_H

#include <inttypes.h>

#define MMC_CS_DDR DDRB
#define MMC_CS_PORT PORTB
#define MMC_CS_PIN 4

#define BUFF_LEN 10

char MMC_command(char cmd, uint16_t ArgH, uint16_t ArgL, char crc );

int MMC_init(void);
int MMC_write(uint16_t sector, char * data);
int MMC_read(uint16_t sector, char * target);
void MMC_shutdown();

#endif

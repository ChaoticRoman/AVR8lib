PRG            = IV_meter
OBJ            = main.o ./libs/usart.o ./libs/T6963C.o ./libs/graphic.o ./libs/DS18B20.o
MCU_TARGET     = atmega16
OPTIMIZE       = -O1
PROGPORT       = /dev/ttyUSB1
SERIALPORT     = /dev/ttyUSB0


CC             = avr-gcc
# Override is only needed by avr-lib build system.
override CFLAGS = -g -Wall $(OPTIMIZE) -mmcu=$(MCU_TARGET) $(DEFS)
OBJCOPY        = avr-objcopy
OBJDUMP        = avr-objdump

all: hex

#all depends on defines.h
$(OBJ): defines.h
./libs/usart.o: ./libs/usart.h
./libs/DS18B20.o: ./libs/DS18B20.h
./libs/T6963C.o: ./libs/T6963C.h
./libs/graphic.o: ./libs/graphic.h

$(PRG).elf: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -rf *.o $(PRG).elf $(PRG).hex

hex:  $(PRG).hex

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

install: load

load: $(PRG).hex
	avrdude -p m16 -c stk500v2 -P $(PROGPORT) -B 2 -U flash:w:$< 

ping:
	avrdude -p m16 -c stk500v2 -P $(PROGPORT)

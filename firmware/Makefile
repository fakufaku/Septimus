
CC=avr-gcc
PROGRAMMER=usbtiny
PORT=usb
CPU=attiny2313

NAME=septimus

SRCS = ${NAME}.c usiTwiSlave.c 
OBJS = $(SRCS:.c=.o)
ELF=${NAME}.elf
HEX=${NAME}.hex

LFUSE=0x64
HFUSE=0xDF
EFUSE=0xFF


%.o : %.c
	@echo "[$(TARGET)] Compiling:" $@...
	#${CC} $(CXXFLAGS) -MMD -MF $(@:%.o=%.d) -c $< -o $@
	${CC} -Os -mmcu=${CPU} -c $<

elf: ${OBJS}
	${CC} -mmcu=${CPU} -o ${ELF} ${OBJS}

size: ${ELF}
	avr-size -C --mcu=${CPU} ${ELF}

hex: elf size
	avr-objcopy -j .text -j .data -O ihex ${ELF} ${HEX}

avrdude: hex
	avrdude -c ${PROGRAMMER} -P ${PORT} -p ${CPU} -U flash:w:${HEX}:i

rfuse:
	avrdude -c ${PROGRAMMER} -p ${CPU} -U lfuse:r:-:h 2> /dev/null
	avrdude -c ${PROGRAMMER} -p ${CPU} -U hfuse:r:-:h 2> /dev/null
	avrdude -c ${PROGRAMMER} -p ${CPU} -U efuse:r:-:h 2> /dev/null

wfuse:
	avrdude -c ${PROGRAMMER} -p ${CPU} -U lfuse:w:${LFUSE}:m 2> /dev/null
	avrdude -c ${PROGRAMMER} -p ${CPU} -U hfuse:w:${HFUSE}:m 2> /dev/null
	avrdude -c ${PROGRAMMER} -p ${CPU} -U efuse:w:${EFUSE}:m 2> /dev/null


clean:
	rm -rf ${ELF} ${OBJS} ${HEX}

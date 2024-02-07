TARGET := MICROBIT
BSPDIR := .

GCCARM=/opt/arm-gcc
CC=arm-none-eabi-gcc
AS=arm-none-eabi-as
LD=arm-none-eabi-ld
AR=arm-none-eabi-ar
OBJCOPY=arm-none-eabi-objcopy
SIZE=arm-none-eabi-size

LINKER_SCRIPT  := nrf52833.ld

# Source files common to all targets
BSP_OBJS += \
  system.o \
  ledbtn.o \
  uart.o \
  gpio.o \
  pwm.o \
  motor.o \
  adc.o \
  printf.o \
  audio.o \
  i2c.o \
  lsm303agr.o \
  timer.o \

OBJS += startup.o main.o \

# C flags common to all targets
CFLAGS += -O3 -g3               # optimization
CFLAGS += -Wall -Werror         # treat all warnings as errors
CFLAGS += -mcpu=cortex-m4       # CPU architecture
CFLAGS += -mthumb -mabi=aapcs   # code generation options
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16    # floating point unit

# keep every function in a separate section, this allows linker to discard unused ones
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing -fno-builtin

# Linker flags
LDFLAGS := -T $(LINKER_SCRIPT)  # linker command file
LDFLAGS += -Map system.map      # linker map file
LDFLAGS += --gc-sections        # let linker dump unused sections

# Add standard libraries at the very end of the linker input, after all objects
# that may need symbols provided by these libraries.
LIBS := $(BSPDIR)/libbsp.a \
        $(GCCARM)/arm-none-eabi/lib/thumb/v7e-m+fp/hard/libm.a \
        $(GCCARM)/arm-none-eabi/lib/thumb/v7e-m+fp/hard/libc.a \
        $(GCCARM)/lib/gcc/arm-none-eabi/12.3.1/thumb/v7e-m+fp/hard/libgcc.a \

.SUFFIXES: .c .i .S .o .out .hex

.c.o:
	$(CC) -c $(CFLAGS) $<

.c.i:
	$(CC) -E $(CFLAGS) $< > $@

.S.o:
	$(CC) -c $(ASMFLAGS) $<

.out.hex:
	$(OBJCOPY) -O ihex $< $@

all: flash

flash: $(TARGET).hex
	cp MICROBIT.hex /mnt/c/Users/vdars/Temp
	cd /mnt/c/Users/vdars/Temp && cmd.exe /c copy 'MICROBIT.hex' E:

$(TARGET).out: $(OBJS) libbsp.a $(LINKER_SCRIPT)
	$(LD) $(LDFLAGS) $(OBJS) $(LIBS) -o $@
	$(SIZE) $@

libbsp.a: $(BSP_OBJS)
	/bin/rm -f $@
	$(AR) -r $@ $(BSP_OBJS)

clean:
	/bin/rm -f $(OBJS) $(BSP_OBJS) $(TARGET).out

# Dependencies
$(TARGET).hex: $(TARGET).out
$(TARGET).out: $(OBJS)

# Dependencies
adc.o: adc.c adc.h
audio.o: audio.c audio.h gpio.h pwm.h adc.h
gpio.o: gpio.c gpio.h
i2c.o: i2c.c
ledbtn.o: ledbtn.c ledbtn.h gpio.h
lsm303agr.o: lsm303agr.c lsm303agr.h
main.o: main.c bsp.h
motor.o: motor.c nrf52833.h motor.h
printf.o: printf.c printf.h
pwm.o: pwm.c pwm.h
startup.o: startup.c
system.o: system.c
timer.o: timer.c timer.h
uart.o: uart.c gpio.h uart.h

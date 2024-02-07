# Path to tool-chain
ARMGCC=/opt/arm-gnu-toolchain-12.3.rel1-x86_64-arm-none-eabi/

# Path to BSP library
BSPDIR=.

# Tool-chain programs
CC=$ARMGCC/bin/arm-none-eabi-gcc
LD=$ARMGCC/bin/arm-none-eabi-ld
AR=$ARMGCC/bin/arm-none-eabi-ar
SIZE=$ARMGCC/bin/arm-none-eabi-size
OBJCOPY=$ARMGCC/bin/arm-none-eabi-objcopy

# ARM architecture specific compiler flags
ARCH="-mcpu=cortex-m4 -mthumb -mabi=aapcs -mfloat-abi=hard -mfpu=fpv4-sp-d16"

# Keep every function in a separate section, this allows linker to discard unused ones
OPT="-ffunction-sections -fdata-sections -fno-strict-aliasing -fno-builtin"

# Compiler flags
CFLAGS="-O3 -g3 -Wall -Werror $ARCH $OPT"

# Compile the source files
echo Compiling startup.c
$CC -c $CFLAGS \
        -DSysTick_Handler=bspSysTick_Handler \
        -DGPIOTE_IRQHandler=bspGPIOTE_IRQHandler \
        startup.c

echo Compiling main.c
$CC -c $CFLAGS main.c

. ./build_lib.sh

# Link the object code to form exectuable program
echo Linking MICROBIT.out
$LD -T nrf52833.ld -Map system.map --entry Reset_Handler --gc-sections \
        startup.o main.o $BSPDIR/libbsp.a \
        $ARMGCC/arm-none-eabi/lib/thumb/v7e-m+fp/hard/libm.a \
        $ARMGCC/arm-none-eabi/lib/thumb/v7e-m+fp/hard/libc.a \
        $ARMGCC/lib/gcc/arm-none-eabi/12.3.1/thumb/v7e-m+fp/hard/libgcc.a \
        -o MICROBIT.out \

# Check sizes
$SIZE MICROBIT.out

# Generate HEX file to load on the target
$OBJCOPY -O ihex MICROBIT.out MICROBIT.hex

# Upload on the target
/bin/cp MICROBIT.hex /mnt/c/Users/vdars/Temp
cd /mnt/c/Users/vdars/Temp
cmd.exe /c copy 'MICROBIT.hex' E:
cd -

# The above command is applicable for a Windows PC. You may change it
# appropriately to on Linux or MacOS. It should be something like:
# cp MICROBIT.hex /media/MICROBIT       # linux
# cp MICROBIT.hex /Volumes/MICROBIT     # mac

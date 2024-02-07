MEMORY
{
    FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 0x00080000
    SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 0x00020000
}

ENTRY(Reset_Handler)

SECTIONS
{
    .text :
    {
        KEEP(*(.isr_vector))
        *(.text*)
        *(.rodata*)
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        . = ALIGN(4);
    } > FLASH

    .data : AT(ADDR(.text) + SIZEOF(.text))
    {
        __etext = LOADADDR (.data);
        __data_load__ = LOADADDR (.data);
        __data_start__ = .;
        . = ALIGN(4);
        *(.data*)
        . = ALIGN(4);
        __data_end__ = .;
    } > SRAM

    .bss :
    {
        __bss_start__ = .;
        . = ALIGN(4);
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
    } > SRAM

    .heap :
    {
        __heap_start__ = .;
        . = ALIGN(4);
        *(.heap*)
        . = ALIGN(4);
        __heap_end__ = .;
    }

    .stack :
    {
        . = ALIGN(8);   /* initial sp should be 8-byte aligned */
        __stack_start_ = .;
        *(.stack*)
        __stack_end_ = .;
    }
}

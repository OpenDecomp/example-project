/* Memory layout of the made-up console's game executable. */
ENTRY(main)

SECTIONS
{
    . = 0x80003100;
    .text : { *(.text) }
    .rodata : { *(.rodata) *(.rodata.*) }
    .data : { *(.data) }
    .sdata : { *(.sdata) }
    .sbss : { *(.sbss) }
    .bss : { *(.bss) }
    /DISCARD/ : { *(.comment) *(.note.*) *(.gnu.attributes) *(.eh_frame) }
}

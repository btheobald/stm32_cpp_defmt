/* GNU Linker Script for STM32F072RB */
/* Based on CMSIS v5 Linker Script */
/* Byron Theobald - 18 April 2025 */

__STACK_SIZE = 1k;
__HEAP_SIZE  = 0;

MEMORY
{
  FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 128k
  RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 16k
}

ENTRY(Reset_Handler)

SECTIONS {
  .text : {
    KEEP(*(.vectors))
    *(.text*)

    KEEP(*(.init))
    KEEP(*(.fini))

    /* .ctors */
    *crtbegin.o(.ctors)
    *crtbegin?.o(.ctors)
    *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
    *(SORT(.ctors.*))
    *(.ctors)

    /* .dtors */
    *crtbegin.o(.dtors)
    *crtbegin?.o(.dtors)
    *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
    *(SORT(.dtors.*))
    *(.dtors)

    *(.rodata*)

    KEEP(*(.eh_frame*))
  } > FLASH

  .note.gnu.build-id : {
    PROVIDE(__log_build_id = .);
    *(.note.gnu.build-id)
  } > FLASH

  .ARM.extab : {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
  } > FLASH

  __exidx_start = .;
  .ARM.exidx : {
    *(.ARM.exidx* .gnu.linkonce.armexidx.*)
  } > FLASH
  __exidx_end = .;

  .copy.table : {
    . = ALIGN(4);
    __copy_table_start__ = .;

    LONG (__etext)
    LONG (__data_start__)
    LONG ((__data_end__ - __data_start__) / 4)

    __copy_table_end__ = .;
  } > FLASH

  .zero.table : {
    . = ALIGN(4);
    __zero_table_start__ = .;
    __zero_table_end__ = .;
  } > FLASH

  __etext = ALIGN (4);

  .data : AT (__etext) {
    __data_start__ = .;
    *(vtable)
    *(.data)
    *(.data.*)

    . = ALIGN(4);
    /* preinit data */
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP(*(.preinit_array))
    PROVIDE_HIDDEN (__preinit_array_end = .);

    . = ALIGN(4);
    /* init data */
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP(*(SORT(.init_array.*)))
    KEEP(*(.init_array))
    PROVIDE_HIDDEN (__init_array_end = .);

    . = ALIGN(4);
    /* finit data */
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP(*(SORT(.fini_array.*)))
    KEEP(*(.fini_array))
    PROVIDE_HIDDEN (__fini_array_end = .);

    KEEP(*(.jcr*))
    . = ALIGN(4);
    /* All data end */
    __data_end__ = .;

  } > RAM

  .bss : {
    . = ALIGN(4);
    __bss_start__ = .;
    *(.bss)
    *(.bss.*)
    *(COMMON)
    . = ALIGN(4);
    __bss_end__ = .;
  } > RAM AT > RAM

  .heap (COPY) : {
    . = ALIGN(8);
    __end__ = .;
    PROVIDE(end = .);
    . = . + __HEAP_SIZE;
    . = ALIGN(8);
    __HeapLimit = .;
  } > RAM

  .stack (ORIGIN(RAM) + LENGTH(RAM) - __STACK_SIZE) (COPY) : {
    . = ALIGN(8);
    __StackLimit = .;
    . = . + __STACK_SIZE;
    . = ALIGN(8);
    __StackTop = .;
  } > RAM
  PROVIDE(__stack = __StackTop);

  /* Check if data + heap + stack exceeds RAM limit */
  ASSERT(__StackLimit >= __HeapLimit, "Stack overflow into RAM")

  .logs 0 (INFO) : {
    KEEP(*(.logs .logs.*))
  }
}

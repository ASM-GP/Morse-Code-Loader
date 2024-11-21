OUTPUT_FORMAT("elf32-avr")
OUTPUT_ARCH(avr:6)
ENTRY(_start)

MEMORY
{
  text   (rx)   : ORIGIN = 0, LENGTH = 256K
  data   (rw!x) : ORIGIN = 0x800100, LENGTH = 8K
  eeprom (rw!x) : ORIGIN = 0x810000, LENGTH = 4K
  fuse   (rw!x) : ORIGIN = 0x820000, LENGTH = 1K
  lock   (rw!x) : ORIGIN = 0x830000, LENGTH = 1K
  signature (rw!x) : ORIGIN = 0x840000, LENGTH = 1K
}

SECTIONS
{
  .text :
  {
    *(.vectors)
    *(.text*)
    *(.rodata*)
    *(.data*)
    *(.bss*)
    *(.noinit*)
    *(.eeprom*)
    *(.fuse*)
    *(.lock*)
    *(.signature*)
    *(.user_signatures*)
    *(.comment)
    *(.note*)
    *(.debug*)
    *(.stab*)
    *(.stabstr*)
    *(.stack)
    *(.heap)
  } > text

  .data : AT (ADDR(.text) + SIZEOF(.text))
  {
    *(.data)
  } > data

  .bss :
  {
    *(.bss)
  } > data

  .eeprom :
  {
    *(.eeprom)
  } > eeprom

  .fuse :
  {
    *(.fuse)
  } > fuse

  .lock :
  {
    *(.lock)
  } > lock

  .signature :
  {
    *(.signature)
  } > signature
}
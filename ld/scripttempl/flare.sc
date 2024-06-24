# Copyright (C) 2023 Free Software Foundation, Inc.
#
# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.

TORS=".tors :
  {
    ___ctors = . ;
    *(.ctors)
    ___ctors_end = . ;
    ___dtors = . ;
    *(.dtors)
    ___dtors_end = . ;
  } > ram"

cat <<EOF
/* Copyright (C) 2023 Free Software Foundation, Inc.

   Copying and distribution of this script, with or without modification,
   are permitted in any medium without royalty provided the copyright
   notice and this notice are preserved.  */

OUTPUT_FORMAT("${OUTPUT_FORMAT}")
OUTPUT_ARCH(${ARCH})

/*
MEMORY
{
  text (rx) : ORIGIN = 0x0, LENGTH = __TEXT_REGION_LENGTH__
}
*/

address_zero = 0x0;
KEEP (address_zero);

SECTIONS
{
  .text :
  {
    _text_start = . ;
    *(.text)
    ${RELOCATING+KEEP (*(SORT_NONE(.init)))
    KEEP (*(SORT_NONE(.fini)))
    *(.strings)
    _etext = . ; }
  } ${RELOCATING+ > ram}

  ${CONSTRUCTING+${TORS}}

  .data :
  {
    ${RELOCATING+ _data_start = 0x2000 ; }
    *(.data)
    ${RELOCATING+ _edata = . ; }
  } ${RELOCATING+ > ram}

  .bss :
  {
    ${RELOCATING+ _bss_start = . ; }
    *(.bss)
    *(COMMON)
    ${RELOCATING+ _bss_end = . ; }
    ${RELOCATING+ _end = . ; }
  } ${RELOCATING+ > ram}

  /*
  .stack ${RELOCATING+ 0x30000 }  :
  {
    ${RELOCATING+ _stack = . ; }
    *(.stack)
  } ${RELOCATING+ > ram}
  */

  .stab 0 ${RELOCATING+(NOLOAD)} :
  {
    *(.stab)
  }

  .stabstr 0 ${RELOCATING+(NOLOAD)} :
  {
    *(.stabstr)
  }
}
EOF

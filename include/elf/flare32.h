/* flare32 ELF support for BFD.
   Copyright (C) 2023 Free Software Foundation, Inc.
   Written by Andrew Clark (FL4SHK)

   This file is part of BFD, the Binary File Descriptor library.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.  */

#ifndef _ELF_FLARE32_H
#define _ELF_FLARE32_H

#include "elf/reloc-macros.h"

/* Relocation types.  */
START_RELOC_NUMBERS (elf_flare32_reloc_type)
  /* -------- */
  RELOC_NUMBER (R_FLARE32_NONE, 0)
  /* -------- */
  RELOC_NUMBER (R_FLARE32_32, 1)
  /* -------- */
  /* RELOC_NUMBER (R_FLARE32_PRE12, 1) */
  /* RELOC_NUMBER (R_FLARE32_LPRE27, 2) */
  /* RELOC_NUMBER (R_FLARE32_IMM32, 3) */
  /* -------- */
  /* Instruction Groups 1, 5, and 6 */
  RELOC_NUMBER (R_FLARE32_G1G5G6_S5, 2) /* no `pre`/`lpre` */
  RELOC_NUMBER (R_FLARE32_G1G5G6_S17, 3) /* `pre` */
  RELOC_NUMBER (R_FLARE32_G1G5G6_S32, 4) /* `lpre` */
  /* -------- */
  /* Instruction Group 3 */
  RELOC_NUMBER (R_FLARE32_G3_S9_PCREL, 5) /* no `pre`/`lpre` */
  RELOC_NUMBER (R_FLARE32_G3_S21_PCREL, 6) /* `pre` */
  RELOC_NUMBER (R_FLARE32_G3_S32_PCREL, 7) /* `lpre` */
  /* -------- */
END_RELOC_NUMBERS (R_FLARE32_max)

#include "bfd.h"
#include "libbfd.h"
#include "elf-bfd.h"

#endif /* _ELF_FLARE32_H */

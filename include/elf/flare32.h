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
  RELOC_NUMBER (R_FLARE32_8, 1)
  RELOC_NUMBER (R_FLARE32_16, 2)
  RELOC_NUMBER (R_FLARE32_32, 3)
  RELOC_NUMBER (R_FLARE32_64, 4)
  /* -------- */
  /* RELOC_NUMBER (R_FLARE32_PRE12, 1) */
  /* RELOC_NUMBER (R_FLARE32_LPRE27, 2) */
  /* RELOC_NUMBER (R_FLARE32_IMM32, 3) */
  /* -------- */
  /* Instruction Groups 1, 5, and 6 */
  RELOC_NUMBER (R_FLARE32_G1G5G6_S5, 5) /* no `pre`/`lpre` */
  RELOC_NUMBER (R_FLARE32_G1G5G6_S17, 6) /* `pre` */
  RELOC_NUMBER (R_FLARE32_G1G5G6_S32, 7) /* `lpre` */
  //RELOC_NUMBER (R_FLARE32_FDE_G1G5G6_S32_PCREL, 8) /* `.eh_frame` stuff */
  RELOC_NUMBER (R_FLARE32_G1G5G6_S32_ADD32, 8) /* local label sub */
  RELOC_NUMBER (R_FLARE32_G1G5G6_S32_SUB32, 9) /* local label sub */
  RELOC_NUMBER (R_FLARE32_G1G5G6_S32_NO_RELAX, 10) /* no relaxing */
  /* -------- */
  /* Instruction Group 3 */
  RELOC_NUMBER (R_FLARE32_G3_S9_PCREL, 11) /* no `pre`/`lpre` */
  RELOC_NUMBER (R_FLARE32_G3_S21_PCREL, 12) /* `pre` */
  RELOC_NUMBER (R_FLARE32_G3_S32_PCREL, 13) /* `lpre` */
  //RELOC_NUMBER (R_FLARE32_FDE_G3_S32_PCREL, 14) /* `.eh_frame` stuff */
  //RELOC_NUMBER (R_FLARE32_G3_S32_PCREL_ADD32, 14) /* local label sub */
  //RELOC_NUMBER (R_FLARE32_G3_S32_PCREL_SUB32, 15) /* local label sub */
  RELOC_NUMBER (R_FLARE32_G3_S32_PCREL_NO_RELAX, 14) /* no relaxing */
  /* -------- */
  //RELOC_NUMBER (R_FLARE32_FDE_32_PCREL, 15) /* `.eh_frame` stuff */
  /* -------- */
  RELOC_NUMBER (R_FLARE32_SET8, 15)
  RELOC_NUMBER (R_FLARE32_SET16, 16)
  RELOC_NUMBER (R_FLARE32_SET32, 17)
  /* -------- */
  /* Local label subtraction within a pseudo-op */
  RELOC_NUMBER (R_FLARE32_PSEUDO_ADD8, 18)
  RELOC_NUMBER (R_FLARE32_PSEUDO_ADD16, 19)
  RELOC_NUMBER (R_FLARE32_PSEUDO_ADD32, 20)
  RELOC_NUMBER (R_FLARE32_PSEUDO_ADD64, 21)
  RELOC_NUMBER (R_FLARE32_PSEUDO_SUB8, 22)
  RELOC_NUMBER (R_FLARE32_PSEUDO_SUB16, 23)
  RELOC_NUMBER (R_FLARE32_PSEUDO_SUB32, 24)
  RELOC_NUMBER (R_FLARE32_PSEUDO_SUB64, 25)
  /* -------- */
  RELOC_NUMBER (R_FLARE32_CFA_SET6, 26)
  RELOC_NUMBER (R_FLARE32_CFA_SUB6, 27)
  /* -------- */
  RELOC_NUMBER (R_FLARE32_EH_32_PCREL, 28)
  /* -------- */
END_RELOC_NUMBERS (R_FLARE32_max)

#endif /* _ELF_FLARE32_H */

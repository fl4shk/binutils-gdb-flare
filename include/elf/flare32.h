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
  RELOC_NUMBER (R_FLARE32_G1G5G6_U5, 5) /* no `pre`/`lpre` */
  RELOC_NUMBER (R_FLARE32_G1G5G6_S17_FOR_U5, 6) /* `pre` */
  RELOC_NUMBER (R_FLARE32_G1G5G6_S32_FOR_U5, 7) /* `lpre` */
  RELOC_NUMBER (R_FLARE32_G1G5G6_S32_FOR_U5_ADD32, 8) /* local label sub */
  RELOC_NUMBER (R_FLARE32_G1G5G6_S32_FOR_U5_SUB32, 9) /* local label sub */
  RELOC_NUMBER (R_FLARE32_G1G5G6_S32_FOR_U5_NO_RELAX, 10) /* no relaxing */

  RELOC_NUMBER (R_FLARE32_G1G5G6_S5, 11) /* no `pre`/`lpre` */
  RELOC_NUMBER (R_FLARE32_G1G5G6_S17, 12) /* `pre` */
  RELOC_NUMBER (R_FLARE32_G1G5G6_S32, 13) /* `lpre` */
  //RELOC_NUMBER (R_FLARE32_FDE_G1G5G6_S32_PCREL, 14) /* `.eh_frame` stuff */
  RELOC_NUMBER (R_FLARE32_G1G5G6_S32_ADD32, 14) /* local label sub */
  RELOC_NUMBER (R_FLARE32_G1G5G6_S32_SUB32, 15) /* local label sub */
  RELOC_NUMBER (R_FLARE32_G1G5G6_S32_NO_RELAX, 16) /* no relaxing */
  /* -------- */
  /* Instruction Group 3 */
  RELOC_NUMBER (R_FLARE32_G3_S9_PCREL, 17) /* no `pre`/`lpre` */
  RELOC_NUMBER (R_FLARE32_G3_S21_PCREL, 18) /* `pre` */
  RELOC_NUMBER (R_FLARE32_G3_S32_PCREL, 19) /* `lpre` */
  //RELOC_NUMBER (R_FLARE32_FDE_G3_S32_PCREL, 20) /* `.eh_frame` stuff */
  //RELOC_NUMBER (R_FLARE32_G3_S32_PCREL_ADD32, 20) /* local label sub */
  //RELOC_NUMBER (R_FLARE32_G3_S32_PCREL_SUB32, 21) /* local label sub */
  RELOC_NUMBER (R_FLARE32_G3_S32_PCREL_NO_RELAX, 20) /* no relaxing */
  /* -------- */
  //RELOC_NUMBER (R_FLARE32_FDE_32_PCREL, 21) /* `.eh_frame` stuff */
  /* -------- */
  RELOC_NUMBER (R_FLARE32_SET8, 21)
  RELOC_NUMBER (R_FLARE32_SET16, 22)
  RELOC_NUMBER (R_FLARE32_SET32, 23)
  /* -------- */
  /* Local label subtraction within a pseudo-op */
  RELOC_NUMBER (R_FLARE32_PSEUDO_ADD8, 24)
  RELOC_NUMBER (R_FLARE32_PSEUDO_ADD16, 25)
  RELOC_NUMBER (R_FLARE32_PSEUDO_ADD32, 26)
  RELOC_NUMBER (R_FLARE32_PSEUDO_ADD64, 27)
  RELOC_NUMBER (R_FLARE32_PSEUDO_SUB8, 28)
  RELOC_NUMBER (R_FLARE32_PSEUDO_SUB16, 29)
  RELOC_NUMBER (R_FLARE32_PSEUDO_SUB32, 30)
  RELOC_NUMBER (R_FLARE32_PSEUDO_SUB64, 31)
  /* -------- */
  RELOC_NUMBER (R_FLARE32_CFA_SET6, 32)
  RELOC_NUMBER (R_FLARE32_CFA_SUB6, 33)
  /* -------- */
  RELOC_NUMBER (R_FLARE32_EH_32_PCREL, 34)
  /* -------- */
END_RELOC_NUMBERS (R_FLARE32_max)

#endif /* _ELF_FLARE32_H */

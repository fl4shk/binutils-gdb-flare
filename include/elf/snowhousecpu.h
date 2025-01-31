/* snowhousecpu ELF support for BFD.
   Copyright (C) 2025 Free Software Foundation, Inc.
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

#ifndef _ELF_SNOWHOUSECPU_H
#define _ELF_SNOWHOUSECPU_H

#include "elf/reloc-macros.h"

/* Relocation types.  */
START_RELOC_NUMBERS (elf_snowhousecpu_reloc_type)
  /* -------- */
  RELOC_NUMBER (R_SNOWHOUSECPU_NONE, 0)
  /* -------- */
  RELOC_NUMBER (R_SNOWHOUSECPU_8, 1)
  RELOC_NUMBER (R_SNOWHOUSECPU_16, 2)
  RELOC_NUMBER (R_SNOWHOUSECPU_32, 3)
  RELOC_NUMBER (R_SNOWHOUSECPU_64, 4)
  //--------
  RELOC_NUMBER (R_SNOWHOUSECPU_S16, 5)
  RELOC_NUMBER (R_SNOWHOUSECPU_S32_FOR_S16, 6)
  RELOC_NUMBER (R_SNOWHOUSECPU_S32_FOR_S16_ADD32, 7)
  RELOC_NUMBER (R_SNOWHOUSECPU_S32_FOR_S16_SUB32, 8)
  //RELOC_NUMBER (R_SNOWHOUSECPU_S32_FOR_S16_NO_RELAX, 9)
  RELOC_NUMBER (R_SNOWHOUSECPU_SHIFT_U5, 9)
  //--------
  RELOC_NUMBER (R_SNOWHOUSECPU_S16_PCREL, 10)
  RELOC_NUMBER (R_SNOWHOUSECPU_S32_FOR_S16_PCREL, 11)
  //RELOC_NUMBER (R_SNOWHOUSECPU_S32_FOR_S16_PCREL_NO_RELAX, 16)
  /* -------- */
  //RELOC_NUMBER (R_SNOWHOUSECPU_FDE_32_PCREL, 27) /* `.eh_frame` stuff */
  /* -------- */
  RELOC_NUMBER (R_SNOWHOUSECPU_SET8, 12)
  RELOC_NUMBER (R_SNOWHOUSECPU_SET16, 13)
  RELOC_NUMBER (R_SNOWHOUSECPU_SET32, 14)
  /* -------- */
  /* Local label subtraction within a pseudo-op */
  RELOC_NUMBER (R_SNOWHOUSECPU_PSEUDO_ADD8, 15)
  RELOC_NUMBER (R_SNOWHOUSECPU_PSEUDO_ADD16, 16)
  RELOC_NUMBER (R_SNOWHOUSECPU_PSEUDO_ADD32, 17)
  RELOC_NUMBER (R_SNOWHOUSECPU_PSEUDO_ADD64, 18)
  RELOC_NUMBER (R_SNOWHOUSECPU_PSEUDO_SUB8, 19)
  RELOC_NUMBER (R_SNOWHOUSECPU_PSEUDO_SUB16, 20)
  RELOC_NUMBER (R_SNOWHOUSECPU_PSEUDO_SUB32, 21)
  RELOC_NUMBER (R_SNOWHOUSECPU_PSEUDO_SUB64, 22)
  /* -------- */
  RELOC_NUMBER (R_SNOWHOUSECPU_CFA_SET6, 23)
  RELOC_NUMBER (R_SNOWHOUSECPU_CFA_SUB6, 24)
  /* -------- */
  RELOC_NUMBER (R_SNOWHOUSECPU_EH_32_PCREL, 25)
  /* -------- */
END_RELOC_NUMBERS (R_SNOWHOUSECPU_max)

#endif /* _ELF_SNOWHOUSECPU_H */

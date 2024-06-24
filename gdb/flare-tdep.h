/* Target-dependent code for Flare
 
   Copyright (C) 2023 Free Software Foundation, Inc.

   This file is part of GDB.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#ifndef FLARE_TDEP_H
#define FLARE_TDEP_H

#include "gdbarch.h"
#include "opcode/flare.h"

struct flare_gdbarch_tdep : gdbarch_tdep_base
{
  /* gdbarch target dependent data here.  Currently unused for FLARE.  */
};

enum flare_regnum
{
  FLARE_FP_REGNUM = FLARE_GPR_ENUM_FP,
  FLARE_SP_REGNUM = FLARE_GPR_ENUM_SP,
  R0_REGNUM = FLARE_GPR_ENUM_R0,
  R1_REGNUM = FLARE_GPR_ENUM_R1,
  FLARE_PC_REGNUM = FLARE_TOTAL_NUM_NON_PC_REGS,
  FLARE_FLAGS_REGNUM
    = FLARE_SIM_FIRST_SPR_REGNO + FLARE_SPR_ENUM_FLAGS,
  RET1_REGNUM = R0_REGNUM,
  ARG1_REGNUM = R0_REGNUM,
  ARGN_REGNUM = R1_REGNUM,
};

#define FLARE_NUM_REGS (FLARE_TOTAL_NUM_REGS)

#endif /* flare-tdep.h */

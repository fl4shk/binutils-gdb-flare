/* Flare32 Simulator definition.
   Copyright (C) 2023 Free Software Foundation, Inc.
   Contributed by Andrew Clark (FL4SHK)

This file is part of GDB, the GNU debugger.

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

#ifndef SIM_MAIN_H
#define SIM_MAIN_H

#include <stdint.h>
#include "opcode/flare32.h"
#include "sim-basics.h"
#include "sim-base.h"

typedef struct flare32_regset_t
{
  int32_t
    gprs[FLARE32_NUM_GPRS],
    sprs[FLARE32_NUM_SPRS];
  uint32_t pc;
} flare32_regset_t;

typedef union flare32_cpu_t
{
  flare32_regset_t asregs;
  int32_t asints[1];    /* accessed as larger values */
} flare32_cpu_t;

#endif    /* SIM_MAIN_H */

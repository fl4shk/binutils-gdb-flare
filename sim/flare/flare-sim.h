/* Flare Simulator definition.
   Copyright (C) 2023 Free Software Foundation, Inc.
   Contributed by Andrew Clark (FL4SHK)

This file is part of the GNU simulators.

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

#ifndef FLARE_SIM_H
#define FLARE_SIM_H

#include "opcode/flare.h"

#define PCIDX (FLARE_NUM_GPRS + FLARE_NUM_SPRS)

struct flare_sim_cpu_t
{
  unsigned_word regs[FLARE_TOTAL_NUM_REGS];
};

#define FLARE_SIM_CPU(cpu)\
  ((struct flare_sim_cpu_t *) CPU_ARCH_DATA (cpu))

#endif    /* FLARE_SIM_H */

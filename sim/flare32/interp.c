/* Flare32 simulator.

   Copyright (C) 2023 Free Software Foundation, Inc.
   Contributed by Andrew Clark (FL4SHK).

   This file is part of simulators.

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

/* This must come before any other includes.  */
#include "defs.h"

#include "bfd.h"

#include "sim/callback.h"
#include "sim-main.h"
#include "sim-options.h"
//#include "target-newlib-syscall.h"

#include "flare32-sim.h"

static const flare32_reg_t gpr_info[FLARE32_NUM_GPRS]
  = FLARE32_INST_GPRS ();
static const flare32_reg_t spr_info[FLARE32_NUM_SPRS]
  = FLARE32_INST_SPRS ();
static const flare32_reg_t pc_info
  = FLARE32_INST_REG_PC ();

static flare32_cpu_t cpu;

static int
flare32_reg_store (SIM_CPU *scpu, int rn, const void *memory, int length)
{
  if (rn < NUM_MOXIE_REGS && rn >= 0)
  {
    if (length == 4)
    {
      long ival;
      
      /* misalignment safe */
      ival = flare32_extract_unsigned_integer (memory, 4);
      cpu.asints[rn] = ival;
    }
    return 4;
  }
  else
    return 0;
}

static int
flare32_reg_fetch (SIM_CPU *scpu, int rn, void *memory, int length)
{
  if (rn < NUM_MOXIE_REGS && rn >= 0)
  {
    if (length == 4)
    {
      int32_t ival = cpu.asints[rn];

      /* misalignment-safe */
      flare32_store_unsigned_integer (memory, 4, ival);
    }
    
    return 4;
  }
  else
    return 0;
}

static sim_cia
flare32_pc_get (sim_cpu *cpu)
{
  return FLARE32_SIM_CPU (cpu)->regs[PCIDX];
}

static void
flare32_pc_set (sim_cpu *cpu, sim_cia pc)
{
  FLARE32_SIM_CPU (cpu)->regs[PCIDX] = pc;
}


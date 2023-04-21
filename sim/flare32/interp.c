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

#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <unistd.h>

#include "bfd.h"
#include "libiberty.h"
#include "sim/sim.h"

#include "sim-main.h"
#include "sim-base.h"
#include "sim-options.h"
#include "sim-io.h"
#include "sim-signal.h"
#include "sim/callback.h"
#include "target-newlib-syscall.h"

#include "flare32-sim.h"
/* -------- */
#include "opcode/flare32-opc-decls.h"
/* -------- */
//static const flare32_reg_t gpr_info[FLARE32_NUM_GPRS]
//  = FLARE32_INST_GPRS ();
//static const flare32_reg_t spr_info[FLARE32_NUM_SPRS]
//  = FLARE32_INST_SPRS ();
//static const flare32_reg_t pc_info
//  = FLARE32_INST_REG_PC ();

static flare32_cpu_t cpu;

void
flare32_cpu_set_asint (flare32_cpu_t *self, unsigned rn, int32_t value)
{
  unsigned char *temp_self = (unsigned char *)self;
  memcpy (temp_self + rn * sizeof (value), &value, sizeof (value));
}
int32_t
flare32_cpu_get_asint (flare32_cpu_t *self, unsigned rn)
{
  int32_t ret = 0;
  unsigned char *temp_self = (unsigned char *)self;
  memcpy (&ret, temp_self + rn * sizeof (ret), sizeof (ret));
  return ret;
}

static void
set_initial_gprs (void)
{
  unsigned i;
  long space;
  
  /* Set up machine just out of reset.  */
  cpu.pc = 0;
  
  /* Clean out the register contents.  */
  for (i=0; i<FLARE32_NUM_GPRS; ++i)
  {
    cpu.gprs[i] = 0;
  }
  for (i=0; i<FLARE32_NUM_SPRS; ++i)
  {
    cpu.sprs[i] = 0;
  }
}
/* -------- */
static unsigned long
flare32_extract_unsigned_integer (const unsigned char *addr, int len)
{
  unsigned long retval;
  unsigned char *p;
  unsigned char *startaddr = (unsigned char *) addr;
  unsigned char *endaddr = startaddr + len;
 
  if (len > (int) sizeof (unsigned long))
  {
    printf ("That operation is not available on integers of more than "
      "%zu bytes.",
      sizeof (unsigned long));
  }
 
  /* Start at the most significant end of the integer, and work towards
     the least significant.  */
  retval = 0;

  for (p = endaddr; p > startaddr;)
  {
    retval = (retval << 8) | * -- p;
  }
  
  return retval;
}

/* -------- */
/* Write a 1 byte value to memory.  */
static INLINE void
wr8 (sim_cpu *scpu, int32_t pc, int32_t x, int32_t v)
{
  address_word cia = CPU_PC_GET (scpu);
  
  sim_core_write_aligned_1 (scpu, cia, write_map, x, v);
}

/* Write a 2 byte value to memory.  */
static INLINE void
wr16 (sim_cpu *scpu, int32_t pc, int32_t x, int32_t v)
{
  address_word cia = CPU_PC_GET (scpu);
  sim_core_write_aligned_2 (scpu, cia, write_map, x, v);
}

/* Write a 4 byte value to memory.  */
static INLINE void
wr32 (sim_cpu *scpu, int32_t pc, int32_t x, int32_t v)
{
  address_word cia = CPU_PC_GET (scpu);
  sim_core_write_aligned_4 (scpu, cia, write_map, x, v);
}

/* Read 1 byte from memory.  */
static INLINE int32_t
rd8s (sim_cpu *scpu, int32_t pc, int32_t x)
{
  address_word cia = CPU_PC_GET (scpu);
  
  return (sim_core_read_aligned_1 (scpu, cia, read_map, x));
}
static INLINE uint32_t
rd8u (sim_cpu *scpu, int32_t pc, int32_t x)
{
  return flare32_zero_extend (rd8s (scpu, pc, x), 8u);
}

/* Read 2 bytes from memory.  */
static INLINE int32_t
rd16s (sim_cpu *scpu, int32_t pc, int32_t x)
{
  address_word cia = CPU_PC_GET (scpu);
  
  return (sim_core_read_aligned_2 (scpu, cia, read_map, x));
}
static INLINE uint32_t
rd16u (sim_cpu *scpu, int32_t pc, int32_t x)
{
  return flare32_zero_extend (rd16s (scpu, pc, x), 16u);
}

/* Read 4 bytes from memory.  */
static INLINE int32_t
rd32 (sim_cpu *scpu, int32_t pc, int32_t x)
{
  address_word cia = CPU_PC_GET (scpu);
  
  return (sim_core_read_aligned_4 (scpu, cia, read_map, x));
}
/* -------- */
static void
flare32_store_unsigned_integer (unsigned char *addr, int len,
  unsigned long val)
{
  unsigned char *p;
  unsigned char *startaddr = (unsigned char *) addr;
  unsigned char *endaddr = startaddr + len;

  for (p = endaddr; p > startaddr;)
  {
    * -- p = val & 0xff;
    val >>= 8;
  }
}
static int
flare32_reg_store (SIM_CPU *scpu, int rn, const void *memory, int length)
{
  if (rn < FLARE32_TOTAL_NUM_NON_PC_REGS && rn >= 0)
  {
    if (length == 4)
    {
      int32_t ival;
      
      /* misalignment safe */
      ival = flare32_extract_unsigned_integer (memory, 4);
      //cpu.asints[rn] = ival;
      flare32_cpu_set_asint (&cpu, rn, ival);
    }
    return 4;
  }
  else
  {
    return 0;
  }
}

static int
flare32_reg_fetch (SIM_CPU *scpu, int rn, void *memory, int length)
{
  if (rn < FLARE32_TOTAL_NUM_NON_PC_REGS && rn >= 0)
  {
    if (length == 4)
    {
      //int32_t ival = cpu.asints[rn];
      int32_t ival = flare32_cpu_get_asint (&cpu, rn);

      /* misalignment-safe */
      flare32_store_unsigned_integer (memory, 4, ival);
    }
    
    return 4;
  }
  else
  {
    return 0;
  }
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

static void
free_state (SIM_DESC sd)
{
  if (STATE_MODULES (sd) != NULL)
  {
    sim_module_uninstall (sd);
  }
  sim_cpu_free_all (sd);
  sim_state_free (sd);
}

SIM_DESC
sim_open (SIM_OPEN_KIND kind, host_callback *cb,
  struct bfd *abfd, char * const *argv)
{
  int i;
  SIM_DESC sd = sim_state_alloc (kind, cb);
  SIM_ASSERT (STATE_MAGIC (sd) == SIM_MAGIC_NUMBER);

  /* Set default options before parsing user options.  */
  current_target_byte_order = BFD_ENDIAN_BIG;

  /* The CPU data is kept in a separately allocated chunk of memory.  */
  if (sim_cpu_alloc_all_extra (sd, 0, sizeof (struct flare32_sim_cpu_t))
    != SIM_RC_OK)
  {
    free_state (sd);
    return 0;
  }

  if (sim_pre_argv_init (sd, argv[0]) != SIM_RC_OK)
  {
    free_state (sd);
    return 0;
  }

  /* The parser will print an error message for us, so we silently return.  */
  if (sim_parse_args (sd, argv) != SIM_RC_OK)
  {
    free_state (sd);
    return 0;
  }

  sim_do_command (sd, " memory region 0x00000000,0x40000000") ; 
  sim_do_command (sd, " memory region 0xE0000000,0x10000") ; 

  /* Check for/establish the a reference program image.  */
  if (sim_analyze_program (sd, STATE_PROG_FILE (sd), abfd) != SIM_RC_OK)
  {
    free_state (sd);
    return 0;
  }

  /* Configure/verify the target byte order and other runtime
     configuration options.  */
  if (sim_config (sd) != SIM_RC_OK)
  {
    sim_module_uninstall (sd);
    return 0;
  }

  if (sim_post_argv_init (sd) != SIM_RC_OK)
  {
    /* Uninstall the modules to avoid memory leaks,
    file descriptor leaks, etc.  */
    sim_module_uninstall (sd);
    return 0;
  }

  /* CPU specific initialization.  */
  for (i=0; i<MAX_NR_PROCESSORS; ++i)
  {
    SIM_CPU *cpu = STATE_CPU (sd, i);

    CPU_REG_FETCH (cpu) = flare32_reg_fetch;
    CPU_REG_STORE (cpu) = flare32_reg_store;
    CPU_PC_FETCH (cpu) = flare32_pc_get;
    CPU_PC_STORE (cpu) = flare32_pc_set;

    set_initial_gprs ();  /* Reset the GPR registers.  */
  }

  return sd;
}
/* -------- */
#define CHECK_FLAG(T, H) \
  if (tflags & T) \
  { \
    hflags |= H; \
    tflags ^= T; \
  }
//#define CHECK_FLAG(T,H) if (tflags & T) { hflags |= H; tflags ^= T; }

static unsigned int
convert_target_flags (unsigned int tflags)
{
  unsigned int hflags = 0x0;

  CHECK_FLAG (0x0001, O_WRONLY);
  CHECK_FLAG (0x0002, O_RDWR);
  CHECK_FLAG (0x0008, O_APPEND);
  CHECK_FLAG (0x0200, O_CREAT);
  CHECK_FLAG (0x0400, O_TRUNC);
  CHECK_FLAG (0x0800, O_EXCL);
  CHECK_FLAG (0x2000, O_SYNC);

  if (tflags != 0x0)
  {
    fprintf (stderr, 
      "Simulator Error: problem converting target open flags for host.  "
      "0x%x\n", 
      tflags);
  }

  return hflags;
}
/* TODO: Split this up into finger trace levels than just insn.  */
#define FLARE32_TRACE_INSN(str) \
  TRACE_INSN (scpu, \
    "0x%08x %s; " \
    "pc=0x%x; " \
    "r0=0x%x r1=0x%x r2=0x%x} r3=0x%x " \
      "r4=0x%x r5=0x%x r6=0x%x r7=0x%x " \
      "r8=0x%x r9=0x%x r10=0x%x r11=0x%x " \
      "r12=0x%x lr=0x%x fp=0x%x sp=0x%x; " \
    "flags=0x%x hi=0x%x lo=0x%x " \
    "ids=0x%x ira=0x%x ie=0x%x ity=0x%x sty=0x%x", \
  opc, str, \
  /* -------- */ \
  cpu.pc, \
  /* -------- */ \
  cpu.gprs[FLARE32_GPR_ENUM_R0], cpu.gprs[FLARE32_GPR_ENUM_R1], \
  cpu.gprs[FLARE32_GPR_ENUM_R2], cpu.gprs[FLARE32_GPR_ENUM_R3], \
  cpu.gprs[FLARE32_GPR_ENUM_R4], cpu.gprs[FLARE32_GPR_ENUM_R5], \
  cpu.gprs[FLARE32_GPR_ENUM_R6], cpu.gprs[FLARE32_GPR_ENUM_R7], \
  cpu.gprs[FLARE32_GPR_ENUM_R8], cpu.gprs[FLARE32_GPR_ENUM_R9], \
  cpu.gprs[FLARE32_GPR_ENUM_R10], cpu.gprs[FLARE32_GPR_ENUM_R11], \
  cpu.gprs[FLARE32_GPR_ENUM_R12], cpu.gprs[FLARE32_GPR_ENUM_LR], \
  cpu.gprs[FLARE32_GPR_ENUM_FP], cpu.gprs[FLARE32_GPR_ENUM_SP], \
  /* -------- */ \
  cpu.sprs[FLARE32_SPR_ENUM_FLAGS], \
  cpu.sprs[FLARE32_SPR_ENUM_HI], cpu.sprs[FLARE32_SPR_ENUM_LO], \
  cpu.sprs[FLARE32_SPR_ENUM_IDS], cpu.sprs[FLARE32_SPR_ENUM_IDS], \
  cpu.sprs[FLARE32_SPR_ENUM_IE], \
  cpu.sprs[FLARE32_SPR_ENUM_ITY], cpu.sprs[FLARE32_SPR_ENUM_STY])


static INLINE void
flare32_sim_set_flags_zn (uint32_t bits,
                          uint64_t result,
                          int32_t *flags_out)
{
  uint64_t
    temp_flags_z_mask = FLARE32_SIM_FLAGS_Z_MASK (bits),
    temp_flags_vn_mask = FLARE32_SIM_FLAGS_VN_MASK (bits);

  /* set the `Z` flag */
  if (result & temp_flags_z_mask)
  {
    *flags_out |= FLARE32_FLAGS_Z_MASK;
  }
  else
  {
    *flags_out &= ~FLARE32_FLAGS_Z_MASK;
  }

  /* set the `N` flag */
  if (result & temp_flags_vn_mask)
  {
    *flags_out |= FLARE32_FLAGS_N_MASK;
  }
  else
  {
    *flags_out &= ~FLARE32_FLAGS_N_MASK;
  }
}
/* Returns the sum/difference of the `add`/`sub`/`cmp`/`cmpb`/`cmph`
  Note: `NULL` `flags_out` indicates don't compute output flags */
static INLINE int32_t
flare32_sim_add_sub (uint32_t bits,
                    int32_t operand_a,
                    int32_t operand_b,
                    int32_t flags_in,
                    int32_t *flags_out, 
                    bool with_carry_in,
                    bool do_sub)
{
  uint64_t
    ret = 0,
    temp_operand_a = operand_a,
    temp_operand_b = operand_b,
    temp_flags_c_mask = 0,
    temp_flags_vn_mask = 0;

  if (!do_sub)
  {
    ret = temp_operand_a + temp_operand_b
      + (with_carry_in
        ? ((flags_in & FLARE32_FLAGS_C_MASK) >> FLARE32_FLAGS_C_BITPOS)
        : 0x0ull);
  }
  else // if (do_sub)
  {
    /* 6502-style subtraction */
    ret = temp_operand_a + (~temp_operand_b)
      + (with_carry_in 
        ? ((flags_in & FLARE32_FLAGS_C_MASK) >> FLARE32_FLAGS_C_BITPOS)
        : 0x1ull);
  }

  if (flags_out != NULL)
  {
    temp_flags_vn_mask = FLARE32_SIM_FLAGS_VN_MASK (bits);
    temp_flags_c_mask = FLARE32_SIM_FLAGS_C_MASK (bits);

    *flags_out = 0x0;
    flare32_sim_set_flags_zn (bits, ret, flags_out);

    /* set the `C` flag */
    if (ret & temp_flags_c_mask)
    {
      *flags_out |= FLARE32_FLAGS_C_MASK;
    }
    /* set the `V` flag (6502-style) */
    //if (!((temp_operand_a ^ temp_operand_b) & temp_flags_vn_mask)
    //  && ((temp_operand_a ^ ret) & temp_flags_vn_mask))
    /* The above ^ commented-out method is equivalent, but slower. */
    if ((temp_operand_a ^ ret) & (temp_operand_b ^ ret)
      & temp_flags_vn_mask)
    {
      *flags_out |= FLARE32_FLAGS_V_MASK;
    }
  }

  return (int32_t) ret;
}

void
sim_engine_run (SIM_DESC sd,
                int next_cpu_nr, /* ignore  */
                int nr_cpus, /* ignore  */
                int siggnal) /* ignore  */
{
  int32_t pc, opc;
  bool have_index_insn, have_pre_insn, have_lpre_insn;
  flare32_temp_t
    index_insn, prefix_insn, insn,
    grp, subgrp, ra_ind, rb_ind, rc_ind, simm, fw,
    nbytes;
  const flare32_opc_info_t
    *opc_info;
  sim_cpu *scpu = STATE_CPU (sd, 0); /* FIXME */
  address_word cia = CPU_PC_GET (scpu);

  pc = cpu.pc;

  /* Run instructions here. */
  do 
  {
    opc = pc;
    have_index_insn = false;
    have_pre_insn = false;
    have_lpre_insn = false;
    index_insn = prefix_insn = insn = 0x0;
    grp = subgrp = ra_ind = rb_ind = rc_ind = simm = fw = 0x0;
    nbytes = 0x0;
    opc_info = (const flare32_opc_info_t *) NULL;

    /* Fetch the instruction at pc.  */
    index_insn = prefix_insn = insn
      = (sim_core_read_aligned_1 (scpu, cia, read_map, pc + nbytes) << 8)
        | sim_core_read_aligned_1 (scpu, cia, read_map, pc + nbytes + 1);

    grp = flare32_get_insn_field_ei (&flare32_enc_info_grp_16, insn);

    if ((have_index_insn = (
      flare32_get_insn_field_ei (&flare32_enc_info_grp_16, index_insn)
        == FLARE32_G4_GRP_VALUE
      && flare32_get_insn_field_ei (&flare32_enc_info_g4_op, index_insn)
        == FLARE32_G4_OP_ENUM_INDEX_RA
      ))
    )
    {
      nbytes += 2;
      rc_ind = flare32_get_insn_field_ei
        (&flare32_enc_info_ra_ind, index_insn);
      prefix_insn = insn
        = (sim_core_read_aligned_1 (scpu, cia, read_map, pc + nbytes) << 8)
        | sim_core_read_aligned_1 (scpu, cia, read_map, pc + nbytes + 1);

      grp = flare32_get_insn_field_ei (&flare32_enc_info_grp_16, insn);
    }

    if ((have_pre_insn = (
      flare32_get_insn_field_ei
        (&flare32_enc_info_g0_pre_fullgrp, prefix_insn)
        == FLARE32_G0_PRE_FULLGRP_VALUE
      ))
    )
    {
      nbytes += 2;
      simm = flare32_sign_extend (flare32_get_insn_field_ei
        (&flare32_enc_info_g0_pre_s12, prefix_insn),
        flare32_enc_info_g0_pre_s12.bitsize);

      insn
        = (sim_core_read_aligned_1 (scpu, cia, read_map, pc + nbytes) << 8)
        | sim_core_read_aligned_1 (scpu, cia, read_map, pc + nbytes + 1);

      grp = flare32_get_insn_field_ei (&flare32_enc_info_grp_16, insn);
    }
    else if ((have_lpre_insn = (
      flare32_get_insn_field_ei
        (&flare32_enc_info_g0_lpre_fullgrp_16, prefix_insn)
        == FLARE32_G0_LPRE_FULLGRP_VALUE
      ))
    )
    {
      nbytes += 2;
      prefix_insn = (prefix_insn << 16)
        | (sim_core_read_aligned_1 (scpu, cia, read_map, pc + nbytes) << 8)
        | sim_core_read_aligned_1 (scpu, cia, read_map, pc + nbytes + 1);

      nbytes += 2;
      insn
        = (sim_core_read_aligned_1 (scpu, cia, read_map, pc + nbytes) << 8)
        | sim_core_read_aligned_1 (scpu, cia, read_map, pc + nbytes + 1);

      switch (grp = flare32_get_insn_field_ei
        (&flare32_enc_info_grp_16, insn))
      {
        case FLARE32_G1_GRP_VALUE:
        case FLARE32_G5_GRP_VALUE:
        case FLARE32_G6_GRP_VALUE:
          simm = flare32_sign_extend (flare32_get_insn_field_ei
            (&flare32_enc_info_g1g5g6_g0_lpre_s27, prefix_insn),
            flare32_enc_info_g1g5g6_g0_lpre_s27.bitsize);
          break;
        case FLARE32_G3_GRP_VALUE:
          simm = flare32_sign_extend (flare32_get_insn_field_ei
            (&flare32_enc_info_g3_g0_lpre_s23, prefix_insn),
            flare32_enc_info_g3_g0_lpre_s23.bitsize);
          break;
        default:
          simm = 0x0;
          break;
      }
    }

    nbytes += 2;

    /* Decode instruction.  */
    ra_ind = flare32_get_insn_field_ei (&flare32_enc_info_ra_ind, insn);
    rb_ind = flare32_get_insn_field_ei (&flare32_enc_info_rb_ind, insn);
    switch (grp)
    {
      case FLARE32_G0_GRP_VALUE:
      {
        FLARE32_TRACE_INSN ("SIGILL_G0");
        sim_engine_halt (sd, scpu, NULL, pc, sim_stopped, SIM_SIGILL);
      }
        break;
      case FLARE32_G1_GRP_VALUE:
      {
        int32_t
          *ra = &cpu.gprs[ra_ind],
          fp = cpu.gprs[FLARE32_GPR_ENUM_FP],
          sp = cpu.gprs[FLARE32_GPR_ENUM_SP],
          temp_flags_in = cpu.sprs[FLARE32_SPR_ENUM_FLAGS],
          *flags = &cpu.sprs[FLARE32_SPR_ENUM_FLAGS],
          //ids = cpu.sprs[FLARE32_SPR_ENUM_IDS],
          //ira = cpu.sprs[FLARE32_SPR_ENUM_IRA],
          *ity = &cpu.sprs[FLARE32_SPR_ENUM_ITY],
          *sty = &cpu.sprs[FLARE32_SPR_ENUM_STY];

        opc_info = &flare32_opc_info_g1
          [flare32_get_insn_field_ei (&flare32_enc_info_g1_op, insn)];
        simm = (simm << flare32_enc_info_g1g5g6_s5.bitsize)
          | flare32_get_insn_field_ei (&flare32_enc_info_g1g5g6_s5, insn);

        switch (opc_info->opcode)
        {
          case FLARE32_G1_OP_ENUM_ADD_RA_S5:
          {
            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *ra = flare32_sim_add_sub 
              (32u, /* bits */
              *ra, /* operand_a */
              (int32_t) simm, /* operand_b */
              temp_flags_in, /* flags_in */
              NULL, /* flags_out */ 
              false, /* with_carry_in */
              false /* do_sub */
              );
          }
            break;
          case FLARE32_G1_OP_ENUM_ADD_RA_PC_S5:
          {
            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *ra = flare32_sim_add_sub 
              (32u, /* bits */
              (int32_t) (pc + nbytes), /* operand_a */
              (int32_t) simm, /* operand_b */
              temp_flags_in, /* flags_in */
              NULL, /* flags_out */ 
              false, /* with_carry_in */
              false /* do_sub */
              );
          }
            break;
          case FLARE32_G1_OP_ENUM_ADD_RA_SP_S5:
          {
            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *ra = flare32_sim_add_sub 
              (32u, /* bits */
              (int32_t) sp, /* operand_a */
              (int32_t) simm, /* operand_b */
              temp_flags_in, /* flags_in */
              NULL, /* flags_out */ 
              false, /* with_carry_in */
              false /* do_sub */
              );
          }
            break;
          case FLARE32_G1_OP_ENUM_ADD_RA_FP_S5:
          {
            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *ra = flare32_sim_add_sub 
              (32u, /* bits */
              (int32_t) fp, /* operand_a */
              (int32_t) simm, /* operand_b */
              temp_flags_in, /* flags_in */
              NULL, /* flags_out */ 
              false, /* with_carry_in */
              false /* do_sub */
              );
          }
            break;
          case FLARE32_G1_OP_ENUM_CMP_RA_S5:
          {
            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *ra = flare32_sim_add_sub 
              (32u, /* bits */
              *ra, /* operand_a */
              (int32_t) simm, /* operand_b */
              temp_flags_in, /* flags_in */
              flags, /* flags_out */ 
              false, /* with_carry_in */
              true /* do_sub */
              );
          }
            break;
          case FLARE32_G1_OP_ENUM_CPY_RA_S5:
          {
            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *ra = simm;
          }
            break;
          case FLARE32_G1_OP_ENUM_LSL_RA_S5:
          {
            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *ra = (uint32_t) (*ra) << (uint32_t) simm;
          }
            break;
          case FLARE32_G1_OP_ENUM_LSR_RA_S5:
          {
            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *ra = (uint32_t) (*ra) >> (uint32_t) simm;
          }
            break;
          case FLARE32_G1_OP_ENUM_ASR_RA_S5:
          {
            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *ra = (int32_t) (*ra) >> (uint32_t) simm;
          }
            break;
          case FLARE32_G1_OP_ENUM_AND_RA_S5:
          {
            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *ra &= simm;
          }
            break;
          case FLARE32_G1_OP_ENUM_ORR_RA_S5:
          {
            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *ra |= simm;
          }
            break;
          case FLARE32_G1_OP_ENUM_XOR_RA_S5:
          {
            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *ra ^= simm;
          }
            break;
          case FLARE32_G1_OP_ENUM_ZE_RA_S5:
          {
            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *ra = (int32_t) flare32_zero_extend (*ra, simm);
          }
            break;
          case FLARE32_G1_OP_ENUM_SE_RA_S5:
          {
            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *ra = (int32_t) flare32_sign_extend (*ra, simm);
          }
            break;
          case FLARE32_G1_OP_ENUM_SWI_RA_S5:
          case FLARE32_G1_OP_ENUM_SWI_S5:
          {
            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *ity = 0x1;
            *sty
              = (opc_info->opcode == FLARE32_G1_OP_ENUM_SWI_RA_S5
                ? *ra 
                : 0x0)
                + simm;

            switch ((uint32_t) (*sty))
            {
              case TARGET_NEWLIB_SYS_exit:
              {
                int32_t
                  r0 = cpu.gprs[FLARE32_GPR_ENUM_R0];

                sim_engine_halt (sd, scpu, NULL, pc, sim_exited, r0);
              }
                break;
              case TARGET_NEWLIB_SYS_open:
              {
                int32_t
                  *r0 = &cpu.gprs[FLARE32_GPR_ENUM_R0],
                  r1 = cpu.gprs[FLARE32_GPR_ENUM_R1],
                  r2 = cpu.gprs[FLARE32_GPR_ENUM_R2];

                char fname[1024];
                int
                  mode
                    = (int) convert_target_flags ((unsigned) r1),
                    //= (int) r1,
                  perm = (int) r2,
                  fd;
                sim_core_read_buffer (sd, scpu, read_map, fname, *r0,
                  1024);
                fd = sim_io_open (sd, fname, mode);
                printf ("%d; opened \"%s\"\n",
                  fd, fname);

                /* FIXME - set errno */
                *r0 = fd;
              }
                break;
              case TARGET_NEWLIB_SYS_close:
              {
                int32_t
                  *r0 = &cpu.gprs[FLARE32_GPR_ENUM_R0];

                int
                  fd = *r0,
                  rv;

                if (fd > 2)
                {
                  rv = sim_io_close (sd, fd);
                }
                else
                {
                  rv = 0;
                }

                *r0 = rv;
              }
                break;
              case TARGET_NEWLIB_SYS_read:
              {
                int32_t
                  *r0 = &cpu.gprs[FLARE32_GPR_ENUM_R0],
                  r1 = cpu.gprs[FLARE32_GPR_ENUM_R1],
                  r2 = cpu.gprs[FLARE32_GPR_ENUM_R2];

                int
                  fd = *r0,
                  read_ret = 0;
                unsigned len = (unsigned) r2;

                char *buf = malloc (len);
                read_ret = sim_io_read (sd, fd, buf, len);
                sim_core_write_buffer (sd, scpu, write_map, buf, r1, len);
                free (buf);

                *r0 = read_ret;
              }
                break;
              case TARGET_NEWLIB_SYS_write:
              {
                int32_t
                  *r0 = &cpu.gprs[FLARE32_GPR_ENUM_R0],
                  r1 = cpu.gprs[FLARE32_GPR_ENUM_R1],
                  r2 = cpu.gprs[FLARE32_GPR_ENUM_R2];

                char *str;
                unsigned
                  count,
                  len = (unsigned) r2;
                str = malloc (len);
                sim_core_read_buffer (sd, scpu, read_map, str, r1, len);
                count = sim_io_write (sd, *r0, str, len);
                free (str);

                *r0 = count;
              }
                break;
              case TARGET_NEWLIB_SYS_unlink:
              {
                int32_t
                  *r0 = &cpu.gprs[FLARE32_GPR_ENUM_R0];

                char
                  fname[1024];
                  int fd;
                sim_core_read_buffer
                  (sd, scpu, read_map, fname, *r0, 1024);
                fd = sim_io_unlink (sd, fname);

                /* FIXME - set errno */
                *r0 = fd;
              }
                break;
              //case 0xffffffff: /* Linux System Call */
              //{
              //  unsigned int handler = cpu.asregs.sregs[1];
              //  unsigned int sp = cpu.gprs[FLARE32_GPR_ENUM_SP];

              //  /* Save a slot for the static chain.  */
              //  sp -= 4;

              //  /* Push the return address.  */
              //  sp -= 4;
              //  wr32 (scpu, opc, sp, pc + 6);
          
              //  /* Push the current frame pointer.  */
              //  sp -= 4;
              //  wr32 (scpu, opc, sp, cpu.gprs[FLARE32_GPR_ENUM_FP]);

              //  /* Uncache the stack pointer and set the fp & pc.  */
              //  cpu.gprs[FLARE32_GPR_ENUM_SP] = sp;
              //  cpu.gprs[FLARE32_GPR_ENUM_FP] = sp;
              //  pc = handler - 6;
              //}
              default:
                break;
            }
          }
            break;
          default:
          {
            FLARE32_TRACE_INSN ("SIGILL_G1");
            sim_engine_halt (sd, scpu, NULL, pc, sim_stopped, SIM_SIGILL);
          }
            break;
        }
      }
        break;
      case FLARE32_G2_GRP_VALUE:
      {
        int32_t
          *ra = &cpu.gprs[ra_ind],
          rb = cpu.gprs[rb_ind],
          fp = cpu.gprs[FLARE32_GPR_ENUM_FP],
          sp = cpu.gprs[FLARE32_GPR_ENUM_SP],
          temp_flags_in = cpu.sprs[FLARE32_SPR_ENUM_FLAGS],
          *flags = &cpu.sprs[FLARE32_SPR_ENUM_FLAGS];

        opc_info = &flare32_opc_info_g2
          [flare32_get_insn_field_ei (&flare32_enc_info_g2_op, insn)];
        fw = flare32_get_insn_field_ei (&flare32_enc_info_g2_f, insn);

        switch (opc_info->opcode)
        {
          case FLARE32_G2_OP_ENUM_ADD_RA_RB:
          {
            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *ra = flare32_sim_add_sub
              (32u, /* bits */
              *ra, /* operand_a */
              rb, /* operand_b */
              temp_flags_in, /* flags_in */
              fw ? flags : NULL, /* flags out */
              false, /* with_carry_in */
              false /* do_sub */
              );
          }
            break;
          case FLARE32_G2_OP_ENUM_SUB_RA_RB:
          {
            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *ra = flare32_sim_add_sub
              (32u, /* bits */
              *ra, /* operand_a */
              rb, /* operand_b */
              temp_flags_in, /* flags_in */
              fw ? flags : NULL, /* flags out */
              false, /* with_carry_in */
              true /* do_sub */
              );
          }
            break;
          case FLARE32_G2_OP_ENUM_ADD_RA_SP_RB:
          {
            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *ra = flare32_sim_add_sub
              (32u, /* bits */
              sp, /* operand_a */
              rb, /* operand_b */
              temp_flags_in, /* flags_in */
              fw ? flags : NULL, /* flags out */
              false, /* with_carry_in */
              false /* do_sub */
              );
          }
            break;
          case FLARE32_G2_OP_ENUM_ADD_RA_FP_RB:
          {
            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *ra = flare32_sim_add_sub
              (32u, /* bits */
              fp, /* operand_a */
              rb, /* operand_b */
              temp_flags_in, /* flags_in */
              fw ? flags : NULL, /* flags out */
              false, /* with_carry_in */
              false /* do_sub */
              );
          }
            break;
          case FLARE32_G2_OP_ENUM_CMP_RA_RB:
          {
            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *ra = flare32_sim_add_sub
              (32u, /* bits */
              *ra, /* operand_a */
              rb, /* operand_b */
              temp_flags_in, /* flags_in */
              fw ? flags : NULL, /* flags out */
              false, /* with_carry_in */
              true /* do_sub */
              );
          }
            break;
          case FLARE32_G2_OP_ENUM_CPY_RA_RB:
          {
            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *ra = rb;

            if (fw)
            {
              flare32_sim_set_flags_zn (32u, *ra, flags);
            }
          }
            break;
          case FLARE32_G2_OP_ENUM_LSL_RA_RB:
          {
            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *ra = (uint32_t) (*ra) << (uint32_t) rb;

            if (fw)
            {
              flare32_sim_set_flags_zn (32u, *ra, flags);
            }
          }
            break;
          case FLARE32_G2_OP_ENUM_LSR_RA_RB:
          {
            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *ra = (uint32_t) (*ra) >> (uint32_t) rb;
            if (fw)
            {
              flare32_sim_set_flags_zn (32u, *ra, flags);
            }
          }
            break;
          case FLARE32_G2_OP_ENUM_ASR_RA_RB:
          {
            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *ra = (*ra) >> (uint32_t) rb;

            if (fw)
            {
              flare32_sim_set_flags_zn (32u, *ra, flags);
            }
          }
            break;
          case FLARE32_G2_OP_ENUM_AND_RA_RB:
          {
            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *ra &= rb;

            if (fw)
            {
              flare32_sim_set_flags_zn (32u, *ra, flags);
            }
          }
            break;
          case FLARE32_G2_OP_ENUM_ORR_RA_RB:
          {
            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *ra |= rb;

            if (fw)
            {
              flare32_sim_set_flags_zn (32u, *ra, flags);
            }
          }
            break;
          case FLARE32_G2_OP_ENUM_XOR_RA_RB:
          {
            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *ra ^= rb;

            if (fw)
            {
              flare32_sim_set_flags_zn (32u, *ra, flags);
            }
          }
            break;
          case FLARE32_G2_OP_ENUM_ADC_RA_RB:
          {
            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *ra = flare32_sim_add_sub
              (32u, /* bits */
              *ra, /* operand_a */
              rb, /* operand_b */
              temp_flags_in, /* flags_in */
              fw ? flags : NULL, /* flags out */
              true, /* with_carry_in */
              false /* do_sub */
              );
          }
            break;
          case FLARE32_G2_OP_ENUM_SBC_RA_RB:
          {
            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *ra = flare32_sim_add_sub
              (32u, /* bits */
              *ra, /* operand_a */
              rb, /* operand_b */
              temp_flags_in, /* flags_in */
              fw ? flags : NULL, /* flags out */
              true, /* with_carry_in */
              true /* do_sub */
              );
          }
            break;
          case FLARE32_G2_OP_ENUM_RESERVED_14:
          case FLARE32_G2_OP_ENUM_RESERVED_15:
          default:
          {
            FLARE32_TRACE_INSN ("SIGILL_G2");
            sim_engine_halt (sd, scpu, NULL, pc, sim_stopped, SIM_SIGILL);
          }
            break;
        }
      }
        break;
      case FLARE32_G3_GRP_VALUE:
      {
        int32_t
          //branch_target,
          flags;
        bool flags_z, flags_c, flags_v, flags_n;

        opc_info = &flare32_opc_info_g3
          [flare32_get_insn_field_ei (&flare32_enc_info_g3_op, insn)];
        simm = (simm << flare32_enc_info_g3_s9.bitsize)
          | flare32_get_insn_field_ei (&flare32_enc_info_g3_s9, insn);

        //branch_target = pc + nbytes + (int32_t) simm;
        flags = cpu.sprs[FLARE32_SPR_ENUM_FLAGS];
        flags_z = (flags & FLARE32_FLAGS_Z_MASK) != 0;
        flags_c = (flags & FLARE32_FLAGS_C_MASK) != 0;
        flags_v = (flags & FLARE32_FLAGS_V_MASK) != 0;
        flags_n = (flags & FLARE32_FLAGS_N_MASK) != 0;

        switch (opc_info->opcode)
        {
          case FLARE32_G3_OP_ENUM_BL_PCREL_S9:
          {
            int32_t
              *lr = &cpu.gprs[FLARE32_GPR_ENUM_LR];

            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *lr = pc + nbytes;
            pc += simm & ~0x1ull;
          }
            break;
          case FLARE32_G3_OP_ENUM_BRA_PCREL_S9:
          {
            FLARE32_TRACE_INSN (opc_info->names[fw]);
            pc += simm & ~0x1ull;
          }
            break;
          case FLARE32_G3_OP_ENUM_BEQ_PCREL_S9:
          {
            FLARE32_TRACE_INSN (opc_info->names[fw]);
            if (flags_z)
            {
              pc += simm & ~0x1ull;
            }
          }
            break;
          case FLARE32_G3_OP_ENUM_BNE_PCREL_S9:
          {
            FLARE32_TRACE_INSN (opc_info->names[fw]);
            if (!flags_z)
            {
              pc += simm & ~0x1ull;
            }
          }
            break;
          case FLARE32_G3_OP_ENUM_BMI_PCREL_S9:
          {
            FLARE32_TRACE_INSN (opc_info->names[fw]);
            if (flags_n)
            {
              pc += simm & ~0x1ull;
            }
          }
            break;
          case FLARE32_G3_OP_ENUM_BPL_PCREL_S9:
          {
            FLARE32_TRACE_INSN (opc_info->names[fw]);
            if (!flags_n)
            {
              pc += simm & ~0x1ull;
            }
          }
            break;
          case FLARE32_G3_OP_ENUM_BVS_PCREL_S9:
          {
            FLARE32_TRACE_INSN (opc_info->names[fw]);
            if (flags_v)
            {
              pc += simm & ~0x1ull;
            }
          }
            break;
          case FLARE32_G3_OP_ENUM_BVC_PCREL_S9:
          {
            FLARE32_TRACE_INSN (opc_info->names[fw]);
            if (!flags_v)
            {
              pc += simm & ~0x1ull;
            }
          }
            break;
          case FLARE32_G3_OP_ENUM_BGEU_PCREL_S9:
          {
            FLARE32_TRACE_INSN (opc_info->names[fw]);
            if (flags_c)
            {
              pc += simm & ~0x1ull;
            }
          }
            break;
          case FLARE32_G3_OP_ENUM_BLTU_PCREL_S9:
          {
            FLARE32_TRACE_INSN (opc_info->names[fw]);
            if (!flags_c)
            {
              pc += simm & ~0x1ull;
            }
          }
            break;
          case FLARE32_G3_OP_ENUM_BGTU_PCREL_S9:
          {
            FLARE32_TRACE_INSN (opc_info->names[fw]);
            if (flags_c && !flags_z)
            {
              pc += simm & ~0x1ull;
            }
          }
            break;
          case FLARE32_G3_OP_ENUM_BLEU_PCREL_S9:
          {
            FLARE32_TRACE_INSN (opc_info->names[fw]);
            if (!flags_c || flags_z)
            {
              pc += simm & ~0x1ull;
            }
          }
            break;
          case FLARE32_G3_OP_ENUM_BGES_PCREL_S9:
          {
            FLARE32_TRACE_INSN (opc_info->names[fw]);
            if (flags_n == flags_v)
            {
              pc += simm & ~0x1ull;
            }
          }
            break;
          case FLARE32_G3_OP_ENUM_BLTS_PCREL_S9:
          {
            FLARE32_TRACE_INSN (opc_info->names[fw]);
            if (flags_n != flags_v)
            {
              pc += simm & ~0x1ull;
            }
          }
            break;
          case FLARE32_G3_OP_ENUM_BGTS_PCREL_S9:
          {
            FLARE32_TRACE_INSN (opc_info->names[fw]);
            if (flags_n == flags_v && !flags_z)
            {
              pc += simm & ~0x1ull;
            }
          }
            break;
          case FLARE32_G3_OP_ENUM_BLES_PCREL_S9:
          {
            FLARE32_TRACE_INSN (opc_info->names[fw]);
            if (flags_n != flags_v || flags_z)
            {
              pc += simm & ~0x1ull;
            }
          }
            break;
          default:
          {
            FLARE32_TRACE_INSN ("SIGILL_G3");
            sim_engine_halt (sd, scpu, NULL, pc, sim_stopped, SIM_SIGILL);
          }
            break;
        }
      }
        break;
      case FLARE32_G4_GRP_VALUE:
      {
        opc_info = &flare32_opc_info_g4
          [flare32_get_insn_field_ei (&flare32_enc_info_g4_op, insn)];

        switch (opc_info->opcode)
        {
          case FLARE32_G4_OP_ENUM_JL_RA:
          {
            int32_t
              ra = cpu.gprs[ra_ind],
              *lr = &cpu.gprs[FLARE32_GPR_ENUM_LR];

            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *lr = pc + nbytes;

            /* Account for the `pc += nbytes;` statement at the end of the 
              do-while loop */
            pc = (ra & ~0x1ull) - nbytes;
          }
            break;
          case FLARE32_G4_OP_ENUM_JMP_RA:
          {
            int32_t
              ra = cpu.gprs[ra_ind];

            FLARE32_TRACE_INSN (opc_info->names[fw]);

            pc = ra;
          }
            break;
          case FLARE32_G4_OP_ENUM_JMP_IRA:
          {
            int32_t
              ira = cpu.sprs[FLARE32_SPR_ENUM_IRA];

            FLARE32_TRACE_INSN (opc_info->names[fw]);

            pc = ira;
          }
            break;
          case FLARE32_G4_OP_ENUM_RETI:
          {
            int32_t
              *ie = &cpu.sprs[FLARE32_SPR_ENUM_IE],
              ira = cpu.sprs[FLARE32_SPR_ENUM_IRA];

            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *ie |= 0x1;
            pc = ira;
          }
            break;
          case FLARE32_G4_OP_ENUM_CPY_RA_SB:
          {
            if (rb_ind < FLARE32_NUM_SPRS)
            {
              int32_t
                *ra = &cpu.gprs[ra_ind],
                sb = cpu.sprs[rb_ind];

              FLARE32_TRACE_INSN (opc_info->names[fw]);

              *ra = sb;
            }
            else
            {
              FLARE32_TRACE_INSN ("SIGILL_G4_CPY_RA_SB");
              sim_engine_halt (sd, scpu, NULL, pc, sim_stopped,
                SIM_SIGILL);
            }
          }
            break;
          case FLARE32_G4_OP_ENUM_CPY_SA_RB:
          {
            if (ra_ind < FLARE32_NUM_SPRS)
            {
              int32_t
                *sa = &cpu.sprs[ra_ind],
                rb = cpu.gprs[rb_ind];

              FLARE32_TRACE_INSN (opc_info->names[fw]);

              *sa = rb;
            }
            else
            {
              FLARE32_TRACE_INSN ("SIGILL_G4_CPY_SA_RB");
              sim_engine_halt (sd, scpu, NULL, pc, sim_stopped,
                SIM_SIGILL);
            }
          }
            break;
          case FLARE32_G4_OP_ENUM_EI:
          {
            int32_t
              *ie = &cpu.sprs[FLARE32_SPR_ENUM_IE];

            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *ie |= 0x1;
          }
            break;
          case FLARE32_G4_OP_ENUM_DI:
          {
            int32_t
              *ie = &cpu.sprs[FLARE32_SPR_ENUM_IE];

            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *ie &= ~0x1;
          }
            break;
          case FLARE32_G4_OP_ENUM_PUSH_RA_RB:
          {
            int32_t
              *ra = &cpu.sprs[ra_ind],
              *rb = &cpu.gprs[rb_ind];

            FLARE32_TRACE_INSN (opc_info->names[fw]);
            wr32 (scpu, opc, *rb, *ra);
            *rb -= sizeof (*ra);
          }
            break;
          case FLARE32_G4_OP_ENUM_POP_RA_RB:
          {
            int32_t
              *ra = &cpu.sprs[ra_ind],
              *rb = &cpu.gprs[rb_ind];

            FLARE32_TRACE_INSN (opc_info->names[fw]);
            *rb += sizeof (*ra);
            *ra = rd32 (scpu, opc, *rb);
          }
            break;
          case FLARE32_G4_OP_ENUM_PUSH_SA_RB:
          {
            if (ra_ind < FLARE32_NUM_SPRS)
            {
              int32_t
                *sa = &cpu.sprs[ra_ind],
                *rb = &cpu.gprs[rb_ind];

              FLARE32_TRACE_INSN (opc_info->names[fw]);
              wr32 (scpu, opc, *rb, *sa);
              *rb -= sizeof (*sa);
            }
            else
            {
              FLARE32_TRACE_INSN ("SIGILL_G4_PUSH_SA_RB");
              sim_engine_halt (sd, scpu, NULL, pc, sim_stopped,
                SIM_SIGILL);
            }
          }
            break;
          case FLARE32_G4_OP_ENUM_POP_SA_RB:
          {
            if (ra_ind < FLARE32_NUM_SPRS)
            {
              int32_t
                *sa = &cpu.sprs[ra_ind],
                *rb = &cpu.gprs[rb_ind];

              FLARE32_TRACE_INSN (opc_info->names[fw]);
              *rb += sizeof (*sa);
              *sa = rd32 (scpu, opc, *rb);
            }
            else
            {
              FLARE32_TRACE_INSN ("SIGILL_G4_POP_SA_RB");
              sim_engine_halt (sd, scpu, NULL, pc, sim_stopped,
                SIM_SIGILL);
            }
          }
            break;
          case FLARE32_G4_OP_ENUM_MUL_RA_RB:
          {
            int32_t
              *ra = &cpu.gprs[ra_ind],
              rb = cpu.gprs[rb_ind];

            int32_t
              lhs = *ra,
              rhs = rb;

            lhs *= rhs;

            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *ra = lhs;
          }
            break;
          case FLARE32_G4_OP_ENUM_UDIV_RA_RB:
          {
            int32_t
              *ra = &cpu.gprs[ra_ind],
              rb = cpu.gprs[rb_ind];

            int32_t
              lhs = *ra,
              rhs = rb;

            lhs /= rhs;

            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *ra = lhs;
          }
            break;
          case FLARE32_G4_OP_ENUM_SDIV_RA_RB:
          {
            int32_t
              *ra = &cpu.gprs[ra_ind],
              rb = cpu.gprs[rb_ind];

            uint32_t
              lhs = (uint32_t) (*ra),
              rhs = (uint32_t) rb;

            lhs /= rhs;

            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *ra = (int32_t) lhs;
          }
            break;

          case FLARE32_G4_OP_ENUM_UMOD_RA_RB:
          {
            int32_t
              *ra = &cpu.gprs[ra_ind],
              rb = cpu.gprs[rb_ind];

            uint32_t
              lhs = (uint32_t) (*ra),
              rhs = (uint32_t) rb;

            lhs %= rhs;

            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *ra = (int32_t) lhs;
          }
            break;
          case FLARE32_G4_OP_ENUM_SMOD_RA_RB:
          {
            int32_t
              *ra = &cpu.gprs[ra_ind],
              rb = cpu.gprs[rb_ind];

            int32_t
              lhs = *ra,
              rhs = rb;

            lhs %= rhs;

            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *ra = lhs;
          }
            break;
          case FLARE32_G4_OP_ENUM_LUMUL_RA_RB:
          {
            int32_t
              ra = cpu.gprs[ra_ind],
              rb = cpu.gprs[rb_ind],
              *hi = &cpu.sprs[FLARE32_SPR_ENUM_HI],
              *lo = &cpu.sprs[FLARE32_SPR_ENUM_LO];

            uint64_t
              ra_signext = flare32_zero_extend (ra, 32u),
              rb_signext = flare32_zero_extend (rb, 32u),
              lhs = ra_signext * rb_signext;

            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *hi = (int32_t) (lhs >> 32u);
            *lo = (int32_t) lhs;
          }
            break;
          case FLARE32_G4_OP_ENUM_LSMUL_RA_RB:
          {
            int32_t
              ra = cpu.gprs[ra_ind],
              rb = cpu.gprs[rb_ind],
              *hi = &cpu.sprs[FLARE32_SPR_ENUM_HI],
              *lo = &cpu.sprs[FLARE32_SPR_ENUM_LO];

            int64_t
              ra_signext = flare32_sign_extend (ra, 32u),
              rb_signext = flare32_sign_extend (rb, 32u),
              lhs = ra_signext * rb_signext;

            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *hi = (int32_t) (lhs >> 32u);
            *lo = (int32_t) lhs;
          }
            break;
          case FLARE32_G4_OP_ENUM_LUDIV_RA_RB:
          {
            int32_t
              ra = cpu.gprs[ra_ind],
              rb = cpu.gprs[rb_ind],
              *hi = &cpu.sprs[FLARE32_SPR_ENUM_HI],
              *lo = &cpu.sprs[FLARE32_SPR_ENUM_LO];

            uint64_t
              lhs = (uint64_t) ((flare32_zero_extend (*hi, 32u) << 32u)
                | flare32_zero_extend (*lo, 32u)),
              rhs = (uint64_t) ((flare32_zero_extend (ra, 32u) << 32u)
                | flare32_zero_extend (rb, 32u));
            lhs /= rhs;

            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *hi = (int32_t) (lhs >> 32u);
            *lo = (int32_t) lhs;
          }
            break;
          case FLARE32_G4_OP_ENUM_LSDIV_RA_RB:
          {
            int32_t
              ra = cpu.gprs[ra_ind],
              rb = cpu.gprs[rb_ind],
              *hi = &cpu.sprs[FLARE32_SPR_ENUM_HI],
              *lo = &cpu.sprs[FLARE32_SPR_ENUM_LO];

            int64_t
              lhs = (int64_t) ((flare32_zero_extend (*hi, 32u) << 32u)
                | flare32_zero_extend (*lo, 32u)),
              rhs = (int64_t) ((flare32_zero_extend (ra, 32u) << 32u)
                | flare32_zero_extend (rb, 32u));
            lhs /= rhs;

            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *hi = (int32_t) (lhs >> 32u);
            *lo = (int32_t) lhs;
          }
            break;
          case FLARE32_G4_OP_ENUM_LUMOD_RA_RB:
          {
            int32_t
              ra = cpu.gprs[ra_ind],
              rb = cpu.gprs[rb_ind],
              *hi = &cpu.sprs[FLARE32_SPR_ENUM_HI],
              *lo = &cpu.sprs[FLARE32_SPR_ENUM_LO];

            uint64_t
              lhs = (uint64_t) ((flare32_zero_extend (*hi, 32u) << 32u)
                | flare32_zero_extend (*lo, 32u)),
              rhs = (uint64_t) ((flare32_zero_extend (ra, 32u) << 32u)
                | flare32_zero_extend (rb, 32u));
            lhs %= rhs;

            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *hi = (int32_t) (lhs >> 32u);
            *lo = (int32_t) lhs;
          }
            break;
          case FLARE32_G4_OP_ENUM_LSMOD_RA_RB:
          {
            int32_t
              ra = cpu.gprs[ra_ind],
              rb = cpu.gprs[rb_ind],
              *hi = &cpu.sprs[FLARE32_SPR_ENUM_HI],
              *lo = &cpu.sprs[FLARE32_SPR_ENUM_LO];

            int64_t
              lhs = (int64_t) ((flare32_zero_extend (*hi, 32u) << 32u)
                | flare32_zero_extend (*lo, 32u)),
              rhs = (int64_t) ((flare32_zero_extend (ra, 32u) << 32u)
                | flare32_zero_extend (rb, 32u));
            lhs %= rhs;

            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *hi = (int32_t) (lhs >> 32u);
            *lo = (int32_t) rhs;
          }
            break;
          case FLARE32_G4_OP_ENUM_LDUB_RA_RB:
          {
            int32_t
              *ra = &cpu.gprs[ra_ind],
              rb = cpu.gprs[rb_ind],
              rc = !have_index_insn ? 0x0 : cpu.gprs[rc_ind];
            uint32_t
              addr = 0;

            addr = rb + rc;

            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *ra = flare32_zero_extend (rd8u (scpu, opc, addr), 8u);
          }
            break;
          case FLARE32_G4_OP_ENUM_LDSB_RA_RB:
          {
            int32_t
              *ra = &cpu.gprs[ra_ind],
              rb = cpu.gprs[rb_ind],
              rc = !have_index_insn ? 0x0 : cpu.gprs[rc_ind];
            uint32_t
              addr = 0;

            addr = rb + rc;

            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *ra = flare32_sign_extend (rd8s (scpu, opc, addr), 8u);
          }
            break;
          case FLARE32_G4_OP_ENUM_LDUH_RA_RB:
          {
            int32_t
              *ra = &cpu.gprs[ra_ind],
              rb = cpu.gprs[rb_ind],
              rc = !have_index_insn ? 0x0 : cpu.gprs[rc_ind];
            uint32_t
              addr = 0;

            addr = rb + rc;

            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *ra = flare32_zero_extend (rd16u (scpu, opc, addr), 16u);
          }
            break;
          case FLARE32_G4_OP_ENUM_LDSH_RA_RB:
          {
            int32_t
              *ra = &cpu.gprs[ra_ind],
              rb = cpu.gprs[rb_ind],
              rc = !have_index_insn ? 0x0 : cpu.gprs[rc_ind];
            uint32_t
              addr = 0;

            addr = rb + rc;

            FLARE32_TRACE_INSN (opc_info->names[fw]);

            *ra = flare32_sign_extend (rd16s (scpu, opc, addr), 16u);
          }
            break;
          case FLARE32_G4_OP_ENUM_STB_RA_RB:
          {
            int32_t
              ra = cpu.gprs[ra_ind],
              rb = cpu.gprs[rb_ind],
              rc = !have_index_insn ? 0x0 : cpu.gprs[rc_ind];
            uint32_t
              addr = 0;

            addr = rb + rc;

            FLARE32_TRACE_INSN (opc_info->names[fw]);

            wr8 (scpu, opc, addr, ra);
          }
            break;
          case FLARE32_G4_OP_ENUM_STH_RA_RB:
          {
            int32_t
              ra = cpu.gprs[ra_ind],
              rb = cpu.gprs[rb_ind],
              rc = !have_index_insn ? 0x0 : cpu.gprs[rc_ind];
            uint32_t
              addr = 0;

            addr = rb + rc;

            FLARE32_TRACE_INSN (opc_info->names[fw]);

            wr16 (scpu, opc, addr, ra);
          }
            break;
          case FLARE32_G4_OP_ENUM_INDEX_RA:
          case FLARE32_G4_OP_ENUM_RESERVED_30:
          case FLARE32_G4_OP_ENUM_RESERVED_31:
          default:
          {
            FLARE32_TRACE_INSN ("SIGILL_G4");
            sim_engine_halt (sd, scpu, NULL, pc, sim_stopped, SIM_SIGILL);
          }
            break;
        }
      }
        break;
      case FLARE32_G5_GRP_VALUE:
      {
        int32_t
          *ra = &cpu.gprs[ra_ind],
          rb = cpu.gprs[rb_ind],
          rc = !have_index_insn ? 0x0 : cpu.gprs[rc_ind];
        uint32_t
          addr = 0;

        opc_info = &flare32_opc_info_g5[0];
        simm = (simm << flare32_enc_info_g1g5g6_s5.bitsize)
          | flare32_get_insn_field_ei (&flare32_enc_info_g1g5g6_s5, insn);


        addr = rb + rc + simm;

        FLARE32_TRACE_INSN (opc_info->names[fw]);

        *ra = rd32 (scpu, opc, addr);
      }
        break;
      case FLARE32_G6_GRP_VALUE:
      {
        int32_t
          ra = cpu.gprs[ra_ind],
          rb = cpu.gprs[rb_ind],
          rc = !have_index_insn ? 0x0 : cpu.gprs[rc_ind];
        uint32_t
          addr = 0;

        opc_info = &flare32_opc_info_g6[0];
        simm = (simm << flare32_enc_info_g1g5g6_s5.bitsize)
          | flare32_get_insn_field_ei (&flare32_enc_info_g1g5g6_s5, insn);

        addr = rb + rc + simm;

        FLARE32_TRACE_INSN (opc_info->names[fw]);

        wr32 (scpu, opc, addr, ra);
      }
        break;
      case FLARE32_G7_GRP_VALUE:
      {
        if ((subgrp = flare32_get_insn_field_ei
          (&flare32_enc_info_g7_aluopbh_subgrp, insn))
          == FLARE32_G7_ALUOPBH_SUBGRP_VALUE)
        {
          opc_info = &flare32_opc_info_g7_aluopbh
            [flare32_get_insn_field_ei
              (&flare32_enc_info_g7_aluopbh_op, insn)];
          fw = flare32_get_insn_field_ei (&flare32_enc_info_g7_aluopbh_w,
            insn);
          switch (opc_info->opcode)
          {
            case FLARE32_G7_ALUOPBH_OP_ENUM_CMP_RA_RB:
            {
              if (fw == FLARE32_G7_ALUOPBH_W_ENUM_8)
              {
                int32_t
                  ra = cpu.gprs[ra_ind],
                  rb = cpu.gprs[rb_ind],
                  temp_flags_in = cpu.sprs[FLARE32_SPR_ENUM_FLAGS],
                  *flags = &cpu.sprs[FLARE32_SPR_ENUM_FLAGS];

                FLARE32_TRACE_INSN (opc_info->names[fw]);

                (void) flare32_sim_add_sub
                  (8u, /* bits */
                  ra, /* operand_a */
                  rb, /* operand_b */
                  temp_flags_in, /* flags_in */
                  flags, /* flags out */
                  false, /* with_carry_in */
                  true /* do_sub */
                  );
              }
              else if (fw == FLARE32_G7_ALUOPBH_W_ENUM_16)
              {
                int32_t
                  ra = cpu.gprs[ra_ind],
                  rb = cpu.gprs[rb_ind],
                  temp_flags_in = cpu.sprs[FLARE32_SPR_ENUM_FLAGS],
                  *flags = &cpu.sprs[FLARE32_SPR_ENUM_FLAGS];

                FLARE32_TRACE_INSN (opc_info->names[fw]);

                (void) flare32_sim_add_sub
                  (16u, /* bits */
                  ra, /* operand_a */
                  rb, /* operand_b */
                  temp_flags_in, /* flags_in */
                  flags, /* flags out */
                  false, /* with_carry_in */
                  true /* do_sub */
                  );
              }
              else
              {
                FLARE32_TRACE_INSN ("SIGILL_G7_ALUOPBH_W");
                sim_engine_halt (sd, scpu, NULL, pc, sim_stopped,
                  SIM_SIGILL);
              }
            }
              break;
            case FLARE32_G7_ALUOPBH_OP_ENUM_LSR_RA_RB:
            {
              if (fw == FLARE32_G7_ALUOPBH_W_ENUM_8)
              {
                uint8_t ra = (uint8_t) (cpu.gprs[ra_ind] & 0xff);
                uint32_t rb = (uint32_t) cpu.gprs[rb_ind];
                ra >>= rb;

                FLARE32_TRACE_INSN (opc_info->names[fw]);

                cpu.gprs[ra_ind] = (flare32_temp_t) ra;
              }
              else if (fw == FLARE32_G7_ALUOPBH_W_ENUM_16)
              {
                uint16_t ra = (uint16_t) (cpu.gprs[ra_ind] & 0xffff);
                uint32_t rb = (uint32_t) cpu.gprs[rb_ind];
                ra >>= rb;

                FLARE32_TRACE_INSN (opc_info->names[fw]);

                cpu.gprs[ra_ind] = (flare32_temp_t) ra;
              }
              else
              {
                FLARE32_TRACE_INSN ("SIGILL_G7_ALUOPBH_W");
                sim_engine_halt (sd, scpu, NULL, pc, sim_stopped,
                  SIM_SIGILL);
              }
            }
              break;
            case FLARE32_G7_ALUOPBH_OP_ENUM_ASR_RA_RB:
            {
              if (fw == FLARE32_G7_ALUOPBH_W_ENUM_8)
              {
                int8_t ra = cpu.gprs[ra_ind] & 0xff;
                uint32_t rb = (uint32_t) cpu.gprs[rb_ind];
                ra >>= rb;

                FLARE32_TRACE_INSN (opc_info->names[fw]);

                cpu.gprs[ra_ind] = flare32_sign_extend
                  ((flare32_temp_t) ra, 8ull);
              }
              else if (fw == FLARE32_G7_ALUOPBH_W_ENUM_16)
              {
                int16_t ra = cpu.gprs[ra_ind] & 0xffff;
                uint32_t rb = (uint32_t) cpu.gprs[rb_ind];
                ra >>= rb;

                FLARE32_TRACE_INSN (opc_info->names[fw]);

                cpu.gprs[ra_ind] = flare32_sign_extend
                  ((flare32_temp_t) ra, 16ull);
              }
              else
              {
                FLARE32_TRACE_INSN ("SIGILL_G7_ALUOPBH_W");
                sim_engine_halt (sd, scpu, NULL, pc, sim_stopped,
                  SIM_SIGILL);
              }
            }
              break;
            default:
            {
              FLARE32_TRACE_INSN ("SIGILL_G7_ALUOPBH_OP");
              sim_engine_halt (sd, scpu, NULL, pc, sim_stopped,
                SIM_SIGILL);
            }
              break;
          }
        }
        else
        {
          FLARE32_TRACE_INSN ("SIGILL_G7_SUBGRP");
          sim_engine_halt (sd, scpu, NULL, pc, sim_stopped, SIM_SIGILL);
        }
      }
        break;
    }

    ++cpu.insn_cnt;

    //pc += 2;
    pc += nbytes;

    cpu.pc = pc;

    if (sim_events_tick (sd))
    {
      sim_events_process (sd);
    }

  } while (1);
}
SIM_RC
sim_create_inferior (SIM_DESC sd, struct bfd *abfd,
                    char * const *argv, char * const *env)
{
  char * const *avp;
  int l, argc, i, tp;
  sim_cpu *scpu = STATE_CPU (sd, 0); /* FIXME */

  if (abfd != NULL)
  {
    cpu.pc = bfd_get_start_address (abfd);
  }

  /* Copy args into target memory.  */
  avp = argv;
  for (argc=0; avp && *avp; ++avp)
  {
    argc++;
  }

  /* Target memory looks like this:
     0x00000000 zero word
     0x00000004 argc word
     0x00000008 start of argv
     .
     0x0000???? end of argv
     0x0000???? zero word 
     0x0000???? start of data pointed to by argv  */

  wr32 (scpu, 0, 0, 0);
  wr32 (scpu, 0, 4, argc);

  /* tp is the offset of our first argv data.  */
  tp = 4 + 4 + argc * 4 + 4;

  for (i=0; i<argc; ++i)
  {
    /* Set the argv value.  */
    wr32 (scpu, 0, 4 + 4 + i * 4, tp);

    /* Store the string.  */
    sim_core_write_buffer (sd, scpu, write_map, argv[i],
                          tp, strlen (argv[i])+1);
    tp += strlen (argv[i]) + 1;
  }

  wr32 (scpu, 0, 4 + 4 + i * 4, 0);

  //load_dtb (sd, DTB);

  return SIM_RC_OK;
}
/* -------- */

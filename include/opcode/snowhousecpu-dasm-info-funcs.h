/* snowhousecpu-disasm-info-funcs.h -- Functions for disassembling snowhousecpu
   insns.

   Copyright (C) 2023 Free Software Foundation, Inc.
   Contributed by Andrew Clark (FL4SHK).

   This file is part of the GNU opcodes library.

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   It is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with this file; see the file COPYING.  If not, write to the
   Free Software Foundation, 51 Franklin Street - Fifth Floor, Boston,
   MA 02110-1301, USA.  */

#ifndef _SNOWHOUSECPU_DASM_INFO_FUNCS_H_
#define _SNOWHOUSECPU_DASM_INFO_FUNCS_H_

//#ifdef SNOWHOUSECPU_CLANGD
#include <stdio.h>
#include "snowhousecpu.h"
#include "snowhousecpu-opc-decls.h"
//#endif

static inline void
snowhousecpu_dasm_info_ctor (snowhousecpu_dasm_info_t *self,
  snowhousecpu_dasm_info_rd32_func rd32_func)
{
  memset (self, 0, sizeof (*self));
  self->rd32_func = rd32_func;
}

static void
snowhousecpu_dasm_info_do_disassemble_worker (snowhousecpu_dasm_info_t *self)
{
  const snowhousecpu_temp_t temp_shift_imm5 = snowhousecpu_get_insn_field (
    SNOWHOUSECPU_SHIFT_IMM5_MASK, SNOWHOUSECPU_SHIFT_IMM5_BITPOS, self->iword
  );
  self->op = snowhousecpu_get_insn_field (
    SNOWHOUSECPU_OP_MASK, SNOWHOUSECPU_OP_BITPOS, self->iword
  );
  self->subop_imm16 = snowhousecpu_get_insn_field (
    SNOWHOUSECPU_SUBOP_IMM16_MASK, SNOWHOUSECPU_SUBOP_IMM16_BITPOS, self->iword
  );
  self->ra_idx = snowhousecpu_get_insn_field (
    SNOWHOUSECPU_RA_IDX_MASK, SNOWHOUSECPU_RA_IDX_BITPOS, self->iword
  );
  self->rb_idx = snowhousecpu_get_insn_field (
    SNOWHOUSECPU_RB_IDX_MASK, SNOWHOUSECPU_RB_IDX_BITPOS, self->iword
  );
  self->rc_idx = snowhousecpu_get_insn_field (
    SNOWHOUSECPU_RC_IDX_MASK, SNOWHOUSECPU_RC_IDX_BITPOS, self->iword
  );
  self->subop_rc_idx = self->rc_idx;
  self->length += 4;
  self->is_bad = false;
  self->opc_info = NULL;
  if (self->op < snowhousecpu_opc_info_a2d_size) {
    //fprintf (
    //  stderr,
    //  "self->op: %u\n",
    //  (unsigned) self->op
    //);
    const snowhousecpu_opc_info_t *opc_info = snowhousecpu_opc_info_a2d[self->op][0];
    //if (self->op != snowhousecpu_opc_info_beq_ra_rb_simm16.op)
    //const snowhousecpu_opc_info_t **opc_info = NULL;
    unsigned count = 0;
    for (
      count=0; //opc_info=opc_info_arr;
      //opc_info!=NULL;
      count++<snowhousecpu_opc_info_size_arr[self->op];
      opc_info=snowhousecpu_opc_info_a2d[self->op][count]
    )
    {
      //const snowhousecpu_opc_info_t *opc_info = *opc_info_arr;
      bool finished = false;
      switch (opc_info->subop.kind)
      {
        case SNOWHOUSECPU_SOK_NONE:
        {
          finished = true;
        }
          break;
        case SNOWHOUSECPU_SOK_RC_IDX_FULL:
        {
          if (self->subop_rc_idx == opc_info->subop.val)
          {
            if (opc_info->subop.is_non_inequality_branch_etc)
            {
              if (
                (
                  !(self->ra_idx == self->rb_idx && self->ra_idx != 0)
                )
                == (opc_info->subop.is_non_inequality_branch_etc - 1ull)
              )
              {
                //fprintf (
                //  stderr,
                //  "dasm: %s %s, %s, simm16; %u\n",
                //  opc_info->name,
                //  gprs[self->ra_idx].name,
                //  gprs[self->rb_idx].name,
                //  (unsigned) opc_info->subop.is_non_inequality_branch_etc
                //);
                finished = true;
              }
            }
            else
            {
              finished = true;
            }
          }
        }
          break;
        case SNOWHOUSECPU_SOK_RC_IDX_NZ:
        {
          if ((self->subop_rc_idx != 0) == opc_info->subop.val)
          {
            finished = true;
          }
        }
          break;
        case SNOWHOUSECPU_SOK_IMM16_LO:
        {
          if (self->subop_imm16 == opc_info->subop.val)
          {
            finished = true;
          }
        }
          break;
      }
      if (finished)
      {
        self->opc_info = opc_info;
        if (opc_info->oparg == SNOWHOUSECPU_OA_RA_RB_SHIFT_U5)
        {
          self->simm = temp_shift_imm5;
        }
        break;
      }
    }
    if (self->opc_info == NULL)
    {
      self->is_bad = true;
    }
    else
    {
      switch (self->opc_info->oparg)
      {
        case SNOWHOUSECPU_OA_RA_RB_S16:
        //case SNOWHOUSECPU_OA_RA_RB_SHIFT_U5:
        case SNOWHOUSECPU_OA_RA_PCREL_S16:
        case SNOWHOUSECPU_OA_RA_RB_PCREL_S16:
        case SNOWHOUSECPU_OA_RB_RA_PCREL_S16:
        case SNOWHOUSECPU_OA_RA_PC_PCREL_S16:
        case SNOWHOUSECPU_OA_SIMM16:
          self->have_non_pre_imm = true;
          break;
        default:
          self->have_non_pre_imm = false;
          break;
      }
    }
    //else
    //{
    //  //switch (self->subop_rc_idx)
    //  {
    //    if (self->subop_rc_idx == snowhousecpu_opc_info_beq_ra_rb_simm16.subop.val)
    //    {
    //      if (self->ra_idx == self->rb_idx && self->ra_idx != 0)
    //      {
    //        if (self->ra_idx == SNOWHOUSECPU_GPR_ENUM_LR)
    //        {
    //          self->opc_info = &snowhousecpu_opc_info_bl_simm16;
    //        }
    //        else
    //        {
    //          self->opc_info = &snowhousecpu_opc_info_bl_ra_simm16;
    //        }
    //      }
    //      else
    //      {
    //        self->opc_info = &snowhousecpu_opc_info_beq_ra_rb_simm16;
    //      }
    //    }
    //    else if (self->subop_rc_idx == snowhousecpu_opc_info_bne_ra_rb_simm16.subop.val)
    //    {
    //      if (self->ra_idx == self->rb_idx && self->ra_idx != 0)
    //      {
    //        self->opc_info = &snowhousecpu_opc_info_add_ra_pc_simm16;
    //      }
    //      else
    //      {
    //        self->opc_info = &snowhousecpu_opc_info_bne_ra_rb_simm16;
    //      }
    //    }
    //    else if (self->subop_rc_idx == snowhousecpu_opc_info_bltu_ra_rb_simm16.subop.val)
    //    {
    //      self->opc_info = &snowhousecpu_opc_info_bltu_ra_rb_simm16;
    //    }
    //    else if (self->subop_rc_idx == snowhousecpu_opc_info_bgeu_ra_rb_simm16.subop.val)
    //    {
    //      self->opc_info = &snowhousecpu_opc_info_bgeu_ra_rb_simm16;
    //    }
    //    else if (self->subop_rc_idx == snowhousecpu_opc_info_blts_ra_rb_simm16.subop.val)
    //    {
    //      self->opc_info = &snowhousecpu_opc_info_blts_ra_rb_simm16;
    //    }
    //    else if (self->subop_rc_idx == snowhousecpu_opc_info_bges_ra_rb_simm16.subop.val)
    //    {
    //      self->opc_info = &snowhousecpu_opc_info_bges_ra_rb_simm16;
    //    }
    //    else if (self->subop_rc_idx == snowhousecpu_opc_info_jl_ra_rb.subop.val)
    //    {
    //      if (self->ra_idx == 0)
    //      {
    //        self->opc_info = &snowhousecpu_opc_info_jmp_rb;
    //      }
    //      else
    //      {
    //        self->opc_info = &snowhousecpu_opc_info_jl_ra_rb;
    //      }
    //    }
    //    else
    //    {
    //      self->is_bad = true;
    //    }
    //  }
    //}
    //{
    //  if (self->op == snowhousecpu_opc_info_add_ra_rb_rc.op)
    //  {
    //    if (self->rc_idx != 0x0)
    //    {
    //      self->opc_info = &snowhousecpu_opc_info_add_ra_rb_rc;
    //    }
    //    else
    //    {
    //      self->opc_info = &snowhousecpu_opc_info_add_ra_rb_simm16;
    //    }
    //  }
    //  else if (self->op == snowhousecpu_opc_info_sub_ra_rb_rc.op)
    //  {
    //    self->opc_info = &snowhousecpu_opc_info_sub_ra_rb_rc;
    //  }
    //  else if (self->op == snowhousecpu_opc_info_sltu_ra_rb_rc.op)
    //  {
    //    //switch (self->subop_imm16)
    //    {
    //      if (self->subop_imm16 == snowhousecpu_opc_info_sltu_ra_rb_rc.subop.val)
    //      {
    //        self->opc_info = &snowhousecpu_opc_info_sltu_ra_rb_rc;
    //      }
    //      else if (self->subop_imm16 == snowhousecpu_opc_info_slts_ra_rb_rc.subop.val)
    //      {
    //        self->opc_info = &snowhousecpu_opc_info_slts_ra_rb_rc;
    //      }
    //      else
    //      {
    //        self->is_bad = true;
    //      }
    //    }
    //  }
    //  else if (self->op == snowhousecpu_opc_info_xor_ra_rb_rc.op)
    //  {
    //    if (self->rc_idx != 0x0)
    //    {
    //      self->opc_info = &snowhousecpu_opc_info_xor_ra_rb_rc;
    //    }
    //    else
    //    {
    //      self->opc_info = &snowhousecpu_opc_info_xor_ra_rb_simm16;
    //    }
    //  }
    //  else if (self->op == snowhousecpu_opc_info_or_ra_rb_rc.op)
    //  {
    //    if (self->rc_idx != 0x0)
    //    {
    //      self->opc_info = &snowhousecpu_opc_info_or_ra_rb_rc;
    //    }
    //    else
    //    {
    //      self->opc_info = &snowhousecpu_opc_info_or_ra_rb_simm16;
    //    }
    //  }
    //  else if (self->op == snowhousecpu_opc_info_and_ra_rb_simm16.op)
    //  {
    //    self->opc_info = &snowhousecpu_opc_info_and_ra_rb_simm16;
    //  }
    //  else if (self->op == snowhousecpu_opc_info_lsl_ra_rb_rc.op)
    //  {
    //    //switch (self->subop_imm16)
    //    {
    //      if (self->subop_imm16 == snowhousecpu_opc_info_lsl_ra_rb_rc.subop.val)
    //      {
    //        self->opc_info = &snowhousecpu_opc_info_lsl_ra_rb_rc;
    //      }
    //      else if (self->subop_imm16 == snowhousecpu_opc_info_lsl_ra_rb_imm5.subop.val)
    //      {
    //        self->opc_info = &snowhousecpu_opc_info_lsl_ra_rb_imm5;
    //        self->simm = temp_shift_imm5;
    //      }
    //      else if (self->subop_imm16 == snowhousecpu_opc_info_lsr_ra_rb_rc.subop.val)
    //      {
    //        self->opc_info = &snowhousecpu_opc_info_lsr_ra_rb_rc;
    //      }
    //      else if (self->subop_imm16 == snowhousecpu_opc_info_lsr_ra_rb_imm5.subop.val)
    //      {
    //        self->opc_info = &snowhousecpu_opc_info_lsr_ra_rb_imm5;
    //        self->simm = temp_shift_imm5;
    //      }
    //      else if (self->subop_imm16 == snowhousecpu_opc_info_asr_ra_rb_rc.subop.val)
    //      {
    //        self->opc_info = &snowhousecpu_opc_info_asr_ra_rb_rc;
    //      }
    //      else if (self->subop_imm16 == snowhousecpu_opc_info_asr_ra_rb_imm5.subop.val)
    //      {
    //        self->opc_info = &snowhousecpu_opc_info_asr_ra_rb_imm5;
    //        self->simm = temp_shift_imm5;
    //      }
    //      else if (self->subop_imm16 == snowhousecpu_opc_info_and_ra_rb_rc.subop.val)
    //      {
    //        self->opc_info = &snowhousecpu_opc_info_and_ra_rb_rc;
    //      }
    //      else if (self->subop_imm16 == snowhousecpu_opc_info_cpy_ids_rb.subop.val)
    //      {
    //        self->opc_info = &snowhousecpu_opc_info_cpy_ids_rb;
    //      }
    //      else if (self->subop_imm16 == snowhousecpu_opc_info_cpy_ra_ira.subop.val)
    //      {
    //        self->opc_info = &snowhousecpu_opc_info_cpy_ra_ira;
    //      }
    //      else if (self->subop_imm16 == snowhousecpu_opc_info_cpy_ie_rb.subop.val)
    //      {
    //        self->opc_info = &snowhousecpu_opc_info_cpy_ie_rb;
    //      }
    //      else if (self->subop_imm16 == snowhousecpu_opc_info_ret_ira.subop.val)
    //      {
    //        self->opc_info = &snowhousecpu_opc_info_ret_ira;
    //      }
    //      else
    //      {
    //        self->is_bad = true;
    //      }
    //    }
    //  }
    //  else if (self->op == snowhousecpu_opc_info_mul_ra_rb_rc.op)
    //  {
    //    //switch (self->subop_imm16)
    //    {
    //      if (self->subop_imm16 == snowhousecpu_opc_info_mul_ra_rb_rc.subop.val)
    //      {
    //        self->opc_info = &snowhousecpu_opc_info_mul_ra_rb_rc;
    //      }
    //      else if (self->subop_imm16 == snowhousecpu_opc_info_udiv_ra_rb_rc.subop.val)
    //      {
    //        self->opc_info = &snowhousecpu_opc_info_udiv_ra_rb_rc;
    //      }
    //      else if (self->subop_imm16 == snowhousecpu_opc_info_sdiv_ra_rb_rc.subop.val)
    //      {
    //        self->opc_info = &snowhousecpu_opc_info_sdiv_ra_rb_rc;
    //      }
    //      else if (self->subop_imm16 == snowhousecpu_opc_info_umod_ra_rb_rc.subop.val)
    //      {
    //        self->opc_info = &snowhousecpu_opc_info_umod_ra_rb_rc;
    //      }
    //      else if (self->subop_imm16 == snowhousecpu_opc_info_smod_ra_rb_rc.subop.val)
    //      {
    //        self->opc_info = &snowhousecpu_opc_info_smod_ra_rb_rc;
    //      }
    //      else
    //      {
    //        self->is_bad = true;
    //      }
    //    }
    //  }
    //  else if (self->op == snowhousecpu_opc_info_ldr_ra_rb_simm16.op)
    //  {
    //    //switch (self->subop_rc_idx)
    //    {
    //      if (self->subop_imm16 == snowhousecpu_opc_info_ldr_ra_rb_simm16.subop.val)
    //      {
    //        self->opc_info = &snowhousecpu_opc_info_ldr_ra_rb_simm16;
    //      }
    //      else if (self->subop_imm16 == snowhousecpu_opc_info_str_ra_rb_simm16.subop.val)
    //      {
    //        self->opc_info = &snowhousecpu_opc_info_str_ra_rb_simm16;
    //      }
    //      else if (self->subop_imm16 == snowhousecpu_opc_info_lduh_ra_rb_simm16.subop.val)
    //      {
    //        self->opc_info = &snowhousecpu_opc_info_lduh_ra_rb_simm16;
    //      }
    //      else if (self->subop_imm16 == snowhousecpu_opc_info_ldsh_ra_rb_simm16.subop.val)
    //      {
    //        self->opc_info = &snowhousecpu_opc_info_ldsh_ra_rb_simm16;
    //      }
    //      else if (self->subop_imm16 == snowhousecpu_opc_info_ldub_ra_rb_simm16.subop.val)
    //      {
    //        self->opc_info = &snowhousecpu_opc_info_ldub_ra_rb_simm16;
    //      }
    //      else if (self->subop_imm16 == snowhousecpu_opc_info_ldsb_ra_rb_simm16.subop.val)
    //      {
    //        self->opc_info = &snowhousecpu_opc_info_ldsb_ra_rb_simm16;
    //      }
    //      else if (self->subop_imm16 == snowhousecpu_opc_info_sth_ra_rb_simm16.subop.val)
    //      {
    //        self->opc_info = &snowhousecpu_opc_info_sth_ra_rb_simm16;
    //      }
    //      else if (self->subop_imm16 == snowhousecpu_opc_info_stb_ra_rb_simm16.subop.val)
    //      {
    //        self->opc_info = &snowhousecpu_opc_info_stb_ra_rb_simm16;
    //      }
    //      else
    //      {
    //        self->is_bad = true;
    //      }
    //    }
    //  }
    //  else if (self->op == snowhousecpu_opc_info_beq_ra_rb_simm16.op)
    //  {
    //    //switch (self->subop_rc_idx)
    //    {
    //      if (self->subop_rc_idx == snowhousecpu_opc_info_beq_ra_rb_simm16.subop.val)
    //      {
    //        if (self->ra_idx == self->rb_idx && self->ra_idx != 0)
    //        {
    //          if (self->ra_idx == SNOWHOUSECPU_GPR_ENUM_LR)
    //          {
    //            self->opc_info = &snowhousecpu_opc_info_bl_simm16;
    //          }
    //          else
    //          {
    //            self->opc_info = &snowhousecpu_opc_info_bl_ra_simm16;
    //          }
    //        }
    //        else
    //        {
    //          self->opc_info = &snowhousecpu_opc_info_beq_ra_rb_simm16;
    //        }
    //      }
    //      else if (self->subop_rc_idx == snowhousecpu_opc_info_bne_ra_rb_simm16.subop.val)
    //      {
    //        if (self->ra_idx == self->rb_idx && self->ra_idx != 0)
    //        {
    //          self->opc_info = &snowhousecpu_opc_info_add_ra_pc_simm16;
    //        }
    //        else
    //        {
    //          self->opc_info = &snowhousecpu_opc_info_bne_ra_rb_simm16;
    //        }
    //      }
    //      else if (self->subop_rc_idx == snowhousecpu_opc_info_bltu_ra_rb_simm16.subop.val)
    //      {
    //        self->opc_info = &snowhousecpu_opc_info_bltu_ra_rb_simm16;
    //      }
    //      else if (self->subop_rc_idx == snowhousecpu_opc_info_bgeu_ra_rb_simm16.subop.val)
    //      {
    //        self->opc_info = &snowhousecpu_opc_info_bgeu_ra_rb_simm16;
    //      }
    //      else if (self->subop_rc_idx == snowhousecpu_opc_info_blts_ra_rb_simm16.subop.val)
    //      {
    //        self->opc_info = &snowhousecpu_opc_info_blts_ra_rb_simm16;
    //      }
    //      else if (self->subop_rc_idx == snowhousecpu_opc_info_bges_ra_rb_simm16.subop.val)
    //      {
    //        self->opc_info = &snowhousecpu_opc_info_bges_ra_rb_simm16;
    //      }
    //      else if (self->subop_rc_idx == snowhousecpu_opc_info_jl_ra_rb.subop.val)
    //      {
    //        if (self->ra_idx == 0)
    //        {
    //          self->opc_info = &snowhousecpu_opc_info_jmp_rb;
    //        }
    //        else
    //        {
    //          self->opc_info = &snowhousecpu_opc_info_jl_ra_rb;
    //        }
    //      }
    //      else
    //      {
    //        self->is_bad = true;
    //      }
    //    }
    //  }
    //  else
    //  {
    //    self->is_bad = true;
    //  }
    //}
  }
  else
  {
    self->is_bad = true;
  }
}
extern void
snowhousecpu_dasm_info_do_disassemble (snowhousecpu_dasm_info_t *self);
#endif    // _SNOWHOUSECPU_DASM_INFO_FUNCS_H_

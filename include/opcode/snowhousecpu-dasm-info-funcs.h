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
snowhousecpu_dasm_info_do_disassemble (snowhousecpu_dasm_info_t *self)
{
  if ((self->status = self->rd32_func (self)))
  {
    return;
  }
  self->iword = bfd_getl32 (self->buffer);
  const snowhousecpu_temp_t temp_simm16 = (
    snowhousecpu_get_insn_field (SNOWHOUSECPU_IMM16_MASK, SNOWHOUSECPU_IMM16_BITPOS, self->iword)
  );
  const snowhousecpu_temp_t temp_imm5 = snowhousecpu_get_insn_field (
    SNOWHOUSECPU_SHIFT_IMM5_MASK, SNOWHOUSECPU_SHIFT_IMM5_BITPOS, self->iword
  );
  snowhousecpu_temp_t temp_simm32 = 0;

  if (
    snowhousecpu_get_insn_field (SNOWHOUSECPU_OP_MASK, SNOWHOUSECPU_OP_BITPOS, self->iword)
    == snowhousecpu_opc_info_pre_simm16.op
  )
  {
    self->length += 4;
    if ((self->status = self->rd32_func (self)))
    {
      return;
    }
    //self->simm = snowhousecpu_sign_extend (snowhousecpu_get_g5_index_s19
    //  ((self->iword >> SNOWHOUSECPU_ONE_EXT_BITPOS), self->iword),
    //  snowhousecpu_enc_info_g5_index_ra_simm_s7.bitsize
    //  + snowhousecpu_enc_info_g0_pre_s12.bitsize);
    //self->simm = snowhousecpu_sign_extend (
    //  32
    //)
    //self->iword = bfd_getl32 (self->buffer);
    self->iword = (self->iword << SNOWHOUSECPU_ONE_EXT_BITPOS)
      | bfd_getl32 (self->buffer);
    temp_simm32 = snowhousecpu_sign_extend(
      (temp_simm16 << 16)
      | snowhousecpu_get_insn_field (
        SNOWHOUSECPU_IMM16_MASK, SNOWHOUSECPU_IMM16_BITPOS, self->iword
      ),
      32
    );
  }
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
  self->simm = temp_simm32; // change it for the bit shifts by immediate values
  self->length += 4;
  self->is_bad = false;

  //switch (self->op)
  {
    if (self->op == snowhousecpu_opc_info_add_ra_rb_rc.op)
    {
      if (self->rc_idx != 0x0)
      {
        self->opc_info = &snowhousecpu_opc_info_add_ra_rb_rc;
      }
      else
      {
        self->opc_info = &snowhousecpu_opc_info_add_ra_rb_simm16;
      }
    }
    else if (self->op == snowhousecpu_opc_info_sub_ra_rb_rc.op)
    {
      self->opc_info = &snowhousecpu_opc_info_sub_ra_rb_rc;
    }
    else if (self->op == snowhousecpu_opc_info_sltu_ra_rb_rc.op)
    {
      //switch (self->subop_imm16)
      {
        if (self->subop_imm16 == snowhousecpu_opc_info_sltu_ra_rb_rc.subop.val)
        {
          self->opc_info = &snowhousecpu_opc_info_sltu_ra_rb_rc;
        }
        else if (self->subop_imm16 == snowhousecpu_opc_info_slts_ra_rb_rc.subop.val)
        {
          self->opc_info = &snowhousecpu_opc_info_slts_ra_rb_rc;
        }
        else
        {
          self->is_bad = true;
        }
      }
    }
    else if (self->op == snowhousecpu_opc_info_xor_ra_rb_rc.op)
    {
      if (self->rc_idx != 0x0)
      {
        self->opc_info = &snowhousecpu_opc_info_xor_ra_rb_rc;
      }
      else
      {
        self->opc_info = &snowhousecpu_opc_info_xor_ra_rb_simm16;
      }
    }
    else if (self->op == snowhousecpu_opc_info_or_ra_rb_rc.op)
    {
      if (self->rc_idx != 0x0)
      {
        self->opc_info = &snowhousecpu_opc_info_or_ra_rb_rc;
      }
      else
      {
        self->opc_info = &snowhousecpu_opc_info_or_ra_rb_simm16;
      }
    }
    else if (self->op == snowhousecpu_opc_info_and_ra_rb_simm16.op)
    {
      self->opc_info = &snowhousecpu_opc_info_and_ra_rb_simm16;
    }
    else if (self->op == snowhousecpu_opc_info_lsl_ra_rb_rc.op)
    {
      //switch (self->subop_imm16)
      {
        if (self->subop_imm16 == snowhousecpu_opc_info_lsl_ra_rb_rc.subop.val)
        {
          self->opc_info = &snowhousecpu_opc_info_lsl_ra_rb_rc;
        }
        else if (self->subop_imm16 == snowhousecpu_opc_info_lsl_ra_rb_imm5.subop.val)
        {
          self->opc_info = &snowhousecpu_opc_info_lsl_ra_rb_imm5;
          self->simm = temp_imm5;
        }
        else if (self->subop_imm16 == snowhousecpu_opc_info_lsr_ra_rb_rc.subop.val)
        {
          self->opc_info = &snowhousecpu_opc_info_lsr_ra_rb_rc;
        }
        else if (self->subop_imm16 == snowhousecpu_opc_info_lsr_ra_rb_imm5.subop.val)
        {
          self->opc_info = &snowhousecpu_opc_info_lsr_ra_rb_imm5;
          self->simm = temp_imm5;
        }
        else if (self->subop_imm16 == snowhousecpu_opc_info_asr_ra_rb_rc.subop.val)
        {
          self->opc_info = &snowhousecpu_opc_info_asr_ra_rb_rc;
        }
        else if (self->subop_imm16 == snowhousecpu_opc_info_asr_ra_rb_imm5.subop.val)
        {
          self->opc_info = &snowhousecpu_opc_info_asr_ra_rb_imm5;
          self->simm = temp_imm5;
        }
        else if (self->subop_imm16 == snowhousecpu_opc_info_and_ra_rb_rc.subop.val)
        {
          self->opc_info = &snowhousecpu_opc_info_and_ra_rb_rc;
        }
        else if (self->subop_imm16 == snowhousecpu_opc_info_cpy_ids_rb.subop.val)
        {
          self->opc_info = &snowhousecpu_opc_info_cpy_ids_rb;
        }
        else if (self->subop_imm16 == snowhousecpu_opc_info_cpy_ra_ira.subop.val)
        {
          self->opc_info = &snowhousecpu_opc_info_cpy_ra_ira;
        }
        else if (self->subop_imm16 == snowhousecpu_opc_info_cpy_ie_rb.subop.val)
        {
          self->opc_info = &snowhousecpu_opc_info_cpy_ie_rb;
        }
        else if (self->subop_imm16 == snowhousecpu_opc_info_ret_ira.subop.val)
        {
          self->opc_info = &snowhousecpu_opc_info_ret_ira;
        }
        else
        {
          self->is_bad = true;
        }
      }
    }
    else if (self->op == snowhousecpu_opc_info_mul_ra_rb_rc.op)
    {
      //switch (self->subop_imm16)
      {
        if (self->subop_imm16 == snowhousecpu_opc_info_mul_ra_rb_rc.subop.val)
        {
          self->opc_info = &snowhousecpu_opc_info_mul_ra_rb_rc;
        }
        else if (self->subop_imm16 == snowhousecpu_opc_info_udiv_ra_rb_rc.subop.val)
        {
          self->opc_info = &snowhousecpu_opc_info_udiv_ra_rb_rc;
        }
        else if (self->subop_imm16 == snowhousecpu_opc_info_sdiv_ra_rb_rc.subop.val)
        {
          self->opc_info = &snowhousecpu_opc_info_sdiv_ra_rb_rc;
        }
        else if (self->subop_imm16 == snowhousecpu_opc_info_umod_ra_rb_rc.subop.val)
        {
          self->opc_info = &snowhousecpu_opc_info_umod_ra_rb_rc;
        }
        else if (self->subop_imm16 == snowhousecpu_opc_info_smod_ra_rb_rc.subop.val)
        {
          self->opc_info = &snowhousecpu_opc_info_smod_ra_rb_rc;
        }
        else
        {
          self->is_bad = true;
        }
      }
    }
    else if (self->op == snowhousecpu_opc_info_ldr_ra_rb_simm16.op)
    {
      //switch (self->subop_rc_idx)
      {
        if (self->subop_imm16 == snowhousecpu_opc_info_ldr_ra_rb_simm16.subop.val)
        {
          self->opc_info = &snowhousecpu_opc_info_ldr_ra_rb_simm16;
        }
        else if (self->subop_imm16 == snowhousecpu_opc_info_str_ra_rb_simm16.subop.val)
        {
          self->opc_info = &snowhousecpu_opc_info_str_ra_rb_simm16;
        }
        else if (self->subop_imm16 == snowhousecpu_opc_info_lduh_ra_rb_simm16.subop.val)
        {
          self->opc_info = &snowhousecpu_opc_info_lduh_ra_rb_simm16;
        }
        else if (self->subop_imm16 == snowhousecpu_opc_info_ldsh_ra_rb_simm16.subop.val)
        {
          self->opc_info = &snowhousecpu_opc_info_ldsh_ra_rb_simm16;
        }
        else if (self->subop_imm16 == snowhousecpu_opc_info_ldub_ra_rb_simm16.subop.val)
        {
          self->opc_info = &snowhousecpu_opc_info_ldub_ra_rb_simm16;
        }
        else if (self->subop_imm16 == snowhousecpu_opc_info_ldsb_ra_rb_simm16.subop.val)
        {
          self->opc_info = &snowhousecpu_opc_info_ldsb_ra_rb_simm16;
        }
        else if (self->subop_imm16 == snowhousecpu_opc_info_sth_ra_rb_simm16.subop.val)
        {
          self->opc_info = &snowhousecpu_opc_info_sth_ra_rb_simm16;
        }
        else if (self->subop_imm16 == snowhousecpu_opc_info_stb_ra_rb_simm16.subop.val)
        {
          self->opc_info = &snowhousecpu_opc_info_stb_ra_rb_simm16;
        }
        else
        {
          self->is_bad = true;
        }
      }
    }
    else if (self->op == snowhousecpu_opc_info_beq_ra_rb_simm16.op)
    {
      //switch (self->subop_rc_idx)
      {
        if (self->subop_rc_idx == snowhousecpu_opc_info_beq_ra_rb_simm16.subop.val)
        {
          if (self->ra_idx == self->rb_idx && self->ra_idx != 0)
          {
            if (self->ra_idx == SNOWHOUSECPU_GPR_ENUM_LR)
            {
              self->opc_info = &snowhousecpu_opc_info_bl_simm16;
            }
            else
            {
              self->opc_info = &snowhousecpu_opc_info_bl_ra_simm16;
            }
          }
          else
          {
            self->opc_info = &snowhousecpu_opc_info_beq_ra_rb_simm16;
          }
        }
        else if (self->subop_rc_idx == snowhousecpu_opc_info_bne_ra_rb_simm16.subop.val)
        {
          if (self->ra_idx == self->rb_idx && self->ra_idx != 0)
          {
            self->opc_info = &snowhousecpu_opc_info_add_ra_pc_simm16;
          }
          else
          {
            self->opc_info = &snowhousecpu_opc_info_bne_ra_rb_simm16;
          }
        }
        else if (self->subop_rc_idx == snowhousecpu_opc_info_bltu_ra_rb_simm16.subop.val)
        {
          self->opc_info = &snowhousecpu_opc_info_bltu_ra_rb_simm16;
        }
        else if (self->subop_rc_idx == snowhousecpu_opc_info_bgeu_ra_rb_simm16.subop.val)
        {
          self->opc_info = &snowhousecpu_opc_info_bgeu_ra_rb_simm16;
        }
        else if (self->subop_rc_idx == snowhousecpu_opc_info_blts_ra_rb_simm16.subop.val)
        {
          self->opc_info = &snowhousecpu_opc_info_blts_ra_rb_simm16;
        }
        else if (self->subop_rc_idx == snowhousecpu_opc_info_bges_ra_rb_simm16.subop.val)
        {
          self->opc_info = &snowhousecpu_opc_info_bges_ra_rb_simm16;
        }
        else if (self->subop_rc_idx == snowhousecpu_opc_info_jl_ra_rb.subop.val)
        {
          if (self->ra_idx == 0)
          {
            self->opc_info = &snowhousecpu_opc_info_jmp_rb;
          }
          else
          {
            self->opc_info = &snowhousecpu_opc_info_jl_ra_rb;
          }
        }
        else
        {
          self->is_bad = true;
        }
      }
    }
    else
    {
      self->is_bad = true;
    }
  }

  //if (
  //  //(snowhousecpu_get_insn_field_ei
  //  //  (&snowhousecpu_enc_info_grp_16, self->iword)
  //  //  == SNOWHOUSECPU_G4_GRP_VALUE
  //  //)
  //  //&& (snowhousecpu_get_insn_field_ei
  //  //  (&snowhousecpu_enc_info_g4_op, self->iword)
  //  //  == SNOWHOUSECPU_G4_OP_ENUM_INDEX_RA_RB))
  //  snowhousecpu_get_insn_field_ei
  //    (&snowhousecpu_enc_info_g5_index_ra_rb_fullgrp, self->iword)
  //    == SNOWHOUSECPU_G5_INDEX_RA_RB_FULLGRP_VALUE
  //)
  //{
  //  //printf(
  //  //  "testificate\n"
  //  //);
  //  self->length += 2;
  //  if ((self->status = self->rd16_func (self)))
  //  {
  //    return;
  //  }
  //  //printf ("testificate\n");
  //  //self->iword = (self->iword << SNOWHOUSECPU_ONE_EXT_BITPOS)
  //  //  | bfd_getl16 (self->buffer);
  //  self->have_index_ra_rb = true;
  //  //self->icreload_ra_ind = snowhousecpu_get_insn_field_ei
  //  //  (&snowhousecpu_enc_info_ra_ind, self->iword);
  //  self->index_ra_ind = snowhousecpu_get_insn_field_ei
  //    (&snowhousecpu_enc_info_ra_ind, self->iword);
  //  //self->rb_ind = snowhousecpu_get_insn_field_ei
  //  //  (&snowhousecpu_enc_info_rb_ind, self->iword);
  //  self->index_rb_ind = snowhousecpu_get_insn_field_ei 
  //    (&snowhousecpu_enc_info_rb_ind, self->iword);
  //  self->iword = bfd_getl16 (self->buffer);
  //}

  //if (snowhousecpu_get_insn_field_ei
  //  (&snowhousecpu_enc_info_g0_pre_fullgrp, self->iword)
  //  == SNOWHOUSECPU_G0_PRE_FULLGRP_VALUE)
  //{
  //  /* `pre` */
  //  self->length += 2;

  //  if ((self->status = self->rd16_func (self)))
  //  {
  //    return;
  //  }
  //  self->iword = (self->iword << SNOWHOUSECPU_ONE_EXT_BITPOS)
  //    | bfd_getl16 (self->buffer);
  //}
  //else if (snowhousecpu_get_insn_field_ei
  //  (&snowhousecpu_enc_info_g0_lpre_fullgrp_16, self->iword)
  //  == SNOWHOUSECPU_G0_LPRE_FULLGRP_VALUE)
  //{
  //  /* `lpre` */
  //  self->length += 2;
  //  if ((self->status = self->rd16_func (self)))
  //  {
  //    return;
  //  }
  //  self->iword = (self->iword << SNOWHOUSECPU_ONE_EXT_BITPOS)
  //    | bfd_getl16 (self->buffer);

  //  self->length += 2;

  //  if ((self->status = self->rd16_func (self)))
  //  {
  //    return;
  //  }
  //  self->iword = (self->iword << SNOWHOUSECPU_ONE_EXT_BITPOS)
  //    | bfd_getl16 (self->buffer);
  //}
  ////printf(
  ////  "iword: %lx\n",
  ////  self->iword
  ////);
  ////printf (
  ////  "debug:\nsg: %lu %lu %lx\nfg: %lu %lu %lx\n",
  ////  (snowhousecpu_temp_t) SNOWHOUSECPU_G5_INDEX_RA_SIMM_SUBGRP_BITPOS,
  ////  (snowhousecpu_temp_t) SNOWHOUSECPU_G5_INDEX_RA_SIMM_SUBGRP_BITSIZE,
  ////  (snowhousecpu_temp_t) SNOWHOUSECPU_G5_INDEX_RA_SIMM_SUBGRP_VALUE,
  ////  (snowhousecpu_temp_t) SNOWHOUSECPU_G5_INDEX_RA_SIMM_FULLGRP_BITPOS,
  ////  (snowhousecpu_temp_t) SNOWHOUSECPU_G5_INDEX_RA_SIMM_FULLGRP_BITSIZE,
  ////  (snowhousecpu_temp_t) SNOWHOUSECPU_G5_INDEX_RA_SIMM_FULLGRP_VALUE
  ////);

  //if (snowhousecpu_get_insn_field_ei
  //  (&snowhousecpu_enc_info_g5_index_ra_simm_fullgrp, self->iword)
  //  == SNOWHOUSECPU_G5_INDEX_RA_SIMM_FULLGRP_VALUE)
  //{
  //  self->length += 2;
  //  //printf (
  //  //  "have_index_ra_simm\n"
  //  //);

  //  if ((self->status = self->rd16_func (self)))
  //  {
  //    return;
  //  }

  //  //const size_t temp_length = (
  //  //  self->length
  //  //  - self->have_index_ra_rb * 2ull
  //  //);

  //  self->have_index_ra_simm = true;
  //  if (!self->have_index_ra_rb)
  //  {
  //    self->index_ra_ind = snowhousecpu_get_insn_field_ei
  //      (&snowhousecpu_enc_info_ra_ind, self->iword);
  //  }

  //  const size_t temp_length = (
  //    self->length
  //    - (self->have_index_ra_rb * 2ull)
  //    //- (self->have_index_ra_simm * 2ull)
  //  );

  //  if (temp_length == 2) /* no `pre` or `lpre` */
  //  {
  //    self->simm
  //      //= GET_INSN_FIELD (SNOWHOUSECPU_G1_S5_MASK,
  //      //SNOWHOUSECPU_G1_S5_BITPOS, self->iword);
  //      = snowhousecpu_sign_extend (snowhousecpu_get_insn_field_ei 
  //        (&snowhousecpu_enc_info_g5_index_ra_simm_s7, self->iword),
  //        snowhousecpu_enc_info_g5_index_ra_simm_s7.bitsize);
  //  }
  //  else if (temp_length == 4) /* `pre` */
  //  {
  //    self->simm = snowhousecpu_sign_extend (snowhousecpu_get_g5_index_s19
  //      ((self->iword >> SNOWHOUSECPU_ONE_EXT_BITPOS), self->iword),
  //      snowhousecpu_enc_info_g5_index_ra_simm_s7.bitsize
  //        + snowhousecpu_enc_info_g0_pre_s12.bitsize);
  //  }
  //  else if (temp_length == 6) /* `lpre` */
  //  {
  //    self->simm = snowhousecpu_sign_extend (snowhousecpu_get_g5_index_s32
  //      ((self->iword >> SNOWHOUSECPU_ONE_EXT_BITPOS), self->iword),
  //      snowhousecpu_enc_info_g5_index_ra_simm_s7.bitsize
  //        + snowhousecpu_enc_info_g0_lpre_s25.bitsize);
  //  }
  //  //printf (
  //  //  "self->simm: %lx\n",
  //  //  self->simm
  //  //);

  //  // This is to see the "main"/"primary" instruction
  //  self->iword = (self->iword << SNOWHOUSECPU_ONE_EXT_BITPOS)
  //    | bfd_getl16 (self->buffer);
  //}
  ////printf(
  ////  "iword: %lx\n",
  ////  self->iword
  ////);

  //self->length += 2;

  //self->grp
  //  //= GET_INSN_FIELD (snowhousecpu_enc_info_grp_16.mask,
  //  //  snowhousecpu_enc_info_grp_16.bitpos, self->iword);
  //  = snowhousecpu_get_insn_field_ei (&snowhousecpu_enc_info_grp_16, self->iword);
  //self->ra_ind
  //  //= GET_INSN_FIELD (snowhousecpu_enc_info_ra_ind.mask,
  //  //  snowhousecpu_enc_info_ra_ind.bitpos, self->iword);
  //  = snowhousecpu_get_insn_field_ei (&snowhousecpu_enc_info_ra_ind, self->iword);
  ////if (!self->have_index)
  //{
  //  self->rb_ind
  //    //= GET_INSN_FIELD (snowhousecpu_enc_info_rb_ind.mask,
  //    //  snowhousecpu_enc_info_rb_ind.bitpos, self->iword);
  //    = snowhousecpu_get_insn_field_ei (&snowhousecpu_enc_info_rb_ind, self->iword);
  //}

  //const size_t temp_length = (
  //  self->length
  //  - (self->have_index_ra_rb * 2ull)
  //  - (self->have_index_ra_simm * 2ull)
  //);

  //switch (self->grp)
  //{
  //  case SNOWHOUSECPU_G0_GRP_VALUE:
  //  {
  //    //if (self->length == 2)
  //    //{
  //    //}
  //    self->simm = 0;
  //    const snowhousecpu_temp_t l = snowhousecpu_get_insn_field_ei
  //      (&snowhousecpu_enc_info_g0_atomic_l, self->iword);
  //    self->opc_info = &snowhousecpu_opc_info_g0[
  //      //l + 2
  //      // we have `cmpxchg` when `have_index_ra_rb` == true
  //      l + (2 * self->have_index_ra_rb) + 2
  //    ];
  //    //self->opc_info = &snowhousecpu_opc_info_g0
  //    //  [];
  //  }
  //    break;
  //  /* -------- */
  //  case SNOWHOUSECPU_G1_GRP_VALUE:
  //  {
  //    /* -------- */
  //    if (temp_length == 2)
  //    {
  //      self->simm
  //        //= GET_INSN_FIELD (SNOWHOUSECPU_G1_S5_MASK,
  //        //SNOWHOUSECPU_G1_S5_BITPOS, self->iword);
  //        = snowhousecpu_sign_extend (snowhousecpu_get_insn_field_ei 
  //          (&snowhousecpu_enc_info_g1_i5, self->iword),
  //          snowhousecpu_enc_info_g1_i5.bitsize);
  //    }
  //    else if (temp_length == 4) /* `pre` */
  //    {
  //      self->simm = snowhousecpu_sign_extend (snowhousecpu_get_g1_s17
  //        ((self->iword >> SNOWHOUSECPU_ONE_EXT_BITPOS), self->iword),
  //        snowhousecpu_enc_info_g1_i5.bitsize
  //          + snowhousecpu_enc_info_g0_pre_s12.bitsize);
  //    }
  //    else if (temp_length == 6) /* `lpre` */
  //    {
  //      self->simm = snowhousecpu_sign_extend (snowhousecpu_get_g1_s32
  //        ((self->iword >> SNOWHOUSECPU_ONE_EXT_BITPOS), self->iword),
  //        snowhousecpu_enc_info_g1_i5.bitsize
  //          + snowhousecpu_enc_info_g0_lpre_s27.bitsize);
  //    }
  //    /* -------- */
  //    self->opc_info = &snowhousecpu_opc_info_g1
  //      //[GET_INSN_FIELD (SNOWHOUSECPU_G1_OP_MASK, SNOWHOUSECPU_G1_OP_BITPOS,
  //      //  self->iword)];
  //      [snowhousecpu_get_insn_field_ei (&snowhousecpu_enc_info_g1_op, self->iword)];
  //    /* -------- */
  //    //do_print_insn_snowhousecpu
  //    //  (self->opc_info,
  //    //  self->iword,
  //    //  self->simm,
  //    //  self->grp,
  //    //  self->ra_ind,
  //    //  self->rb_ind,
  //    //  self->fwl,
  //    //  self->length);
  //    /* -------- */
  //  }
  //    break;
  //  /* -------- */
  //  case SNOWHOUSECPU_G2_GRP_VALUE:
  //  {
  //    /* -------- */
  //    self->opc_info = &snowhousecpu_opc_info_g2
  //      //[GET_INSN_FIELD (SNOWHOUSECPU_G2_OP_MASK, SNOWHOUSECPU_G2_OP_BITPOS,
  //      //  self->iword)];
  //      [snowhousecpu_get_insn_field_ei (&snowhousecpu_enc_info_g2_op, self->iword)];

  //    self->fw
  //      //= GET_INSN_FIELD (SNOWHOUSECPU_G2_F_MASK, SNOWHOUSECPU_G2_F_BITPOS,
  //      //  self->iword);
  //      = snowhousecpu_get_insn_field_ei (&snowhousecpu_enc_info_g2_f, self->iword);
  //    /* -------- */
  //    //do_print_insn_snowhousecpu
  //    //  (self->opc_info,
  //    //  self->iword,
  //    //  self->simm,
  //    //  self->grp,
  //    //  self->ra_ind,
  //    //  self->rb_ind,
  //    //  self->fwl,
  //    //  self->length);
  //    /* -------- */
  //  }
  //    break;
  //  /* -------- */
  //  case SNOWHOUSECPU_G3_GRP_VALUE:
  //  {
  //    /* -------- */
  //    if (temp_length == 2)
  //    {
  //      self->simm
  //        //= GET_INSN_FIELD (SNOWHOUSECPU_G1_S5_MASK,
  //        //SNOWHOUSECPU_G1_S5_BITPOS, self->iword);
  //        = snowhousecpu_sign_extend (snowhousecpu_get_insn_field_ei 
  //          (&snowhousecpu_enc_info_g3_s9, self->iword),
  //          snowhousecpu_enc_info_g3_s9.bitsize);
  //    }
  //    else if (temp_length == 4) /* `pre` */
  //    {
  //      self->simm = snowhousecpu_sign_extend (snowhousecpu_get_g3_s21
  //        ((self->iword >> SNOWHOUSECPU_ONE_EXT_BITPOS), self->iword),
  //        snowhousecpu_enc_info_g3_s9.bitsize
  //          + snowhousecpu_enc_info_g0_pre_s12.bitsize);
  //      //fpr (stream,
  //      //  "testificate: %lu\n",
  //      //  self->simm);
  //    }
  //    else if (temp_length == 6) /* `lpre` */
  //    {
  //      self->simm = snowhousecpu_sign_extend (snowhousecpu_get_g3_s32
  //        ((self->iword >> SNOWHOUSECPU_ONE_EXT_BITPOS), self->iword),
  //        snowhousecpu_enc_info_g3_s9.bitsize
  //          + snowhousecpu_enc_info_g0_lpre_s23.bitsize);
  //    }
  //    /* -------- */
  //    self->opc_info = &snowhousecpu_opc_info_g3
  //      //[GET_INSN_FIELD (SNOWHOUSECPU_G3_OP_MASK, SNOWHOUSECPU_G3_OP_BITPOS,
  //      //  self->iword)];
  //      [snowhousecpu_get_insn_field_ei (&snowhousecpu_enc_info_g3_op, self->iword)];
  //    /* -------- */
  //    //do_print_insn_snowhousecpu
  //    //  (self->opc_info,
  //    //  self->iword,
  //    //  self->simm,
  //    //  self->grp,
  //    //  self->ra_ind,
  //    //  self->rb_ind,
  //    //  self->fwl,
  //    //  self->length);
  //    /* -------- */
  //  }
  //    break;
  //  /* -------- */
  //  case SNOWHOUSECPU_G4_GRP_VALUE:
  //  {
  //    /* -------- */
  //    //printf (
  //    //  "here's a string\n"
  //    //);
  //    self->opc_info = &snowhousecpu_opc_info_g4
  //      //[GET_INSN_FIELD (SNOWHOUSECPU_G4_OP_MASK, SNOWHOUSECPU_G4_OP_BITPOS,
  //      //  self->iword)];
  //      [snowhousecpu_get_insn_field_ei (&snowhousecpu_enc_info_g4_op, self->iword)];
  //    /* -------- */
  //    //do_print_insn_snowhousecpu
  //    //  (self->opc_info,
  //    //  self->iword,
  //    //  self->simm,
  //    //  self->grp,
  //    //  self->ra_ind,
  //    //  self->rb_ind,
  //    //  self->fwl,
  //    //  self->length);
  //    /* -------- */
  //  }
  //    break;
  //  /* -------- */
  //  case SNOWHOUSECPU_G5_GRP_VALUE:
  //  {
  //    ///* -------- */
  //    //if (temp_length == 2)
  //    //{
  //    //  self->simm
  //    //    //= GET_INSN_FIELD (SNOWHOUSECPU_G1_S5_MASK,
  //    //    //SNOWHOUSECPU_G1_S5_BITPOS, self->iword);
  //    //    = snowhousecpu_sign_extend (snowhousecpu_get_insn_field_ei 
  //    //      (&snowhousecpu_enc_info_g1_i5, self->iword),
  //    //      snowhousecpu_enc_info_g1_i5.bitsize);
  //    //}
  //    //else if (temp_length == 4) /* `pre` */
  //    //{
  //    //  self->simm = snowhousecpu_sign_extend (snowhousecpu_get_g1_s17
  //    //    ((self->iword >> SNOWHOUSECPU_ONE_EXT_BITPOS), self->iword),
  //    //    snowhousecpu_enc_info_g1_i5.bitsize
  //    //      + snowhousecpu_enc_info_g0_pre_s12.bitsize);
  //    //}
  //    //else if (temp_length == 6) /* `lpre` */
  //    //{
  //    //  self->simm = snowhousecpu_sign_extend (snowhousecpu_get_g1_s32
  //    //    ((self->iword >> SNOWHOUSECPU_ONE_EXT_BITPOS), self->iword),
  //    //    snowhousecpu_enc_info_g1_i5.bitsize
  //    //      + snowhousecpu_enc_info_g0_lpre_s27.bitsize);
  //    //}
  //    ///* -------- */
  //    //self->opc_info = &snowhousecpu_opc_info_g5[0];

  //    /* -------- */
  //    //do_print_insn_snowhousecpu
  //    //  (self->opc_info,
  //    //  self->iword,
  //    //  self->simm,
  //    //  self->grp,
  //    //  self->ra_ind,
  //    //  self->rb_ind,
  //    //  self->fwl,
  //    //  self->length);
  //    self->grp_decode_err = true;
  //  }
  //    break;
  //  /* -------- */
  //  case SNOWHOUSECPU_G6_GRP_VALUE:
  //  {
  //    /* -------- */
  //    //if (temp_length == 2)
  //    //{
  //    //  self->simm
  //    //    //= GET_INSN_FIELD (SNOWHOUSECPU_G1_S5_MASK,
  //    //    //SNOWHOUSECPU_G1_S5_BITPOS, self->iword);
  //    //    = snowhousecpu_sign_extend (snowhousecpu_get_insn_field_ei 
  //    //      (&snowhousecpu_enc_info_g1_i5, self->iword),
  //    //      snowhousecpu_enc_info_g1_i5.bitsize);
  //    //}
  //    //else if (temp_length == 4) /* `pre` */
  //    //{
  //    //  self->simm = snowhousecpu_sign_extend (snowhousecpu_get_g1_s17
  //    //    ((self->iword >> SNOWHOUSECPU_ONE_EXT_BITPOS), self->iword),
  //    //    snowhousecpu_enc_info_g1_i5.bitsize
  //    //      + snowhousecpu_enc_info_g0_pre_s12.bitsize);
  //    //}
  //    //else if (temp_length == 6) /* `lpre` */
  //    //{
  //    //  self->simm = snowhousecpu_sign_extend (snowhousecpu_get_g1_s32
  //    //    ((self->iword >> SNOWHOUSECPU_ONE_EXT_BITPOS), self->iword),
  //    //    snowhousecpu_enc_info_g1_i5.bitsize
  //    //      + snowhousecpu_enc_info_g0_lpre_s27.bitsize);
  //    //}
  //    ///* -------- */
  //    //self->opc_info = &snowhousecpu_opc_info_g6[0];
  //    /* -------- */
  //    //do_print_insn_snowhousecpu
  //    //  (self->opc_info,
  //    //  self->iword,
  //    //  self->simm,
  //    //  self->grp,
  //    //  self->ra_ind,
  //    //  self->rb_ind,
  //    //  self->fwl,
  //    //  self->length);
  //    /* -------- */
  //    self->grp_decode_err = true;
  //  }
  //    break;
  //  /* -------- */
  //  case SNOWHOUSECPU_G7_GRP_VALUE:
  //  {
  //    /* -------- */
  //    //snowhousecpu_temp_t
  //    //  g7_aluopbh_subgrp, g7_sprldst_subgrp;
  //    self->g7_aluopbh_subgrp = snowhousecpu_get_insn_field_ei
  //      (&snowhousecpu_enc_info_g7_aluopbh_subgrp, self->iword);
  //    self->g7_sprldst_subgrp = snowhousecpu_get_insn_field_ei
  //      (&snowhousecpu_enc_info_g7_sprldst_subgrp, self->iword);
  //    self->g7_icreload_subgrp = snowhousecpu_get_insn_field_ei
  //      (&snowhousecpu_enc_info_g7_icreload_subgrp, self->iword);
  //    self->g7_icflush_subgrp = snowhousecpu_get_insn_field_ei
  //      (&snowhousecpu_enc_info_g7_icflush_subgrp, self->iword);
  //    //if (self->g7_aluopbh_subgrp != SNOWHOUSECPU_G7_ALUOPBH_SUBGRP_VALUE
  //    //  && self->g7_sprldst_subgrp != SNOWHOUSECPU_G7_SPRLDST_SUBGRP_VALUE
  //    //  && self->g7_icreload_subgrp != SNOWHOUSECPU_G7_ICRELOAD_SUBGRP_VALUE)
  //    //{
  //    //  break;
  //    //}
  //    if (self->g7_aluopbh_subgrp == SNOWHOUSECPU_G7_ALUOPBH_SUBGRP_VALUE)
  //    {
  //      self->opc_info = &snowhousecpu_opc_info_g7_aluopbh
  //        [snowhousecpu_get_insn_field_ei (&snowhousecpu_enc_info_g7_aluopbh_op,
  //          self->iword)];
  //      self->fw = snowhousecpu_get_insn_field_ei
  //        (&snowhousecpu_enc_info_g7_aluopbh_w, self->iword);
  //    }
  //    else if (self->g7_sprldst_subgrp == SNOWHOUSECPU_G7_SPRLDST_SUBGRP_VALUE)
  //    {
  //      self->opc_info = &snowhousecpu_opc_info_g7_sprldst
  //        [snowhousecpu_get_insn_field_ei
  //          (&snowhousecpu_enc_info_g7_sprldst_op, self->iword)];
  //    }
  //    else if (
  //      self->g7_icreload_subgrp == SNOWHOUSECPU_G7_ICRELOAD_SUBGRP_VALUE
  //    )
  //    {
  //      self->opc_info = &snowhousecpu_opc_info_g7_icreload[0];

  //      if (temp_length == 2)
  //      {
  //        self->simm
  //          //= GET_INSN_FIELD (SNOWHOUSECPU_G1_S5_MASK,
  //          //SNOWHOUSECPU_G1_S5_BITPOS, self->iword);
  //          = snowhousecpu_sign_extend (snowhousecpu_get_insn_field_ei 
  //            (&snowhousecpu_enc_info_g7_icreload_s5, self->iword),
  //            snowhousecpu_enc_info_g7_icreload_s5.bitsize);
  //        //printf
  //        //  ("self->simm: %lx\n", self->simm);
  //      }
  //      else if (temp_length == 4) /* `pre` */
  //      {
  //        self->simm = snowhousecpu_sign_extend (snowhousecpu_get_g7_icreload_s17
  //          ((self->iword >> SNOWHOUSECPU_ONE_EXT_BITPOS), self->iword),
  //          snowhousecpu_enc_info_g7_icreload_s5.bitsize
  //            + snowhousecpu_enc_info_g0_pre_s12.bitsize);
  //      }
  //      else if (temp_length == 6) /* `lpre` */
  //      {
  //        self->simm = snowhousecpu_sign_extend (snowhousecpu_get_g7_icreload_s32
  //          ((self->iword >> SNOWHOUSECPU_ONE_EXT_BITPOS), self->iword),
  //          snowhousecpu_enc_info_g7_icreload_s5.bitsize
  //            + snowhousecpu_enc_info_g0_lpre_s27.bitsize);
  //      }
  //    }
  //    else if (
  //      self->g7_icflush_subgrp == SNOWHOUSECPU_G7_ICFLUSH_SUBGRP_VALUE
  //    )
  //    {
  //      self->opc_info = &snowhousecpu_opc_info_g7_icflush[0];
  //    }
  //    else
  //    {
  //      fprintf 
  //        (stderr,
  //        "invalid group 7 instruction subgroup! %lx\n",
  //        self->iword);
  //      exit(1);
  //      break;
  //    }
  //    //do_print_insn_snowhousecpu
  //    //  (self->opc_info,
  //    //  self->iword,
  //    //  self->simm,
  //    //  self->grp,
  //    //  self->ra_ind,
  //    //  self->rb_ind,
  //    //  self->fwl,
  //    //  self->length);
  //    /* -------- */
  //  }
  //    break;
  //  /* -------- */
  //  default:
  //    //fpr (stream, "bad (unknown self->grp 0x%x)",
  //    //  (unsigned) self->grp);
  //    self->grp_decode_err = true;
  //    break;
  //}

  //return self->length;
}
#endif    // _SNOWHOUSECPU_DASM_INFO_FUNCS_H_

/* flare-disasm-info-funcs.h -- Functions for disassembling flare
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

#ifndef _FLARE_DASM_INFO_FUNCS_H_
#define _FLARE_DASM_INFO_FUNCS_H_

#ifdef FLARE_CLANGD
#include <stdio.h>
#include "flare.h"
#include "flare-opc-decls.h"
#endif

static inline void
flare_dasm_info_ctor (flare_dasm_info_t *self,
  flare_dasm_info_rd16_func rd16_func)
{
  memset (self, 0, sizeof (*self));
  self->rd16_func = rd16_func;
}

static void
flare_dasm_info_do_disassemble (flare_dasm_info_t *self)
{
  //flare_dasm_info_t dasm_info;
  //memset (&dasm_info, 0, sizeof (dasm_info));
  //dasm_info.rd16_func = rd16_func;

  //const flare_enc_info_t
  //  *self->grp = NULL,
  //  *subgrp = NULL;
  //unsigned
  //  grp_value = 0,
  //  subgrp_value = 0;
  //const flare_opc_info_t
  //  *self->opc_info = (const flare_opc_info_t *)NULL;

  //if ((status = info->read_memory_func (addr + self->length, self->buffer, 2, 
  //  info)))
  //{
  //  goto fail;
  //}
  if ((self->status = self->rd16_func (self)))
  {
    return;
  }
  self->iword = bfd_getl16 (self->buffer);

  if (
    //(flare_get_insn_field_ei
    //  (&flare_enc_info_grp_16, self->iword)
    //  == FLARE_G4_GRP_VALUE
    //)
    //&& (flare_get_insn_field_ei
    //  (&flare_enc_info_g4_op, self->iword)
    //  == FLARE_G4_OP_ENUM_INDEX_RA_RB))
    flare_get_insn_field_ei
      (&flare_enc_info_g5_index_ra_rb_fullgrp, self->iword)
      == FLARE_G5_INDEX_RA_RB_FULLGRP_VALUE
  )
  {
    //printf(
    //  "testificate\n"
    //);
    self->length += 2;
    if ((self->status = self->rd16_func (self)))
    {
      return;
    }
    //printf ("testificate\n");
    //self->iword = (self->iword << FLARE_ONE_EXT_BITPOS)
    //  | bfd_getl16 (self->buffer);
    self->have_index_ra_rb = true;
    //self->icreload_ra_ind = flare_get_insn_field_ei
    //  (&flare_enc_info_ra_ind, self->iword);
    self->index_ra_ind = flare_get_insn_field_ei
      (&flare_enc_info_ra_ind, self->iword);
    //self->rb_ind = flare_get_insn_field_ei
    //  (&flare_enc_info_rb_ind, self->iword);
    self->index_rb_ind = flare_get_insn_field_ei 
      (&flare_enc_info_rb_ind, self->iword);
    self->iword = bfd_getl16 (self->buffer);
  }

  if (flare_get_insn_field_ei
    (&flare_enc_info_g0_pre_fullgrp, self->iword)
    == FLARE_G0_PRE_FULLGRP_VALUE)
  {
    /* `pre` */
    self->length += 2;

    if ((self->status = self->rd16_func (self)))
    {
      return;
    }
    self->iword = (self->iword << FLARE_ONE_EXT_BITPOS)
      | bfd_getl16 (self->buffer);
  }
  else if (flare_get_insn_field_ei
    (&flare_enc_info_g0_lpre_fullgrp_16, self->iword)
    == FLARE_G0_LPRE_FULLGRP_VALUE)
  {
    /* `lpre` */
    self->length += 2;
    if ((self->status = self->rd16_func (self)))
    {
      return;
    }
    self->iword = (self->iword << FLARE_ONE_EXT_BITPOS)
      | bfd_getl16 (self->buffer);

    self->length += 2;

    if ((self->status = self->rd16_func (self)))
    {
      return;
    }
    self->iword = (self->iword << FLARE_ONE_EXT_BITPOS)
      | bfd_getl16 (self->buffer);
  }
  //printf(
  //  "iword: %lx\n",
  //  self->iword
  //);
  //printf (
  //  "debug:\nsg: %lu %lu %lx\nfg: %lu %lu %lx\n",
  //  (flare_temp_t) FLARE_G5_INDEX_RA_SIMM_SUBGRP_BITPOS,
  //  (flare_temp_t) FLARE_G5_INDEX_RA_SIMM_SUBGRP_BITSIZE,
  //  (flare_temp_t) FLARE_G5_INDEX_RA_SIMM_SUBGRP_VALUE,
  //  (flare_temp_t) FLARE_G5_INDEX_RA_SIMM_FULLGRP_BITPOS,
  //  (flare_temp_t) FLARE_G5_INDEX_RA_SIMM_FULLGRP_BITSIZE,
  //  (flare_temp_t) FLARE_G5_INDEX_RA_SIMM_FULLGRP_VALUE
  //);

  if (flare_get_insn_field_ei
    (&flare_enc_info_g5_index_ra_simm_fullgrp, self->iword)
    == FLARE_G5_INDEX_RA_SIMM_FULLGRP_VALUE)
  {
    self->length += 2;
    //printf (
    //  "have_index_ra_simm\n"
    //);

    if ((self->status = self->rd16_func (self)))
    {
      return;
    }

    //const size_t temp_length = (
    //  self->length
    //  - self->have_index_ra_rb * 2ull
    //);

    self->have_index_ra_simm = true;
    if (!self->have_index_ra_rb)
    {
      self->index_ra_ind = flare_get_insn_field_ei
	(&flare_enc_info_ra_ind, self->iword);
    }

    const size_t temp_length = (
      self->length
      - (self->have_index_ra_rb * 2ull)
      //- (self->have_index_ra_simm * 2ull)
    );

    if (temp_length == 2) /* no `pre` or `lpre` */
    {
      self->simm
	//= GET_INSN_FIELD (FLARE_G1_S5_MASK,
	//FLARE_G1_S5_BITPOS, self->iword);
	= flare_sign_extend (flare_get_insn_field_ei 
	  (&flare_enc_info_g5_index_ra_simm_s7, self->iword),
	  flare_enc_info_g5_index_ra_simm_s7.bitsize);
    }
    else if (temp_length == 4) /* `pre` */
    {
      self->simm = flare_sign_extend (flare_get_g5_index_s19
	((self->iword >> FLARE_ONE_EXT_BITPOS), self->iword),
	flare_enc_info_g5_index_ra_simm_s7.bitsize
	  + flare_enc_info_g0_pre_s12.bitsize);
    }
    else if (temp_length == 6) /* `lpre` */
    {
      self->simm = flare_sign_extend (flare_get_g5_index_s32
	((self->iword >> FLARE_ONE_EXT_BITPOS), self->iword),
	flare_enc_info_g5_index_ra_simm_s7.bitsize
	  + flare_enc_info_g0_lpre_s25.bitsize);
    }
    //printf (
    //  "self->simm: %lx\n",
    //  self->simm
    //);

    // This is to see the "main"/"primary" instruction
    self->iword = (self->iword << FLARE_ONE_EXT_BITPOS)
      | bfd_getl16 (self->buffer);
  }
  //printf(
  //  "iword: %lx\n",
  //  self->iword
  //);

  self->length += 2;

  self->grp
    //= GET_INSN_FIELD (flare_enc_info_grp_16.mask,
    //  flare_enc_info_grp_16.bitpos, self->iword);
    = flare_get_insn_field_ei (&flare_enc_info_grp_16, self->iword);
  self->ra_ind
    //= GET_INSN_FIELD (flare_enc_info_ra_ind.mask,
    //  flare_enc_info_ra_ind.bitpos, self->iword);
    = flare_get_insn_field_ei (&flare_enc_info_ra_ind, self->iword);
  //if (!self->have_index)
  {
    self->rb_ind
      //= GET_INSN_FIELD (flare_enc_info_rb_ind.mask,
      //  flare_enc_info_rb_ind.bitpos, self->iword);
      = flare_get_insn_field_ei (&flare_enc_info_rb_ind, self->iword);
  }

  const size_t temp_length = (
    self->length
    - (self->have_index_ra_rb * 2ull)
    - (self->have_index_ra_simm * 2ull)
  );

  switch (self->grp)
  {
    case FLARE_G0_GRP_VALUE:
    {
      //if (self->length == 2)
      //{
      //}
      self->simm = 0;
      const flare_temp_t l = flare_get_insn_field_ei
        (&flare_enc_info_g0_atomic_l, self->iword);
      self->opc_info = &flare_opc_info_g0[
        //l + 2
        // we have `cmpxchg` when `have_index_ra_rb` == true
        l + (2 * self->have_index_ra_rb) + 2
      ];
      //self->opc_info = &flare_opc_info_g0
      //  [];
    }
      break;
    /* -------- */
    case FLARE_G1_GRP_VALUE:
    {
      /* -------- */
      if (temp_length == 2)
      {
        self->simm
          //= GET_INSN_FIELD (FLARE_G1_S5_MASK,
          //FLARE_G1_S5_BITPOS, self->iword);
          = flare_sign_extend (flare_get_insn_field_ei 
            (&flare_enc_info_g1_i5, self->iword),
            flare_enc_info_g1_i5.bitsize);
      }
      else if (temp_length == 4) /* `pre` */
      {
        self->simm = flare_sign_extend (flare_get_g1_s17
          ((self->iword >> FLARE_ONE_EXT_BITPOS), self->iword),
          flare_enc_info_g1_i5.bitsize
            + flare_enc_info_g0_pre_s12.bitsize);
      }
      else if (temp_length == 6) /* `lpre` */
      {
        self->simm = flare_sign_extend (flare_get_g1_s32
          ((self->iword >> FLARE_ONE_EXT_BITPOS), self->iword),
          flare_enc_info_g1_i5.bitsize
            + flare_enc_info_g0_lpre_s27.bitsize);
      }
      /* -------- */
      self->opc_info = &flare_opc_info_g1
        //[GET_INSN_FIELD (FLARE_G1_OP_MASK, FLARE_G1_OP_BITPOS,
        //  self->iword)];
        [flare_get_insn_field_ei (&flare_enc_info_g1_op, self->iword)];
      /* -------- */
      //do_print_insn_flare
      //  (self->opc_info,
      //  self->iword,
      //  self->simm,
      //  self->grp,
      //  self->ra_ind,
      //  self->rb_ind,
      //  self->fwl,
      //  self->length);
      /* -------- */
    }
      break;
    /* -------- */
    case FLARE_G2_GRP_VALUE:
    {
      /* -------- */
      self->opc_info = &flare_opc_info_g2
        //[GET_INSN_FIELD (FLARE_G2_OP_MASK, FLARE_G2_OP_BITPOS,
        //  self->iword)];
        [flare_get_insn_field_ei (&flare_enc_info_g2_op, self->iword)];

      self->fw
        //= GET_INSN_FIELD (FLARE_G2_F_MASK, FLARE_G2_F_BITPOS,
        //  self->iword);
        = flare_get_insn_field_ei (&flare_enc_info_g2_f, self->iword);
      /* -------- */
      //do_print_insn_flare
      //  (self->opc_info,
      //  self->iword,
      //  self->simm,
      //  self->grp,
      //  self->ra_ind,
      //  self->rb_ind,
      //  self->fwl,
      //  self->length);
      /* -------- */
    }
      break;
    /* -------- */
    case FLARE_G3_GRP_VALUE:
    {
      /* -------- */
      if (temp_length == 2)
      {
        self->simm
          //= GET_INSN_FIELD (FLARE_G1_S5_MASK,
          //FLARE_G1_S5_BITPOS, self->iword);
          = flare_sign_extend (flare_get_insn_field_ei 
            (&flare_enc_info_g3_s9, self->iword),
            flare_enc_info_g3_s9.bitsize);
      }
      else if (temp_length == 4) /* `pre` */
      {
        self->simm = flare_sign_extend (flare_get_g3_s21
          ((self->iword >> FLARE_ONE_EXT_BITPOS), self->iword),
          flare_enc_info_g3_s9.bitsize
            + flare_enc_info_g0_pre_s12.bitsize);
        //fpr (stream,
        //  "testificate: %lu\n",
        //  self->simm);
      }
      else if (temp_length == 6) /* `lpre` */
      {
        self->simm = flare_sign_extend (flare_get_g3_s32
          ((self->iword >> FLARE_ONE_EXT_BITPOS), self->iword),
          flare_enc_info_g3_s9.bitsize
            + flare_enc_info_g0_lpre_s23.bitsize);
      }
      /* -------- */
      self->opc_info = &flare_opc_info_g3
        //[GET_INSN_FIELD (FLARE_G3_OP_MASK, FLARE_G3_OP_BITPOS,
        //  self->iword)];
        [flare_get_insn_field_ei (&flare_enc_info_g3_op, self->iword)];
      /* -------- */
      //do_print_insn_flare
      //  (self->opc_info,
      //  self->iword,
      //  self->simm,
      //  self->grp,
      //  self->ra_ind,
      //  self->rb_ind,
      //  self->fwl,
      //  self->length);
      /* -------- */
    }
      break;
    /* -------- */
    case FLARE_G4_GRP_VALUE:
    {
      /* -------- */
      //printf (
      //  "here's a string\n"
      //);
      self->opc_info = &flare_opc_info_g4
        //[GET_INSN_FIELD (FLARE_G4_OP_MASK, FLARE_G4_OP_BITPOS,
        //  self->iword)];
        [flare_get_insn_field_ei (&flare_enc_info_g4_op, self->iword)];
      /* -------- */
      //do_print_insn_flare
      //  (self->opc_info,
      //  self->iword,
      //  self->simm,
      //  self->grp,
      //  self->ra_ind,
      //  self->rb_ind,
      //  self->fwl,
      //  self->length);
      /* -------- */
    }
      break;
    /* -------- */
    case FLARE_G5_GRP_VALUE:
    {
      ///* -------- */
      //if (temp_length == 2)
      //{
      //  self->simm
      //    //= GET_INSN_FIELD (FLARE_G1_S5_MASK,
      //    //FLARE_G1_S5_BITPOS, self->iword);
      //    = flare_sign_extend (flare_get_insn_field_ei 
      //      (&flare_enc_info_g1_i5, self->iword),
      //      flare_enc_info_g1_i5.bitsize);
      //}
      //else if (temp_length == 4) /* `pre` */
      //{
      //  self->simm = flare_sign_extend (flare_get_g1_s17
      //    ((self->iword >> FLARE_ONE_EXT_BITPOS), self->iword),
      //    flare_enc_info_g1_i5.bitsize
      //      + flare_enc_info_g0_pre_s12.bitsize);
      //}
      //else if (temp_length == 6) /* `lpre` */
      //{
      //  self->simm = flare_sign_extend (flare_get_g1_s32
      //    ((self->iword >> FLARE_ONE_EXT_BITPOS), self->iword),
      //    flare_enc_info_g1_i5.bitsize
      //      + flare_enc_info_g0_lpre_s27.bitsize);
      //}
      ///* -------- */
      //self->opc_info = &flare_opc_info_g5[0];

      /* -------- */
      //do_print_insn_flare
      //  (self->opc_info,
      //  self->iword,
      //  self->simm,
      //  self->grp,
      //  self->ra_ind,
      //  self->rb_ind,
      //  self->fwl,
      //  self->length);
      self->grp_decode_err = true;
    }
      break;
    /* -------- */
    case FLARE_G6_GRP_VALUE:
    {
      /* -------- */
      //if (temp_length == 2)
      //{
      //  self->simm
      //    //= GET_INSN_FIELD (FLARE_G1_S5_MASK,
      //    //FLARE_G1_S5_BITPOS, self->iword);
      //    = flare_sign_extend (flare_get_insn_field_ei 
      //      (&flare_enc_info_g1_i5, self->iword),
      //      flare_enc_info_g1_i5.bitsize);
      //}
      //else if (temp_length == 4) /* `pre` */
      //{
      //  self->simm = flare_sign_extend (flare_get_g1_s17
      //    ((self->iword >> FLARE_ONE_EXT_BITPOS), self->iword),
      //    flare_enc_info_g1_i5.bitsize
      //      + flare_enc_info_g0_pre_s12.bitsize);
      //}
      //else if (temp_length == 6) /* `lpre` */
      //{
      //  self->simm = flare_sign_extend (flare_get_g1_s32
      //    ((self->iword >> FLARE_ONE_EXT_BITPOS), self->iword),
      //    flare_enc_info_g1_i5.bitsize
      //      + flare_enc_info_g0_lpre_s27.bitsize);
      //}
      ///* -------- */
      //self->opc_info = &flare_opc_info_g6[0];
      /* -------- */
      //do_print_insn_flare
      //  (self->opc_info,
      //  self->iword,
      //  self->simm,
      //  self->grp,
      //  self->ra_ind,
      //  self->rb_ind,
      //  self->fwl,
      //  self->length);
      /* -------- */
      self->grp_decode_err = true;
    }
      break;
    /* -------- */
    case FLARE_G7_GRP_VALUE:
    {
      /* -------- */
      //flare_temp_t
      //  g7_aluopbh_subgrp, g7_sprldst_subgrp;
      self->g7_aluopbh_subgrp = flare_get_insn_field_ei
        (&flare_enc_info_g7_aluopbh_subgrp, self->iword);
      self->g7_sprldst_subgrp = flare_get_insn_field_ei
        (&flare_enc_info_g7_sprldst_subgrp, self->iword);
      self->g7_icreload_subgrp = flare_get_insn_field_ei
        (&flare_enc_info_g7_icreload_subgrp, self->iword);
      self->g7_icflush_subgrp = flare_get_insn_field_ei
        (&flare_enc_info_g7_icflush_subgrp, self->iword);
      //if (self->g7_aluopbh_subgrp != FLARE_G7_ALUOPBH_SUBGRP_VALUE
      //  && self->g7_sprldst_subgrp != FLARE_G7_SPRLDST_SUBGRP_VALUE
      //  && self->g7_icreload_subgrp != FLARE_G7_ICRELOAD_SUBGRP_VALUE)
      //{
      //  break;
      //}
      if (self->g7_aluopbh_subgrp == FLARE_G7_ALUOPBH_SUBGRP_VALUE)
      {
        self->opc_info = &flare_opc_info_g7_aluopbh
          [flare_get_insn_field_ei (&flare_enc_info_g7_aluopbh_op,
            self->iword)];
        self->fw = flare_get_insn_field_ei
          (&flare_enc_info_g7_aluopbh_w, self->iword);
      }
      else if (self->g7_sprldst_subgrp == FLARE_G7_SPRLDST_SUBGRP_VALUE)
      {
        self->opc_info = &flare_opc_info_g7_sprldst
          [flare_get_insn_field_ei
            (&flare_enc_info_g7_sprldst_op, self->iword)];
      }
      else if (
        self->g7_icreload_subgrp == FLARE_G7_ICRELOAD_SUBGRP_VALUE
      )
      {
        self->opc_info = &flare_opc_info_g7_icreload[0];

        if (temp_length == 2)
        {
          self->simm
            //= GET_INSN_FIELD (FLARE_G1_S5_MASK,
            //FLARE_G1_S5_BITPOS, self->iword);
            = flare_sign_extend (flare_get_insn_field_ei 
              (&flare_enc_info_g7_icreload_s5, self->iword),
              flare_enc_info_g7_icreload_s5.bitsize);
          //printf
	  //  ("self->simm: %lx\n", self->simm);
        }
        else if (temp_length == 4) /* `pre` */
        {
          self->simm = flare_sign_extend (flare_get_g7_icreload_s17
            ((self->iword >> FLARE_ONE_EXT_BITPOS), self->iword),
            flare_enc_info_g7_icreload_s5.bitsize
              + flare_enc_info_g0_pre_s12.bitsize);
        }
        else if (temp_length == 6) /* `lpre` */
        {
          self->simm = flare_sign_extend (flare_get_g7_icreload_s32
            ((self->iword >> FLARE_ONE_EXT_BITPOS), self->iword),
            flare_enc_info_g7_icreload_s5.bitsize
              + flare_enc_info_g0_lpre_s27.bitsize);
        }
      }
      else if (
        self->g7_icflush_subgrp == FLARE_G7_ICFLUSH_SUBGRP_VALUE
      )
      {
        self->opc_info = &flare_opc_info_g7_icflush[0];
      }
      else
      {
	fprintf 
	  (stderr,
	  "invalid group 7 instruction subgroup! %lx\n",
	  self->iword);
	exit(1);
        break;
      }
      //do_print_insn_flare
      //  (self->opc_info,
      //  self->iword,
      //  self->simm,
      //  self->grp,
      //  self->ra_ind,
      //  self->rb_ind,
      //  self->fwl,
      //  self->length);
      /* -------- */
    }
      break;
    /* -------- */
    default:
      //fpr (stream, "bad (unknown self->grp 0x%x)",
      //  (unsigned) self->grp);
      self->grp_decode_err = true;
      break;
  }

  //return self->length;
}
#endif    // _FLARE_DASM_INFO_FUNCS_H_

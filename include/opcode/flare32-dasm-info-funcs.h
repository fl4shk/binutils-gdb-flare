/* flare32-disasm-info-funcs.h -- Functions for disassembling flare32
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

#ifndef _FLARE32_DASM_INFO_FUNCS_H_
#define _FLARE32_DASM_INFO_FUNCS_H_

static inline void
flare32_dasm_info_ctor (flare32_dasm_info_t *self,
  flare32_dasm_info_rd16_func rd16_func)
{
  memset (self, 0, sizeof (*self));
  self->rd16_func = rd16_func;
}

static void
flare32_dasm_info_do_disassemble (flare32_dasm_info_t *self)
{
  //flare32_dasm_info_t dasm_info;
  //memset (&dasm_info, 0, sizeof (dasm_info));
  //dasm_info.rd16_func = rd16_func;

  //const flare32_enc_info_t
  //  *self->grp = NULL,
  //  *subgrp = NULL;
  //unsigned
  //  grp_value = 0,
  //  subgrp_value = 0;
  //const flare32_opc_info_t
  //  *self->opc_info = (const flare32_opc_info_t *)NULL;

  //if ((status = info->read_memory_func (addr + self->length, self->buffer, 2, 
  //  info)))
  //{
  //  goto fail;
  //}
  if ((self->status = self->rd16_func (self)))
  {
    return;
  }
  self->iword = bfd_getb16 (self->buffer);

  if (flare32_get_insn_field_ei
    (&flare32_enc_info_g0_pre_fullgrp, self->iword)
    == FLARE32_G0_PRE_FULLGRP_VALUE)
  {
    /* `pre` */
    self->length += 2;

    if ((self->status = self->rd16_func (self)))
    {
      return;
    }
    self->iword = (self->iword << FLARE32_ONE_EXT_BITPOS)
      | bfd_getb16 (self->buffer);
  }
  else if (flare32_get_insn_field_ei
    (&flare32_enc_info_g0_lpre_fullgrp_16, self->iword)
    == FLARE32_G0_LPRE_FULLGRP_VALUE)
  {
    /* `lpre` */
    self->length += 2;
    if ((self->status = self->rd16_func (self)))
    {
      return;
    }
    self->iword = (self->iword << FLARE32_ONE_EXT_BITPOS)
      | bfd_getb16 (self->buffer);

    self->length += 2;

    if ((self->status = self->rd16_func (self)))
    {
      return;
    }
    self->iword = (self->iword << FLARE32_ONE_EXT_BITPOS)
      | bfd_getb16 (self->buffer);
  }

  self->length += 2;

  self->grp
    //= GET_INSN_FIELD (flare32_enc_info_grp_16.mask,
    //  flare32_enc_info_grp_16.bitpos, self->iword);
    = flare32_get_insn_field_ei (&flare32_enc_info_grp_16, self->iword);
  self->ra_ind
    //= GET_INSN_FIELD (flare32_enc_info_ra_ind.mask,
    //  flare32_enc_info_ra_ind.bitpos, self->iword);
    = flare32_get_insn_field_ei (&flare32_enc_info_ra_ind, self->iword);
  self->rb_ind
    //= GET_INSN_FIELD (flare32_enc_info_rb_ind.mask,
    //  flare32_enc_info_rb_ind.bitpos, self->iword);
    = flare32_get_insn_field_ei (&flare32_enc_info_rb_ind, self->iword);

  switch (self->grp)
  {
    /* -------- */
    case FLARE32_G1_GRP_VALUE:
    {
      /* -------- */
      if (self->length == 2)
      {
        self->simm
          //= GET_INSN_FIELD (FLARE32_G1G5G6_S5_MASK,
          //FLARE32_G1G5G6_S5_BITPOS, self->iword);
          = flare32_sign_extend (flare32_get_insn_field_ei 
            (&flare32_enc_info_g1g5g6_i5, self->iword),
            flare32_enc_info_g1g5g6_i5.bitsize);
      }
      else if (self->length == 4) /* `pre` */
      {
        self->simm = flare32_sign_extend (flare32_get_g1g5g6_s17
          ((self->iword >> FLARE32_ONE_EXT_BITPOS), self->iword),
          flare32_enc_info_g1g5g6_i5.bitsize
            + flare32_enc_info_g0_pre_s12.bitsize);
      }
      else if (self->length == 6) /* `lpre` */
      {
        self->simm = flare32_sign_extend (flare32_get_g1g5g6_s32
          ((self->iword >> FLARE32_ONE_EXT_BITPOS), self->iword),
          flare32_enc_info_g1g5g6_i5.bitsize
            + flare32_enc_info_g0_lpre_s27.bitsize);
      }
      /* -------- */
      self->opc_info = &flare32_opc_info_g1
        //[GET_INSN_FIELD (FLARE32_G1_OP_MASK, FLARE32_G1_OP_BITPOS,
        //  self->iword)];
        [flare32_get_insn_field_ei (&flare32_enc_info_g1_op, self->iword)];
      /* -------- */
      //do_print_insn_flare32
      //  (self->opc_info,
      //  self->iword,
      //  self->simm,
      //  self->grp,
      //  self->ra_ind,
      //  self->rb_ind,
      //  self->fw,
      //  self->length);
      /* -------- */
    }
      break;
    /* -------- */
    case FLARE32_G2_GRP_VALUE:
    {
      /* -------- */
      self->opc_info = &flare32_opc_info_g2
        //[GET_INSN_FIELD (FLARE32_G2_OP_MASK, FLARE32_G2_OP_BITPOS,
        //  self->iword)];
        [flare32_get_insn_field_ei (&flare32_enc_info_g2_op, self->iword)];

      self->fw
        //= GET_INSN_FIELD (FLARE32_G2_F_MASK, FLARE32_G2_F_BITPOS,
        //  self->iword);
        = flare32_get_insn_field_ei (&flare32_enc_info_g2_f, self->iword);
      /* -------- */
      //do_print_insn_flare32
      //  (self->opc_info,
      //  self->iword,
      //  self->simm,
      //  self->grp,
      //  self->ra_ind,
      //  self->rb_ind,
      //  self->fw,
      //  self->length);
      /* -------- */
    }
      break;
    /* -------- */
    case FLARE32_G3_GRP_VALUE:
    {
      /* -------- */
      if (self->length == 2)
      {
        self->simm
          //= GET_INSN_FIELD (FLARE32_G1G5G6_S5_MASK,
          //FLARE32_G1G5G6_S5_BITPOS, self->iword);
          = flare32_sign_extend (flare32_get_insn_field_ei 
            (&flare32_enc_info_g3_s9, self->iword),
            flare32_enc_info_g3_s9.bitsize);
      }
      else if (self->length == 4) /* `pre` */
      {
        self->simm = flare32_sign_extend (flare32_get_g3_s21
          ((self->iword >> FLARE32_ONE_EXT_BITPOS), self->iword),
          flare32_enc_info_g3_s9.bitsize
            + flare32_enc_info_g0_pre_s12.bitsize);
        //fpr (stream,
        //  "testificate: %lu\n",
        //  self->simm);
      }
      else if (self->length == 6) /* `lpre` */
      {
        self->simm = flare32_sign_extend (flare32_get_g3_s32
          ((self->iword >> FLARE32_ONE_EXT_BITPOS), self->iword),
          flare32_enc_info_g3_s9.bitsize
            + flare32_enc_info_g0_lpre_s23.bitsize);
      }
      /* -------- */
      self->opc_info = &flare32_opc_info_g3
        //[GET_INSN_FIELD (FLARE32_G3_OP_MASK, FLARE32_G3_OP_BITPOS,
        //  self->iword)];
        [flare32_get_insn_field_ei (&flare32_enc_info_g3_op, self->iword)];
      /* -------- */
      //do_print_insn_flare32
      //  (self->opc_info,
      //  self->iword,
      //  self->simm,
      //  self->grp,
      //  self->ra_ind,
      //  self->rb_ind,
      //  self->fw,
      //  self->length);
      /* -------- */
    }
      break;
    /* -------- */
    case FLARE32_G4_GRP_VALUE:
    {
      /* -------- */
      self->opc_info = &flare32_opc_info_g4
        //[GET_INSN_FIELD (FLARE32_G4_OP_MASK, FLARE32_G4_OP_BITPOS,
        //  self->iword)];
        [flare32_get_insn_field_ei (&flare32_enc_info_g4_op, self->iword)];
      /* -------- */
      //do_print_insn_flare32
      //  (self->opc_info,
      //  self->iword,
      //  self->simm,
      //  self->grp,
      //  self->ra_ind,
      //  self->rb_ind,
      //  self->fw,
      //  self->length);
      /* -------- */
    }
      break;
    /* -------- */
    case FLARE32_G5_GRP_VALUE:
    {
      /* -------- */
      if (self->length == 2)
      {
        self->simm
          //= GET_INSN_FIELD (FLARE32_G1G5G6_S5_MASK,
          //FLARE32_G1G5G6_S5_BITPOS, self->iword);
          = flare32_sign_extend (flare32_get_insn_field_ei 
            (&flare32_enc_info_g1g5g6_i5, self->iword),
            flare32_enc_info_g1g5g6_i5.bitsize);
      }
      else if (self->length == 4) /* `pre` */
      {
        self->simm = flare32_sign_extend (flare32_get_g1g5g6_s17
          ((self->iword >> FLARE32_ONE_EXT_BITPOS), self->iword),
          flare32_enc_info_g1g5g6_i5.bitsize
            + flare32_enc_info_g0_pre_s12.bitsize);
      }
      else if (self->length == 6) /* `lpre` */
      {
        self->simm = flare32_sign_extend (flare32_get_g1g5g6_s32
          ((self->iword >> FLARE32_ONE_EXT_BITPOS), self->iword),
          flare32_enc_info_g1g5g6_i5.bitsize
            + flare32_enc_info_g0_lpre_s27.bitsize);
      }
      /* -------- */
      self->opc_info = &flare32_opc_info_g5[0];
      /* -------- */
      //do_print_insn_flare32
      //  (self->opc_info,
      //  self->iword,
      //  self->simm,
      //  self->grp,
      //  self->ra_ind,
      //  self->rb_ind,
      //  self->fw,
      //  self->length);
    }
      break;
    /* -------- */
    case FLARE32_G6_GRP_VALUE:
    {
      /* -------- */
      if (self->length == 2)
      {
        self->simm
          //= GET_INSN_FIELD (FLARE32_G1G5G6_S5_MASK,
          //FLARE32_G1G5G6_S5_BITPOS, self->iword);
          = flare32_sign_extend (flare32_get_insn_field_ei 
            (&flare32_enc_info_g1g5g6_i5, self->iword),
            flare32_enc_info_g1g5g6_i5.bitsize);
      }
      else if (self->length == 4) /* `pre` */
      {
        self->simm = flare32_sign_extend (flare32_get_g1g5g6_s17
          ((self->iword >> FLARE32_ONE_EXT_BITPOS), self->iword),
          flare32_enc_info_g1g5g6_i5.bitsize
            + flare32_enc_info_g0_pre_s12.bitsize);
      }
      else if (self->length == 6) /* `lpre` */
      {
        self->simm = flare32_sign_extend (flare32_get_g1g5g6_s32
          ((self->iword >> FLARE32_ONE_EXT_BITPOS), self->iword),
          flare32_enc_info_g1g5g6_i5.bitsize
            + flare32_enc_info_g0_lpre_s27.bitsize);
      }
      /* -------- */
      self->opc_info = &flare32_opc_info_g6[0];
      /* -------- */
      //do_print_insn_flare32
      //  (self->opc_info,
      //  self->iword,
      //  self->simm,
      //  self->grp,
      //  self->ra_ind,
      //  self->rb_ind,
      //  self->fw,
      //  self->length);
      /* -------- */
    }
      break;
    /* -------- */
    case FLARE32_G7_GRP_VALUE:
    {
      /* -------- */
      //flare32_temp_t
      //  g7_aluopbh_subgrp, g7_sprldst_subgrp;
      self->g7_aluopbh_subgrp = flare32_get_insn_field_ei
        (&flare32_enc_info_g7_aluopbh_subgrp, self->iword);
      self->g7_sprldst_subgrp = flare32_get_insn_field_ei
        (&flare32_enc_info_g7_sprldst_subgrp, self->iword);
      self->g7_icreload_subgrp = flare32_get_insn_field_ei
        (&flare32_enc_info_g7_icreload_subgrp, self->iword);
      //if (self->g7_aluopbh_subgrp != FLARE32_G7_ALUOPBH_SUBGRP_VALUE
      //  && self->g7_sprldst_subgrp != FLARE32_G7_SPRLDST_SUBGRP_VALUE
      //  && self->g7_icreload_subgrp != FLARE32_G7_ICRELOAD_SUBGRP_VALUE)
      //{
      //  break;
      //}
      if (self->g7_aluopbh_subgrp == FLARE32_G7_ALUOPBH_SUBGRP_VALUE)
      {
        self->opc_info = &flare32_opc_info_g7_aluopbh
          [flare32_get_insn_field_ei (&flare32_enc_info_g7_aluopbh_op,
            self->iword)];
        self->fw = flare32_get_insn_field_ei
          (&flare32_enc_info_g7_aluopbh_w, self->iword);
      }
      else if (self->g7_sprldst_subgrp == FLARE32_G7_SPRLDST_SUBGRP_VALUE)
      {
        self->opc_info = &flare32_opc_info_g7_sprldst
          [flare32_get_insn_field_ei
            (&flare32_enc_info_g7_sprldst_op, self->iword)];
      }
      else if (
        self->g7_icreload_subgrp == FLARE32_G7_ICRELOAD_SUBGRP_VALUE
      )
      {
        self->opc_info = &flare32_opc_info_g7_icreload[0];

        if (self->length == 2)
        {
          self->simm
            //= GET_INSN_FIELD (FLARE32_G1G5G6_S5_MASK,
            //FLARE32_G1G5G6_S5_BITPOS, self->iword);
            = flare32_sign_extend (flare32_get_insn_field_ei 
              (&flare32_enc_info_g7_icreload_s5, self->iword),
              flare32_enc_info_g7_icreload_s5.bitsize);
        }
        else if (self->length == 4) /* `pre` */
        {
          self->simm = flare32_sign_extend (flare32_get_g7_icreload_s17
            ((self->iword >> FLARE32_ONE_EXT_BITPOS), self->iword),
            flare32_enc_info_g7_icreload_s5.bitsize
              + flare32_enc_info_g0_pre_s12.bitsize);
        }
        else if (self->length == 6) /* `lpre` */
        {
          self->simm = flare32_sign_extend (flare32_get_g7_icreload_s32
            ((self->iword >> FLARE32_ONE_EXT_BITPOS), self->iword),
            flare32_enc_info_g7_icreload_s5.bitsize
              + flare32_enc_info_g0_lpre_s27.bitsize);
        }
      }
      else
      {
        break;
      }
      //do_print_insn_flare32
      //  (self->opc_info,
      //  self->iword,
      //  self->simm,
      //  self->grp,
      //  self->ra_ind,
      //  self->rb_ind,
      //  self->fw,
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
#endif    // _FLARE32_DASM_INFO_FUNCS_H_

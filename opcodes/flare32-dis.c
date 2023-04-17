/* Disassemble flare32 instructions.
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
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston,
   MA 02110-1301, USA.  */

#include "sysdep.h"
#include <stdio.h>

#define STATIC_TABLE
#define DEFINE_TABLE

#include "opcode/flare32.h"
#include "disassemble.h"

static fprintf_ftype fpr;
static void *stream;

#define FLARE32_DIS_CBUF_LIM 50
static char cbuf[FLARE32_DIS_CBUF_LIM];
/* -------- */
/* General purpose register names */
const flare32_reg_t gprs[FLARE32_NUM_GPRS] = FLARE32_INST_GPRS ();
/* Special purpose register names */
const flare32_reg_t sprs[FLARE32_NUM_SPRS] = FLARE32_INST_SPRS ();

const flare32_reg_t reg_pc = FLARE32_INST_REG_PC ();

static char*
do_snprintf_insn_flare32_maybe_pre_lpre (int length,
                                        flare32_temp_t iword,
                                        flare32_temp_t grp)
{
  if (length == 2)
  {
    cbuf[0] = '\0';
  }
  else if (length == 4)
  {
    snprintf (cbuf, FLARE32_DIS_CBUF_LIM,
      " // pre #0x%x",
      //(unsigned) GET_INSN_FIELD (FLARE32_PRE_S12_MASK,
      //  FLARE32_PRE_S12_BITPOS, iword)
      (unsigned) flare32_get_insn_field_ei
        (&flare32_enc_info_g0_pre_s12,
        iword >> FLARE32_ONE_EXT_BITPOS)
    );
  }
  else if (length == 6)
  {
    if (grp != FLARE32_G3_GRP_VALUE) /* groups 1, 5, or 6 */
    {
      snprintf (cbuf, FLARE32_DIS_CBUF_LIM,
        " // lpre #0x%x",
        //(unsigned) GET_INSN_FIELD (FLARE32_G1G5G6_LPRE_S27_MASK,
        //  FLARE32_G1G5G6_LPRE_S27_BITPOS, iword)
        (unsigned) flare32_get_insn_field_ei
          (&flare32_enc_info_g1g5g6_g0_lpre_s27,
          iword >> FLARE32_ONE_EXT_BITPOS)
      );
    }
    else /* if (grp == FLARE32_G3_GRP_VALUE) */
    {
      snprintf (cbuf, FLARE32_DIS_CBUF_LIM,
        " // lpre #0x%x",
        //(unsigned) GET_INSN_FIELD (FLARE32_G3_LPRE_S23_MASK,
        //  FLARE32_G3_LPRE_S23_BITPOS, iword)
        (unsigned) flare32_get_insn_field_ei
          (&flare32_enc_info_g3_g0_lpre_s23,
          iword >> FLARE32_ONE_EXT_BITPOS)
      );
    }
  }
  return cbuf;
}

static void
do_print_insn_flare32 (const flare32_opc_info_t *opc_info,
                      flare32_temp_t iword,
                      flare32_temp_t simm,
                      flare32_temp_t grp,
                      flare32_temp_t ra_ind,
                      flare32_temp_t rb_ind,
                      flare32_temp_t fw,
                      int length)
{
  switch (opc_info->oparg)
  {
    /* -------- */
    case FLARE32_OA_NONE:
    {
      fpr (stream, "%s",
        opc_info->names[fw]);
    }
      break;
    case FLARE32_OA_RA_S5:
    {
      fpr (stream, "%s\t%s, #%i%s",
        opc_info->names[fw], gprs[ra_ind].name, (signed) simm,
        do_snprintf_insn_flare32_maybe_pre_lpre (length, iword, grp));
    }
      break;
    case FLARE32_OA_RA_PC_S5:
    {
      fpr (stream, "%s\t%s, pc, #%i%s",
        opc_info->names[fw], gprs[ra_ind].name, (signed) simm,
        do_snprintf_insn_flare32_maybe_pre_lpre (length, iword, grp));
    }
      break;
    case FLARE32_OA_RA_SP_S5:
    {
      fpr (stream, "%s\t%s, sp, #%i%s",
        opc_info->names[fw], gprs[ra_ind].name, (signed) simm,
        do_snprintf_insn_flare32_maybe_pre_lpre (length, iword, grp));
    }
      break;
    case FLARE32_OA_RA_FP_S5:
    {
      fpr (stream, "%s\t%s, fp, #%i%s",
        opc_info->names[fw], gprs[ra_ind].name, (signed) simm,
        do_snprintf_insn_flare32_maybe_pre_lpre (length, iword, grp));
    }
      break;
    case FLARE32_OA_S5:
    {
      fpr (stream, "%s\t#%i%s",
        opc_info->names[fw], (signed) simm,
        do_snprintf_insn_flare32_maybe_pre_lpre (length, iword, grp));
    }
      break;
    case FLARE32_OA_RA:
    {
      if (length != 2)
      {
        fpr (stream, "%s%x%s",
          "bad (grp 0x",
          (unsigned) grp,
          "; `FLARE32_OA_RA`; `pre`/`lpre`)");
      }
      else
      {
        fpr (stream, "%s\t%s",
          opc_info->names[fw], gprs[ra_ind].name);
      }
    }
      break;
    case FLARE32_OA_RA_RB:
    {
      if (length != 2)
      {
        fpr (stream, "%s%x%s",
          "bad (grp 0x",
          (unsigned) grp,
          "; `FLARE32_OA_RA_RB`; `pre`/`lpre`)");
      }
      else
      {
        fpr (stream, "%s\t%s, %s",
          opc_info->names[fw], gprs[ra_ind].name, gprs[rb_ind].name);
      }
    }
      break;
    case FLARE32_OA_RA_SP_RB:
    {
      if (length != 2)
      {
        fpr (stream, "%s%x%s",
          "bad (grp 0x",
          (unsigned) grp,
          "; `FLARE32_OA_RA_SP_RB`; `pre`/`lpre`)");
      }
      else
      {
        fpr (stream, "%s\t%s, sp, %s",
          opc_info->names[fw], gprs[ra_ind].name, gprs[rb_ind].name);
      }
    }
      break;
    case FLARE32_OA_RA_FP_RB:
    {
      if (length != 2)
      {
        fpr (stream, "%s%x%s",
          "bad (grp 0x",
          (unsigned) grp,
          "; `FLARE32_OA_RA_FP_RB`; `pre`/`lpre`)");
      }
      else
      {
        fpr (stream, "%s\t%s, fp, %s",
          opc_info->names[fw], gprs[ra_ind].name, gprs[rb_ind].name);
      }
    }
      break;
    case FLARE32_OA_PCREL_S9:
    {
      fpr (stream, "%s\t%i%s",
        opc_info->names[fw], (signed) simm,
        do_snprintf_insn_flare32_maybe_pre_lpre (length, iword, grp));
    }
      break;
    case FLARE32_OA_IRA:
    {
      if (length != 2)
      {
        fpr (stream, "%s%x%s",
          "bad (grp 0x",
          (unsigned) grp,
          "; `FLARE32_OA_IRA`; `pre`/`lpre`)");
      }
      else
      {
        fpr (stream, "%s\t%s",
          opc_info->names[fw], "ira");
      }
    }
      break;
    case FLARE32_OA_RA_SB:
    {
      if (length != 2 || rb_ind >= FLARE32_NUM_SPRS)
      {
        fpr (stream, "bad (grp 0x%x; `FLARE32_OA_RA_SC`: %s; %s;)",
          (unsigned) grp,
          ((length != 2) ? "`pre`/lpre`" : ""),
          ((rb_ind >= FLARE32_NUM_SPRS) ? "rb_ind" : ""));
      }
      else
      {
        fpr (stream, "%s\t%s, %s",
          opc_info->names[fw], gprs[ra_ind].name, sprs[rb_ind].name);
      }
    }
      break;
    case FLARE32_OA_SA_RB:
    {
      if (length != 2 || ra_ind >= FLARE32_NUM_SPRS)
      {
        fpr (stream, "bad (grp 0x%x; `FLARE32_OA_SA_RB`: %s; %s;)",
          (unsigned) grp,
          ((length != 2) ? "`pre`/lpre`" : ""),
          ((ra_ind >= FLARE32_NUM_SPRS) ? "ra_ind" : ""));
      }
      else
      {
        fpr (stream, "%s\t%s, %s",
          opc_info->names[fw], sprs[ra_ind].name, gprs[rb_ind].name);
      }
    }
      break;
    case FLARE32_OA_RA_RB_LDST:
    {
      if (length != 2)
      {
        fpr (stream, "%s%x%s",
          "bad (grp 0x",
          (unsigned) grp,
          "; `FLARE32_OA_RA_RB_LDST`; `pre`/`lpre`)");
      }
      else
      {
        fpr (stream, "%s\t%s, [%s]",
          opc_info->names[fw], gprs[ra_ind].name, gprs[rb_ind].name);
      }
    }
      break;
    //case FLARE32_OA_RA_RB_RC_LDST:
    //  break;
    case FLARE32_OA_RA_RB_S5_LDST:
    {
        fpr (stream, "%s\t%s, [%s, #%i]%s",
          opc_info->names[fw], gprs[ra_ind].name, gprs[rb_ind].name,
          (signed) simm,
          do_snprintf_insn_flare32_maybe_pre_lpre (length, iword, grp));
    }
      break;
    //case FLARE32_OA_RA_RB_RC_S5_LDST:
    //  break;
    default:
      fpr (stream, "bad (grp 0x%x; oparg 0x%x; opcode 0x%x; name %s)",
        (unsigned) grp, (unsigned) opc_info->oparg,
        (unsigned) opc_info->opcode,
        opc_info->names[fw]);
      break;
    /* -------- */
  }
}

int
print_insn_flare32 (bfd_vma addr, struct disassemble_info *info)
{
  stream = info->stream;
  fpr = info->fprintf_func;

  int
    length = 0,
    status;
  bfd_byte buffer[2];
  const flare32_opc_info_t *opc_info;
  flare32_temp_t
    iword,
    simm = 0,
    grp,
    ra_ind,
    rb_ind,
    fw = 0;

  if ((status = info->read_memory_func (addr + length, buffer, 2, info)))
  {
    goto fail;
  }
  iword = bfd_getb16 (buffer);

  if (flare32_get_insn_field_ei (&flare32_enc_info_g0_pre_fullgrp, iword)
    == FLARE32_G0_PRE_FULLGRP_VALUE)
  {
    /* `pre` */
    length += 2;

    if ((status = info->read_memory_func (addr + length, buffer, 2, info)))
    {
      goto fail;
    }
    iword = (iword << FLARE32_ONE_EXT_BITPOS) | bfd_getb16 (buffer);
  }
  //else if (GET_INSN_FIELD (FLARE32_LPRE_FULL_GRP_16_MASK,
  //  FLARE32_LPRE_FULL_GRP_16_BITPOS, iword) == FLARE32_LPRE_FULL_GRP_VALUE)
  else if (flare32_get_insn_field_ei (&flare32_enc_info_g0_lpre_fullgrp_16,
    iword)
    == FLARE32_G0_LPRE_FULLGRP_VALUE)
  {
    /* `lpre` */
    length += 2;
    if ((status = info->read_memory_func (addr + length, buffer, 2, info)))
    {
      goto fail;
    }
    iword = (iword << 16ull) | bfd_getb16 (buffer);

    length += 2;

    //iword <<= FLARE32_ONE_EXT_BITPOS;
    if ((status = info->read_memory_func (addr + length, buffer, 2, info)))
    {
      goto fail;
    }
    iword = (iword << FLARE32_ONE_EXT_BITPOS) | bfd_getb16 (buffer);
  }

  length += 2;

  grp
    //= GET_INSN_FIELD (flare32_enc_info_grp_16.mask,
    //flare32_enc_info_grp_16.bitpos, iword);
    = flare32_get_insn_field_ei (&flare32_enc_info_grp_16, iword);
  ra_ind
    //= GET_INSN_FIELD (flare32_enc_info_ra_ind.mask,
    //flare32_enc_info_ra_ind.bitpos, iword);
    = flare32_get_insn_field_ei (&flare32_enc_info_ra_ind, iword);
  rb_ind
    //= GET_INSN_FIELD (flare32_enc_info_rb_ind.mask,
    //flare32_enc_info_rb_ind.bitpos, iword);
    = flare32_get_insn_field_ei (&flare32_enc_info_rb_ind, iword);

  switch (grp)
  {
    /* -------- */
    case FLARE32_G1_GRP_VALUE:
    {
      /* -------- */
      if (length == 2)
      {
        simm
          //= GET_INSN_FIELD (FLARE32_G1G5G6_S5_MASK,
          //FLARE32_G1G5G6_S5_BITPOS, iword);
          = flare32_sign_extend (flare32_get_insn_field_ei 
            (&flare32_enc_info_g1g5g6_s5, iword),
            flare32_enc_info_g1g5g6_s5.bitsize);
      }
      else if (length == 4) /* `pre` */
      {
        simm = flare32_sign_extend (flare32_get_g1g5g6_s17
          ((iword >> FLARE32_ONE_EXT_BITPOS), iword),
          flare32_enc_info_g1g5g6_s5.bitsize
            + flare32_enc_info_g0_pre_s12.bitsize);
      }
      else if (length == 6) /* `lpre` */
      {
        simm = flare32_sign_extend (flare32_get_g1g5g6_s32
          ((iword >> FLARE32_ONE_EXT_BITPOS), iword),
          flare32_enc_info_g1g5g6_s5.bitsize
            + flare32_enc_info_g1g5g6_g0_lpre_s27.bitsize);
      }
      /* -------- */
      opc_info = &flare32_opc_info_g1
        //[GET_INSN_FIELD (FLARE32_G1_OP_MASK, FLARE32_G1_OP_BITPOS, iword)];
        [flare32_get_insn_field_ei (&flare32_enc_info_g1_op, iword)];
      /* -------- */
      do_print_insn_flare32
        (opc_info,
        iword,
        simm,
        grp,
        ra_ind,
        rb_ind,
        fw,
        length);
      /* -------- */
    }
      break;
    /* -------- */
    case FLARE32_G2_GRP_VALUE:
    {
      /* -------- */
      opc_info = &flare32_opc_info_g2
        //[GET_INSN_FIELD (FLARE32_G2_OP_MASK, FLARE32_G2_OP_BITPOS, iword)];
        [flare32_get_insn_field_ei (&flare32_enc_info_g2_op, iword)];

      fw
        //= GET_INSN_FIELD (FLARE32_G2_F_MASK, FLARE32_G2_F_BITPOS, iword);
        = flare32_get_insn_field_ei (&flare32_enc_info_g2_f, iword);
      /* -------- */
      do_print_insn_flare32
        (opc_info,
        iword,
        simm,
        grp,
        ra_ind,
        rb_ind,
        fw,
        length);
      /* -------- */
    }
      break;
    /* -------- */
    case FLARE32_G3_GRP_VALUE:
    {
      /* -------- */
      if (length == 2)
      {
        simm
          //= GET_INSN_FIELD (FLARE32_G1G5G6_S5_MASK,
          //FLARE32_G1G5G6_S5_BITPOS, iword);
          = flare32_sign_extend (flare32_get_insn_field_ei 
            (&flare32_enc_info_g3_s9, iword),
            flare32_enc_info_g3_s9.bitsize);
      }
      else if (length == 4) /* `pre` */
      {
        simm = flare32_sign_extend (flare32_get_g3_s21
          ((iword >> FLARE32_ONE_EXT_BITPOS), iword),
          flare32_enc_info_g3_s9.bitsize
            + flare32_enc_info_g0_pre_s12.bitsize);
      }
      else if (length == 6) /* `lpre` */
      {
        simm = flare32_sign_extend (flare32_get_g3_s32
          ((iword >> FLARE32_ONE_EXT_BITPOS), iword),
          flare32_enc_info_g3_s9.bitsize
            + flare32_enc_info_g3_g0_lpre_s23.bitsize);
      }
      /* -------- */
      opc_info = &flare32_opc_info_g3
        //[GET_INSN_FIELD (FLARE32_G3_OP_MASK, FLARE32_G3_OP_BITPOS, iword)];
        [flare32_get_insn_field_ei (&flare32_enc_info_g3_op, iword)];
      /* -------- */
      do_print_insn_flare32
        (opc_info,
        iword,
        simm,
        grp,
        ra_ind,
        rb_ind,
        fw,
        length);
      /* -------- */
    }
      break;
    /* -------- */
    case FLARE32_G4_GRP_VALUE:
    {
      /* -------- */
      opc_info = &flare32_opc_info_g4
        //[GET_INSN_FIELD (FLARE32_G4_OP_MASK, FLARE32_G4_OP_BITPOS, iword)];
        [flare32_get_insn_field_ei (&flare32_enc_info_g4_op, iword)];
      /* -------- */
      do_print_insn_flare32
        (opc_info,
        iword,
        simm,
        grp,
        ra_ind,
        rb_ind,
        fw,
        length);
      /* -------- */
    }
      break;
    /* -------- */
    case FLARE32_G5_GRP_VALUE:
    {
      /* -------- */
      if (length == 2)
      {
        simm
          //= GET_INSN_FIELD (FLARE32_G1G5G6_S5_MASK,
          //FLARE32_G1G5G6_S5_BITPOS, iword);
          = flare32_sign_extend (flare32_get_insn_field_ei 
            (&flare32_enc_info_g1g5g6_s5, iword),
            flare32_enc_info_g1g5g6_s5.bitsize);
      }
      else if (length == 4) /* `pre` */
      {
        simm = flare32_sign_extend (flare32_get_g1g5g6_s17
          ((iword >> FLARE32_ONE_EXT_BITPOS), iword),
          flare32_enc_info_g1g5g6_s5.bitsize
            + flare32_enc_info_g0_pre_s12.bitsize);
      }
      else if (length == 6) /* `lpre` */
      {
        simm = flare32_sign_extend (flare32_get_g1g5g6_s32
          ((iword >> FLARE32_ONE_EXT_BITPOS), iword),
          flare32_enc_info_g1g5g6_s5.bitsize
            + flare32_enc_info_g1g5g6_g0_lpre_s27.bitsize);
      }
      /* -------- */
      opc_info = &flare32_opc_info_g5[0];
      /* -------- */
      do_print_insn_flare32
        (opc_info,
        iword,
        simm,
        grp,
        ra_ind,
        rb_ind,
        fw,
        length);
    }
      break;
    /* -------- */
    case FLARE32_G6_GRP_VALUE:
    {
      /* -------- */
      if (length == 2)
      {
        simm
          //= GET_INSN_FIELD (FLARE32_G1G5G6_S5_MASK,
          //FLARE32_G1G5G6_S5_BITPOS, iword);
          = flare32_sign_extend (flare32_get_insn_field_ei 
            (&flare32_enc_info_g1g5g6_s5, iword),
            flare32_enc_info_g1g5g6_s5.bitsize);
      }
      else if (length == 4) /* `pre` */
      {
        simm = flare32_sign_extend (flare32_get_g1g5g6_s17
          ((iword >> FLARE32_ONE_EXT_BITPOS), iword),
          flare32_enc_info_g1g5g6_s5.bitsize
            + flare32_enc_info_g0_pre_s12.bitsize);
      }
      else if (length == 6) /* `lpre` */
      {
        simm = flare32_sign_extend (flare32_get_g1g5g6_s32
          ((iword >> FLARE32_ONE_EXT_BITPOS), iword),
          flare32_enc_info_g1g5g6_s5.bitsize
            + flare32_enc_info_g1g5g6_g0_lpre_s27.bitsize);
      }
      /* -------- */
      opc_info = &flare32_opc_info_g6[0];
      /* -------- */
      do_print_insn_flare32
        (opc_info,
        iword,
        simm,
        grp,
        ra_ind,
        rb_ind,
        fw,
        length);
      /* -------- */
    }
      break;
    /* -------- */
    case FLARE32_G7_GRP_VALUE:
    {
      /* -------- */
      flare32_temp_t temp_subgrp;
      /* There is only one subgroup in group 7 for now */
      opc_info = &flare32_opc_info_g7
        [flare32_get_insn_field_ei (&flare32_enc_info_g7_aluopbh_op,
          iword)];
      if ((temp_subgrp = flare32_get_insn_field_ei
          (&flare32_enc_info_g7_aluopbh_subgrp, iword))
        != FLARE32_G7_ALUOPBH_SUBGRP_VALUE)
      {
        fpr (stream, "bad (grp 0x%x; unknown subgrp 0x%x)",
          (unsigned) grp, (unsigned) temp_subgrp);
      }
      fw = flare32_get_insn_field_ei (&flare32_enc_info_g7_aluopbh_w,
        iword);
      do_print_insn_flare32
        (opc_info,
        iword,
        simm,
        grp,
        ra_ind,
        rb_ind,
        fw,
        length);
      /* -------- */
    }
      break;
    /* -------- */
    default:
      fpr (stream, "bad (unknown grp 0x%x)",
        (unsigned) grp);
      break;
  }

  return length;
    
fail:
  info->memory_error_func (status, addr, info);
  return -1;
}

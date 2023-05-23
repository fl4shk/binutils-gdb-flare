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
#include "opcode/flare32-opc-decls.h"
#include "opcode/flare32-disasm-info-funcs.h"
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
do_print_insn_flare32 (flare32_dasm_info_t *args)
{
  switch (args->opc_info->oparg)
  {
    /* -------- */
    case FLARE32_OA_NONE:
    {
      fpr (stream, "%s",
        args->opc_info->names[args->fw]);
    }
      break;
    case FLARE32_OA_RA_S5:
    {
      fpr (stream, "%s\t%s, #%i%s",
        args->opc_info->names[args->fw],
        gprs[args->ra_ind].name,
        (signed) args->simm,
        do_snprintf_insn_flare32_maybe_pre_lpre
          (args->length, args->iword, args->grp));
    }
      break;
    case FLARE32_OA_RA_PC_S5:
    {
      fpr (stream, "%s\t%s, pc, #%i%s",
        args->opc_info->names[args->fw],
        gprs[args->ra_ind].name,
        (signed) args->simm,
        do_snprintf_insn_flare32_maybe_pre_lpre
          (args->length, args->iword, args->grp));
    }
      break;
    case FLARE32_OA_RA_SP_S5:
    {
      fpr (stream, "%s\t%s, sp, #%i%s",
        args->opc_info->names[args->fw],
        gprs[args->ra_ind].name,
        (signed) args->simm,
        do_snprintf_insn_flare32_maybe_pre_lpre
          (args->length, args->iword, args->grp));
    }
      break;
    case FLARE32_OA_RA_FP_S5:
    {
      fpr (stream, "%s\t%s, fp, #%i%s",
        args->opc_info->names[args->fw],
        gprs[args->ra_ind].name,
        (signed) args->simm,
        do_snprintf_insn_flare32_maybe_pre_lpre
          (args->length, args->iword, args->grp));
    }
      break;
    case FLARE32_OA_RA_U5:
    {
      if (args->length == 2)
      {
        fpr (stream, "%s\t%s, #%u%s",
          args->opc_info->names[args->fw],
          gprs[args->ra_ind].name,
          (unsigned) flare32_zero_extend (args->simm,
            flare32_enc_info_g1g5g6_i5.bitsize),
          do_snprintf_insn_flare32_maybe_pre_lpre
            (args->length, args->iword, args->grp));
      }
      else
      {
        fpr (stream, "%s\t%s, #%i%s",
          args->opc_info->names[args->fw],
          gprs[args->ra_ind].name,
          (signed) args->simm,
          do_snprintf_insn_flare32_maybe_pre_lpre
            (args->length, args->iword, args->grp));
      }
    }
      break;
    case FLARE32_OA_U5:
    {
      if (args->length == 2)
      {
        fpr (stream, "%s\t#%u%s",
          args->opc_info->names[args->fw],
          (unsigned) flare32_zero_extend (args->simm,
            flare32_enc_info_g1g5g6_i5.bitsize),
          do_snprintf_insn_flare32_maybe_pre_lpre
            (args->length, args->iword, args->grp));
      }
      else
      {
        fpr (stream, "%s\t#%i%s",
          args->opc_info->names[args->fw],
          (signed) args->simm,
          do_snprintf_insn_flare32_maybe_pre_lpre
            (args->length, args->iword, args->grp));
      }
    }
      break;
    case FLARE32_OA_RA:
    {
      if (args->length != 2)
      {
        fpr (stream, "%s%x%s",
          "bad (grp 0x",
          (unsigned) args->grp,
          "; `FLARE32_OA_RA`; `pre`/`lpre`)");
      }
      else
      {
        fpr (stream, "%s\t%s",
          args->opc_info->names[args->fw],
          gprs[args->ra_ind].name);
      }
    }
      break;
    case FLARE32_OA_RA_RB:
    {
      if (args->length != 2)
      {
        fpr (stream, "%s%x%s",
          "bad (grp 0x",
          (unsigned) args->grp,
          "; `FLARE32_OA_RA_RB`; `pre`/`lpre`)");
      }
      else
      {
        fpr (stream, "%s\t%s, %s",
          args->opc_info->names[args->fw],
          gprs[args->ra_ind].name,
          gprs[args->rb_ind].name);
      }
    }
      break;
    case FLARE32_OA_RA_SP_RB:
    {
      if (args->length != 2)
      {
        fpr (stream, "%s%x%s",
          "bad (grp 0x",
          (unsigned) args->grp,
          "; `FLARE32_OA_RA_SP_RB`; `pre`/`lpre`)");
      }
      else
      {
        fpr (stream, "%s\t%s, sp, %s",
          args->opc_info->names[args->fw],
          gprs[args->ra_ind].name,
          gprs[args->rb_ind].name);
      }
    }
      break;
    case FLARE32_OA_RA_FP_RB:
    {
      if (args->length != 2)
      {
        fpr (stream, "%s%x%s",
          "bad (grp 0x",
          (unsigned) args->grp,
          "; `FLARE32_OA_RA_FP_RB`; `pre`/`lpre`)");
      }
      else
      {
        fpr (stream, "%s\t%s, fp, %s",
          args->opc_info->names[args->fw],
          gprs[args->ra_ind].name,
          gprs[args->rb_ind].name);
      }
    }
      break;
    case FLARE32_OA_PCREL_S9:
    {
      fpr (stream, "%s\t%i%s",
        args->opc_info->names[args->fw], (signed) args->simm,
        do_snprintf_insn_flare32_maybe_pre_lpre
          (args->length, args->iword, args->grp));
    }
      break;
    case FLARE32_OA_IRA:
    {
      if (args->length != 2)
      {
        fpr (stream, "%s%x%s",
          "bad (grp 0x",
          (unsigned) args->grp,
          "; `FLARE32_OA_IRA`; `pre`/`lpre`)");
      }
      else
      {
        fpr (stream, "%s\t%s",
          args->opc_info->names[args->fw], "ira");
      }
    }
      break;
    case FLARE32_OA_RA_SB:
    {
      if (args->length != 2 || args->rb_ind >= FLARE32_NUM_SPRS)
      {
        fpr (stream, "bad (grp 0x%x; `FLARE32_OA_RA_SC`: %s; %s;)",
          (unsigned) args->grp,
          ((args->length != 2) ? "`pre`/lpre`" : ""),
          ((args->rb_ind >= FLARE32_NUM_SPRS) ? "args->rb_ind" : ""));
      }
      else
      {
        fpr (stream, "%s\t%s, %s",
          args->opc_info->names[args->fw],
          gprs[args->ra_ind].name,
          sprs[args->rb_ind].name);
      }
    }
      break;
    case FLARE32_OA_SA_RB:
    {
      if (args->length != 2 || args->ra_ind >= FLARE32_NUM_SPRS)
      {
        fpr (stream, "bad (grp 0x%x; `FLARE32_OA_SA_RB`: %s; %s;)",
          (unsigned) args->grp,
          ((args->length != 2) ? "`pre`/lpre`" : ""),
          ((args->ra_ind >= FLARE32_NUM_SPRS) ? "args->ra_ind" : ""));
      }
      else
      {
        fpr (stream, "%s\t%s, %s",
          args->opc_info->names[args->fw],
          sprs[args->ra_ind].name,
          gprs[args->rb_ind].name);
      }
    }
      break;
    case FLARE32_OA_SA_SB:
    {
      if (args->length != 2
        || args->ra_ind >= FLARE32_NUM_SPRS
        || args->rb_ind >= FLARE32_NUM_SPRS)
      {
        fpr (stream, "bad (grp 0x%x; `FLARE32_OA_SA_SB`: %s; %s; %s;)",
          (unsigned) args->grp,
          ((args->length != 2) ? "`pre`/lpre`" : ""),
          ((args->ra_ind >= FLARE32_NUM_SPRS) ? "args->ra_ind" : ""),
          ((args->rb_ind >= FLARE32_NUM_SPRS) ? "args->rb_ind" : ""));
      }
      else
      {
        fpr (stream, "%s\t%s, %s",
          args->opc_info->names[args->fw],
          sprs[args->ra_ind].name,
          sprs[args->rb_ind].name);
      }
    }
      break;
    case FLARE32_OA_PC_RB:
    {
      if (args->length != 2)
      {
        fpr (stream, "bad (grp 0x%x; `FLARE32_OA_PC_RB`: %s)",
          (unsigned) args->grp,
          "`pre`/lpre`");
      }
      else
      {
        fpr (stream, "%s\tpc, %s",
          args->opc_info->names[args->fw],
          gprs[args->rb_ind].name);
      }
    }
      break;
    case FLARE32_OA_RA_RB_LDST:
    {
      if (args->length != 2)
      {
        fpr (stream, "%s%x%s",
          "bad (grp 0x",
          (unsigned) args->grp,
          "; `FLARE32_OA_RA_RB_LDST`; `pre`/`lpre`)");
      }
      else
      {
        fpr (stream, "%s\t%s, [%s]",
          args->opc_info->names[args->fw],
          gprs[args->ra_ind].name,
          gprs[args->rb_ind].name);
      }
    }
      break;
    case FLARE32_OA_SA_RB_LDST:
    {
      if (args->length != 2)
      {
        fpr (stream, "%s%x%s",
          "bad (grp 0x",
          (unsigned) args->grp,
          "; `FLARE32_OA_SA_RB_LDST`; `pre`/`lpre`)");
      }
      else
      {
        fpr (stream, "%s\t%s, [%s]",
          args->opc_info->names[args->fw],
          sprs[args->ra_ind].name,
          gprs[args->rb_ind].name);
      }
    }
      break;
    case FLARE32_OA_SA_SB_LDST:
    {
      if (args->length != 2)
      {
        fpr (stream, "%s%x%s",
          "bad (grp 0x",
          (unsigned) args->grp,
          "; `FLARE32_OA_SA_SB_LDST`; `pre`/`lpre`)");
      }
      else
      {
        fpr (stream, "%s\t%s, [%s]",
          args->opc_info->names[args->fw],
          sprs[args->ra_ind].name,
          sprs[args->rb_ind].name);
      }
    }
      break;
    //case FLARE32_OA_RA_RB_RC_LDST:
    //  break;
    case FLARE32_OA_RA_RB_S5_LDST:
    {
        fpr (stream, "%s\t%s, [%s, #%i]%s",
          args->opc_info->names[args->fw],
          gprs[args->ra_ind].name,
          gprs[args->rb_ind].name,
          (signed) args->simm,
          do_snprintf_insn_flare32_maybe_pre_lpre
            (args->length, args->iword, args->grp));
    }
      break;
    //case FLARE32_OA_RA_RB_RC_S5_LDST:
    //  break;
    default:
      fpr (stream, "bad (grp 0x%x; oparg 0x%x; opcode 0x%x; name %s)",
        (unsigned) args->grp, (unsigned) args->opc_info->oparg,
        (unsigned) args->opc_info->opcode,
        args->opc_info->names[args->fw]);
      break;
    /* -------- */
  }
}

static bfd_vma loc_addr = 0;
static struct disassemble_info *loc_info = NULL;

static int
print_insn_flare32_rd16 (flare32_dasm_info_t *dasm_info)
{
  return loc_info->read_memory_func
    (loc_addr + dasm_info->length, dasm_info->buffer, 2, loc_info);
}
int
print_insn_flare32 (bfd_vma addr, struct disassemble_info *info)
{
  stream = info->stream;
  fpr = info->fprintf_func;

  //int
  //  length = 0,
  //  status;
  //bfd_byte buffer[2];
  //const flare32_opc_info_t *opc_info;
  //flare32_temp_t
  //  iword,
  //  simm = 0,
  //  grp,
  //  ra_ind,
  //  rb_ind,
  //  fw = 0;

  loc_addr = addr;
  loc_info = info;

  flare32_dasm_info_t dasm_info;
  flare32_dasm_info_ctor (&dasm_info, &print_insn_flare32_rd16);
  flare32_dasm_info_do_disassemble (&dasm_info);

  if (dasm_info.status)
  {
    //goto fail;
    info->memory_error_func (dasm_info.status, addr, info);
    return -1;
  }
  else if (dasm_info.grp_decode_err)
  {
    fpr (stream, "bad (unknown grp 0x%x)",
      (unsigned) dasm_info.grp);
  }
  else if (dasm_info.g7_subgrp_decode_err)
  {
    fpr (stream, "bad (grp 0x%x; unknown subgrp "
      "(checked: 0x%x, 0x%x))",
      (unsigned) dasm_info.grp,
      (unsigned) dasm_info.aluopbh_subgrp,
      (unsigned) dasm_info.sprldst_subgrp);
  }
  else
  {
    do_print_insn_flare32 (&dasm_info);
  }

  return dasm_info.length;
}

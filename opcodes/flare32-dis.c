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
const char *gpr_names[FLARE32_NUM_GPRS] =
{
  "r0", "r1", "r2", "r3",
  "r4", "r5", "r6", "r7",
  "r8", "r9", "r10", "r11",
  "r12", "lr", "fp", "sp",
};
/* Special purpose register names */
const char *spr_names[FLARE32_NUM_SPRS] =
{
  "pc", "flags", "ids", "ira",
  "ie", "hi", "lo", "badspr7",
  "badspr8", "badspr9", "badspr10", "badspr11",
  "badspr12", "badspr13", "badspr14", "badspr15",
};

static char*
snprintf_insn_flare32_maybe_pre_lpre (int length, flare32_temp_t iword,
                                    flare32_temp_t grp)
{
  if (length == 2)
  {
    cbuf[0] = '\0';
  }
  else if (length == 4)
  {
    snprintf(cbuf, FLARE32_DIS_CBUF_LIM,
      " ; pre #0x%x",
      (unsigned)GET_INSN_FIELD (FLARE32_PRE_S12_MASK,
        FLARE32_PRE_S12_BITPOS, iword));
  }
  else if (length == 6)
  {
    if (grp != FLARE32_G3_GRP_VALUE) /* groups 1, 5, or 6 */
    {
      snprintf(cbuf, FLARE32_DIS_CBUF_LIM,
        " ; lpre #0x%x",
        (unsigned)GET_INSN_FIELD (FLARE32_G1G5G6_LPRE_S27_MASK,
          FLARE32_G1G5G6_LPRE_S27_BITPOS, iword));
    }
    else /* if (grp == FLARE32_G3_GRP_VALUE) */
    {
      snprintf(cbuf, FLARE32_DIS_CBUF_LIM,
        " ; lpre #0x%x",
        (unsigned)GET_INSN_FIELD (FLARE32_G3_LPRE_S23_MASK,
          FLARE32_G3_LPRE_S23_BITPOS, iword));
    }
  }
  return cbuf;
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
    rc_ind,
    f;

  if ((status = info->read_memory_func (addr + length, buffer, 2, info)))
  {
    goto fail;
  }
  iword = bfd_getb16 (buffer);

  if (GET_INSN_FIELD (FLARE32_PRE_FULL_GRP_MASK, 
    FLARE32_PRE_FULL_GRP_BITPOS, iword) == FLARE32_PRE_FULL_GRP_VALUE)
  {
    /* `pre` */
    length += 2;
    //simm = GET_INSN_FIELD (FLARE32_PRE_S12_MASK, FLARE32_PRE_S12_BITPOS,
    //  iword);

    if ((status = info->read_memory_func (addr + length, buffer, 2, info)))
    {
      goto fail;
    }
    iword = (iword << FLARE32_PRE_LPRE_EXT_BITPOS) | bfd_getb16 (buffer);
  }
  else if (GET_INSN_FIELD (FLARE32_LPRE_FULL_GRP_16_MASK,
    FLARE32_LPRE_FULL_GRP_16_BITPOS, iword) == FLARE32_LPRE_FULL_GRP_VALUE)
  {
    /* `lpre` */
    length += 2;
    if ((status = info->read_memory_func (addr + length, buffer, 2, info)))
    {
      goto fail;
    }
    iword = (iword << 16ull) | bfd_getb16 (buffer);

    length += 2;

    //iword <<= FLARE32_PRE_LPRE_EXT_BITPOS;
    if ((status = info->read_memory_func (addr + length, buffer, 2, info)))
    {
      goto fail;
    }
    iword = (iword << FLARE32_PRE_LPRE_EXT_BITPOS) | bfd_getb16 (buffer);
  }

  length += 2;

  grp = GET_INSN_FIELD (FLARE32_GRP_16_MASK,
    FLARE32_GRP_16_BITPOS, iword);
  ra_ind = GET_INSN_FIELD (FLARE32_RA_IND_MASK,
    FLARE32_RA_IND_BITPOS, iword);
  rc_ind = GET_INSN_FIELD (FLARE32_RC_IND_MASK,
    FLARE32_RC_IND_BITPOS, iword);

  switch (grp)
  {
    /* -------- */
    case FLARE32_G1_GRP_VALUE:
    {
      /* -------- */
      if (length == 2)
      {
        simm = GET_INSN_FIELD (FLARE32_G1G5G6_S5_MASK,
          FLARE32_G1G5G6_S5_BITPOS, iword);
      }
      else if (length == 4) /* `pre` */
      {
        simm = flare32_get_g1g5g6_s17
          ((iword >> FLARE32_PRE_LPRE_EXT_BITPOS), iword);
      }
      else if (length == 6) /* `lpre` */
      {
        simm = flare32_get_g1g5g6_s32
          ((iword >> FLARE32_PRE_LPRE_EXT_BITPOS), iword);
      }
      /* -------- */
      opc_info = &flare32_opc_info_g1
        [GET_INSN_FIELD (FLARE32_G1_OP_MASK, FLARE32_G1_OP_BITPOS, iword)];
      /* -------- */
      switch (opc_info->oparg)
      {
        /* -------- */
        case FLARE32_OA_RA_S5:
        {
          fpr (stream, "%s\t%s, #0x%x%s",
            opc_info->names[0], gpr_names[ra_ind], (unsigned)simm,
            snprintf_insn_flare32_maybe_pre_lpre(length, iword, grp));
        }
          break;
        case FLARE32_OA_RA_PC_S5:
        {
          fpr (stream, "%s\t%s, pc, #0x%x%s",
            opc_info->names[0], gpr_names[ra_ind], (unsigned)simm,
            snprintf_insn_flare32_maybe_pre_lpre(length, iword, grp));
        }
          break;
        case FLARE32_OA_RA_SP_S5:
        {
          fpr (stream, "%s\t%s, sp, #0x%x%s",
            opc_info->names[0], gpr_names[ra_ind], (unsigned)simm,
            snprintf_insn_flare32_maybe_pre_lpre(length, iword, grp));
        }
          break;
        case FLARE32_OA_RA_FP_S5:
        {
          fpr (stream, "%s\t%s, fp, #0x%x%s",
            opc_info->names[0], gpr_names[ra_ind], (unsigned)simm,
            snprintf_insn_flare32_maybe_pre_lpre(length, iword, grp));
        }
          break;
        case FLARE32_OA_RA:
        {
          if (length != 2)
          {
            fpr (stream, "%s",
              "bad (grp 1; `FLARE32_OA_RA`; `pre`/`lpre`)");
          }
          else
          {
            fpr (stream, "%s\t%s",
              opc_info->names[0], gpr_names[ra_ind]);
          }
        }
          break;
        default:
          fpr (stream, "bad (grp 1)");
          break;
        /* -------- */
      }
      /* -------- */
    }
      break;
    /* -------- */
    case FLARE32_G2_GRP_VALUE:
    {
      /* -------- */
      opc_info = &flare32_opc_info_g2
        [GET_INSN_FIELD (FLARE32_G2_OP_MASK, FLARE32_G2_OP_BITPOS, iword)];
      f = GET_INSN_FIELD (FLARE32_G2_F_MASK, FLARE32_G2_F_BITPOS, iword);
      /* -------- */
      switch (opc_info->oparg)
      {
        case FLARE32_OA_RA_RC:
        {
          if (length != 2)
          {
            fpr (stream, "%s",
              "bad (grp 2; `FLARE32_OA_RA_RC`; `pre`/`lpre`)");
          }
          else
          {
            fpr (stream, "%s\t%s, %s",
              opc_info->names[f], gpr_names[ra_ind], gpr_names[rc_ind]);
          }
        }
          break;
        case FLARE32_OA_RA_SP_RC:
        {
          if (length != 2)
          {
            fpr (stream, "%s",
              "bad (grp 2; `FLARE32_OA_RA_SP_RC`; `pre`/`lpre`)");
          }
          else
          {
            fpr (stream, "%s\t%s, sp, %s",
              opc_info->names[f], gpr_names[ra_ind], gpr_names[rc_ind]);
          }
        }
          break;
        case FLARE32_OA_RA_FP_RC:
        {
          if (length != 2)
          {
            fpr (stream, "%s",
              "bad (grp 2; `FLARE32_OA_RA_FP_RC`; `pre`/`lpre`)");
          }
          else
          {
            fpr (stream, "%s\t%s, fp, %s",
              opc_info->names[f], gpr_names[ra_ind], gpr_names[rc_ind]);
          }
        }
          break;
        default:
          fpr (stream, "bad (grp 2)");
          break;
      }
      /* -------- */
    }
      break;
    /* -------- */
    case FLARE32_G3_GRP_VALUE:
    {
      /* -------- */
      if (length == 2)
      {
        simm = GET_INSN_FIELD (FLARE32_G3_S9_MASK,
          FLARE32_G3_S9_BITPOS, iword);
      }
      else if (length == 4) /* `pre` */
      {
        simm = flare32_get_g3_s21
          ((iword >> FLARE32_PRE_LPRE_EXT_BITPOS), iword);
      }
      else if (length == 6) /* `lpre` */
      {
        simm = flare32_get_g3_s32
          ((iword >> FLARE32_PRE_LPRE_EXT_BITPOS), iword);
      }
      /* -------- */
      opc_info = &flare32_opc_info_g3
        [GET_INSN_FIELD (FLARE32_G3_OP_MASK, FLARE32_G3_OP_BITPOS, iword)];
      /* -------- */
      switch (opc_info->oparg)
      {
        case (FLARE32_OA_PCREL_S9):
        {
          fpr (stream, "%s\t#0x%x%s",
            opc_info->names[0], (unsigned)simm,
            snprintf_insn_flare32_maybe_pre_lpre(length, iword, grp));
        }
          break;
        default:
          fpr (stream, "bad (grp 3)");
          break;
      }
      /* -------- */
    }
      break;
    /* -------- */
    case FLARE32_G4_GRP_VALUE:
    {
      /* -------- */
      opc_info = &flare32_opc_info_g4
        [GET_INSN_FIELD (FLARE32_G4_OP_MASK, FLARE32_G4_OP_BITPOS, iword)];
      /* -------- */
      switch (opc_info->oparg)
      {
        case FLARE32_OA_RA:
        {
          if (length != 2)
          {
            fpr (stream, "%s",
              "bad (grp 4; `FLARE32_OA_RA`; `pre`/`lpre`)");
          }
          else
          {
            fpr (stream, "%s\t%s",
              opc_info->names[0], gpr_names[ra_ind]);
          }
        }
          break;
        case FLARE32_OA_RA_RC:
        {
          if (length != 2)
          {
            fpr (stream, "%s",
              "bad (grp 4; `FLARE32_OA_RA_RC`; `pre`/`lpre`)");
          }
          else
          {
            fpr (stream, "%s\t%s, %s",
              opc_info->names[0], gpr_names[ra_ind], gpr_names[rc_ind]);
          }
        }
          break;
        case FLARE32_OA_IRA:
        {
          if (length != 2)
          {
            fpr (stream, "%s",
              "bad (grp 4; `FLARE32_OA_IRA`; `pre`/`lpre`)");
          }
          else
          {
            fpr (stream, "%s\t%s",
              opc_info->names[0], "ira");
          }
        }
          break;
        case FLARE32_OA_NONE:
        {
          if (length != 2)
          {
            fpr (stream, "%s",
              "bad (grp 4; `FLARE32_OA_NONE`; `pre`/`lpre`)");
          }
          else
          {
            fpr (stream, "%s",
              opc_info->names[0]);
          }
        }
          break;
        case FLARE32_OA_RA_SC:
        {
          if (length != 2)
          {
            fpr (stream, "%s",
              "bad (grp 4; `FLARE32_OA_RA_SC`; `pre`/`lpre`)");
          }
          else
          {
            fpr (stream, "%s\t%s, %s",
              opc_info->names[0], gpr_names[ra_ind], spr_names[rc_ind]);
          }
        }
          break;
        case FLARE32_OA_SA_RC:
        {
          if (length != 2)
          {
            fpr (stream, "%s",
              "bad (grp 4; `FLARE32_OA_SA_RC`; `pre`/`lpre`)");
          }
          else
          {
            fpr (stream, "%s\t%s, %s",
              opc_info->names[0], spr_names[ra_ind], gpr_names[rc_ind]);
          }
        }
          break;
        case FLARE32_OA_RA_RC_LDST:
        {
          if (length != 2)
          {
            fpr (stream, "%s",
              "bad (grp 4; `FLARE32_OA_RA_RC_LDST`; `pre`/`lpre`)");
          }
          else
          {
            fpr (stream, "%s\t%s, [%s]",
              opc_info->names[0], gpr_names[ra_ind], gpr_names[rc_ind]);
          }
        }
          break;
        default:
          fpr (stream, "bad (grp 4)");
          break;
      }
      /* -------- */
    }
      break;
    /* -------- */
    case FLARE32_G5_GRP_VALUE:
    {
      /* -------- */
      if (length == 2)
      {
        simm = GET_INSN_FIELD (FLARE32_G1G5G6_S5_MASK,
          FLARE32_G1G5G6_S5_BITPOS, iword);
      }
      else if (length == 4) /* `pre` */
      {
        simm = flare32_get_g1g5g6_s17
          ((iword >> FLARE32_PRE_LPRE_EXT_BITPOS), iword);
      }
      else if (length == 6) /* `lpre` */
      {
        simm = flare32_get_g1g5g6_s32
          ((iword >> FLARE32_PRE_LPRE_EXT_BITPOS), iword);
      }
      /* -------- */
      opc_info = &flare32_opc_info_g5[0];
      /* -------- */
      switch (opc_info->oparg)
      {
        case FLARE32_OA_RA_RC_S5_LDST:
        {
            fpr (stream, "%s\t%s, [%s, #0x%x]%s",
              opc_info->names[0], gpr_names[ra_ind], gpr_names[rc_ind],
              (unsigned)simm,
              snprintf_insn_flare32_maybe_pre_lpre(length, iword, grp));
        }
          break;
        default:
          fpr (stream, "bad (grp 5)");
          break;
      }
    }
      break;
    /* -------- */
    case FLARE32_G6_GRP_VALUE:
    {
      /* -------- */
      if (length == 2)
      {
        simm = GET_INSN_FIELD (FLARE32_G1G5G6_S5_MASK,
          FLARE32_G1G5G6_S5_BITPOS, iword);
      }
      else if (length == 4) /* `pre` */
      {
        simm = flare32_get_g1g5g6_s17
          ((iword >> FLARE32_PRE_LPRE_EXT_BITPOS), iword);
      }
      else if (length == 6) /* `lpre` */
      {
        simm = flare32_get_g1g5g6_s32
          ((iword >> FLARE32_PRE_LPRE_EXT_BITPOS), iword);
      }
      /* -------- */
      opc_info = &flare32_opc_info_g6[0];
      /* -------- */
      switch (opc_info->oparg)
      {
        case FLARE32_OA_RA_RC_S5_LDST:
        {
            fpr (stream, "%s\t%s, [%s, #0x%x]%s",
              opc_info->names[0], gpr_names[ra_ind], gpr_names[rc_ind],
              (unsigned)simm,
              snprintf_insn_flare32_maybe_pre_lpre(length, iword, grp));
        }
          break;
        default:
          fpr (stream, "bad (grp 6)");
          break;
      }
      /* -------- */
    }
      break;
    /* -------- */
    default:
      fpr (stream, "bad");
      break;
  }

  return length;
    
fail:
  info->memory_error_func (status, addr, info);
  return -1;
}

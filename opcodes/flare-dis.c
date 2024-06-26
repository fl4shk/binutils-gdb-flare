/* Disassemble flare instructions.
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

//#ifndef FLARE_CLANGD
#include "../include/opcode/flare.h"
#include "../include/opcode/flare-opc-decls.h"
#include "../include/opcode/flare-dasm-info-funcs.h"
//#else
//#include "opcode/flare.h"
//#include "opcode/flare-opc-decls.h"
//#include "opcode/flare-dasm-info-funcs.h"
//#endif
#include "disassemble.h"

static fprintf_ftype fpr;
static void *stream;

#define FLARE_DIS_CBUF_LIM 50
static char cbuf[FLARE_DIS_CBUF_LIM];
/* -------- */
/* General purpose register names */
const flare_reg_t gprs[FLARE_NUM_GPRS] = FLARE_INST_GPRS ();
/* Special purpose register names */
const flare_reg_t sprs[FLARE_NUM_SPRS] = FLARE_INST_SPRS ();

const flare_reg_t reg_pc = FLARE_INST_REG_PC ();

static char *
do_snprintf_insn_flare_maybe_pre_lpre
  //(int length, flare_temp_t iword, //flare_temp_t grp
  //const flare_opc_info_t *opc_info)
  (flare_dasm_info_t *args)
{
  const size_t
    temp_length = args->length - args->have_index * 2ull;
  if (temp_length == 2)
  {
    cbuf[0] = '\0';
  }
  else if (temp_length == 4)
  {
    snprintf (cbuf, FLARE_DIS_CBUF_LIM,
      " // pre #0x%x",
      //(unsigned) GET_INSN_FIELD (FLARE_PRE_S12_MASK,
      //  FLARE_PRE_S12_BITPOS, iword)
      (unsigned) flare_get_insn_field_ei
        (&flare_enc_info_g0_pre_s12,
        args->iword >> FLARE_ONE_EXT_BITPOS)
    );
  }
  else if (temp_length == 6)
  {
    const flare_grp_info_t
      *grp_info = args->opc_info->grp_info;
    if (grp_info->grp_value != FLARE_G3_GRP_VALUE)
      /* instructions with simm5/uimm5 */
    {
      if (grp_info->grp_value == FLARE_G7_GRP_VALUE
        && grp_info->subgrp != &flare_enc_info_g7_icreload_subgrp)
      {
        snprintf (cbuf, FLARE_DIS_CBUF_LIM,
          " // lpre g7 bad `subgrp`");
      }
      else
      {
        snprintf (cbuf, FLARE_DIS_CBUF_LIM,
          " // lpre #0x%x",
          //(unsigned) GET_INSN_FIELD (FLARE_G0_LPRE_S27_MASK,
          //  FLARE_G0_LPRE_S27_BITPOS, iword)
          (unsigned) flare_get_insn_field_ei
            (&flare_enc_info_g0_lpre_s27,
            args->iword >> FLARE_ONE_EXT_BITPOS)
        );
      }
    }
    else /* if (grp_info->grp_value == FLARE_G3_GRP_VALUE) */
    {
      snprintf (cbuf, FLARE_DIS_CBUF_LIM,
        " // lpre #0x%x",
        //(unsigned) GET_INSN_FIELD (FLARE_G0_LPRE_S23_MASK,
        //  FLARE_G0_LPRE_S23_BITPOS, iword)
        (unsigned) flare_get_insn_field_ei
          (&flare_enc_info_g0_lpre_s23,
          args->iword >> FLARE_ONE_EXT_BITPOS)
      );
    }
  }
  return cbuf;
}

//void
//do_print_insn_flare (flare_dasm_info_t *args);
static void
do_print_insn_flare (flare_dasm_info_t *args)
{
  switch (args->opc_info->oparg)
  {
    /* -------- */
    case FLARE_OA_NONE:
    {
      fpr (stream, "%s",
        args->opc_info->names[args->fw]);
    }
      break;
    case FLARE_OA_RA_RB_XCHG_LOCK:
    {
      fpr (stream, "%s\t[%s], %s",
	args->opc_info->names[args->fw],
	gprs[args->ra_ind].name,
	gprs[args->rb_ind].name);
    }
      break;
    case FLARE_OA_RA_RB_XCHG:
    {
      fpr (stream, "%s\t%s, %s",
	args->opc_info->names[args->fw],
	gprs[args->ra_ind].name,
	gprs[args->rb_ind].name);
    }
      break;
    case FLARE_OA_RA_RC_RB_CMPXCHG:
    {
      fpr (stream, "%s\t[%s], %s, %s",
        args->opc_info->names[args->fw],
        gprs[args->ra_ind].name,
        gprs[args->rc_ind].name,
        gprs[args->rb_ind].name);
    }
      break;
    case FLARE_OA_RA_RC_RB_CMPXCHG_LOCK:
    {
      fpr (stream, "%s\t[%s], %s, %s",
        args->opc_info->names[args->fw],
        gprs[args->ra_ind].name,
        gprs[args->rc_ind].name,
        gprs[args->rb_ind].name);
    }
      break;
    case FLARE_OA_RA_S5:
    {
      fpr (stream, "%s\t%s, #%i%s",
        args->opc_info->names[args->fw],
        gprs[args->ra_ind].name,
        (signed) args->simm,
        //do_snprintf_insn_flare_maybe_pre_lpre
        //  (args->length, args->iword, args->grp)
        do_snprintf_insn_flare_maybe_pre_lpre (args));
    }
      break;
    case FLARE_OA_RA_PC_S5:
    {
      fpr (stream, "%s\t%s, pc, #%i%s",
        args->opc_info->names[args->fw],
        gprs[args->ra_ind].name,
        (signed) args->simm,
        do_snprintf_insn_flare_maybe_pre_lpre (args));
    }
      break;
    case FLARE_OA_RA_SP_S5:
    {
      fpr (stream, "%s\t%s, sp, #%i%s",
        args->opc_info->names[args->fw],
        gprs[args->ra_ind].name,
        (signed) args->simm,
        do_snprintf_insn_flare_maybe_pre_lpre (args));
    }
      break;
    case FLARE_OA_RA_FP_S5:
    {
      fpr (stream, "%s\t%s, fp, #%i%s",
        args->opc_info->names[args->fw],
        gprs[args->ra_ind].name,
        (signed) args->simm,
        do_snprintf_insn_flare_maybe_pre_lpre (args));
    }
      break;
    case FLARE_OA_RA_U5:
    {
      if (args->length == 2)
      {
        fpr (stream, "%s\t%s, #%u%s",
          args->opc_info->names[args->fw],
          gprs[args->ra_ind].name,
          (unsigned) flare_zero_extend (args->simm,
            flare_enc_info_g1g5g6_i5.bitsize),
          do_snprintf_insn_flare_maybe_pre_lpre (args));
      }
      else
      {
        fpr (stream, "%s\t%s, #%i%s",
          args->opc_info->names[args->fw],
          gprs[args->ra_ind].name,
          (signed) args->simm,
          do_snprintf_insn_flare_maybe_pre_lpre (args));
      }
    }
      break;
    case FLARE_OA_U5:
    {
      if (args->length == 2)
      {
        fpr (stream, "%s\t#%u%s",
          args->opc_info->names[args->fw],
          (unsigned) flare_zero_extend (args->simm,
            flare_enc_info_g1g5g6_i5.bitsize),
          do_snprintf_insn_flare_maybe_pre_lpre (args));
      }
      else
      {
        fpr (stream, "%s\t#%i%s",
          args->opc_info->names[args->fw],
          (signed) args->simm,
          do_snprintf_insn_flare_maybe_pre_lpre (args));
      }
    }
      break;
    case FLARE_OA_RA:
    {
      if (args->length != 2)
      {
        fpr (stream, "%s%x%s",
          "bad (grp 0x",
          (unsigned) args->grp,
          "; `FLARE_OA_RA`; `pre`/`lpre`)");
      }
      else
      {
        fpr (stream, "%s\t%s",
          args->opc_info->names[args->fw],
          gprs[args->ra_ind].name);
      }
    }
      break;
    case FLARE_OA_RA_RB:
    {
      if (args->length != 2)
      {
        fpr (stream, "%s%x%s",
          "bad (grp 0x",
          (unsigned) args->grp,
          "; `FLARE_OA_RA_RB`; `pre`/`lpre`)");
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
    case FLARE_OA_RA_SP_RB:
    {
      if (args->length != 2)
      {
        fpr (stream, "%s%x%s",
          "bad (grp 0x",
          (unsigned) args->grp,
          "; `FLARE_OA_RA_SP_RB`; `pre`/`lpre`)");
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
    case FLARE_OA_RA_FP_RB:
    {
      if (args->length != 2)
      {
        fpr (stream, "%s%x%s",
          "bad (grp 0x",
          (unsigned) args->grp,
          "; `FLARE_OA_RA_FP_RB`; `pre`/`lpre`)");
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
    case FLARE_OA_PCREL_S9:
    {
      fpr (stream, "%s\t%i%s",
        args->opc_info->names[args->fw], (signed) args->simm,
        do_snprintf_insn_flare_maybe_pre_lpre (args));
    }
      break;
    case FLARE_OA_IRA:
    {
      if (args->length != 2)
      {
        fpr (stream, "%s%x%s",
          "bad (grp 0x",
          (unsigned) args->grp,
          "; `FLARE_OA_IRA`; `pre`/`lpre`)");
      }
      else
      {
        fpr (stream, "%s\t%s",
          args->opc_info->names[args->fw], "ira");
      }
    }
      break;
    case FLARE_OA_RA_SB:
    {
      if (args->length != 2 || args->rb_ind >= FLARE_NUM_SPRS)
      {
        fpr (stream, "bad (grp 0x%x; `FLARE_OA_RA_SC`: %s; %s;)",
          (unsigned) args->grp,
          ((args->length != 2) ? "`pre`/lpre`" : ""),
          ((args->rb_ind >= FLARE_NUM_SPRS) ? "args->rb_ind" : ""));
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
    case FLARE_OA_SA_RB:
    {
      if (args->length != 2 || args->ra_ind >= FLARE_NUM_SPRS)
      {
        fpr (stream, "bad (grp 0x%x; `FLARE_OA_SA_RB`: %s; %s;)",
          (unsigned) args->grp,
          ((args->length != 2) ? "`pre`/lpre`" : ""),
          ((args->ra_ind >= FLARE_NUM_SPRS) ? "args->ra_ind" : ""));
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
    case FLARE_OA_SA_SB:
    {
      if (args->length != 2
        || args->ra_ind >= FLARE_NUM_SPRS
        || args->rb_ind >= FLARE_NUM_SPRS)
      {
        fpr (stream, "bad (grp 0x%x; `FLARE_OA_SA_SB`: %s; %s; %s;)",
          (unsigned) args->grp,
          ((args->length != 2) ? "`pre`/lpre`" : ""),
          ((args->ra_ind >= FLARE_NUM_SPRS) ? "args->ra_ind" : ""),
          ((args->rb_ind >= FLARE_NUM_SPRS) ? "args->rb_ind" : ""));
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
    case FLARE_OA_PC_RB:
    {
      if (args->length != 2)
      {
        fpr (stream, "bad (grp 0x%x; `FLARE_OA_PC_RB`: %s)",
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
    case FLARE_OA_RA_RB_LDST:
    {
      if (
	args->length != 2
      )
      {
        fpr (stream, "%s%x%s",
          "bad (grp 0x",
          (unsigned) args->grp,
          "; `FLARE_OA_RA_RB_LDST`; `pre`/`lpre`)");
      }
      else
      {
	if (args->have_index)
	{
	  fpr (stream, "%s\t%s, [%s, %s]",
	    args->opc_info->names[args->fw],
	    gprs[args->ra_ind].name,
	    gprs[args->index_ra_ind].name,
	    gprs[args->rc_ind].name);
	}
	else
	{
	  fpr (stream, "%s\t%s, [%s]",
	    args->opc_info->names[args->fw],
	    gprs[args->ra_ind].name,
	    gprs[args->rb_ind].name);
	}
      }
    }
      break;
    case FLARE_OA_SA_RB_LDST:
    {
      if (args->length != 2)
      {
        fpr (stream, "%s%x%s",
          "bad (grp 0x",
          (unsigned) args->grp,
          "; `FLARE_OA_SA_RB_LDST`; `pre`/`lpre`)");
      }
      else
      {
	//if (args->have_index)
	//{
	//  fpr (stream, "%s\t%s, [%s, %s]",
	//    args->opc_info->names[args->fw],
	//    sprs[args->ra_ind].name,
	//    gprs[args->rb_ind].name,
	//    gprs[args->rc_ind].name);
	//}
	//else
	{
	  fpr (stream, "%s\t%s, [%s]",
	    args->opc_info->names[args->fw],
	    sprs[args->ra_ind].name,
	    gprs[args->rb_ind].name);
	}
      }
    }
      break;
    case FLARE_OA_SA_SB_LDST:
    {
      if (args->length != 2)
      {
        fpr (stream, "%s%x%s",
          "bad (grp 0x",
          (unsigned) args->grp,
          "; `FLARE_OA_SA_SB_LDST`; `pre`/`lpre`)");
      }
      else
      {
	//if (args->have_index)
	//{
	//  fpr (stream, "%s\t%s, [%s, %s]",
	//    args->opc_info->names[args->fw],
	//    sprs[args->ra_ind].name,
	//    sprs[args->rb_ind].name,
	//    gprs[args->rc_ind].name);
	//}
	//else
	{
	  fpr (stream, "%s\t%s, [%s]",
	    args->opc_info->names[args->fw],
	    sprs[args->ra_ind].name,
	    sprs[args->rb_ind].name);
	}
      }
    }
      break;
    //case FLARE_OA_RA_RB_RC_LDST:
    //  break;
    case FLARE_OA_RA_RB_S5_LDST:
    {
      if (args->have_index)
      {
	fpr (stream, "%s\t%s, [%s, %s, #%i]%s",
	  args->opc_info->names[args->fw],
	  gprs[args->ra_ind].name,
	  gprs[args->index_ra_ind].name,
	  gprs[args->rc_ind].name,
	  (signed) args->simm,
	  do_snprintf_insn_flare_maybe_pre_lpre (args));
      }
      else
      {
	fpr (stream, "%s\t%s, [%s, #%i]%s",
	  args->opc_info->names[args->fw],
	  gprs[args->ra_ind].name,
	  gprs[args->rb_ind].name,
	  (signed) args->simm,
	  do_snprintf_insn_flare_maybe_pre_lpre (args));
      }
    }
      break;
    case FLARE_OA_RA_S5_JUSTADDR:
    {
      if (args->have_index)
      {
	fpr (stream, "%s\t[%s, %s, #%i]%s",
	  args->opc_info->names[args->fw],
	  gprs[args->index_ra_ind].name,
	  gprs[args->rc_ind].name,
	  (signed) args->simm,
	  do_snprintf_insn_flare_maybe_pre_lpre (args));
      }
      else
      {
	fpr (stream, "%s\t[%s, #%i]%s",
	  args->opc_info->names[args->fw],
	  gprs[args->ra_ind].name,
	  (signed) args->simm,
	  do_snprintf_insn_flare_maybe_pre_lpre (args));
      }
    }
      break;
    //case FLARE_OA_RA_RB_RC_S5_LDST:
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
print_insn_flare_rd16 (flare_dasm_info_t *dasm_info)
{
  return loc_info->read_memory_func
    (loc_addr + dasm_info->length, dasm_info->buffer, 2, loc_info);
}

int
print_insn_flare (bfd_vma addr, struct disassemble_info *info)
{
  stream = info->stream;
  fpr = info->fprintf_func;

  //int
  //  length = 0,
  //  status;
  //bfd_byte buffer[2];
  //const flare_opc_info_t *opc_info;
  //flare_temp_t
  //  iword,
  //  simm = 0,
  //  grp,
  //  ra_ind,
  //  rb_ind,
  //  fw = 0;

  loc_addr = addr;
  loc_info = info;

  flare_dasm_info_t dasm_info;
  flare_dasm_info_ctor (&dasm_info, &print_insn_flare_rd16);
  flare_dasm_info_do_disassemble (&dasm_info);

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
      "(checked: 0x%x, 0x%x, 0x%x))",
      (unsigned) dasm_info.grp,
      (unsigned) dasm_info.g7_aluopbh_subgrp,
      (unsigned) dasm_info.g7_sprldst_subgrp,
      (unsigned) dasm_info.g7_icreload_subgrp);
  }
  else
  {
    do_print_insn_flare (&dasm_info);
  }

  return dasm_info.length; //+ dasm_info.have_index * 2;
}

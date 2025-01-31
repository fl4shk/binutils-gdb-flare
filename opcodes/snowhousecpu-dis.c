/* Disassemble snowhousecpu instructions.
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

//#ifndef SNOWHOUSECPU_CLANGD
//#include "../include/opcode/snowhousecpu.h"
//#include "../include/opcode/snowhousecpu-opc-decls.h"
//#include "../include/opcode/snowhousecpu-dasm-info-funcs.h"
//#else
#include "opcode/snowhousecpu.h"
#include "opcode/snowhousecpu-opc-decls.h"
#include "opcode/snowhousecpu-dasm-info-funcs.h"
//#endif
#include "disassemble.h"

static fprintf_ftype fpr;
static void *stream;

#define SNOWHOUSECPU_DIS_CBUF_LIM 100
static char cbuf[SNOWHOUSECPU_DIS_CBUF_LIM];
/* -------- */
/* General purpose register names */
const snowhousecpu_reg_t gprs[SNOWHOUSECPU_NUM_GPRS] = SNOWHOUSECPU_INST_GPRS ();
/* Special purpose register names */
const snowhousecpu_reg_t sprs[SNOWHOUSECPU_NUM_SPRS] = SNOWHOUSECPU_INST_SPRS ();

const snowhousecpu_reg_t reg_pc = SNOWHOUSECPU_INST_REG_PC ();
extern void
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
  snowhousecpu_temp_t temp_simm32 = temp_simm16;

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

  self->simm = temp_simm32; // change it for the bit shifts by immediate values
  snowhousecpu_dasm_info_do_disassemble_worker (self);
}

static char *
do_snprintf_insn_snowhousecpu_maybe_pre
  //(int length, snowhousecpu_temp_t iword, //snowhousecpu_temp_t grp
  //const snowhousecpu_opc_info_t *opc_info)
  (snowhousecpu_dasm_info_t *args)
{
  const size_t temp_length = (
    args->length
    //- args->have_index_ra_rb * 2ull
    //- args->have_index_ra_simm * 2ull
  );
  const snowhousecpu_temp_t temp_iword = (
    //args->have_index_ra_simm 
    //? (args->iword >> 32ull)
    //:
    args->iword
  );
  if (temp_length == 4)
  {
    cbuf[0] = '\0';
  }
  else if (temp_length == 8)
  {
    snprintf (cbuf, SNOWHOUSECPU_DIS_CBUF_LIM,
      " // pre #0x%x",
      //(unsigned) GET_INSN_FIELD (SNOWHOUSECPU_PRE_S12_MASK,
      //  SNOWHOUSECPU_PRE_S12_BITPOS, iword)
      //(unsigned) snowhousecpu_get_insn_field_ei
      //  (&snowhousecpu_enc_info_g0_pre_s12,
      //  temp_iword >> SNOWHOUSECPU_ONE_EXT_BITPOS)
      (unsigned) snowhousecpu_get_insn_field(
	SNOWHOUSECPU_IMM16_MASK, SNOWHOUSECPU_IMM16_BITPOS,
	temp_iword >> SNOWHOUSECPU_ONE_EXT_BITPOS
      )
    );
  }
  //else if (temp_length == 6)
  //{
  //  const snowhousecpu_grp_info_t
  //    *grp_info = args->opc_info->grp_info;
  //  if (grp_info->grp_value != SNOWHOUSECPU_G3_GRP_VALUE)
  //    /* instructions with simm5/uimm5 */
  //  {
  //    if (grp_info->grp_value == SNOWHOUSECPU_G7_GRP_VALUE
  //      && grp_info->subgrp != &snowhousecpu_enc_info_g7_icreload_subgrp)
  //    {
  //      snprintf (cbuf, SNOWHOUSECPU_DIS_CBUF_LIM,
  //        " // lpre g7 bad `subgrp`");
  //    }
  //    else
  //    {
  //      snprintf (cbuf, SNOWHOUSECPU_DIS_CBUF_LIM,
  //        " // lpre #0x%x",
  //        //(unsigned) GET_INSN_FIELD (SNOWHOUSECPU_G0_LPRE_S27_MASK,
  //        //  SNOWHOUSECPU_G0_LPRE_S27_BITPOS, iword)
  //        (unsigned) snowhousecpu_get_insn_field_ei
  //          (&snowhousecpu_enc_info_g0_lpre_s27,
  //          temp_iword >> SNOWHOUSECPU_ONE_EXT_BITPOS)
  //      );
  //    }
  //  }
  //  else /* if (grp_info->grp_value == SNOWHOUSECPU_G3_GRP_VALUE) */
  //  {
  //    snprintf (cbuf, SNOWHOUSECPU_DIS_CBUF_LIM,
  //      " // lpre #0x%x",
  //      //(unsigned) GET_INSN_FIELD (SNOWHOUSECPU_G0_LPRE_S23_MASK,
  //      //  SNOWHOUSECPU_G0_LPRE_S23_BITPOS, iword)
  //      (unsigned) snowhousecpu_get_insn_field_ei
  //        (&snowhousecpu_enc_info_g0_lpre_s23,
  //        temp_iword >> SNOWHOUSECPU_ONE_EXT_BITPOS)
  //    );
  //  }
  //}
  return cbuf;
}

//void
//do_print_insn_snowhousecpu (snowhousecpu_dasm_info_t *args);
static void
do_print_insn_snowhousecpu (snowhousecpu_dasm_info_t *args)
{
  switch (args->opc_info->oparg)
  {
    /* -------- */
    case SNOWHOUSECPU_OA_NONE:
    {
      if (args->length != 4)
      {
        fpr (stream, "%s%x%s",
          "bad (op 0x",
          (unsigned) args->op,
          "; `SNOWHOUSECPU_OA_RA_RB`; `pre`)");
      }
      else
      {
	fpr (stream, "%s",
	  args->opc_info->name);
      }
    }
      break;
    //case SNOWHOUSECPU_OA_PRE:
    //{
    //}
    //  break;
    case SNOWHOUSECPU_OA_RA_RB:
    {
      if (args->length != 4)
      {
        fpr (stream, "%s%x%s",
          "bad (op 0x",
          (unsigned) args->op,
          "; `SNOWHOUSECPU_OA_RA_RB`; `pre`)");
      }
      else
      {
	fpr (stream, "%s\t%s, %s",
	  args->opc_info->name,
	  gprs[args->ra_idx].name,
	  gprs[args->rb_idx].name
	);
      }
    }
      break;
    case SNOWHOUSECPU_OA_RA_RB_RC:
    {
      if (args->length != 4)
      {
        fpr (stream, "%s%x%s",
          "bad (op 0x",
          (unsigned) args->op,
          "; `SNOWHOUSECPU_OA_RA_RB_RC`; `pre`)");
      }
      else
      {
	fpr (stream, "%s\t%s, %s, %s",
	  args->opc_info->name,
	  gprs[args->ra_idx].name,
	  gprs[args->rb_idx].name,
	  gprs[args->rc_idx].name
	);
      }
    }
      break;
    case SNOWHOUSECPU_OA_RA_RB_S16:
    {
      fpr (stream, "%s\t%s, %s, %i%s",
	args->opc_info->name,
	gprs[args->ra_idx].name,
	gprs[args->rb_idx].name,
	(signed) args->simm,
	do_snprintf_insn_snowhousecpu_maybe_pre (args));
    }
      break;
    case SNOWHOUSECPU_OA_RA_RB_SHIFT_U5:
    {
      if (args->length != 4)
      {
        fpr (stream, "%s%x%s",
          "bad (op 0x",
          (unsigned) args->op,
          "; `SNOWHOUSECPU_OA_RA_RB_SHIFT_U5`; `pre`)");
      }
      else
      {
	fpr (stream, "%s\t%s, %s, %u",
	  args->opc_info->name,
	  gprs[args->ra_idx].name,
	  gprs[args->rb_idx].name,
	  (unsigned) args->simm);
      }
    }
      break;
    case SNOWHOUSECPU_OA_RA_PCREL_S16:
    {
      fpr (stream, "%s\t%s, %i%s",
	args->opc_info->name,
	gprs[args->ra_idx].name,
	(signed) args->simm,
	do_snprintf_insn_snowhousecpu_maybe_pre (args));
    }
      break;
    case SNOWHOUSECPU_OA_RA_RB_PCREL_S16:
    {
      fpr (stream, "%s\t%s, %s, %i%s",
	args->opc_info->name,
	gprs[args->ra_idx].name,
	gprs[args->rb_idx].name,
	(signed) args->simm,
	do_snprintf_insn_snowhousecpu_maybe_pre (args));
    }
      break;
    case SNOWHOUSECPU_OA_PCREL_S16_IMPLICIT_LR:
    {
      fpr (stream, "%s\t%i%s",
	args->opc_info->name,
	//gprs[args->ra_idx].name,
	//gprs[args->rb_idx].name,
	(signed) args->simm,
	do_snprintf_insn_snowhousecpu_maybe_pre (args));
    }
      break;
    case SNOWHOUSECPU_OA_RA_PC_PCREL_S16:
    {
      fpr (stream, "%s\t%s, %i%s",
	args->opc_info->name,
	gprs[args->ra_idx].name,
	(signed) args->simm,
	do_snprintf_insn_snowhousecpu_maybe_pre (args));
    }
      break;
    case SNOWHOUSECPU_OA_IDS_RB:
    {
      if (args->length != 4)
      {
        fpr (stream, "%s%x%s",
          "bad (op 0x",
          (unsigned) args->op,
          "; `SNOWHOUSECPU_OA_IDS_RB`; `pre`)");
      }
      else
      {
	fpr (stream, "%s\t%s, %s",
	  args->opc_info->name,
	  gprs[SNOWHOUSECPU_SPR_ENUM_IDS].name,
	  gprs[args->rb_idx].name);
      }
    }
      break;
    case SNOWHOUSECPU_OA_RA_IRA:
    {
      if (args->length != 4)
      {
        fpr (stream, "%s%x%s",
          "bad (op 0x",
          (unsigned) args->op,
          "; `SNOWHOUSECPU_OA_RA_IRA`; `pre`)");
      }
      else
      {
	fpr (stream, "%s\t%s, %s",
	  args->opc_info->name,
	  gprs[args->ra_idx].name,
	  gprs[SNOWHOUSECPU_SPR_ENUM_IRA].name);
      }
    }
      break;
    case SNOWHOUSECPU_OA_IE_RB:
    {
      if (args->length != 4)
      {
        fpr (stream, "%s%x%s",
          "bad (op 0x",
          (unsigned) args->op,
          "; `SNOWHOUSECPU_OA_IE_RB`; `pre`)");
      }
      else
      {
	fpr (stream, "%s\t%s, %s",
	  args->opc_info->name,
	  gprs[SNOWHOUSECPU_SPR_ENUM_IE].name,
	  gprs[args->rb_idx].name);
      }
    }
      break;
    case SNOWHOUSECPU_OA_IRA:
    {
      if (args->length != 4)
      {
        fpr (stream, "%s%x%s",
          "bad (op 0x",
          (unsigned) args->op,
          "; `SNOWHOUSECPU_OA_IRA`; `pre`)");
      }
      else
      {
	fpr (stream, "%s\t%s",
	  args->opc_info->name,
	  gprs[SNOWHOUSECPU_SPR_ENUM_IRA].name);
      }
    }
      break;
    default:
      //fpr (stream, "bad (grp 0x%x; oparg 0x%x; opcode 0x%x; name %s)",
      //  (unsigned) args->grp, (unsigned) args->opc_info->oparg,
      //  (unsigned) args->opc_info->opcode,
      //  args->opc_info->names[args->fw]);

      fpr (stream, "%s%x%s%s%s",
	"bad (op 0x",
	(unsigned) args->op,
	"; ",
	args->opc_info->name,
	")"
      );
      break;
    /* -------- */
  }
}

static bfd_vma loc_addr = 0;
static struct disassemble_info *loc_info = NULL;

static int
print_insn_snowhousecpu_rd32 (snowhousecpu_dasm_info_t *dasm_info)
{
  return loc_info->read_memory_func
    (loc_addr + dasm_info->length, dasm_info->buffer, 4, loc_info);
}

int
print_insn_snowhousecpu (bfd_vma addr, struct disassemble_info *info)
{
  stream = info->stream;
  fpr = info->fprintf_func;

  //int
  //  length = 0,
  //  status;
  //bfd_byte buffer[2];
  //const snowhousecpu_opc_info_t *opc_info;
  //snowhousecpu_temp_t
  //  iword,
  //  simm = 0,
  //  grp,
  //  ra_ind,
  //  rb_ind,
  //  fw = 0;

  loc_addr = addr;
  loc_info = info;

  snowhousecpu_dasm_info_t dasm_info;
  snowhousecpu_dasm_info_ctor (&dasm_info, &print_insn_snowhousecpu_rd32);
  snowhousecpu_dasm_info_do_disassemble (&dasm_info);

  if (dasm_info.status)
  {
    //goto fail;
    info->memory_error_func (dasm_info.status, addr, info);
    return -1;
  }
  else if (dasm_info.is_bad)
  {
    fpr (stream, "bad");
  }
  //else if (dasm_info.g7_subgrp_decode_err)
  //{
  //  fpr (stream, "bad (grp 0x%x; unknown subgrp "
  //    "(checked: 0x%x, 0x%x, 0x%x))",
  //    (unsigned) dasm_info.grp,
  //    (unsigned) dasm_info.g7_aluopbh_subgrp,
  //    (unsigned) dasm_info.g7_sprldst_subgrp,
  //    (unsigned) dasm_info.g7_icreload_subgrp);
  //}
  else
  {
    do_print_insn_snowhousecpu (&dasm_info);
  }

  return dasm_info.length; //+ dasm_info.have_index * 2;
}

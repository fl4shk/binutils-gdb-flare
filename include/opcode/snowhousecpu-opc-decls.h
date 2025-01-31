/* snowhousecpu-opc-decls.h -- Definitions for snowhousecpu opcodes.
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

#ifndef _SNOWHOUSECPU_OPC_DECLS_H_
#define _SNOWHOUSECPU_OPC_DECLS_H_

//#ifdef SNOWHOUSECPU_CLANGD
#include "snowhousecpu.h"
//#endif

static const snowhousecpu_opc_info_t
  snowhousecpu_opc_info_add_ra_rb_rc = SNOWHOUSECPU_OI_INST_ADD_RA_RB_RC,
  snowhousecpu_opc_info_add_ra_rb_simm16 = SNOWHOUSECPU_OI_INST_ADD_RA_RB_SIMM16,
  snowhousecpu_opc_info_cpy_ra_rb = SNOWHOUSECPU_OI_INST_CPY_RA_RB,
  snowhousecpu_opc_info_cpy_ra_simm16 = SNOWHOUSECPU_OI_INST_CPY_RA_SIMM16,
  snowhousecpu_opc_info_sub_ra_rb_rc = SNOWHOUSECPU_OI_INST_SUB_RA_RB_RC,
  snowhousecpu_opc_info_sltu_ra_rb_rc = SNOWHOUSECPU_OI_INST_SLTU_RA_RB_RC,
  snowhousecpu_opc_info_slts_ra_rb_rc = SNOWHOUSECPU_OI_INST_SLTS_RA_RB_RC,
  snowhousecpu_opc_info_xor_ra_rb_rc = SNOWHOUSECPU_OI_INST_XOR_RA_RB_RC,
  snowhousecpu_opc_info_xor_ra_rb_simm16 = SNOWHOUSECPU_OI_INST_XOR_RA_RB_SIMM16,
  snowhousecpu_opc_info_or_ra_rb_rc = SNOWHOUSECPU_OI_INST_OR_RA_RB_RC,
  snowhousecpu_opc_info_or_ra_rb_simm16 = SNOWHOUSECPU_OI_INST_OR_RA_RB_SIMM16,

  snowhousecpu_opc_info_and_ra_rb_simm16 = SNOWHOUSECPU_OI_INST_AND_RA_RB_SIMM16,
  snowhousecpu_opc_info_lsl_ra_rb_rc = SNOWHOUSECPU_OI_INST_LSL_RA_RB_RC,
  snowhousecpu_opc_info_lsl_ra_rb_imm5 = SNOWHOUSECPU_OI_INST_LSL_RA_RB_IMM5,
  snowhousecpu_opc_info_lsr_ra_rb_rc = SNOWHOUSECPU_OI_INST_LSR_RA_RB_RC,
  snowhousecpu_opc_info_lsr_ra_rb_imm5 = SNOWHOUSECPU_OI_INST_LSR_RA_RB_IMM5,
  snowhousecpu_opc_info_asr_ra_rb_rc = SNOWHOUSECPU_OI_INST_ASR_RA_RB_RC,
  snowhousecpu_opc_info_asr_ra_rb_imm5 = SNOWHOUSECPU_OI_INST_ASR_RA_RB_IMM5,
  snowhousecpu_opc_info_and_ra_rb_rc = SNOWHOUSECPU_OI_INST_AND_RA_RB_RC,
  snowhousecpu_opc_info_cpy_ids_rb = SNOWHOUSECPU_OI_INST_CPY_IDS_RB,
  snowhousecpu_opc_info_cpy_ra_ira = SNOWHOUSECPU_OI_INST_CPY_RA_IRA,
  snowhousecpu_opc_info_cpy_ie_rb = SNOWHOUSECPU_OI_INST_CPY_IE_RB,
  snowhousecpu_opc_info_ret_ira = SNOWHOUSECPU_OI_INST_RET_IRA,
  snowhousecpu_opc_info_mul_ra_rb_rc = SNOWHOUSECPU_OI_INST_MUL_RA_RB_RC,
  snowhousecpu_opc_info_udiv_ra_rb_rc = SNOWHOUSECPU_OI_INST_UDIV_RA_RB_RC,
  snowhousecpu_opc_info_sdiv_ra_rb_rc = SNOWHOUSECPU_OI_INST_SDIV_RA_RB_RC,
  snowhousecpu_opc_info_umod_ra_rb_rc = SNOWHOUSECPU_OI_INST_UMOD_RA_RB_RC,
  snowhousecpu_opc_info_smod_ra_rb_rc = SNOWHOUSECPU_OI_INST_SMOD_RA_RB_RC,
  snowhousecpu_opc_info_ldr_ra_rb_simm16 = SNOWHOUSECPU_OI_INST_LDR_RA_RB_SIMM16,
  snowhousecpu_opc_info_str_ra_rb_simm16 = SNOWHOUSECPU_OI_INST_STR_RA_RB_SIMM16,
  snowhousecpu_opc_info_lduh_ra_rb_simm16 = SNOWHOUSECPU_OI_INST_LDUH_RA_RB_SIMM16,
  snowhousecpu_opc_info_ldsh_ra_rb_simm16 = SNOWHOUSECPU_OI_INST_LDSH_RA_RB_SIMM16,
  snowhousecpu_opc_info_ldub_ra_rb_simm16 = SNOWHOUSECPU_OI_INST_LDUB_RA_RB_SIMM16,
  snowhousecpu_opc_info_ldsb_ra_rb_simm16 = SNOWHOUSECPU_OI_INST_LDSB_RA_RB_SIMM16,
  snowhousecpu_opc_info_sth_ra_rb_simm16 = SNOWHOUSECPU_OI_INST_STH_RA_RB_SIMM16,
  snowhousecpu_opc_info_stb_ra_rb_simm16 = SNOWHOUSECPU_OI_INST_STB_RA_RB_SIMM16,
  snowhousecpu_opc_info_beq_ra_rb_simm16 = SNOWHOUSECPU_OI_INST_BEQ_RA_RB_SIMM16,
  snowhousecpu_opc_info_bl_ra_simm16 = SNOWHOUSECPU_OI_INST_BL_RA_SIMM16,
  snowhousecpu_opc_info_bl_simm16 = SNOWHOUSECPU_OI_INST_BL_SIMM16,
  snowhousecpu_opc_info_bne_ra_rb_simm16 = SNOWHOUSECPU_OI_INST_BNE_RA_RB_SIMM16,
  snowhousecpu_opc_info_add_ra_pc_simm16 = SNOWHOUSECPU_OI_INST_ADD_RA_PC_SIMM16,
  snowhousecpu_opc_info_bltu_ra_rb_simm16 = SNOWHOUSECPU_OI_INST_BLTU_RA_RB_SIMM16,
  snowhousecpu_opc_info_bgtu_ra_rb_simm16 = SNOWHOUSECPU_OI_INST_BGTU_RA_RB_SIMM16,
  snowhousecpu_opc_info_bgeu_ra_rb_simm16 = SNOWHOUSECPU_OI_INST_BGEU_RA_RB_SIMM16,
  snowhousecpu_opc_info_bleu_ra_rb_simm16 = SNOWHOUSECPU_OI_INST_BLEU_RA_RB_SIMM16,
  snowhousecpu_opc_info_blts_ra_rb_simm16 = SNOWHOUSECPU_OI_INST_BLTS_RA_RB_SIMM16,
  snowhousecpu_opc_info_bgts_ra_rb_simm16 = SNOWHOUSECPU_OI_INST_BGTS_RA_RB_SIMM16,
  snowhousecpu_opc_info_bges_ra_rb_simm16 = SNOWHOUSECPU_OI_INST_BGES_RA_RB_SIMM16,
  snowhousecpu_opc_info_bles_ra_rb_simm16 = SNOWHOUSECPU_OI_INST_BLES_RA_RB_SIMM16,
  snowhousecpu_opc_info_jl_ra_rb = SNOWHOUSECPU_OI_INST_JL_RA_RB,
  snowhousecpu_opc_info_jmp_rb = SNOWHOUSECPU_OI_INST_JMP_RB,
  snowhousecpu_opc_info_pre_simm16 = SNOWHOUSECPU_OI_INST_PRE_SIMM16;

#define SNOWHOUSECPU_OPC_ARRAY_SIZE(arr) \
  ((sizeof (arr) / sizeof (arr[0])))
//static inline unsigned
//snowhousecpu_opc_info_arr_size (const snowhousecpu_opc_info_t **arr)
//{
//  for (size_t i=0; i<512u; ++i)
//  {
//    if (arr[i] == NULL)
//    {
//      return i;
//    }
//  }
//  // failsafe
//  return 0;
//}
//static inline unsigned
//snowhousecpu_opc_info_a2d_size (const snowhousecpu_opc_info_t ***arr)
//{
//  for (size_t i=0; i<512u; ++i)
//  {
//    if (arr[i] == NULL)
//    {
//      return i;
//    }
//  }
//  // failsafe
//  return 0;
//}
static const snowhousecpu_opc_info_t *
snowhousecpu_opc_info_add_arr[] =
{
  &snowhousecpu_opc_info_add_ra_rb_rc,
  &snowhousecpu_opc_info_add_ra_rb_simm16,
  &snowhousecpu_opc_info_cpy_ra_rb,
  &snowhousecpu_opc_info_cpy_ra_simm16,
  //NULL,
};
//#define snowhousecpu_opc_info_add_arr_size
//  (snowhousecpu_opc_info_arr_size (snowhousecpu_opc_info_add_arr))
static const size_t snowhousecpu_opc_info_add_arr_size = (
  SNOWHOUSECPU_OPC_ARRAY_SIZE (snowhousecpu_opc_info_add_arr)
);

static const snowhousecpu_opc_info_t *
snowhousecpu_opc_info_sub_arr[] =
{
  &snowhousecpu_opc_info_sub_ra_rb_rc,
  //NULL,
};
//#define snowhousecpu_opc_info_sub_arr_size
//  (snowhousecpu_opc_info_arr_size (snowhousecpu_opc_info_sub_arr))
static const size_t snowhousecpu_opc_info_sub_arr_size = (
  SNOWHOUSECPU_OPC_ARRAY_SIZE (snowhousecpu_opc_info_sub_arr)
);
static const snowhousecpu_opc_info_t *
snowhousecpu_opc_info_slt_arr[] =
{
  &snowhousecpu_opc_info_sltu_ra_rb_rc,
  &snowhousecpu_opc_info_slts_ra_rb_rc,
  //NULL,
};
static const size_t snowhousecpu_opc_info_slt_arr_size = (
  SNOWHOUSECPU_OPC_ARRAY_SIZE (snowhousecpu_opc_info_slt_arr)
);
//#define snowhousecpu_opc_info_slt_arr_size
//  (snowhousecpu_opc_info_arr_size (snowhousecpu_opc_info_slt_arr))
static const snowhousecpu_opc_info_t *
snowhousecpu_opc_info_xor_arr[] =
{
  &snowhousecpu_opc_info_xor_ra_rb_rc,
  &snowhousecpu_opc_info_xor_ra_rb_simm16,
  //NULL,
};
static const size_t snowhousecpu_opc_info_xor_arr_size = (
  SNOWHOUSECPU_OPC_ARRAY_SIZE (snowhousecpu_opc_info_xor_arr)
);
//#define snowhousecpu_opc_info_xor_arr_size
//  (snowhousecpu_opc_info_arr_size (snowhousecpu_opc_info_xor_arr))
static const snowhousecpu_opc_info_t *
snowhousecpu_opc_info_or_arr[] =
{
  &snowhousecpu_opc_info_or_ra_rb_rc,
  &snowhousecpu_opc_info_or_ra_rb_simm16,
  //NULL,
};
static const size_t snowhousecpu_opc_info_or_arr_size = (
  SNOWHOUSECPU_OPC_ARRAY_SIZE (snowhousecpu_opc_info_or_arr)
);
//#define snowhousecpu_opc_info_xor_arr_size
//  (snowhousecpu_opc_info_arr_size (snowhousecpu_opc_info_xor_arr))
static const snowhousecpu_opc_info_t *
snowhousecpu_opc_info_and_simm16_arr[] =
{
  &snowhousecpu_opc_info_and_ra_rb_simm16,
  //NULL,
};
static const size_t snowhousecpu_opc_info_and_simm16_arr_size = (
  SNOWHOUSECPU_OPC_ARRAY_SIZE (snowhousecpu_opc_info_and_simm16_arr)
);
//#define snowhousecpu_opc_info_and_simm16_arr_size
//  (snowhousecpu_opc_info_arr_size (snowhousecpu_opc_info_and_simm16_arr))
static const snowhousecpu_opc_info_t *
snowhousecpu_opc_info_shift_etc_arr[] =
{
  &snowhousecpu_opc_info_lsl_ra_rb_rc,
  &snowhousecpu_opc_info_lsl_ra_rb_imm5,
  &snowhousecpu_opc_info_lsr_ra_rb_rc,
  &snowhousecpu_opc_info_lsr_ra_rb_imm5,
  &snowhousecpu_opc_info_asr_ra_rb_rc,
  &snowhousecpu_opc_info_asr_ra_rb_imm5,
  &snowhousecpu_opc_info_and_ra_rb_rc,
  &snowhousecpu_opc_info_cpy_ids_rb,
  &snowhousecpu_opc_info_cpy_ra_ira,
  &snowhousecpu_opc_info_cpy_ie_rb,
  &snowhousecpu_opc_info_ret_ira,
  //NULL,
};
//#define snowhousecpu_opc_info_shift_etc_arr_size
//  (snowhousecpu_opc_info_arr_size (snowhousecpu_opc_info_shift_etc_arr))
static const size_t snowhousecpu_opc_info_shift_etc_arr_size = (
  SNOWHOUSECPU_OPC_ARRAY_SIZE (snowhousecpu_opc_info_shift_etc_arr)
);

static const snowhousecpu_opc_info_t *
snowhousecpu_opc_info_multi_cycle_arr[] =
{
  &snowhousecpu_opc_info_mul_ra_rb_rc,
  &snowhousecpu_opc_info_udiv_ra_rb_rc,
  &snowhousecpu_opc_info_sdiv_ra_rb_rc,
  &snowhousecpu_opc_info_umod_ra_rb_rc,
  &snowhousecpu_opc_info_smod_ra_rb_rc,
  //NULL,
};
//#define snowhousecpu_opc_info_multi_cycle_arr_size
//  (snowhousecpu_opc_info_arr_size (snowhousecpu_opc_info_multi_cycle_arr))
static const size_t snowhousecpu_opc_info_multi_cycle_arr_size = (
  SNOWHOUSECPU_OPC_ARRAY_SIZE (snowhousecpu_opc_info_multi_cycle_arr)
);

static const snowhousecpu_opc_info_t *
snowhousecpu_opc_info_ldst_arr[] =
{
  &snowhousecpu_opc_info_ldr_ra_rb_simm16,
  &snowhousecpu_opc_info_str_ra_rb_simm16,
  &snowhousecpu_opc_info_lduh_ra_rb_simm16,
  &snowhousecpu_opc_info_ldsh_ra_rb_simm16,
  &snowhousecpu_opc_info_ldub_ra_rb_simm16,
  &snowhousecpu_opc_info_ldsb_ra_rb_simm16,
  &snowhousecpu_opc_info_sth_ra_rb_simm16,
  &snowhousecpu_opc_info_stb_ra_rb_simm16,
  //NULL,
};
//#define snowhousecpu_opc_info_ldst_arr_size
//  (snowhousecpu_opc_info_arr_size (snowhousecpu_opc_info_ldst_arr))
static const size_t snowhousecpu_opc_info_ldst_arr_size = (
  SNOWHOUSECPU_OPC_ARRAY_SIZE (snowhousecpu_opc_info_ldst_arr)
);
static const snowhousecpu_opc_info_t *
snowhousecpu_opc_info_branch_etc_arr[] =
{
  &snowhousecpu_opc_info_beq_ra_rb_simm16,
  &snowhousecpu_opc_info_bl_ra_simm16,
  &snowhousecpu_opc_info_bl_simm16,
  &snowhousecpu_opc_info_bne_ra_rb_simm16,
  &snowhousecpu_opc_info_add_ra_pc_simm16,
  &snowhousecpu_opc_info_bltu_ra_rb_simm16,
  &snowhousecpu_opc_info_bgtu_ra_rb_simm16,
  &snowhousecpu_opc_info_bgeu_ra_rb_simm16,
  &snowhousecpu_opc_info_bleu_ra_rb_simm16,
  &snowhousecpu_opc_info_blts_ra_rb_simm16,
  &snowhousecpu_opc_info_bgts_ra_rb_simm16,
  &snowhousecpu_opc_info_bges_ra_rb_simm16,
  &snowhousecpu_opc_info_bles_ra_rb_simm16,
  &snowhousecpu_opc_info_jl_ra_rb,
  &snowhousecpu_opc_info_jmp_rb,
  //NULL,
};
//#define snowhousecpu_opc_info_branch_etc_arr_size
//  (snowhousecpu_opc_info_arr_size (snowhousecpu_opc_info_branch_etc_arr))
static const size_t snowhousecpu_opc_info_branch_etc_arr_size = (
  SNOWHOUSECPU_OPC_ARRAY_SIZE (snowhousecpu_opc_info_branch_etc_arr)
);
static const snowhousecpu_opc_info_t *
snowhousecpu_opc_info_pre_arr[] =
{
  &snowhousecpu_opc_info_pre_simm16,
  //NULL,
};
//#define snowhousecpu_opc_info_pre_arr_size
//  (snowhousecpu_opc_info_arr_size (snowhousecpu_opc_info_pre_arr))
static const size_t snowhousecpu_opc_info_pre_arr_size = (
  SNOWHOUSECPU_OPC_ARRAY_SIZE (snowhousecpu_opc_info_pre_arr)
);

// indexed by the opcode!
static const snowhousecpu_opc_info_t **
snowhousecpu_opc_info_a2d[] =
{
  snowhousecpu_opc_info_add_arr,
  snowhousecpu_opc_info_sub_arr,
  snowhousecpu_opc_info_slt_arr,
  snowhousecpu_opc_info_xor_arr,
  snowhousecpu_opc_info_or_arr,
  snowhousecpu_opc_info_and_simm16_arr,
  snowhousecpu_opc_info_shift_etc_arr,
  snowhousecpu_opc_info_multi_cycle_arr,
  snowhousecpu_opc_info_ldst_arr,
  snowhousecpu_opc_info_branch_etc_arr,
  snowhousecpu_opc_info_pre_arr,
  //NULL,
};
static const size_t
snowhousecpu_opc_info_size_arr[] =
{
  snowhousecpu_opc_info_add_arr_size,
  snowhousecpu_opc_info_sub_arr_size,
  snowhousecpu_opc_info_slt_arr_size,
  snowhousecpu_opc_info_xor_arr_size,
  snowhousecpu_opc_info_or_arr_size,
  snowhousecpu_opc_info_and_simm16_arr_size,
  snowhousecpu_opc_info_shift_etc_arr_size,
  snowhousecpu_opc_info_multi_cycle_arr_size,
  snowhousecpu_opc_info_ldst_arr_size,
  snowhousecpu_opc_info_branch_etc_arr_size,
  snowhousecpu_opc_info_pre_arr_size,
};

//#define snowhousecpu_opc_info_a2d_size
//  (snowhousecpu_opc_info_a2d_size (snowhousecpu_opc_info_a2d))
static const size_t snowhousecpu_opc_info_a2d_size = (
  SNOWHOUSECPU_OPC_ARRAY_SIZE (snowhousecpu_opc_info_a2d)
);

//static const size_t
//snowhousecpu_opc_info_a2d_size = (
//  sizeof(snowhousecpu_opc_info_a2d) / sizeof(const snowhousecpu_opc_info_t **)
//);

//static const snowhousecpu_opc_info_t *
//snowhousecpu_opc_info_arr[] =
//{
//  &snowhousecpu_opc_info_add_ra_rb_rc,
//  &snowhousecpu_opc_info_add_ra_rb_simm16,
//  &snowhousecpu_opc_info_sub_ra_rb_rc,
//  &snowhousecpu_opc_info_sltu_ra_rb_rc,
//  &snowhousecpu_opc_info_slts_ra_rb_rc,
//  &snowhousecpu_opc_info_xor_ra_rb_rc,
//  &snowhousecpu_opc_info_xor_ra_rb_simm16,
//  &snowhousecpu_opc_info_or_ra_rb_rc,
//  &snowhousecpu_opc_info_or_ra_rb_simm16,
//  &snowhousecpu_opc_info_and_ra_rb_simm16,
//  &snowhousecpu_opc_info_lsl_ra_rb_rc,
//  &snowhousecpu_opc_info_lsl_ra_rb_imm5,
//  &snowhousecpu_opc_info_lsr_ra_rb_rc,
//  &snowhousecpu_opc_info_lsr_ra_rb_imm5,
//  &snowhousecpu_opc_info_asr_ra_rb_rc,
//  &snowhousecpu_opc_info_asr_ra_rb_imm5,
//  &snowhousecpu_opc_info_and_ra_rb_rc,
//  &snowhousecpu_opc_info_cpy_ids_rb,
//  &snowhousecpu_opc_info_cpy_ra_ira,
//  &snowhousecpu_opc_info_cpy_ie_rb,
//  &snowhousecpu_opc_info_ret_ira,
//  &snowhousecpu_opc_info_mul_ra_rb_rc,
//  &snowhousecpu_opc_info_udiv_ra_rb_rc,
//  &snowhousecpu_opc_info_sdiv_ra_rb_rc,
//  &snowhousecpu_opc_info_umod_ra_rb_rc,
//  &snowhousecpu_opc_info_smod_ra_rb_rc,
//  &snowhousecpu_opc_info_ldr_ra_rb_simm16,
//  &snowhousecpu_opc_info_str_ra_rb_simm16,
//  &snowhousecpu_opc_info_lduh_ra_rb_simm16,
//  &snowhousecpu_opc_info_ldsh_ra_rb_simm16,
//  &snowhousecpu_opc_info_ldub_ra_rb_simm16,
//  &snowhousecpu_opc_info_ldsb_ra_rb_simm16,
//  &snowhousecpu_opc_info_sth_ra_rb_simm16,
//  &snowhousecpu_opc_info_stb_ra_rb_simm16,
//  &snowhousecpu_opc_info_beq_ra_rb_simm16,
//  &snowhousecpu_opc_info_bl_ra_simm16,
//  &snowhousecpu_opc_info_bl_simm16,
//  &snowhousecpu_opc_info_bne_ra_rb_simm16,
//  &snowhousecpu_opc_info_add_ra_pc_simm16,
//  &snowhousecpu_opc_info_bltu_ra_rb_simm16,
//  &snowhousecpu_opc_info_bgeu_ra_rb_simm16,
//  &snowhousecpu_opc_info_blts_ra_rb_simm16,
//  &snowhousecpu_opc_info_bges_ra_rb_simm16,
//  &snowhousecpu_opc_info_jl_ra_rb,
//  &snowhousecpu_opc_info_jmp_rb,
//  &snowhousecpu_opc_info_pre_simm16,
//};
//static const size_t
//snowhousecpu_opc_info_arr_size = (
//  sizeof(snowhousecpu_opc_info_arr) / sizeof(const snowhousecpu_opc_info_t *)
//);

#endif    /* _SNOWHOUSECPU_OPC_DECLS_H_ */

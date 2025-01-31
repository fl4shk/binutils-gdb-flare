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
  snowhousecpu_opc_info_bgeu_ra_rb_simm16 = SNOWHOUSECPU_OI_INST_BGEU_RA_RB_SIMM16,
  snowhousecpu_opc_info_blts_ra_rb_simm16 = SNOWHOUSECPU_OI_INST_BLTS_RA_RB_SIMM16,
  snowhousecpu_opc_info_bges_ra_rb_simm16 = SNOWHOUSECPU_OI_INST_BGES_RA_RB_SIMM16,
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
  &snowhousecpu_opc_info_bgeu_ra_rb_simm16,
  &snowhousecpu_opc_info_blts_ra_rb_simm16,
  &snowhousecpu_opc_info_bges_ra_rb_simm16,
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

//static const snowhousecpu_enc_info_t
//snowhousecpu_enc_info_grp_32 =
//  {SNOWHOUSECPU_GRP_BITSIZE,
//  SNOWHOUSECPU_GRP_32_BITPOS,
//  SNOWHOUSECPU_GRP_RSMASK,
//  SNOWHOUSECPU_GRP_16_MASK};

////static inline snowhousecpu_enc_info_t
////snowhousecpu_build_enc_info (snowhousecpu_temp_t bitsize, snowhousecpu_temp_t bitpos)
////{
////  snowhousecpu_enc_info_t ret;
////
////  ret.bitsize = bitsize;
////  ret.bitpos = bitpos;
////  ret.rsmask = SNOWHOUSECPU_N_ONES (ret.bitsize);
////  ret.mask = ret.rsmask << ret.bitpos;
////
////  return ret;
////}
//static const snowhousecpu_enc_info_t
//  /* -------- */
//  snowhousecpu_enc_info_grp_16 =
//    {SNOWHOUSECPU_GRP_BITSIZE,
//    SNOWHOUSECPU_GRP_16_BITPOS,
//    SNOWHOUSECPU_GRP_RSMASK,
//    SNOWHOUSECPU_GRP_16_MASK},
//  snowhousecpu_enc_info_grp_32 = 
//    {SNOWHOUSECPU_GRP_BITSIZE,
//    SNOWHOUSECPU_GRP_32_BITPOS,
//    SNOWHOUSECPU_GRP_RSMASK,
//    SNOWHOUSECPU_GRP_32_MASK},
//  /* -------- */
//  snowhousecpu_enc_info_g0_pre_subgrp =
//    {SNOWHOUSECPU_G0_PRE_SUBGRP_BITSIZE, 
//    SNOWHOUSECPU_G0_PRE_SUBGRP_BITPOS,
//    SNOWHOUSECPU_G0_PRE_SUBGRP_RSMASK,
//    SNOWHOUSECPU_G0_PRE_SUBGRP_MASK},
//
//  snowhousecpu_enc_info_g0_pre_fullgrp =
//    {SNOWHOUSECPU_G0_PRE_FULLGRP_BITSIZE,
//    SNOWHOUSECPU_G0_PRE_FULLGRP_BITPOS,
//    SNOWHOUSECPU_G0_PRE_FULLGRP_RSMASK,
//    SNOWHOUSECPU_G0_PRE_FULLGRP_MASK},
//  snowhousecpu_enc_info_g0_pre_s12 =
//    {SNOWHOUSECPU_G0_PRE_S12_BITSIZE,
//    SNOWHOUSECPU_G0_PRE_S12_BITPOS,
//    SNOWHOUSECPU_G0_PRE_S12_RSMASK,
//    SNOWHOUSECPU_G0_PRE_S12_MASK},
//  /* -------- */
//  snowhousecpu_enc_info_g0_lpre_subgrp =
//    {SNOWHOUSECPU_G0_LPRE_SUBGRP_BITSIZE,
//    SNOWHOUSECPU_G0_LPRE_SUBGRP_BITPOS,
//    SNOWHOUSECPU_G0_LPRE_SUBGRP_RSMASK,
//    SNOWHOUSECPU_G0_LPRE_SUBGRP_MASK},
//  snowhousecpu_enc_info_g0_lpre_fullgrp =
//    {SNOWHOUSECPU_G0_LPRE_FULLGRP_BITSIZE,
//    SNOWHOUSECPU_G0_LPRE_FULLGRP_BITPOS,
//    SNOWHOUSECPU_G0_LPRE_FULLGRP_RSMASK,
//    SNOWHOUSECPU_G0_LPRE_FULLGRP_MASK},
//  snowhousecpu_enc_info_g0_lpre_subgrp_16 =
//    {SNOWHOUSECPU_G0_LPRE_SUBGRP_BITSIZE,
//    SNOWHOUSECPU_G0_LPRE_SUBGRP_16_BITPOS,
//    SNOWHOUSECPU_G0_LPRE_SUBGRP_RSMASK,
//    SNOWHOUSECPU_G0_LPRE_SUBGRP_16_MASK},
//  snowhousecpu_enc_info_g0_lpre_fullgrp_16 =
//    {SNOWHOUSECPU_G0_LPRE_FULLGRP_BITSIZE,
//    SNOWHOUSECPU_G0_LPRE_FULLGRP_16_BITPOS,
//    SNOWHOUSECPU_G0_LPRE_FULLGRP_RSMASK,
//    SNOWHOUSECPU_G0_LPRE_FULLGRP_16_MASK},
//
//  snowhousecpu_enc_info_g0_lpre_s27 =
//    {SNOWHOUSECPU_G0_LPRE_S27_BITSIZE,
//    SNOWHOUSECPU_G0_LPRE_S27_BITPOS,
//    SNOWHOUSECPU_G0_LPRE_S27_RSMASK,
//    SNOWHOUSECPU_G0_LPRE_S27_MASK},
//  snowhousecpu_enc_info_g0_lpre_s25 =
//    {SNOWHOUSECPU_G0_LPRE_S25_BITSIZE,
//    SNOWHOUSECPU_G0_LPRE_S25_BITPOS,
//    SNOWHOUSECPU_G0_LPRE_S25_RSMASK,
//    SNOWHOUSECPU_G0_LPRE_S25_MASK},
//  snowhousecpu_enc_info_g0_lpre_s23 =
//    {SNOWHOUSECPU_G0_LPRE_S23_BITSIZE,
//    SNOWHOUSECPU_G0_LPRE_S23_BITPOS,
//    SNOWHOUSECPU_G0_LPRE_S23_RSMASK,
//    SNOWHOUSECPU_G0_LPRE_S23_MASK},
//  /* -------- */
//  snowhousecpu_enc_info_g0_atomic_subgrp =
//    {SNOWHOUSECPU_G0_ATOMIC_SUBGRP_BITSIZE, 
//    SNOWHOUSECPU_G0_ATOMIC_SUBGRP_BITPOS,
//    SNOWHOUSECPU_G0_ATOMIC_SUBGRP_RSMASK,
//    SNOWHOUSECPU_G0_ATOMIC_SUBGRP_MASK},
//
//  snowhousecpu_enc_info_g0_atomic_fullgrp =
//    {SNOWHOUSECPU_G0_ATOMIC_FULLGRP_BITSIZE,
//    SNOWHOUSECPU_G0_ATOMIC_FULLGRP_BITPOS,
//    SNOWHOUSECPU_G0_ATOMIC_FULLGRP_RSMASK,
//    SNOWHOUSECPU_G0_ATOMIC_FULLGRP_MASK},
//  snowhousecpu_enc_info_g0_atomic_l =
//    {SNOWHOUSECPU_G0_ATOMIC_L_BITSIZE,
//    SNOWHOUSECPU_G0_ATOMIC_L_BITPOS,
//    SNOWHOUSECPU_G0_ATOMIC_L_RSMASK,
//    SNOWHOUSECPU_G0_ATOMIC_L_MASK},
//  /* -------- */
//  snowhousecpu_enc_info_g1_i5 =
//    {SNOWHOUSECPU_G1_I5_BITSIZE,
//    SNOWHOUSECPU_G1_I5_BITPOS,
//    SNOWHOUSECPU_G1_I5_RSMASK,
//    SNOWHOUSECPU_G1_I5_MASK},
//  snowhousecpu_enc_info_g1_op =
//    {SNOWHOUSECPU_G1_OP_BITSIZE,
//    SNOWHOUSECPU_G1_OP_BITPOS,
//    SNOWHOUSECPU_G1_OP_RSMASK,
//    SNOWHOUSECPU_G1_OP_MASK},
//  snowhousecpu_enc_info_g2_f =
//    {SNOWHOUSECPU_G2_F_BITSIZE,
//    SNOWHOUSECPU_G2_F_BITPOS,
//    SNOWHOUSECPU_G2_F_RSMASK,
//    SNOWHOUSECPU_G2_F_MASK},
//  snowhousecpu_enc_info_g2_op =
//    {SNOWHOUSECPU_G2_OP_BITSIZE,
//    SNOWHOUSECPU_G2_OP_BITPOS,
//    SNOWHOUSECPU_G2_OP_RSMASK,
//    SNOWHOUSECPU_G2_OP_MASK},
//  snowhousecpu_enc_info_g3_s9 =
//    {SNOWHOUSECPU_G3_S9_BITSIZE,
//    SNOWHOUSECPU_G3_S9_BITPOS,
//    SNOWHOUSECPU_G3_S9_RSMASK,
//    SNOWHOUSECPU_G3_S9_MASK},
//  snowhousecpu_enc_info_g3_op =
//    {SNOWHOUSECPU_G3_OP_BITSIZE,
//    SNOWHOUSECPU_G3_OP_BITPOS,
//    SNOWHOUSECPU_G3_OP_RSMASK,
//    SNOWHOUSECPU_G3_OP_MASK},
//  snowhousecpu_enc_info_g4_op =
//    {SNOWHOUSECPU_G4_OP_BITSIZE,
//    SNOWHOUSECPU_G4_OP_BITPOS,
//    SNOWHOUSECPU_G4_OP_RSMASK,
//    SNOWHOUSECPU_G4_OP_MASK},
//  /* -------- */
//  snowhousecpu_enc_info_g5_index_ra_rb_subgrp =
//    {SNOWHOUSECPU_G5_INDEX_RA_RB_SUBGRP_BITSIZE, 
//    SNOWHOUSECPU_G5_INDEX_RA_RB_SUBGRP_BITPOS,
//    SNOWHOUSECPU_G5_INDEX_RA_RB_SUBGRP_RSMASK,
//    SNOWHOUSECPU_G5_INDEX_RA_RB_SUBGRP_MASK},
//
//  snowhousecpu_enc_info_g5_index_ra_rb_fullgrp =
//    {SNOWHOUSECPU_G5_INDEX_RA_RB_FULLGRP_BITSIZE,
//    SNOWHOUSECPU_G5_INDEX_RA_RB_FULLGRP_BITPOS,
//    SNOWHOUSECPU_G5_INDEX_RA_RB_FULLGRP_RSMASK,
//    SNOWHOUSECPU_G5_INDEX_RA_RB_FULLGRP_MASK},
//
//  snowhousecpu_enc_info_g5_index_ra_simm_subgrp =
//    {SNOWHOUSECPU_G5_INDEX_RA_SIMM_SUBGRP_BITSIZE, 
//    SNOWHOUSECPU_G5_INDEX_RA_SIMM_SUBGRP_BITPOS,
//    SNOWHOUSECPU_G5_INDEX_RA_SIMM_SUBGRP_RSMASK,
//    SNOWHOUSECPU_G5_INDEX_RA_SIMM_SUBGRP_MASK},
//
//  snowhousecpu_enc_info_g5_index_ra_simm_fullgrp =
//    {SNOWHOUSECPU_G5_INDEX_RA_SIMM_FULLGRP_BITSIZE,
//    SNOWHOUSECPU_G5_INDEX_RA_SIMM_FULLGRP_BITPOS,
//    SNOWHOUSECPU_G5_INDEX_RA_SIMM_FULLGRP_RSMASK,
//    SNOWHOUSECPU_G5_INDEX_RA_SIMM_FULLGRP_MASK},
//  snowhousecpu_enc_info_g5_index_ra_simm_s7 =
//    {SNOWHOUSECPU_G5_INDEX_RA_SIMM_S7_BITSIZE,
//    SNOWHOUSECPU_G5_INDEX_RA_SIMM_S7_BITPOS,
//    SNOWHOUSECPU_G5_INDEX_RA_SIMM_S7_RSMASK,
//    SNOWHOUSECPU_G5_INDEX_RA_SIMM_S7_MASK},
//  /* -------- */
//  snowhousecpu_enc_info_g7_aluopbh_subgrp =
//    {SNOWHOUSECPU_G7_ALUOPBH_SUBGRP_BITSIZE,
//    SNOWHOUSECPU_G7_ALUOPBH_SUBGRP_BITPOS,
//    SNOWHOUSECPU_G7_ALUOPBH_SUBGRP_RSMASK,
//    SNOWHOUSECPU_G7_ALUOPBH_SUBGRP_MASK},
//  snowhousecpu_enc_info_g7_aluopbh_fullgrp =
//    {SNOWHOUSECPU_G7_ALUOPBH_FULLGRP_BITSIZE,
//    SNOWHOUSECPU_G7_ALUOPBH_FULLGRP_BITPOS,
//    SNOWHOUSECPU_G7_ALUOPBH_FULLGRP_RSMASK,
//    SNOWHOUSECPU_G7_ALUOPBH_FULLGRP_MASK},
//  snowhousecpu_enc_info_g7_aluopbh_w =
//    {SNOWHOUSECPU_G7_ALUOPBH_W_BITSIZE,
//    SNOWHOUSECPU_G7_ALUOPBH_W_BITPOS,
//    SNOWHOUSECPU_G7_ALUOPBH_W_RSMASK,
//    SNOWHOUSECPU_G7_ALUOPBH_W_MASK},
//  snowhousecpu_enc_info_g7_aluopbh_op =
//    {SNOWHOUSECPU_G7_ALUOPBH_OP_BITSIZE,
//    SNOWHOUSECPU_G7_ALUOPBH_OP_BITPOS,
//    SNOWHOUSECPU_G7_ALUOPBH_OP_RSMASK,
//    SNOWHOUSECPU_G7_ALUOPBH_OP_MASK},
//
//  snowhousecpu_enc_info_g7_sprldst_subgrp =
//    {SNOWHOUSECPU_G7_SPRLDST_SUBGRP_BITSIZE,
//    SNOWHOUSECPU_G7_SPRLDST_SUBGRP_BITPOS,
//    SNOWHOUSECPU_G7_SPRLDST_SUBGRP_RSMASK,
//    SNOWHOUSECPU_G7_SPRLDST_SUBGRP_MASK},
//  snowhousecpu_enc_info_g7_sprldst_fullgrp =
//    {SNOWHOUSECPU_G7_SPRLDST_FULLGRP_BITSIZE,
//    SNOWHOUSECPU_G7_SPRLDST_FULLGRP_BITPOS,
//    SNOWHOUSECPU_G7_SPRLDST_FULLGRP_RSMASK,
//    SNOWHOUSECPU_G7_SPRLDST_FULLGRP_MASK},
//  snowhousecpu_enc_info_g7_sprldst_op =
//    {SNOWHOUSECPU_G7_SPRLDST_OP_BITSIZE,
//    SNOWHOUSECPU_G7_SPRLDST_OP_BITPOS,
//    SNOWHOUSECPU_G7_SPRLDST_OP_RSMASK,
//    SNOWHOUSECPU_G7_SPRLDST_OP_MASK},
//
//  snowhousecpu_enc_info_g7_icreload_subgrp =
//    {SNOWHOUSECPU_G7_ICRELOAD_SUBGRP_BITSIZE,
//    SNOWHOUSECPU_G7_ICRELOAD_SUBGRP_BITPOS,
//    SNOWHOUSECPU_G7_ICRELOAD_SUBGRP_RSMASK,
//    SNOWHOUSECPU_G7_ICRELOAD_SUBGRP_MASK},
//  snowhousecpu_enc_info_g7_icreload_fullgrp =
//    {SNOWHOUSECPU_G7_ICRELOAD_FULLGRP_BITSIZE,
//    SNOWHOUSECPU_G7_ICRELOAD_FULLGRP_BITPOS,
//    SNOWHOUSECPU_G7_ICRELOAD_FULLGRP_RSMASK,
//    SNOWHOUSECPU_G7_ICRELOAD_FULLGRP_MASK},
//  snowhousecpu_enc_info_g7_icreload_s5 =
//    {SNOWHOUSECPU_G7_ICRELOAD_S5_BITSIZE,
//    SNOWHOUSECPU_G7_ICRELOAD_S5_BITPOS,
//    SNOWHOUSECPU_G7_ICRELOAD_S5_RSMASK,
//    SNOWHOUSECPU_G7_ICRELOAD_S5_MASK},
//  //snowhousecpu_enc_info_g7_icflush = 
//  //  {
//  //    SNOWHOUSECPU_G7_ICF
//  //  },
//  snowhousecpu_enc_info_g7_icflush_subgrp =
//    {SNOWHOUSECPU_G7_ICFLUSH_SUBGRP_BITSIZE,
//    SNOWHOUSECPU_G7_ICFLUSH_SUBGRP_BITPOS,
//    SNOWHOUSECPU_G7_ICFLUSH_SUBGRP_RSMASK,
//    SNOWHOUSECPU_G7_ICFLUSH_SUBGRP_MASK},
//  snowhousecpu_enc_info_g7_icflush_fullgrp =
//    {SNOWHOUSECPU_G7_ICFLUSH_FULLGRP_BITSIZE,
//    SNOWHOUSECPU_G7_ICFLUSH_FULLGRP_BITPOS,
//    SNOWHOUSECPU_G7_ICFLUSH_FULLGRP_RSMASK,
//    SNOWHOUSECPU_G7_ICFLUSH_FULLGRP_MASK},
//  /* -------- */
//  snowhousecpu_enc_info_ra_ind =
//    {SNOWHOUSECPU_RA_IND_BITSIZE,
//    SNOWHOUSECPU_RA_IND_BITPOS,
//    SNOWHOUSECPU_RA_IND_RSMASK,
//    SNOWHOUSECPU_RA_IND_MASK},
//  snowhousecpu_enc_info_rb_ind =
//    {SNOWHOUSECPU_RB_IND_BITSIZE,
//    SNOWHOUSECPU_RB_IND_BITPOS,
//    SNOWHOUSECPU_RB_IND_RSMASK,
//    SNOWHOUSECPU_RB_IND_MASK};
//  /* -------- */
///* -------- */
///* -------- */
//static const snowhousecpu_grp_info_t snowhousecpu_grp_info_g0_pre =
//{
//  .grp=&snowhousecpu_enc_info_grp_16,
//  .grp_value=SNOWHOUSECPU_G0_GRP_VALUE,
//
//  .subgrp=&snowhousecpu_enc_info_g0_pre_subgrp,
//  .subgrp_value=SNOWHOUSECPU_G0_PRE_SUBGRP_VALUE,
//};
//static const snowhousecpu_grp_info_t snowhousecpu_grp_info_g0_lpre =
//{
//  .grp=&snowhousecpu_enc_info_grp_32,
//  .grp_value=SNOWHOUSECPU_G0_GRP_VALUE,
//
//  .subgrp=&snowhousecpu_enc_info_g0_lpre_subgrp,
//  .subgrp_value=SNOWHOUSECPU_G0_LPRE_SUBGRP_VALUE,
//};
//static const snowhousecpu_grp_info_t snowhousecpu_grp_info_g0_atomic = {
//  .grp=&snowhousecpu_enc_info_grp_16,
//  .grp_value=SNOWHOUSECPU_G0_GRP_VALUE,
//
//  .subgrp=&snowhousecpu_enc_info_g0_atomic_subgrp,
//  .subgrp_value=SNOWHOUSECPU_G0_ATOMIC_SUBGRP_VALUE,
//};
//static const snowhousecpu_opc_info_t
//  snowhousecpu_opc_info_g0[SNOWHOUSECPU_G0_OPC_INFO_LIM] =
//{
//  /* pre #simm */
//  {&snowhousecpu_grp_info_g0_pre,
//    SNOWHOUSECPU_OPC_INFO_NULL_OP, SNOWHOUSECPU_OA_PRE,
//    {"pre", "pre"}, {"pre.nr", "pre.nr"}},
//
//  /* lpre #simm */
//  {&snowhousecpu_grp_info_g0_lpre,
//    SNOWHOUSECPU_OPC_INFO_NULL_OP, SNOWHOUSECPU_OA_LPRE,
//    {"lpre", "lpre"}, {"lpre.nr", "lpre.nr"}},
//
//  /* xchg rA, rB */
//  {&snowhousecpu_grp_info_g0_atomic,
//    SNOWHOUSECPU_OPC_INFO_NULL_OP, SNOWHOUSECPU_OA_RA_RB_XCHG,
//    {"xchg", "xchg"}, {"xchg.nr", "xchg.nr"}},
//
//  /* xchg.l [rA], rB */
//  {&snowhousecpu_grp_info_g0_atomic,
//    SNOWHOUSECPU_OPC_INFO_NULL_OP, SNOWHOUSECPU_OA_RA_RB_XCHG_LOCK,
//    {"xchg.l", "xchg.l"}, {"xchg.l.nr", "xchg.l.nr"}},
//
//  /* cmpxchg [rA], rC, rB */
//  {&snowhousecpu_grp_info_g0_atomic,
//    SNOWHOUSECPU_OPC_INFO_NULL_OP, SNOWHOUSECPU_OA_RA_RC_RB_CMPXCHG,
//    {"cmpxchg", "cmpxchg"}, {"cmpxchg.nr", "cmpxchg.nr"}},
//
//  /* cmpxchg.l [rA], rC, rB */
//  {&snowhousecpu_grp_info_g0_atomic,
//    SNOWHOUSECPU_OPC_INFO_NULL_OP, SNOWHOUSECPU_OA_RA_RC_RB_CMPXCHG_LOCK,
//    {"cmpxchg.l", "cmpxchg.l"}, {"cmpxchg.l.nr", "cmpxchg.l.nr"}},
//
//};
//
//static const snowhousecpu_grp_info_t snowhousecpu_grp_info_g1 =
//{
//  .grp=&snowhousecpu_enc_info_grp_16,
//  .grp_value=SNOWHOUSECPU_G1_GRP_VALUE,
//
//  .subgrp=NULL,
//  .subgrp_value=0,
//};
//static const snowhousecpu_opc_info_t
//  snowhousecpu_opc_info_g1[SNOWHOUSECPU_G1_OPC_INFO_LIM] =
//{
//  /* -------- */
//  /* add rA, #simm */
//  {&snowhousecpu_grp_info_g1,
//    SNOWHOUSECPU_G1_OP_ENUM_ADD_RA_S5, SNOWHOUSECPU_OA_RA_S5,
//    {"add", "add"}, {"add.nr", "add.nr"}},
//
//  /* add rA, pc, #simm */
//  {&snowhousecpu_grp_info_g1,
//    SNOWHOUSECPU_G1_OP_ENUM_ADD_RA_PC_S5, SNOWHOUSECPU_OA_RA_PC_S5,
//    {"add", "add"}, {"add.nr", "add.nr"}},
//
//  /* add rA, sp, #simm */
//  {&snowhousecpu_grp_info_g1,
//    SNOWHOUSECPU_G1_OP_ENUM_ADD_RA_SP_S5, SNOWHOUSECPU_OA_RA_SP_S5,
//    {"add", "add"}, {"add.nr", "add.nr"}},
//
//  /* add rA, fp, #simm */
//  {&snowhousecpu_grp_info_g1,
//    SNOWHOUSECPU_G1_OP_ENUM_ADD_RA_FP_S5, SNOWHOUSECPU_OA_RA_FP_S5,
//    {"add", "add"}, {"add.nr", "add.nr"}},
//  /* -------- */
//  /* cmp rA, #simm */
//  {&snowhousecpu_grp_info_g1,
//    SNOWHOUSECPU_G1_OP_ENUM_CMP_RA_S5, SNOWHOUSECPU_OA_RA_S5,
//    {"cmp", "cmp"}, {"cmp.nr", "cmp.nr"}},
//
//  /* cpy rA, #simm */
//  {&snowhousecpu_grp_info_g1,
//    SNOWHOUSECPU_G1_OP_ENUM_CPY_RA_S5, SNOWHOUSECPU_OA_RA_S5,
//    {"cpy", "cpy"}, {"cpy.nr", "cpy.nr"}},
//
//  /* lsl rA, #imm */
//  {&snowhousecpu_grp_info_g1,
//    SNOWHOUSECPU_G1_OP_ENUM_LSL_RA_U5, SNOWHOUSECPU_OA_RA_U5,
//    {"lsl", "lsl"}, {"lsl.nr", "lsl.nr"}},
//
//  /* lsr rA, #imm */
//  {&snowhousecpu_grp_info_g1,
//    SNOWHOUSECPU_G1_OP_ENUM_LSR_RA_U5, SNOWHOUSECPU_OA_RA_U5,
//    {"lsr", "lsr"}, {"lsr.nr", "lsr.nr"}},
//  /* -------- */
//  /* asr rA, #imm */
//  {&snowhousecpu_grp_info_g1,
//    SNOWHOUSECPU_G1_OP_ENUM_ASR_RA_U5, SNOWHOUSECPU_OA_RA_U5,
//    {"asr", "asr"}, {"asr.nr", "asr.nr"}},
//
//  /* and rA, #simm */
//  {&snowhousecpu_grp_info_g1,
//    SNOWHOUSECPU_G1_OP_ENUM_AND_RA_S5, SNOWHOUSECPU_OA_RA_S5,
//    {"and", "and"}, {"and.nr", "and.nr"}},
//
//  /* orr rA, #simm */
//  {&snowhousecpu_grp_info_g1,
//    SNOWHOUSECPU_G1_OP_ENUM_ORR_RA_S5, SNOWHOUSECPU_OA_RA_S5,
//    {"orr", "orr"}, {"orr.nr", "orr.nr"}},
//
//  /* xor rA, #simm */
//  {&snowhousecpu_grp_info_g1,
//    SNOWHOUSECPU_G1_OP_ENUM_XOR_RA_S5, SNOWHOUSECPU_OA_RA_S5,
//    {"xor", "xor"}, {"xor.nr", "xor.nr"}},
//  /* -------- */
//  /* ze rA, #imm */
//  {&snowhousecpu_grp_info_g1,
//  SNOWHOUSECPU_G1_OP_ENUM_ZE_RA_U5, SNOWHOUSECPU_OA_RA_U5,
//    {"ze", "ze"}, {"ze.nr", "ze.nr"}},
//
//  /* se rA, #imm */
//  {&snowhousecpu_grp_info_g1,
//  SNOWHOUSECPU_G1_OP_ENUM_SE_RA_U5, SNOWHOUSECPU_OA_RA_U5,
//    {"se", "se"}, {"se.nr", "se.nr"}},
//
//  /* swi rA, #simm */
//  {&snowhousecpu_grp_info_g1,
//  SNOWHOUSECPU_G1_OP_ENUM_SWI_RA_S5, SNOWHOUSECPU_OA_RA_S5,
//    {"swi", "swi"}, {"swi.nr", "swi.nr"}},
//
//  /* swi #imm */
//  {&snowhousecpu_grp_info_g1,
//  SNOWHOUSECPU_G1_OP_ENUM_SWI_U5, SNOWHOUSECPU_OA_U5,
//    {"swi", "swi"}, {"swi.nr", "swi.nr"}},
//  /* -------- */
//  /* Pseudo instructions start here */
//  /* cpydf rA, rB, #const_double */
//  {&snowhousecpu_grp_info_g1,
//  SNOWHOUSECPU_G1_OP_ENUM_CPY_RA_S5, SNOWHOUSECPU_OA_RA_RB_CPY64,
//    {"cpy64", "cpy64"}, {"cpy64.nr", "cpy64.nr"}},
//  /* -------- */
//};
//
//static const snowhousecpu_grp_info_t snowhousecpu_grp_info_g2 =
//{
//  .grp=&snowhousecpu_enc_info_grp_16,
//  .grp_value=SNOWHOUSECPU_G2_GRP_VALUE,
//
//  .subgrp=NULL,
//  .subgrp_value=0,
//};
//static const snowhousecpu_opc_info_t
//  snowhousecpu_opc_info_g2[SNOWHOUSECPU_G2_OPC_INFO_LIM] =
//{
//  /* -------- */
//  /* add rA, rB */
//  {&snowhousecpu_grp_info_g2,
//    SNOWHOUSECPU_G2_OP_ENUM_ADD_RA_RB, SNOWHOUSECPU_OA_RA_RB,
//    {"add", "add.f"}, {"add.nr", "add.f.nr"}},
//
//  /* sub rA, rB */
//  {&snowhousecpu_grp_info_g2,
//    SNOWHOUSECPU_G2_OP_ENUM_SUB_RA_RB, SNOWHOUSECPU_OA_RA_RB,
//    {"sub", "sub.f"}, {"sub.nr", "sub.f.nr"}},
//
//  /* add rA, sp, rB */
//  {&snowhousecpu_grp_info_g2,
//    SNOWHOUSECPU_G2_OP_ENUM_ADD_RA_SP_RB, SNOWHOUSECPU_OA_RA_SP_RB, 
//    {"add", "add.f"}, {"add.nr", "add.f.nr"}},
//
//  /* add rA, fp, rB */
//  {&snowhousecpu_grp_info_g2,
//    SNOWHOUSECPU_G2_OP_ENUM_ADD_RA_FP_RB, SNOWHOUSECPU_OA_RA_FP_RB,
//    {"add", "add.f"}, {"add.nr", "add.f.nr"}},
//  /* -------- */
//  /* cmp rA, rB */
//  {&snowhousecpu_grp_info_g2,
//    SNOWHOUSECPU_G2_OP_ENUM_CMP_RA_RB, SNOWHOUSECPU_OA_RA_RB,
//    {"cmp", "cmp"}, {"cmp.nr", "cmp.nr"}},
//
//  /* cpy rA, rB */
//  {&snowhousecpu_grp_info_g2,
//    SNOWHOUSECPU_G2_OP_ENUM_CPY_RA_RB, SNOWHOUSECPU_OA_RA_RB,
//    {"cpy", "cpy.f"}, {"cpy.nr", "cpy.f.nr"}},
//
//  /* lsl rA, rB */
//  {&snowhousecpu_grp_info_g2,
//    SNOWHOUSECPU_G2_OP_ENUM_LSL_RA_RB, SNOWHOUSECPU_OA_RA_RB,
//    {"lsl", "lsl.f"}, {"lsl.nr", "lsl.f.nr"}},
//
//  /* lsr rA, rB */
//  {&snowhousecpu_grp_info_g2,
//    SNOWHOUSECPU_G2_OP_ENUM_LSR_RA_RB, SNOWHOUSECPU_OA_RA_RB,
//    {"lsr", "lsr.f"}, {"lsr.nr", "lsr.f.nr"}},
//  /* -------- */
//  /* asr rA, rB */
//  {&snowhousecpu_grp_info_g2,
//    SNOWHOUSECPU_G2_OP_ENUM_ASR_RA_RB, SNOWHOUSECPU_OA_RA_RB, 
//    {"asr", "asr.f"}, {"asr.nr", "asr.f.nr"}},
//
//  /* and rA, rB */
//  {&snowhousecpu_grp_info_g2,
//    SNOWHOUSECPU_G2_OP_ENUM_AND_RA_RB, SNOWHOUSECPU_OA_RA_RB,
//    {"and", "and.f"}, {"and.nr", "and.f.nr"}},
//
//  /* orr rA, rB */
//  {&snowhousecpu_grp_info_g2,
//    SNOWHOUSECPU_G2_OP_ENUM_ORR_RA_RB, SNOWHOUSECPU_OA_RA_RB,
//    {"orr", "orr.f"}, {"orr.nr", "orr.f.nr"}},
//
//  /* xor rA, rB */
//  {&snowhousecpu_grp_info_g2,
//    SNOWHOUSECPU_G2_OP_ENUM_XOR_RA_RB, SNOWHOUSECPU_OA_RA_RB,
//    {"xor", "xor.f"}, {"xor.nr", "xor.f.nr"}},
//  /* -------- */
//  /* adc rA, rB  */
//  {&snowhousecpu_grp_info_g2,
//    SNOWHOUSECPU_G2_OP_ENUM_ADC_RA_RB, SNOWHOUSECPU_OA_RA_RB,
//    {"adc", "adc.f"}, {"adc.nr", "adc.f.nr"}},
//
//  /* sbc rA, rB */
//  {&snowhousecpu_grp_info_g2,
//    SNOWHOUSECPU_G2_OP_ENUM_SBC_RA_RB, SNOWHOUSECPU_OA_RA_RB,
//    {"sbc", "sbc.f"}, {"sbc.nr", "sbc.f.nr"}},
//
//  /* cmpbc rA, rB */
//  {&snowhousecpu_grp_info_g2,
//    SNOWHOUSECPU_G2_OP_ENUM_CMPBC_RA_RB, SNOWHOUSECPU_OA_RA_RB,
//    {"cmpbc", "cmpbc.f"}, {"cmpbc.nr", "cmpbc.f.nr"}},
//
//  /* reserved */
//  {&snowhousecpu_grp_info_g2,
//    SNOWHOUSECPU_G2_OP_ENUM_RESERVED_15, SNOWHOUSECPU_OA_BAD,
//    {"bad", "bad.f"}, {"bad.nr", "bad.f.nr"}},
//  /* -------- */
//};
//
//static const snowhousecpu_grp_info_t snowhousecpu_grp_info_g3 =
//{
//  .grp=&snowhousecpu_enc_info_grp_16,
//  .grp_value=SNOWHOUSECPU_G3_GRP_VALUE,
//
//  .subgrp=NULL,
//  .subgrp_value=0,
//};
//static const snowhousecpu_opc_info_t
//  snowhousecpu_opc_info_g3[SNOWHOUSECPU_G3_OPC_INFO_LIM] =
//{
//  /* -------- */
//  /* bl #simm */
//  {&snowhousecpu_grp_info_g3,
//    SNOWHOUSECPU_G3_OP_ENUM_BL_PCREL_S9, SNOWHOUSECPU_OA_PCREL_S9,
//    {"bl", "bl"}, {"bl.nr", "bl.nr"}},
//
//  /* bra #simm */
//  {&snowhousecpu_grp_info_g3,
//    SNOWHOUSECPU_G3_OP_ENUM_BRA_PCREL_S9, SNOWHOUSECPU_OA_PCREL_S9,
//    {"bra", "bra"}, {"bra.nr", "bra.nr"}},
//
//  /* beq #simm */
//  {&snowhousecpu_grp_info_g3,
//    SNOWHOUSECPU_G3_OP_ENUM_BEQ_PCREL_S9, SNOWHOUSECPU_OA_PCREL_S9,
//    {"beq", "beq"}, {"beq.nr", "beq.nr"}},
//
//  /* bne #simm */
//  {&snowhousecpu_grp_info_g3,
//    SNOWHOUSECPU_G3_OP_ENUM_BNE_PCREL_S9, SNOWHOUSECPU_OA_PCREL_S9,
//    {"bne", "bne"}, {"bne.nr", "bne.nr"}},
//  /* -------- */
//  /* bmi #simm */
//  {&snowhousecpu_grp_info_g3,
//    SNOWHOUSECPU_G3_OP_ENUM_BMI_PCREL_S9, SNOWHOUSECPU_OA_PCREL_S9,
//    {"bmi", "bmi"}, {"bmi.nr", "bmi.nr"}},
//
//  /* bpl #simm */
//  {&snowhousecpu_grp_info_g3,
//    SNOWHOUSECPU_G3_OP_ENUM_BPL_PCREL_S9, SNOWHOUSECPU_OA_PCREL_S9,
//    {"bpl", "bpl"}, {"bpl.nr", "bpl.nr"}},
//
//  /* bvs #simm */
//  {&snowhousecpu_grp_info_g3,
//    SNOWHOUSECPU_G3_OP_ENUM_BVS_PCREL_S9, SNOWHOUSECPU_OA_PCREL_S9,
//    {"bvs", "bvs"}, {"bvs.nr", "bvs.nr"}},
//
//  /* bvc simm */
//  {&snowhousecpu_grp_info_g3,
//    SNOWHOUSECPU_G3_OP_ENUM_BVC_PCREL_S9, SNOWHOUSECPU_OA_PCREL_S9,
//    {"bvc", "bvc"}, {"bvc.nr", "bvc.nr"}},
//  /* -------- */
//  /* bgeu #simm */
//  {&snowhousecpu_grp_info_g3,
//    SNOWHOUSECPU_G3_OP_ENUM_BGEU_PCREL_S9, SNOWHOUSECPU_OA_PCREL_S9,
//    {"bgeu", "bgeu"}, {"bgeu.nr", "bgeu.nr"}},
//
//  /* bltu #simm */
//  {&snowhousecpu_grp_info_g3,
//    SNOWHOUSECPU_G3_OP_ENUM_BLTU_PCREL_S9, SNOWHOUSECPU_OA_PCREL_S9,
//    {"bltu", "bltu"}, {"bltu.nr", "bltu.nr"}},
//
//  /* bgtu #simm */
//  {&snowhousecpu_grp_info_g3,
//    SNOWHOUSECPU_G3_OP_ENUM_BGTU_PCREL_S9, SNOWHOUSECPU_OA_PCREL_S9,
//    {"bgtu", "bgtu"}, {"bgtu.nr", "bgtu.nr"}},
//
//  /* bleu #simm */
//  {&snowhousecpu_grp_info_g3,
//    SNOWHOUSECPU_G3_OP_ENUM_BLEU_PCREL_S9, SNOWHOUSECPU_OA_PCREL_S9,
//    {"bleu", "bleu"}, {"bleu.nr", "bleu.nr"}},
//  /* -------- */
//  /* bges #simm */
//  {&snowhousecpu_grp_info_g3,
//    SNOWHOUSECPU_G3_OP_ENUM_BGES_PCREL_S9, SNOWHOUSECPU_OA_PCREL_S9,
//    {"bges", "bges"}, {"bges.nr", "bges.nr"}},
//
//  /* blts #simm */
//  {&snowhousecpu_grp_info_g3,
//    SNOWHOUSECPU_G3_OP_ENUM_BLTS_PCREL_S9, SNOWHOUSECPU_OA_PCREL_S9,
//    {"blts", "blts"}, {"blts.nr", "blts.nr"}},
//
//  /* bgts #simm */
//  {&snowhousecpu_grp_info_g3,
//    SNOWHOUSECPU_G3_OP_ENUM_BGTS_PCREL_S9, SNOWHOUSECPU_OA_PCREL_S9,
//    {"bgts", "bgts"}, {"bgts.nr", "bgts.nr"}},
//
//  /* bles #simm */
//  {&snowhousecpu_grp_info_g3,
//    SNOWHOUSECPU_G3_OP_ENUM_BLES_PCREL_S9, SNOWHOUSECPU_OA_PCREL_S9,
//    {"bles", "bles"}, {"bles.nr", "bles.nr"}},
//  /* -------- */
//};
//static const snowhousecpu_grp_info_t snowhousecpu_grp_info_g4 =
//{
//  .grp=&snowhousecpu_enc_info_grp_16,
//  .grp_value=SNOWHOUSECPU_G4_GRP_VALUE,
//
//  .subgrp=NULL,
//  .subgrp_value=0,
//};
//static const snowhousecpu_opc_info_t
//  snowhousecpu_opc_info_g4[SNOWHOUSECPU_G4_OPC_INFO_LIM] =
//{
//  /* -------- */
//  /* jl rA */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_JL_RA, SNOWHOUSECPU_OA_RA,
//    {"jl", "jl"}, {"jl.nr", "jl.nr"}},
//
//  /* jmp rA */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_JMP_RA, SNOWHOUSECPU_OA_RA,
//    {"jmp", "jmp"}, {"jmp.nr", "jmp.nr"}},
//
//  /* jmp rA, rB */
//  //{&snowhousecpu_grp_info_g4,
//  //  SNOWHOUSECPU_G4_OP_ENUM_JMP_RA_RB, SNOWHOUSECPU_OA_RA_RB,
//  //  {"jmp", "jmp"}, {"jmp.nr", "jmp.nr"}},
//
//  /* jmp ira */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_JMP_IRA, SNOWHOUSECPU_OA_IRA,
//    {"jmp", "jmp"}, {"jmp.nr", "jmp.nr"}},
//
//  /* reti */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_RETI, SNOWHOUSECPU_OA_NONE,
//    {"reti", "reti"}, {"reti.nr", "reti.nr"}},
//  /* -------- */
//  /* ei */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_EI, SNOWHOUSECPU_OA_NONE,
//    {"ei", "ei"}, {"ei.nr", "ei.nr"}},
//
//  /* di */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_DI, SNOWHOUSECPU_OA_NONE,
//    {"di", "di"}, {"di.nr", "di.nr"}},
//
//  /* push rA, rB */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_PUSH_RA_RB, SNOWHOUSECPU_OA_RA_RB,
//    {"push", "push"}, {"push.nr", "push.nr"}},
//
//  /* push sA, rB */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_PUSH_SA_RB, SNOWHOUSECPU_OA_SA_RB,
//    {"push", "push"}, {"push.nr", "push.nr"}},
//  /* -------- */
//  /* pop rA, rB */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_POP_RA_RB, SNOWHOUSECPU_OA_RA_RB,
//    {"pop", "pop"}, {"pop.nr", "pop.nr"}},
//
//  /* pop sA, rB */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_POP_SA_RB, SNOWHOUSECPU_OA_SA_RB,
//    {"pop", "pop"}, {"pop.nr", "pop.nr"}},
//
//  /* pop pc, rB */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_POP_PC_RB, SNOWHOUSECPU_OA_PC_RB,
//    {"pop", "pop"}, {"pop.nr", "pop.nr"}},
//
//  /* mul rA, rB */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_MUL_RA_RB, SNOWHOUSECPU_OA_RA_RB,
//    {"mul", "mul"}, {"mul.nr", "mul.nr"}},
//  /* -------- */
//  /* udivmod rA, rB */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_UDIVMOD_RA_RB, SNOWHOUSECPU_OA_RA_RB,
//    {"udivmod", "udivmod"}, {"udivmod.nr", "udivmod.nr"}},
//
//  /* sdivmod rA, rB */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_SDIVMOD_RA_RB, SNOWHOUSECPU_OA_RA_RB,
//    {"sdivmod", "sdivmod"}, {"sdivmod.nr", "sdivmod.nr"}},
//
//  ///* umod rA, rB */
//  //{&snowhousecpu_grp_info_g4,
//  //  SNOWHOUSECPU_G4_OP_ENUM_UMOD_RA_RB, SNOWHOUSECPU_OA_RA_RB,
//  //  {"umod", "umod"}, {"umod.nr", "umod.nr"}},
//
//  ///* smod rA, rB */
//  //{&snowhousecpu_grp_info_g4,
//  //  SNOWHOUSECPU_G4_OP_ENUM_SMOD_RA_RB, SNOWHOUSECPU_OA_RA_RB,
//  //  {"smod", "smod"}, {"smod.nr", "smod.nr"}},
//
//
//  /* lumul rA, rB */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_LUMUL_RA_RB, SNOWHOUSECPU_OA_RA_RB,
//    {"lumul", "lumul"}, {"lumul.nr", "lumul.nr"}},
//
//  /* lsmul rA, rB */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_LSMUL_RA_RB, SNOWHOUSECPU_OA_RA_RB,
//    {"lsmul", "lsmul"}, {"lsmul.nr", "lsmul.nr"}},
//
//
//  /* -------- */
//
//  /* udivmod64 rA, rB */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_UDIVMOD64_RA_RB, SNOWHOUSECPU_OA_RA_RB,
//    {"udivmod64", "udivmod64"}, {"udivmod64.nr", "udivmod64.nr"}},
//
//  /* sdivmod64 rA, rB */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_SDIVMOD64_RA_RB, SNOWHOUSECPU_OA_RA_RB,
//    {"sdivmod64", "sdivmod64"}, {"sdivmod64.nr", "sdivmod64.nr"}},
//  /* -------- */
//  ///* umod64 rA, rB */
//  //{&snowhousecpu_grp_info_g4,
//  //  SNOWHOUSECPU_G4_OP_ENUM_UMOD64_RA_RB, SNOWHOUSECPU_OA_RA_RB,
//  //  {"umod64", "umod64"}, {"umod64.nr", "umod64.nr"}},
//
//  ///* smod64 rA, rB */
//  //{&snowhousecpu_grp_info_g4,
//  //  SNOWHOUSECPU_G4_OP_ENUM_SMOD64_RA_RB, SNOWHOUSECPU_OA_RA_RB,
//  //  {"smod64", "smod64"}, {"smod64.nr", "smod64.nr"}},
//
//  /* ldub rA, [rB] */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_LDUB_RA_RB, SNOWHOUSECPU_OA_RA_RB_LDST,
//    {"ldub", "ldub"}, {"ldub.nr", "ldub.nr"}},
//
//  /* ldsb rA, [rB] */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_LDSB_RA_RB, SNOWHOUSECPU_OA_RA_RB_LDST,
//    {"ldsb", "ldsb"}, {"ldsb.nr", "ldsb.nr"}},
//  /* -------- */
//  /* lduh rA, [rB] */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_LDUH_RA_RB, SNOWHOUSECPU_OA_RA_RB_LDST,
//    {"lduh", "lduh"}, {"lduh.nr", "lduh.nr"}},
//
//  /* ldsh rA, [rB] */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_LDSH_RA_RB, SNOWHOUSECPU_OA_RA_RB_LDST,
//    {"ldsh", "ldsh"}, {"ldsh.nr", "ldsh.nr"}},
//
//  /* ldr rA, [rB] */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_LDR_RA_RB, SNOWHOUSECPU_OA_RA_RB_LDST,
//    {"ldr", "ldr"}, {"ldr.nr", "ldr.nr"}},
//
//  /* future 64-bit load instruction: ldq rA, [rB] */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_RESERVED_17, SNOWHOUSECPU_OA_BAD,
//    {"bad", "bad"}, {"bad.nr", "bad.nr"}},
//  /* -------- */
//  /* stb rA, [rB] */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_STB_RA_RB, SNOWHOUSECPU_OA_RA_RB_LDST,
//    {"stb", "stb"}, {"stb.nr", "stb.nr"}},
//
//  /* sth rA, [rB] */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_STH_RA_RB, SNOWHOUSECPU_OA_RA_RB_LDST,
//    {"sth", "sth"}, {"sth.nr", "sth.nr"}},
//
//  /* str rA, [rB] */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_STR_RA_RB, SNOWHOUSECPU_OA_RA_RB_LDST,
//    {"str", "str"}, {"str.nr", "str.nr"}},
//
//  /* future 64-bit store instruction: stq rA, [rB] */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_RESERVED_1A, SNOWHOUSECPU_OA_BAD,
//    {"bad", "bad"}, {"bad.nr", "bad.nr"}},
//  /* -------- */
//  /* cpy rA, sB */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_CPY_RA_SB, SNOWHOUSECPU_OA_RA_SB,
//    {"cpy", "cpy"}, {"cpy.nr", "cpy.nr"}},
//
//  /* cpy sA, rB */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_CPY_SA_RB, SNOWHOUSECPU_OA_SA_RB,
//    {"cpy", "cpy"}, {"cpy.nr", "cpy.nr"}},
//
//  /* cpy sA, sB */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_CPY_SA_SB, SNOWHOUSECPU_OA_SA_SB,
//    {"cpy", "cpy"}, {"cpy.nr", "cpy.nr"}},
//
//  /* reserved */
//  //{&snowhousecpu_grp_info_g4,
//  //  SNOWHOUSECPU_G4_OP_ENUM_RESERVED_31, SNOWHOUSECPU_OA_BAD,
//  //  {"bad", "bad"}, {"bad.nr", "bad.nr"}},
//  /* index rA */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_RESERVED_1E, SNOWHOUSECPU_OA_BAD,
//    {"bad", "bad"}, {"bad.nr", "bad.nr"}},
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_RESERVED_1F, SNOWHOUSECPU_OA_BAD,
//    {"bad", "bad"}, {"bad.nr", "bad.nr"}},
//  /* -------- */
//  /* Following are some pseudo instructions. */
//  /* push rA
//    Note: implicit `sp` */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_PUSH_RA_RB, SNOWHOUSECPU_OA_RA_IMPLICIT_SP,
//    {"push", "push"}, {"push.nr", "push.nr"}},
//
//  /* pop rA
//    Note: implicit `sp` */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_POP_RA_RB, SNOWHOUSECPU_OA_RA_IMPLICIT_SP,
//    {"pop", "pop"}, {"pop.nr", "pop.nr"}},
//
//  /* push sA
//    Note: implicit `sp` */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_PUSH_SA_RB, SNOWHOUSECPU_OA_SA_IMPLICIT_SP,
//    {"push", "push"}, {"push.nr", "push.nr"}},
//
//  /* pop sA
//    Note: implicit `sp` */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_POP_SA_RB, SNOWHOUSECPU_OA_SA_IMPLICIT_SP,
//    {"pop", "pop"}, {"pop.nr", "pop.nr"}},
//
//  /* pop pc
//    Note: implicit `sp` */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_POP_PC_RB, SNOWHOUSECPU_OA_PC_IMPLICIT_SP,
//    {"pop", "pop"}, {"pop.nr", "pop.nr"}},
//  /* -------- */
//  /* ldub rA, [rB, rC] */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_LDUB_RA_RB, SNOWHOUSECPU_OA_RA_RB_RC_LDST,
//    {"ldub", "ldub"}, {"ldub.nr", "ldub.nr"}},
//
//  /* ldsb rA, [rB, rC] */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_LDSB_RA_RB, SNOWHOUSECPU_OA_RA_RB_RC_LDST,
//    {"ldsb", "ldsb"}, {"ldsb.nr", "ldsb.nr"}},
//
//  /* lduh rA, [rB, rC] */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_LDUH_RA_RB, SNOWHOUSECPU_OA_RA_RB_RC_LDST,
//    {"lduh", "lduh"}, {"lduh.nr", "lduh.nr"}},
//
//  /* ldsh rA, [rB, rC] */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_LDSH_RA_RB, SNOWHOUSECPU_OA_RA_RB_RC_LDST,
//    {"ldsh", "ldsh"}, {"ldsh.nr", "ldsh.nr"}},
//
//  /* ldr rA, [rB, rC] */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_LDR_RA_RB, SNOWHOUSECPU_OA_RA_RB_RC_LDST,
//    {"ldr", "ldr"}, {"ldr.nr", "ldr.nr"}},
//  /* -------- */
//  /* ldub rA, [rB, #simm] */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_LDUB_RA_RB, SNOWHOUSECPU_OA_RA_RB_S7_LDST,
//    {"ldub", "ldub"}, {"ldub.nr", "ldub.nr"}},
//
//  /* ldsb rA, [rB, #simm] */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_LDSB_RA_RB, SNOWHOUSECPU_OA_RA_RB_S7_LDST,
//    {"ldsb", "ldsb"}, {"ldsb.nr", "ldsb.nr"}},
//
//  /* lduh rA, [rB, #simm] */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_LDUH_RA_RB, SNOWHOUSECPU_OA_RA_RB_S7_LDST,
//    {"lduh", "lduh"}, {"lduh.nr", "lduh.nr"}},
//
//  /* ldsh rA, [rB, #simm] */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_LDSH_RA_RB, SNOWHOUSECPU_OA_RA_RB_S7_LDST,
//    {"ldsh", "ldsh"}, {"ldsh.nr", "ldsh.nr"}},
//  /* ldr rA, [rB, #simm] */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_LDR_RA_RB, SNOWHOUSECPU_OA_RA_RB_S7_LDST,
//    {"ldr", "ldr"}, {"ldr.nr", "ldr.nr"}},
//  /* -------- */
//  /* ldub rA, [rB, rC, #simm] */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_LDUB_RA_RB, SNOWHOUSECPU_OA_RA_RB_RC_S7_LDST,
//    {"ldub", "ldub"}, {"ldub.nr", "ldub.nr"}},
//
//  /* ldsb rA, [rB, rC, #simm] */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_LDSB_RA_RB, SNOWHOUSECPU_OA_RA_RB_RC_S7_LDST,
//    {"ldsb", "ldsb"}, {"ldsb.nr", "ldsb.nr"}},
//
//  /* lduh rA, [rB, rC, #simm] */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_LDUH_RA_RB, SNOWHOUSECPU_OA_RA_RB_RC_S7_LDST,
//    {"lduh", "lduh"}, {"lduh.nr", "lduh.nr"}},
//
//  /* ldsh rA, [rB, rC, #simm] */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_LDSH_RA_RB, SNOWHOUSECPU_OA_RA_RB_RC_S7_LDST,
//    {"ldsh", "ldsh"}, {"ldsh.nr", "ldsh.nr"}},
//  /* ldr rA, [rB, rC, #simm] */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_LDR_RA_RB, SNOWHOUSECPU_OA_RA_RB_RC_S7_LDST,
//    {"ldr", "ldr"}, {"ldr.nr", "ldr.nr"}},
//  /* -------- */
//  /* stb rA, [rB, rC] */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_STB_RA_RB, SNOWHOUSECPU_OA_RA_RB_RC_LDST,
//    {"stb", "stb"}, {"stb.nr", "stb.nr"}},
//
//  /* sth rA, [rB, rC] */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_STH_RA_RB, SNOWHOUSECPU_OA_RA_RB_RC_LDST,
//    {"sth", "sth"}, {"sth.nr", "sth.nr"}},
//
//  /* str rA, [rB, rC] */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_STR_RA_RB, SNOWHOUSECPU_OA_RA_RB_RC_LDST,
//    {"str", "str"}, {"str.nr", "str.nr"}},
//  /* -------- */
//  /* stb rA, [rB, #simm] */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_STB_RA_RB, SNOWHOUSECPU_OA_RA_RB_S7_LDST,
//    {"stb", "stb"}, {"stb.nr", "stb.nr"}},
//
//  /* sth rA, [rB, #simm] */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_STH_RA_RB, SNOWHOUSECPU_OA_RA_RB_S7_LDST,
//    {"sth", "sth"}, {"sth.nr", "sth.nr"}},
//
//  /* str rA, [rB, #simm] */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_STR_RA_RB, SNOWHOUSECPU_OA_RA_RB_S7_LDST,
//    {"str", "str"}, {"str.nr", "str.nr"}},
//  /* -------- */
//  /* stb rA, [rB, rC, #simm] */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_STB_RA_RB, SNOWHOUSECPU_OA_RA_RB_RC_S7_LDST,
//    {"stb", "stb"}, {"stb.nr", "stb.nr"}},
//
//  /* sth rA, [rB, rC, #simm] */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_STH_RA_RB, SNOWHOUSECPU_OA_RA_RB_RC_S7_LDST,
//    {"sth", "sth"}, {"sth.nr", "sth.nr"}},
//
//  /* str rA, [rB, rC, #simm] */
//  {&snowhousecpu_grp_info_g4,
//    SNOWHOUSECPU_G4_OP_ENUM_STR_RA_RB, SNOWHOUSECPU_OA_RA_RB_RC_S7_LDST,
//    {"str", "str"}, {"str.nr", "str.nr"}},
//  /* -------- */
//  ///* udivmod rA, rB, rC */
//  //{&snowhousecpu_grp_info_g4,
//  //  SNOWHOUSECPU_G4_OP_ENUM_UDIV_RA_RB, SNOWHOUSECPU_OA_RA_RB_RC_DIVMOD,
//  //  {"udivmod", "udivmod"}, {"udivmod.nr", "udivmod.nr"}},
//
//  ///* sdivmod rA, rB, rC */
//  //{&snowhousecpu_grp_info_g4,
//  //  SNOWHOUSECPU_G4_OP_ENUM_SDIV_RA_RB, SNOWHOUSECPU_OA_RA_RB_RC_DIVMOD,
//  //  {"sdivmod", "sdivmod"}, {"sdivmod.nr", "sdivmod.nr"}},
//
//  ///* lumul rC, rD, rA, rB */
//  //{&snowhousecpu_grp_info_g4,
//  //  SNOWHOUSECPU_G4_OP_ENUM_LUMUL_RA_RB, SNOWHOUSECPU_OA_RC_RD_RA_RB_LMUL,
//  //  {"lumul", "lumul"}, {"lumul.nr", "lumul.nr"}},
//
//  ///* lsmul rC, rD, rA, rB */
//  //{&snowhousecpu_grp_info_g4,
//  //  SNOWHOUSECPU_G4_OP_ENUM_LSMUL_RA_RB, SNOWHOUSECPU_OA_RC_RD_RA_RB_LMUL,
//  //  {"lsmul", "lsmul"}, {"lsmul.nr", "lsmul.nr"}},
//
//  ///* udivmod64 rA, rB, rC, rD */
//  //{&snowhousecpu_grp_info_g4,
//  //  SNOWHOUSECPU_G4_OP_ENUM_UDIV64_RA_RB, SNOWHOUSECPU_OA_RA_RB_RC_RD_DIVMOD64,
//  //  {"udivmod64", "udivmod64"}, {"udivmod64.nr", "udivmod64.nr"}},
//
//  ///* sdivmod64 rA, rB, rC, rD */
//  //{&snowhousecpu_grp_info_g4,
//  //  SNOWHOUSECPU_G4_OP_ENUM_SDIV64_RA_RB, SNOWHOUSECPU_OA_RA_RB_RC_RD_DIVMOD64,
//  //  {"sdivmod64", "sdivmod64"}, {"sdivmod64.nr", "sdivmod64.nr"}},
//};
//static const snowhousecpu_grp_info_t snowhousecpu_grp_info_g5_index_ra_rb =
//{
//  .grp=&snowhousecpu_enc_info_grp_16,
//  .grp_value=SNOWHOUSECPU_G5_GRP_VALUE,
//
//  .subgrp=&snowhousecpu_enc_info_g5_index_ra_rb_subgrp,
//  .subgrp_value=SNOWHOUSECPU_G5_INDEX_RA_RB_SUBGRP_VALUE,
//};
//static const snowhousecpu_opc_info_t
//  snowhousecpu_opc_info_g5_index_ra_rb[SNOWHOUSECPU_G5_INDEX_RA_RB_OPC_INFO_LIM] =
//{
//  /* index rA, rB */
//  {&snowhousecpu_grp_info_g5_index_ra_rb,
//    SNOWHOUSECPU_OPC_INFO_NULL_OP, SNOWHOUSECPU_OA_RA_RB,
//    {"index", "index"}, {"index.nr", "index.nr"}}
//  ///* -------- */
//  ///* ldr rA, [rB, #simm] */
//  //{&snowhousecpu_grp_info_g5,
//  //  SNOWHOUSECPU_OPC_INFO_NULL_OP, SNOWHOUSECPU_OA_RA_RB_S5_LDST,
//  //  {"ldr", "ldr"}, {"ldr.nr", "ldr.nr"}},
//  ///* -------- */
//  ///* Following are some pseudo instructions */
//  ///* ldr rA, [rB] */
//  //{&snowhousecpu_grp_info_g5,
//  //  SNOWHOUSECPU_OPC_INFO_NULL_OP, SNOWHOUSECPU_OA_RA_RB_LDST_32,
//  //  {"ldr", "ldr"}, {"ldr.nr", "ldr.nr"}},
//
//  ///* ldr rA, [rB, rC] */
//  //{&snowhousecpu_grp_info_g5,
//  //  SNOWHOUSECPU_OPC_INFO_NULL_OP, SNOWHOUSECPU_OA_RA_RB_RC_LDST_32,
//  //  {"ldr", "ldr"}, {"ldr.nr", "ldr.nr"}},
//
//  ///* ldr rA, [rB, rC, #simm] */
//  //{&snowhousecpu_grp_info_g5,
//  //  SNOWHOUSECPU_OPC_INFO_NULL_OP, SNOWHOUSECPU_OA_RA_RB_RC_S5_LDST,
//  //  {"ldr", "ldr"}, {"ldr.nr", "ldr.nr"}},
//  /* -------- */
//};
//static const snowhousecpu_grp_info_t snowhousecpu_grp_info_g5_index_ra_simm =
//{
//  .grp=&snowhousecpu_enc_info_grp_16,
//  .grp_value=SNOWHOUSECPU_G5_GRP_VALUE,
//
//  .subgrp=&snowhousecpu_enc_info_g5_index_ra_simm_subgrp,
//  .subgrp_value=SNOWHOUSECPU_G5_INDEX_RA_SIMM_SUBGRP_VALUE,
//};
//static const snowhousecpu_opc_info_t
//  snowhousecpu_opc_info_g5_index_ra_simm[SNOWHOUSECPU_G5_INDEX_RA_SIMM_OPC_INFO_LIM] =
//{
//  /* index rA, rB */
//  {&snowhousecpu_grp_info_g5_index_ra_simm,
//    SNOWHOUSECPU_OPC_INFO_NULL_OP, SNOWHOUSECPU_OA_RA_S7,
//    {"index", "index"}, {"index.nr", "index.nr"}}
//  ///* -------- */
//  ///* ldr rA, [rB, #simm] */
//  //{&snowhousecpu_grp_info_g5,
//  //  SNOWHOUSECPU_OPC_INFO_NULL_OP, SNOWHOUSECPU_OA_RA_RB_S5_LDST,
//  //  {"ldr", "ldr"}, {"ldr.nr", "ldr.nr"}},
//  ///* -------- */
//  ///* Following are some pseudo instructions */
//  ///* ldr rA, [rB] */
//  //{&snowhousecpu_grp_info_g5,
//  //  SNOWHOUSECPU_OPC_INFO_NULL_OP, SNOWHOUSECPU_OA_RA_RB_LDST_32,
//  //  {"ldr", "ldr"}, {"ldr.nr", "ldr.nr"}},
//
//  ///* ldr rA, [rB, rC] */
//  //{&snowhousecpu_grp_info_g5,
//  //  SNOWHOUSECPU_OPC_INFO_NULL_OP, SNOWHOUSECPU_OA_RA_RB_RC_LDST_32,
//  //  {"ldr", "ldr"}, {"ldr.nr", "ldr.nr"}},
//
//  ///* ldr rA, [rB, rC, #simm] */
//  //{&snowhousecpu_grp_info_g5,
//  //  SNOWHOUSECPU_OPC_INFO_NULL_OP, SNOWHOUSECPU_OA_RA_RB_RC_S5_LDST,
//  //  {"ldr", "ldr"}, {"ldr.nr", "ldr.nr"}},
//  /* -------- */
//};
//
////static const snowhousecpu_grp_info_t snowhousecpu_grp_info_g6 =
////{
////  .grp=&snowhousecpu_enc_info_grp_16,
////  .grp_value=SNOWHOUSECPU_G6_GRP_VALUE,
////
////  .subgrp=NULL,
////  .subgrp_value=0,
////};
////static const snowhousecpu_opc_info_t
////  snowhousecpu_opc_info_g6[SNOWHOUSECPU_G6_OPC_INFO_LIM] =
////{
////  /* -------- */
////  /* str rA, [rB, #simm] */
////  {&snowhousecpu_grp_info_g6,
////    SNOWHOUSECPU_OPC_INFO_NULL_OP, SNOWHOUSECPU_OA_RA_RB_S5_LDST,
////    {"str", "str"}, {"str.nr", "str.nr"}},
////  /* -------- */
////  /* Following are some pseudo instructions */
////  /* st rA, [rB] */
////  {&snowhousecpu_grp_info_g6,
////    SNOWHOUSECPU_OPC_INFO_NULL_OP, SNOWHOUSECPU_OA_RA_RB_LDST_32,
////    {"str", "str"}, {"str.nr", "str.nr"}},
////
////  /* str rA, [rB, rC] */
////  {&snowhousecpu_grp_info_g6,
////    SNOWHOUSECPU_OPC_INFO_NULL_OP, SNOWHOUSECPU_OA_RA_RB_RC_LDST_32,
////    {"str", "str"}, {"str.nr", "str.nr"}},
////
////  /* str rA, [rB, rC, #simm] */
////  {&snowhousecpu_grp_info_g6,
////    SNOWHOUSECPU_OPC_INFO_NULL_OP, SNOWHOUSECPU_OA_RA_RB_RC_S5_LDST,
////    {"str", "str"}, {"str.nr", "str.nr"}},
////  /* -------- */
////};
//
//static const snowhousecpu_grp_info_t snowhousecpu_grp_info_g7_aluopbh =
//{
//  .grp=&snowhousecpu_enc_info_grp_16,
//  .grp_value=SNOWHOUSECPU_G7_GRP_VALUE,
//
//  .subgrp=&snowhousecpu_enc_info_g7_aluopbh_subgrp,
//  .subgrp_value=SNOWHOUSECPU_G7_ALUOPBH_SUBGRP_VALUE,
//};
//static const snowhousecpu_opc_info_t
//  snowhousecpu_opc_info_g7_aluopbh[SNOWHOUSECPU_G7_ALUOPBH_OPC_INFO_LIM] =
//{
//  /* -------- */
//  /* cmpb rA, rB
//    cmph rA, rB */
//  {&snowhousecpu_grp_info_g7_aluopbh,
//    SNOWHOUSECPU_G7_ALUOPBH_OP_ENUM_CMP_RA_RB, SNOWHOUSECPU_OA_RA_RB,
//    {"cmpb", "cmph"}, {"cmpb.nr", "cmph.nr"}},
//
//  /* lsrb rA, rB
//    lsrh rA, rB */
//  {&snowhousecpu_grp_info_g7_aluopbh,
//    SNOWHOUSECPU_G7_ALUOPBH_OP_ENUM_LSR_RA_RB, SNOWHOUSECPU_OA_RA_RB,
//    {"lsrb", "lsrh"}, {"lsrb.nr", "lsrh.nr"}},
//
//  /* asrb rA, rB
//    asrh rA, rB */
//  {&snowhousecpu_grp_info_g7_aluopbh,
//    SNOWHOUSECPU_G7_ALUOPBH_OP_ENUM_ASR_RA_RB, SNOWHOUSECPU_OA_RA_RB,
//    {"asrb", "asrh"}, {"asrb.nr", "asrh.nr"}},
//
//  /* reserved */
//  {&snowhousecpu_grp_info_g7_aluopbh,
//    SNOWHOUSECPU_G7_ALUOPBH_OP_ENUM_RESERVED_3, SNOWHOUSECPU_OA_BAD,
//    {"bad", "bad"}, {"bad.nr", "bad.nr"}},
//  /* -------- */
//};
//
//static const snowhousecpu_grp_info_t snowhousecpu_grp_info_g7_sprldst =
//{
//  .grp=&snowhousecpu_enc_info_grp_16,
//  .grp_value=SNOWHOUSECPU_G7_GRP_VALUE,
//
//  .subgrp=&snowhousecpu_enc_info_g7_sprldst_subgrp,
//  .subgrp_value=SNOWHOUSECPU_G7_SPRLDST_SUBGRP_VALUE,
//};
//static const snowhousecpu_opc_info_t
//  snowhousecpu_opc_info_g7_sprldst[SNOWHOUSECPU_G7_SPRLDST_OPC_INFO_LIM] =
//{
//  /* -------- */
//  /* ldr sA, [rB] */
//  {&snowhousecpu_grp_info_g7_sprldst,
//    SNOWHOUSECPU_G7_SPRLDST_OP_ENUM_LDR_SA_RB, SNOWHOUSECPU_OA_SA_RB_LDST,
//    {"ldr", "ldr"}, {"ldr.nr", "ldr.nr"}},
//
//  /* ldr sA, [sB] */
//  {&snowhousecpu_grp_info_g7_sprldst,
//    SNOWHOUSECPU_G7_SPRLDST_OP_ENUM_LDR_SA_SB, SNOWHOUSECPU_OA_SA_SB_LDST,
//    {"ldr", "ldr"}, {"ldr.nr", "ldr.nr"}},
//
//  /* str sA, [rB] */
//  {&snowhousecpu_grp_info_g7_sprldst,
//    SNOWHOUSECPU_G7_SPRLDST_OP_ENUM_STR_SA_RB, SNOWHOUSECPU_OA_SA_RB_LDST,
//    {"str", "str"}, {"str.nr", "str.nr"}},
//
//  /* str sA, [sB] */
//  {&snowhousecpu_grp_info_g7_sprldst,
//    SNOWHOUSECPU_G7_SPRLDST_OP_ENUM_STR_SA_SB, SNOWHOUSECPU_OA_SA_SB_LDST,
//    {"str", "str"}, {"str.nr", "str.nr"}},
//  /* -------- */
//  /* ldr sA, [rB, rC] */
//  {&snowhousecpu_grp_info_g7_sprldst,
//    SNOWHOUSECPU_G7_SPRLDST_OP_ENUM_LDR_SA_RB, SNOWHOUSECPU_OA_SA_RB_RC_LDST,
//    {"ldr", "ldr"}, {"ldr.nr", "ldr.nr"}},
//
//  /* ldr sA, [sB, rC] */
//  {&snowhousecpu_grp_info_g7_sprldst,
//    SNOWHOUSECPU_G7_SPRLDST_OP_ENUM_LDR_SA_SB, SNOWHOUSECPU_OA_SA_SB_RC_LDST,
//    {"ldr", "ldr"}, {"ldr.nr", "ldr.nr"}},
//
//  /* str sA, [rB, rC] */
//  {&snowhousecpu_grp_info_g7_sprldst,
//    SNOWHOUSECPU_G7_SPRLDST_OP_ENUM_STR_SA_RB, SNOWHOUSECPU_OA_SA_RB_RC_LDST,
//    {"str", "str"}, {"str.nr", "str.nr"}},
//
//  /* str sA, [sB, rC] */
//  {&snowhousecpu_grp_info_g7_sprldst,
//    SNOWHOUSECPU_G7_SPRLDST_OP_ENUM_STR_SA_SB, SNOWHOUSECPU_OA_SA_SB_RC_LDST,
//    {"str", "str"}, {"str.nr", "str.nr"}},
//  /* -------- */
//};
///* -------- */
//static const snowhousecpu_grp_info_t snowhousecpu_grp_info_g7_icreload =
//{
//  .grp=&snowhousecpu_enc_info_grp_16,
//  .grp_value=SNOWHOUSECPU_G7_GRP_VALUE,
//
//  .subgrp=&snowhousecpu_enc_info_g7_icreload_subgrp,
//  .subgrp_value=SNOWHOUSECPU_G7_ICRELOAD_SUBGRP_VALUE,
//};
//static const snowhousecpu_opc_info_t
//  snowhousecpu_opc_info_g7_icreload[SNOWHOUSECPU_G7_ICRELOAD_OPC_INFO_LIM] =
//{
//  /* -------- */
//  /* icreload [rA, #simm] */
//  {&snowhousecpu_grp_info_g7_icreload,
//    SNOWHOUSECPU_OPC_INFO_NULL_OP, SNOWHOUSECPU_OA_RA_S5_JUSTADDR,
//    {"icreload", "icreload"}, {"icreload.nr", "icreload.nr"}},
//  /* -------- */
//  /* Following are some pseudo instructions */
//  /* icreload [rA] */
//  {&snowhousecpu_grp_info_g7_icreload,
//    SNOWHOUSECPU_OPC_INFO_NULL_OP, SNOWHOUSECPU_OA_RA_JUSTADDR,
//    {"icreload", "icreload"}, {"icreload.nr", "icreload.nr"}},
//
//  /* icreload [rA, rC] */
//  {&snowhousecpu_grp_info_g7_icreload,
//    SNOWHOUSECPU_OPC_INFO_NULL_OP, SNOWHOUSECPU_OA_RA_RC_JUSTADDR,
//    {"icreload", "icreload"}, {"icreload.nr", "icreload.nr"}},
//
//  /* icreload [rA, rC, #simm] */
//  {&snowhousecpu_grp_info_g7_icreload,
//    SNOWHOUSECPU_OPC_INFO_NULL_OP, SNOWHOUSECPU_OA_RA_RC_S5_JUSTADDR,
//    {"icreload", "icreload"}, {"icreload.nr", "icreload.nr"}},
//  /* -------- */
//};
//static const snowhousecpu_grp_info_t snowhousecpu_grp_info_g7_icflush =
//{
//  .grp=&snowhousecpu_enc_info_grp_16,
//  .grp_value=SNOWHOUSECPU_G7_GRP_VALUE,
//
//  .subgrp=&snowhousecpu_enc_info_g7_icflush_subgrp,
//  .subgrp_value=SNOWHOUSECPU_G7_ICFLUSH_SUBGRP_VALUE,
//};
//static const snowhousecpu_opc_info_t
//  snowhousecpu_opc_info_g7_icflush[SNOWHOUSECPU_G7_ICFLUSH_OPC_INFO_LIM] =
//{
//  /* -------- */
//  /* icflush */
//  {&snowhousecpu_grp_info_g7_icflush,
//    SNOWHOUSECPU_OPC_INFO_NULL_OP, SNOWHOUSECPU_OA_NONE,
//    {"icflush", "icflush"}, {"icflush.nr", "icflush.nr"}},
//  /* -------- */
//};
///* -------- */
#endif    /* _SNOWHOUSECPU_OPC_DECLS_H_ */

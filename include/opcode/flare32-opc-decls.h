/* flare32-opc-decls.h -- Definitions for flare32 opcodes.
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

#ifndef _FLARE32_OPC_DECLS_H_
#define _FLARE32_OPC_DECLS_H_

//static inline flare32_enc_info_t
//flare32_build_enc_info (flare32_temp_t bitsize, flare32_temp_t bitpos)
//{
//  flare32_enc_info_t ret;
//
//  ret.bitsize = bitsize;
//  ret.bitpos = bitpos;
//  ret.rsmask = FLARE32_N_ONES (ret.bitsize);
//  ret.mask = ret.rsmask << ret.bitpos;
//
//  return ret;
//}
static const flare32_enc_info_t
  /* -------- */
  flare32_enc_info_grp_16 =
    {FLARE32_GRP_BITSIZE,
    FLARE32_GRP_16_BITPOS,
    FLARE32_GRP_RSMASK,
    FLARE32_GRP_16_MASK},
  flare32_enc_info_grp_32 = 
    {FLARE32_GRP_BITSIZE,
    FLARE32_GRP_32_BITPOS,
    FLARE32_GRP_RSMASK,
    FLARE32_GRP_32_MASK},
  /* -------- */
  flare32_enc_info_g0_pre_subgrp =
    {FLARE32_G0_PRE_SUBGRP_BITSIZE, 
    FLARE32_G0_PRE_SUBGRP_BITPOS,
    FLARE32_G0_PRE_SUBGRP_RSMASK,
    FLARE32_G0_PRE_SUBGRP_MASK},

  flare32_enc_info_g0_pre_fullgrp =
    {FLARE32_G0_PRE_FULLGRP_BITSIZE,
    FLARE32_G0_PRE_FULLGRP_BITPOS,
    FLARE32_G0_PRE_FULLGRP_RSMASK,
    FLARE32_G0_PRE_FULLGRP_MASK},
  flare32_enc_info_g0_pre_s12 =
    {FLARE32_G0_PRE_S12_BITSIZE,
    FLARE32_G0_PRE_S12_BITPOS,
    FLARE32_G0_PRE_S12_RSMASK,
    FLARE32_G0_PRE_S12_MASK},
  /* -------- */
  flare32_enc_info_g0_lpre_subgrp =
    {FLARE32_G0_LPRE_SUBGRP_BITSIZE,
    FLARE32_G0_LPRE_SUBGRP_BITPOS,
    FLARE32_G0_LPRE_SUBGRP_RSMASK,
    FLARE32_G0_LPRE_SUBGRP_MASK},
  flare32_enc_info_g0_lpre_fullgrp =
    {FLARE32_G0_LPRE_FULLGRP_BITSIZE,
    FLARE32_G0_LPRE_FULLGRP_BITPOS,
    FLARE32_G0_LPRE_FULLGRP_RSMASK,
    FLARE32_G0_LPRE_FULLGRP_MASK},
  flare32_enc_info_g0_lpre_subgrp_16 =
    {FLARE32_G0_LPRE_SUBGRP_BITSIZE,
    FLARE32_G0_LPRE_SUBGRP_16_BITPOS,
    FLARE32_G0_LPRE_SUBGRP_RSMASK,
    FLARE32_G0_LPRE_SUBGRP_16_MASK},
  flare32_enc_info_g0_lpre_fullgrp_16 =
    {FLARE32_G0_LPRE_FULLGRP_BITSIZE,
    FLARE32_G0_LPRE_FULLGRP_16_BITPOS,
    FLARE32_G0_LPRE_FULLGRP_RSMASK,
    FLARE32_G0_LPRE_FULLGRP_16_MASK},

  flare32_enc_info_g1g5g6_g0_lpre_s27 =
    {FLARE32_G1G5G6_G0_LPRE_S27_BITSIZE,
    FLARE32_G1G5G6_G0_LPRE_S27_BITPOS,
    FLARE32_G1G5G6_G0_LPRE_S27_RSMASK,
    FLARE32_G1G5G6_G0_LPRE_S27_MASK},
  flare32_enc_info_g3_g0_lpre_s23 =
    {FLARE32_G3_G0_LPRE_S23_BITSIZE,
    FLARE32_G3_G0_LPRE_S23_BITPOS,
    FLARE32_G3_G0_LPRE_S23_RSMASK,
    FLARE32_G3_G0_LPRE_S23_MASK},
  /* -------- */
  flare32_enc_info_g1g5g6_s5 =
    {FLARE32_G1G5G6_S5_BITSIZE,
    FLARE32_G1G5G6_S5_BITPOS,
    FLARE32_G1G5G6_S5_RSMASK,
    FLARE32_G1G5G6_S5_MASK},
  flare32_enc_info_g1_op =
    {FLARE32_G1_OP_BITSIZE,
    FLARE32_G1_OP_BITPOS,
    FLARE32_G1_OP_RSMASK,
    FLARE32_G1_OP_MASK},
  flare32_enc_info_g2_f =
    {FLARE32_G2_F_BITSIZE,
    FLARE32_G2_F_BITPOS,
    FLARE32_G2_F_RSMASK,
    FLARE32_G2_F_MASK},
  flare32_enc_info_g2_op =
    {FLARE32_G2_OP_BITSIZE,
    FLARE32_G2_OP_BITPOS,
    FLARE32_G2_OP_RSMASK,
    FLARE32_G2_OP_MASK},
  flare32_enc_info_g3_s9 =
    {FLARE32_G3_S9_BITSIZE,
    FLARE32_G3_S9_BITPOS,
    FLARE32_G3_S9_RSMASK,
    FLARE32_G3_S9_MASK},
  flare32_enc_info_g3_op =
    {FLARE32_G3_OP_BITSIZE,
    FLARE32_G3_OP_BITPOS,
    FLARE32_G3_OP_RSMASK,
    FLARE32_G3_OP_MASK},
  flare32_enc_info_g4_op =
    {FLARE32_G4_OP_BITSIZE,
    FLARE32_G4_OP_BITPOS,
    FLARE32_G4_OP_RSMASK,
    FLARE32_G4_OP_MASK},
  /* -------- */
  flare32_enc_info_g7_aluopbh_subgrp =
    {FLARE32_G7_ALUOPBH_SUBGRP_BITSIZE,
    FLARE32_G7_ALUOPBH_SUBGRP_BITPOS,
    FLARE32_G7_ALUOPBH_SUBGRP_RSMASK,
    FLARE32_G7_ALUOPBH_SUBGRP_MASK},
  flare32_enc_info_g7_aluopbh_fullgrp =
    {FLARE32_G7_ALUOPBH_FULLGRP_BITSIZE,
    FLARE32_G7_ALUOPBH_FULLGRP_BITPOS,
    FLARE32_G7_ALUOPBH_FULLGRP_RSMASK,
    FLARE32_G7_ALUOPBH_FULLGRP_MASK},
  flare32_enc_info_g7_aluopbh_w =
    {FLARE32_G7_ALUOPBH_W_BITSIZE,
    FLARE32_G7_ALUOPBH_W_BITPOS,
    FLARE32_G7_ALUOPBH_W_RSMASK,
    FLARE32_G7_ALUOPBH_W_MASK},
  flare32_enc_info_g7_aluopbh_op =
    {FLARE32_G7_ALUOPBH_OP_BITSIZE,
    FLARE32_G7_ALUOPBH_OP_BITPOS,
    FLARE32_G7_ALUOPBH_OP_RSMASK,
    FLARE32_G7_ALUOPBH_OP_MASK},

  flare32_enc_info_g7_sprldst_subgrp =
    {FLARE32_G7_SPRLDST_SUBGRP_BITSIZE,
    FLARE32_G7_SPRLDST_SUBGRP_BITPOS,
    FLARE32_G7_SPRLDST_SUBGRP_RSMASK,
    FLARE32_G7_SPRLDST_SUBGRP_MASK},
  flare32_enc_info_g7_sprldst_fullgrp =
    {FLARE32_G7_SPRLDST_FULLGRP_BITSIZE,
    FLARE32_G7_SPRLDST_FULLGRP_BITPOS,
    FLARE32_G7_SPRLDST_FULLGRP_RSMASK,
    FLARE32_G7_SPRLDST_FULLGRP_MASK},
  flare32_enc_info_g7_sprldst_op =
    {FLARE32_G7_SPRLDST_OP_BITSIZE,
    FLARE32_G7_SPRLDST_OP_BITPOS,
    FLARE32_G7_SPRLDST_OP_RSMASK,
    FLARE32_G7_SPRLDST_OP_MASK},
  /* -------- */
  flare32_enc_info_ra_ind =
    {FLARE32_RA_IND_BITSIZE,
    FLARE32_RA_IND_BITPOS,
    FLARE32_RA_IND_RSMASK,
    FLARE32_RA_IND_MASK},
  flare32_enc_info_rb_ind =
    {FLARE32_RB_IND_BITSIZE,
    FLARE32_RB_IND_BITPOS,
    FLARE32_RB_IND_RSMASK,
    FLARE32_RB_IND_MASK};
  /* -------- */
/* -------- */
/* -------- */
static const flare32_grp_info_t flare32_grp_info_g0_pre =
{
  .grp=&flare32_enc_info_grp_16,
  .grp_value=FLARE32_G0_GRP_VALUE,

  .subgrp=&flare32_enc_info_g0_pre_subgrp,
  .subgrp_value=FLARE32_G0_PRE_SUBGRP_VALUE,
};
static const flare32_grp_info_t flare32_grp_info_g0_lpre =
{
  .grp=&flare32_enc_info_grp_32,
  .grp_value=FLARE32_G0_GRP_VALUE,

  .subgrp=&flare32_enc_info_g0_lpre_subgrp,
  .subgrp_value=FLARE32_G0_LPRE_SUBGRP_VALUE,
};
static const flare32_opc_info_t
  flare32_opc_info_g0[FLARE32_G0_OPC_INFO_LIM] =
{
  /* pre #simm */
  {&flare32_grp_info_g0_pre,
    FLARE32_OPC_INFO_NULL_OP, FLARE32_OA_PRE,
    {"pre", "pre"}},

  /* lpre #simm */
  {&flare32_grp_info_g0_lpre,
    FLARE32_OPC_INFO_NULL_OP, FLARE32_OA_LPRE,
    {"lpre", "lpre"}},
};

static const flare32_grp_info_t flare32_grp_info_g1 =
{
  .grp=&flare32_enc_info_grp_16,
  .grp_value=FLARE32_G1_GRP_VALUE,

  .subgrp=NULL,
  .subgrp_value=0,
};
static const flare32_opc_info_t
  flare32_opc_info_g1[FLARE32_G1_OPC_INFO_LIM] =
{
  /* -------- */
  /* add rA, #simm */
  {&flare32_grp_info_g1,
    FLARE32_G1_OP_ENUM_ADD_RA_S5, FLARE32_OA_RA_S5,
    {"add", "add"}},

  /* add rA, pc, #simm */
  {&flare32_grp_info_g1,
    FLARE32_G1_OP_ENUM_ADD_RA_PC_S5, FLARE32_OA_RA_PC_S5,
    {"add", "add"}},

  /* add rA, sp, #simm */
  {&flare32_grp_info_g1,
    FLARE32_G1_OP_ENUM_ADD_RA_SP_S5, FLARE32_OA_RA_SP_S5,
    {"add", "add"}},

  /* add rA, fp, #simm */
  {&flare32_grp_info_g1,
    FLARE32_G1_OP_ENUM_ADD_RA_FP_S5, FLARE32_OA_RA_FP_S5,
    {"add", "add"}},
  /* -------- */
  /* cmp rA, #simm */
  {&flare32_grp_info_g1,
    FLARE32_G1_OP_ENUM_CMP_RA_S5, FLARE32_OA_RA_S5,
    {"cmp", "cmp"}},

  /* cpy rA, #simm */
  {&flare32_grp_info_g1,
    FLARE32_G1_OP_ENUM_CPY_RA_S5, FLARE32_OA_RA_S5,
    {"cpy", "cpy"}},

  /* lsl rA, #simm */
  {&flare32_grp_info_g1,
    FLARE32_G1_OP_ENUM_LSL_RA_S5, FLARE32_OA_RA_S5,
    {"lsl", "lsl"}},

  /* lsr rA, #simm */
  {&flare32_grp_info_g1,
    FLARE32_G1_OP_ENUM_LSR_RA_S5, FLARE32_OA_RA_S5,
    {"lsr", "lsr"}},
  /* -------- */
  /* asr rA, #simm */
  {&flare32_grp_info_g1,
    FLARE32_G1_OP_ENUM_ASR_RA_S5, FLARE32_OA_RA_S5,
    {"asr", "asr"}},

  /* and rA, #simm */
  {&flare32_grp_info_g1,
    FLARE32_G1_OP_ENUM_AND_RA_S5, FLARE32_OA_RA_S5,
    {"and", "and"}},

  /* orr rA, #simm */
  {&flare32_grp_info_g1,
    FLARE32_G1_OP_ENUM_ORR_RA_S5, FLARE32_OA_RA_S5,
    {"orr", "orr"}},

  /* xor rA, #simm */
  {&flare32_grp_info_g1,
    FLARE32_G1_OP_ENUM_XOR_RA_S5, FLARE32_OA_RA_S5,
    {"xor", "xor"}},
  /* -------- */
  /* ze rA, #simm */
  {&flare32_grp_info_g1,
  FLARE32_G1_OP_ENUM_ZE_RA_S5, FLARE32_OA_RA_S5,
    {"ze", "ze"}},

  /* se rA, #simm */
  {&flare32_grp_info_g1,
  FLARE32_G1_OP_ENUM_SE_RA_S5, FLARE32_OA_RA_S5,
    {"se", "se"}},

  /* swi rA, #simm */
  {&flare32_grp_info_g1,
  FLARE32_G1_OP_ENUM_SWI_RA_S5, FLARE32_OA_RA_S5,
    {"swi", "swi"}},

  /* swi #simm */
  {&flare32_grp_info_g1,
  FLARE32_G1_OP_ENUM_SWI_S5, FLARE32_OA_S5,
    {"swi", "swi"}},
  /* -------- */
  /* Pseudo instructions start here */
  /* cpydf rA, rB, #const_double */
  {&flare32_grp_info_g1,
  FLARE32_G1_OP_ENUM_CPY_RA_S5, FLARE32_OA_RA_RB_CPY64,
    {"cpy64", "cpy64"}},
  /* -------- */
};

static const flare32_grp_info_t flare32_grp_info_g2 =
{
  .grp=&flare32_enc_info_grp_16,
  .grp_value=FLARE32_G2_GRP_VALUE,

  .subgrp=NULL,
  .subgrp_value=0,
};
static const flare32_opc_info_t
  flare32_opc_info_g2[FLARE32_G2_OPC_INFO_LIM] =
{
  /* -------- */
  /* add rA, rB */
  {&flare32_grp_info_g2,
    FLARE32_G2_OP_ENUM_ADD_RA_RB, FLARE32_OA_RA_RB,
    {"add", "add.f"}},

  /* sub rA, rB */
  {&flare32_grp_info_g2,
    FLARE32_G2_OP_ENUM_SUB_RA_RB, FLARE32_OA_RA_RB,
    {"sub", "sub.f"}},

  /* add rA, sp, rB */
  {&flare32_grp_info_g2,
    FLARE32_G2_OP_ENUM_ADD_RA_SP_RB, FLARE32_OA_RA_SP_RB, 
    {"add", "add.f"}},

  /* add rA, fp, rB */
  {&flare32_grp_info_g2,
    FLARE32_G2_OP_ENUM_ADD_RA_FP_RB, FLARE32_OA_RA_FP_RB,
    {"add", "add.f"}},
  /* -------- */
  /* cmp rA, rB */
  {&flare32_grp_info_g2,
    FLARE32_G2_OP_ENUM_CMP_RA_RB, FLARE32_OA_RA_RB,
    {"cmp", "cmp"}},

  /* cpy rA, rB */
  {&flare32_grp_info_g2,
    FLARE32_G2_OP_ENUM_CPY_RA_RB, FLARE32_OA_RA_RB,
    {"cpy", "cpy.f"}},

  /* lsl rA, rB */
  {&flare32_grp_info_g2,
    FLARE32_G2_OP_ENUM_LSL_RA_RB, FLARE32_OA_RA_RB,
    {"lsl", "lsl.f"}},

  /* lsr rA, rB */
  {&flare32_grp_info_g2,
    FLARE32_G2_OP_ENUM_LSR_RA_RB, FLARE32_OA_RA_RB,
    {"lsr", "lsr.f"}},
  /* -------- */
  /* asr rA, rB */
  {&flare32_grp_info_g2,
    FLARE32_G2_OP_ENUM_ASR_RA_RB, FLARE32_OA_RA_RB, 
    {"asr", "asr.f"}},

  /* and rA, rB */
  {&flare32_grp_info_g2,
    FLARE32_G2_OP_ENUM_AND_RA_RB, FLARE32_OA_RA_RB,
    {"and", "and.f"}},

  /* orr rA, rB */
  {&flare32_grp_info_g2,
    FLARE32_G2_OP_ENUM_ORR_RA_RB, FLARE32_OA_RA_RB,
    {"orr", "orr.f"}},

  /* xor rA, rB */
  {&flare32_grp_info_g2,
    FLARE32_G2_OP_ENUM_XOR_RA_RB, FLARE32_OA_RA_RB,
    {"xor", "xor.f"}},
  /* -------- */
  /* adc rA, rB  */
  {&flare32_grp_info_g2,
    FLARE32_G2_OP_ENUM_ADC_RA_RB, FLARE32_OA_RA_RB,
    {"adc", "adc.f"}},

  /* sbc rA, rB */
  {&flare32_grp_info_g2,
    FLARE32_G2_OP_ENUM_SBC_RA_RB, FLARE32_OA_RA_RB,
    {"sbc", "sbc.f"}},

  /* reserved */
  {&flare32_grp_info_g2,
    FLARE32_G2_OP_ENUM_RESERVED_14, FLARE32_OA_BAD,
    {"bad", "bad.f"}},

  /* reserved */
  {&flare32_grp_info_g2,
    FLARE32_G2_OP_ENUM_RESERVED_15, FLARE32_OA_BAD,
    {"bad", "bad.f"}},
  /* -------- */
};

static const flare32_grp_info_t flare32_grp_info_g3 =
{
  .grp=&flare32_enc_info_grp_16,
  .grp_value=FLARE32_G3_GRP_VALUE,

  .subgrp=NULL,
  .subgrp_value=0,
};
static const flare32_opc_info_t
  flare32_opc_info_g3[FLARE32_G3_OPC_INFO_LIM] =
{
  /* -------- */
  /* bl #simm */
  {&flare32_grp_info_g3,
    FLARE32_G3_OP_ENUM_BL_PCREL_S9, FLARE32_OA_PCREL_S9,
    {"bl", "bl"}},

  /* bra #simm */
  {&flare32_grp_info_g3,
    FLARE32_G3_OP_ENUM_BRA_PCREL_S9, FLARE32_OA_PCREL_S9,
    {"bra", "bra"}},

  /* beq #simm */
  {&flare32_grp_info_g3,
    FLARE32_G3_OP_ENUM_BEQ_PCREL_S9, FLARE32_OA_PCREL_S9,
    {"beq", "beq"}},

  /* bne #simm */
  {&flare32_grp_info_g3,
    FLARE32_G3_OP_ENUM_BNE_PCREL_S9, FLARE32_OA_PCREL_S9,
    {"bne", "bne"}},
  /* -------- */
  /* bmi #simm */
  {&flare32_grp_info_g3,
    FLARE32_G3_OP_ENUM_BMI_PCREL_S9, FLARE32_OA_PCREL_S9,
    {"bmi", "bmi"}},

  /* bpl #simm */
  {&flare32_grp_info_g3,
    FLARE32_G3_OP_ENUM_BPL_PCREL_S9, FLARE32_OA_PCREL_S9,
    {"bpl", "bpl"}},

  /* bvs #simm */
  {&flare32_grp_info_g3,
    FLARE32_G3_OP_ENUM_BVS_PCREL_S9, FLARE32_OA_PCREL_S9,
    {"bvs", "bvs"}},

  /* bvc simm */
  {&flare32_grp_info_g3,
    FLARE32_G3_OP_ENUM_BVC_PCREL_S9, FLARE32_OA_PCREL_S9,
    {"bvc", "bvc"}},
  /* -------- */
  /* bgeu #simm */
  {&flare32_grp_info_g3,
    FLARE32_G3_OP_ENUM_BGEU_PCREL_S9, FLARE32_OA_PCREL_S9,
    {"bgeu", "bgeu"}},

  /* bltu #simm */
  {&flare32_grp_info_g3,
    FLARE32_G3_OP_ENUM_BLTU_PCREL_S9, FLARE32_OA_PCREL_S9,
    {"bltu", "bltu"}},

  /* bgtu #simm */
  {&flare32_grp_info_g3,
    FLARE32_G3_OP_ENUM_BGTU_PCREL_S9, FLARE32_OA_PCREL_S9,
    {"bgtu", "bgtu"}},

  /* bleu #simm */
  {&flare32_grp_info_g3,
    FLARE32_G3_OP_ENUM_BLEU_PCREL_S9, FLARE32_OA_PCREL_S9,
    {"bleu", "bleu"}},
  /* -------- */
  /* bges #simm */
  {&flare32_grp_info_g3,
    FLARE32_G3_OP_ENUM_BGES_PCREL_S9, FLARE32_OA_PCREL_S9,
    {"bges", "bges"}},

  /* blts #simm */
  {&flare32_grp_info_g3,
    FLARE32_G3_OP_ENUM_BLTS_PCREL_S9, FLARE32_OA_PCREL_S9,
    {"blts", "blts"}},

  /* bgts #simm */
  {&flare32_grp_info_g3,
    FLARE32_G3_OP_ENUM_BGTS_PCREL_S9, FLARE32_OA_PCREL_S9,
    {"bgts", "bgts"}},

  /* bles #simm */
  {&flare32_grp_info_g3,
    FLARE32_G3_OP_ENUM_BLES_PCREL_S9, FLARE32_OA_PCREL_S9,
    {"bles", "bles"}},
  /* -------- */
  /* Pseudo instructions start here */
  /* bl.l #simm */
  {&flare32_grp_info_g3,
    FLARE32_G3_OP_ENUM_BL_PCREL_S9, FLARE32_OA_PCREL_S32_NO_RELAX,
    {"bl.l", "bl.l"}},

  /* bra.l #simm */
  {&flare32_grp_info_g3,
    FLARE32_G3_OP_ENUM_BRA_PCREL_S9, FLARE32_OA_PCREL_S32_NO_RELAX,
    {"bra.l", "bra.l"}},

  /* beq.l #simm */
  {&flare32_grp_info_g3,
    FLARE32_G3_OP_ENUM_BEQ_PCREL_S9, FLARE32_OA_PCREL_S32_NO_RELAX,
    {"beq.l.", "beq"}},

  /* bne.l #simm */
  {&flare32_grp_info_g3,
    FLARE32_G3_OP_ENUM_BNE_PCREL_S9, FLARE32_OA_PCREL_S32_NO_RELAX,
    {"bne.l", "bne.l"}},
  /* -------- */
  /* bmi.l #simm */
  {&flare32_grp_info_g3,
    FLARE32_G3_OP_ENUM_BMI_PCREL_S9, FLARE32_OA_PCREL_S32_NO_RELAX,
    {"bmi.l", "bmi.l"}},

  /* bpl.l #simm */
  {&flare32_grp_info_g3,
    FLARE32_G3_OP_ENUM_BPL_PCREL_S9, FLARE32_OA_PCREL_S32_NO_RELAX,
    {"bpl.l", "bpl.l"}},

  /* bvs.l #simm */
  {&flare32_grp_info_g3,
    FLARE32_G3_OP_ENUM_BVS_PCREL_S9, FLARE32_OA_PCREL_S32_NO_RELAX,
    {"bvs.l", "bvs.l"}},

  /* bvc.l simm */
  {&flare32_grp_info_g3,
    FLARE32_G3_OP_ENUM_BVC_PCREL_S9, FLARE32_OA_PCREL_S32_NO_RELAX,
    {"bvc.l", "bvc.l"}},
  /* -------- */
  /* bgeu.l #simm */
  {&flare32_grp_info_g3,
    FLARE32_G3_OP_ENUM_BGEU_PCREL_S9, FLARE32_OA_PCREL_S32_NO_RELAX,
    {"bgeu.l", "bgeu.l"}},

  /* bltu.l #simm */
  {&flare32_grp_info_g3,
    FLARE32_G3_OP_ENUM_BLTU_PCREL_S9, FLARE32_OA_PCREL_S32_NO_RELAX,
    {"bltu.l", "bltu.l"}},

  /* bgtu.l #simm */
  {&flare32_grp_info_g3,
    FLARE32_G3_OP_ENUM_BGTU_PCREL_S9, FLARE32_OA_PCREL_S32_NO_RELAX,
    {"bgtu.l", "bgtu.l"}},

  /* bleu.l #simm */
  {&flare32_grp_info_g3,
    FLARE32_G3_OP_ENUM_BLEU_PCREL_S9, FLARE32_OA_PCREL_S32_NO_RELAX,
    {"bleu.l", "bleu.l"}},
  /* -------- */
  /* bges.l #simm */
  {&flare32_grp_info_g3,
    FLARE32_G3_OP_ENUM_BGES_PCREL_S9, FLARE32_OA_PCREL_S32_NO_RELAX,
    {"bges.l", "bges.l"}},

  /* blts.l #simm */
  {&flare32_grp_info_g3,
    FLARE32_G3_OP_ENUM_BLTS_PCREL_S9, FLARE32_OA_PCREL_S32_NO_RELAX,
    {"blts.l", "blts.l"}},

  /* bgts.l #simm */
  {&flare32_grp_info_g3,
    FLARE32_G3_OP_ENUM_BGTS_PCREL_S9, FLARE32_OA_PCREL_S32_NO_RELAX,
    {"bgts.l", "bgts.l"}},

  /* bles.l #simm */
  {&flare32_grp_info_g3,
    FLARE32_G3_OP_ENUM_BLES_PCREL_S9, FLARE32_OA_PCREL_S32_NO_RELAX,
    {"bles.l", "bles.l"}},
  /* -------- */
};
static const flare32_grp_info_t flare32_grp_info_g4 =
{
  .grp=&flare32_enc_info_grp_16,
  .grp_value=FLARE32_G4_GRP_VALUE,

  .subgrp=NULL,
  .subgrp_value=0,
};
static const flare32_opc_info_t
  flare32_opc_info_g4[FLARE32_G4_OPC_INFO_LIM] =
{
  /* -------- */
  /* jl rA */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_JL_RA, FLARE32_OA_RA,
    {"jl", "jl"}},

  /* jmp rA */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_JMP_RA, FLARE32_OA_RA,
    {"jmp", "jmp"}},

  /* jmp rA, rB */
  //{&flare32_grp_info_g4,
  //  FLARE32_G4_OP_ENUM_JMP_RA_RB, FLARE32_OA_RA_RB,
  //  {"jmp", "jmp"}},

  /* jmp ira */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_JMP_IRA, FLARE32_OA_IRA,
    {"jmp", "jmp"}},

  /* reti */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_RETI, FLARE32_OA_NONE,
    {"reti", "reti"}},
  /* -------- */
  /* ei */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_EI, FLARE32_OA_NONE,
    {"ei", "ei"}},

  /* di */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_DI, FLARE32_OA_NONE,
    {"di", "di"}},

  /* push rA, rB */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_PUSH_RA_RB, FLARE32_OA_RA_RB,
    {"push", "push"}},

  /* push sA, rB */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_PUSH_SA_RB, FLARE32_OA_SA_RB,
    {"push", "push"}},
  /* -------- */
  /* pop rA, rB */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_POP_RA_RB, FLARE32_OA_RA_RB,
    {"pop", "pop"}},

  /* pop sA, rB */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_POP_SA_RB, FLARE32_OA_SA_RB,
    {"pop", "pop"}},

  /* index rA */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_INDEX_RA, FLARE32_OA_RA,
    {"index", "index"}},

  /* mul rA, rB */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_MUL_RA_RB, FLARE32_OA_RA_RB,
    {"mul", "mul"}},
  /* -------- */
  /* udiv rA, rB */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_UDIV_RA_RB, FLARE32_OA_RA_RB,
    {"udiv", "udiv"}},

  /* sdiv rA, rB */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_SDIV_RA_RB, FLARE32_OA_RA_RB,
    {"sdiv", "sdiv"}},

  /* umod rA, rB */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_UMOD_RA_RB, FLARE32_OA_RA_RB,
    {"umod", "umod"}},

  /* smod rA, rB */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_SMOD_RA_RB, FLARE32_OA_RA_RB,
    {"smod", "smod"}},

  /* -------- */

  /* lumul rA, rB */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_LUMUL_RA_RB, FLARE32_OA_RA_RB,
    {"lumul", "lumul"}},

  /* lsmul rA, rB */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_LSMUL_RA_RB, FLARE32_OA_RA_RB,
    {"lsmul", "lsmul"}},

  /* ludiv rA, rB */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_LUDIV_RA_RB, FLARE32_OA_RA_RB,
    {"ludiv", "ludiv"}},

  /* lsdiv rA, rB */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_LSDIV_RA_RB, FLARE32_OA_RA_RB,
    {"lsdiv", "lsdiv"}},
  /* -------- */
  /* lumod rA, rB */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_LUMOD_RA_RB, FLARE32_OA_RA_RB,
    {"lumod", "lumod"}},
  /* lsmod rA, rB */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_LSMOD_RA_RB, FLARE32_OA_RA_RB,
    {"lsmod", "lsmod"}},

  /* ldub rA, [rB] */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_LDUB_RA_RB, FLARE32_OA_RA_RB_LDST,
    {"ldub", "ldub"}},

  /* ldsb rA, [rB] */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_LDSB_RA_RB, FLARE32_OA_RA_RB_LDST,
    {"ldsb", "ldsb"}},
  /* -------- */
  /* lduh rA, [rB] */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_LDUH_RA_RB, FLARE32_OA_RA_RB_LDST,
    {"lduh", "lduh"}},

  /* ldsh rA, [rB] */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_LDSH_RA_RB, FLARE32_OA_RA_RB_LDST,
    {"ldsh", "ldsh"}},

  /* stb rA, [rB] */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_STB_RA_RB, FLARE32_OA_RA_RB_LDST,
    {"stb", "stb"}},

  /* sth rA, [rB] */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_STH_RA_RB, FLARE32_OA_RA_RB_LDST,
    {"sth", "sth"}},
  /* -------- */
  /* cpy rA, sB */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_CPY_RA_SB, FLARE32_OA_RA_SB,
    {"cpy", "cpy"}},

  /* cpy sA, rB */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_CPY_SA_RB, FLARE32_OA_SA_RB,
    {"cpy", "cpy"}},

  /* cpy sA, sB */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_CPY_SA_SB, FLARE32_OA_SA_SB,
    {"cpy", "cpy"}},

  /* reserved */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_RESERVED_31, FLARE32_OA_BAD,
    {"bad", "bad"}},
  /* -------- */
  /* Following are some pseudo instructions. */
  /* push rA
    Note: implicit `sp` */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_PUSH_RA_RB, FLARE32_OA_RA_IMPLICIT_SP,
    {"push", "push"}},

  /* pop rA
    Note: implicit `sp` */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_POP_RA_RB, FLARE32_OA_RA_IMPLICIT_SP,
    {"pop", "pop"}},

  /* push sA
    Note: implicit `sp` */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_PUSH_SA_RB, FLARE32_OA_SA_IMPLICIT_SP,
    {"push", "push"}},

  /* pop sA
    Note: implicit `sp` */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_POP_SA_RB, FLARE32_OA_SA_IMPLICIT_SP,
    {"pop", "pop"}},
  /* -------- */
  /* ldub rA, [rB, rC] */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_LDUB_RA_RB, FLARE32_OA_RA_RB_RC_LDST,
    {"ldub", "ldub"}},

  /* ldsb rA, [rB, rC] */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_LDSB_RA_RB, FLARE32_OA_RA_RB_RC_LDST,
    {"ldsb", "ldsb"}},

  /* lduh rA, [rB, rC] */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_LDUH_RA_RB, FLARE32_OA_RA_RB_RC_LDST,
    {"lduh", "lduh"}},

  /* ldsh rA, [rB, rC] */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_LDSH_RA_RB, FLARE32_OA_RA_RB_RC_LDST,
    {"ldsh", "ldsh"}},
  /* -------- */
  /* stb rA, [rB, rC] */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_STB_RA_RB, FLARE32_OA_RA_RB_RC_LDST,
    {"stb", "stb"}},

  /* sth rA, [rB, rC] */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_STH_RA_RB, FLARE32_OA_RA_RB_RC_LDST,
    {"sth", "sth"}},
  /* -------- */
};
static const flare32_grp_info_t flare32_grp_info_g5 =
{
  .grp=&flare32_enc_info_grp_16,
  .grp_value=FLARE32_G5_GRP_VALUE,

  .subgrp=NULL,
  .subgrp_value=0,
};
static const flare32_opc_info_t
  flare32_opc_info_g5[FLARE32_G5_OPC_INFO_LIM] =
{
  /* -------- */
  /* ldr rA, [rB, #simm] */
  {&flare32_grp_info_g5,
    FLARE32_OPC_INFO_NULL_OP, FLARE32_OA_RA_RB_S5_LDST,
    {"ldr", "ldr"}},
  /* -------- */
  /* Following are some pseudo instructions */
  /* ldr rA, [rB] */
  {&flare32_grp_info_g5,
    FLARE32_OPC_INFO_NULL_OP, FLARE32_OA_RA_RB_LDST,
    {"ldr", "ldr"}},

  /* ldr rA, [rB, rC] */
  {&flare32_grp_info_g5,
    FLARE32_OPC_INFO_NULL_OP, FLARE32_OA_RA_RB_RC_LDST,
    {"ldr", "ldr"}},

  /* ldr rA, [rB, rC, #simm] */
  {&flare32_grp_info_g5,
    FLARE32_OPC_INFO_NULL_OP, FLARE32_OA_RA_RB_RC_S5_LDST,
    {"ldr", "ldr"}},
  /* -------- */
};

static const flare32_grp_info_t flare32_grp_info_g6 =
{
  .grp=&flare32_enc_info_grp_16,
  .grp_value=FLARE32_G6_GRP_VALUE,

  .subgrp=NULL,
  .subgrp_value=0,
};
static const flare32_opc_info_t
  flare32_opc_info_g6[FLARE32_G6_OPC_INFO_LIM] =
{
  /* -------- */
  /* str rA, [rB, #simm] */
  {&flare32_grp_info_g6,
    FLARE32_OPC_INFO_NULL_OP, FLARE32_OA_RA_RB_S5_LDST,
    {"str", "str"}},
  /* -------- */
  /* Following are some pseudo instructions */
  /* st rA, [rB] */
  {&flare32_grp_info_g6,
    FLARE32_OPC_INFO_NULL_OP, FLARE32_OA_RA_RB_LDST,
    {"str", "str"}},

  /* str rA, [rB, rC] */
  {&flare32_grp_info_g6,
    FLARE32_OPC_INFO_NULL_OP, FLARE32_OA_RA_RB_RC_LDST,
    {"str", "str"}},

  /* str rA, [rB, rC, #simm] */
  {&flare32_grp_info_g6,
    FLARE32_OPC_INFO_NULL_OP, FLARE32_OA_RA_RB_RC_S5_LDST,
    {"str", "str"}},
  /* -------- */
};

static const flare32_grp_info_t flare32_grp_info_g7_aluopbh =
{
  .grp=&flare32_enc_info_grp_16,
  .grp_value=FLARE32_G7_GRP_VALUE,

  .subgrp=&flare32_enc_info_g7_aluopbh_subgrp,
  .subgrp_value=FLARE32_G7_ALUOPBH_SUBGRP_VALUE,
};
static const flare32_opc_info_t
  flare32_opc_info_g7_aluopbh[FLARE32_G7_ALUOPBH_OPC_INFO_LIM] =
{
  /* -------- */
  /* cmpb rA, rB
    cmph rA, rB */
  {&flare32_grp_info_g7_aluopbh,
    FLARE32_G7_ALUOPBH_OP_ENUM_CMP_RA_RB, FLARE32_OA_RA_RB,
    {"cmpb", "cmph"}},

  /* lsrb rA, rB
    lsrh rA, rB */
  {&flare32_grp_info_g7_aluopbh,
    FLARE32_G7_ALUOPBH_OP_ENUM_LSR_RA_RB, FLARE32_OA_RA_RB,
    {"lsrb", "lsrh"}},

  /* asrb rA, rB
    asrh rA, rB */
  {&flare32_grp_info_g7_aluopbh,
    FLARE32_G7_ALUOPBH_OP_ENUM_ASR_RA_RB, FLARE32_OA_RA_RB,
    {"asrb", "asrh"}},

  /* reserved */
  {&flare32_grp_info_g7_aluopbh,
    FLARE32_G7_ALUOPBH_OP_ENUM_RESERVED_3, FLARE32_OA_BAD,
    {"bad", "bad"}},
  /* -------- */
};

static const flare32_grp_info_t flare32_grp_info_g7_sprldst =
{
  .grp=&flare32_enc_info_grp_16,
  .grp_value=FLARE32_G7_GRP_VALUE,

  .subgrp=&flare32_enc_info_g7_sprldst_subgrp,
  .subgrp_value=FLARE32_G7_SPRLDST_SUBGRP_VALUE,
};
static const flare32_opc_info_t
  flare32_opc_info_g7_sprldst[FLARE32_G7_SPRLDST_OPC_INFO_LIM] =
{
  /* -------- */
  /* ldr sA, [rB] */
  {&flare32_grp_info_g7_sprldst,
    FLARE32_G7_SPRLDST_OP_ENUM_LDR_SA_RB, FLARE32_OA_SA_RB_LDST,
    {"ldr", "ldr"}},

  /* ldr sA, [sB] */
  {&flare32_grp_info_g7_sprldst,
    FLARE32_G7_SPRLDST_OP_ENUM_LDR_SA_SB, FLARE32_OA_SA_SB_LDST,
    {"ldr", "ldr"}},

  /* str sA, [rB] */
  {&flare32_grp_info_g7_sprldst,
    FLARE32_G7_SPRLDST_OP_ENUM_STR_SA_RB, FLARE32_OA_SA_RB_LDST,
    {"str", "str"}},

  /* str sA, [sB] */
  {&flare32_grp_info_g7_sprldst,
    FLARE32_G7_SPRLDST_OP_ENUM_STR_SA_SB, FLARE32_OA_SA_SB_LDST,
    {"str", "str"}},
  /* -------- */
};
/* -------- */
#endif    // _FLARE32_OPC_DECLS_H_

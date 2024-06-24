/* flare-opc-decls.h -- Definitions for flare opcodes.
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

#ifndef _FLARE_OPC_DECLS_H_
#define _FLARE_OPC_DECLS_H_

//static inline flare_enc_info_t
//flare_build_enc_info (flare_temp_t bitsize, flare_temp_t bitpos)
//{
//  flare_enc_info_t ret;
//
//  ret.bitsize = bitsize;
//  ret.bitpos = bitpos;
//  ret.rsmask = FLARE_N_ONES (ret.bitsize);
//  ret.mask = ret.rsmask << ret.bitpos;
//
//  return ret;
//}
static const flare_enc_info_t
  /* -------- */
  flare_enc_info_grp_16 =
    {FLARE_GRP_BITSIZE,
    FLARE_GRP_16_BITPOS,
    FLARE_GRP_RSMASK,
    FLARE_GRP_16_MASK},
  flare_enc_info_grp_32 = 
    {FLARE_GRP_BITSIZE,
    FLARE_GRP_32_BITPOS,
    FLARE_GRP_RSMASK,
    FLARE_GRP_32_MASK},
  /* -------- */
  flare_enc_info_g0_pre_subgrp =
    {FLARE_G0_PRE_SUBGRP_BITSIZE, 
    FLARE_G0_PRE_SUBGRP_BITPOS,
    FLARE_G0_PRE_SUBGRP_RSMASK,
    FLARE_G0_PRE_SUBGRP_MASK},

  flare_enc_info_g0_pre_fullgrp =
    {FLARE_G0_PRE_FULLGRP_BITSIZE,
    FLARE_G0_PRE_FULLGRP_BITPOS,
    FLARE_G0_PRE_FULLGRP_RSMASK,
    FLARE_G0_PRE_FULLGRP_MASK},
  flare_enc_info_g0_pre_s12 =
    {FLARE_G0_PRE_S12_BITSIZE,
    FLARE_G0_PRE_S12_BITPOS,
    FLARE_G0_PRE_S12_RSMASK,
    FLARE_G0_PRE_S12_MASK},
  /* -------- */
  flare_enc_info_g0_lpre_subgrp =
    {FLARE_G0_LPRE_SUBGRP_BITSIZE,
    FLARE_G0_LPRE_SUBGRP_BITPOS,
    FLARE_G0_LPRE_SUBGRP_RSMASK,
    FLARE_G0_LPRE_SUBGRP_MASK},
  flare_enc_info_g0_lpre_fullgrp =
    {FLARE_G0_LPRE_FULLGRP_BITSIZE,
    FLARE_G0_LPRE_FULLGRP_BITPOS,
    FLARE_G0_LPRE_FULLGRP_RSMASK,
    FLARE_G0_LPRE_FULLGRP_MASK},
  flare_enc_info_g0_lpre_subgrp_16 =
    {FLARE_G0_LPRE_SUBGRP_BITSIZE,
    FLARE_G0_LPRE_SUBGRP_16_BITPOS,
    FLARE_G0_LPRE_SUBGRP_RSMASK,
    FLARE_G0_LPRE_SUBGRP_16_MASK},
  flare_enc_info_g0_lpre_fullgrp_16 =
    {FLARE_G0_LPRE_FULLGRP_BITSIZE,
    FLARE_G0_LPRE_FULLGRP_16_BITPOS,
    FLARE_G0_LPRE_FULLGRP_RSMASK,
    FLARE_G0_LPRE_FULLGRP_16_MASK},

  flare_enc_info_g0_lpre_s27 =
    {FLARE_G0_LPRE_S27_BITSIZE,
    FLARE_G0_LPRE_S27_BITPOS,
    FLARE_G0_LPRE_S27_RSMASK,
    FLARE_G0_LPRE_S27_MASK},
  flare_enc_info_g0_lpre_s23 =
    {FLARE_G0_LPRE_S23_BITSIZE,
    FLARE_G0_LPRE_S23_BITPOS,
    FLARE_G0_LPRE_S23_RSMASK,
    FLARE_G0_LPRE_S23_MASK},
  /* -------- */
  flare_enc_info_g1g5g6_i5 =
    {FLARE_G1G5G6_I5_BITSIZE,
    FLARE_G1G5G6_I5_BITPOS,
    FLARE_G1G5G6_I5_RSMASK,
    FLARE_G1G5G6_I5_MASK},
  flare_enc_info_g1_op =
    {FLARE_G1_OP_BITSIZE,
    FLARE_G1_OP_BITPOS,
    FLARE_G1_OP_RSMASK,
    FLARE_G1_OP_MASK},
  flare_enc_info_g2_f =
    {FLARE_G2_F_BITSIZE,
    FLARE_G2_F_BITPOS,
    FLARE_G2_F_RSMASK,
    FLARE_G2_F_MASK},
  flare_enc_info_g2_op =
    {FLARE_G2_OP_BITSIZE,
    FLARE_G2_OP_BITPOS,
    FLARE_G2_OP_RSMASK,
    FLARE_G2_OP_MASK},
  flare_enc_info_g3_s9 =
    {FLARE_G3_S9_BITSIZE,
    FLARE_G3_S9_BITPOS,
    FLARE_G3_S9_RSMASK,
    FLARE_G3_S9_MASK},
  flare_enc_info_g3_op =
    {FLARE_G3_OP_BITSIZE,
    FLARE_G3_OP_BITPOS,
    FLARE_G3_OP_RSMASK,
    FLARE_G3_OP_MASK},
  flare_enc_info_g4_op =
    {FLARE_G4_OP_BITSIZE,
    FLARE_G4_OP_BITPOS,
    FLARE_G4_OP_RSMASK,
    FLARE_G4_OP_MASK},
  /* -------- */
  flare_enc_info_g7_aluopbh_subgrp =
    {FLARE_G7_ALUOPBH_SUBGRP_BITSIZE,
    FLARE_G7_ALUOPBH_SUBGRP_BITPOS,
    FLARE_G7_ALUOPBH_SUBGRP_RSMASK,
    FLARE_G7_ALUOPBH_SUBGRP_MASK},
  flare_enc_info_g7_aluopbh_fullgrp =
    {FLARE_G7_ALUOPBH_FULLGRP_BITSIZE,
    FLARE_G7_ALUOPBH_FULLGRP_BITPOS,
    FLARE_G7_ALUOPBH_FULLGRP_RSMASK,
    FLARE_G7_ALUOPBH_FULLGRP_MASK},
  flare_enc_info_g7_aluopbh_w =
    {FLARE_G7_ALUOPBH_W_BITSIZE,
    FLARE_G7_ALUOPBH_W_BITPOS,
    FLARE_G7_ALUOPBH_W_RSMASK,
    FLARE_G7_ALUOPBH_W_MASK},
  flare_enc_info_g7_aluopbh_op =
    {FLARE_G7_ALUOPBH_OP_BITSIZE,
    FLARE_G7_ALUOPBH_OP_BITPOS,
    FLARE_G7_ALUOPBH_OP_RSMASK,
    FLARE_G7_ALUOPBH_OP_MASK},

  flare_enc_info_g7_sprldst_subgrp =
    {FLARE_G7_SPRLDST_SUBGRP_BITSIZE,
    FLARE_G7_SPRLDST_SUBGRP_BITPOS,
    FLARE_G7_SPRLDST_SUBGRP_RSMASK,
    FLARE_G7_SPRLDST_SUBGRP_MASK},
  flare_enc_info_g7_sprldst_fullgrp =
    {FLARE_G7_SPRLDST_FULLGRP_BITSIZE,
    FLARE_G7_SPRLDST_FULLGRP_BITPOS,
    FLARE_G7_SPRLDST_FULLGRP_RSMASK,
    FLARE_G7_SPRLDST_FULLGRP_MASK},
  flare_enc_info_g7_sprldst_op =
    {FLARE_G7_SPRLDST_OP_BITSIZE,
    FLARE_G7_SPRLDST_OP_BITPOS,
    FLARE_G7_SPRLDST_OP_RSMASK,
    FLARE_G7_SPRLDST_OP_MASK},

  flare_enc_info_g7_icreload_subgrp =
    {FLARE_G7_ICRELOAD_SUBGRP_BITSIZE,
    FLARE_G7_ICRELOAD_SUBGRP_BITPOS,
    FLARE_G7_ICRELOAD_SUBGRP_RSMASK,
    FLARE_G7_ICRELOAD_SUBGRP_MASK},
  flare_enc_info_g7_icreload_fullgrp =
    {FLARE_G7_ICRELOAD_FULLGRP_BITSIZE,
    FLARE_G7_ICRELOAD_FULLGRP_BITPOS,
    FLARE_G7_ICRELOAD_FULLGRP_RSMASK,
    FLARE_G7_ICRELOAD_FULLGRP_MASK},
  flare_enc_info_g7_icreload_s5 =
    {FLARE_G7_ICRELOAD_S5_BITSIZE,
    FLARE_G7_ICRELOAD_S5_BITPOS,
    FLARE_G7_ICRELOAD_S5_RSMASK,
    FLARE_G7_ICRELOAD_S5_MASK},
  /* -------- */
  flare_enc_info_ra_ind =
    {FLARE_RA_IND_BITSIZE,
    FLARE_RA_IND_BITPOS,
    FLARE_RA_IND_RSMASK,
    FLARE_RA_IND_MASK},
  flare_enc_info_rb_ind =
    {FLARE_RB_IND_BITSIZE,
    FLARE_RB_IND_BITPOS,
    FLARE_RB_IND_RSMASK,
    FLARE_RB_IND_MASK};
  /* -------- */
/* -------- */
/* -------- */
static const flare_grp_info_t flare_grp_info_g0_pre =
{
  .grp=&flare_enc_info_grp_16,
  .grp_value=FLARE_G0_GRP_VALUE,

  .subgrp=&flare_enc_info_g0_pre_subgrp,
  .subgrp_value=FLARE_G0_PRE_SUBGRP_VALUE,
};
static const flare_grp_info_t flare_grp_info_g0_lpre =
{
  .grp=&flare_enc_info_grp_32,
  .grp_value=FLARE_G0_GRP_VALUE,

  .subgrp=&flare_enc_info_g0_lpre_subgrp,
  .subgrp_value=FLARE_G0_LPRE_SUBGRP_VALUE,
};
static const flare_opc_info_t
  flare_opc_info_g0[FLARE_G0_OPC_INFO_LIM] =
{
  /* pre #simm */
  {&flare_grp_info_g0_pre,
    FLARE_OPC_INFO_NULL_OP, FLARE_OA_PRE,
    {"pre", "pre"}, {"pre.nr", "pre.nr"}},

  /* lpre #simm */
  {&flare_grp_info_g0_lpre,
    FLARE_OPC_INFO_NULL_OP, FLARE_OA_LPRE,
    {"lpre", "lpre"}, {"lpre.nr", "lpre.nr"}},
};

static const flare_grp_info_t flare_grp_info_g1 =
{
  .grp=&flare_enc_info_grp_16,
  .grp_value=FLARE_G1_GRP_VALUE,

  .subgrp=NULL,
  .subgrp_value=0,
};
static const flare_opc_info_t
  flare_opc_info_g1[FLARE_G1_OPC_INFO_LIM] =
{
  /* -------- */
  /* add rA, #simm */
  {&flare_grp_info_g1,
    FLARE_G1_OP_ENUM_ADD_RA_S5, FLARE_OA_RA_S5,
    {"add", "add"}, {"add.nr", "add.nr"}},

  /* add rA, pc, #simm */
  {&flare_grp_info_g1,
    FLARE_G1_OP_ENUM_ADD_RA_PC_S5, FLARE_OA_RA_PC_S5,
    {"add", "add"}, {"add.nr", "add.nr"}},

  /* add rA, sp, #simm */
  {&flare_grp_info_g1,
    FLARE_G1_OP_ENUM_ADD_RA_SP_S5, FLARE_OA_RA_SP_S5,
    {"add", "add"}, {"add.nr", "add.nr"}},

  /* add rA, fp, #simm */
  {&flare_grp_info_g1,
    FLARE_G1_OP_ENUM_ADD_RA_FP_S5, FLARE_OA_RA_FP_S5,
    {"add", "add"}, {"add.nr", "add.nr"}},
  /* -------- */
  /* cmp rA, #simm */
  {&flare_grp_info_g1,
    FLARE_G1_OP_ENUM_CMP_RA_S5, FLARE_OA_RA_S5,
    {"cmp", "cmp"}, {"cmp.nr", "cmp.nr"}},

  /* cpy rA, #simm */
  {&flare_grp_info_g1,
    FLARE_G1_OP_ENUM_CPY_RA_S5, FLARE_OA_RA_S5,
    {"cpy", "cpy"}, {"cpy.nr", "cpy.nr"}},

  /* lsl rA, #imm */
  {&flare_grp_info_g1,
    FLARE_G1_OP_ENUM_LSL_RA_U5, FLARE_OA_RA_U5,
    {"lsl", "lsl"}, {"lsl.nr", "lsl.nr"}},

  /* lsr rA, #imm */
  {&flare_grp_info_g1,
    FLARE_G1_OP_ENUM_LSR_RA_U5, FLARE_OA_RA_U5,
    {"lsr", "lsr"}, {"lsr.nr", "lsr.nr"}},
  /* -------- */
  /* asr rA, #imm */
  {&flare_grp_info_g1,
    FLARE_G1_OP_ENUM_ASR_RA_U5, FLARE_OA_RA_U5,
    {"asr", "asr"}, {"asr.nr", "asr.nr"}},

  /* and rA, #simm */
  {&flare_grp_info_g1,
    FLARE_G1_OP_ENUM_AND_RA_S5, FLARE_OA_RA_S5,
    {"and", "and"}, {"and.nr", "and.nr"}},

  /* orr rA, #simm */
  {&flare_grp_info_g1,
    FLARE_G1_OP_ENUM_ORR_RA_S5, FLARE_OA_RA_S5,
    {"orr", "orr"}, {"orr.nr", "orr.nr"}},

  /* xor rA, #simm */
  {&flare_grp_info_g1,
    FLARE_G1_OP_ENUM_XOR_RA_S5, FLARE_OA_RA_S5,
    {"xor", "xor"}, {"xor.nr", "xor.nr"}},
  /* -------- */
  /* ze rA, #imm */
  {&flare_grp_info_g1,
  FLARE_G1_OP_ENUM_ZE_RA_U5, FLARE_OA_RA_U5,
    {"ze", "ze"}, {"ze.nr", "ze.nr"}},

  /* se rA, #imm */
  {&flare_grp_info_g1,
  FLARE_G1_OP_ENUM_SE_RA_U5, FLARE_OA_RA_U5,
    {"se", "se"}, {"se.nr", "se.nr"}},

  /* swi rA, #simm */
  {&flare_grp_info_g1,
  FLARE_G1_OP_ENUM_SWI_RA_S5, FLARE_OA_RA_S5,
    {"swi", "swi"}, {"swi.nr", "swi.nr"}},

  /* swi #imm */
  {&flare_grp_info_g1,
  FLARE_G1_OP_ENUM_SWI_U5, FLARE_OA_U5,
    {"swi", "swi"}, {"swi.nr", "swi.nr"}},
  /* -------- */
  /* Pseudo instructions start here */
  /* cpydf rA, rB, #const_double */
  {&flare_grp_info_g1,
  FLARE_G1_OP_ENUM_CPY_RA_S5, FLARE_OA_RA_RB_CPY64,
    {"cpy64", "cpy64"}, {"cpy64.nr", "cpy64.nr"}},
  /* -------- */
};

static const flare_grp_info_t flare_grp_info_g2 =
{
  .grp=&flare_enc_info_grp_16,
  .grp_value=FLARE_G2_GRP_VALUE,

  .subgrp=NULL,
  .subgrp_value=0,
};
static const flare_opc_info_t
  flare_opc_info_g2[FLARE_G2_OPC_INFO_LIM] =
{
  /* -------- */
  /* add rA, rB */
  {&flare_grp_info_g2,
    FLARE_G2_OP_ENUM_ADD_RA_RB, FLARE_OA_RA_RB,
    {"add", "add.f"}, {"add.nr", "add.f.nr"}},

  /* sub rA, rB */
  {&flare_grp_info_g2,
    FLARE_G2_OP_ENUM_SUB_RA_RB, FLARE_OA_RA_RB,
    {"sub", "sub.f"}, {"sub.nr", "sub.f.nr"}},

  /* add rA, sp, rB */
  {&flare_grp_info_g2,
    FLARE_G2_OP_ENUM_ADD_RA_SP_RB, FLARE_OA_RA_SP_RB, 
    {"add", "add.f"}, {"add.nr", "add.f.nr"}},

  /* add rA, fp, rB */
  {&flare_grp_info_g2,
    FLARE_G2_OP_ENUM_ADD_RA_FP_RB, FLARE_OA_RA_FP_RB,
    {"add", "add.f"}, {"add.nr", "add.f.nr"}},
  /* -------- */
  /* cmp rA, rB */
  {&flare_grp_info_g2,
    FLARE_G2_OP_ENUM_CMP_RA_RB, FLARE_OA_RA_RB,
    {"cmp", "cmp"}, {"cmp.nr", "cmp.nr"}},

  /* cpy rA, rB */
  {&flare_grp_info_g2,
    FLARE_G2_OP_ENUM_CPY_RA_RB, FLARE_OA_RA_RB,
    {"cpy", "cpy.f"}, {"cpy.nr", "cpy.f.nr"}},

  /* lsl rA, rB */
  {&flare_grp_info_g2,
    FLARE_G2_OP_ENUM_LSL_RA_RB, FLARE_OA_RA_RB,
    {"lsl", "lsl.f"}, {"lsl.nr", "lsl.f.nr"}},

  /* lsr rA, rB */
  {&flare_grp_info_g2,
    FLARE_G2_OP_ENUM_LSR_RA_RB, FLARE_OA_RA_RB,
    {"lsr", "lsr.f"}, {"lsr.nr", "lsr.f.nr"}},
  /* -------- */
  /* asr rA, rB */
  {&flare_grp_info_g2,
    FLARE_G2_OP_ENUM_ASR_RA_RB, FLARE_OA_RA_RB, 
    {"asr", "asr.f"}, {"asr.nr", "asr.f.nr"}},

  /* and rA, rB */
  {&flare_grp_info_g2,
    FLARE_G2_OP_ENUM_AND_RA_RB, FLARE_OA_RA_RB,
    {"and", "and.f"}, {"and.nr", "and.f.nr"}},

  /* orr rA, rB */
  {&flare_grp_info_g2,
    FLARE_G2_OP_ENUM_ORR_RA_RB, FLARE_OA_RA_RB,
    {"orr", "orr.f"}, {"orr.nr", "orr.f.nr"}},

  /* xor rA, rB */
  {&flare_grp_info_g2,
    FLARE_G2_OP_ENUM_XOR_RA_RB, FLARE_OA_RA_RB,
    {"xor", "xor.f"}, {"xor.nr", "xor.f.nr"}},
  /* -------- */
  /* adc rA, rB  */
  {&flare_grp_info_g2,
    FLARE_G2_OP_ENUM_ADC_RA_RB, FLARE_OA_RA_RB,
    {"adc", "adc.f"}, {"adc.nr", "adc.f.nr"}},

  /* sbc rA, rB */
  {&flare_grp_info_g2,
    FLARE_G2_OP_ENUM_SBC_RA_RB, FLARE_OA_RA_RB,
    {"sbc", "sbc.f"}, {"sbc.nr", "sbc.f.nr"}},

  /* cmpbc rA, rB */
  {&flare_grp_info_g2,
    FLARE_G2_OP_ENUM_CMPBC_RA_RB, FLARE_OA_RA_RB,
    {"cmpbc", "cmpbc.f"}, {"cmpbc.nr", "cmpbc.f.nr"}},

  /* reserved */
  {&flare_grp_info_g2,
    FLARE_G2_OP_ENUM_RESERVED_15, FLARE_OA_BAD,
    {"bad", "bad.f"}, {"bad.nr", "bad.f.nr"}},
  /* -------- */
};

static const flare_grp_info_t flare_grp_info_g3 =
{
  .grp=&flare_enc_info_grp_16,
  .grp_value=FLARE_G3_GRP_VALUE,

  .subgrp=NULL,
  .subgrp_value=0,
};
static const flare_opc_info_t
  flare_opc_info_g3[FLARE_G3_OPC_INFO_LIM] =
{
  /* -------- */
  /* bl #simm */
  {&flare_grp_info_g3,
    FLARE_G3_OP_ENUM_BL_PCREL_S9, FLARE_OA_PCREL_S9,
    {"bl", "bl"}, {"bl.nr", "bl.nr"}},

  /* bra #simm */
  {&flare_grp_info_g3,
    FLARE_G3_OP_ENUM_BRA_PCREL_S9, FLARE_OA_PCREL_S9,
    {"bra", "bra"}, {"bra.nr", "bra.nr"}},

  /* beq #simm */
  {&flare_grp_info_g3,
    FLARE_G3_OP_ENUM_BEQ_PCREL_S9, FLARE_OA_PCREL_S9,
    {"beq", "beq"}, {"beq.nr", "beq.nr"}},

  /* bne #simm */
  {&flare_grp_info_g3,
    FLARE_G3_OP_ENUM_BNE_PCREL_S9, FLARE_OA_PCREL_S9,
    {"bne", "bne"}, {"bne.nr", "bne.nr"}},
  /* -------- */
  /* bmi #simm */
  {&flare_grp_info_g3,
    FLARE_G3_OP_ENUM_BMI_PCREL_S9, FLARE_OA_PCREL_S9,
    {"bmi", "bmi"}, {"bmi.nr", "bmi.nr"}},

  /* bpl #simm */
  {&flare_grp_info_g3,
    FLARE_G3_OP_ENUM_BPL_PCREL_S9, FLARE_OA_PCREL_S9,
    {"bpl", "bpl"}, {"bpl.nr", "bpl.nr"}},

  /* bvs #simm */
  {&flare_grp_info_g3,
    FLARE_G3_OP_ENUM_BVS_PCREL_S9, FLARE_OA_PCREL_S9,
    {"bvs", "bvs"}, {"bvs.nr", "bvs.nr"}},

  /* bvc simm */
  {&flare_grp_info_g3,
    FLARE_G3_OP_ENUM_BVC_PCREL_S9, FLARE_OA_PCREL_S9,
    {"bvc", "bvc"}, {"bvc.nr", "bvc.nr"}},
  /* -------- */
  /* bgeu #simm */
  {&flare_grp_info_g3,
    FLARE_G3_OP_ENUM_BGEU_PCREL_S9, FLARE_OA_PCREL_S9,
    {"bgeu", "bgeu"}, {"bgeu.nr", "bgeu.nr"}},

  /* bltu #simm */
  {&flare_grp_info_g3,
    FLARE_G3_OP_ENUM_BLTU_PCREL_S9, FLARE_OA_PCREL_S9,
    {"bltu", "bltu"}, {"bltu.nr", "bltu.nr"}},

  /* bgtu #simm */
  {&flare_grp_info_g3,
    FLARE_G3_OP_ENUM_BGTU_PCREL_S9, FLARE_OA_PCREL_S9,
    {"bgtu", "bgtu"}, {"bgtu.nr", "bgtu.nr"}},

  /* bleu #simm */
  {&flare_grp_info_g3,
    FLARE_G3_OP_ENUM_BLEU_PCREL_S9, FLARE_OA_PCREL_S9,
    {"bleu", "bleu"}, {"bleu.nr", "bleu.nr"}},
  /* -------- */
  /* bges #simm */
  {&flare_grp_info_g3,
    FLARE_G3_OP_ENUM_BGES_PCREL_S9, FLARE_OA_PCREL_S9,
    {"bges", "bges"}, {"bges.nr", "bges.nr"}},

  /* blts #simm */
  {&flare_grp_info_g3,
    FLARE_G3_OP_ENUM_BLTS_PCREL_S9, FLARE_OA_PCREL_S9,
    {"blts", "blts"}, {"blts.nr", "blts.nr"}},

  /* bgts #simm */
  {&flare_grp_info_g3,
    FLARE_G3_OP_ENUM_BGTS_PCREL_S9, FLARE_OA_PCREL_S9,
    {"bgts", "bgts"}, {"bgts.nr", "bgts.nr"}},

  /* bles #simm */
  {&flare_grp_info_g3,
    FLARE_G3_OP_ENUM_BLES_PCREL_S9, FLARE_OA_PCREL_S9,
    {"bles", "bles"}, {"bles.nr", "bles.nr"}},
  /* -------- */
};
static const flare_grp_info_t flare_grp_info_g4 =
{
  .grp=&flare_enc_info_grp_16,
  .grp_value=FLARE_G4_GRP_VALUE,

  .subgrp=NULL,
  .subgrp_value=0,
};
static const flare_opc_info_t
  flare_opc_info_g4[FLARE_G4_OPC_INFO_LIM] =
{
  /* -------- */
  /* jl rA */
  {&flare_grp_info_g4,
    FLARE_G4_OP_ENUM_JL_RA, FLARE_OA_RA,
    {"jl", "jl"}, {"jl.nr", "jl.nr"}},

  /* jmp rA */
  {&flare_grp_info_g4,
    FLARE_G4_OP_ENUM_JMP_RA, FLARE_OA_RA,
    {"jmp", "jmp"}, {"jmp.nr", "jmp.nr"}},

  /* jmp rA, rB */
  //{&flare_grp_info_g4,
  //  FLARE_G4_OP_ENUM_JMP_RA_RB, FLARE_OA_RA_RB,
  //  {"jmp", "jmp"}, {"jmp.nr", "jmp.nr"}},

  /* jmp ira */
  {&flare_grp_info_g4,
    FLARE_G4_OP_ENUM_JMP_IRA, FLARE_OA_IRA,
    {"jmp", "jmp"}, {"jmp.nr", "jmp.nr"}},

  /* reti */
  {&flare_grp_info_g4,
    FLARE_G4_OP_ENUM_RETI, FLARE_OA_NONE,
    {"reti", "reti"}, {"reti.nr", "reti.nr"}},
  /* -------- */
  /* ei */
  {&flare_grp_info_g4,
    FLARE_G4_OP_ENUM_EI, FLARE_OA_NONE,
    {"ei", "ei"}, {"ei.nr", "ei.nr"}},

  /* di */
  {&flare_grp_info_g4,
    FLARE_G4_OP_ENUM_DI, FLARE_OA_NONE,
    {"di", "di"}, {"di.nr", "di.nr"}},

  /* push rA, rB */
  {&flare_grp_info_g4,
    FLARE_G4_OP_ENUM_PUSH_RA_RB, FLARE_OA_RA_RB,
    {"push", "push"}, {"push.nr", "push.nr"}},

  /* push sA, rB */
  {&flare_grp_info_g4,
    FLARE_G4_OP_ENUM_PUSH_SA_RB, FLARE_OA_SA_RB,
    {"push", "push"}, {"push.nr", "push.nr"}},
  /* -------- */
  /* pop rA, rB */
  {&flare_grp_info_g4,
    FLARE_G4_OP_ENUM_POP_RA_RB, FLARE_OA_RA_RB,
    {"pop", "pop"}, {"pop.nr", "pop.nr"}},

  /* pop sA, rB */
  {&flare_grp_info_g4,
    FLARE_G4_OP_ENUM_POP_SA_RB, FLARE_OA_SA_RB,
    {"pop", "pop"}, {"pop.nr", "pop.nr"}},

  /* pop pc, rB */
  {&flare_grp_info_g4,
    FLARE_G4_OP_ENUM_POP_PC_RB, FLARE_OA_PC_RB,
    {"pop", "pop"}, {"pop.nr", "pop.nr"}},

  /* mul rA, rB */
  {&flare_grp_info_g4,
    FLARE_G4_OP_ENUM_MUL_RA_RB, FLARE_OA_RA_RB,
    {"mul", "mul"}, {"mul.nr", "mul.nr"}},
  /* -------- */
  /* udiv rA, rB */
  {&flare_grp_info_g4,
    FLARE_G4_OP_ENUM_UDIV_RA_RB, FLARE_OA_RA_RB,
    {"udiv", "udiv"}, {"udiv.nr", "udiv.nr"}},

  /* sdiv rA, rB */
  {&flare_grp_info_g4,
    FLARE_G4_OP_ENUM_SDIV_RA_RB, FLARE_OA_RA_RB,
    {"sdiv", "sdiv"}, {"sdiv.nr", "sdiv.nr"}},

  /* umod rA, rB */
  {&flare_grp_info_g4,
    FLARE_G4_OP_ENUM_UMOD_RA_RB, FLARE_OA_RA_RB,
    {"umod", "umod"}, {"umod.nr", "umod.nr"}},

  /* smod rA, rB */
  {&flare_grp_info_g4,
    FLARE_G4_OP_ENUM_SMOD_RA_RB, FLARE_OA_RA_RB,
    {"smod", "smod"}, {"smod.nr", "smod.nr"}},

  /* -------- */

  /* lumul rA, rB */
  {&flare_grp_info_g4,
    FLARE_G4_OP_ENUM_LUMUL_RA_RB, FLARE_OA_RA_RB,
    {"lumul", "lumul"}, {"lumul.nr", "lumul.nr"}},

  /* lsmul rA, rB */
  {&flare_grp_info_g4,
    FLARE_G4_OP_ENUM_LSMUL_RA_RB, FLARE_OA_RA_RB,
    {"lsmul", "lsmul"}, {"lsmul.nr", "lsmul.nr"}},

  /* udiv64 rA, rB */
  {&flare_grp_info_g4,
    FLARE_G4_OP_ENUM_UDIV64_RA_RB, FLARE_OA_RA_RB,
    {"udiv64", "udiv64"}, {"udiv64.nr", "udiv64.nr"}},

  /* sdiv64 rA, rB */
  {&flare_grp_info_g4,
    FLARE_G4_OP_ENUM_SDIV64_RA_RB, FLARE_OA_RA_RB,
    {"sdiv64", "sdiv64"}, {"sdiv64.nr", "sdiv64.nr"}},
  /* -------- */
  /* umod64 rA, rB */
  {&flare_grp_info_g4,
    FLARE_G4_OP_ENUM_UMOD64_RA_RB, FLARE_OA_RA_RB,
    {"umod64", "umod64"}, {"umod64.nr", "umod64.nr"}},

  /* smod64 rA, rB */
  {&flare_grp_info_g4,
    FLARE_G4_OP_ENUM_SMOD64_RA_RB, FLARE_OA_RA_RB,
    {"smod64", "smod64"}, {"smod64.nr", "smod64.nr"}},

  /* ldub rA, [rB] */
  {&flare_grp_info_g4,
    FLARE_G4_OP_ENUM_LDUB_RA_RB, FLARE_OA_RA_RB_LDST,
    {"ldub", "ldub"}, {"ldub.nr", "ldub.nr"}},

  /* ldsb rA, [rB] */
  {&flare_grp_info_g4,
    FLARE_G4_OP_ENUM_LDSB_RA_RB, FLARE_OA_RA_RB_LDST,
    {"ldsb", "ldsb"}, {"ldsb.nr", "ldsb.nr"}},
  /* -------- */
  /* lduh rA, [rB] */
  {&flare_grp_info_g4,
    FLARE_G4_OP_ENUM_LDUH_RA_RB, FLARE_OA_RA_RB_LDST,
    {"lduh", "lduh"}, {"lduh.nr", "lduh.nr"}},

  /* ldsh rA, [rB] */
  {&flare_grp_info_g4,
    FLARE_G4_OP_ENUM_LDSH_RA_RB, FLARE_OA_RA_RB_LDST,
    {"ldsh", "ldsh"}, {"ldsh.nr", "ldsh.nr"}},

  /* stb rA, [rB] */
  {&flare_grp_info_g4,
    FLARE_G4_OP_ENUM_STB_RA_RB, FLARE_OA_RA_RB_LDST,
    {"stb", "stb"}, {"stb.nr", "stb.nr"}},

  /* sth rA, [rB] */
  {&flare_grp_info_g4,
    FLARE_G4_OP_ENUM_STH_RA_RB, FLARE_OA_RA_RB_LDST,
    {"sth", "sth"}, {"sth.nr", "sth.nr"}},
  /* -------- */
  /* cpy rA, sB */
  {&flare_grp_info_g4,
    FLARE_G4_OP_ENUM_CPY_RA_SB, FLARE_OA_RA_SB,
    {"cpy", "cpy"}, {"cpy.nr", "cpy.nr"}},

  /* cpy sA, rB */
  {&flare_grp_info_g4,
    FLARE_G4_OP_ENUM_CPY_SA_RB, FLARE_OA_SA_RB,
    {"cpy", "cpy"}, {"cpy.nr", "cpy.nr"}},

  /* cpy sA, sB */
  {&flare_grp_info_g4,
    FLARE_G4_OP_ENUM_CPY_SA_SB, FLARE_OA_SA_SB,
    {"cpy", "cpy"}, {"cpy.nr", "cpy.nr"}},

  /* reserved */
  //{&flare_grp_info_g4,
  //  FLARE_G4_OP_ENUM_RESERVED_31, FLARE_OA_BAD,
  //  {"bad", "bad"}, {"bad.nr", "bad.nr"}},
  /* index rA */
  {&flare_grp_info_g4,
    FLARE_G4_OP_ENUM_INDEX_RA, FLARE_OA_RA,
    {"index", "index"}, {"index.nr", "index.nr"}},
  /* -------- */
  /* Following are some pseudo instructions. */
  /* push rA
    Note: implicit `sp` */
  {&flare_grp_info_g4,
    FLARE_G4_OP_ENUM_PUSH_RA_RB, FLARE_OA_RA_IMPLICIT_SP,
    {"push", "push"}, {"push.nr", "push.nr"}},

  /* pop rA
    Note: implicit `sp` */
  {&flare_grp_info_g4,
    FLARE_G4_OP_ENUM_POP_RA_RB, FLARE_OA_RA_IMPLICIT_SP,
    {"pop", "pop"}, {"pop.nr", "pop.nr"}},

  /* push sA
    Note: implicit `sp` */
  {&flare_grp_info_g4,
    FLARE_G4_OP_ENUM_PUSH_SA_RB, FLARE_OA_SA_IMPLICIT_SP,
    {"push", "push"}, {"push.nr", "push.nr"}},

  /* pop sA
    Note: implicit `sp` */
  {&flare_grp_info_g4,
    FLARE_G4_OP_ENUM_POP_SA_RB, FLARE_OA_SA_IMPLICIT_SP,
    {"pop", "pop"}, {"pop.nr", "pop.nr"}},

  /* pop pc
    Note: implicit `sp` */
  {&flare_grp_info_g4,
    FLARE_G4_OP_ENUM_POP_PC_RB, FLARE_OA_PC_IMPLICIT_SP,
    {"pop", "pop"}, {"pop.nr", "pop.nr"}},
  /* -------- */
  /* ldub rA, [rB, rC] */
  {&flare_grp_info_g4,
    FLARE_G4_OP_ENUM_LDUB_RA_RB, FLARE_OA_RA_RB_RC_LDST,
    {"ldub", "ldub"}, {"ldub.nr", "ldub.nr"}},

  /* ldsb rA, [rB, rC] */
  {&flare_grp_info_g4,
    FLARE_G4_OP_ENUM_LDSB_RA_RB, FLARE_OA_RA_RB_RC_LDST,
    {"ldsb", "ldsb"}, {"ldsb.nr", "ldsb.nr"}},

  /* lduh rA, [rB, rC] */
  {&flare_grp_info_g4,
    FLARE_G4_OP_ENUM_LDUH_RA_RB, FLARE_OA_RA_RB_RC_LDST,
    {"lduh", "lduh"}, {"lduh.nr", "lduh.nr"}},

  /* ldsh rA, [rB, rC] */
  {&flare_grp_info_g4,
    FLARE_G4_OP_ENUM_LDSH_RA_RB, FLARE_OA_RA_RB_RC_LDST,
    {"ldsh", "ldsh"}, {"ldsh.nr", "ldsh.nr"}},
  /* -------- */
  /* stb rA, [rB, rC] */
  {&flare_grp_info_g4,
    FLARE_G4_OP_ENUM_STB_RA_RB, FLARE_OA_RA_RB_RC_LDST,
    {"stb", "stb"}, {"stb.nr", "stb.nr"}},

  /* sth rA, [rB, rC] */
  {&flare_grp_info_g4,
    FLARE_G4_OP_ENUM_STH_RA_RB, FLARE_OA_RA_RB_RC_LDST,
    {"sth", "sth"}, {"sth.nr", "sth.nr"}},
  /* -------- */
};
static const flare_grp_info_t flare_grp_info_g5 =
{
  .grp=&flare_enc_info_grp_16,
  .grp_value=FLARE_G5_GRP_VALUE,

  .subgrp=NULL,
  .subgrp_value=0,
};
static const flare_opc_info_t
  flare_opc_info_g5[FLARE_G5_OPC_INFO_LIM] =
{
  /* -------- */
  /* ldr rA, [rB, #simm] */
  {&flare_grp_info_g5,
    FLARE_OPC_INFO_NULL_OP, FLARE_OA_RA_RB_S5_LDST,
    {"ldr", "ldr"}, {"ldr.nr", "ldr.nr"}},
  /* -------- */
  /* Following are some pseudo instructions */
  /* ldr rA, [rB] */
  {&flare_grp_info_g5,
    FLARE_OPC_INFO_NULL_OP, FLARE_OA_RA_RB_LDST_32,
    {"ldr", "ldr"}, {"ldr.nr", "ldr.nr"}},

  /* ldr rA, [rB, rC] */
  {&flare_grp_info_g5,
    FLARE_OPC_INFO_NULL_OP, FLARE_OA_RA_RB_RC_LDST_32,
    {"ldr", "ldr"}, {"ldr.nr", "ldr.nr"}},

  /* ldr rA, [rB, rC, #simm] */
  {&flare_grp_info_g5,
    FLARE_OPC_INFO_NULL_OP, FLARE_OA_RA_RB_RC_S5_LDST,
    {"ldr", "ldr"}, {"ldr.nr", "ldr.nr"}},
  /* -------- */
};

static const flare_grp_info_t flare_grp_info_g6 =
{
  .grp=&flare_enc_info_grp_16,
  .grp_value=FLARE_G6_GRP_VALUE,

  .subgrp=NULL,
  .subgrp_value=0,
};
static const flare_opc_info_t
  flare_opc_info_g6[FLARE_G6_OPC_INFO_LIM] =
{
  /* -------- */
  /* str rA, [rB, #simm] */
  {&flare_grp_info_g6,
    FLARE_OPC_INFO_NULL_OP, FLARE_OA_RA_RB_S5_LDST,
    {"str", "str"}, {"str.nr", "str.nr"}},
  /* -------- */
  /* Following are some pseudo instructions */
  /* st rA, [rB] */
  {&flare_grp_info_g6,
    FLARE_OPC_INFO_NULL_OP, FLARE_OA_RA_RB_LDST_32,
    {"str", "str"}, {"str.nr", "str.nr"}},

  /* str rA, [rB, rC] */
  {&flare_grp_info_g6,
    FLARE_OPC_INFO_NULL_OP, FLARE_OA_RA_RB_RC_LDST_32,
    {"str", "str"}, {"str.nr", "str.nr"}},

  /* str rA, [rB, rC, #simm] */
  {&flare_grp_info_g6,
    FLARE_OPC_INFO_NULL_OP, FLARE_OA_RA_RB_RC_S5_LDST,
    {"str", "str"}, {"str.nr", "str.nr"}},
  /* -------- */
};

static const flare_grp_info_t flare_grp_info_g7_aluopbh =
{
  .grp=&flare_enc_info_grp_16,
  .grp_value=FLARE_G7_GRP_VALUE,

  .subgrp=&flare_enc_info_g7_aluopbh_subgrp,
  .subgrp_value=FLARE_G7_ALUOPBH_SUBGRP_VALUE,
};
static const flare_opc_info_t
  flare_opc_info_g7_aluopbh[FLARE_G7_ALUOPBH_OPC_INFO_LIM] =
{
  /* -------- */
  /* cmpb rA, rB
    cmph rA, rB */
  {&flare_grp_info_g7_aluopbh,
    FLARE_G7_ALUOPBH_OP_ENUM_CMP_RA_RB, FLARE_OA_RA_RB,
    {"cmpb", "cmph"}, {"cmpb.nr", "cmph.nr"}},

  /* lsrb rA, rB
    lsrh rA, rB */
  {&flare_grp_info_g7_aluopbh,
    FLARE_G7_ALUOPBH_OP_ENUM_LSR_RA_RB, FLARE_OA_RA_RB,
    {"lsrb", "lsrh"}, {"lsrb.nr", "lsrh.nr"}},

  /* asrb rA, rB
    asrh rA, rB */
  {&flare_grp_info_g7_aluopbh,
    FLARE_G7_ALUOPBH_OP_ENUM_ASR_RA_RB, FLARE_OA_RA_RB,
    {"asrb", "asrh"}, {"asrb.nr", "asrh.nr"}},

  /* reserved */
  {&flare_grp_info_g7_aluopbh,
    FLARE_G7_ALUOPBH_OP_ENUM_RESERVED_3, FLARE_OA_BAD,
    {"bad", "bad"}, {"bad.nr", "bad.nr"}},
  /* -------- */
};

static const flare_grp_info_t flare_grp_info_g7_sprldst =
{
  .grp=&flare_enc_info_grp_16,
  .grp_value=FLARE_G7_GRP_VALUE,

  .subgrp=&flare_enc_info_g7_sprldst_subgrp,
  .subgrp_value=FLARE_G7_SPRLDST_SUBGRP_VALUE,
};
static const flare_opc_info_t
  flare_opc_info_g7_sprldst[FLARE_G7_SPRLDST_OPC_INFO_LIM] =
{
  /* -------- */
  /* ldr sA, [rB] */
  {&flare_grp_info_g7_sprldst,
    FLARE_G7_SPRLDST_OP_ENUM_LDR_SA_RB, FLARE_OA_SA_RB_LDST,
    {"ldr", "ldr"}, {"ldr.nr", "ldr.nr"}},

  /* ldr sA, [sB] */
  {&flare_grp_info_g7_sprldst,
    FLARE_G7_SPRLDST_OP_ENUM_LDR_SA_SB, FLARE_OA_SA_SB_LDST,
    {"ldr", "ldr"}, {"ldr.nr", "ldr.nr"}},

  /* str sA, [rB] */
  {&flare_grp_info_g7_sprldst,
    FLARE_G7_SPRLDST_OP_ENUM_STR_SA_RB, FLARE_OA_SA_RB_LDST,
    {"str", "str"}, {"str.nr", "str.nr"}},

  /* str sA, [sB] */
  {&flare_grp_info_g7_sprldst,
    FLARE_G7_SPRLDST_OP_ENUM_STR_SA_SB, FLARE_OA_SA_SB_LDST,
    {"str", "str"}, {"str.nr", "str.nr"}},
  /* -------- */
};
/* -------- */
static const flare_grp_info_t flare_grp_info_g7_icreload =
{
  .grp=&flare_enc_info_grp_16,
  .grp_value=FLARE_G7_GRP_VALUE,

  .subgrp=&flare_enc_info_g7_icreload_subgrp,
  .subgrp_value=FLARE_G7_ICRELOAD_SUBGRP_VALUE,
};
static const flare_opc_info_t
  flare_opc_info_g7_icreload[FLARE_G7_ICRELOAD_OPC_INFO_LIM] =
{
  /* -------- */
  /* icreload [rA, #simm] */
  {&flare_grp_info_g7_icreload,
    FLARE_OPC_INFO_NULL_OP, FLARE_OA_RA_S5_JUSTADDR,
    {"icreload", "icreload"}, {"icreload.nr", "icreload.nr"}},
  /* -------- */
  /* Following are some pseudo instructions */
  /* icreload [rA] */
  {&flare_grp_info_g7_icreload,
    FLARE_OPC_INFO_NULL_OP, FLARE_OA_RA_JUSTADDR,
    {"icreload", "icreload"}, {"icreload.nr", "icreload.nr"}},

  /* icreload [rA, rC] */
  {&flare_grp_info_g7_icreload,
    FLARE_OPC_INFO_NULL_OP, FLARE_OA_RA_RC_JUSTADDR,
    {"icreload", "icreload"}, {"icreload.nr", "icreload.nr"}},

  /* icreload [rA, rC, #simm] */
  {&flare_grp_info_g7_icreload,
    FLARE_OPC_INFO_NULL_OP, FLARE_OA_RA_RC_S5_JUSTADDR,
    {"icreload", "icreload"}, {"icreload.nr", "icreload.nr"}},
  /* -------- */
};
/* -------- */
#endif    /* _FLARE_OPC_DECLS_H_ */

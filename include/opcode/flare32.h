/* BFD support for the flare32 microprocessor.
   Copyright (C) 2023 Free Software Foundation, Inc.
   Contributed by Andrew Clark (FL4SHK)

   This file is part of BFD, the Binary File Descriptor library.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston,
   MA 02110-1301, USA.  */

#ifndef _FLARE32_H_
#define _FLARE32_H_

#include "bfd.h"
#include <stdlib.h>
#include <stdint.h>

#define FLARE32_N_ONES(n) \
  ((((1ull << ((n) - 1ull)) - 1ull) << 1ull) | 1ull)

#define FLARE32_ARRAY_SIZE(x) (sizeof (x) / sizeof (x[0]))

/* In case we're on a 32-bit machine, construct a 64-bit "-1" value
  from smaller values. Start with zero, widen, *then* decrement.  */
#define MINUS_ONE       (((bfd_vma) 0) - 1)

/* Macros for getting and setting an instruction field.  */
#define GET_INSN_FIELD_WORKER(mask, bitpos, i) \
  (((i) & (mask)) >> (bitpos))
#define SET_INSN_FIELD_WORKER(mask, bitpos, i, v) \
  ( \
    (i) = ( \
      ((i) & ~(mask)) \
      | (((v) << (bitpos)) & (mask)) \
    ) \
  )
/* -------- */
typedef uint64_t flare32_temp_t;
typedef struct flare32_enc_info_t
{
  flare32_temp_t
    bitsize,
    bitpos,
    rsmask,
    mask;
} flare32_enc_info_t;

static inline flare32_temp_t
flare32_get_insn_field (flare32_temp_t mask,
                        flare32_temp_t bitpos,
                        flare32_temp_t insn)
{
  return (flare32_temp_t) GET_INSN_FIELD_WORKER (mask, bitpos, insn);
}
static inline flare32_temp_t
flare32_set_insn_field (flare32_temp_t mask,
                        flare32_temp_t bitpos,
                        flare32_temp_t insn,
                        flare32_temp_t value)
{
  return (flare32_temp_t) SET_INSN_FIELD_WORKER
    (mask, bitpos, insn, value);
}
static inline flare32_temp_t *
flare32_set_insn_field_p (flare32_temp_t mask,
                          flare32_temp_t bitpos,
                          flare32_temp_t *insn,
                          flare32_temp_t value)
{
  *insn = flare32_set_insn_field (mask, bitpos, *insn, value);
  return insn;
}
/* -------- */
static inline flare32_temp_t
flare32_get_insn_field_ei (const flare32_enc_info_t *enc_info,
                          flare32_temp_t insn)
{
  return (flare32_temp_t) GET_INSN_FIELD_WORKER
    (enc_info->mask, enc_info->bitpos, insn);
}
static inline flare32_temp_t
flare32_set_insn_field_ei (const flare32_enc_info_t *enc_info,
                          flare32_temp_t insn,
                          flare32_temp_t value)
{
  return (flare32_temp_t) SET_INSN_FIELD_WORKER
    (enc_info->mask, enc_info->bitpos, insn, value);
}
static inline flare32_temp_t *
flare32_set_insn_field_ei_p (const flare32_enc_info_t *enc_info,
                            flare32_temp_t *insn,
                            flare32_temp_t value)
{
  *insn = flare32_set_insn_field_ei (enc_info, *insn, value);
  return insn;
}


//extern const flare32_enc_info_t
//  /* -------- */
//  flare32_enc_info_grp_16,
//  flare32_enc_info_grp_32,
//  /* -------- */
//  flare32_enc_info_g0_pre_subgrp,
//  flare32_enc_info_g0_pre_fullgrp,
//  flare32_enc_info_g0_pre_s12,
//  /* -------- */
//  flare32_enc_info_g0_lpre_subgrp,
//  flare32_enc_info_g0_lpre_fullgrp,
//  flare32_enc_info_g0_lpre_subgrp_16,
//  flare32_enc_info_g0_lpre_fullgrp_16,
//  flare32_enc_info_g1g5g6_g0_lpre_s27,
//  flare32_enc_info_g3_g0_lpre_s23,
//  /* -------- */
//  flare32_enc_info_g1g5g6_s5,
//  flare32_enc_info_g1_op,
//  flare32_enc_info_g2_f,
//  flare32_enc_info_g2_op,
//  flare32_enc_info_g3_s9,
//  flare32_enc_info_g3_op,
//  flare32_enc_info_g4_op,
//  /* -------- */
//  flare32_enc_info_g7_aluopbh_subgrp,
//  flare32_enc_info_g7_aluopbh_fullgrp,
//  flare32_enc_info_g7_aluopbh_w,
//  flare32_enc_info_g7_aluopbh_op,
//  /* -------- */
//  flare32_enc_info_ra_ind,
//  flare32_enc_info_rb_ind;
//  /* -------- */
/* -------- */
#define FLARE32_ENC_RSMASK(name) \
  (FLARE32_N_ONES (FLARE32_##name##_BITSIZE))
#define FLARE32_ENC_MASK(name) \
  ((FLARE32_##name##_RSMASK) << (FLARE32_##name##_BITPOS))

#define FLARE32_GRP_BITSIZE (3ull)
#define FLARE32_GRP_RSMASK \
  (FLARE32_ENC_RSMASK (GRP))

#define FLARE32_GRP_16_BITPOS \
  (16ull - FLARE32_GRP_BITSIZE)
#define FLARE32_GRP_16_MASK \
  ((FLARE32_GRP_RSMASK) << (FLARE32_GRP_16_BITPOS))
  //(FLARE32_ENC_MASK (GRP_16))

#define FLARE32_GRP_32_BITPOS \
  (32ull - FLARE32_GRP_BITSIZE)
#define FLARE32_GRP_32_MASK \
  ((FLARE32_GRP_RSMASK) << (FLARE32_GRP_32_BITPOS))
  //(FLARE32_ENC_MASK (GRP_32))

#define FLARE32_FULLGRP_VALUE(subgrp_inner_name, grp_value, subgrp_value) \
  ( \
    (((grp_value) & FLARE32_GRP_RSMASK) \
      << (FLARE32_##subgrp_inner_name##_BITSIZE)) \
    | ((subgrp_value) & (FLARE32_##subgrp_inner_name##_RSMASK)) \
  )
/* -------- */
#define FLARE32_G0_GRP_VALUE (0x0ull)
/* -------- */
/* -------- */
/* `pre` subgrp */
#define FLARE32_G0_PRE_SUBGRP_BITSIZE (1ull)
#define FLARE32_G0_PRE_SUBGRP_BITPOS \
  (FLARE32_GRP_16_BITPOS - FLARE32_G0_PRE_SUBGRP_BITSIZE)
#define FLARE32_G0_PRE_SUBGRP_RSMASK \
  (FLARE32_ENC_RSMASK (G0_PRE_SUBGRP))
#define FLARE32_G0_PRE_SUBGRP_MASK \
  (FLARE32_ENC_MASK (G0_PRE_SUBGRP))


/* `pre` full group */
#define FLARE32_G0_PRE_FULLGRP_BITSIZE \
  (FLARE32_GRP_BITSIZE + FLARE32_G0_PRE_SUBGRP_BITSIZE)
#define FLARE32_G0_PRE_FULLGRP_BITPOS \
  (FLARE32_G0_PRE_SUBGRP_BITPOS)
#define FLARE32_G0_PRE_FULLGRP_RSMASK \
  (FLARE32_ENC_RSMASK (G0_PRE_FULLGRP))
#define FLARE32_G0_PRE_FULLGRP_MASK \
  (FLARE32_ENC_MASK (G0_PRE_FULLGRP))

/* `pre`-specific group values */
#define FLARE32_G0_PRE_SUBGRP_VALUE (0x0ull)
#define FLARE32_G0_PRE_FULLGRP_VALUE \
  (FLARE32_FULLGRP_VALUE(G0_PRE_SUBGRP, \
    FLARE32_G0_GRP_VALUE, \
    FLARE32_G0_PRE_SUBGRP_VALUE))

/* `pre` simm12 */
#define FLARE32_G0_PRE_S12_BITSIZE (12ull)
#define FLARE32_G0_PRE_S12_BITPOS (0ull)
#define FLARE32_G0_PRE_S12_RSMASK \
  (FLARE32_ENC_RSMASK (G0_PRE_S12))
#define FLARE32_G0_PRE_S12_MASK \
  (FLARE32_ENC_MASK (G0_PRE_S12))
/* -------- */
/* `lpre` subgrp */
#define FLARE32_G0_LPRE_SUBGRP_BITSIZE (2ull)
#define FLARE32_G0_LPRE_SUBGRP_BITPOS \
  (FLARE32_GRP_32_BITPOS - FLARE32_G0_LPRE_SUBGRP_BITSIZE)
#define FLARE32_G0_LPRE_SUBGRP_RSMASK \
  (FLARE32_ENC_RSMASK (G0_LPRE_SUBGRP))
#define FLARE32_G0_LPRE_SUBGRP_MASK \
  (FLARE32_ENC_MASK (G0_LPRE_SUBGRP))

/* `lpre` full group */
#define FLARE32_G0_LPRE_FULLGRP_BITSIZE \
  (FLARE32_GRP_BITSIZE + FLARE32_G0_LPRE_SUBGRP_BITSIZE)
#define FLARE32_G0_LPRE_FULLGRP_BITPOS (FLARE32_G0_LPRE_SUBGRP_BITPOS)
#define FLARE32_G0_LPRE_FULLGRP_RSMASK \
  (FLARE32_ENC_RSMASK (G0_LPRE_FULLGRP))
#define FLARE32_G0_LPRE_FULLGRP_MASK \
  (FLARE32_ENC_MASK (G0_LPRE_FULLGRP))

/* `lpre` (top 16-bit half) subgrp */
#define FLARE32_G0_LPRE_SUBGRP_16_BITPOS \
  (FLARE32_GRP_16_BITPOS - FLARE32_G0_LPRE_SUBGRP_BITSIZE)
#define FLARE32_G0_LPRE_SUBGRP_16_MASK \
  ((FLARE32_G0_LPRE_SUBGRP_RSMASK) << (FLARE32_G0_LPRE_SUBGRP_16_BITPOS))

/* `lpre` (top 16-bit half) full group */
#define FLARE32_G0_LPRE_FULLGRP_16_BITPOS \
  (FLARE32_G0_LPRE_SUBGRP_16_BITPOS)
#define FLARE32_G0_LPRE_FULLGRP_16_MASK \
  ((FLARE32_G0_LPRE_FULLGRP_RSMASK) << (FLARE32_G0_LPRE_FULLGRP_16_BITPOS))

/* `lpre`-specific group values */
#define FLARE32_G0_LPRE_SUBGRP_VALUE (0x2ull)
#define FLARE32_G0_LPRE_FULLGRP_VALUE \
  (FLARE32_FULLGRP_VALUE(G0_LPRE_SUBGRP, \
    FLARE32_G0_GRP_VALUE, \
    FLARE32_G0_LPRE_SUBGRP_VALUE))

//#define FLARE32_LPRE_GRP_16_MASK (FLARE32_LPRE_GRP_RSMASK << FLARE32_LPRE_GRP_BITPOS_16)
//#define FLARE32_LPRE_GRP_MASK (FLARE32_LPRE_GRP_RSMASK << FLARE32_LPRE_GRP_BITPOS)

/* `lpre` simm27, i.e. when used with an instruction from
  group 1, 5, or 6 */
#define FLARE32_G1G5G6_G0_LPRE_S27_BITSIZE (27ull)
#define FLARE32_G1G5G6_G0_LPRE_S27_BITPOS (0ull)
#define FLARE32_G1G5G6_G0_LPRE_S27_RSMASK \
  (FLARE32_ENC_RSMASK (G1G5G6_G0_LPRE_S27))
#define FLARE32_G1G5G6_G0_LPRE_S27_MASK \
  (FLARE32_ENC_MASK (G1G5G6_G0_LPRE_S27))

/* `lpre` simm23, i.e. when used with an instruction from group 3 */
#define FLARE32_G3_G0_LPRE_S23_BITSIZE (23ull)
#define FLARE32_G3_G0_LPRE_S23_BITPOS (0ull)
#define FLARE32_G3_G0_LPRE_S23_RSMASK \
  (FLARE32_ENC_RSMASK (G3_G0_LPRE_S23))
#define FLARE32_G3_G0_LPRE_S23_MASK \
  (FLARE32_ENC_MASK (G3_G0_LPRE_S23))
/* -------- */
/* simm5, i.e. when part of an instruction from groups 1, 5, or 6 */
#define FLARE32_G1G5G6_S5_BITSIZE (5ull)
#define FLARE32_G1G5G6_S5_BITPOS (8ull)
#define FLARE32_G1G5G6_S5_RSMASK \
  (FLARE32_ENC_RSMASK (G1G5G6_S5))
#define FLARE32_G1G5G6_S5_MASK \
  (FLARE32_ENC_MASK (G1G5G6_S5))
/* -------- */
/* the `grp` field of group 1 instructions */
#define FLARE32_G1_GRP_VALUE (0x1ull)

/* the `opcode` field of group 1 instructions */
#define FLARE32_G1_OP_BITSIZE (4ull)
#define FLARE32_G1_OP_BITPOS (4ull)
#define FLARE32_G1_OP_RSMASK \
  (FLARE32_ENC_RSMASK (G1_OP))
#define FLARE32_G1_OP_MASK \
  (FLARE32_ENC_MASK (G1_OP))

/* the specific opcode values usable with group 1 instructions */
#define FLARE32_G1_OP_ENUM_ADD_RA_S5 (0x0ull)
#define FLARE32_G1_OP_ENUM_ADD_RA_PC_S5 (0x1ull)
#define FLARE32_G1_OP_ENUM_ADD_RA_SP_S5 (0x2ull)
#define FLARE32_G1_OP_ENUM_ADD_RA_FP_S5 (0x3ull)
#define FLARE32_G1_OP_ENUM_CMP_RA_S5 (0x4ull)
#define FLARE32_G1_OP_ENUM_CPY_RA_S5 (0x5ull)
#define FLARE32_G1_OP_ENUM_LSL_RA_S5 (0x6ull)
#define FLARE32_G1_OP_ENUM_LSR_RA_S5 (0x7ull)
#define FLARE32_G1_OP_ENUM_ASR_RA_S5 (0x8ull)
#define FLARE32_G1_OP_ENUM_AND_RA_S5 (0x9ull)
#define FLARE32_G1_OP_ENUM_ORR_RA_S5 (0xaull)
#define FLARE32_G1_OP_ENUM_XOR_RA_S5 (0xbull)
#define FLARE32_G1_OP_ENUM_ZE_RA_S5 (0xcull)
#define FLARE32_G1_OP_ENUM_SE_RA_S5 (0xdull)
#define FLARE32_G1_OP_ENUM_SWI_RA_S5 (0xeull)
#define FLARE32_G1_OP_ENUM_SWI_S5 (0xfull)
/* -------- */
/* the `grp` field of group 2 instructions */
#define FLARE32_G2_GRP_VALUE (0x2ull)

/* the `flag` field of group 1 instructions */
#define FLARE32_G2_F_BITSIZE (1ull)
#define FLARE32_G2_F_BITPOS (12ull)
#define FLARE32_G2_F_RSMASK \
  (FLARE32_ENC_RSMASK (G2_F))
#define FLARE32_G2_F_MASK \
  (FLARE32_ENC_MASK (G2_F))

/* the `opcode` field of group 2 instructions */
#define FLARE32_G2_OP_BITSIZE (4ull)
#define FLARE32_G2_OP_BITPOS (8ull)
#define FLARE32_G2_OP_RSMASK \
  (FLARE32_ENC_RSMASK (G2_OP))
#define FLARE32_G2_OP_MASK \
  (FLARE32_ENC_MASK (G2_OP))

/* the specific opcode values usable with group 2 instructions */
#define FLARE32_G2_OP_ENUM_ADD_RA_RB (0x0ull)
#define FLARE32_G2_OP_ENUM_SUB_RA_RB (0x1ull)
#define FLARE32_G2_OP_ENUM_ADD_RA_SP_RB (0x2ull)
#define FLARE32_G2_OP_ENUM_ADD_RA_FP_RB (0x3ull)
#define FLARE32_G2_OP_ENUM_CMP_RA_RB (0x4ull)
#define FLARE32_G2_OP_ENUM_CPY_RA_RB (0x5ull)
#define FLARE32_G2_OP_ENUM_LSL_RA_RB (0x6ull)
#define FLARE32_G2_OP_ENUM_LSR_RA_RB (0x7ull)
#define FLARE32_G2_OP_ENUM_ASR_RA_RB (0x8ull)
#define FLARE32_G2_OP_ENUM_AND_RA_RB (0x9ull)
#define FLARE32_G2_OP_ENUM_ORR_RA_RB (0xaull)
#define FLARE32_G2_OP_ENUM_XOR_RA_RB (0xbull)
#define FLARE32_G2_OP_ENUM_ADC_RA_RB (0xcull)
#define FLARE32_G2_OP_ENUM_SBC_RA_RB (0xdull)
#define FLARE32_G2_OP_ENUM_CMPBC_RA_RB (0xeull)
#define FLARE32_G2_OP_ENUM_RESERVED_15 (0xfull)
/* -------- */
/* the `grp` field of group 3 instructions */
#define FLARE32_G3_GRP_VALUE (0x3ull)

/* the PC-relative `simm9` field of group 3 instructions */
#define FLARE32_G3_S9_BITSIZE (9ull)
#define FLARE32_G3_S9_BITPOS (4ull)
#define FLARE32_G3_S9_RSMASK \
  (FLARE32_ENC_RSMASK (G3_S9))
#define FLARE32_G3_S9_MASK \
  (FLARE32_ENC_MASK (G3_S9))

/* the `opcode` field of group 1 instructions */
#define FLARE32_G3_OP_BITSIZE (4ull)
#define FLARE32_G3_OP_BITPOS (0ull)
#define FLARE32_G3_OP_RSMASK \
  (FLARE32_ENC_RSMASK (G3_OP))
#define FLARE32_G3_OP_MASK \
  (FLARE32_ENC_MASK (G3_OP))

/* the specific opcode values usable with group 1 instructions */
#define FLARE32_G3_OP_ENUM_BL_PCREL_S9 (0x0ull)
#define FLARE32_G3_OP_ENUM_BRA_PCREL_S9 (0x1ull)
#define FLARE32_G3_OP_ENUM_BEQ_PCREL_S9 (0x2ull)
#define FLARE32_G3_OP_ENUM_BNE_PCREL_S9 (0x3ull)
#define FLARE32_G3_OP_ENUM_BMI_PCREL_S9 (0x4ull)
#define FLARE32_G3_OP_ENUM_BPL_PCREL_S9 (0x5ull)
#define FLARE32_G3_OP_ENUM_BVS_PCREL_S9 (0x6ull)
#define FLARE32_G3_OP_ENUM_BVC_PCREL_S9 (0x7ull)
#define FLARE32_G3_OP_ENUM_BGEU_PCREL_S9 (0x8ull)
#define FLARE32_G3_OP_ENUM_BLTU_PCREL_S9 (0x9ull)
#define FLARE32_G3_OP_ENUM_BGTU_PCREL_S9 (0xaull)
#define FLARE32_G3_OP_ENUM_BLEU_PCREL_S9 (0xbull)
#define FLARE32_G3_OP_ENUM_BGES_PCREL_S9 (0xcull)
#define FLARE32_G3_OP_ENUM_BLTS_PCREL_S9 (0xdull)
#define FLARE32_G3_OP_ENUM_BGTS_PCREL_S9 (0xeull)
#define FLARE32_G3_OP_ENUM_BLES_PCREL_S9 (0xfull)

/* The offset used in calcuations of a branch's target address.
  The processor adds this value to the encoded immediate field of relative
  branches during branch target address computation. */
#define FLARE32_PCREL_S9_BASE_OFFSET (0x2ull)
/* -------- */
/* the `grp` field of group 4 instructions */
#define FLARE32_G4_GRP_VALUE (0x4ull)

/* the `opcode` field of group 4 instructions */
#define FLARE32_G4_OP_BITSIZE (5ull)
#define FLARE32_G4_OP_BITPOS (8ull)
#define FLARE32_G4_OP_RSMASK \
  (FLARE32_ENC_RSMASK (G4_OP))
#define FLARE32_G4_OP_MASK \
  (FLARE32_ENC_MASK (G4_OP))

/* the specific opcode values usable with group 4 instructions */
#define FLARE32_G4_OP_ENUM_JL_RA (0x0ull)
#define FLARE32_G4_OP_ENUM_JMP_RA (0x1ull)
//#define FLARE32_G4_OP_ENUM_JMP_RA_RB (0x2ull)
#define FLARE32_G4_OP_ENUM_JMP_IRA (0x2ull)
#define FLARE32_G4_OP_ENUM_RETI (0x3ull)
#define FLARE32_G4_OP_ENUM_EI (0x4ull)
#define FLARE32_G4_OP_ENUM_DI (0x5ull)
#define FLARE32_G4_OP_ENUM_PUSH_RA_RB (0x6ull)
#define FLARE32_G4_OP_ENUM_PUSH_SA_RB (0x7ull)
#define FLARE32_G4_OP_ENUM_POP_RA_RB (0x8ull)
#define FLARE32_G4_OP_ENUM_POP_SA_RB (0x9ull)
#define FLARE32_G4_OP_ENUM_POP_PC_RB (0xaull)
#define FLARE32_G4_OP_ENUM_MUL_RA_RB (0xbull)
#define FLARE32_G4_OP_ENUM_UDIV_RA_RB (0xcull)
#define FLARE32_G4_OP_ENUM_SDIV_RA_RB (0xdull)
#define FLARE32_G4_OP_ENUM_UMOD_RA_RB (0xeull)
#define FLARE32_G4_OP_ENUM_SMOD_RA_RB (0xfull)

#define FLARE32_G4_OP_ENUM_LUMUL_RA_RB (0x10ull)
#define FLARE32_G4_OP_ENUM_LSMUL_RA_RB (0x11ull)
#define FLARE32_G4_OP_ENUM_LUDIV_RA_RB (0x12ull)
#define FLARE32_G4_OP_ENUM_LSDIV_RA_RB (0x13ull)
#define FLARE32_G4_OP_ENUM_LUMOD_RA_RB (0x14ull)
#define FLARE32_G4_OP_ENUM_LSMOD_RA_RB (0x15ull)
#define FLARE32_G4_OP_ENUM_LDUB_RA_RB (0x16ull)
#define FLARE32_G4_OP_ENUM_LDSB_RA_RB (0x17ull)
#define FLARE32_G4_OP_ENUM_LDUH_RA_RB (0x18ull)
#define FLARE32_G4_OP_ENUM_LDSH_RA_RB (0x19ull)
#define FLARE32_G4_OP_ENUM_STB_RA_RB (0x1aull)
#define FLARE32_G4_OP_ENUM_STH_RA_RB (0x1bull)
#define FLARE32_G4_OP_ENUM_CPY_RA_SB (0x1cull)
#define FLARE32_G4_OP_ENUM_CPY_SA_RB (0x1dull)
#define FLARE32_G4_OP_ENUM_CPY_SA_SB (0x1eull)
#define FLARE32_G4_OP_ENUM_INDEX_RA (0x1full)
/* -------- */
/* the `grp` field of group 5 instructions */
#define FLARE32_G5_GRP_VALUE (0x5ull)
/* -------- */
/* the `grp` field of group 6 instructions */
#define FLARE32_G6_GRP_VALUE (0x6ull)
/* -------- */
/* the `grp` field of group 7 instructions */
#define FLARE32_G7_GRP_VALUE (0x7ull)
/* -------- */
/* The `subgrp` field of 8-bit/16-bit ALU Ops in group 7 */
#define FLARE32_G7_ALUOPBH_SUBGRP_BITSIZE (2ull)
#define FLARE32_G7_ALUOPBH_SUBGRP_BITPOS \
  (FLARE32_GRP_16_BITPOS - FLARE32_G7_ALUOPBH_SUBGRP_BITSIZE)
#define FLARE32_G7_ALUOPBH_SUBGRP_RSMASK \
  (FLARE32_ENC_RSMASK (G7_ALUOPBH_SUBGRP))
#define FLARE32_G7_ALUOPBH_SUBGRP_MASK \
  (FLARE32_ENC_MASK (G7_ALUOPBH_SUBGRP))

#define FLARE32_G7_ALUOPBH_FULLGRP_BITSIZE \
  (FLARE32_GRP_BITSIZE + FLARE32_G7_ALUOPBH_SUBGRP_BITSIZE)
#define FLARE32_G7_ALUOPBH_FULLGRP_BITPOS \
  (FLARE32_G7_ALUOPBH_SUBGRP_BITPOS)
#define FLARE32_G7_ALUOPBH_FULLGRP_RSMASK \
  (FLARE32_ENC_RSMASK (G7_ALUOPBH_FULLGRP))
#define FLARE32_G7_ALUOPBH_FULLGRP_MASK \
  (FLARE32_ENC_MASK (G7_ALUOPBH_FULLGRP))

/* the `width` field of 8-bit/16-bit ALU ops in group 7 */
#define FLARE32_G7_ALUOPBH_W_BITSIZE (1ull)
#define FLARE32_G7_ALUOPBH_W_BITPOS (10ull)
#define FLARE32_G7_ALUOPBH_W_RSMASK \
  (FLARE32_ENC_RSMASK (G7_ALUOPBH_W))
#define FLARE32_G7_ALUOPBH_W_MASK \
  (FLARE32_ENC_MASK (G7_ALUOPBH_W))

/* the specific width values usable with 8-bit/16-bit ALU Ops in 
  group 7 */
#define FLARE32_G7_ALUOPBH_W_ENUM_8 (0x0ull)
#define FLARE32_G7_ALUOPBH_W_ENUM_16 (0x1ull)

/* the `opcode` field of 8-bit/16-bit ALU Ops in group 7 */
#define FLARE32_G7_ALUOPBH_OP_BITSIZE (2ull)
#define FLARE32_G7_ALUOPBH_OP_BITPOS (8ull)
#define FLARE32_G7_ALUOPBH_OP_RSMASK \
  (FLARE32_ENC_RSMASK (G7_ALUOPBH_OP))
#define FLARE32_G7_ALUOPBH_OP_MASK \
  (FLARE32_ENC_MASK (G7_ALUOPBH_OP))

/* the specific opcode values usable with 8-bit/16-bit ALU Ops in 
  group 7 */
#define FLARE32_G7_ALUOPBH_OP_ENUM_CMP_RA_RB (0x0ull)
#define FLARE32_G7_ALUOPBH_OP_ENUM_LSR_RA_RB (0x1ull)
#define FLARE32_G7_ALUOPBH_OP_ENUM_ASR_RA_RB (0x2ull)
#define FLARE32_G7_ALUOPBH_OP_ENUM_RESERVED_3 (0x3ull)

/* Constant fields of Group 7 8-bit/16-bit ALU Ops */
#define FLARE32_G7_ALUOPBH_SUBGRP_VALUE (0x0ull)
#define FLARE32_G7_ALUOPBH_FULLGRP_VALUE \
  (FLARE32_FULLGRP_VALUE(G7_ALUOPBH_SUBGRP, \
    FLARE32_G7_GRP_VALUE, \
    FLARE32_G7_ALUOPBH_SUBGRP_VALUE))

/* -------- */

/* The `subgrp` field of the extra spr-using `ldr`/`str` instructions in
  group 7 */
#define FLARE32_G7_SPRLDST_SUBGRP_BITSIZE (3ull)
#define FLARE32_G7_SPRLDST_SUBGRP_BITPOS \
  (FLARE32_GRP_16_BITPOS - FLARE32_G7_SPRLDST_SUBGRP_BITSIZE)
#define FLARE32_G7_SPRLDST_SUBGRP_RSMASK \
  (FLARE32_ENC_RSMASK (G7_SPRLDST_SUBGRP))
#define FLARE32_G7_SPRLDST_SUBGRP_MASK \
  (FLARE32_ENC_MASK (G7_SPRLDST_SUBGRP))

#define FLARE32_G7_SPRLDST_FULLGRP_BITSIZE \
  (FLARE32_GRP_BITSIZE + FLARE32_G7_SPRLDST_SUBGRP_BITSIZE)
#define FLARE32_G7_SPRLDST_FULLGRP_BITPOS \
  (FLARE32_G7_SPRLDST_SUBGRP_BITPOS)
#define FLARE32_G7_SPRLDST_FULLGRP_RSMASK \
  (FLARE32_ENC_RSMASK (G7_SPRLDST_FULLGRP))
#define FLARE32_G7_SPRLDST_FULLGRP_MASK \
  (FLARE32_ENC_MASK (G7_SPRLDST_FULLGRP))

/* the `opcode` field of 8-bit/16-bit ALU Ops in group 7 */
#define FLARE32_G7_SPRLDST_OP_BITSIZE (2ull)
#define FLARE32_G7_SPRLDST_OP_BITPOS (8ull)
#define FLARE32_G7_SPRLDST_OP_RSMASK \
  (FLARE32_ENC_RSMASK (G7_SPRLDST_OP))
#define FLARE32_G7_SPRLDST_OP_MASK \
  (FLARE32_ENC_MASK (G7_SPRLDST_OP))

/* the specific opcode values usable with spr-using `ldr`/`str` Ops in 
  group 7 */
#define FLARE32_G7_SPRLDST_OP_ENUM_LDR_SA_RB (0x0ull)
#define FLARE32_G7_SPRLDST_OP_ENUM_LDR_SA_SB (0x1ull)
#define FLARE32_G7_SPRLDST_OP_ENUM_STR_SA_RB (0x2ull)
#define FLARE32_G7_SPRLDST_OP_ENUM_STR_SA_SB (0x3ull)

/* Constant fields of Group 7 spr-using `ldr`/`str` instruction */
#define FLARE32_G7_SPRLDST_SUBGRP_VALUE (0x02ull)
#define FLARE32_G7_SPRLDST_FULLGRP_VALUE \
  (FLARE32_FULLGRP_VALUE(G7_SPRLDST_SUBGRP, \
    FLARE32_G7_GRP_VALUE, \
    FLARE32_G7_SPRLDST_SUBGRP_VALUE))
/* -------- */
/* encoding of rA */
#define FLARE32_RA_IND_BITSIZE (4ull)
#define FLARE32_RA_IND_BITPOS (0ull)
#define FLARE32_RA_IND_RSMASK \
  (FLARE32_ENC_RSMASK (RA_IND))
#define FLARE32_RA_IND_MASK \
  (FLARE32_ENC_MASK (RA_IND))

/* encoding of rB */
#define FLARE32_RB_IND_BITSIZE (4ull)
#define FLARE32_RB_IND_BITPOS (4ull)
#define FLARE32_RB_IND_RSMASK \
  (FLARE32_ENC_RSMASK (RB_IND))
#define FLARE32_RB_IND_MASK \
  (FLARE32_ENC_MASK (RB_IND))
/* -------- */
#define FLARE32_INDEX_LPRE_EXT_BITPOS (48ull)

/* the left shift amount for a single `pre`/`lpre`/`index` when storing a
  "whole" instruction in a `flare32_temp_t` */
#define FLARE32_ONE_EXT_BITPOS (16ull)
#define FLARE32_NO_EXT_MASK ((1ull << FLARE32_ONE_EXT_BITPOS) - 1ull)
#define FLARE32_ONE_EXT_MASK (~(FLARE32_NO_EXT_MASK))

#define FLARE32_PRE_EXT_LSMASK \
  (FLARE32_G0_PRE_S12_MASK << FLARE32_ONE_EXT_BITPOS)
#define FLARE32_G1G5G6_LPRE_EXT_LSMASK \
  (FLARE32_G1G5G6_G0_LPRE_S27_MASK << FLARE32_ONE_EXT_BITPOS)
#define FLARE32_G3_LPRE_EXT_LSMASK \
  (FLARE32_G3_G0_LPRE_S23_MASK << FLARE32_ONE_EXT_BITPOS)
/* -------- */
typedef enum flare32_reg_kind_t
{
  FLARE32_REG_KIND_GPR,
  FLARE32_REG_KIND_SPR,
  FLARE32_REG_KIND_PC,
} flare32_reg_kind_t;

typedef struct flare32_reg_t
{
  const char *name;
  flare32_temp_t index;
  flare32_reg_kind_t kind;
} flare32_reg_t;
#define FLARE32_REG_NAME_MAX_LEN (10ull)
/* general-purpose registers */
#define FLARE32_NUM_GPRS (16ull)
extern const flare32_reg_t gprs[FLARE32_NUM_GPRS];

#define FLARE32_GPR_ENUM_R0 (0x0ull)
#define FLARE32_GPR_ENUM_R1 (0x1ull)
#define FLARE32_GPR_ENUM_R2 (0x2ull)
#define FLARE32_GPR_ENUM_R3 (0x3ull)
#define FLARE32_GPR_ENUM_R4 (0x4ull)
#define FLARE32_GPR_ENUM_R5 (0x5ull)
#define FLARE32_GPR_ENUM_R6 (0x6ull)
#define FLARE32_GPR_ENUM_R7 (0x7ull)
#define FLARE32_GPR_ENUM_R8 (0x8ull)
#define FLARE32_GPR_ENUM_R9 (0x9ull)
#define FLARE32_GPR_ENUM_R10 (0xaull)
#define FLARE32_GPR_ENUM_R11 (0xbull)
#define FLARE32_GPR_ENUM_R12 (0xcull)
#define FLARE32_GPR_ENUM_LR (0xdull)
#define FLARE32_GPR_ENUM_FP (0xeull)
#define FLARE32_GPR_ENUM_SP (0xfull)

#define FLARE32_INST_GPRS() \
  { \
    {"r0", FLARE32_GPR_ENUM_R0, FLARE32_REG_KIND_GPR}, \
    {"r1", FLARE32_GPR_ENUM_R1, FLARE32_REG_KIND_GPR}, \
    {"r2", FLARE32_GPR_ENUM_R2, FLARE32_REG_KIND_GPR}, \
    {"r3", FLARE32_GPR_ENUM_R3, FLARE32_REG_KIND_GPR}, \
    {"r4", FLARE32_GPR_ENUM_R4, FLARE32_REG_KIND_GPR}, \
    {"r5", FLARE32_GPR_ENUM_R5, FLARE32_REG_KIND_GPR}, \
    {"r6", FLARE32_GPR_ENUM_R6, FLARE32_REG_KIND_GPR}, \
    {"r7", FLARE32_GPR_ENUM_R7, FLARE32_REG_KIND_GPR}, \
    {"r8", FLARE32_GPR_ENUM_R8, FLARE32_REG_KIND_GPR}, \
    {"r9", FLARE32_GPR_ENUM_R9, FLARE32_REG_KIND_GPR}, \
    {"r10", FLARE32_GPR_ENUM_R10, FLARE32_REG_KIND_GPR}, \
    {"r11", FLARE32_GPR_ENUM_R11, FLARE32_REG_KIND_GPR}, \
    {"r12", FLARE32_GPR_ENUM_R12, FLARE32_REG_KIND_GPR}, \
    {"lr", FLARE32_GPR_ENUM_LR, FLARE32_REG_KIND_GPR}, \
    {"fp", FLARE32_GPR_ENUM_FP, FLARE32_REG_KIND_GPR}, \
    {"sp", FLARE32_GPR_ENUM_SP, FLARE32_REG_KIND_GPR}, \
  }

/* special-purpose registers */
//#define FLARE32_NUM_SPRS (16ull)
//#define FLARE32_REAL_NUM_SPRS (8ull)
//#define FLARE32_NUM_SPRS (8ull)
#define FLARE32_NUM_SPRS (6ull)
extern const flare32_reg_t sprs[FLARE32_NUM_SPRS];

#define FLARE32_SPR_ENUM_FLAGS (0x0ull)
//#define FLARE32_SPR_ENUM_HI (0x1ull)
//#define FLARE32_SPR_ENUM_LO (0x2ull)
#define FLARE32_SPR_ENUM_IDS (0x1ull)
#define FLARE32_SPR_ENUM_IRA (0x2ull)
#define FLARE32_SPR_ENUM_IE (0x3ull)
#define FLARE32_SPR_ENUM_ITY (0x4ull)
#define FLARE32_SPR_ENUM_STY (0x5ull)

#define FLARE32_INST_SPRS() \
  { \
    {"flags", FLARE32_SPR_ENUM_FLAGS, FLARE32_REG_KIND_SPR}, \
    /* {"hi", FLARE32_SPR_ENUM_HI, FLARE32_REG_KIND_SPR}, */ \
    /* {"lo", FLARE32_SPR_ENUM_LO, FLARE32_REG_KIND_SPR}, */ \
    {"ids", FLARE32_SPR_ENUM_IDS, FLARE32_REG_KIND_SPR}, \
    {"ira", FLARE32_SPR_ENUM_IRA, FLARE32_REG_KIND_SPR}, \
    {"ie", FLARE32_SPR_ENUM_IE, FLARE32_REG_KIND_SPR}, \
    {"ity", FLARE32_SPR_ENUM_ITY, FLARE32_REG_KIND_SPR}, \
    {"sty", FLARE32_SPR_ENUM_STY, FLARE32_REG_KIND_SPR}, \
  }

/* bits of `flags` */
#define FLARE32_FLAGS_Z_BITPOS ((flare32_temp_t) 0x0ull)
#define FLARE32_FLAGS_C_BITPOS ((flare32_temp_t) 0x1ull)
#define FLARE32_FLAGS_V_BITPOS ((flare32_temp_t) 0x2ull)
#define FLARE32_FLAGS_N_BITPOS ((flare32_temp_t) 0x3ull)

#define FLARE32_FLAGS_Z_MASK \
  (((flare32_temp_t) 0x1ull) << FLARE32_FLAGS_Z_BITPOS) 
#define FLARE32_FLAGS_C_MASK \
  (((flare32_temp_t) 0x1ull) << FLARE32_FLAGS_C_BITPOS) 
#define FLARE32_FLAGS_V_MASK \
  (((flare32_temp_t) 0x1ull) << FLARE32_FLAGS_V_BITPOS) 
#define FLARE32_FLAGS_N_MASK \
  (((flare32_temp_t) 0x1ull) << FLARE32_FLAGS_N_BITPOS) 

#define FLARE32_SIM_FLAGS_VN_MASK(bits) \
  ((uint64_t) 0x1ull << (uint64_t) bits)
#define FLARE32_SIM_FLAGS_Z_MASK(bits) \
  (FLARE32_SIM_FLAGS_VN_MASK (bits) - (int64_t) 0x1ll)
#define FLARE32_SIM_FLAGS_C_MASK(bits) \
  (FLARE32_SIM_FLAGS_VN_MASK (bits) << (uint64_t) 0x1ull)

extern const flare32_reg_t reg_pc;
#define FLARE32_INST_REG_PC() \
  {"pc", 0x0, FLARE32_REG_KIND_PC}

#define FLARE32_SIM_FIRST_SPR_REGNO \
  (FLARE32_NUM_GPRS)
#define FLARE32_TOTAL_NUM_NON_PC_REGS \
  (FLARE32_NUM_GPRS + FLARE32_NUM_SPRS)
#define FLARE32_TOTAL_NUM_REGS \
  (FLARE32_TOTAL_NUM_NON_PC_REGS + 1ull)
/* -------- */
typedef enum flare32_have_plp_t {
  FLARE32_HAVE_PLP_NEITHER,
  FLARE32_HAVE_PLP_PRE,
  FLARE32_HAVE_PLP_LPRE,
} flare32_have_plp_t;
static inline unsigned
flare32_have_plp_insn_length (flare32_have_plp_t have_plp)
{
  switch (have_plp)
  {
    case FLARE32_HAVE_PLP_NEITHER:
      return 2;
    case FLARE32_HAVE_PLP_PRE:
      return 4;
    case FLARE32_HAVE_PLP_LPRE:
      return 6;
    default:
      abort ();
      return 0;
  }
}
static inline unsigned
flare32_have_plp_distance
  (flare32_have_plp_t from, flare32_have_plp_t to)
{
  return (
    flare32_have_plp_insn_length (from)
    - flare32_have_plp_insn_length (to)
  );
}
typedef enum flare32_oparg_t
{
  FLARE32_OA_BAD,
  FLARE32_OA_NONE,
  FLARE32_OA_PRE,
  FLARE32_OA_LPRE,
  FLARE32_OA_RA_S5,
  FLARE32_OA_RA_PC_S5,
  FLARE32_OA_RA_SP_S5,
  FLARE32_OA_RA_FP_S5,
  FLARE32_OA_S5,
  FLARE32_OA_RA,
  FLARE32_OA_RA_RB,
  FLARE32_OA_RA_SP_RB,
  FLARE32_OA_RA_FP_RB,
  FLARE32_OA_PCREL_S9,
  //FLARE32_OA_PCREL_S32_NO_RELAX,
  FLARE32_OA_IRA,
  FLARE32_OA_RA_SB,
  FLARE32_OA_SA_RB,
  FLARE32_OA_SA_SB,
  FLARE32_OA_PC_RB,
  FLARE32_OA_RA_IMPLICIT_SP,
  FLARE32_OA_SA_IMPLICIT_SP,
  FLARE32_OA_PC_IMPLICIT_SP,
  FLARE32_OA_RA_RB_LDST,
  FLARE32_OA_RA_RB_LDST_32,
  FLARE32_OA_RA_RB_RC_LDST,
  FLARE32_OA_RA_RB_RC_LDST_32,
  FLARE32_OA_RA_RB_S5_LDST,
  FLARE32_OA_RA_RB_RC_S5_LDST,
  FLARE32_OA_RA_RB_CPY64,
  FLARE32_OA_SA_RB_LDST,
  FLARE32_OA_SA_SB_LDST,
} flare32_oparg_t;


typedef struct flare32_grp_info_t
{
  const flare32_enc_info_t
    *grp;
  unsigned
    //grp_bitsize,
    //grp_bitpos,
    //grp_rsmask,
    //grp_mask,
    grp_value; /* The primary group, i.e. the top three bits */

  const flare32_enc_info_t
    *subgrp;
  unsigned
    //subgrp_bitsize,
    //subgrp_bitpos,
    //subgrp_rsmask,
    //subgrp_mask,
    subgrp_value;
} flare32_grp_info_t;
//extern const flare32_grp_info_t
//  flare32_grp_info_g0_pre,
//  flare32_grp_info_g0_lpre,
//  flare32_grp_info_g1,
//  flare32_grp_info_g2,
//  flare32_grp_info_g3,
//  flare32_grp_info_g4,
//  flare32_grp_info_g5,
//  flare32_grp_info_g6,
//  flare32_grp_info_g7_aluopbh;

#define FLARE32_OPC_INFO_NAME_MAX_LEN (15ull)
#define FLARE32_OPC_INFO_NAMES_LIM (2ull)
typedef struct flare32_opc_info_t
{
  const flare32_grp_info_t *grp_info;
  flare32_temp_t opcode;
  flare32_oparg_t oparg;
  const char
    *names[FLARE32_OPC_INFO_NAMES_LIM],
    *nr_names[FLARE32_OPC_INFO_NAMES_LIM];
} flare32_opc_info_t;

//#define FLARE32_HTAB_KEY_BUF_LIM (64ull)
//extern char
//  flare32_opc_info_htab_key_buf[FLARE32_HTAB_KEY_BUF_LIM];
//extern const char *flare32_opc_info_to_htab_key
//  (char *cbuf, unsigned cbuf_lim, const flare32_opc_info_t *opc_info,
//  unsigned names_ind);
//extern const char *flare32_op_oa_htab_key
//  (char *cbuf, unsigned cbuf_lim, const char *name, flare32_oparg_t oparg);

// TODO: change this to use a `realloc ()`ed 2D dynamic array instead.
// For that, we can use an `htab_t` that maps instruction name keys to
// indices into 2D dynamic array. I think this would probably be faster?
//typedef struct flare32_opci_list_t
//{
//  const flare32_opc_info_t *opc_info;
//  struct flare32_opci_list_t *next;
//} flare32_opci_list_t;
//
//extern flare32_opci_list_t *flare32_opci_list_create (void);
//extern flare32_opci_list_t *flare32_opci_list_append
//  (flare32_opci_list_t *self, const flare32_opc_info_t *opc_info);
//extern void flare32_opci_list_delete (flare32_opci_list_t *self);

//typedef struct flare32_opci_list2d_t
//{
//  struct flare32_opci_list_t *list,
//  struct flare32_opci_list2d_t *next;
//} flare32_opci_list2d_t;
//extern flare32_opci_list2d_t *flare32_opci_list2d_create (void);
//extern flare32_opci_list2d_t *flare32_opci_list2d_append
//  (flare32_opci_list2d_t *self, flare32_opci_list_t *list);
//extern void flare32_opci_list2d_delete (flare32_opci_list2d_t *self);

typedef struct flare32_opci_vec_t
{
  const flare32_opc_info_t **data;
  size_t size;
} flare32_opci_vec_t;

//extern flare32_opci_vec_t *flare32_opci_vec_create (void);
extern void flare32_opci_vec_create (flare32_opci_vec_t *self);
extern const flare32_opc_info_t *flare32_opci_vec_append
  (flare32_opci_vec_t *self, const flare32_opc_info_t *to_append);
extern void flare32_opci_vec_delete_data (flare32_opci_vec_t *self);

typedef struct flare32_opci_v2d_t
{
  flare32_opci_vec_t *data;
  size_t size;
} flare32_opci_v2d_t;
//extern flare32_opci_v2d_t *flare32_opci_v2d_create (void);
extern void flare32_opci_v2d_create (flare32_opci_v2d_t *self);
extern flare32_opci_vec_t *flare32_opci_v2d_append
  (flare32_opci_v2d_t *self, const flare32_opci_vec_t *to_append);
//extern void flare32_opci_v2d_append_opci (flare32_opci_v2d_t *self,
//  const flare32_opc_info_t *opc_info, size_t index);
extern void flare32_opci_v2d_delete_data (flare32_opci_v2d_t *self);

//typedef struct flare32_relax_general_t
//{
//  bool
//    was_lpre: 1,
//    rm_prefix: 1,
//    is_pc_relative: 1;
//  bfd *abfd;
//  asection *sec;
//  bfd_vma value;
//  bfd_byte *buf;
//  reloc_howto_type *howto;
//} flare32_relax_general_t;


#define FLARE32_OPC_INFO_NULL_OP (-1ull)
//#define FLARE32_OPC_INFO_PSEUDO_OP (-2ull)

#define FLARE32_G0_OPC_INFO_LIM (2ull)
//extern const flare32_opc_info_t
//  flare32_opc_info_g0[FLARE32_G0_OPC_INFO_LIM];

#define FLARE32_G1_OPC_INFO_LIM (16ull + 1ull)
//extern const flare32_opc_info_t
//  flare32_opc_info_g1[FLARE32_G1_OPC_INFO_LIM];

#define FLARE32_G2_OPC_INFO_LIM (16ull)
//extern const flare32_opc_info_t
//  flare32_opc_info_g2[FLARE32_G2_OPC_INFO_LIM];

#define FLARE32_G3_OPC_INFO_LIM (16ull)
//#define FLARE32_G3_OPC_INFO_LIM (16ull + 16ull)
//#define FLARE32_G3_OPC_INFO_LIM (16ull + 1ull)
//extern const flare32_opc_info_t
//  flare32_opc_info_g3[FLARE32_G3_OPC_INFO_LIM];

#define FLARE32_G4_OPC_INFO_LIM (32ull + 5ull + 6ull)
//extern const flare32_opc_info_t
//  flare32_opc_info_g4[FLARE32_G4_OPC_INFO_LIM];

#define FLARE32_G5_OPC_INFO_LIM (4ull)
//extern const flare32_opc_info_t
//  flare32_opc_info_g5[FLARE32_G5_OPC_INFO_LIM];

#define FLARE32_G6_OPC_INFO_LIM (4ull)
//extern const flare32_opc_info_t
//  flare32_opc_info_g6[FLARE32_G6_OPC_INFO_LIM];

#define FLARE32_G7_ALUOPBH_OPC_INFO_LIM (4ull)
//extern const flare32_opc_info_t
//  flare32_opc_info_g7_aluopbh[FLARE32_G7_ALUOPBH_OPC_INFO_LIM];

#define FLARE32_G7_SPRLDST_OPC_INFO_LIM (4ull)
//extern const flare32_opc_info_t
//  flare32_opc_info_g7_sprldst[FLARE32_G7_SPRLDST_OPC_INFO_LIM];

/* This is definitely excessive, but it should work fine */
#define FLARE32_MAX_UNIQUE_OPCI_NAMES (512ull)
/* -------- */
/* -------- */
static inline flare32_temp_t
flare32_enc_temp_insn_pre (flare32_temp_t simm12)
{
  flare32_temp_t ret = 0;
  //SET_INSN_FIELD(FLARE32_PRE_FULLGRP_MASK, FLARE32_G0_PRE_FULLGRP_BITPOS,
  //  ret, FLARE32_G0_PRE_FULLGRP_VALUE);
  //SET_INSN_FIELD(FLARE32_G0_PRE_S12_MASK, FLARE32_G0_PRE_S12_BITPOS, ret,
  //  simm12);
  flare32_set_insn_field_p (FLARE32_G0_PRE_FULLGRP_MASK,
    FLARE32_G0_PRE_FULLGRP_BITPOS, &ret,
    FLARE32_G0_PRE_FULLGRP_VALUE);
  flare32_set_insn_field_p (FLARE32_G0_PRE_S12_MASK, 
    FLARE32_G0_PRE_S12_BITPOS, &ret, simm12);
  return ret;
}
static inline flare32_temp_t
flare32_enc_temp_insn_g1g5g6_lpre (flare32_temp_t simm27)
{
  flare32_temp_t ret = 0;
  //SET_INSN_FIELD(FLARE32_G0_LPRE_FULLGRP_MASK, FLARE32_G0_LPRE_FULLGRP_BITPOS,
  //  ret, FLARE32_G0_LPRE_FULLGRP_VALUE);
  //SET_INSN_FIELD(FLARE32_G1G5G6_LPRE_S27_MASK,
  //  FLARE32_G1G5G6_G0_LPRE_S27_BITPOS, ret, simm27);
  flare32_set_insn_field_p (FLARE32_G0_LPRE_FULLGRP_MASK,
    FLARE32_G0_LPRE_FULLGRP_BITPOS, &ret, FLARE32_G0_LPRE_FULLGRP_VALUE);
  flare32_set_insn_field_p (FLARE32_G1G5G6_G0_LPRE_S27_MASK,
    FLARE32_G1G5G6_G0_LPRE_S27_BITPOS, &ret, simm27);
  return ret;
}
static inline flare32_temp_t
flare32_enc_temp_insn_g3_lpre (flare32_temp_t simm23)
{
  flare32_temp_t ret = 0;
  //SET_INSN_FIELD(FLARE32_G0_LPRE_FULLGRP_MASK, FLARE32_G0_LPRE_FULLGRP_BITPOS,
  //  ret, FLARE32_G0_LPRE_FULLGRP_VALUE);
  //SET_INSN_FIELD(FLARE32_G3_LPRE_S23_MASK,
  //  FLARE32_G3_LPRE_S23_BITPOS, ret, simm23);
  flare32_set_insn_field_p (FLARE32_G0_LPRE_FULLGRP_MASK,
    FLARE32_G0_LPRE_FULLGRP_BITPOS, &ret, FLARE32_G0_LPRE_FULLGRP_VALUE);
  flare32_set_insn_field_p (FLARE32_G3_G0_LPRE_S23_MASK,
    FLARE32_G3_G0_LPRE_S23_BITPOS, &ret, simm23);
  return ret;
}
/* -------- */
static inline flare32_temp_t
flare32_enc_temp_insn_g1 (flare32_temp_t op,
                          flare32_temp_t ra_ind,
                          flare32_temp_t simm5)
{
  flare32_temp_t ret = 0;
  //SET_INSN_FIELD(FLARE32_GRP_16_MASK, FLARE32_GRP_16_BITPOS, ret,
  //  FLARE32_G1_GRP_VALUE);
  //SET_INSN_FIELD(FLARE32_G1G5G6_S5_MASK, FLARE32_G1G5G6_S5_BITPOS, ret,
  //  simm5);
  //SET_INSN_FIELD(FLARE32_G1_OP_MASK, FLARE32_G1_OP_BITPOS, ret, op);
  //SET_INSN_FIELD(FLARE32_RA_IND_MASK, FLARE32_RA_IND_BITPOS, ret, ra_ind);
  flare32_set_insn_field_p (FLARE32_GRP_16_MASK, FLARE32_GRP_16_BITPOS, 
    &ret, FLARE32_G1_GRP_VALUE);
  flare32_set_insn_field_p (FLARE32_G1_OP_MASK, FLARE32_G1_OP_BITPOS, 
    &ret, op);
  flare32_set_insn_field_p (FLARE32_RA_IND_MASK, FLARE32_RA_IND_BITPOS, 
    &ret, ra_ind);
  flare32_set_insn_field_p (FLARE32_G1G5G6_S5_MASK, 
    FLARE32_G1G5G6_S5_BITPOS, &ret, simm5);
  return ret;
}
/* -------- */
static inline flare32_temp_t
flare32_enc_temp_insn_g2 (flare32_temp_t op,
                          flare32_temp_t f,
                          flare32_temp_t ra_ind,
                          flare32_temp_t rb_ind)
{
  flare32_temp_t ret = 0;
  //SET_INSN_FIELD(FLARE32_GRP_16_MASK, FLARE32_GRP_16_BITPOS, ret,
  //  FLARE32_G2_GRP_VALUE);
  //SET_INSN_FIELD(FLARE32_G2_F_MASK, FLARE32_G2_F_BITPOS, ret, f);
  //SET_INSN_FIELD(FLARE32_G2_OP_MASK, FLARE32_G2_OP_BITPOS, ret, op);
  //SET_INSN_FIELD(FLARE32_RB_IND_MASK, FLARE32_RB_IND_BITPOS, ret, rb_ind);
  //SET_INSN_FIELD(FLARE32_RA_IND_MASK, FLARE32_RA_IND_BITPOS, ret, ra_ind);

  flare32_set_insn_field_p (FLARE32_GRP_16_MASK,
    FLARE32_GRP_16_BITPOS, &ret, FLARE32_G2_GRP_VALUE);
  flare32_set_insn_field_p (FLARE32_G2_OP_MASK, FLARE32_G2_OP_BITPOS, 
    &ret, op);
  flare32_set_insn_field_p (FLARE32_G2_F_MASK, FLARE32_G2_F_BITPOS,
    &ret, f);
  flare32_set_insn_field_p (FLARE32_RA_IND_MASK, FLARE32_RA_IND_BITPOS, 
    &ret, ra_ind);
  flare32_set_insn_field_p (FLARE32_RB_IND_MASK, FLARE32_RB_IND_BITPOS, 
    &ret, rb_ind);

  return ret;
}
/* -------- */
static inline flare32_temp_t
flare32_enc_temp_insn_g3 (flare32_temp_t op,
                          flare32_temp_t simm9)
{
  flare32_temp_t ret = 0;
  //SET_INSN_FIELD(FLARE32_GRP_16_MASK, FLARE32_GRP_16_BITPOS, ret,
  //  FLARE32_G3_GRP_VALUE);
  //SET_INSN_FIELD(FLARE32_G3_S9_MASK, FLARE32_G3_S9_BITPOS, ret, simm9);
  //SET_INSN_FIELD(FLARE32_G3_OP_MASK, FLARE32_G3_OP_BITPOS, ret, op);

  flare32_set_insn_field_p (FLARE32_GRP_16_MASK, FLARE32_GRP_16_BITPOS, 
    &ret, FLARE32_G3_GRP_VALUE);
  flare32_set_insn_field_p (FLARE32_G3_OP_MASK, FLARE32_G3_OP_BITPOS, 
    &ret, op);
  flare32_set_insn_field_p (FLARE32_G3_S9_MASK, FLARE32_G3_S9_BITPOS, 
    &ret, simm9);
  return ret;
}
/* -------- */
static inline flare32_temp_t
flare32_enc_temp_insn_g4 (flare32_temp_t op,
                          flare32_temp_t ra_ind,
                          flare32_temp_t rb_ind)
{
  flare32_temp_t ret = 0;
  //SET_INSN_FIELD(FLARE32_GRP_16_MASK, FLARE32_GRP_16_BITPOS, ret,
  //  FLARE32_G4_GRP_VALUE);
  //SET_INSN_FIELD(FLARE32_G4_OP_MASK, FLARE32_G4_OP_BITPOS, ret, op);
  //SET_INSN_FIELD(FLARE32_RB_IND_MASK, FLARE32_RB_IND_BITPOS, ret, rb_ind);
  //SET_INSN_FIELD(FLARE32_RA_IND_MASK, FLARE32_RA_IND_BITPOS, ret, ra_ind);

  //ret |= (0x4ull << 13ull);
  flare32_set_insn_field_p (FLARE32_GRP_16_MASK, FLARE32_GRP_16_BITPOS, 
    &ret, FLARE32_G4_GRP_VALUE);
  flare32_set_insn_field_p (FLARE32_G4_OP_MASK, FLARE32_G4_OP_BITPOS, 
    &ret, op);
  flare32_set_insn_field_p (FLARE32_RA_IND_MASK, FLARE32_RA_IND_BITPOS, 
    &ret, ra_ind);
  flare32_set_insn_field_p (FLARE32_RB_IND_MASK, FLARE32_RB_IND_BITPOS, 
    &ret, rb_ind);
  return ret;
}
/* -------- */
static inline flare32_temp_t
flare32_enc_temp_insn_g5 (flare32_temp_t ra_ind,
                          flare32_temp_t rb_ind,
                          flare32_temp_t simm5)
{
  flare32_temp_t ret = 0;
  //SET_INSN_FIELD(FLARE32_GRP_16_MASK, FLARE32_GRP_16_BITPOS, ret,
  //  FLARE32_G5_GRP_VALUE);
  //SET_INSN_FIELD(FLARE32_G1G5G6_S5_MASK, FLARE32_G1G5G6_S5_BITPOS, ret,
  //  simm5);
  //SET_INSN_FIELD(FLARE32_RB_IND_MASK, FLARE32_RB_IND_BITPOS, ret, rb_ind);
  //SET_INSN_FIELD(FLARE32_RA_IND_MASK, FLARE32_RA_IND_BITPOS, ret, ra_ind);
  flare32_set_insn_field_p (FLARE32_GRP_16_MASK, FLARE32_GRP_16_BITPOS, 
    &ret, FLARE32_G5_GRP_VALUE);
  flare32_set_insn_field_p (FLARE32_RA_IND_MASK, FLARE32_RA_IND_BITPOS, 
    &ret, ra_ind);
  flare32_set_insn_field_p (FLARE32_RB_IND_MASK, FLARE32_RB_IND_BITPOS, 
    &ret, rb_ind);
  flare32_set_insn_field_p (FLARE32_G1G5G6_S5_MASK, 
    FLARE32_G1G5G6_S5_BITPOS, &ret, simm5);
  return ret;
}
/* -------- */
static inline flare32_temp_t
flare32_enc_temp_insn_g6 (flare32_temp_t ra_ind,
                          flare32_temp_t rb_ind,
                          flare32_temp_t simm5)
{
  flare32_temp_t ret = 0;
  //SET_INSN_FIELD(FLARE32_GRP_16_MASK, FLARE32_GRP_16_BITPOS, ret,
  //  FLARE32_G6_GRP_VALUE);
  //SET_INSN_FIELD(FLARE32_G1G5G6_S5_MASK, FLARE32_G1G5G6_S5_BITPOS, ret,
  //  simm5);
  //SET_INSN_FIELD(FLARE32_RB_IND_MASK, FLARE32_RB_IND_BITPOS, ret, rb_ind);
  //SET_INSN_FIELD(FLARE32_RA_IND_MASK, FLARE32_RA_IND_BITPOS, ret, ra_ind);

  flare32_set_insn_field_p (FLARE32_GRP_16_MASK, FLARE32_GRP_16_BITPOS, 
    &ret, FLARE32_G6_GRP_VALUE);
  flare32_set_insn_field_p (FLARE32_RA_IND_MASK, FLARE32_RA_IND_BITPOS, 
    &ret, ra_ind);
  flare32_set_insn_field_p (FLARE32_RB_IND_MASK, FLARE32_RB_IND_BITPOS, 
    &ret, rb_ind);
  flare32_set_insn_field_p (FLARE32_G1G5G6_S5_MASK, 
    FLARE32_G1G5G6_S5_BITPOS, &ret, simm5);
  return ret;
}
/* -------- */
static inline flare32_temp_t
flare32_enc_temp_insn_g7_aluopbh (flare32_temp_t op,
                                  flare32_temp_t w,
                                  flare32_temp_t ra_ind,
                                  flare32_temp_t rb_ind)
{
  flare32_temp_t ret = 0;
  flare32_set_insn_field_p (FLARE32_G7_ALUOPBH_FULLGRP_MASK,
    FLARE32_G7_ALUOPBH_FULLGRP_BITPOS, &ret,
    FLARE32_G7_ALUOPBH_FULLGRP_VALUE);
  flare32_set_insn_field_p (FLARE32_G7_ALUOPBH_OP_MASK,
    FLARE32_G7_ALUOPBH_OP_BITPOS, &ret, op);
  flare32_set_insn_field_p (FLARE32_G7_ALUOPBH_W_MASK,
    FLARE32_G7_ALUOPBH_W_BITPOS, &ret, w);
  flare32_set_insn_field_p (FLARE32_RA_IND_MASK, FLARE32_RA_IND_BITPOS, 
    &ret, ra_ind);
  flare32_set_insn_field_p (FLARE32_RB_IND_MASK, FLARE32_RB_IND_BITPOS, 
    &ret, rb_ind);
  return ret;
}
static inline flare32_temp_t
flare32_enc_temp_insn_g7_sprldst (flare32_temp_t op,
                                  flare32_temp_t ra_ind,
                                  flare32_temp_t rb_ind)
{
  flare32_temp_t ret = 0;
  flare32_set_insn_field_p (FLARE32_G7_SPRLDST_FULLGRP_MASK,
    FLARE32_G7_SPRLDST_FULLGRP_BITPOS, &ret,
    FLARE32_G7_SPRLDST_FULLGRP_VALUE);
  flare32_set_insn_field_p (FLARE32_G7_SPRLDST_OP_MASK,
    FLARE32_G7_SPRLDST_OP_BITPOS, &ret, op);
  flare32_set_insn_field_p (FLARE32_RA_IND_MASK, FLARE32_RA_IND_BITPOS, 
    &ret, ra_ind);
  flare32_set_insn_field_p (FLARE32_RB_IND_MASK, FLARE32_RB_IND_BITPOS, 
    &ret, rb_ind);
  return ret;
}
/* -------- */
static inline flare32_temp_t
flare32_sign_extend (flare32_temp_t value,
                    flare32_temp_t bits)
{
  if (value & (1ull << (bits - 1ull)))
  {
    /* VALUE is negative.  */
    value |= ((flare32_temp_t) -1ll) << bits;
  }
  else
  {
    value &= ~(((flare32_temp_t) -1ll) << bits)
      | ((((flare32_temp_t) 1ull) << bits) - 1ll);
  }

  return value;
}
static inline flare32_temp_t
flare32_zero_extend (flare32_temp_t value,
                    flare32_temp_t bits)
{
  value &= ~(((flare32_temp_t) -1ll) << bits)
    | ((((flare32_temp_t) 1ull) << bits) - 1ll);
  return value;
}

static inline flare32_temp_t
flare32_get_ext_imm (flare32_temp_t prefix_mask,
                    flare32_temp_t prefix_bitpos,
                    flare32_temp_t prefix_insn,
                    flare32_temp_t insn_mask,
                    flare32_temp_t insn_bitsize,
                    flare32_temp_t insn_bitpos,
                    flare32_temp_t insn)
{
  //return (flare32_temp_t) (
  //  (((prefix_insn & prefix_mask) >> prefix_bitpos) << insn_bitsize)
  //  | ((insn & insn_mask) >> insn_bitpos)
  //);
  return (flare32_temp_t) (
    (flare32_get_insn_field (prefix_mask, prefix_bitpos, prefix_insn)
      << insn_bitsize)
    | flare32_get_insn_field (insn_mask, insn_bitpos, insn)
  );
}

//static inline flare32_temp_t
//flare32_get_ext_imm (const flare32_enc_info_t *prefix_enc_info,
//                    flare32_temp_t prefix_insn,
//                    const flare32_enc_info_t *insn_enc_info,
//                    flare32_temp_t insn)
//{
//  return flare32_get_ext_imm_worker
//    (prefix_enc_info->mask, /* prefix_mask, */
//    prefix_enc_info->bitpos, /* prefix_bitpos, */
//    prefix_insn, /* prefix_insn, */
//    insn_enc_info->mask, /* insn_mask, */
//    insn_enc_info->bitsize, /* insn_bitsize, */
//    insn_enc_info->bitpos, /* insn_bitpos, */
//    insn /* insn */);
//}

static inline flare32_temp_t
flare32_get_g1g5g6_s17 (flare32_temp_t prefix_insn,
                        flare32_temp_t insn)
{
  return flare32_get_ext_imm 
    (FLARE32_G0_PRE_S12_MASK,
    FLARE32_G0_PRE_S12_BITPOS,
    prefix_insn,
    FLARE32_G1G5G6_S5_MASK,
    FLARE32_G1G5G6_S5_BITSIZE,
    FLARE32_G1G5G6_S5_BITPOS,
    insn);
}
static inline flare32_temp_t
flare32_get_g1g5g6_s32 (flare32_temp_t prefix_insn,
                        flare32_temp_t insn)
{
  return flare32_get_ext_imm
    (FLARE32_G1G5G6_G0_LPRE_S27_MASK,
    FLARE32_G1G5G6_G0_LPRE_S27_BITPOS,
    prefix_insn,
    FLARE32_G1G5G6_S5_MASK,
    FLARE32_G1G5G6_S5_BITSIZE,
    FLARE32_G1G5G6_S5_BITPOS,
    insn);
}
static inline flare32_temp_t
flare32_get_g3_s21 (flare32_temp_t prefix_insn,
                    flare32_temp_t insn)
{
  return flare32_get_ext_imm
    (FLARE32_G0_PRE_S12_MASK,
    FLARE32_G0_PRE_S12_BITPOS,
    prefix_insn,
    FLARE32_G3_S9_MASK,
    FLARE32_G3_S9_BITSIZE,
    FLARE32_G3_S9_BITPOS,
    insn);
}

static inline flare32_temp_t
flare32_get_g3_s32 (flare32_temp_t prefix_insn,
                    flare32_temp_t insn)
{
  return flare32_get_ext_imm
    (FLARE32_G1G5G6_G0_LPRE_S27_MASK,
    FLARE32_G1G5G6_G0_LPRE_S27_BITPOS,
    prefix_insn,
    FLARE32_G3_S9_MASK,
    FLARE32_G3_S9_BITSIZE,
    FLARE32_G3_S9_BITPOS,
    insn);
}

static inline flare32_temp_t
flare32_get_g5g6_s32_index (flare32_temp_t index_insn,
                            flare32_temp_t prefix_insn,
                            flare32_temp_t insn)
{
  return (
    (index_insn << FLARE32_INDEX_LPRE_EXT_BITPOS)
    | flare32_get_g1g5g6_s32 (prefix_insn, insn)
  );
}

static inline void
flare32_put_ext_imm (flare32_temp_t prefix_mask,
                    flare32_temp_t prefix_bitpos,
                    flare32_temp_t *prefix_insn,
                    flare32_temp_t insn_mask,
                    flare32_temp_t insn_bitsize,
                    flare32_temp_t insn_bitpos,
                    flare32_temp_t *insn,
                    flare32_temp_t combined)
{
  //*insn &= ~insn_mask;
  //*insn |= (combined << insn_bitpos) & insn_mask;
  flare32_set_insn_field_p (insn_mask, insn_bitpos, insn, combined);
  *prefix_insn = flare32_set_insn_field (prefix_mask, prefix_bitpos,
    *prefix_insn,
    combined >> insn_bitsize);

  //*prefix_insn &= ~prefix_mask;
  //*prefix_insn |= (combined >> insn_bitsize) & prefix_mask;
}

//static inline void
//flare32_put_ext_imm (const flare32_enc_info_t *prefix_enc_info,
//                    flare32_temp_t *prefix_insn,
//                    const flare32_enc_info_t *insn_enc_info,
//                    flare32_temp_t *insn,
//                    flare32_temp_t combined)
//{
//
//  flare32_put_ext_imm_worker
//    (prefix_enc_info->mask, /* prefix_mask, */
//    prefix_insn, /* prefix_insn, */
//    insn_enc_info->mask, /* insn_mask, */
//    insn_enc_info->bitsize, /* insn_bitsize, */
//    insn_enc_info->bitpos, /* insn_bitpos, */
//    insn, /* insn, */
//    combined /* combined */);
//}
static inline void
flare32_put_g1g5g6_s17 (flare32_temp_t *prefix_insn,
                        flare32_temp_t *insn,
                        flare32_temp_t combined)
{
  //flare32_put_ext_imm (FLARE32_G0_PRE_S12_MASK,
  //                    prefix_insn,
  //                    FLARE32_G1G5G6_S5_MASK,
  //                    FLARE32_G1G5G6_S5_BITSIZE,
  //                    FLARE32_G1G5G6_S5_BITPOS,
  //                    insn,
  //                    combined);
  flare32_put_ext_imm (FLARE32_G0_PRE_S12_MASK,
                      FLARE32_G0_PRE_S12_BITPOS,
                      prefix_insn,
                      FLARE32_G1G5G6_S5_MASK,
                      FLARE32_G1G5G6_S5_BITSIZE,
                      FLARE32_G1G5G6_S5_BITPOS,
                      insn,
                      combined);
}
static inline void
flare32_put_g1g5g6_s32 (flare32_temp_t *prefix_insn,
                        flare32_temp_t *insn,
                        flare32_temp_t combined)
{
  //flare32_put_ext_imm (FLARE32_G1G5G6_LPRE_S27_MASK,
  //                    prefix_insn,
  //                    FLARE32_G1G5G6_S5_MASK,
  //                    FLARE32_G1G5G6_S5_BITSIZE,
  //                    FLARE32_G1G5G6_S5_BITPOS,
  //                    insn,
  //                    combined);
  flare32_put_ext_imm (FLARE32_G1G5G6_G0_LPRE_S27_MASK,
                      FLARE32_G1G5G6_G0_LPRE_S27_BITPOS,
                      prefix_insn,
                      FLARE32_G1G5G6_S5_MASK,
                      FLARE32_G1G5G6_S5_BITSIZE,
                      FLARE32_G1G5G6_S5_BITPOS,
                      insn,
                      combined);
}

static inline void
flare32_put_g3_s21 (flare32_temp_t *prefix_insn,
                    flare32_temp_t *insn,
                    flare32_temp_t combined)
{
  /* flare32_put_ext_imm (FLARE32_G0_PRE_S12_MASK,*/ /* prefix_mask */
  /*                     prefix_insn,*/  /* prefix_insn */
  /*                     FLARE32_G3_S9_MASK,*/ /* insn_mask */
  /*                     FLARE32_G3_S9_BITSIZE,*/ /* insn_bitsize */
  /*                     FLARE32_G3_S9_BITPOS,*/ /* insn_bitpos */
  /*                     insn,*/ /* insn */
  /*                     combined);*/ /* combined */

  flare32_put_ext_imm (FLARE32_G0_PRE_S12_MASK,
                      FLARE32_G0_PRE_S12_BITPOS,
                      prefix_insn,
                      FLARE32_G3_S9_MASK,
                      FLARE32_G3_S9_BITSIZE,
                      FLARE32_G3_S9_BITPOS,
                      insn,
                      combined);
}
static inline void
flare32_put_g3_s32 (flare32_temp_t *prefix_insn,
                    flare32_temp_t *insn,
                    flare32_temp_t combined)
{
  //flare32_put_ext_imm (FLARE32_G3_LPRE_S23_MASK,
  //                    prefix_insn,
  //                    FLARE32_G3_S9_MASK,
  //                    FLARE32_G3_S9_BITSIZE,
  //                    FLARE32_G3_S9_BITPOS,
  //                    insn,
  //                    combined);
  flare32_put_ext_imm (FLARE32_G3_G0_LPRE_S23_MASK,
                      FLARE32_G3_G0_LPRE_S23_BITPOS,
                      prefix_insn,
                      FLARE32_G3_S9_MASK,
                      FLARE32_G3_S9_BITSIZE,
                      FLARE32_G3_S9_BITPOS,
                      insn,
                      combined);
}
/* -------- */
#endif    // _FLARE32_H_

/* BFD support for the flare microprocessor.
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

#ifndef _FLARE_H_
#define _FLARE_H_

#include "bfd.h"
#include <stdlib.h>
#include <stdint.h>

#define FLARE_N_ONES(n) \
  ((((1ull << ((n) - 1ull)) - 1ull) << 1ull) | 1ull)

#define FLARE_ARRAY_SIZE(x) (sizeof (x) / sizeof (x[0]))

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
typedef uint64_t flare_temp_t;
typedef struct flare_enc_info_t
{
  flare_temp_t
    bitsize,
    bitpos,
    rsmask,
    mask;
} flare_enc_info_t;

static inline flare_temp_t
flare_get_insn_field (flare_temp_t mask,
                        flare_temp_t bitpos,
                        flare_temp_t insn)
{
  return (flare_temp_t) GET_INSN_FIELD_WORKER (mask, bitpos, insn);
}
static inline flare_temp_t
flare_set_insn_field (flare_temp_t mask,
                        flare_temp_t bitpos,
                        flare_temp_t insn,
                        flare_temp_t value)
{
  return (flare_temp_t) SET_INSN_FIELD_WORKER
    (mask, bitpos, insn, value);
}
static inline flare_temp_t *
flare_set_insn_field_p (flare_temp_t mask,
                          flare_temp_t bitpos,
                          flare_temp_t *insn,
                          flare_temp_t value)
{
  *insn = flare_set_insn_field (mask, bitpos, *insn, value);
  return insn;
}
/* -------- */
static inline flare_temp_t
flare_get_insn_field_ei (const flare_enc_info_t *enc_info,
                          flare_temp_t insn)
{
  return (flare_temp_t) GET_INSN_FIELD_WORKER
    (enc_info->mask, enc_info->bitpos, insn);
}
static inline flare_temp_t
flare_set_insn_field_ei (const flare_enc_info_t *enc_info,
                          flare_temp_t insn,
                          flare_temp_t value)
{
  return (flare_temp_t) SET_INSN_FIELD_WORKER
    (enc_info->mask, enc_info->bitpos, insn, value);
}
static inline flare_temp_t *
flare_set_insn_field_ei_p (const flare_enc_info_t *enc_info,
                            flare_temp_t *insn,
                            flare_temp_t value)
{
  *insn = flare_set_insn_field_ei (enc_info, *insn, value);
  return insn;
}


//extern const flare_enc_info_t
//  /* -------- */
//  flare_enc_info_grp_16,
//  flare_enc_info_grp_32,
//  /* -------- */
//  flare_enc_info_g0_pre_subgrp,
//  flare_enc_info_g0_pre_fullgrp,
//  flare_enc_info_g0_pre_s12,
//  /* -------- */
//  flare_enc_info_g0_lpre_subgrp,
//  flare_enc_info_g0_lpre_fullgrp,
//  flare_enc_info_g0_lpre_subgrp_16,
//  flare_enc_info_g0_lpre_fullgrp_16,
//  flare_enc_info_g0_lpre_s27,
//  flare_enc_info_g0_lpre_s23,
//  /* -------- */
//  flare_enc_info_g1g5g6_s5,
//  flare_enc_info_g1_op,
//  flare_enc_info_g2_f,
//  flare_enc_info_g2_op,
//  flare_enc_info_g3_s9,
//  flare_enc_info_g3_op,
//  flare_enc_info_g4_op,
//  /* -------- */
//  flare_enc_info_g7_aluopbh_subgrp,
//  flare_enc_info_g7_aluopbh_fullgrp,
//  flare_enc_info_g7_aluopbh_w,
//  flare_enc_info_g7_aluopbh_op,
//  /* -------- */
//  flare_enc_info_ra_ind,
//  flare_enc_info_rb_ind;
//  /* -------- */
/* -------- */
#define FLARE_ENC_RSMASK(name) \
  (FLARE_N_ONES (FLARE_##name##_BITSIZE))
#define FLARE_ENC_MASK(name) \
  ((FLARE_##name##_RSMASK) << (FLARE_##name##_BITPOS))

#define FLARE_GRP_BITSIZE (3ull)
#define FLARE_GRP_RSMASK \
  (FLARE_ENC_RSMASK (GRP))

#define FLARE_GRP_16_BITPOS \
  (16ull - FLARE_GRP_BITSIZE)
#define FLARE_GRP_16_MASK \
  ((FLARE_GRP_RSMASK) << (FLARE_GRP_16_BITPOS))
  //(FLARE_ENC_MASK (GRP_16))

#define FLARE_GRP_32_BITPOS \
  (32ull - FLARE_GRP_BITSIZE)
#define FLARE_GRP_32_MASK \
  ((FLARE_GRP_RSMASK) << (FLARE_GRP_32_BITPOS))
  //(FLARE_ENC_MASK (GRP_32))

#define FLARE_FULLGRP_VALUE(subgrp_inner_name, grp_value, subgrp_value) \
  ( \
    (((grp_value) & FLARE_GRP_RSMASK) \
      << (FLARE_##subgrp_inner_name##_BITSIZE)) \
    | ((subgrp_value) & (FLARE_##subgrp_inner_name##_RSMASK)) \
  )
/* -------- */
#define FLARE_G0_GRP_VALUE (0x0ull)
/* -------- */
/* -------- */
/* `pre` subgrp */
#define FLARE_G0_PRE_SUBGRP_BITSIZE (1ull)
#define FLARE_G0_PRE_SUBGRP_BITPOS \
  (FLARE_GRP_16_BITPOS - FLARE_G0_PRE_SUBGRP_BITSIZE)
#define FLARE_G0_PRE_SUBGRP_RSMASK \
  (FLARE_ENC_RSMASK (G0_PRE_SUBGRP))
#define FLARE_G0_PRE_SUBGRP_MASK \
  (FLARE_ENC_MASK (G0_PRE_SUBGRP))


/* `pre` full group */
#define FLARE_G0_PRE_FULLGRP_BITSIZE \
  (FLARE_GRP_BITSIZE + FLARE_G0_PRE_SUBGRP_BITSIZE)
#define FLARE_G0_PRE_FULLGRP_BITPOS \
  (FLARE_G0_PRE_SUBGRP_BITPOS)
#define FLARE_G0_PRE_FULLGRP_RSMASK \
  (FLARE_ENC_RSMASK (G0_PRE_FULLGRP))
#define FLARE_G0_PRE_FULLGRP_MASK \
  (FLARE_ENC_MASK (G0_PRE_FULLGRP))

/* `pre`-specific group values */
#define FLARE_G0_PRE_SUBGRP_VALUE (0x0ull)
#define FLARE_G0_PRE_FULLGRP_VALUE \
  (FLARE_FULLGRP_VALUE(G0_PRE_SUBGRP, \
    FLARE_G0_GRP_VALUE, \
    FLARE_G0_PRE_SUBGRP_VALUE))

/* `pre` simm12 */
#define FLARE_G0_PRE_S12_BITSIZE (12ull)
#define FLARE_G0_PRE_S12_BITPOS (0ull)
#define FLARE_G0_PRE_S12_RSMASK \
  (FLARE_ENC_RSMASK (G0_PRE_S12))
#define FLARE_G0_PRE_S12_MASK \
  (FLARE_ENC_MASK (G0_PRE_S12))
/* -------- */
/* `lpre` subgrp */
#define FLARE_G0_LPRE_SUBGRP_BITSIZE (2ull)
#define FLARE_G0_LPRE_SUBGRP_BITPOS \
  (FLARE_GRP_32_BITPOS - FLARE_G0_LPRE_SUBGRP_BITSIZE)
#define FLARE_G0_LPRE_SUBGRP_RSMASK \
  (FLARE_ENC_RSMASK (G0_LPRE_SUBGRP))
#define FLARE_G0_LPRE_SUBGRP_MASK \
  (FLARE_ENC_MASK (G0_LPRE_SUBGRP))

/* `lpre` full group */
#define FLARE_G0_LPRE_FULLGRP_BITSIZE \
  (FLARE_GRP_BITSIZE + FLARE_G0_LPRE_SUBGRP_BITSIZE)
#define FLARE_G0_LPRE_FULLGRP_BITPOS (FLARE_G0_LPRE_SUBGRP_BITPOS)
#define FLARE_G0_LPRE_FULLGRP_RSMASK \
  (FLARE_ENC_RSMASK (G0_LPRE_FULLGRP))
#define FLARE_G0_LPRE_FULLGRP_MASK \
  (FLARE_ENC_MASK (G0_LPRE_FULLGRP))

/* `lpre` (top 16-bit half) subgrp */
#define FLARE_G0_LPRE_SUBGRP_16_BITPOS \
  (FLARE_GRP_16_BITPOS - FLARE_G0_LPRE_SUBGRP_BITSIZE)
#define FLARE_G0_LPRE_SUBGRP_16_MASK \
  ((FLARE_G0_LPRE_SUBGRP_RSMASK) << (FLARE_G0_LPRE_SUBGRP_16_BITPOS))

/* `lpre` (top 16-bit half) full group */
#define FLARE_G0_LPRE_FULLGRP_16_BITPOS \
  (FLARE_G0_LPRE_SUBGRP_16_BITPOS)
#define FLARE_G0_LPRE_FULLGRP_16_MASK \
  ((FLARE_G0_LPRE_FULLGRP_RSMASK) << (FLARE_G0_LPRE_FULLGRP_16_BITPOS))

/* `lpre`-specific group values */
#define FLARE_G0_LPRE_SUBGRP_VALUE (0x2ull)
#define FLARE_G0_LPRE_FULLGRP_VALUE \
  (FLARE_FULLGRP_VALUE(G0_LPRE_SUBGRP, \
    FLARE_G0_GRP_VALUE, \
    FLARE_G0_LPRE_SUBGRP_VALUE))

//#define FLARE_LPRE_GRP_16_MASK (FLARE_LPRE_GRP_RSMASK << FLARE_LPRE_GRP_BITPOS_16)
//#define FLARE_LPRE_GRP_MASK (FLARE_LPRE_GRP_RSMASK << FLARE_LPRE_GRP_BITPOS)

/* `lpre` simm27, when used with an instruction from
  group 1, 5, or 6 */
#define FLARE_G0_LPRE_S27_BITSIZE (27ull)
#define FLARE_G0_LPRE_S27_BITPOS (0ull)
#define FLARE_G0_LPRE_S27_RSMASK \
  (FLARE_ENC_RSMASK (G0_LPRE_S27))
#define FLARE_G0_LPRE_S27_MASK \
  (FLARE_ENC_MASK (G0_LPRE_S27))

/* `lpre` simm23, when used with an instruction from group 3 */
#define FLARE_G0_LPRE_S23_BITSIZE (23ull)
#define FLARE_G0_LPRE_S23_BITPOS (0ull)
#define FLARE_G0_LPRE_S23_RSMASK \
  (FLARE_ENC_RSMASK (G0_LPRE_S23))
#define FLARE_G0_LPRE_S23_MASK \
  (FLARE_ENC_MASK (G0_LPRE_S23))

/* `lpre` simm27, when used with an `icreload` instruction from group 7 */
#define FLARE_G7_ICRELOAD_G0_LPRE_S27_BITSIZE (27ull)
#define FLARE_G7_ICRELOAD_G0_LPRE_S27_BITPOS (0ull)
#define FLARE_G7_ICRELOAD_G0_LPRE_S27_RSMASK \
  (FLARE_ENC_RSMASK (G7_ICRELOAD_G0_LPRE_S27))
#define FLARE_G7_ICRELOAD_G0_LPRE_S27_MASK \
  (FLARE_ENC_MASK (G7_ICRELOAD_G0_LPRE_S27))
/* -------- */
/* simm5, for when part of an instruction from groups 1, 5, or 6 */
#define FLARE_G1G5G6_I5_BITSIZE (5ull)
#define FLARE_G1G5G6_I5_BITPOS (8ull)
#define FLARE_G1G5G6_I5_RSMASK \
  (FLARE_ENC_RSMASK (G1G5G6_I5))
#define FLARE_G1G5G6_I5_MASK \
  (FLARE_ENC_MASK (G1G5G6_I5))
/* -------- */
/* the `grp` field of group 1 instructions */
#define FLARE_G1_GRP_VALUE (0x1ull)

/* the `opcode` field of group 1 instructions */
#define FLARE_G1_OP_BITSIZE (4ull)
#define FLARE_G1_OP_BITPOS (4ull)
#define FLARE_G1_OP_RSMASK \
  (FLARE_ENC_RSMASK (G1_OP))
#define FLARE_G1_OP_MASK \
  (FLARE_ENC_MASK (G1_OP))

/* the specific opcode values usable with group 1 instructions */
#define FLARE_G1_OP_ENUM_ADD_RA_S5 (0x0ull)
#define FLARE_G1_OP_ENUM_ADD_RA_PC_S5 (0x1ull)
#define FLARE_G1_OP_ENUM_ADD_RA_SP_S5 (0x2ull)
#define FLARE_G1_OP_ENUM_ADD_RA_FP_S5 (0x3ull)
#define FLARE_G1_OP_ENUM_CMP_RA_S5 (0x4ull)
#define FLARE_G1_OP_ENUM_CPY_RA_S5 (0x5ull)
#define FLARE_G1_OP_ENUM_LSL_RA_U5 (0x6ull)
#define FLARE_G1_OP_ENUM_LSR_RA_U5 (0x7ull)
#define FLARE_G1_OP_ENUM_ASR_RA_U5 (0x8ull)
#define FLARE_G1_OP_ENUM_AND_RA_S5 (0x9ull)
#define FLARE_G1_OP_ENUM_ORR_RA_S5 (0xaull)
#define FLARE_G1_OP_ENUM_XOR_RA_S5 (0xbull)
#define FLARE_G1_OP_ENUM_ZE_RA_U5 (0xcull)
#define FLARE_G1_OP_ENUM_SE_RA_U5 (0xdull)
#define FLARE_G1_OP_ENUM_SWI_RA_S5 (0xeull)
#define FLARE_G1_OP_ENUM_SWI_U5 (0xfull)
/* -------- */
/* the `grp` field of group 2 instructions */
#define FLARE_G2_GRP_VALUE (0x2ull)

/* the `flag` field of group 1 instructions */
#define FLARE_G2_F_BITSIZE (1ull)
#define FLARE_G2_F_BITPOS (12ull)
#define FLARE_G2_F_RSMASK \
  (FLARE_ENC_RSMASK (G2_F))
#define FLARE_G2_F_MASK \
  (FLARE_ENC_MASK (G2_F))

/* the `opcode` field of group 2 instructions */
#define FLARE_G2_OP_BITSIZE (4ull)
#define FLARE_G2_OP_BITPOS (8ull)
#define FLARE_G2_OP_RSMASK \
  (FLARE_ENC_RSMASK (G2_OP))
#define FLARE_G2_OP_MASK \
  (FLARE_ENC_MASK (G2_OP))

/* the specific opcode values usable with group 2 instructions */
#define FLARE_G2_OP_ENUM_ADD_RA_RB (0x0ull)
#define FLARE_G2_OP_ENUM_SUB_RA_RB (0x1ull)
#define FLARE_G2_OP_ENUM_ADD_RA_SP_RB (0x2ull)
#define FLARE_G2_OP_ENUM_ADD_RA_FP_RB (0x3ull)
#define FLARE_G2_OP_ENUM_CMP_RA_RB (0x4ull)
#define FLARE_G2_OP_ENUM_CPY_RA_RB (0x5ull)
#define FLARE_G2_OP_ENUM_LSL_RA_RB (0x6ull)
#define FLARE_G2_OP_ENUM_LSR_RA_RB (0x7ull)
#define FLARE_G2_OP_ENUM_ASR_RA_RB (0x8ull)
#define FLARE_G2_OP_ENUM_AND_RA_RB (0x9ull)
#define FLARE_G2_OP_ENUM_ORR_RA_RB (0xaull)
#define FLARE_G2_OP_ENUM_XOR_RA_RB (0xbull)
#define FLARE_G2_OP_ENUM_ADC_RA_RB (0xcull)
#define FLARE_G2_OP_ENUM_SBC_RA_RB (0xdull)
#define FLARE_G2_OP_ENUM_CMPBC_RA_RB (0xeull)
#define FLARE_G2_OP_ENUM_RESERVED_15 (0xfull)
/* -------- */
/* the `grp` field of group 3 instructions */
#define FLARE_G3_GRP_VALUE (0x3ull)

/* the PC-relative `simm9` field of group 3 instructions */
#define FLARE_G3_S9_BITSIZE (9ull)
#define FLARE_G3_S9_BITPOS (4ull)
#define FLARE_G3_S9_RSMASK \
  (FLARE_ENC_RSMASK (G3_S9))
#define FLARE_G3_S9_MASK \
  (FLARE_ENC_MASK (G3_S9))

/* the `opcode` field of group 1 instructions */
#define FLARE_G3_OP_BITSIZE (4ull)
#define FLARE_G3_OP_BITPOS (0ull)
#define FLARE_G3_OP_RSMASK \
  (FLARE_ENC_RSMASK (G3_OP))
#define FLARE_G3_OP_MASK \
  (FLARE_ENC_MASK (G3_OP))

/* the specific opcode values usable with group 1 instructions */
#define FLARE_G3_OP_ENUM_BL_PCREL_S9 (0x0ull)
#define FLARE_G3_OP_ENUM_BRA_PCREL_S9 (0x1ull)
#define FLARE_G3_OP_ENUM_BEQ_PCREL_S9 (0x2ull)
#define FLARE_G3_OP_ENUM_BNE_PCREL_S9 (0x3ull)
#define FLARE_G3_OP_ENUM_BMI_PCREL_S9 (0x4ull)
#define FLARE_G3_OP_ENUM_BPL_PCREL_S9 (0x5ull)
#define FLARE_G3_OP_ENUM_BVS_PCREL_S9 (0x6ull)
#define FLARE_G3_OP_ENUM_BVC_PCREL_S9 (0x7ull)
#define FLARE_G3_OP_ENUM_BGEU_PCREL_S9 (0x8ull)
#define FLARE_G3_OP_ENUM_BLTU_PCREL_S9 (0x9ull)
#define FLARE_G3_OP_ENUM_BGTU_PCREL_S9 (0xaull)
#define FLARE_G3_OP_ENUM_BLEU_PCREL_S9 (0xbull)
#define FLARE_G3_OP_ENUM_BGES_PCREL_S9 (0xcull)
#define FLARE_G3_OP_ENUM_BLTS_PCREL_S9 (0xdull)
#define FLARE_G3_OP_ENUM_BGTS_PCREL_S9 (0xeull)
#define FLARE_G3_OP_ENUM_BLES_PCREL_S9 (0xfull)

/* The offset used in calcuations of a branch's target address.
  The processor adds this value to the encoded immediate field of relative
  branches during branch target address computation. */
#define FLARE_PCREL_S9_BASE_OFFSET (0x2ull)
/* -------- */
/* the `grp` field of group 4 instructions */
#define FLARE_G4_GRP_VALUE (0x4ull)

/* the `opcode` field of group 4 instructions */
#define FLARE_G4_OP_BITSIZE (5ull)
#define FLARE_G4_OP_BITPOS (8ull)
#define FLARE_G4_OP_RSMASK \
  (FLARE_ENC_RSMASK (G4_OP))
#define FLARE_G4_OP_MASK \
  (FLARE_ENC_MASK (G4_OP))

/* the specific opcode values usable with group 4 instructions */
#define FLARE_G4_OP_ENUM_JL_RA (0x0ull)
#define FLARE_G4_OP_ENUM_JMP_RA (0x1ull)
//#define FLARE_G4_OP_ENUM_JMP_RA_RB (0x2ull)
#define FLARE_G4_OP_ENUM_JMP_IRA (0x2ull)
#define FLARE_G4_OP_ENUM_RETI (0x3ull)
#define FLARE_G4_OP_ENUM_EI (0x4ull)
#define FLARE_G4_OP_ENUM_DI (0x5ull)
#define FLARE_G4_OP_ENUM_PUSH_RA_RB (0x6ull)
#define FLARE_G4_OP_ENUM_PUSH_SA_RB (0x7ull)
#define FLARE_G4_OP_ENUM_POP_RA_RB (0x8ull)
#define FLARE_G4_OP_ENUM_POP_SA_RB (0x9ull)
#define FLARE_G4_OP_ENUM_POP_PC_RB (0xaull)
#define FLARE_G4_OP_ENUM_MUL_RA_RB (0xbull)
#define FLARE_G4_OP_ENUM_UDIV_RA_RB (0xcull)
#define FLARE_G4_OP_ENUM_SDIV_RA_RB (0xdull)
#define FLARE_G4_OP_ENUM_UMOD_RA_RB (0xeull)
#define FLARE_G4_OP_ENUM_SMOD_RA_RB (0xfull)

#define FLARE_G4_OP_ENUM_LUMUL_RA_RB (0x10ull)
#define FLARE_G4_OP_ENUM_LSMUL_RA_RB (0x11ull)
#define FLARE_G4_OP_ENUM_UDIV64_RA_RB (0x12ull)
#define FLARE_G4_OP_ENUM_SDIV64_RA_RB (0x13ull)
#define FLARE_G4_OP_ENUM_UMOD64_RA_RB (0x14ull)
#define FLARE_G4_OP_ENUM_SMOD64_RA_RB (0x15ull)
#define FLARE_G4_OP_ENUM_LDUB_RA_RB (0x16ull)
#define FLARE_G4_OP_ENUM_LDSB_RA_RB (0x17ull)
#define FLARE_G4_OP_ENUM_LDUH_RA_RB (0x18ull)
#define FLARE_G4_OP_ENUM_LDSH_RA_RB (0x19ull)
#define FLARE_G4_OP_ENUM_STB_RA_RB (0x1aull)
#define FLARE_G4_OP_ENUM_STH_RA_RB (0x1bull)
#define FLARE_G4_OP_ENUM_CPY_RA_SB (0x1cull)
#define FLARE_G4_OP_ENUM_CPY_SA_RB (0x1dull)
#define FLARE_G4_OP_ENUM_CPY_SA_SB (0x1eull)
#define FLARE_G4_OP_ENUM_INDEX_RA (0x1full)
/* -------- */
/* the `grp` field of group 5 instructions */
#define FLARE_G5_GRP_VALUE (0x5ull)
/* -------- */
/* the `grp` field of group 6 instructions */
#define FLARE_G6_GRP_VALUE (0x6ull)
/* -------- */
/* the `grp` field of group 7 instructions */
#define FLARE_G7_GRP_VALUE (0x7ull)
/* -------- */
/* The `subgrp` field of 8-bit/16-bit ALU Ops in group 7 */
#define FLARE_G7_ALUOPBH_SUBGRP_BITSIZE (2ull)
#define FLARE_G7_ALUOPBH_SUBGRP_BITPOS \
  (FLARE_GRP_16_BITPOS - FLARE_G7_ALUOPBH_SUBGRP_BITSIZE)
#define FLARE_G7_ALUOPBH_SUBGRP_RSMASK \
  (FLARE_ENC_RSMASK (G7_ALUOPBH_SUBGRP))
#define FLARE_G7_ALUOPBH_SUBGRP_MASK \
  (FLARE_ENC_MASK (G7_ALUOPBH_SUBGRP))

#define FLARE_G7_ALUOPBH_FULLGRP_BITSIZE \
  (FLARE_GRP_BITSIZE + FLARE_G7_ALUOPBH_SUBGRP_BITSIZE)
#define FLARE_G7_ALUOPBH_FULLGRP_BITPOS \
  (FLARE_G7_ALUOPBH_SUBGRP_BITPOS)
#define FLARE_G7_ALUOPBH_FULLGRP_RSMASK \
  (FLARE_ENC_RSMASK (G7_ALUOPBH_FULLGRP))
#define FLARE_G7_ALUOPBH_FULLGRP_MASK \
  (FLARE_ENC_MASK (G7_ALUOPBH_FULLGRP))

/* the `width` field of 8-bit/16-bit ALU ops in group 7 */
#define FLARE_G7_ALUOPBH_W_BITSIZE (1ull)
#define FLARE_G7_ALUOPBH_W_BITPOS (10ull)
#define FLARE_G7_ALUOPBH_W_RSMASK \
  (FLARE_ENC_RSMASK (G7_ALUOPBH_W))
#define FLARE_G7_ALUOPBH_W_MASK \
  (FLARE_ENC_MASK (G7_ALUOPBH_W))

/* the specific width values usable with 8-bit/16-bit ALU Ops in 
  group 7 */
#define FLARE_G7_ALUOPBH_W_ENUM_8 (0x0ull)
#define FLARE_G7_ALUOPBH_W_ENUM_16 (0x1ull)

/* the `opcode` field of 8-bit/16-bit ALU Ops in group 7 */
#define FLARE_G7_ALUOPBH_OP_BITSIZE (2ull)
#define FLARE_G7_ALUOPBH_OP_BITPOS (8ull)
#define FLARE_G7_ALUOPBH_OP_RSMASK \
  (FLARE_ENC_RSMASK (G7_ALUOPBH_OP))
#define FLARE_G7_ALUOPBH_OP_MASK \
  (FLARE_ENC_MASK (G7_ALUOPBH_OP))

/* the specific opcode values usable with 8-bit/16-bit ALU Ops in 
  group 7 */
#define FLARE_G7_ALUOPBH_OP_ENUM_CMP_RA_RB (0x0ull)
#define FLARE_G7_ALUOPBH_OP_ENUM_LSR_RA_RB (0x1ull)
#define FLARE_G7_ALUOPBH_OP_ENUM_ASR_RA_RB (0x2ull)
#define FLARE_G7_ALUOPBH_OP_ENUM_RESERVED_3 (0x3ull)

/* Constant fields of Group 7 8-bit/16-bit ALU Ops */
#define FLARE_G7_ALUOPBH_SUBGRP_VALUE (0x0ull)
#define FLARE_G7_ALUOPBH_FULLGRP_VALUE \
  (FLARE_FULLGRP_VALUE(G7_ALUOPBH_SUBGRP, \
    FLARE_G7_GRP_VALUE, \
    FLARE_G7_ALUOPBH_SUBGRP_VALUE))

/* -------- */

/* The `subgrp` field of the extra spr-using `ldr`/`str` instructions in
  group 7 */
#define FLARE_G7_SPRLDST_SUBGRP_BITSIZE (3ull)
#define FLARE_G7_SPRLDST_SUBGRP_BITPOS \
  (FLARE_GRP_16_BITPOS - FLARE_G7_SPRLDST_SUBGRP_BITSIZE)
#define FLARE_G7_SPRLDST_SUBGRP_RSMASK \
  (FLARE_ENC_RSMASK (G7_SPRLDST_SUBGRP))
#define FLARE_G7_SPRLDST_SUBGRP_MASK \
  (FLARE_ENC_MASK (G7_SPRLDST_SUBGRP))

#define FLARE_G7_SPRLDST_FULLGRP_BITSIZE \
  (FLARE_GRP_BITSIZE + FLARE_G7_SPRLDST_SUBGRP_BITSIZE)
#define FLARE_G7_SPRLDST_FULLGRP_BITPOS \
  (FLARE_G7_SPRLDST_SUBGRP_BITPOS)
#define FLARE_G7_SPRLDST_FULLGRP_RSMASK \
  (FLARE_ENC_RSMASK (G7_SPRLDST_FULLGRP))
#define FLARE_G7_SPRLDST_FULLGRP_MASK \
  (FLARE_ENC_MASK (G7_SPRLDST_FULLGRP))

/* the `opcode` field of 8-bit/16-bit ALU Ops in group 7 */
#define FLARE_G7_SPRLDST_OP_BITSIZE (2ull)
#define FLARE_G7_SPRLDST_OP_BITPOS (8ull)
#define FLARE_G7_SPRLDST_OP_RSMASK \
  (FLARE_ENC_RSMASK (G7_SPRLDST_OP))
#define FLARE_G7_SPRLDST_OP_MASK \
  (FLARE_ENC_MASK (G7_SPRLDST_OP))

/* the specific opcode values usable with spr-using `ldr`/`str` Ops in 
  group 7 */
#define FLARE_G7_SPRLDST_OP_ENUM_LDR_SA_RB (0x0ull)
#define FLARE_G7_SPRLDST_OP_ENUM_LDR_SA_SB (0x1ull)
#define FLARE_G7_SPRLDST_OP_ENUM_STR_SA_RB (0x2ull)
#define FLARE_G7_SPRLDST_OP_ENUM_STR_SA_SB (0x3ull)

/* Constant fields of Group 7 spr-using `ldr`/`str` instructions */
#define FLARE_G7_SPRLDST_SUBGRP_VALUE (0x02ull)
#define FLARE_G7_SPRLDST_FULLGRP_VALUE \
  (FLARE_FULLGRP_VALUE(G7_SPRLDST_SUBGRP, \
    FLARE_G7_GRP_VALUE, \
    FLARE_G7_SPRLDST_SUBGRP_VALUE))

/* -------- */

/* The `subgrp` field of the `icreload` instruction in group 7 */
#define FLARE_G7_ICRELOAD_SUBGRP_BITSIZE (4ull)
#define FLARE_G7_ICRELOAD_SUBGRP_BITPOS \
  (FLARE_GRP_16_BITPOS - FLARE_G7_ICRELOAD_SUBGRP_BITSIZE)
#define FLARE_G7_ICRELOAD_SUBGRP_RSMASK \
  (FLARE_ENC_RSMASK (G7_ICRELOAD_SUBGRP))
#define FLARE_G7_ICRELOAD_SUBGRP_MASK \
  (FLARE_ENC_MASK (G7_ICRELOAD_SUBGRP))

#define FLARE_G7_ICRELOAD_FULLGRP_BITSIZE \
  (FLARE_GRP_BITSIZE + FLARE_G7_ICRELOAD_SUBGRP_BITSIZE)
#define FLARE_G7_ICRELOAD_FULLGRP_BITPOS \
  (FLARE_G7_ICRELOAD_SUBGRP_BITPOS)
#define FLARE_G7_ICRELOAD_FULLGRP_RSMASK \
  (FLARE_ENC_RSMASK (G7_ICRELOAD_FULLGRP))
#define FLARE_G7_ICRELOAD_FULLGRP_MASK \
  (FLARE_ENC_MASK (G7_ICRELOAD_FULLGRP))

/* simm5, for when part of the `icreload` instruction from group 7 */
#define FLARE_G7_ICRELOAD_S5_BITSIZE (5ull)
#define FLARE_G7_ICRELOAD_S5_BITPOS (4ull)
#define FLARE_G7_ICRELOAD_S5_RSMASK \
  (FLARE_ENC_RSMASK (G7_ICRELOAD_S5))
#define FLARE_G7_ICRELOAD_S5_MASK \
  (FLARE_ENC_MASK (G7_ICRELOAD_S5))

/* Constant fields of Group 7 the `icreload` instruction */
#define FLARE_G7_ICRELOAD_SUBGRP_VALUE (0x06ull)
#define FLARE_G7_ICRELOAD_FULLGRP_VALUE \
  (FLARE_FULLGRP_VALUE(G7_ICRELOAD_SUBGRP, \
    FLARE_G7_GRP_VALUE, \
    FLARE_G7_ICRELOAD_SUBGRP_VALUE))

/* -------- */
/* encoding of rA */
#define FLARE_RA_IND_BITSIZE (4ull)
#define FLARE_RA_IND_BITPOS (0ull)
#define FLARE_RA_IND_RSMASK \
  (FLARE_ENC_RSMASK (RA_IND))
#define FLARE_RA_IND_MASK \
  (FLARE_ENC_MASK (RA_IND))

/* encoding of rB */
#define FLARE_RB_IND_BITSIZE (4ull)
#define FLARE_RB_IND_BITPOS (4ull)
#define FLARE_RB_IND_RSMASK \
  (FLARE_ENC_RSMASK (RB_IND))
#define FLARE_RB_IND_MASK \
  (FLARE_ENC_MASK (RB_IND))
/* -------- */
#define FLARE_INDEX_LPRE_EXT_BITPOS (48ull)

/* the left shift amount for a single `pre`/`lpre`/`index` when storing a
  "whole" instruction in a `flare_temp_t` */
#define FLARE_ONE_EXT_BITPOS (16ull)
#define FLARE_NO_EXT_MASK ((1ull << FLARE_ONE_EXT_BITPOS) - 1ull)
#define FLARE_ONE_EXT_MASK (~(FLARE_NO_EXT_MASK))

#define FLARE_G0_PRE_S12_EXT_LSMASK \
  (FLARE_G0_PRE_S12_MASK << FLARE_ONE_EXT_BITPOS)
#define FLARE_G0_LPRE_S27_EXT \
  (FLARE_G0_LPRE_S27_MASK << FLARE_ONE_EXT_BITPOS)
#define FLARE_G0_LPRE_S23_EXT_LSMASK \
  (FLARE_G0_LPRE_S23_MASK << FLARE_ONE_EXT_BITPOS)
#define FLARE_G0_LPRE_S27_EXT_LSMASK \
  (FLARE_G7_ICRELOAD_G0_LPRE_S27_MASK << FLARE_ONE_EXT_BITPOS)
/* -------- */
typedef enum flare_reg_kind_t
{
  FLARE_REG_KIND_GPR,
  FLARE_REG_KIND_SPR,
  FLARE_REG_KIND_PC,
} flare_reg_kind_t;

typedef struct flare_reg_t
{
  const char *name;
  flare_temp_t index;
  flare_reg_kind_t kind;
} flare_reg_t;
#define FLARE_REG_NAME_MAX_LEN (10ull)
/* general-purpose registers */
#define FLARE_NUM_GPRS (16ull)
extern const flare_reg_t gprs[FLARE_NUM_GPRS];

#define FLARE_GPR_ENUM_R0 (0x0ull)
#define FLARE_GPR_ENUM_R1 (0x1ull)
#define FLARE_GPR_ENUM_R2 (0x2ull)
#define FLARE_GPR_ENUM_R3 (0x3ull)
#define FLARE_GPR_ENUM_R4 (0x4ull)
#define FLARE_GPR_ENUM_R5 (0x5ull)
#define FLARE_GPR_ENUM_R6 (0x6ull)
#define FLARE_GPR_ENUM_R7 (0x7ull)
#define FLARE_GPR_ENUM_R8 (0x8ull)
#define FLARE_GPR_ENUM_R9 (0x9ull)
#define FLARE_GPR_ENUM_R10 (0xaull)
#define FLARE_GPR_ENUM_R11 (0xbull)
#define FLARE_GPR_ENUM_R12 (0xcull)
#define FLARE_GPR_ENUM_LR (0xdull)
#define FLARE_GPR_ENUM_FP (0xeull)
#define FLARE_GPR_ENUM_SP (0xfull)

#define FLARE_INST_GPRS() \
  { \
    {"r0", FLARE_GPR_ENUM_R0, FLARE_REG_KIND_GPR}, \
    {"r1", FLARE_GPR_ENUM_R1, FLARE_REG_KIND_GPR}, \
    {"r2", FLARE_GPR_ENUM_R2, FLARE_REG_KIND_GPR}, \
    {"r3", FLARE_GPR_ENUM_R3, FLARE_REG_KIND_GPR}, \
    {"r4", FLARE_GPR_ENUM_R4, FLARE_REG_KIND_GPR}, \
    {"r5", FLARE_GPR_ENUM_R5, FLARE_REG_KIND_GPR}, \
    {"r6", FLARE_GPR_ENUM_R6, FLARE_REG_KIND_GPR}, \
    {"r7", FLARE_GPR_ENUM_R7, FLARE_REG_KIND_GPR}, \
    {"r8", FLARE_GPR_ENUM_R8, FLARE_REG_KIND_GPR}, \
    {"r9", FLARE_GPR_ENUM_R9, FLARE_REG_KIND_GPR}, \
    {"r10", FLARE_GPR_ENUM_R10, FLARE_REG_KIND_GPR}, \
    {"r11", FLARE_GPR_ENUM_R11, FLARE_REG_KIND_GPR}, \
    {"r12", FLARE_GPR_ENUM_R12, FLARE_REG_KIND_GPR}, \
    {"lr", FLARE_GPR_ENUM_LR, FLARE_REG_KIND_GPR}, \
    {"fp", FLARE_GPR_ENUM_FP, FLARE_REG_KIND_GPR}, \
    {"sp", FLARE_GPR_ENUM_SP, FLARE_REG_KIND_GPR}, \
  }

/* special-purpose registers */
//#define FLARE_NUM_SPRS (16ull)
//#define FLARE_REAL_NUM_SPRS (8ull)
//#define FLARE_NUM_SPRS (8ull)
#define FLARE_NUM_SPRS (6ull)
extern const flare_reg_t sprs[FLARE_NUM_SPRS];

#define FLARE_SPR_ENUM_FLAGS (0x0ull)
//#define FLARE_SPR_ENUM_HI (0x1ull)
//#define FLARE_SPR_ENUM_LO (0x2ull)
#define FLARE_SPR_ENUM_IDS (0x1ull)
#define FLARE_SPR_ENUM_IRA (0x2ull)
#define FLARE_SPR_ENUM_IE (0x3ull)
#define FLARE_SPR_ENUM_ITY (0x4ull)
#define FLARE_SPR_ENUM_STY (0x5ull)

#define FLARE_INST_SPRS() \
  { \
    {"flags", FLARE_SPR_ENUM_FLAGS, FLARE_REG_KIND_SPR}, \
    /* {"hi", FLARE_SPR_ENUM_HI, FLARE_REG_KIND_SPR}, */ \
    /* {"lo", FLARE_SPR_ENUM_LO, FLARE_REG_KIND_SPR}, */ \
    {"ids", FLARE_SPR_ENUM_IDS, FLARE_REG_KIND_SPR}, \
    {"ira", FLARE_SPR_ENUM_IRA, FLARE_REG_KIND_SPR}, \
    {"ie", FLARE_SPR_ENUM_IE, FLARE_REG_KIND_SPR}, \
    {"ity", FLARE_SPR_ENUM_ITY, FLARE_REG_KIND_SPR}, \
    {"sty", FLARE_SPR_ENUM_STY, FLARE_REG_KIND_SPR}, \
  }

/* bits of `flags` */
#define FLARE_FLAGS_Z_BITPOS ((flare_temp_t) 0x0ull)
#define FLARE_FLAGS_C_BITPOS ((flare_temp_t) 0x1ull)
#define FLARE_FLAGS_V_BITPOS ((flare_temp_t) 0x2ull)
#define FLARE_FLAGS_N_BITPOS ((flare_temp_t) 0x3ull)

#define FLARE_FLAGS_Z_MASK \
  (((flare_temp_t) 0x1ull) << FLARE_FLAGS_Z_BITPOS) 
#define FLARE_FLAGS_C_MASK \
  (((flare_temp_t) 0x1ull) << FLARE_FLAGS_C_BITPOS) 
#define FLARE_FLAGS_V_MASK \
  (((flare_temp_t) 0x1ull) << FLARE_FLAGS_V_BITPOS) 
#define FLARE_FLAGS_N_MASK \
  (((flare_temp_t) 0x1ull) << FLARE_FLAGS_N_BITPOS) 

#define FLARE_SIM_FLAGS_VN_MASK(bits) \
  ((uint64_t) 0x1ull << (uint64_t) (bits - 1))
#define FLARE_SIM_FLAGS_Z_MASK(bits) \
  (FLARE_SIM_FLAGS_VN_MASK (bits) - (int64_t) 0x1ll)
#define FLARE_SIM_FLAGS_C_MASK(bits) \
  (FLARE_SIM_FLAGS_VN_MASK (bits) << (uint64_t) 0x1ull)

extern const flare_reg_t reg_pc;
#define FLARE_INST_REG_PC() \
  {"pc", 0x0, FLARE_REG_KIND_PC}

#define FLARE_SIM_FIRST_SPR_REGNO \
  (FLARE_NUM_GPRS)
#define FLARE_TOTAL_NUM_NON_PC_REGS \
  (FLARE_NUM_GPRS + FLARE_NUM_SPRS)
#define FLARE_TOTAL_NUM_REGS \
  (FLARE_TOTAL_NUM_NON_PC_REGS + 1ull)
/* -------- */
typedef enum flare_have_plp_t {
  FLARE_HAVE_PLP_NEITHER,
  FLARE_HAVE_PLP_PRE,
  FLARE_HAVE_PLP_LPRE,
} flare_have_plp_t;
static inline unsigned
flare_have_plp_insn_length (flare_have_plp_t have_plp)
{
  switch (have_plp)
  {
    case FLARE_HAVE_PLP_NEITHER:
      return 2;
    case FLARE_HAVE_PLP_PRE:
      return 4;
    case FLARE_HAVE_PLP_LPRE:
      return 6;
    default:
      abort ();
      return 0;
  }
}
static inline unsigned
flare_have_plp_distance
  (flare_have_plp_t from, flare_have_plp_t to)
{
  return (
    flare_have_plp_insn_length (from)
    - flare_have_plp_insn_length (to)
  );
}
typedef enum flare_oparg_t
{
  FLARE_OA_BAD,
  FLARE_OA_NONE,
  FLARE_OA_PRE,
  FLARE_OA_LPRE,
  FLARE_OA_RA_S5,
  FLARE_OA_RA_PC_S5,
  FLARE_OA_RA_SP_S5,
  FLARE_OA_RA_FP_S5,
  FLARE_OA_RA_U5,
  FLARE_OA_U5,
  FLARE_OA_RA,
  FLARE_OA_RA_RB,
  FLARE_OA_RA_SP_RB,
  FLARE_OA_RA_FP_RB,
  FLARE_OA_PCREL_S9,
  //FLARE_OA_PCREL_S32_NO_RELAX,
  FLARE_OA_IRA,
  FLARE_OA_RA_SB,
  FLARE_OA_SA_RB,
  FLARE_OA_SA_SB,
  FLARE_OA_PC_RB,
  FLARE_OA_RA_IMPLICIT_SP,
  FLARE_OA_SA_IMPLICIT_SP,
  FLARE_OA_PC_IMPLICIT_SP,
  FLARE_OA_RA_RB_LDST,
  FLARE_OA_RA_RB_LDST_32,
  FLARE_OA_RA_RB_RC_LDST,
  FLARE_OA_RA_RB_RC_LDST_32,
  FLARE_OA_RA_RB_S5_LDST,
  FLARE_OA_RA_RB_RC_S5_LDST,
  FLARE_OA_RA_RB_CPY64,
  FLARE_OA_SA_RB_LDST,
  FLARE_OA_SA_SB_LDST,
  FLARE_OA_RA_S5_JUSTADDR,
  FLARE_OA_RA_JUSTADDR,
  FLARE_OA_RA_RC_JUSTADDR,
  FLARE_OA_RA_RC_S5_JUSTADDR,
} flare_oparg_t;
//static inline bool
//flare_oparg_has_u5 (flare_oparg_t oparg)
//{
//  return (
//    (oparg == FLARE_OA_RA_U5) || (oparg == FLARE_OA_U5)
//  );
//}


typedef struct flare_grp_info_t
{
  const flare_enc_info_t
    *grp;
  unsigned
    //grp_bitsize,
    //grp_bitpos,
    //grp_rsmask,
    //grp_mask,
    grp_value; /* The primary group, i.e. the top three bits */

  const flare_enc_info_t
    *subgrp;
  unsigned
    //subgrp_bitsize,
    //subgrp_bitpos,
    //subgrp_rsmask,
    //subgrp_mask,
    subgrp_value;
} flare_grp_info_t;
//extern const flare_grp_info_t
//  flare_grp_info_g0_pre,
//  flare_grp_info_g0_lpre,
//  flare_grp_info_g1,
//  flare_grp_info_g2,
//  flare_grp_info_g3,
//  flare_grp_info_g4,
//  flare_grp_info_g5,
//  flare_grp_info_g6,
//  flare_grp_info_g7_aluopbh;

#define FLARE_OPC_INFO_NAME_MAX_LEN (15ull)
#define FLARE_OPC_INFO_NAMES_LIM (2ull)
typedef struct flare_opc_info_t
{
  const flare_grp_info_t *grp_info;
  flare_temp_t opcode;
  flare_oparg_t oparg;
  const char
    *names[FLARE_OPC_INFO_NAMES_LIM],
    *nr_names[FLARE_OPC_INFO_NAMES_LIM];
} flare_opc_info_t;

struct flare_dasm_info_t;
typedef int (*flare_dasm_info_rd16_func)
  (struct flare_dasm_info_t * /* self */);
typedef struct flare_dasm_info_t
{
  int
    length,
    status;
  bool
    grp_decode_err: 1,
    g7_subgrp_decode_err: 1;
  const flare_opc_info_t *opc_info;
  flare_temp_t
    iword,
    simm,
    grp,
    ra_ind,
    rb_ind,
    fw,
    g7_aluopbh_subgrp,
    g7_sprldst_subgrp,
    g7_icreload_subgrp;
  bfd_byte buffer[2];
  flare_dasm_info_rd16_func rd16_func;
} flare_dasm_info_t;

//#define FLARE_HTAB_KEY_BUF_LIM (64ull)
//extern char
//  flare_opc_info_htab_key_buf[FLARE_HTAB_KEY_BUF_LIM];
//extern const char *flare_opc_info_to_htab_key
//  (char *cbuf, unsigned cbuf_lim, const flare_opc_info_t *opc_info,
//  unsigned names_ind);
//extern const char *flare_op_oa_htab_key
//  (char *cbuf, unsigned cbuf_lim, const char *name, flare_oparg_t oparg);

// TODO: change this to use a `realloc ()`ed 2D dynamic array instead.
// For that, we can use an `htab_t` that maps instruction name keys to
// indices into 2D dynamic array. I think this would probably be faster?
//typedef struct flare_opci_list_t
//{
//  const flare_opc_info_t *opc_info;
//  struct flare_opci_list_t *next;
//} flare_opci_list_t;
//
//extern flare_opci_list_t *flare_opci_list_create (void);
//extern flare_opci_list_t *flare_opci_list_append
//  (flare_opci_list_t *self, const flare_opc_info_t *opc_info);
//extern void flare_opci_list_delete (flare_opci_list_t *self);

//typedef struct flare_opci_list2d_t
//{
//  struct flare_opci_list_t *list,
//  struct flare_opci_list2d_t *next;
//} flare_opci_list2d_t;
//extern flare_opci_list2d_t *flare_opci_list2d_create (void);
//extern flare_opci_list2d_t *flare_opci_list2d_append
//  (flare_opci_list2d_t *self, flare_opci_list_t *list);
//extern void flare_opci_list2d_delete (flare_opci_list2d_t *self);

typedef struct flare_opci_vec_t
{
  const flare_opc_info_t **data;
  size_t size;
} flare_opci_vec_t;

//extern flare_opci_vec_t *flare_opci_vec_create (void);
extern void flare_opci_vec_create (flare_opci_vec_t *self);
extern const flare_opc_info_t *flare_opci_vec_append
  (flare_opci_vec_t *self, const flare_opc_info_t *to_append);
extern void flare_opci_vec_delete_data (flare_opci_vec_t *self);

typedef struct flare_opci_v2d_t
{
  flare_opci_vec_t *data;
  size_t size;
} flare_opci_v2d_t;
//extern flare_opci_v2d_t *flare_opci_v2d_create (void);
extern void flare_opci_v2d_create (flare_opci_v2d_t *self);
extern flare_opci_vec_t *flare_opci_v2d_append
  (flare_opci_v2d_t *self, const flare_opci_vec_t *to_append);
//extern void flare_opci_v2d_append_opci (flare_opci_v2d_t *self,
//  const flare_opc_info_t *opc_info, size_t index);
extern void flare_opci_v2d_delete_data (flare_opci_v2d_t *self);

//typedef struct flare_relax_general_t
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
//} flare_relax_general_t;


#define FLARE_OPC_INFO_NULL_OP (-1ull)
//#define FLARE_OPC_INFO_PSEUDO_OP (-2ull)

#define FLARE_G0_OPC_INFO_LIM (2ull)
//extern const flare_opc_info_t
//  flare_opc_info_g0[FLARE_G0_OPC_INFO_LIM];

#define FLARE_G1_OPC_INFO_LIM (16ull + 1ull)
//extern const flare_opc_info_t
//  flare_opc_info_g1[FLARE_G1_OPC_INFO_LIM];

#define FLARE_G2_OPC_INFO_LIM (16ull)
//extern const flare_opc_info_t
//  flare_opc_info_g2[FLARE_G2_OPC_INFO_LIM];

#define FLARE_G3_OPC_INFO_LIM (16ull)
//#define FLARE_G3_OPC_INFO_LIM (16ull + 16ull)
//#define FLARE_G3_OPC_INFO_LIM (16ull + 1ull)
//extern const flare_opc_info_t
//  flare_opc_info_g3[FLARE_G3_OPC_INFO_LIM];

#define FLARE_G4_OPC_INFO_LIM (32ull + 5ull + 6ull)
//extern const flare_opc_info_t
//  flare_opc_info_g4[FLARE_G4_OPC_INFO_LIM];

#define FLARE_G5_OPC_INFO_LIM (4ull)
//extern const flare_opc_info_t
//  flare_opc_info_g5[FLARE_G5_OPC_INFO_LIM];

#define FLARE_G6_OPC_INFO_LIM (4ull)
//extern const flare_opc_info_t
//  flare_opc_info_g6[FLARE_G6_OPC_INFO_LIM];

#define FLARE_G7_ALUOPBH_OPC_INFO_LIM (4ull)
//extern const flare_opc_info_t
//  flare_opc_info_g7_aluopbh[FLARE_G7_ALUOPBH_OPC_INFO_LIM];

#define FLARE_G7_SPRLDST_OPC_INFO_LIM (4ull)
//extern const flare_opc_info_t
//  flare_opc_info_g7_sprldst[FLARE_G7_SPRLDST_OPC_INFO_LIM];

#define FLARE_G7_ICRELOAD_OPC_INFO_LIM (4ull)
//extern const flare_opc_info_t
//  flare_opc_info_g7_icreload[FLARE_G7_ICRELOAD_OPC_INFO_LIM];

/* This is definitely excessive, but it should work fine */
#define FLARE_MAX_UNIQUE_OPCI_NAMES (512ull)
/* -------- */
/* -------- */
static inline flare_temp_t
flare_enc_temp_insn_pre (flare_temp_t simm12)
{
  flare_temp_t ret = 0;
  //SET_INSN_FIELD(FLARE_PRE_FULLGRP_MASK, FLARE_G0_PRE_FULLGRP_BITPOS,
  //  ret, FLARE_G0_PRE_FULLGRP_VALUE);
  //SET_INSN_FIELD(FLARE_G0_PRE_S12_MASK, FLARE_G0_PRE_S12_BITPOS, ret,
  //  simm12);
  flare_set_insn_field_p (FLARE_G0_PRE_FULLGRP_MASK,
    FLARE_G0_PRE_FULLGRP_BITPOS, &ret,
    FLARE_G0_PRE_FULLGRP_VALUE);
  flare_set_insn_field_p (FLARE_G0_PRE_S12_MASK, 
    FLARE_G0_PRE_S12_BITPOS, &ret, simm12);
  return ret;
}
static inline flare_temp_t
flare_enc_temp_insn_g0_lpre_s27 (flare_temp_t simm27)
{
  flare_temp_t ret = 0;
  //SET_INSN_FIELD(FLARE_G0_LPRE_FULLGRP_MASK, FLARE_G0_LPRE_FULLGRP_BITPOS,
  //  ret, FLARE_G0_LPRE_FULLGRP_VALUE);
  //SET_INSN_FIELD(FLARE_G0_LPRE_S27_MASK,
  //  FLARE_G0_LPRE_S27_BITPOS, ret, simm27);
  flare_set_insn_field_p (FLARE_G0_LPRE_FULLGRP_MASK,
    FLARE_G0_LPRE_FULLGRP_BITPOS, &ret, FLARE_G0_LPRE_FULLGRP_VALUE);
  flare_set_insn_field_p (FLARE_G0_LPRE_S27_MASK,
    FLARE_G0_LPRE_S27_BITPOS, &ret, simm27);
  return ret;
}
static inline flare_temp_t
flare_enc_temp_insn_g0_lpre_s23 (flare_temp_t simm23)
{
  flare_temp_t ret = 0;
  //SET_INSN_FIELD(FLARE_G0_LPRE_FULLGRP_MASK, FLARE_G0_LPRE_FULLGRP_BITPOS,
  //  ret, FLARE_G0_LPRE_FULLGRP_VALUE);
  //SET_INSN_FIELD(FLARE_G0_LPRE_S23_MASK,
  //  FLARE_G0_LPRE_S23_BITPOS, ret, simm23);
  flare_set_insn_field_p (FLARE_G0_LPRE_FULLGRP_MASK,
    FLARE_G0_LPRE_FULLGRP_BITPOS, &ret, FLARE_G0_LPRE_FULLGRP_VALUE);
  flare_set_insn_field_p (FLARE_G0_LPRE_S23_MASK,
    FLARE_G0_LPRE_S23_BITPOS, &ret, simm23);
  return ret;
}
/* -------- */
static inline flare_temp_t
flare_enc_temp_insn_g1 (flare_temp_t op,
                          flare_temp_t ra_ind,
                          flare_temp_t simm5)
{
  flare_temp_t ret = 0;
  //SET_INSN_FIELD(FLARE_GRP_16_MASK, FLARE_GRP_16_BITPOS, ret,
  //  FLARE_G1_GRP_VALUE);
  //SET_INSN_FIELD(FLARE_G1G5G6_I5_MASK, FLARE_G1G5G6_I5_BITPOS, ret,
  //  simm5);
  //SET_INSN_FIELD(FLARE_G1_OP_MASK, FLARE_G1_OP_BITPOS, ret, op);
  //SET_INSN_FIELD(FLARE_RA_IND_MASK, FLARE_RA_IND_BITPOS, ret, ra_ind);
  flare_set_insn_field_p (FLARE_GRP_16_MASK, FLARE_GRP_16_BITPOS, 
    &ret, FLARE_G1_GRP_VALUE);
  flare_set_insn_field_p (FLARE_G1_OP_MASK, FLARE_G1_OP_BITPOS, 
    &ret, op);
  flare_set_insn_field_p (FLARE_RA_IND_MASK, FLARE_RA_IND_BITPOS, 
    &ret, ra_ind);
  flare_set_insn_field_p (FLARE_G1G5G6_I5_MASK, 
    FLARE_G1G5G6_I5_BITPOS, &ret, simm5);
  return ret;
}
/* -------- */
static inline flare_temp_t
flare_enc_temp_insn_g2 (flare_temp_t op,
                          flare_temp_t f,
                          flare_temp_t ra_ind,
                          flare_temp_t rb_ind)
{
  flare_temp_t ret = 0;
  //SET_INSN_FIELD(FLARE_GRP_16_MASK, FLARE_GRP_16_BITPOS, ret,
  //  FLARE_G2_GRP_VALUE);
  //SET_INSN_FIELD(FLARE_G2_F_MASK, FLARE_G2_F_BITPOS, ret, f);
  //SET_INSN_FIELD(FLARE_G2_OP_MASK, FLARE_G2_OP_BITPOS, ret, op);
  //SET_INSN_FIELD(FLARE_RB_IND_MASK, FLARE_RB_IND_BITPOS, ret, rb_ind);
  //SET_INSN_FIELD(FLARE_RA_IND_MASK, FLARE_RA_IND_BITPOS, ret, ra_ind);

  flare_set_insn_field_p (FLARE_GRP_16_MASK,
    FLARE_GRP_16_BITPOS, &ret, FLARE_G2_GRP_VALUE);
  flare_set_insn_field_p (FLARE_G2_OP_MASK, FLARE_G2_OP_BITPOS, 
    &ret, op);
  flare_set_insn_field_p (FLARE_G2_F_MASK, FLARE_G2_F_BITPOS,
    &ret, f);
  flare_set_insn_field_p (FLARE_RA_IND_MASK, FLARE_RA_IND_BITPOS, 
    &ret, ra_ind);
  flare_set_insn_field_p (FLARE_RB_IND_MASK, FLARE_RB_IND_BITPOS, 
    &ret, rb_ind);

  return ret;
}
/* -------- */
static inline flare_temp_t
flare_enc_temp_insn_g3 (flare_temp_t op,
                          flare_temp_t simm9)
{
  flare_temp_t ret = 0;
  //SET_INSN_FIELD(FLARE_GRP_16_MASK, FLARE_GRP_16_BITPOS, ret,
  //  FLARE_G3_GRP_VALUE);
  //SET_INSN_FIELD(FLARE_G3_S9_MASK, FLARE_G3_S9_BITPOS, ret, simm9);
  //SET_INSN_FIELD(FLARE_G3_OP_MASK, FLARE_G3_OP_BITPOS, ret, op);

  flare_set_insn_field_p (FLARE_GRP_16_MASK, FLARE_GRP_16_BITPOS, 
    &ret, FLARE_G3_GRP_VALUE);
  flare_set_insn_field_p (FLARE_G3_OP_MASK, FLARE_G3_OP_BITPOS, 
    &ret, op);
  flare_set_insn_field_p (FLARE_G3_S9_MASK, FLARE_G3_S9_BITPOS, 
    &ret, simm9);
  return ret;
}
/* -------- */
static inline flare_temp_t
flare_enc_temp_insn_g4 (flare_temp_t op,
                          flare_temp_t ra_ind,
                          flare_temp_t rb_ind)
{
  flare_temp_t ret = 0;
  //SET_INSN_FIELD(FLARE_GRP_16_MASK, FLARE_GRP_16_BITPOS, ret,
  //  FLARE_G4_GRP_VALUE);
  //SET_INSN_FIELD(FLARE_G4_OP_MASK, FLARE_G4_OP_BITPOS, ret, op);
  //SET_INSN_FIELD(FLARE_RB_IND_MASK, FLARE_RB_IND_BITPOS, ret, rb_ind);
  //SET_INSN_FIELD(FLARE_RA_IND_MASK, FLARE_RA_IND_BITPOS, ret, ra_ind);

  //ret |= (0x4ull << 13ull);
  flare_set_insn_field_p (FLARE_GRP_16_MASK, FLARE_GRP_16_BITPOS, 
    &ret, FLARE_G4_GRP_VALUE);
  flare_set_insn_field_p (FLARE_G4_OP_MASK, FLARE_G4_OP_BITPOS, 
    &ret, op);
  flare_set_insn_field_p (FLARE_RA_IND_MASK, FLARE_RA_IND_BITPOS, 
    &ret, ra_ind);
  flare_set_insn_field_p (FLARE_RB_IND_MASK, FLARE_RB_IND_BITPOS, 
    &ret, rb_ind);
  return ret;
}
/* -------- */
static inline flare_temp_t
flare_enc_temp_insn_g5 (flare_temp_t ra_ind,
                          flare_temp_t rb_ind,
                          flare_temp_t simm5)
{
  flare_temp_t ret = 0;
  //SET_INSN_FIELD(FLARE_GRP_16_MASK, FLARE_GRP_16_BITPOS, ret,
  //  FLARE_G5_GRP_VALUE);
  //SET_INSN_FIELD(FLARE_G1G5G6_I5_MASK, FLARE_G1G5G6_I5_BITPOS, ret,
  //  simm5);
  //SET_INSN_FIELD(FLARE_RB_IND_MASK, FLARE_RB_IND_BITPOS, ret, rb_ind);
  //SET_INSN_FIELD(FLARE_RA_IND_MASK, FLARE_RA_IND_BITPOS, ret, ra_ind);
  flare_set_insn_field_p (FLARE_GRP_16_MASK, FLARE_GRP_16_BITPOS, 
    &ret, FLARE_G5_GRP_VALUE);
  flare_set_insn_field_p (FLARE_RA_IND_MASK, FLARE_RA_IND_BITPOS, 
    &ret, ra_ind);
  flare_set_insn_field_p (FLARE_RB_IND_MASK, FLARE_RB_IND_BITPOS, 
    &ret, rb_ind);
  flare_set_insn_field_p (FLARE_G1G5G6_I5_MASK, 
    FLARE_G1G5G6_I5_BITPOS, &ret, simm5);
  return ret;
}
/* -------- */
static inline flare_temp_t
flare_enc_temp_insn_g6 (flare_temp_t ra_ind,
                          flare_temp_t rb_ind,
                          flare_temp_t simm5)
{
  flare_temp_t ret = 0;
  //SET_INSN_FIELD(FLARE_GRP_16_MASK, FLARE_GRP_16_BITPOS, ret,
  //  FLARE_G6_GRP_VALUE);
  //SET_INSN_FIELD(FLARE_G1G5G6_I5_MASK, FLARE_G1G5G6_I5_BITPOS, ret,
  //  simm5);
  //SET_INSN_FIELD(FLARE_RB_IND_MASK, FLARE_RB_IND_BITPOS, ret, rb_ind);
  //SET_INSN_FIELD(FLARE_RA_IND_MASK, FLARE_RA_IND_BITPOS, ret, ra_ind);

  flare_set_insn_field_p (FLARE_GRP_16_MASK, FLARE_GRP_16_BITPOS, 
    &ret, FLARE_G6_GRP_VALUE);
  flare_set_insn_field_p (FLARE_RA_IND_MASK, FLARE_RA_IND_BITPOS, 
    &ret, ra_ind);
  flare_set_insn_field_p (FLARE_RB_IND_MASK, FLARE_RB_IND_BITPOS, 
    &ret, rb_ind);
  flare_set_insn_field_p (FLARE_G1G5G6_I5_MASK, 
    FLARE_G1G5G6_I5_BITPOS, &ret, simm5);
  return ret;
}
/* -------- */
static inline flare_temp_t
flare_enc_temp_insn_g7_aluopbh (flare_temp_t op,
                                  flare_temp_t w,
                                  flare_temp_t ra_ind,
                                  flare_temp_t rb_ind)
{
  flare_temp_t ret = 0;
  flare_set_insn_field_p (FLARE_G7_ALUOPBH_FULLGRP_MASK,
    FLARE_G7_ALUOPBH_FULLGRP_BITPOS, &ret,
    FLARE_G7_ALUOPBH_FULLGRP_VALUE);
  flare_set_insn_field_p (FLARE_G7_ALUOPBH_OP_MASK,
    FLARE_G7_ALUOPBH_OP_BITPOS, &ret, op);
  flare_set_insn_field_p (FLARE_G7_ALUOPBH_W_MASK,
    FLARE_G7_ALUOPBH_W_BITPOS, &ret, w);
  flare_set_insn_field_p (FLARE_RA_IND_MASK, FLARE_RA_IND_BITPOS, 
    &ret, ra_ind);
  flare_set_insn_field_p (FLARE_RB_IND_MASK, FLARE_RB_IND_BITPOS, 
    &ret, rb_ind);
  return ret;
}
static inline flare_temp_t
flare_enc_temp_insn_g7_sprldst (flare_temp_t op,
                                  flare_temp_t ra_ind,
                                  flare_temp_t rb_ind)
{
  flare_temp_t ret = 0;
  flare_set_insn_field_p (FLARE_G7_SPRLDST_FULLGRP_MASK,
    FLARE_G7_SPRLDST_FULLGRP_BITPOS, &ret,
    FLARE_G7_SPRLDST_FULLGRP_VALUE);
  flare_set_insn_field_p (FLARE_G7_SPRLDST_OP_MASK,
    FLARE_G7_SPRLDST_OP_BITPOS, &ret, op);
  flare_set_insn_field_p (FLARE_RA_IND_MASK, FLARE_RA_IND_BITPOS, 
    &ret, ra_ind);
  flare_set_insn_field_p (FLARE_RB_IND_MASK, FLARE_RB_IND_BITPOS, 
    &ret, rb_ind);
  return ret;
}
static inline flare_temp_t
flare_enc_temp_insn_g7_icreload (flare_temp_t ra_ind,
                                  flare_temp_t simm5)
{
  flare_temp_t ret = 0;
  flare_set_insn_field_p (FLARE_G7_ICRELOAD_FULLGRP_MASK,
    FLARE_G7_ICRELOAD_FULLGRP_BITPOS, &ret,
    FLARE_G7_ICRELOAD_FULLGRP_VALUE);
  flare_set_insn_field_p (FLARE_G7_ICRELOAD_S5_MASK,
    FLARE_G7_ICRELOAD_S5_BITPOS, &ret,
    simm5);
  flare_set_insn_field_p (FLARE_RA_IND_MASK, FLARE_RA_IND_BITPOS,
    &ret, ra_ind);
  return ret;
}
/* -------- */
static inline flare_temp_t
flare_sign_extend (flare_temp_t value,
                    flare_temp_t bits)
{
  if (value & (1ull << (bits - 1ull)))
  {
    /* VALUE is negative.  */
    value |= ((flare_temp_t) -1ll) << bits;
  }
  else
  {
    value &= ~(((flare_temp_t) -1ll) << bits)
      | ((((flare_temp_t) 1ull) << bits) - 1ll);
  }

  return value;
}
static inline flare_temp_t
flare_zero_extend (flare_temp_t value,
                    flare_temp_t bits)
{
  value &= ~(((flare_temp_t) -1ll) << bits)
    | ((((flare_temp_t) 1ull) << bits) - 1ll);
  return value;
}

static inline flare_temp_t
flare_get_ext_imm (flare_temp_t prefix_mask,
                    flare_temp_t prefix_bitpos,
                    flare_temp_t prefix_insn,
                    flare_temp_t insn_mask,
                    flare_temp_t insn_bitsize,
                    flare_temp_t insn_bitpos,
                    flare_temp_t insn)
{
  //return (flare_temp_t) (
  //  (((prefix_insn & prefix_mask) >> prefix_bitpos) << insn_bitsize)
  //  | ((insn & insn_mask) >> insn_bitpos)
  //);
  return (flare_temp_t) (
    (flare_get_insn_field (prefix_mask, prefix_bitpos, prefix_insn)
      << insn_bitsize)
    | flare_get_insn_field (insn_mask, insn_bitpos, insn)
  );
}

//static inline flare_temp_t
//flare_get_ext_imm (const flare_enc_info_t *prefix_enc_info,
//                    flare_temp_t prefix_insn,
//                    const flare_enc_info_t *insn_enc_info,
//                    flare_temp_t insn)
//{
//  return flare_get_ext_imm_worker
//    (prefix_enc_info->mask, /* prefix_mask, */
//    prefix_enc_info->bitpos, /* prefix_bitpos, */
//    prefix_insn, /* prefix_insn, */
//    insn_enc_info->mask, /* insn_mask, */
//    insn_enc_info->bitsize, /* insn_bitsize, */
//    insn_enc_info->bitpos, /* insn_bitpos, */
//    insn /* insn */);
//}

static inline flare_temp_t
flare_get_g1g5g6_s17 (flare_temp_t prefix_insn,
                        flare_temp_t insn)
{
  return flare_get_ext_imm 
    (FLARE_G0_PRE_S12_MASK,
    FLARE_G0_PRE_S12_BITPOS,
    prefix_insn,
    FLARE_G1G5G6_I5_MASK,
    FLARE_G1G5G6_I5_BITSIZE,
    FLARE_G1G5G6_I5_BITPOS,
    insn);
}
static inline flare_temp_t
flare_get_g1g5g6_s32 (flare_temp_t prefix_insn,
                        flare_temp_t insn)
{
  return flare_get_ext_imm
    (FLARE_G0_LPRE_S27_MASK,
    FLARE_G0_LPRE_S27_BITPOS,
    prefix_insn,
    FLARE_G1G5G6_I5_MASK,
    FLARE_G1G5G6_I5_BITSIZE,
    FLARE_G1G5G6_I5_BITPOS,
    insn);
}
static inline flare_temp_t
flare_get_g3_s21 (flare_temp_t prefix_insn,
                    flare_temp_t insn)
{
  return flare_get_ext_imm
    (FLARE_G0_PRE_S12_MASK,
    FLARE_G0_PRE_S12_BITPOS,
    prefix_insn,
    FLARE_G3_S9_MASK,
    FLARE_G3_S9_BITSIZE,
    FLARE_G3_S9_BITPOS,
    insn);
}

static inline flare_temp_t
flare_get_g3_s32 (flare_temp_t prefix_insn,
                    flare_temp_t insn)
{
  return flare_get_ext_imm
    (FLARE_G0_LPRE_S27_MASK,
    FLARE_G0_LPRE_S27_BITPOS,
    prefix_insn,
    FLARE_G3_S9_MASK,
    FLARE_G3_S9_BITSIZE,
    FLARE_G3_S9_BITPOS,
    insn);
}

static inline flare_temp_t
flare_get_g5g6_s32_index (flare_temp_t index_insn,
                            flare_temp_t prefix_insn,
                            flare_temp_t insn)
{
  return (
    (index_insn << FLARE_INDEX_LPRE_EXT_BITPOS)
    | flare_get_g1g5g6_s32 (prefix_insn, insn)
  );
}

static inline flare_temp_t
flare_get_g7_icreload_s17 (flare_temp_t prefix_insn,
                        flare_temp_t insn)
{
  return flare_get_ext_imm 
    (FLARE_G0_PRE_S12_MASK,
    FLARE_G0_PRE_S12_BITPOS,
    prefix_insn,
    FLARE_G7_ICRELOAD_S5_MASK,
    FLARE_G7_ICRELOAD_S5_BITSIZE,
    FLARE_G7_ICRELOAD_S5_BITPOS,
    insn);
}
static inline flare_temp_t
flare_get_g7_icreload_s32 (flare_temp_t prefix_insn,
                        flare_temp_t insn)
{
  return flare_get_ext_imm
    (FLARE_G7_ICRELOAD_G0_LPRE_S27_MASK,
    FLARE_G7_ICRELOAD_G0_LPRE_S27_BITPOS,
    prefix_insn,
    FLARE_G7_ICRELOAD_S5_MASK,
    FLARE_G7_ICRELOAD_S5_BITSIZE,
    FLARE_G7_ICRELOAD_S5_BITPOS,
    insn);
}

static inline void
flare_put_ext_imm (flare_temp_t prefix_mask,
                    flare_temp_t prefix_bitpos,
                    flare_temp_t *prefix_insn,
                    flare_temp_t insn_mask,
                    flare_temp_t insn_bitsize,
                    flare_temp_t insn_bitpos,
                    flare_temp_t *insn,
                    flare_temp_t combined)
{
  //*insn &= ~insn_mask;
  //*insn |= (combined << insn_bitpos) & insn_mask;
  flare_set_insn_field_p (insn_mask, insn_bitpos, insn, combined);
  *prefix_insn = flare_set_insn_field (prefix_mask, prefix_bitpos,
    *prefix_insn,
    combined >> insn_bitsize);

  //*prefix_insn &= ~prefix_mask;
  //*prefix_insn |= (combined >> insn_bitsize) & prefix_mask;
}

//static inline void
//flare_put_ext_imm (const flare_enc_info_t *prefix_enc_info,
//                    flare_temp_t *prefix_insn,
//                    const flare_enc_info_t *insn_enc_info,
//                    flare_temp_t *insn,
//                    flare_temp_t combined)
//{
//
//  flare_put_ext_imm_worker
//    (prefix_enc_info->mask, /* prefix_mask, */
//    prefix_insn, /* prefix_insn, */
//    insn_enc_info->mask, /* insn_mask, */
//    insn_enc_info->bitsize, /* insn_bitsize, */
//    insn_enc_info->bitpos, /* insn_bitpos, */
//    insn, /* insn, */
//    combined /* combined */);
//}
static inline void
flare_put_g1g5g6_s17 (flare_temp_t *prefix_insn,
                        flare_temp_t *insn,
                        flare_temp_t combined)
{
  flare_put_ext_imm (FLARE_G0_PRE_S12_MASK,
                      FLARE_G0_PRE_S12_BITPOS,
                      prefix_insn,
                      FLARE_G1G5G6_I5_MASK,
                      FLARE_G1G5G6_I5_BITSIZE,
                      FLARE_G1G5G6_I5_BITPOS,
                      insn,
                      combined);
}
static inline void
flare_put_g1g5g6_s32 (flare_temp_t *prefix_insn,
                        flare_temp_t *insn,
                        flare_temp_t combined)
{
  flare_put_ext_imm (FLARE_G0_LPRE_S27_MASK,
                      FLARE_G0_LPRE_S27_BITPOS,
                      prefix_insn,
                      FLARE_G1G5G6_I5_MASK,
                      FLARE_G1G5G6_I5_BITSIZE,
                      FLARE_G1G5G6_I5_BITPOS,
                      insn,
                      combined);
}

static inline void
flare_put_g3_s21 (flare_temp_t *prefix_insn,
                    flare_temp_t *insn,
                    flare_temp_t combined)
{
  flare_put_ext_imm (FLARE_G0_PRE_S12_MASK,
                      FLARE_G0_PRE_S12_BITPOS,
                      prefix_insn,
                      FLARE_G3_S9_MASK,
                      FLARE_G3_S9_BITSIZE,
                      FLARE_G3_S9_BITPOS,
                      insn,
                      combined);
}
static inline void
flare_put_g3_s32 (flare_temp_t *prefix_insn,
                    flare_temp_t *insn,
                    flare_temp_t combined)
{
  flare_put_ext_imm (FLARE_G0_LPRE_S23_MASK,
                      FLARE_G0_LPRE_S23_BITPOS,
                      prefix_insn,
                      FLARE_G3_S9_MASK,
                      FLARE_G3_S9_BITSIZE,
                      FLARE_G3_S9_BITPOS,
                      insn,
                      combined);
}
static inline void
flare_put_g7_icreload_s17 (flare_temp_t *prefix_insn,
                        flare_temp_t *insn,
                        flare_temp_t combined)
{
  flare_put_ext_imm (FLARE_G0_PRE_S12_MASK,
                      FLARE_G0_PRE_S12_BITPOS,
                      prefix_insn,
                      FLARE_G7_ICRELOAD_S5_MASK,
                      FLARE_G7_ICRELOAD_S5_BITSIZE,
                      FLARE_G7_ICRELOAD_S5_BITPOS,
                      insn,
                      combined);
}
static inline void
flare_put_g7_icreload_s32 (flare_temp_t *prefix_insn,
                        flare_temp_t *insn,
                        flare_temp_t combined)
{
  flare_put_ext_imm (FLARE_G7_ICRELOAD_G0_LPRE_S27_MASK,
                      FLARE_G7_ICRELOAD_G0_LPRE_S27_BITPOS,
                      prefix_insn,
                      FLARE_G7_ICRELOAD_S5_MASK,
                      FLARE_G7_ICRELOAD_S5_BITSIZE,
                      FLARE_G7_ICRELOAD_S5_BITPOS,
                      insn,
                      combined);
}
/* -------- */
#endif    // _FLARE_H_

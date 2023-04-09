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

#define FLARE32_N_ONES(n) \
  ((((1ull << ((n) - 1ull)) - 1ull) << 1ull) | 1ull)

#define FLARE32_ARRAY_SIZE(x) (sizeof (x) / sizeof (x[0]))

/* Macros for getting and setting an instruction field.  */
#define GET_INSN_FIELD(mask, bitpos, i) \
  (((i) & (mask)) >> (bitpos))
#define SET_INSN_FIELD(mask, bitpos, i, v) \
  ( \
    (i) = ( \
      ((i) & ~(mask)) \
      | (((v) << (bitpos)) & (mask)) \
    ) \
  )
/* -------- */
#define FLARE32_GRP_BITSIZE (3ull)
#define FLARE32_GRP_RSMASK \
  (FLARE32_N_ONES(FLARE32_GRP_BITSIZE))

#define FLARE32_GRP_16_BITPOS (16ull - FLARE32_GRP_BITSIZE)
#define FLARE32_GRP_16_MASK \
  (FLARE32_GRP_RSMASK << FLARE32_GRP_16_BITPOS)

#define FLARE32_GRP_32_BITPOS (32ull - FLARE32_GRP_BITSIZE)
#define FLARE32_GRP_32_MASK \
  (FLARE32_GRP_RSMASK << FLARE32_GRP_32_BITPOS)

#define FLARE32_PRE_LPRE_GRP_VALUE (0x0ull)
/* -------- */
#define FLARE32_FULL_GRP_VALUE(subgrp_bitsize, subgrp_rsmask, grp, subgrp) \
  ( \
    (((grp) & FLARE32_GRP_RSMASK) << (subgrp_bitsize)) \
    || ((subgrp) & (subgrp_rsmask)) \
  )
/* -------- */
/* `pre` subgrp */
#define FLARE32_PRE_SUBGRP_BITSIZE (1ull)
#define FLARE32_PRE_SUBGRP_BITPOS \
  (FLARE32_GRP_16_BITPOS - FLARE32_PRE_SUBGRP_BITSIZE)
#define FLARE32_PRE_SUBGRP_RSMASK \
  (FLARE32_N_ONES(FLARE32_PRE_SUBGRP_BITSIZE))
#define FLARE32_PRE_SUBGRP_MASK \
  (FLARE32_GRP_RSMASK << FLARE32_PRE_SUBGRP_BITPOS)

/* `pre` full group */
#define FLARE32_PRE_FULL_GRP_BITSIZE \
  (FLARE32_GRP_BITSIZE + FLARE32_PRE_SUBGRP_BITSIZE)
#define FLARE32_PRE_FULL_GRP_BITPOS (FLARE32_PRE_SUBGRP_BITPOS)
#define FLARE32_PRE_FULL_GRP_RSMASK \
  (FLARE32_N_ONES(FLARE32_PRE_FULL_GRP_BITSIZE))
#define FLARE32_PRE_FULL_GRP_MASK \
  (FLARE32_PRE_FULL_GRP_RSMASK << FLARE32_PRE_FULL_GRP_BITPOS)
//#define FLARE32_PRE_FULL_GRP_VALUE (0x0ull)

/* `pre`-specific group values */
#define FLARE32_PRE_SUBGRP_VALUE (0x0ull)
#define FLARE32_PRE_FULL_GRP_VALUE \
  (FLARE32_FULL_GRP_VALUE(FLARE32_PRE_SUBGRP_BITSIZE, \
    FLARE32_PRE_SUBGRP_RSMASK, FLARE32_PRE_LPRE_GRP_VALUE, \
    FLARE32_PRE_SUBGRP_VALUE))

/* `pre` simm12 */
#define FLARE32_PRE_S12_BITSIZE (12ull)
#define FLARE32_PRE_S12_BITPOS (0ull)
#define FLARE32_PRE_S12_RSMASK \
  (FLARE32_N_ONES(FLARE32_PRE_S12_BITSIZE))
#define FLARE32_PRE_S12_MASK \
  (FLARE32_PRE_S12_RSMASK << FLARE32_PRE_S12_BITPOS)
/* -------- */
/* `lpre` subgrp */
#define FLARE32_LPRE_SUBGRP_BITSIZE (2ull)
#define FLARE32_LPRE_SUBGRP_BITPOS \
  (FLARE32_GRP_32_BITPOS - FLARE32_LPRE_SUBGRP_BITSIZE)
#define FLARE32_LPRE_SUBGRP_RSMASK \
  (FLARE32_N_ONES(FLARE32_LPRE_SUBGRP_BITSIZE))
#define FLARE32_LPRE_SUBGRP_MASK \
  (FLARE32_GRP_RSMASK << FLARE32_LPRE_SUBGRP_BITPOS)

/* `lpre` full group */
#define FLARE32_LPRE_FULL_GRP_BITSIZE \
  (FLARE32_GRP_BITSIZE + FLARE32_LPRE_SUBGRP_BITSIZE)
#define FLARE32_LPRE_FULL_GRP_BITPOS (FLARE32_LPRE_SUBGRP_BITPOS)
#define FLARE32_LPRE_FULL_GRP_RSMASK \
  (FLARE32_N_ONES(FLARE32_LPRE_FULL_GRP_BITSIZE))
#define FLARE32_LPRE_FULL_GRP_MASK \
  (FLARE32_LPRE_FULL_GRP_RSMASK << FLARE32_LPRE_FULL_GRP_BITPOS)


/* `lpre` (top 16-bit half) subgrp */
#define FLARE32_LPRE_SUBGRP_16_BITPOS \
  (FLARE32_GRP_16_BITPOS - FLARE32_LPRE_SUBGRP_BITSIZE)
#define FLARE32_LPRE_SUBGRP_16_MASK \
  (FLARE32_GRP_RSMASK << FLARE32_LPRE_SUBGRP_16_BITPOS)

/* `lpre` (top 16-bit half) full group */
#define FLARE32_LPRE_FULL_GRP_16_BITPOS (FLARE32_LPRE_SUBGRP_16_BITPOS)
#define FLARE32_LPRE_FULL_GRP_16_MASK \
  (FLARE32_LPRE_FULL_GRP_RSMASK << FLARE32_LPRE_FULL_GRP_16_BITPOS)

/* `lpre`-specifc group values */
#define FLARE32_LPRE_SUBGRP_VALUE (0x2ull)
#define FLARE32_LPRE_FULL_GRP_VALUE \
  (FLARE32_FULL_GRP_VALUE(FLARE32_LPRE_SUBGRP_BITSIZE, \
    FLARE32_LPRE_SUBGRP_RSMASK, FLARE32_PRE_LPRE_GRP_VALUE, \
    FLARE32_LPRE_SUBGRP_VALUE))

//#define FLARE32_LPRE_GRP_16_MASK (FLARE32_LPRE_GRP_RSMASK << FLARE32_LPRE_GRP_BITPOS_16)
//#define FLARE32_LPRE_GRP_MASK (FLARE32_LPRE_GRP_RSMASK << FLARE32_LPRE_GRP_BITPOS)

/* `lpre` simm27, i.e. when used with an instruction from
  group 1, 5, or 6 */
#define FLARE32_G1G5G6_LPRE_S27_BITSIZE (27ull)
#define FLARE32_G1G5G6_LPRE_S27_BITPOS (0ull)
#define FLARE32_G1G5G6_LPRE_S27_RSMASK \
  (FLARE32_N_ONES(FLARE32_G1G5G6_LPRE_S27_BITSIZE))
#define FLARE32_G1G5G6_LPRE_S27_MASK \
  (FLARE32_G1G5G6_LPRE_S27_RSMASK << FLARE32_G1G5G6_LPRE_S27_BITPOS)

/* `lpre` simm23, i.e. when used with an instruction from group 3 */
#define FLARE32_G3_LPRE_S23_BITSIZE (23ull)
#define FLARE32_G3_LPRE_S23_BITPOS (0ull)
#define FLARE32_G3_LPRE_S23_RSMASK \
  (FLARE32_N_ONES(FLARE32_G3_LPRE_S23_BITSIZE))
#define FLARE32_G3_LPRE_S23_MASK \
  (FLARE32_G3_LPRE_S23_RSMASK << FLARE32_G3_LPRE_S23_BITPOS)
/* -------- */
/* simm5, i.e. when part of an instruction from groups 1, 5, or 6 */
#define FLARE32_G1G5G6_S5_BITSIZE (5ull)
#define FLARE32_G1G5G6_S5_BITPOS (8ull)
#define FLARE32_G1G5G6_S5_RSMASK \
  (FLARE32_N_ONES(FLARE32_G1G5G6_S5_BITSIZE))
#define FLARE32_G1G5G6_S5_MASK \
  (FLARE32_G1G5G6_S5_RSMASK << FLARE32_G1G5G6_S5_BITPOS)
/* -------- */
/* the `grp` field of group 1 instructions */
#define FLARE32_G1_GRP_VALUE (0x1ull)

/* the `opcode` field of group 1 instructions */
#define FLARE32_G1_OP_BITSIZE (4ull)
#define FLARE32_G1_OP_BITPOS (4ull)
#define FLARE32_G1_OP_RSMASK \
  (FLARE32_N_ONES(FLARE32_G1_OP_BITSIZE))
#define FLARE32_G1_OP_MASK \
  (FLARE32_G1_OP_RSMASK << FLARE32_G1_OP_BITPOS)

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
#define FLARE32_G1_OP_ENUM_ZEB_RA (0xcull)
#define FLARE32_G1_OP_ENUM_ZEH_RA (0xdull)
#define FLARE32_G1_OP_ENUM_SEB_RA (0xeull)
#define FLARE32_G1_OP_ENUM_SEH_RA (0xfull)
/* -------- */
/* the `grp` field of group 2 instructions */
#define FLARE32_G2_GRP_VALUE (0x2ull)

/* the `flag` field of group 1 instructions */
#define FLARE32_G2_F_BITSIZE (1ull)
#define FLARE32_G2_F_BITPOS (12ull)
#define FLARE32_G2_F_RSMASK \
  (FLARE32_N_ONES(FLARE32_G2_F_BITSIZE))
#define FLARE32_G2_F_MASK \
  (FLARE32_G2_F_RSMASK << FLARE32_G2_F_BITPOS)

/* the `opcode` field of group 2 instructions */
#define FLARE32_G2_OP_BITSIZE (4ull)
#define FLARE32_G2_OP_BITPOS (8ull)
#define FLARE32_G2_OP_RSMASK \
  (FLARE32_N_ONES(FLARE32_G2_OP_BITSIZE))
#define FLARE32_G2_OP_MASK \
  (FLARE32_G2_OP_RSMASK << FLARE32_G2_OP_BITPOS)

/* the specific opcode values usable with group 2 instructions */
#define FLARE32_G2_OP_ENUM_ADD_RA_RC (0x0ull)
#define FLARE32_G2_OP_ENUM_SUB_RA_RC (0x1ull)
#define FLARE32_G2_OP_ENUM_ADD_RA_SP_RC (0x2ull)
#define FLARE32_G2_OP_ENUM_ADD_RA_FP_RC (0x3ull)
#define FLARE32_G2_OP_ENUM_CMP_RA_RC (0x4ull)
#define FLARE32_G2_OP_ENUM_CPY_RA_RC (0x5ull)
#define FLARE32_G2_OP_ENUM_LSL_RA_RC (0x6ull)
#define FLARE32_G2_OP_ENUM_LSR_RA_RC (0x7ull)
#define FLARE32_G2_OP_ENUM_ASR_RA_RC (0x8ull)
#define FLARE32_G2_OP_ENUM_AND_RA_RC (0x9ull)
#define FLARE32_G2_OP_ENUM_ORR_RA_RC (0xaull)
#define FLARE32_G2_OP_ENUM_XOR_RA_RC (0xbull)
#define FLARE32_G2_OP_ENUM_ADC_RA_RC (0xcull)
#define FLARE32_G2_OP_ENUM_SBC_RA_RC (0xdull)
#define FLARE32_G2_OP_ENUM_RESERVED_14 (0xeull)
#define FLARE32_G2_OP_ENUM_RESERVED_15 (0xfull)
/* -------- */
/* the `grp` field of group 3 instructions */
#define FLARE32_G3_GRP_VALUE (0x3ull)

/* the PC-relative `simm9` field of group 3 instructions */
#define FLARE32_G3_S9_BITSIZE (9ull)
#define FLARE32_G3_S9_BITPOS (4ull)
#define FLARE32_G3_S9_RSMASK \
  (FLARE32_N_ONES(FLARE32_G3_S9_BITSIZE))
#define FLARE32_G3_S9_MASK \
  (FLARE32_G3_S9_RSMASK << FLARE32_G3_S9_BITPOS)

/* the `opcode` field of group 1 instructions */
#define FLARE32_G3_OP_BITSIZE (4ull)
#define FLARE32_G3_OP_BITPOS (0ull)
#define FLARE32_G3_OP_RSMASK \
  (FLARE32_N_ONES(FLARE32_G3_OP_BITSIZE))
#define FLARE32_G3_OP_MASK \
  (FLARE32_G3_OP_RSMASK << FLARE32_G3_OP_BITPOS)

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
#define FLARE32_G4_OP_BITPOS (12ull)
#define FLARE32_G4_OP_RSMASK \
  (FLARE32_N_ONES(FLARE32_G4_OP_BITSIZE))
#define FLARE32_G4_OP_MASK \
  (FLARE32_G4_OP_RSMASK << FLARE32_G4_OP_BITPOS)

/* the specific opcode values usable with group 4 instructions */
#define FLARE32_G4_OP_ENUM_JL_RA (0x0ull)
#define FLARE32_G4_OP_ENUM_JMP_RA (0x1ull)
#define FLARE32_G4_OP_ENUM_JMP_RA_RC (0x2ull)
#define FLARE32_G4_OP_ENUM_JMP_IRA (0x3ull)
#define FLARE32_G4_OP_ENUM_RETI (0x4ull)
#define FLARE32_G4_OP_ENUM_CPY_RA_SC (0x5ull)
#define FLARE32_G4_OP_ENUM_CPY_SA_RC (0x6ull)
#define FLARE32_G4_OP_ENUM_EI (0x7ull)
#define FLARE32_G4_OP_ENUM_DI (0x8ull)
#define FLARE32_G4_OP_ENUM_PUSH_RA_RC (0x9ull)
#define FLARE32_G4_OP_ENUM_POP_RA_RC (0xaull)
#define FLARE32_G4_OP_ENUM_PUSH_SA_RC (0xbull)
#define FLARE32_G4_OP_ENUM_POP_SA_RC (0xcull)
#define FLARE32_G4_OP_ENUM_INDEX_RA (0xdull)
#define FLARE32_G4_OP_ENUM_MUL_RA_RC (0xeull)
#define FLARE32_G4_OP_ENUM_UDIV_RA_RC (0xfull)

#define FLARE32_G4_OP_ENUM_SDIV_RA_RC (0x10ull)
#define FLARE32_G4_OP_ENUM_UMOD_RA_RC (0x11ull)
#define FLARE32_G4_OP_ENUM_SMOD_RA_RC (0x12ull)
#define FLARE32_G4_OP_ENUM_LUMUL_RA_RC (0x13ull)
#define FLARE32_G4_OP_ENUM_LSMUL_RA_RC (0x14ull)
#define FLARE32_G4_OP_ENUM_LUDIV_RA_RC (0x15ull)
#define FLARE32_G4_OP_ENUM_LSDIV_RA_RC (0x16ull)
#define FLARE32_G4_OP_ENUM_LUMOD_RA_RC (0x17ull)
#define FLARE32_G4_OP_ENUM_LSMOD_RA_RC (0x18ull)
#define FLARE32_G4_OP_ENUM_LDUB_RA_RC (0x19ull)
#define FLARE32_G4_OP_ENUM_LDSB_RA_RC (0x1aull)
#define FLARE32_G4_OP_ENUM_LDUH_RA_RC (0x1bull)
#define FLARE32_G4_OP_ENUM_LDSH_RA_RC (0x1cull)
#define FLARE32_G4_OP_ENUM_STB_RA_RC (0x1dull)
#define FLARE32_G4_OP_ENUM_STH_RA_RC (0x1eull)
#define FLARE32_G4_OP_ENUM_RESERVED_31 (0x1full)
/* -------- */
/* the `grp` field of group 5 instructions */
#define FLARE32_G5_GRP_VALUE (0x5ull)
/* -------- */
/* the `opcode` field of group 6 instructions */
#define FLARE32_G6_GRP_VALUE (0x6ull)
/* -------- */
/* encoding of rA */
#define FLARE32_RA_IND_BITSIZE (4ull)
#define FLARE32_RA_IND_BITPOS (0ull)
#define FLARE32_RA_IND_RSMASK \
  (FLARE32_N_ONES(FLARE32_RA_IND_BITSIZE))
#define FLARE32_RA_IND_MASK \
  (FLARE32_RA_IND_RSMASK << FLARE32_RA_IND_BITPOS)

/* encoding of rC */
#define FLARE32_RC_IND_BITSIZE (4ull)
#define FLARE32_RC_IND_BITPOS (4ull)
#define FLARE32_RC_IND_RSMASK \
  (FLARE32_N_ONES(FLARE32_RC_IND_BITSIZE))
#define FLARE32_RC_IND_MASK \
  (FLARE32_RC_IND_RSMASK << FLARE32_RC_IND_BITPOS)
/* -------- */
/* the left shift amount for `pre`/`lpre` when storing a "whole" 
  instruction in a `flare32_temp_t` */
#define FLARE32_PRE_LPRE_EXT_BITPOS (16ull)

#define FLARE32_PRE_EXT_LSMASK \
  (FLARE32_PRE_S12_MASK << FLARE32_PRE_LPRE_EXT_BITPOS)
#define FLARE32_G1G5G6_LPRE_EXT_LSMASK \
  (FLARE32_G1G5G6_LPRE_S27_MASK << FLARE32_PRE_LPRE_EXT_BITPOS)
#define FLARE32_G3_LPRE_EXT_LSMASK \
  (FLARE32_G3_LPRE_S23_MASK << FLARE32_PRE_LPRE_EXT_BITPOS)
/* -------- */
/* general-purpose registers */
#define FLARE32_NUM_GPRS (16ull)
extern const char *gpr_names[FLARE32_NUM_GPRS];

/* special-purpose registers */
#define FLARE32_NUM_SPRS (16ull)
#define FLARE32_REAL_NUM_SPRS (7ull)
extern const char *spr_names[FLARE32_NUM_SPRS];
/* -------- */
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
  FLARE32_OA_RA,
  FLARE32_OA_RA_RC,
  FLARE32_OA_RA_SP_RC,
  FLARE32_OA_RA_FP_RC,
  FLARE32_OA_PCREL_S9,
  FLARE32_OA_IRA,
  FLARE32_OA_RA_SC,
  FLARE32_OA_SA_RC,
  FLARE32_OA_RA_RC_LDST,
  FLARE32_OA_RA_RC_RB_LDST,
  FLARE32_OA_RA_RC_S5_LDST,
  FLARE32_OA_RA_RC_RB_S5_LDST,
} flare32_oparg_t;


typedef struct flare32_grp_info_t
{
  unsigned grp_bitsize;
  unsigned grp_bitpos;
  unsigned grp_rsmask;
  unsigned grp_mask;
  unsigned grp; /* The primary group, i.e. the top three bits */

  bool has_subgrp;
  unsigned subgrp_bitsize;
  unsigned subgrp_bitpos;
  unsigned subgrp_rsmask;
  unsigned subgrp_mask;
  unsigned subgrp;
} flare32_grp_info_t;
extern const flare32_grp_info_t flare32_grp_info_pre;
extern const flare32_grp_info_t flare32_grp_info_lpre;
extern const flare32_grp_info_t flare32_grp_info_g1;
extern const flare32_grp_info_t flare32_grp_info_g2;
extern const flare32_grp_info_t flare32_grp_info_g3;
extern const flare32_grp_info_t flare32_grp_info_g4;
extern const flare32_grp_info_t flare32_grp_info_g5;
extern const flare32_grp_info_t flare32_grp_info_g6;

typedef struct flare32_opc_info_t
{
  const flare32_grp_info_t *grp_info;
  signed opcode;
  flare32_oparg_t oparg;
  const char *name;
} flare32_opc_info_t;
#define FLARE32_OPC_INFO_NULL_OP (-1)
#define FLARE32_OPC_INFO_PSEUDO_OP (-2)

#define FLARE32_PRE_LPRE_OPC_INFO_LIM (2ull)
extern const flare32_opc_info_t
  flare32_opc_info_pre_lpre[FLARE32_PRE_LPRE_OPC_INFO_LIM];

#define FLARE32_G1_OPC_INFO_LIM (16ull)
extern const flare32_opc_info_t
  flare32_opc_info_g1[FLARE32_G1_OPC_INFO_LIM];

#define FLARE32_G2_OPC_INFO_LIM (16ull)
extern const flare32_opc_info_t
  flare32_opc_info_g2[FLARE32_G2_OPC_INFO_LIM];

#define FLARE32_G3_OPC_INFO_LIM (16ull)
extern const flare32_opc_info_t
  flare32_opc_info_g3[FLARE32_G3_OPC_INFO_LIM];

#define FLARE32_G4_OPC_INFO_LIM (32ull + 6ull)
extern const flare32_opc_info_t
  flare32_opc_info_g4[FLARE32_G4_OPC_INFO_LIM];

#define FLARE32_G5_OPC_INFO_LIM (4ull)
extern const flare32_opc_info_t
  flare32_opc_info_g5[FLARE32_G5_OPC_INFO_LIM];

#define FLARE32_G6_OPC_INFO_LIM (4ull)
extern const flare32_opc_info_t
  flare32_opc_info_g6[FLARE32_G6_OPC_INFO_LIM];
/* -------- */
typedef unsigned long long flare32_temp_t;
/* -------- */
static inline flare32_temp_t
flare32_enc_temp_insn_pre (flare32_temp_t simm12)
{
  flare32_temp_t ret = 0;
  SET_INSN_FIELD(FLARE32_PRE_FULL_GRP_MASK, FLARE32_PRE_FULL_GRP_BITPOS,
    ret, FLARE32_PRE_FULL_GRP_VALUE);
  SET_INSN_FIELD(FLARE32_PRE_S12_MASK, FLARE32_PRE_S12_BITPOS, ret,
    simm12);
  return ret;
}
static inline flare32_temp_t
flare32_enc_temp_insn_g1g5g6_lpre (flare32_temp_t simm27)
{
  flare32_temp_t ret = 0;
  SET_INSN_FIELD(FLARE32_LPRE_FULL_GRP_MASK, FLARE32_LPRE_FULL_GRP_BITPOS,
    ret, FLARE32_LPRE_FULL_GRP_VALUE);
  SET_INSN_FIELD(FLARE32_G1G5G6_LPRE_S27_MASK,
    FLARE32_G1G5G6_LPRE_S27_BITPOS, ret, simm27);
  return ret;
}
static inline flare32_temp_t
flare32_enc_temp_insn_g3_lpre (flare32_temp_t simm23)
{
  flare32_temp_t ret = 0;
  SET_INSN_FIELD(FLARE32_LPRE_FULL_GRP_MASK, FLARE32_LPRE_FULL_GRP_BITPOS,
    ret, FLARE32_LPRE_FULL_GRP_VALUE);
  SET_INSN_FIELD(FLARE32_G3_LPRE_S23_MASK,
    FLARE32_G3_LPRE_S23_BITPOS, ret, simm23);
  return ret;
}
/* -------- */
static inline flare32_temp_t
flare32_enc_temp_insn_g1 (flare32_temp_t simm5,
                          flare32_temp_t op,
                          flare32_temp_t ra_ind)
{
  flare32_temp_t ret = 0;
  SET_INSN_FIELD(FLARE32_GRP_16_MASK, FLARE32_GRP_16_BITPOS, ret,
    FLARE32_G1_GRP_VALUE);
  SET_INSN_FIELD(FLARE32_G1G5G6_S5_MASK, FLARE32_G1G5G6_S5_BITPOS, ret,
    simm5);
  SET_INSN_FIELD(FLARE32_G1_OP_MASK, FLARE32_G1_OP_BITPOS, ret, op);
  SET_INSN_FIELD(FLARE32_RA_IND_MASK, FLARE32_RA_IND_BITPOS, ret, ra_ind);
  return ret;
}
/* -------- */
static inline flare32_temp_t
flare32_enc_temp_insn_g2 (flare32_temp_t f,
                          flare32_temp_t op,
                          flare32_temp_t rc_ind,
                          flare32_temp_t ra_ind)
{
  flare32_temp_t ret = 0;
  SET_INSN_FIELD(FLARE32_GRP_16_MASK, FLARE32_GRP_16_BITPOS, ret,
    FLARE32_G2_GRP_VALUE);
  SET_INSN_FIELD(FLARE32_G2_F_MASK, FLARE32_G2_F_BITPOS, ret, f);
  SET_INSN_FIELD(FLARE32_G2_OP_MASK, FLARE32_G2_OP_BITPOS, ret, op);
  SET_INSN_FIELD(FLARE32_RC_IND_MASK, FLARE32_RC_IND_BITPOS, ret, rc_ind);
  SET_INSN_FIELD(FLARE32_RA_IND_MASK, FLARE32_RA_IND_BITPOS, ret, ra_ind);
  return ret;
}
/* -------- */
static inline flare32_temp_t
flare32_enc_temp_insn_g3 (flare32_temp_t simm9,
                          flare32_temp_t op)
{
  flare32_temp_t ret = 0;
  SET_INSN_FIELD(FLARE32_GRP_16_MASK, FLARE32_GRP_16_BITPOS, ret,
    FLARE32_G3_GRP_VALUE);
  SET_INSN_FIELD(FLARE32_G3_S9_MASK, FLARE32_G3_S9_BITPOS, ret, simm9);
  SET_INSN_FIELD(FLARE32_G3_OP_MASK, FLARE32_G3_OP_BITPOS, ret, op);
  return ret;
}
/* -------- */
static inline flare32_temp_t
flare32_enc_temp_insn_g4 (flare32_temp_t op,
                          flare32_temp_t rc_ind,
                          flare32_temp_t ra_ind)
{
  flare32_temp_t ret = 0;
  SET_INSN_FIELD(FLARE32_GRP_16_MASK, FLARE32_GRP_16_BITPOS, ret,
    FLARE32_G4_GRP_VALUE);
  SET_INSN_FIELD(FLARE32_G4_OP_MASK, FLARE32_G4_OP_BITPOS, ret, op);
  SET_INSN_FIELD(FLARE32_RC_IND_MASK, FLARE32_RC_IND_BITPOS, ret, rc_ind);
  SET_INSN_FIELD(FLARE32_RA_IND_MASK, FLARE32_RA_IND_BITPOS, ret, ra_ind);
  return ret;
}
/* -------- */
static inline flare32_temp_t
flare32_enc_temp_insn_g5 (flare32_temp_t simm5,
                          flare32_temp_t rc_ind,
                          flare32_temp_t ra_ind)
{
  flare32_temp_t ret = 0;
  SET_INSN_FIELD(FLARE32_GRP_16_MASK, FLARE32_GRP_16_BITPOS, ret,
    FLARE32_G5_GRP_VALUE);
  SET_INSN_FIELD(FLARE32_G1G5G6_S5_MASK, FLARE32_G1G5G6_S5_BITPOS, ret,
    simm5);
  SET_INSN_FIELD(FLARE32_RC_IND_MASK, FLARE32_RC_IND_BITPOS, ret, rc_ind);
  SET_INSN_FIELD(FLARE32_RA_IND_MASK, FLARE32_RA_IND_BITPOS, ret, ra_ind);
  return ret;
}
/* -------- */
static inline flare32_temp_t
flare32_enc_temp_insn_g6 (flare32_temp_t simm5,
                          flare32_temp_t rc_ind,
                          flare32_temp_t ra_ind)
{
  flare32_temp_t ret = 0;
  SET_INSN_FIELD(FLARE32_GRP_16_MASK, FLARE32_GRP_16_BITPOS, ret,
    FLARE32_G6_GRP_VALUE);
  SET_INSN_FIELD(FLARE32_G1G5G6_S5_MASK, FLARE32_G1G5G6_S5_BITPOS, ret,
    simm5);
  SET_INSN_FIELD(FLARE32_RC_IND_MASK, FLARE32_RC_IND_BITPOS, ret, rc_ind);
  SET_INSN_FIELD(FLARE32_RA_IND_MASK, FLARE32_RA_IND_BITPOS, ret, ra_ind);
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
    value |= ((flare32_temp_t) - 1ll) << bits;
  }

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
  return (flare32_temp_t)(
    (((prefix_insn & prefix_mask) >> prefix_bitpos) << insn_bitsize)
    | ((insn & insn_mask) >> insn_bitpos)
  );
}

static inline flare32_temp_t
flare32_get_g1g5g6_s17 (flare32_temp_t prefix_insn,
                        flare32_temp_t insn)
{
  return flare32_get_ext_imm (FLARE32_PRE_S12_MASK,
                              FLARE32_PRE_S12_BITPOS,
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
  return flare32_get_ext_imm (FLARE32_G1G5G6_LPRE_S27_MASK,
                              FLARE32_G1G5G6_LPRE_S27_BITPOS,
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
  return flare32_get_ext_imm (FLARE32_PRE_S12_MASK,
                              FLARE32_PRE_S12_BITPOS,
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
  return flare32_get_ext_imm (FLARE32_G3_LPRE_S23_MASK,
                              FLARE32_G3_LPRE_S23_BITPOS,
                              prefix_insn,
                              FLARE32_G3_S9_MASK,
                              FLARE32_G3_S9_BITSIZE,
                              FLARE32_G3_S9_BITPOS,
                              insn);
}

static inline void
flare32_put_ext_imm (flare32_temp_t prefix_mask,
                    flare32_temp_t *prefix_insn,
                    flare32_temp_t insn_mask,
                    flare32_temp_t insn_bitsize,
                    flare32_temp_t insn_bitpos,
                    flare32_temp_t *insn,
                    flare32_temp_t combined)
{
  *insn &= ~insn_mask;
  *insn |= (combined << insn_bitpos) & insn_mask;

  *prefix_insn &= ~prefix_mask;
  *prefix_insn |= (combined >> insn_bitsize) & prefix_mask;
}

static inline void
flare32_put_g1g5g6_s17 (flare32_temp_t *prefix_insn,
                        flare32_temp_t *insn,
                        flare32_temp_t combined)
{
  flare32_put_ext_imm (FLARE32_PRE_S12_MASK,
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
  flare32_put_ext_imm (FLARE32_G1G5G6_LPRE_S27_MASK,
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
  flare32_put_ext_imm (FLARE32_PRE_S12_MASK, /* prefix_mask */
                      prefix_insn,  /* prefix_insn */
                      FLARE32_G3_S9_MASK, /* insn_mask */
                      FLARE32_G3_S9_BITSIZE, /* insn_bitsize */
                      FLARE32_G3_S9_BITPOS, /* insn_bitpos */
                      insn, /* insn */
                      combined); /* combined */
}
static inline void
flare32_put_g3_s32 (flare32_temp_t *prefix_insn,
                    flare32_temp_t *insn,
                    flare32_temp_t combined)
{
  flare32_put_ext_imm (FLARE32_G3_LPRE_S23_MASK,
                      prefix_insn,
                      FLARE32_G3_S9_MASK,
                      FLARE32_G3_S9_BITSIZE,
                      FLARE32_G3_S9_BITPOS,
                      insn,
                      combined);
}
/* -------- */
#endif    // _FLARE32_H_

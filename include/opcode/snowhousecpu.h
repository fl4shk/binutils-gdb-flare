/* BFD support for the snowhousecpu microprocessor.
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

#ifndef _SNOWHOUSECPU_H_
#define _SNOWHOUSECPU_H_

#include "bfd.h"
#include <stdlib.h>
#include <stdint.h>

#define SNOWHOUSECPU_N_ONES(n) \
  ((((1ull << ((n) - 1ull)) - 1ull) << 1ull) | 1ull)

#define SNOWHOUSECPU_ARRAY_SIZE(x) (sizeof (x) / sizeof (x[0]))

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
typedef uint64_t snowhousecpu_temp_t;
//typedef struct snowhousecpu_enc_info_t
//{
//  snowhousecpu_temp_t bitsize;
//  snowhousecpu_temp_t bitpos;
//  snowhousecpu_temp_t rsmask;
//  snowhousecpu_temp_t mask;
//} snowhousecpu_enc_info_t;

static inline snowhousecpu_temp_t
snowhousecpu_get_insn_field (snowhousecpu_temp_t mask,
                        snowhousecpu_temp_t bitpos,
                        snowhousecpu_temp_t insn)
{
  return (snowhousecpu_temp_t) GET_INSN_FIELD_WORKER (mask, bitpos, insn);
}
static inline snowhousecpu_temp_t
snowhousecpu_set_insn_field (snowhousecpu_temp_t mask,
                        snowhousecpu_temp_t bitpos,
                        snowhousecpu_temp_t insn,
                        snowhousecpu_temp_t value)
{
  return (snowhousecpu_temp_t) SET_INSN_FIELD_WORKER
    (mask, bitpos, insn, value);
}
static inline snowhousecpu_temp_t *
snowhousecpu_set_insn_field_p (snowhousecpu_temp_t mask,
                          snowhousecpu_temp_t bitpos,
                          snowhousecpu_temp_t *insn,
                          snowhousecpu_temp_t value)
{
  *insn = snowhousecpu_set_insn_field (mask, bitpos, *insn, value);
  return insn;
}
/* -------- */
//static inline snowhousecpu_temp_t
//snowhousecpu_get_insn_field_ei (const snowhousecpu_enc_info_t *enc_info,
//                          snowhousecpu_temp_t insn)
//{
//  return (snowhousecpu_temp_t) GET_INSN_FIELD_WORKER
//    (enc_info->mask, enc_info->bitpos, insn);
//}
//static inline snowhousecpu_temp_t
//snowhousecpu_set_insn_field_ei (const snowhousecpu_enc_info_t *enc_info,
//                          snowhousecpu_temp_t insn,
//                          snowhousecpu_temp_t value)
//{
//  return (snowhousecpu_temp_t) SET_INSN_FIELD_WORKER
//    (enc_info->mask, enc_info->bitpos, insn, value);
//}
//static inline snowhousecpu_temp_t *
//snowhousecpu_set_insn_field_ei_p (const snowhousecpu_enc_info_t *enc_info,
//                            snowhousecpu_temp_t *insn,
//                            snowhousecpu_temp_t value)
//{
//  *insn = snowhousecpu_set_insn_field_ei (enc_info, *insn, value);
//  return insn;
//}


/* -------- */
#define SNOWHOUSECPU_ENC_RSMASK(name) \
  (SNOWHOUSECPU_N_ONES (SNOWHOUSECPU_##name##_BITSIZE))
#define SNOWHOUSECPU_ENC_MASK(name) \
  ((SNOWHOUSECPU_##name##_RSMASK) << (SNOWHOUSECPU_##name##_BITPOS))

//#define SNOWHOUSECPU_OP_BITSIZE (4ull)
//#define SNOWHOUSECPU_OP_RSMASK
//  (SNOWHOUSECPU_ENC_RSMASK (GRP))

#define SNOWHOUSECPU_OP_BITSIZE (4ull)
#define SNOWHOUSECPU_OP_BITPOS (28ull)
#define SNOWHOUSECPU_OP_RSMASK \
  (SNOWHOUSECPU_ENC_RSMASK (OP))
#define SNOWHOUSECPU_OP_MASK \
  (SNOWHOUSECPU_ENC_MASK (OP))


//#define SNOWHOUSECPU_OP_16_BITPOS 
//  (16ull - SNOWHOUSECPU_OP_BITSIZE)
//#define SNOWHOUSECPU_OP_16_MASK 
//  ((SNOWHOUSECPU_OP_RSMASK) << (SNOWHOUSECPU_OP_16_BITPOS))
//  //(SNOWHOUSECPU_ENC_MASK (GRP_16))
// {'op': (31, 28), 'raIdx': (27, 24), 'rbIdx': (23, 20), 'rcIdx': (19, 16), 'imm16': (15, 0)}

#define SNOWHOUSECPU_OP_64_BITPOS \
  (64ull - SNOWHOUSECPU_OP_BITSIZE)
#define SNOWHOUSECPU_OP_64_MASK \
  ((SNOWHOUSECPU_OP_RSMASK) << (SNOWHOUSECPU_OP_64_BITPOS))
  //(SNOWHOUSECPU_ENC_MASK (OP_64))

//#define SNOWHOUSECPU_FULLGRP_VALUE(subgrp_inner_name, grp_value, subgrp_value) 
//  ( 
//    (((grp_value) & SNOWHOUSECPU_OP_RSMASK) 
//      << (SNOWHOUSECPU_##subgrp_inner_name##_BITSIZE)) 
//    | ((subgrp_value) & (SNOWHOUSECPU_##subgrp_inner_name##_RSMASK)) 
//  )
/* -------- */
/* -------- */

/* encoding of rA */
#define SNOWHOUSECPU_RA_IDX_BITSIZE (4ull)
#define SNOWHOUSECPU_RA_IDX_BITPOS (24ull)
#define SNOWHOUSECPU_RA_IDX_RSMASK \
  (SNOWHOUSECPU_ENC_RSMASK (RA_IDX))
#define SNOWHOUSECPU_RA_IDX_MASK \
  (SNOWHOUSECPU_ENC_MASK (RA_IDX))

/* encoding of rB */
#define SNOWHOUSECPU_RB_IDX_BITSIZE (4ull)
#define SNOWHOUSECPU_RB_IDX_BITPOS (20ull)
#define SNOWHOUSECPU_RB_IDX_RSMASK \
  (SNOWHOUSECPU_ENC_RSMASK (RB_IDX))
#define SNOWHOUSECPU_RB_IDX_MASK \
  (SNOWHOUSECPU_ENC_MASK (RB_IDX))

/* encoding of rC */
#define SNOWHOUSECPU_RC_IDX_BITSIZE (4ull)
#define SNOWHOUSECPU_RC_IDX_BITPOS (16ull)
#define SNOWHOUSECPU_RC_IDX_RSMASK \
  (SNOWHOUSECPU_ENC_RSMASK (RC_IDX))
#define SNOWHOUSECPU_RC_IDX_MASK \
  (SNOWHOUSECPU_ENC_MASK (RC_IDX))

/* encoding of imm16 */
#define SNOWHOUSECPU_IMM16_BITSIZE (16ull)
#define SNOWHOUSECPU_IMM16_BITPOS (0ull)
#define SNOWHOUSECPU_IMM16_RSMASK \
  (SNOWHOUSECPU_ENC_RSMASK (IMM16))
#define SNOWHOUSECPU_IMM16_MASK \
  (SNOWHOUSECPU_ENC_MASK (IMM16))

/* encoding of sub-operation field */
#define SNOWHOUSECPU_SUBOP_IMM16_BITSIZE (4ull)
#define SNOWHOUSECPU_SUBOP_IMM16_BITPOS (0ull)
#define SNOWHOUSECPU_SUBOP_IMM16_RSMASK \
  (SNOWHOUSECPU_ENC_RSMASK (SUBOP_IMM16))
#define SNOWHOUSECPU_SUBOP_IMM16_MASK \
  (SNOWHOUSECPU_ENC_MASK (SUBOP_IMM16))

/* encoding of shift immediate field */
#define SNOWHOUSECPU_SHIFT_IMM5_BITSIZE (5ull)
#define SNOWHOUSECPU_SHIFT_IMM5_BITPOS (4ull)
#define SNOWHOUSECPU_SHIFT_IMM5_RSMASK \
  (SNOWHOUSECPU_ENC_RSMASK (SHIFT_IMM5))
#define SNOWHOUSECPU_SHIFT_IMM5_MASK \
  (SNOWHOUSECPU_ENC_MASK (SHIFT_IMM5))

/* -------- */

/* the left shift amount for a single `pre`/`lpre`/`index` when storing a
  "whole" instruction in a `snowhousecpu_temp_t` */
#define SNOWHOUSECPU_ONE_EXT_BITPOS (32ull)
#define SNOWHOUSECPU_NO_EXT_MASK ((1ull << SNOWHOUSECPU_ONE_EXT_BITPOS) - 1ull)
#define SNOWHOUSECPU_ONE_EXT_MASK (~(SNOWHOUSECPU_NO_EXT_MASK))

#define SNOWHOUSECPU_PRE_S16_EXT_LSMASK \
  (SNOWHOUSECPU_IMM16_MASK << SNOWHOUSECPU_ONE_EXT_BITPOS)

//#define SNOWHOUSECPU_G0_PRE_S12_EXT_LSMASK 
//  (SNOWHOUSECPU_G0_PRE_S12_MASK << SNOWHOUSECPU_ONE_EXT_BITPOS)
//#define SNOWHOUSECPU_G0_LPRE_S27_EXT_LSMASK 
//  (SNOWHOUSECPU_G0_LPRE_S27_MASK << SNOWHOUSECPU_ONE_EXT_BITPOS)
//#define SNOWHOUSECPU_G0_LPRE_S25_EXT_LSMASK 
//  (SNOWHOUSECPU_G0_LPRE_S25_MASK << SNOWHOUSECPU_ONE_EXT_BITPOS)
//#define SNOWHOUSECPU_G0_LPRE_S23_EXT_LSMASK 
//  (SNOWHOUSECPU_G0_LPRE_S23_MASK << SNOWHOUSECPU_ONE_EXT_BITPOS)
//#define SNOWHOUSECPU_G0_LPRE_S27_EXT_LSMASK_G7_ICRELOAD
//  (SNOWHOUSECPU_G7_ICRELOAD_G0_LPRE_S27_MASK << SNOWHOUSECPU_ONE_EXT_BITPOS)
/* -------- */
typedef enum snowhousecpu_reg_kind_t
{
  SNOWHOUSECPU_REG_KIND_GPR,
  SNOWHOUSECPU_REG_KIND_SPR,
  SNOWHOUSECPU_REG_KIND_PC,
} snowhousecpu_reg_kind_t;

typedef struct snowhousecpu_reg_t
{
  const char *name;
  snowhousecpu_temp_t index;
  snowhousecpu_reg_kind_t kind;
} snowhousecpu_reg_t;
#define SNOWHOUSECPU_REG_NAME_MAX_LEN (10ull)
/* general-purpose registers */
#define SNOWHOUSECPU_NUM_GPRS (16ull)
extern const snowhousecpu_reg_t gprs[SNOWHOUSECPU_NUM_GPRS];

#define SNOWHOUSECPU_GPR_ENUM_R0 (0x0ull)
#define SNOWHOUSECPU_GPR_ENUM_R1 (0x1ull)
#define SNOWHOUSECPU_GPR_ENUM_R2 (0x2ull)
#define SNOWHOUSECPU_GPR_ENUM_R3 (0x3ull)
#define SNOWHOUSECPU_GPR_ENUM_R4 (0x4ull)
#define SNOWHOUSECPU_GPR_ENUM_R5 (0x5ull)
#define SNOWHOUSECPU_GPR_ENUM_R6 (0x6ull)
#define SNOWHOUSECPU_GPR_ENUM_R7 (0x7ull)
#define SNOWHOUSECPU_GPR_ENUM_R8 (0x8ull)
#define SNOWHOUSECPU_GPR_ENUM_R9 (0x9ull)
#define SNOWHOUSECPU_GPR_ENUM_R10 (0xaull)
#define SNOWHOUSECPU_GPR_ENUM_R11 (0xbull)
#define SNOWHOUSECPU_GPR_ENUM_R12 (0xcull)
#define SNOWHOUSECPU_GPR_ENUM_LR (0xdull)
#define SNOWHOUSECPU_GPR_ENUM_FP (0xeull)
#define SNOWHOUSECPU_GPR_ENUM_SP (0xfull)

#define SNOWHOUSECPU_INST_GPRS() \
  { \
    {"r0", SNOWHOUSECPU_GPR_ENUM_R0, SNOWHOUSECPU_REG_KIND_GPR}, \
    {"r1", SNOWHOUSECPU_GPR_ENUM_R1, SNOWHOUSECPU_REG_KIND_GPR}, \
    {"r2", SNOWHOUSECPU_GPR_ENUM_R2, SNOWHOUSECPU_REG_KIND_GPR}, \
    {"r3", SNOWHOUSECPU_GPR_ENUM_R3, SNOWHOUSECPU_REG_KIND_GPR}, \
    {"r4", SNOWHOUSECPU_GPR_ENUM_R4, SNOWHOUSECPU_REG_KIND_GPR}, \
    {"r5", SNOWHOUSECPU_GPR_ENUM_R5, SNOWHOUSECPU_REG_KIND_GPR}, \
    {"r6", SNOWHOUSECPU_GPR_ENUM_R6, SNOWHOUSECPU_REG_KIND_GPR}, \
    {"r7", SNOWHOUSECPU_GPR_ENUM_R7, SNOWHOUSECPU_REG_KIND_GPR}, \
    {"r8", SNOWHOUSECPU_GPR_ENUM_R8, SNOWHOUSECPU_REG_KIND_GPR}, \
    {"r9", SNOWHOUSECPU_GPR_ENUM_R9, SNOWHOUSECPU_REG_KIND_GPR}, \
    {"r10", SNOWHOUSECPU_GPR_ENUM_R10, SNOWHOUSECPU_REG_KIND_GPR}, \
    {"r11", SNOWHOUSECPU_GPR_ENUM_R11, SNOWHOUSECPU_REG_KIND_GPR}, \
    {"r12", SNOWHOUSECPU_GPR_ENUM_R12, SNOWHOUSECPU_REG_KIND_GPR}, \
    {"lr", SNOWHOUSECPU_GPR_ENUM_LR, SNOWHOUSECPU_REG_KIND_GPR}, \
    {"fp", SNOWHOUSECPU_GPR_ENUM_FP, SNOWHOUSECPU_REG_KIND_GPR}, \
    {"sp", SNOWHOUSECPU_GPR_ENUM_SP, SNOWHOUSECPU_REG_KIND_GPR}, \
  }

/* special-purpose registers */
//#define SNOWHOUSECPU_NUM_SPRS (16ull)
//#define SNOWHOUSECPU_REAL_NUM_SPRS (8ull)
#define SNOWHOUSECPU_NUM_SPRS (3ull)
//#define SNOWHOUSECPU_NUM_SPRS (6ull)
extern const snowhousecpu_reg_t sprs[SNOWHOUSECPU_NUM_SPRS];

//#define SNOWHOUSECPU_SPR_ENUM_FLAGS (0x0ull)
//#define SNOWHOUSECPU_SPR_ENUM_HI (0x1ull)
//#define SNOWHOUSECPU_SPR_ENUM_LO (0x2ull)
#define SNOWHOUSECPU_SPR_ENUM_IDS (0x0ull)
#define SNOWHOUSECPU_SPR_ENUM_IRA (0x1ull)
#define SNOWHOUSECPU_SPR_ENUM_IE (0x2ull)
//#define SNOWHOUSECPU_SPR_ENUM_ITY (0x4ull)
//#define SNOWHOUSECPU_SPR_ENUM_STY (0x5ull)
//#define SNOWHOUSECPU_SPR_ENUM_HI (0x6ull)
//#define SNOWHOUSECPU_SPR_ENUM_LO (0x7ull)

#define SNOWHOUSECPU_INST_SPRS() \
  { \
    /* {"flags", SNOWHOUSECPU_SPR_ENUM_FLAGS, SNOWHOUSECPU_REG_KIND_SPR}, */ \
    /* {"hi", SNOWHOUSECPU_SPR_ENUM_HI, SNOWHOUSECPU_REG_KIND_SPR}, */ \
    /* {"lo", SNOWHOUSECPU_SPR_ENUM_LO, SNOWHOUSECPU_REG_KIND_SPR}, */ \
    {"ids", SNOWHOUSECPU_SPR_ENUM_IDS, SNOWHOUSECPU_REG_KIND_SPR}, \
    {"ira", SNOWHOUSECPU_SPR_ENUM_IRA, SNOWHOUSECPU_REG_KIND_SPR}, \
    {"ie", SNOWHOUSECPU_SPR_ENUM_IE, SNOWHOUSECPU_REG_KIND_SPR}, \
    /* {"ity", SNOWHOUSECPU_SPR_ENUM_ITY, SNOWHOUSECPU_REG_KIND_SPR}, */ \
    /* {"sty", SNOWHOUSECPU_SPR_ENUM_STY, SNOWHOUSECPU_REG_KIND_SPR}, */ \
    /* {"hi", SNOWHOUSECPU_SPR_ENUM_HI, SNOWHOUSECPU_REG_KIND_SPR}, */ \
    /* {"lo", SNOWHOUSECPU_SPR_ENUM_LO, SNOWHOUSECPU_REG_KIND_SPR}, */ \
  }

/* bits of `flags` */
//#define SNOWHOUSECPU_FLAGS_Z_BITPOS ((snowhousecpu_temp_t) 0x0ull)
//#define SNOWHOUSECPU_FLAGS_C_BITPOS ((snowhousecpu_temp_t) 0x1ull)
//#define SNOWHOUSECPU_FLAGS_V_BITPOS ((snowhousecpu_temp_t) 0x2ull)
//#define SNOWHOUSECPU_FLAGS_N_BITPOS ((snowhousecpu_temp_t) 0x3ull)

//#define SNOWHOUSECPU_FLAGS_Z_MASK 
//  (((snowhousecpu_temp_t) 0x1ull) << SNOWHOUSECPU_FLAGS_Z_BITPOS) 
//#define SNOWHOUSECPU_FLAGS_C_MASK 
//  (((snowhousecpu_temp_t) 0x1ull) << SNOWHOUSECPU_FLAGS_C_BITPOS) 
//#define SNOWHOUSECPU_FLAGS_V_MASK 
//  (((snowhousecpu_temp_t) 0x1ull) << SNOWHOUSECPU_FLAGS_V_BITPOS) 
//#define SNOWHOUSECPU_FLAGS_N_MASK 
//  (((snowhousecpu_temp_t) 0x1ull) << SNOWHOUSECPU_FLAGS_N_BITPOS) 
//
//#define SNOWHOUSECPU_SIM_FLAGS_VN_MASK(bits) 
//  ((uint64_t) 0x1ull << (uint64_t) (bits - 1))
//#define SNOWHOUSECPU_SIM_FLAGS_Z_MASK(bits) 
//  (SNOWHOUSECPU_SIM_FLAGS_VN_MASK (bits) - (int64_t) 0x1ll)
//#define SNOWHOUSECPU_SIM_FLAGS_C_MASK(bits) 
//  (SNOWHOUSECPU_SIM_FLAGS_VN_MASK (bits) << (uint64_t) 0x1ull)

extern const snowhousecpu_reg_t reg_pc;
#define SNOWHOUSECPU_INST_REG_PC() \
  {"pc", 0x0, SNOWHOUSECPU_REG_KIND_PC}

#define SNOWHOUSECPU_SIM_FIRST_SPR_REGNO \
  (SNOWHOUSECPU_NUM_GPRS)
#define SNOWHOUSECPU_TOTAL_NUM_NON_PC_REGS \
  (SNOWHOUSECPU_NUM_GPRS + SNOWHOUSECPU_NUM_SPRS)
#define SNOWHOUSECPU_TOTAL_NUM_REGS \
  (SNOWHOUSECPU_TOTAL_NUM_NON_PC_REGS + 1ull)
/* -------- */
typedef enum snowhousecpu_have_pre_t {
  SNOWHOUSECPU_HAVE_PRE_NONE,
  SNOWHOUSECPU_HAVE_PRE_PRE,
} snowhousecpu_have_pre_t;
static inline unsigned
snowhousecpu_have_pre_insn_length (snowhousecpu_have_pre_t have_pre)
{
  switch (have_pre)
  {
    case SNOWHOUSECPU_HAVE_PRE_NONE:
      return 4;
    case SNOWHOUSECPU_HAVE_PRE_PRE:
      return 8;
    //case SNOWHOUSECPU_HAVE_PLP_LPRE:
    //  return 6;
    default:
      abort ();
      return 0;
  }
}
static inline unsigned
snowhousecpu_have_pre_distance
  (snowhousecpu_have_pre_t from, snowhousecpu_have_pre_t to)
{
  return (
    snowhousecpu_have_pre_insn_length (from)
    - snowhousecpu_have_pre_insn_length (to)
  );
}
typedef enum snowhousecpu_oparg_t
{
  SNOWHOUSECPU_OA_BAD,
  SNOWHOUSECPU_OA_NONE,
  SNOWHOUSECPU_OA_PRE,
  SNOWHOUSECPU_OA_RB,
  SNOWHOUSECPU_OA_RA_RB,
  SNOWHOUSECPU_OA_RA_RB_RC,
  SNOWHOUSECPU_OA_RA_RB_S16,
  //SNOWHOUSECPU_OA_RA_RB_U16,
  SNOWHOUSECPU_OA_RA_RB_SHIFT_U5,
  SNOWHOUSECPU_OA_RA_PCREL_S16,
  SNOWHOUSECPU_OA_RA_RB_PCREL_S16,
  SNOWHOUSECPU_OA_RB_RA_PCREL_S16,
  SNOWHOUSECPU_OA_PCREL_S16,
  SNOWHOUSECPU_OA_RA_PC_PCREL_S16,
  //SNOWHOUSECPU_OA_RA_RB_S16_LDST,
  SNOWHOUSECPU_OA_IDS_RB,
  SNOWHOUSECPU_OA_RA_IRA,
  SNOWHOUSECPU_OA_IE_RB,
  SNOWHOUSECPU_OA_IRA,
  SNOWHOUSECPU_OA_SIMM16,
} snowhousecpu_oparg_t;

typedef enum snowhousecpu_opc_subop_kind_t {
  SNOWHOUSECPU_SOK_NONE,
  SNOWHOUSECPU_SOK_RC_IDX_FULL,
  SNOWHOUSECPU_SOK_RC_IDX_NZ,
  SNOWHOUSECPU_SOK_IMM16_LO,
} snowhousecpu_opc_subop_kind_t;
typedef struct snowhousecpu_opc_subop_subop_t {
  snowhousecpu_temp_t is_non_ineqaulity_branch_etc: 2;
  snowhousecpu_opc_subop_kind_t kind: 2;
  snowhousecpu_temp_t val;
} snowhousecpu_opc_subop_t;
typedef struct snowhousecpu_opc_info_t {
  const char *name;
  snowhousecpu_oparg_t oparg;
  snowhousecpu_temp_t op;
  snowhousecpu_opc_subop_t subop;
} snowhousecpu_opc_info_t;


//#define SNOWHOUSECPU_ENUM_OP_ADD (0x0ull)
#define SNOWHOUSECPU_OI_INST_ADD_RA_RB_SIMM16 \
  {"add", SNOWHOUSECPU_OA_RA_RB_S16, 0ull, {0ull,SNOWHOUSECPU_SOK_RC_IDX_NZ, 0ull}}
#define SNOWHOUSECPU_OI_INST_ADD_RA_RB_RC \
  {"add", SNOWHOUSECPU_OA_RA_RB_RC, 0ull, {0ull, SNOWHOUSECPU_SOK_RC_IDX_NZ, 1ull}}
#define SNOWHOUSECPU_OI_INST_SUB_RA_RB_RC \
  {"sub", SNOWHOUSECPU_OA_RA_RB_RC, 1ull, {0ull, SNOWHOUSECPU_SOK_NONE, 0ull}}
//#define SubReserved {1ull, 1ull} // 1, 1
#define SNOWHOUSECPU_OI_INST_SLTU_RA_RB_RC \
  {"sltu", SNOWHOUSECPU_OA_RA_RB_RC, 2ull, {0ull, SNOWHOUSECPU_SOK_IMM16_LO, 0ull}}
#define SNOWHOUSECPU_OI_INST_SLTS_RA_RB_RC \
  {"slts", SNOWHOUSECPU_OA_RA_RB_RC, 2ull, {0ull, SNOWHOUSECPU_SOK_IMM16_LO, 1ull}}
  //--------
#define SNOWHOUSECPU_OI_INST_XOR_RA_RB_RC \
  {"xor", SNOWHOUSECPU_OA_RA_RB_RC, 3ull, {0ull, SNOWHOUSECPU_SOK_RC_IDX_NZ, 1ull}}
#define SNOWHOUSECPU_OI_INST_XOR_RA_RB_SIMM16 \
  {"xor", SNOWHOUSECPU_OA_RA_RB_S16, 3ull, {0ull, SNOWHOUSECPU_SOK_RC_IDX_NZ, 0ull}}

#define SNOWHOUSECPU_OI_INST_OR_RA_RB_SIMM16 \
  {"or", SNOWHOUSECPU_OA_RA_RB_S16, 4ull, {0ull, SNOWHOUSECPU_SOK_RC_IDX_NZ, 0ull}}
#define SNOWHOUSECPU_OI_INST_OR_RA_RB_RC \
  {"or", SNOWHOUSECPU_OA_RA_RB_RC, 4ull, {0ull, SNOWHOUSECPU_SOK_RC_IDX_NZ, 1ull}}

// `AND_RA_RB_SIMM16` is moved down here to help with synthesis/routing of
// executing/decoding instructions
#define SNOWHOUSECPU_OI_INST_AND_RA_RB_SIMM16 \
  {"and", SNOWHOUSECPU_OA_RA_RB_S16, 5ull, {0ull, SNOWHOUSECPU_SOK_NONE, 0ull}}

#define SNOWHOUSECPU_OI_INST_LSL_RA_RB_RC \
  {"lsl", SNOWHOUSECPU_OA_RA_RB_RC, 6ull, {0ull, SNOWHOUSECPU_SOK_IMM16_LO, 0ull}}
#define SNOWHOUSECPU_OI_INST_LSL_RA_RB_IMM5 \
  {"lsl", SNOWHOUSECPU_OA_RA_RB_SHIFT_U5, 6ull, {0ull, SNOWHOUSECPU_SOK_IMM16_LO, 1ull}}
#define SNOWHOUSECPU_OI_INST_LSR_RA_RB_RC \
  {"lsr", SNOWHOUSECPU_OA_RA_RB_RC, 6ull, {0ull, SNOWHOUSECPU_SOK_IMM16_LO, 2ull}}
#define SNOWHOUSECPU_OI_INST_LSR_RA_RB_IMM5 \
  {"lsr", SNOWHOUSECPU_OA_RA_RB_SHIFT_U5, 6ull, {0ull, SNOWHOUSECPU_SOK_IMM16_LO, 3ull}}
#define SNOWHOUSECPU_OI_INST_ASR_RA_RB_RC \
  {"asr", SNOWHOUSECPU_OA_RA_RB_RC, 6ull, {0ull, SNOWHOUSECPU_SOK_IMM16_LO, 4ull}}
#define SNOWHOUSECPU_OI_INST_ASR_RA_RB_IMM5 \
  {"asr", SNOWHOUSECPU_OA_RA_RB_SHIFT_U5, 6ull, {0ull, SNOWHOUSECPU_SOK_IMM16_LO, 5ull}}
#define SNOWHOUSECPU_OI_INST_AND_RA_RB_RC \
  {"and", SNOWHOUSECPU_OA_RA_RB_RC, 6ull, {0ull, SNOWHOUSECPU_SOK_IMM16_LO, 6ull}}
#define SNOWHOUSECPU_OI_INST_CPY_IDS_RB \
  {"cpy", SNOWHOUSECPU_OA_IDS_RB, 6ull, {0ull, SNOWHOUSECPU_SOK_IMM16_LO, 7ull}}
#define SNOWHOUSECPU_OI_INST_CPY_RA_IRA \
  {"cpy", SNOWHOUSECPU_OA_RA_IRA, 6ull, {0ull, SNOWHOUSECPU_SOK_IMM16_LO, 8ull}}
#define SNOWHOUSECPU_OI_INST_CPY_IE_RB \
  {"cpy", SNOWHOUSECPU_OA_IE_RB, 6ull, {0ull, SNOWHOUSECPU_SOK_IMM16_LO, 9ull}}
#define SNOWHOUSECPU_OI_INST_RET_IRA \
  {"ret", SNOWHOUSECPU_OA_IRA, 6ull, {0ull, SNOWHOUSECPU_SOK_IMM16_LO, 10ull}}

#define SNOWHOUSECPU_OI_INST_MUL_RA_RB_RC \
  {"mul", SNOWHOUSECPU_OA_RA_RB_RC, 7ull, {0ull, SNOWHOUSECPU_SOK_IMM16_LO, 0ull}}
#define SNOWHOUSECPU_OI_INST_UDIV_RA_RB_RC \
  {"udiv", SNOWHOUSECPU_OA_RA_RB_RC, 7ull, {0ull, SNOWHOUSECPU_SOK_IMM16_LO, 1ull}}
#define SNOWHOUSECPU_OI_INST_SDIV_RA_RB_RC \
  {"sdiv", SNOWHOUSECPU_OA_RA_RB_RC, 7ull, {0ull, SNOWHOUSECPU_SOK_IMM16_LO, 2ull}}
#define SNOWHOUSECPU_OI_INST_UMOD_RA_RB_RC \
  {"umod", SNOWHOUSECPU_OA_RA_RB_RC, 7ull, {0ull, SNOWHOUSECPU_SOK_IMM16_LO, 3ull}}
#define SNOWHOUSECPU_OI_INST_SMOD_RA_RB_RC \
  {"smod", SNOWHOUSECPU_OA_RA_RB_RC, 7ull, {0ull, SNOWHOUSECPU_SOK_IMM16_LO, 4ull}}

#define SNOWHOUSECPU_OI_INST_LDR_RA_RB_SIMM16 \
  {"ldr", SNOWHOUSECPU_OA_RA_RB_S16, 8ull, {0ull, SNOWHOUSECPU_SOK_RC_IDX_FULL, 0ull}}
#define SNOWHOUSECPU_OI_INST_STR_RA_RB_SIMM16 \
  {"str", SNOWHOUSECPU_OA_RA_RB_S16, 8ull, {0ull, SNOWHOUSECPU_SOK_RC_IDX_FULL, 1ull}}
#define SNOWHOUSECPU_OI_INST_LDUH_RA_RB_SIMM16 \
  {"lduh", SNOWHOUSECPU_OA_RA_RB_S16, 8ull, {0ull, SNOWHOUSECPU_SOK_RC_IDX_FULL, 2ull}}
#define SNOWHOUSECPU_OI_INST_LDSH_RA_RB_SIMM16 \
  {"ldsh", SNOWHOUSECPU_OA_RA_RB_S16, 8ull, {0ull, SNOWHOUSECPU_SOK_RC_IDX_FULL, 3ull}}
#define SNOWHOUSECPU_OI_INST_LDUB_RA_RB_SIMM16 \
  {"ldub", SNOWHOUSECPU_OA_RA_RB_S16, 8ull, {0ull, SNOWHOUSECPU_SOK_RC_IDX_FULL, 4ull}}
#define SNOWHOUSECPU_OI_INST_LDSB_RA_RB_SIMM16 \
  {"ldsb", SNOWHOUSECPU_OA_RA_RB_S16, 8ull, {0ull, SNOWHOUSECPU_SOK_RC_IDX_FULL, 5ull}}
#define SNOWHOUSECPU_OI_INST_STH_RA_RB_SIMM16 \
  {"sth", SNOWHOUSECPU_OA_RA_RB_S16, 8ull, {0ull, SNOWHOUSECPU_SOK_RC_IDX_FULL, 6ull}}
#define SNOWHOUSECPU_OI_INST_STB_RA_RB_SIMM16 \
  {"stb", SNOWHOUSECPU_OA_RA_RB_S16, 8ull, {0ull, SNOWHOUSECPU_SOK_RC_IDX_FULL, 7ull}}
  //--------
#define SNOWHOUSECPU_OI_INST_BEQ_RA_RB_SIMM16 \
  {"beq", SNOWHOUSECPU_OA_RA_RB_PCREL_S16, 9ull, {2ull, SNOWHOUSECPU_SOK_RC_IDX_FULL, 0ull}}
#define SNOWHOUSECPU_OI_INST_BL_RA_SIMM16 \
  {"bl", SNOWHOUSECPU_OA_RA_PCREL_S16, 9ull, {1ull, SNOWHOUSECPU_SOK_RC_IDX_FULL, 0ull}}
#define SNOWHOUSECPU_OI_INST_BL_SIMM16 \
  {"bl", SNOWHOUSECPU_OA_PCREL_S16, 9ull, {1ull, SNOWHOUSECPU_SOK_RC_IDX_FULL, 0ull}}
#define SNOWHOUSECPU_OI_INST_BNE_RA_RB_SIMM16 \
  {"bne", SNOWHOUSECPU_OA_RA_RB_PCREL_S16, 9ull, {2ull, SNOWHOUSECPU_SOK_RC_IDX_FULL, 1ull}}
#define SNOWHOUSECPU_OI_INST_ADD_RA_PC_SIMM16 \
  {"add", SNOWHOUSECPU_OA_RA_PC_PCREL_S16, 9ull, {1ull, SNOWHOUSECPU_SOK_RC_IDX_FULL, 1ull}}

#define SNOWHOUSECPU_OI_INST_BLTU_RA_RB_SIMM16 \
  {"bltu", SNOWHOUSECPU_OA_RA_RB_PCREL_S16, 9ull, {0ull, SNOWHOUSECPU_SOK_RC_IDX_FULL, 2ull}}
#define SNOWHOUSECPU_OI_INST_BGTU_RA_RB_SIMM16 \
  {"bgtu", SNOWHOUSECPU_OA_RB_RA_PCREL_S16, 9ull, {0ull, SNOWHOUSECPU_SOK_RC_IDX_FULL, 2ull}}

#define SNOWHOUSECPU_OI_INST_BGEU_RA_RB_SIMM16 \
  {"bgeu", SNOWHOUSECPU_OA_RA_RB_PCREL_S16, 9ull, {0ull, SNOWHOUSECPU_SOK_RC_IDX_FULL, 3ull}}
#define SNOWHOUSECPU_OI_INST_BLEU_RA_RB_SIMM16 \
  {"bleu", SNOWHOUSECPU_OA_RB_RA_PCREL_S16, 9ull, {0ull, SNOWHOUSECPU_SOK_RC_IDX_FULL, 3ull}}

#define SNOWHOUSECPU_OI_INST_BLTS_RA_RB_SIMM16 \
  {"blts", SNOWHOUSECPU_OA_RA_RB_PCREL_S16, 9ull, {0ull, SNOWHOUSECPU_SOK_RC_IDX_FULL, 4ull}}
#define SNOWHOUSECPU_OI_INST_BGTS_RA_RB_SIMM16 \
  {"bgts", SNOWHOUSECPU_OA_RB_RA_PCREL_S16, 9ull, {0ull, SNOWHOUSECPU_SOK_RC_IDX_FULL, 4ull}}

#define SNOWHOUSECPU_OI_INST_BGES_RA_RB_SIMM16 \
  {"bges", SNOWHOUSECPU_OA_RA_RB_PCREL_S16, 9ull, {0ull, SNOWHOUSECPU_SOK_RC_IDX_FULL, 5ull}}
#define SNOWHOUSECPU_OI_INST_BLES_RA_RB_SIMM16 \
  {"bles", SNOWHOUSECPU_OA_RB_RA_PCREL_S16, 9ull, {0ull, SNOWHOUSECPU_SOK_RC_IDX_FULL, 5ull}}

#define SNOWHOUSECPU_OI_INST_JL_RA_RB \
  {"jl", SNOWHOUSECPU_OA_RA_RB, 9ull, {0ull, SNOWHOUSECPU_SOK_RC_IDX_FULL, 6ull}}
#define SNOWHOUSECPU_OI_INST_JMP_RB \
  {"jmp", SNOWHOUSECPU_OA_RB, 9ull, {0ull, SNOWHOUSECPU_SOK_RC_IDX_FULL, 6ull}}

#define SNOWHOUSECPU_OI_INST_PRE_SIMM16 \
  {"pre", SNOWHOUSECPU_OA_SIMM16, 10ull, {0ull, SNOWHOUSECPU_SOK_NONE, 0ull}}



//static inline bool
//snowhousecpu_oparg_has_u5 (snowhousecpu_oparg_t oparg)
//{
//  return (
//    (oparg == SNOWHOUSECPU_OA_RA_U5) || (oparg == SNOWHOUSECPU_OA_U5)
//  );
//}

//typedef struct snowhousecpu_opsubop_info_t
//{
//  const snowhousecpu_enc_info_t *op;
//  unsigned op_value; /* The primary group, i.e. the top three bits */
//
//  const snowhousecpu_enc_info_t *subop;
//  unsigned subop_value;
//} snowhousecpu_opsubop_info_t;

#define SNOWHOUSECPU_OPC_INFO_NAME_MAX_LEN (15ull)
//#define SNOWHOUSECPU_OPC_INFO_NAMES_LIM (1ull)
//typedef struct snowhousecpu_opc_info_t
//{
//  bool is_add_ra_pc_simm16: 1;
//  bool is_bl_ra_simm16: 1;
//  bool is_jmp_rb: 1;
//  //const snowhousecpu_opsubop_info_t *opsubop_info;
//  //snowhousecpu_temp_t opcode;
//  //snowhousecpu_oparg_t oparg;
//  //const char *names[SNOWHOUSECPU_OPC_INFO_NAMES_LIM];
//  //const char *nr_names[SNOWHOUSECPU_OPC_INFO_NAMES_LIM];
//  const snowhousecpu_opc_info_t *main;
//} snowhousecpu_opc_info_t;

struct snowhousecpu_dasm_info_t;
typedef int (*snowhousecpu_dasm_info_rd32_func)
  (struct snowhousecpu_dasm_info_t * /* self */);
typedef struct snowhousecpu_dasm_info_t
{
  int length;
  int status;
  bool is_bad: 1;
  bool have_non_pre_relaxable_imm: 1;
  //bool grp_decode_err: 1;
  //bool g7_subgrp_decode_err: 1;
  //have_index_have_ra_ind: 1; // true for `rA`, false for `rB`
  //have_index_have_rb_ind: 1;
  //have_index_ra_rb: 1,
  //have_index_ra_simm: 1;
  //have_icreload: 1;
  const snowhousecpu_opc_info_t *opc_info;
  //const snowhousecpu_opc_info_t *opc_main;
  snowhousecpu_temp_t iword;
  snowhousecpu_temp_t simm;
  snowhousecpu_temp_t op;
  snowhousecpu_temp_t subop_imm16;
  snowhousecpu_temp_t subop_rc_idx;
  snowhousecpu_temp_t ra_idx;
  snowhousecpu_temp_t rb_idx;
  snowhousecpu_temp_t rc_idx;
  //index_ra_ind,
  //index_rb_ind,
  //icreload_ra_ind,
  //fw,
  //g7_aluopbh_subgrp,
  //g7_sprldst_subgrp,
  //g7_icreload_subgrp,
  //g7_icflush_subgrp;
  bfd_byte buffer[4];
  snowhousecpu_dasm_info_rd32_func rd32_func;
} snowhousecpu_dasm_info_t;


typedef struct snowhousecpu_opci_vec_t
{
  const snowhousecpu_opc_info_t **data;
  size_t size;
} snowhousecpu_opci_vec_t;

//extern snowhousecpu_opci_vec_t *snowhousecpu_opci_vec_create (void);
extern void snowhousecpu_opci_vec_create (snowhousecpu_opci_vec_t *self);
extern const snowhousecpu_opc_info_t *snowhousecpu_opci_vec_append
  (snowhousecpu_opci_vec_t *self, const snowhousecpu_opc_info_t *to_append);
extern void snowhousecpu_opci_vec_delete_data (snowhousecpu_opci_vec_t *self);

typedef struct snowhousecpu_opci_v2d_t
{
  snowhousecpu_opci_vec_t *data;
  size_t size;
} snowhousecpu_opci_v2d_t;
//extern snowhousecpu_opci_v2d_t *snowhousecpu_opci_v2d_create (void);
extern void snowhousecpu_opci_v2d_create (snowhousecpu_opci_v2d_t *self);
extern snowhousecpu_opci_vec_t *snowhousecpu_opci_v2d_append
  (snowhousecpu_opci_v2d_t *self, const snowhousecpu_opci_vec_t *to_append);
//extern void snowhousecpu_opci_v2d_append_opci (snowhousecpu_opci_v2d_t *self,
//  const snowhousecpu_opc_info_t *opc_info, size_t index);
extern void snowhousecpu_opci_v2d_delete_data (snowhousecpu_opci_v2d_t *self);



#define SNOWHOUSECPU_OPC_INFO_NULL_OP ((snowhousecpu_oparg_t)(-1ll))
//#define SNOWHOUSECPU_OPC_INFO_PSEUDO_OP (-2ull)

//#define SNOWHOUSECPU_G0_OPC_INFO_LIM (6ull)
////extern const snowhousecpu_opc_info_t
////  snowhousecpu_opc_info_g0[SNOWHOUSECPU_G0_OPC_INFO_LIM];
//
//#define SNOWHOUSECPU_G1_OPC_INFO_LIM (16ull + 1ull)
////extern const snowhousecpu_opc_info_t
////  snowhousecpu_opc_info_g1[SNOWHOUSECPU_G1_OPC_INFO_LIM];
//
//#define SNOWHOUSECPU_G2_OPC_INFO_LIM (16ull)
////extern const snowhousecpu_opc_info_t
////  snowhousecpu_opc_info_g2[SNOWHOUSECPU_G2_OPC_INFO_LIM];
//
//#define SNOWHOUSECPU_G3_OPC_INFO_LIM (16ull)
////#define SNOWHOUSECPU_G3_OPC_INFO_LIM (16ull + 16ull)
////#define SNOWHOUSECPU_G3_OPC_INFO_LIM (16ull + 1ull)
////extern const snowhousecpu_opc_info_t
////  snowhousecpu_opc_info_g3[SNOWHOUSECPU_G3_OPC_INFO_LIM];
//
//#define SNOWHOUSECPU_G4_OPC_INFO_LIM ( 
//  32ull + 5ull + 6ull + 7ull + 5ull + 7ull 
//)
////extern const snowhousecpu_opc_info_t
////  snowhousecpu_opc_info_g4[SNOWHOUSECPU_G4_OPC_INFO_LIM];
//
//#define SNOWHOUSECPU_G5_INDEX_RA_RB_OPC_INFO_LIM (1ull)
//#define SNOWHOUSECPU_G5_INDEX_RA_SIMM_OPC_INFO_LIM (1ull)
////extern const snowhousecpu_opc_info_t
////  snowhousecpu_opc_info_g5[SNOWHOUSECPU_G5_OPC_INFO_LIM];
//
//#define SNOWHOUSECPU_G6_OPC_INFO_LIM (4ull)
////extern const snowhousecpu_opc_info_t
////  snowhousecpu_opc_info_g6[SNOWHOUSECPU_G6_OPC_INFO_LIM];
//
//#define SNOWHOUSECPU_G7_ALUOPBH_OPC_INFO_LIM (4ull)
////extern const snowhousecpu_opc_info_t
////  snowhousecpu_opc_info_g7_aluopbh[SNOWHOUSECPU_G7_ALUOPBH_OPC_INFO_LIM];
//
//#define SNOWHOUSECPU_G7_SPRLDST_OPC_INFO_LIM (8ull)
////extern const snowhousecpu_opc_info_t
////  snowhousecpu_opc_info_g7_sprldst[SNOWHOUSECPU_G7_SPRLDST_OPC_INFO_LIM];
//
//#define SNOWHOUSECPU_G7_ICRELOAD_OPC_INFO_LIM (4ull)
////extern const snowhousecpu_opc_info_t
////  snowhousecpu_opc_info_g7_icreload[SNOWHOUSECPU_G7_ICRELOAD_OPC_INFO_LIM];
//
//#define SNOWHOUSECPU_G7_ICFLUSH_OPC_INFO_LIM (1ull)

/* This is definitely excessive, but it should work fine */
#define SNOWHOUSECPU_MAX_UNIQUE_OPCI_NAMES (512ull)

//typedef struct snowhousecpu_enc_instr_t {
//  snowhousecpu_temp_t op: 4;
//  snowhousecpu_temp_t ra_idx: 4;
//  snowhousecpu_temp_t rb_idx: 4;
//  snowhousecpu_temp_t rc_idx: 4;
//} snowhousecpu_enc_instr_t;
/* -------- */
static inline snowhousecpu_temp_t
snowhousecpu_enc_temp_insn (snowhousecpu_temp_t op,
				snowhousecpu_temp_t ra_idx,
				snowhousecpu_temp_t rb_idx,
				snowhousecpu_temp_t rc_idx,
				snowhousecpu_temp_t imm16)
{
  snowhousecpu_temp_t insn = 0x0;
  snowhousecpu_set_insn_field_p (SNOWHOUSECPU_OP_MASK, SNOWHOUSECPU_OP_BITPOS, &insn, op);
  snowhousecpu_set_insn_field_p (SNOWHOUSECPU_RA_IDX_MASK, SNOWHOUSECPU_RA_IDX_BITPOS, &insn, ra_idx);
  snowhousecpu_set_insn_field_p (SNOWHOUSECPU_RB_IDX_MASK, SNOWHOUSECPU_RB_IDX_BITPOS, &insn, rb_idx);
  snowhousecpu_set_insn_field_p (SNOWHOUSECPU_RC_IDX_MASK, SNOWHOUSECPU_RC_IDX_BITPOS, &insn, rc_idx);
  snowhousecpu_set_insn_field_p (SNOWHOUSECPU_IMM16_MASK, SNOWHOUSECPU_IMM16_BITPOS, &insn, imm16);
  return insn;
}
//typedef struct snowhousecpu_dasm_temp_instr_t {
//  const snowhousecpu_opc_info_t *opc_info;
//  snowhousecpu_temp_t ra_idx;
//  snowhousecpu_temp_t rb_idx;
//  snowhousecpu_temp_t rc_idx;
//} snowhousecpu_dasm_temp_instr_t;
//static inline snowhousecpu_temp_t
//snowhousecpu_enc_temp_insn_pre (snowhousecpu_temp_t simm16)
//{
//  return snowhousecpu_enc_temp_insn (
//    /* op */
//  );
//}
/* -------- */
/* -------- */
//static inline snowhousecpu_temp_t
//snowhousecpu_enc_temp_insn_g7_aluopbh (snowhousecpu_temp_t op,
//                                  snowhousecpu_temp_t w,
//                                  snowhousecpu_temp_t ra_ind,
//                                  snowhousecpu_temp_t rb_ind)
//{
//  snowhousecpu_temp_t ret = 0;
//  snowhousecpu_set_insn_field_p (SNOWHOUSECPU_G7_ALUOPBH_FULLGRP_MASK,
//    SNOWHOUSECPU_G7_ALUOPBH_FULLGRP_BITPOS, &ret,
//    SNOWHOUSECPU_G7_ALUOPBH_FULLGRP_VALUE);
//  snowhousecpu_set_insn_field_p (SNOWHOUSECPU_G7_ALUOPBH_OP_MASK,
//    SNOWHOUSECPU_G7_ALUOPBH_OP_BITPOS, &ret, op);
//  snowhousecpu_set_insn_field_p (SNOWHOUSECPU_G7_ALUOPBH_W_MASK,
//    SNOWHOUSECPU_G7_ALUOPBH_W_BITPOS, &ret, w);
//  snowhousecpu_set_insn_field_p (SNOWHOUSECPU_RA_IDX_MASK, SNOWHOUSECPU_RA_IDX_BITPOS, 
//    &ret, ra_ind);
//  snowhousecpu_set_insn_field_p (SNOWHOUSECPU_RB_IDX_MASK, SNOWHOUSECPU_RB_IDX_BITPOS, 
//    &ret, rb_ind);
//  return ret;
//}
/* -------- */
static inline snowhousecpu_temp_t
snowhousecpu_sign_extend (snowhousecpu_temp_t value,
                    snowhousecpu_temp_t bits)
{
  if (value & (1ull << (bits - 1ull)))
  {
    /* VALUE is negative.  */
    value |= ((snowhousecpu_temp_t) -1ll) << bits;
  }
  else
  {
    value &= ~(((snowhousecpu_temp_t) -1ll) << bits)
      | ((((snowhousecpu_temp_t) 1ull) << bits) - 1ll);
  }

  return value;
}
static inline snowhousecpu_temp_t
snowhousecpu_zero_extend (snowhousecpu_temp_t value,
                    snowhousecpu_temp_t bits)
{
  value &= ~(((snowhousecpu_temp_t) -1ll) << bits)
    | ((((snowhousecpu_temp_t) 1ull) << bits) - 1ll);
  return value;
}

static inline snowhousecpu_temp_t
snowhousecpu_get_ext_imm (snowhousecpu_temp_t prefix_mask,
                    snowhousecpu_temp_t prefix_bitpos,
                    snowhousecpu_temp_t prefix_insn,
                    snowhousecpu_temp_t insn_mask,
                    snowhousecpu_temp_t insn_bitsize,
                    snowhousecpu_temp_t insn_bitpos,
                    snowhousecpu_temp_t insn)
{
  //return (snowhousecpu_temp_t) (
  //  (((prefix_insn & prefix_mask) >> prefix_bitpos) << insn_bitsize)
  //  | ((insn & insn_mask) >> insn_bitpos)
  //);
  return (snowhousecpu_temp_t) (
    (snowhousecpu_get_insn_field (prefix_mask, prefix_bitpos, prefix_insn)
      << insn_bitsize)
    | snowhousecpu_get_insn_field (insn_mask, insn_bitpos, insn)
  );
}

//static inline snowhousecpu_temp_t
//snowhousecpu_get_ext_imm (const snowhousecpu_enc_info_t *prefix_enc_info,
//                    snowhousecpu_temp_t prefix_insn,
//                    const snowhousecpu_enc_info_t *insn_enc_info,
//                    snowhousecpu_temp_t insn)
//{
//  return snowhousecpu_get_ext_imm_worker
//    (prefix_enc_info->mask, /* prefix_mask, */
//    prefix_enc_info->bitpos, /* prefix_bitpos, */
//    prefix_insn, /* prefix_insn, */
//    insn_enc_info->mask, /* insn_mask, */
//    insn_enc_info->bitsize, /* insn_bitsize, */
//    insn_enc_info->bitpos, /* insn_bitpos, */
//    insn /* insn */);
//}


//static inline snowhousecpu_temp_t
//snowhousecpu_get_g7_icreload_s17 (snowhousecpu_temp_t prefix_insn,
//                        snowhousecpu_temp_t insn)
//{
//  return snowhousecpu_get_ext_imm 
//    (SNOWHOUSECPU_G0_PRE_S12_MASK,
//    SNOWHOUSECPU_G0_PRE_S12_BITPOS,
//    prefix_insn,
//    SNOWHOUSECPU_G7_ICRELOAD_S5_MASK,
//    SNOWHOUSECPU_G7_ICRELOAD_S5_BITSIZE,
//    SNOWHOUSECPU_G7_ICRELOAD_S5_BITPOS,
//    insn);
//}
//static inline snowhousecpu_temp_t
//snowhousecpu_get_g7_icreload_s32 (snowhousecpu_temp_t prefix_insn,
//                        snowhousecpu_temp_t insn)
//{
//  return snowhousecpu_get_ext_imm
//    (SNOWHOUSECPU_G7_ICRELOAD_G0_LPRE_S27_MASK,
//    SNOWHOUSECPU_G7_ICRELOAD_G0_LPRE_S27_BITPOS,
//    prefix_insn,
//    SNOWHOUSECPU_G7_ICRELOAD_S5_MASK,
//    SNOWHOUSECPU_G7_ICRELOAD_S5_BITSIZE,
//    SNOWHOUSECPU_G7_ICRELOAD_S5_BITPOS,
//    insn);
//}

static inline snowhousecpu_temp_t
snowhousecpu_get_s32 (snowhousecpu_temp_t prefix_insn,
		      snowhousecpu_temp_t insn)
{
  snowhousecpu_temp_t ret = 0;
  const snowhousecpu_temp_t temp = (
    (
      snowhousecpu_get_insn_field (SNOWHOUSECPU_IMM16_MASK, SNOWHOUSECPU_IMM16_BITPOS, prefix_insn)
      << SNOWHOUSECPU_IMM16_BITSIZE
    ) | (
      snowhousecpu_get_insn_field (SNOWHOUSECPU_IMM16_MASK, SNOWHOUSECPU_IMM16_BITPOS, insn)
    )
  );
  ret = snowhousecpu_sign_extend (temp, 32);
  return ret;
}
static inline void
snowhousecpu_put_s32_p (snowhousecpu_temp_t *prefix_insn,
		      snowhousecpu_temp_t *insn,
		      snowhousecpu_temp_t combined)
{
  snowhousecpu_set_insn_field_p (
    SNOWHOUSECPU_IMM16_MASK, SNOWHOUSECPU_IMM16_BITPOS, prefix_insn,
    combined >> SNOWHOUSECPU_IMM16_BITSIZE
  );
  snowhousecpu_set_insn_field_p (
    SNOWHOUSECPU_IMM16_MASK, SNOWHOUSECPU_IMM16_BITPOS, insn,
    combined
  );
}
static inline snowhousecpu_temp_t 
snowhousecpu_get_shift_u5 (snowhousecpu_temp_t insn)
{
  return snowhousecpu_get_insn_field (
    SNOWHOUSECPU_SHIFT_IMM5_MASK, SNOWHOUSECPU_SHIFT_IMM5_BITPOS, insn
  );
}
static inline void
snowhousecpu_put_shift_u5_p (snowhousecpu_temp_t *insn,
			  snowhousecpu_temp_t imm)
{
  snowhousecpu_set_insn_field_p (
    SNOWHOUSECPU_SHIFT_IMM5_MASK, SNOWHOUSECPU_SHIFT_IMM5_BITPOS, insn, imm
  );
}

static inline void
snowhousecpu_put_ext_imm (snowhousecpu_temp_t prefix_mask,
                    snowhousecpu_temp_t prefix_bitpos,
                    snowhousecpu_temp_t *prefix_insn,
                    snowhousecpu_temp_t insn_mask,
                    snowhousecpu_temp_t insn_bitsize,
                    snowhousecpu_temp_t insn_bitpos,
                    snowhousecpu_temp_t *insn,
                    snowhousecpu_temp_t combined)
{
  snowhousecpu_set_insn_field_p (insn_mask, insn_bitpos, insn, combined);
  *prefix_insn = snowhousecpu_set_insn_field (prefix_mask, prefix_bitpos,
    *prefix_insn,
    combined >> insn_bitsize);
}
/* -------- */
#endif    // _SNOWHOUSECPU_H_

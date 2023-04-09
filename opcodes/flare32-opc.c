/* flare32-opc.c -- Definitions for flare32 opcodes.
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

#include "sysdep.h"
#include "opcode/flare32.h"

const flare32_grp_info_t flare32_grp_info_pre =
{
  .grp_bitsize=FLARE32_GRP_BITSIZE,
  .grp_bitpos=FLARE32_GRP_16_BITPOS,
  .grp_rsmask=FLARE32_GRP_RSMASK,
  .grp_mask=FLARE32_GRP_16_MASK,
  .grp=FLARE32_PRE_LPRE_GRP_VALUE,

  .has_subgrp=true,
  .subgrp_bitsize=FLARE32_PRE_SUBGRP_BITSIZE,
  .subgrp_bitpos=FLARE32_PRE_SUBGRP_BITPOS,
  .subgrp_rsmask=FLARE32_PRE_SUBGRP_RSMASK,
  .subgrp_mask=FLARE32_PRE_SUBGRP_MASK,
  .subgrp=0,
};
const flare32_opc_info_t
  flare32_opc_info_pre_lpre[FLARE32_PRE_LPRE_OPC_INFO_LIM] =
{
  /* pre #simm */
  {&flare32_grp_info_pre,
    FLARE32_OPC_INFO_NULL_OP, FLARE32_OA_PRE,
    {"pre", "pre"}},

  /* lpre #simm */
  {&flare32_grp_info_lpre,
    FLARE32_OPC_INFO_NULL_OP, FLARE32_OA_LPRE,
    {"lpre", "lpre"}},
};

const flare32_grp_info_t flare32_grp_info_g1 =
{
  .grp_bitsize=FLARE32_GRP_BITSIZE,
  .grp_bitpos=FLARE32_GRP_16_BITPOS,
  .grp_rsmask=FLARE32_GRP_RSMASK,
  .grp_mask=FLARE32_GRP_16_MASK,
  .grp=FLARE32_G1_GRP_VALUE,

  .has_subgrp=false,
  .subgrp_bitsize=0,
  .subgrp_bitpos=0,
  .subgrp_rsmask=0,
  .subgrp_mask=0,
  .subgrp=0,
};
const flare32_opc_info_t
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
  /* zeb rA */
  {&flare32_grp_info_g1,
    FLARE32_G1_OP_ENUM_ZEB_RA, FLARE32_OA_RA,
    {"zeb", "zeb"}},

  /* zeh rA */
  {&flare32_grp_info_g1,
    FLARE32_G1_OP_ENUM_ZEH_RA, FLARE32_OA_RA,
    {"zeh", "zeh"}},

  /* seb rA */
  {&flare32_grp_info_g1,
    FLARE32_G1_OP_ENUM_SEB_RA, FLARE32_OA_RA,
    {"seb", "seb"}},

  /* seh rA */
  {&flare32_grp_info_g1,
    FLARE32_G1_OP_ENUM_SEH_RA, FLARE32_OA_RA,
    {"seh", "seh"}},
  /* -------- */
};

const flare32_grp_info_t flare32_grp_info_g2 =
{
  .grp_bitsize=FLARE32_GRP_BITSIZE,
  .grp_bitpos=FLARE32_GRP_16_BITPOS,
  .grp_rsmask=FLARE32_GRP_RSMASK,
  .grp_mask=FLARE32_GRP_16_MASK,
  .grp=FLARE32_G2_GRP_VALUE,

  .has_subgrp=false,
  .subgrp_bitsize=0,
  .subgrp_bitpos=0,
  .subgrp_rsmask=0,
  .subgrp_mask=0,
  .subgrp=0,
};
const flare32_opc_info_t
  flare32_opc_info_g2[FLARE32_G2_OPC_INFO_LIM] =
{
  /* -------- */
  /* add rA, rC */
  {&flare32_grp_info_g2,
    FLARE32_G2_OP_ENUM_ADD_RA_RC, FLARE32_OA_RA_RC,
    {"add", "add.f"}},

  /* sub rA, rC */
  {&flare32_grp_info_g2,
    FLARE32_G2_OP_ENUM_SUB_RA_RC, FLARE32_OA_RA_RC,
    {"sub", "sub.f"}},

  /* add rA, sp, rC */
  {&flare32_grp_info_g2,
    FLARE32_G2_OP_ENUM_ADD_RA_SP_RC, FLARE32_OA_RA_SP_RC, 
    {"add", "add.f"}},

  /* add rA, fp, rC */
  {&flare32_grp_info_g2,
    FLARE32_G2_OP_ENUM_ADD_RA_FP_RC, FLARE32_OA_RA_FP_RC,
    {"add", "add.f"}},
  /* -------- */
  /* cmp rA, rC */
  {&flare32_grp_info_g2,
    FLARE32_G2_OP_ENUM_CMP_RA_RC, FLARE32_OA_RA_RC,
    {"cmp", "cmp.f"}},

  /* cpy rA, rC */
  {&flare32_grp_info_g2,
    FLARE32_G2_OP_ENUM_CPY_RA_RC, FLARE32_OA_RA_RC,
    {"cpy", "cpy.f"}},

  /* lsl rA, rC */
  {&flare32_grp_info_g2,
    FLARE32_G2_OP_ENUM_LSL_RA_RC, FLARE32_OA_RA_RC,
    {"lsl", "lsl.f"}},

  /* lsr rA, rC */
  {&flare32_grp_info_g2,
    FLARE32_G2_OP_ENUM_LSR_RA_RC, FLARE32_OA_RA_RC,
    {"lsr", "lsr.f"}},
  /* -------- */
  /* asr rA, rC */
  {&flare32_grp_info_g2,
    FLARE32_G2_OP_ENUM_ASR_RA_RC, FLARE32_OA_RA_RC, 
    {"asr", "asr.f"}},

  /* and rA, rC */
  {&flare32_grp_info_g2,
    FLARE32_G2_OP_ENUM_AND_RA_RC, FLARE32_OA_RA_RC,
    {"and", "and.f"}},

  /* orr rA, rC */
  {&flare32_grp_info_g2,
    FLARE32_G2_OP_ENUM_ORR_RA_RC, FLARE32_OA_RA_RC,
    {"orr", "orr.f"}},

  /* xor rA, rC */
  {&flare32_grp_info_g2,
    FLARE32_G2_OP_ENUM_XOR_RA_RC, FLARE32_OA_RA_RC,
    {"xor", "xor.f"}},
  /* -------- */
  /* adc rA, rC  */
  {&flare32_grp_info_g2,
    FLARE32_G2_OP_ENUM_ADC_RA_RC, FLARE32_OA_RA_RC,
    {"adc", "adc.f"}},

  /* sbc rA, rC */
  {&flare32_grp_info_g2,
    FLARE32_G2_OP_ENUM_SBC_RA_RC, FLARE32_OA_RA_RC,
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

const flare32_grp_info_t flare32_grp_info_g3 =
{
  .grp_bitsize=FLARE32_GRP_BITSIZE,
  .grp_bitpos=FLARE32_GRP_16_BITPOS,
  .grp_rsmask=FLARE32_GRP_RSMASK,
  .grp_mask=FLARE32_GRP_16_MASK,
  .grp=FLARE32_G3_GRP_VALUE,

  .has_subgrp=false,
  .subgrp_bitsize=0,
  .subgrp_bitpos=0,
  .subgrp_rsmask=0,
  .subgrp_mask=0,
  .subgrp=0,
};
const flare32_opc_info_t
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
};
const flare32_grp_info_t flare32_grp_info_g4 =
{
  .grp_bitsize=FLARE32_GRP_BITSIZE,
  .grp_bitpos=FLARE32_GRP_16_BITPOS,
  .grp_rsmask=FLARE32_GRP_RSMASK,
  .grp_mask=FLARE32_GRP_16_MASK,
  .grp=FLARE32_G4_GRP_VALUE,

  .has_subgrp=false,
  .subgrp_bitsize=0,
  .subgrp_bitpos=0,
  .subgrp_rsmask=0,
  .subgrp_mask=0,
  .subgrp=0,
};
const flare32_opc_info_t
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

  /* jmp rA, rC */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_JMP_RA_RC, FLARE32_OA_RA_RC,
    {"jmp", "jmp"}},

  /* jmp ira */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_JMP_IRA, FLARE32_OA_IRA,
    {"jmp", "jmp"}},
  /* -------- */
  /* reti */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_RETI, FLARE32_OA_NONE,
    {"reti", "reti"}},

  /* cpy rA, sC */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_CPY_RA_SC, FLARE32_OA_RA_SC,
    {"cpy", "cpy"}},

  /* cpy sA, rC */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_CPY_SA_RC, FLARE32_OA_SA_RC,
    {"cpy", "cpy"}},

  /* ei */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_EI, FLARE32_OA_NONE,
    {"ei", "ei"}},
  /* -------- */
  /* di */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_DI, FLARE32_OA_NONE,
    {"di", "di"}},

  /* push rA, rC */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_PUSH_RA_RC, FLARE32_OA_RA_RC,
    {"push", "push"}},

  /* pop rA, rC */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_POP_RA_RC, FLARE32_OA_RA_RC,
    {"pop", "pop"}},

  /* push sA, rC */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_PUSH_SA_RC, FLARE32_OA_SA_RC,
    {"push", "push"}},
  /* -------- */
  /* pop sA, rC */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_POP_SA_RC, FLARE32_OA_SA_RC,
    {"pop", "pop"}},

  /* index rA */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_INDEX_RA, FLARE32_OA_RA,
    {"index", "index"}},

  /* mul rA, rC */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_MUL_RA_RC, FLARE32_OA_RA_RC,
    {"mul", "mul"}},

  /* udiv rA, rC */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_UDIV_RA_RC, FLARE32_OA_RA_RC,
    {"udiv", "udiv"}},

  /* -------- */

  /* sdiv rA, rC */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_SDIV_RA_RC, FLARE32_OA_RA_RC,
    {"sdiv", "sdiv"}},

  /* umod rA, rC */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_UMOD_RA_RC, FLARE32_OA_RA_RC,
    {"umod", "umod"}},

  /* smod rA, rC */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_SMOD_RA_RC, FLARE32_OA_RA_RC,
    {"smod", "smod"}},

  /* lumul rA, rC */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_LUMUL_RA_RC, FLARE32_OA_RA_RC,
    {"lumul", "lumul"}},
  /* -------- */
  /* lsmul rA, rC */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_LSMUL_RA_RC, FLARE32_OA_RA_RC,
    {"lsmul", "lsmul"}},

  /* ludiv rA, rC */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_LUDIV_RA_RC, FLARE32_OA_RA_RC,
    {"ludiv", "ludiv"}},

  /* lsdiv rA, rC */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_LSDIV_RA_RC, FLARE32_OA_RA_RC,
    {"lsdiv", "lsdiv"}},

  /* lumod rA, rC */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_LUMOD_RA_RC, FLARE32_OA_RA_RC,
    {"lumod", "lumod"}},
  /* -------- */
  /* lsmod rA, rC */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_LSMOD_RA_RC, FLARE32_OA_RA_RC,
    {"lsmod", "lsmod"}},

  /* ldub rA, [rC] */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_LDUB_RA_RC, FLARE32_OA_RA_RC_LDST,
    {"ldub", "ldub"}},

  /* ldsb rA, [rC] */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_LDSB_RA_RC, FLARE32_OA_RA_RC_LDST,
    {"ldsb", "ldsb"}},

  /* lduh rA, [rC] */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_LDUH_RA_RC, FLARE32_OA_RA_RC_LDST,
    {"lduh", "lduh"}},
  /* -------- */
  /* ldsh rA, [rC] */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_LDSH_RA_RC, FLARE32_OA_RA_RC_LDST,
    {"ldsh", "ldsh"}},

  /* stb rA, [rC] */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_STB_RA_RC, FLARE32_OA_RA_RC_LDST,
    {"stb", "stb"}},

  /* sth rA, [rC] */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_STH_RA_RC, FLARE32_OA_RA_RC_LDST,
    {"sth", "sth"}},

  /* reserved */
  {&flare32_grp_info_g4,
    FLARE32_G4_OP_ENUM_RESERVED_31, FLARE32_OA_BAD,
    {"bad", "bad"}},
  /* -------- */
  /* Following are some pseudo instructions. */
  /* ldub rA, [rC, rB] */
  {&flare32_grp_info_g4,
    FLARE32_OPC_INFO_PSEUDO_OP, FLARE32_OA_RA_RC_RB_LDST,
    {"ldub", "ldub"}},

  /* ldsb rA, [rC, rB] */
  {&flare32_grp_info_g4,
    FLARE32_OPC_INFO_PSEUDO_OP, FLARE32_OA_RA_RC_RB_LDST,
    {"ldsb", "ldsb"}},

  /* lduh rA, [rC, rB] */
  {&flare32_grp_info_g4,
    FLARE32_OPC_INFO_PSEUDO_OP, FLARE32_OA_RA_RC_RB_LDST,
    {"lduh", "lduh"}},

  /* ldsh rA, [rC, rB] */
  {&flare32_grp_info_g4,
    FLARE32_OPC_INFO_PSEUDO_OP, FLARE32_OA_RA_RC_RB_LDST,
    {"ldsh", "ldsh"}},
  /* -------- */
  /* stb rA, [rC, rB] */
  {&flare32_grp_info_g4,
    FLARE32_OPC_INFO_PSEUDO_OP, FLARE32_OA_RA_RC_RB_LDST,
    {"stb", "stb"}},

  /* sth rA, [rC, rB] */
  {&flare32_grp_info_g4,
    FLARE32_OPC_INFO_PSEUDO_OP, FLARE32_OA_RA_RC_RB_LDST,
    {"sth", "sth"}},
  /* -------- */
};
const flare32_grp_info_t flare32_grp_info_g5 =
{
  .grp_bitsize=FLARE32_GRP_BITSIZE,
  .grp_bitpos=FLARE32_GRP_16_BITPOS,
  .grp_rsmask=FLARE32_GRP_RSMASK,
  .grp_mask=FLARE32_GRP_16_MASK,
  .grp=FLARE32_G5_GRP_VALUE,

  .has_subgrp=false,
  .subgrp_bitsize=0,
  .subgrp_bitpos=0,
  .subgrp_rsmask=0,
  .subgrp_mask=0,
  .subgrp=0,
};
const flare32_opc_info_t
  flare32_opc_info_g5[FLARE32_G5_OPC_INFO_LIM] =
{
  /* -------- */
  /* ldr rA, [rC, #simm] */
  {&flare32_grp_info_g5,
    FLARE32_OPC_INFO_NULL_OP, FLARE32_OA_RA_RC_S5_LDST,
    {"ldr", "ldr"}},
  /* -------- */
  /* Following are some pseudo instructions */
  /* ldr rA, [rC] */
  {&flare32_grp_info_g5,
    FLARE32_OPC_INFO_PSEUDO_OP, FLARE32_OA_RA_RC_LDST,
    {"ldr", "ldr"}},

  /* ldr rA, [rC, rB] */
  {&flare32_grp_info_g5,
    FLARE32_OPC_INFO_PSEUDO_OP, FLARE32_OA_RA_RC_RB_LDST,
    {"ldr", "ldr"}},

  /* ldr rA, [rC, rB, #simm] */
  {&flare32_grp_info_g5,
    FLARE32_OPC_INFO_PSEUDO_OP, FLARE32_OA_RA_RC_RB_S5_LDST,
    {"ldr", "ldr"}},
  /* -------- */
};

const flare32_grp_info_t flare32_grp_info_g6 =
{
  .grp_bitsize=FLARE32_GRP_BITSIZE,
  .grp_bitpos=FLARE32_GRP_16_BITPOS,
  .grp_rsmask=FLARE32_GRP_RSMASK,
  .grp_mask=FLARE32_GRP_16_MASK,
  .grp=FLARE32_G6_GRP_VALUE,

  .has_subgrp=false,
  .subgrp_bitsize=0,
  .subgrp_bitpos=0,
  .subgrp_rsmask=0,
  .subgrp_mask=0,
  .subgrp=0,
};
const flare32_opc_info_t
  flare32_opc_info_g6[FLARE32_G6_OPC_INFO_LIM] =
{
  /* -------- */
  /* str rA, [rC, #simm] */
  {&flare32_grp_info_g6,
    FLARE32_OPC_INFO_NULL_OP, FLARE32_OA_RA_RC_S5_LDST,
    {"str", "str"}},
  /* -------- */
  /* Following are some pseudo instructions */
  /* st rA, [rC] */
  {&flare32_grp_info_g6,
    FLARE32_OPC_INFO_PSEUDO_OP, FLARE32_OA_RA_RC_LDST,
    {"str", "str"}},

  /* str rA, [rC, rB] */
  {&flare32_grp_info_g6,
    FLARE32_OPC_INFO_PSEUDO_OP, FLARE32_OA_RA_RC_RB_LDST,
    {"str", "str"}},

  /* str rA, [rC, rB, #simm] */
  {&flare32_grp_info_g6,
    FLARE32_OPC_INFO_PSEUDO_OP, FLARE32_OA_RA_RC_RB_S5_LDST,
    {"str", "str"}},
  /* -------- */
};

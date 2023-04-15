/* tc-flare32.h -- Header file for tc-flare32.c.

   Copyright (C) 2023 Free Software Foundation, Inc.
   Contributed by Andrew Clark (FL4SHK)

   This file is part of GAS, the GNU Assembler.

   GAS is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   GAS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with GAS; see the file COPYING.  If not, write to the Free Software
   Foundation, 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.  */

#include "as.h"
#include "safe-ctype.h"
#include "dwarf2dbg.h"
#include "dw2gencfi.h"
#include "opcode/flare32.h"
#include "elf/flare32.h"
#include <stdint.h>
#include <string.h>

/* This array holds the chars that always start a comment.  If the
  pre-processor is disabled, these aren't very useful.  */
const char comment_chars[]        = "//";

/* This array holds the chars that always start a comment at the beginning
 of a line. */
const char line_comment_chars[]   = "//";

/* This array holds machine specific line separator characters. */
const char line_separator_chars[] = "";

/* Chars that can be used to separate mant from exp in floating point nums.  */
const char EXP_CHARS[] = "eE";

/* Chars that mean this number is a floating point constant.
   As in 0f12.456
   or 0d1.2345e12  */
const char FLT_CHARS[] = "rRsSfFdDxXpP";

//static int pending_reloc;
//static htab_t opcode_hash_control;

/* Opcode hash tables, etc. */
//static flare32_opci_list2d_t flare32_opci_list2d;

//static const char flare32_op_name_hset_dummy[] = "dummy";

//static htab_t flare32_op_name_hset = NULL;
//#define flare32_op_name_hset_lookup(key) ((const char *) str_hash_find (flare32_op_name_hset, (key)))

//static htab_t flare32_op_oa_hash = NULL;
//#define flare32_op_oa_lookup(key) ((flare32_opc_info_t *) str_hash_find (flare32_op_oa_hash, (key)))

static htab_t flare32_opci_list_hash = NULL;
#define flare32_opci_list_lookup(key) \
  ((flare32_opci_list_t *) str_hash_find (flare32_opci_list_hash, (key)))

static htab_t flare32_reg_hash = NULL;
#define flare32_reg_lookup(key) \
  ((const flare32_reg_t *) str_hash_find (flare32_reg_hash, (key)))

static flare32_opci_list_t *flare32_opci_list_vec = NULL;
static size_t flare32_opci_list_vec_size = 0;

static void
flare32_opci_list_vec_create (void)
{
  flare32_opci_list_vec = NULL;
  flare32_opci_list_vec_size = 0;
}
static flare32_opci_list_t *
flare32_opci_list_vec_append(flare32_opci_list_t *to_append)
{
  const size_t
    old_size = flare32_opci_list_vec_size;
  ++flare32_opci_list_vec_size;

  flare32_opci_list_vec = (flare32_opci_list_t *)reallocarray
    (flare32_opci_list_vec,
    sizeof (flare32_opci_list_t),
    flare32_opci_list_vec_size);

  return (flare32_opci_list_t *)memcpy
    (flare32_opci_list_vec + old_size,
    to_append,
    sizeof (flare32_opci_list_t));
}
static void
flare32_opci_list_vec_delete (void)
{
  for (size_t i=0; i<flare32_opci_list_vec_size; ++i)
  {
    flare32_opci_list_delete(flare32_opci_list_vec + i);
  }
  free(flare32_opci_list_vec);
}
static void
flare32_opci_list_hnv_append
  (const flare32_opc_info_t *opc_info, unsigned names_ind)
{
  if (opc_info->oparg == FLARE32_OA_BAD)
  {
    return;
  }

  flare32_opci_list_t
    *opci_list = flare32_opci_list_lookup (opc_info->names[names_ind]);

  if (opci_list == NULL)
  {
    opci_list = flare32_opci_list_append
      (flare32_opci_list_create (), opc_info);

    flare32_opci_list_vec_append (opci_list);

    str_hash_insert (flare32_opci_list_hash,
      opc_info->names[names_ind],
      opc_info,
      0);
  }
  else
  {
    flare32_opci_list_append (opci_list, opc_info);
  }

  //return opci_list;
}

//typedef struct flare32_parse_info_t
//{
//  const flare32_opc_info_t
//    *opc_info;
//  const flare32_reg_t
//    *ra,
//    *rb;
//
//} flare32_parse_info_t;

/* The known current alignment of the current section. */
//static int flare32_current_align;
//static segT flare32_current_align_seg;
//
//static int flare32_auto_align_on = 1;

static flare32_temp_t
flare32_enc_temp_insn_lpre_rshift
  (const flare32_opc_info_t *opc_info,
  flare32_temp_t simm)
{
  switch (opc_info->grp_info->grp_value)
  {
    case FLARE32_G1_GRP_VALUE:
    case FLARE32_G5_GRP_VALUE:
    case FLARE32_G6_GRP_VALUE:
      return flare32_enc_temp_insn_g1g5g6_lpre
        (simm >> FLARE32_G1G5G6_S5_BITSIZE);
    case FLARE32_G3_GRP_VALUE:
      return flare32_enc_temp_insn_g3_lpre
        (simm >> FLARE32_G3_S9_BITSIZE);
    default:
      abort ();
      break;
  }
}
static flare32_temp_t
flare32_enc_temp_insn_non_pre_lpre
  (const flare32_opc_info_t *opc_info,
  flare32_temp_t ra_ind,
  flare32_temp_t rb_ind,
  flare32_temp_t simm,
  flare32_temp_t fw)
{
  switch (opc_info->grp_info->grp_value)
  {
    case FLARE32_G1_GRP_VALUE:
      return flare32_enc_temp_insn_g1
        (opc_info->opcode, ra_ind, simm);
    case FLARE32_G2_GRP_VALUE:
      return flare32_enc_temp_insn_g2
        (opc_info->opcode, fw, ra_ind, rb_ind);
    case FLARE32_G3_GRP_VALUE:
      return flare32_enc_temp_insn_g3
        (opc_info->opcode, simm);
    case FLARE32_G4_GRP_VALUE:
      return flare32_enc_temp_insn_g4
        (opc_info->opcode, ra_ind, rb_ind);
    case FLARE32_G5_GRP_VALUE:
      return flare32_enc_temp_insn_g5
        (ra_ind, rb_ind, simm);
    case FLARE32_G6_GRP_VALUE:
      return flare32_enc_temp_insn_g6
        (ra_ind, rb_ind, simm);
    case FLARE32_G7_GRP_VALUE:
      /* For now, there is only one subgroup in group 7 */
      return flare32_enc_temp_insn_g7_aluopbh
        (opc_info->opcode, fw, ra_ind, rb_ind);
    default:
      abort ();
      break;
  }
}
static flare32_temp_t
flare32_enc_temp_insn_index (flare32_temp_t rc_ind)
{
  return flare32_enc_temp_insn_non_pre_lpre
    (flare32_opci_list_lookup ("index")->opc_info, /* opc_info */
    rc_ind, /* ra_ind */
    0, /* rb_ind */
    0, /* simm */
    0 /* fw */);
}
/* -------- */
static bool
flare32_fix_addsy_subsy_handler (fixS *fixP)
{
  if (fixP->fx_addsy && fixP->fx_subsy)
  {
    bfd_reloc_code_real_type
      r_add,
      r_sub;
    switch (fixP->fx_r_type)
    {
      case BFD_RELOC_8:
        r_add = BFD_RELOC_FLARE32_PSEUDO_ADD8;
        r_sub = BFD_RELOC_FLARE32_PSEUDO_SUB8;
        break;
      case BFD_RELOC_16:
        r_add = BFD_RELOC_FLARE32_PSEUDO_ADD16;
        r_sub = BFD_RELOC_FLARE32_PSEUDO_SUB16;
        break;
      case BFD_RELOC_32:
        r_add = BFD_RELOC_FLARE32_PSEUDO_ADD32;
        r_sub = BFD_RELOC_FLARE32_PSEUDO_SUB32;
        break;
      case BFD_RELOC_64:
        r_add = BFD_RELOC_FLARE32_PSEUDO_ADD64;
        r_sub = BFD_RELOC_FLARE32_PSEUDO_SUB64;
        break;

      case BFD_RELOC_FLARE32_G1G5G6_S32:
        r_add = BFD_RELOC_FLARE32_G1G5G6_S32_ADD32;
        r_sub = BFD_RELOC_FLARE32_G1G5G6_S32_SUB32;
        break;
      case BFD_RELOC_FLARE32_G3_S32_PCREL:
        r_add = BFD_RELOC_FLARE32_G3_S32_PCREL_ADD32;
        r_sub = BFD_RELOC_FLARE32_G3_S32_PCREL_SUB32;
        //as_bad (_("can't subtract symbols in PC-relative immediate"));
        //as_bad_subtract (fixP);
        break;
      default:
        abort ();
        break;
    }
    /* Blindly copied from RISC-V's `md_apply_fix()` */
    fixP->fx_next = xmemdup (fixP, sizeof (*fixP), sizeof (*fixP));
    fixP->fx_next->fx_addsy = fixP->fx_subsy;
    fixP->fx_next->fx_subsy = NULL;
    fixP->fx_next->fx_offset = 0;
    fixP->fx_subsy = NULL;
    fixP->fx_r_type = r_add;
    fixP->fx_next->fx_r_type = r_sub;
    return true;
  }
  return false;
}

/* Parse an expression and then restore the input line pointer.  */
static char *
clear_and_parse_exp_save_ilp (char *s, expressionS *ex)
{
  char *save = input_line_pointer;
  (void) memset (ex, 0, sizeof (*ex));

  input_line_pointer = s;
  /* It looks like we can't use `deferred_expression ()`. */
  expression (ex);
  s = input_line_pointer;
  input_line_pointer = save;
  return s;
}

static bool
parse_worker (char **ptr,
              const char *name)
{
  char *s;
  size_t nlen;

  nlen = 0;

  for (s=*ptr;
    *s
      && !is_end_of_line[*s & 0xff]
      && *s != ' '
      && *name != '\0';
    ++s, ++name)
  {
    if (*s != *name)
    {
      return false;
    }
    ++nlen;
  }

  *ptr += nlen;

  return true;
}

static bool
parse_register_operand_worker (char **ptr,
                              const flare32_reg_t *test_reg)
{
  return parse_worker (ptr, test_reg->name);
}

static const flare32_reg_t *
parse_register_operand (char **ptr)
{
  unsigned i;
  for (i=0; i<FLARE32_NUM_GPRS; ++i)
  {
    if (parse_register_operand_worker (ptr, gprs + i))
    {
      return gprs + i;
    }
  }
  for (i=0; i<FLARE32_NUM_SPRS; ++i)
  {
    if (parse_register_operand_worker (ptr, sprs + i))
    {
      return sprs + i;
    }
  }
  {
    if (parse_register_operand_worker (ptr, &reg_pc))
    {
      return &reg_pc;
    }
  }
  return (const flare32_reg_t *) NULL;
}
/* -------- */
/* We don't need `md_pcrel_from` because we do all relaxing in the linker */
long
md_pcrel_from (fixS *fixP)
{
  valueT addr = fixP->fx_where + fixP->fx_frag->fr_address;

/* Offset is from the end of the instruction. */
  switch (fixP->fx_r_type)
  {
    case BFD_RELOC_FLARE32_G1G5G6_S5:
    case BFD_RELOC_FLARE32_G3_S9_PCREL:
      return addr + 2;

    case BFD_RELOC_FLARE32_G1G5G6_S17:
    case BFD_RELOC_FLARE32_G3_S21_PCREL:
      return addr + 4;

    case BFD_RELOC_FLARE32_G1G5G6_S32:
    case BFD_RELOC_FLARE32_G3_S32_PCREL:
      return addr + 6;

    default:
      abort ();
      return addr;
  }
}

//static bool
//relaxable_code_section (asection *sec)
//{
  /* Only relax non-debugging code sections */
  //return ((sec->flags & SEC_DEBUGGING) == 0
  //  && (sec->flags & SEC_CODE) != 0
  //  && (sec->flags & SEC_ALLOC) != 0);
  //return ((sec->flags & SEC_CODE) != 0);
//}

/* Does whatever the xtensa port does.  */
//int
//flare32_validate_fix_sub (fixS *fix)
//{
  //segT add_symbol_segment, sub_symbol_segment;

  /* The difference of two symbols should be resolved by the assembler when
    linkrelax is not set.  If the linker may relax the section containing
    the symbols, then an Xtensa DIFF relocation must be generated so that
    the linker knows to adjust the difference value.  */
  //if (//!linkrelax ||
  //  fix->fx_addsy == NULL)
  //{
  //  return 0;
  //}

  /* Make sure both symbols are in the same segment, and that segment is
     "normal" and relaxable.  If the segment is not "normal", then the
     fix is not valid.  If the segment is not "relaxable", then the fix
     should have been handled earlier.  */
  //add_symbol_segment = S_GET_SEGMENT (fix->fx_addsy);
  //if (!SEG_NORMAL (add_symbol_segment)
  //  || !relaxable_section (add_symbol_segment))
  //{
  //  return 0;
  //}

  //sub_symbol_segment = S_GET_SEGMENT (fix->fx_subsy);
  //return (sub_symbol_segment == add_symbol_segment);
//}

/* If linkrelax is turned on, and the symbol to relocate
  against is in a relaxable segment, don't compute the value -
  generate a relocation instead.  */
//int
//flare32_force_relocation (fixS *fix)
//{
  //if (//linkrelax &&
  //  fix->fx_addsy
  //  && relaxable_section (S_GET_SEGMENT (fix->fx_addsy)))
  //{
  //  return 1;
  //}
  /* We might only need to handle 32-bit relocations here */
  //if (linkrelax
  //  && (
      /* -------- */
      //fix->fx_r_type == BFD_RELOC_FLARE32_G1G5G6_S5
      //|| fix->fx_r_type == BFD_RELOC_FLARE32_G1G5G6_S17
      //|| fix->fx_r_type == BFD_RELOC_FLARE32_G1G5G6_S32
      /* -------- */
      //|| fix->fx_r_type == BFD_RELOC_FLARE32_G3_S9_PCREL
      //|| fix->fx_r_type == BFD_RELOC_FLARE32_G3_S21_PCREL
      //|| fix->fx_r_type == BFD_RELOC_FLARE32_G3_S32_PCREL
      //|| fix->fx_r_type == BFD_RELOC_FLARE32_G3_S32_PCREL_ADD32
      //|| fix->fx_r_type == BFD_RELOC_FLARE32_G3_S32_PCREL_SUB32
      /* -------- */
      //|| fix->fx_r_type == BFD_RELOC_FLARE32_PSEUDO_ADD8
      //|| fix->fx_r_type == BFD_RELOC_FLARE32_PSEUDO_ADD16
      //|| fix->fx_r_type == BFD_RELOC_FLARE32_PSEUDO_ADD32
      //|| fix->fx_r_type == BFD_RELOC_FLARE32_PSEUDO_ADD64
      //|| fix->fx_r_type == BFD_RELOC_FLARE32_PSEUDO_SUB8
      //|| fix->fx_r_type == BFD_RELOC_FLARE32_PSEUDO_SUB16
      //|| fix->fx_r_type == BFD_RELOC_FLARE32_PSEUDO_SUB32
      //|| fix->fx_r_type == BFD_RELOC_FLARE32_PSEUDO_SUB64
      /* -------- */
    //) //&& relaxable_section (S_GET_SEGMENT (fix->fx_addsy))
    //)
  //{
  //  return 1;
  //}

  //return generic_force_reloc (fix);
//}

//bool
//flare32_allow_local_subtract (expressionS *left,
//                              expressionS *right,
//                              segT section)
//{
  /* If we are not in relaxation mode, subtraction is OK.  */
  //if (!linkrelax)
  //{
  //  return true;
  //}

  /* If the symbols are not in a code section then they are OK.  */
  //if ((section->flags & SEC_CODE) == 0)
  //{
  //  return true;
  //}

  //if (left->X_add_symbol == right->X_add_symbol)
  //{
  //  return true;
  //}

  /* We have to assume that there may be instructions between the
    two symbols and that relaxation may increase the distance between
    them.  */
  //return false;
//}

/* We have to define `md_apply_fix`, but as we always do linker relaxing,
  we don't need this function to do anything. */
/* Apply a fixup to the object file.  */
void
md_apply_fix (fixS *fixP ATTRIBUTE_UNUSED,
              valueT *valP ATTRIBUTE_UNUSED,
              segT seg ATTRIBUTE_UNUSED)
{
  /* We do nothing because all relocations are resolved by the linker. */

  //char *buf = fixP->fx_where + fixP->fx_frag->fr_literal;
  //long val = *valP;
  //long newval;
  //long max, min;

  //max = min = 0;

  //switch (fixP->fx_r_type)
  //{
  //case BFD_RELOC_32:
  //  buf[0] = val >> 24;
  //  buf[1] = val >> 16;
  //  buf[2] = val >> 8;
  //  buf[3] = val >> 0;
  //  buf += 4;
  //  break;

  //case BFD_RELOC_16:
  //  buf[0] = val >> 8;
  //  buf[1] = val >> 0;
  //  buf += 2;
  //  break;

  //case BFD_RELOC_8:
  //  *buf++ = val;
  //  break;

  //case BFD_RELOC_FLARE32_G1G5G6_S5:
  //  *buf++ = val & ((1ull << 5ull) - 1ull);
  //  break;
  //case BFD_RELOC_FLARE32_G1G5G6_S17;
  //  buf[0] = (val >> 16ull) & 0x1ull;
  //  buf[1] = val >> 8ull;
  //  buf[2] = val >> 0ull;
  //  buf += 3;
  //  break;

  //case BFD_RELOC_FLARE32_G1G5G6_S32:
  //  buf[0] = val >> 24ull;
  //  buf[1] = val >> 16ull;
  //  buf[2] = val >> 8ull;
  //  buf[3] = val >> 0ull;
  //  buf += 4;
  //  break;

  //case BFD_RELOC_FLARE32_G3_S9_PCREL:
  //  if (!val)
  //  {
  //    break;
  //  }
    /* 9-bit offset even numbered, so we remove right bit. */
  //  //val >>= 1;
  //  break;
  //case BFD_RELOC_FLARE32_G3_S21_PCREL:
  //  if (!val)
  //  {
  //    break;
  //  }
    /* 21-bit offset even numbered, so we remove right bit. */
  //  //val >>= 1;
  //  break;
  //case BFD_RELOC_FLARE32_G3_S32_PCREL:
  //{
  //  reloc_howto_type *howto
  //    = bfd_reloc_type_lookup (stdoutput, fixP->fx_r_type);
  //  //if (!val)
  //  //{
  //  //  break;
  //  //}
    /* 32-bit offset even numbered, so we remove right bit. */
  //  val >>= howto->rightshift;
  //  newval = md_chars_to_number (buf, 4);
  //  newval |= val & FLARE32_G3_
  //}
  //  break;


  //case BFD_RELOC_MOXIE_10_PCREL:
  //  if (!val)
  //  {
  //    break;
  //  }
  //  if (val < -1024 || val > 1022)
  //  {
  //    as_bad_where (fixP->fx_file, fixP->fx_line,
  //                  _("pcrel too far BFD_RELOC_MOXIE_10"));
  //  }
    /* 11 bit offset even numbered, so we remove right bit.  */
  //  val >>= 1;
  //  newval = md_chars_to_number (buf, 2);
  //  newval |= val & 0x03ff;
  //  md_number_to_chars (buf, newval, 2);
  //  break;

  //default:
  //  abort ();
  //  break;
  //}

  //if (max != 0 && (val < min || val > max))
  //{
  //  as_bad_where (fixP->fx_file, fixP->fx_line, _("offset out of range"));
  //}

  //if (fixP->fx_addsy == NULL && fixP->fx_pcrel == 0)
  //{
  //  fixP->fx_done = 1;
  //}
}

//static valueT
//md_chars_to_number (char *buf, int n)
//{
//  valueT ret = 0;
//  unsigned char *where = (unsigned char *) buf;
//
//  while (n--)
//  {
//    ret <<= 8;
//    ret |= (*where++ & 0xff);
//  }
//
//  return ret;
//}

/* This function turns a C long int, short int or char
   into the series of bytes that represent the number
   on the target machine.  */
void
md_number_to_chars (char *buf, valueT val, int n)
{
  //gas_assert (n == 1 || n == 2 || n == 4 || n == 8);
  number_to_chars_bigendian (buf, val, n);
}

/* Turn a string in input_line_pointer into a floating point constant
   of type TYPE, and store the appropriate bytes in *LITP.  The number
   of LITTLENUMS emitted is stored in *SIZEP.  An error message is
   returned, or NULL on OK.  */
const char *
md_atof (int type, char *litP, int *sizeP)
{
  //return ieee_md_atof (type, litP, sizeP, false);
  int prec;
  LITTLENUM_TYPE words[4];
  char *t;
  int i;

  switch (type)
    {
    case 'f':
      prec = 2;
      break;

    case 'd':
      prec = 4;
      break;

    default:
      *sizeP = 0;
      return _("bad call to md_atof");
    }

  t = atof_ieee (input_line_pointer, type, words);
  if (t)
  {
    input_line_pointer = t;
  }

  *sizeP = prec * 2;

  for (i = prec - 1; i >= 0; i--)
  {
    md_number_to_chars (litP, (valueT) words[i], 2);
    litP += 2;
  }

  return NULL;
}

struct option md_longopts[] =
{
  { NULL, no_argument, NULL, 0},
};
size_t md_longopts_size = sizeof (md_longopts);

const char *md_shortopts = "";

int
md_parse_option (int c ATTRIBUTE_UNUSED, const char *arg ATTRIBUTE_UNUSED)
{
  return 1;
}

/* Blank, as the are no machine-dependent options */
void
md_show_usage (FILE *stream ATTRIBUTE_UNUSED)
{
}

void
md_operand (expressionS *op ATTRIBUTE_UNUSED)
{
  /* Empty for now. */
}

/* This function is called once, at assembler startup time.  It sets
   up the hash table with all the opcodes in it, and also initializes
   some aliases for compatibility with other assemblers.  */
void
md_begin (void)
{
  /* -------- */
  unsigned count;
  const flare32_opc_info_t *opc_info;
  const flare32_reg_t *reg;
  //char cbuf[FLARE32_HTAB_KEY_BUF_LIM];
  //flare32_opci_list_t *opci_list;
  /* -------- */
  /* This means we should always do linker relaxing */
  linkrelax = true;
  /* -------- */
  flare32_opci_list_vec_create ();

  flare32_opci_list_hash = str_htab_create ();
  //flare32_op_oa_hash = str_htab_create ();
  flare32_reg_hash = str_htab_create ();
  /* -------- */
  /* Insert `const flare32_opc_info_t *`s.  */
  for (count=0, opc_info=flare32_opc_info_g0;
    count++<FLARE32_G0_OPC_INFO_LIM;
    ++opc_info)
  {
    //str_hash_insert (flare32_op_oa_hash,
    //  flare32_opc_info_to_htab_key
    //    (cbuf, FLARE32_HTAB_KEY_BUF_LIM, opc_info, 0),
    //  opc_info, 0);

    /* Don't insert `pre` and `lpre`, as they should be inserted
      automatically during `md_assemble ()`. */
    if (opc_info->grp_info != &flare32_grp_info_g0_pre
      && opc_info->grp_info != &flare32_grp_info_g0_lpre)
    {
      flare32_opci_list_hnv_append (opc_info, 0);
    }
  }

  for (count=0, opc_info=flare32_opc_info_g1;
    count++<FLARE32_G1_OPC_INFO_LIM;
    ++opc_info)
  {
    flare32_opci_list_hnv_append (opc_info, 0);
  }

  for (count=0, opc_info=flare32_opc_info_g2;
    count++<FLARE32_G2_OPC_INFO_LIM;
    ++opc_info)
  {
    flare32_opci_list_hnv_append (opc_info, 0);
    flare32_opci_list_hnv_append (opc_info, 1);
  }

  for (count=0, opc_info=flare32_opc_info_g3;
    count++<FLARE32_G3_OPC_INFO_LIM;
    ++opc_info)
  {
    flare32_opci_list_hnv_append (opc_info, 0);
  }

  for (count=0, opc_info=flare32_opc_info_g4;
    count++<FLARE32_G4_OPC_INFO_LIM;
    ++opc_info)
  {
    flare32_opci_list_hnv_append (opc_info, 0);
  }

  for (count=0, opc_info=flare32_opc_info_g5;
    count++<FLARE32_G5_OPC_INFO_LIM;
    ++opc_info)
  {
    flare32_opci_list_hnv_append (opc_info, 0);
  }

  for (count=0, opc_info=flare32_opc_info_g6;
    count++<FLARE32_G6_OPC_INFO_LIM;
    ++opc_info)
  {
    flare32_opci_list_hnv_append (opc_info, 0);
  }

  for (count=0, opc_info=flare32_opc_info_g7;
    count++<FLARE32_G7_OPC_INFO_LIM;
    ++opc_info)
  {
    flare32_opci_list_hnv_append (opc_info, 0);
    flare32_opci_list_hnv_append (opc_info, 1);
  }
  /* -------- */
  /* Insert `const flare32_reg_t *`s */
  for (count=0, reg=gprs; count++<FLARE32_NUM_GPRS; ++reg)
  {
    str_hash_insert (flare32_reg_hash, reg->name, reg, 0);
  }

  for (count=0, reg=sprs; count++<FLARE32_NUM_SPRS; ++reg)
  {
    str_hash_insert (flare32_reg_hash, reg->name, reg, 0);
  }

  str_hash_insert (flare32_reg_hash, reg_pc.name, &reg_pc, 0);
  /* -------- */
  //bfd_set_arch_mach (stdoutput, TARGET_ARCH, 0);
  /* -------- */
}
/* Called just before the assembler exits.  */
void
flare32_md_end (void)
{
  flare32_opci_list_vec_delete ();
  htab_delete (flare32_opci_list_hash);
  //htab_delete (flare32_op_oa_hash);
  htab_delete (flare32_reg_hash);
}


//static bool
//flare32_parse_insn (char *s, flare32_parse_info_t *parse_info)
//{
//}

/* This is the guts of the machine-dependent assembler.  STR points to
  a machine dependent instruction.  This function is supposed to emit
  the frags/bytes it assembles to.  */
void
md_assemble (char *str)
{
  char
    *op_start,
    *op_end,
    *op_end_prev,
    *p,
    pend;
  const flare32_opc_info_t
    *opc_info;
  flare32_opci_list_t
    *opci_list;
  flare32_temp_t
    index_insn = 0,
    prefix_insn = 0,
    insn = 0,
    simm = 0,
    fw = 0;
  const flare32_reg_t
    *reg_a,
    *reg_b,
    *reg_c;
  size_t
    nlen = 0,
    nbytes = 0;
  bool
    have_index = false,
    have_lpre = false,
    //have_simm = false,
    is_pcrel = false,
    parse_good = false;
  expressionS
    ex;

  /* Drop leading whitespace. */
  while (*str == ' ')
  {
    ++str;
  }

  /* Find the opcode end. */
  /* Note: this should still work for the ".f" instructions that set
    `flags`. */
  op_start = str;
  for (op_end=str;
    *op_end
      && !is_end_of_line[*op_end & 0xff]
      && *op_end != ' ';
    ++op_end)
  {
    ++nlen;
  }

  pend = *op_end;
  *op_end = '\0';
  op_end_prev = op_end;

  if (nlen == 0)
  {
    as_bad (_("can't find opcode "));
  }

  opci_list = flare32_opci_list_lookup (op_start);
  *op_end = pend;

  if (opci_list == NULL)
  {
    as_bad (_("unknown opcode %s"), op_start);
  }

  for (opc_info=opci_list->opc_info;
    opci_list!=NULL;
    opci_list=opci_list->next, opc_info=opci_list->opc_info)
  {
    op_end = op_end_prev;
    is_pcrel = false;
    reg_a = (const flare32_reg_t *) NULL;
    reg_b = (const flare32_reg_t *) NULL;
    reg_c = (const flare32_reg_t *) NULL;
    simm = 0;
    fw = (flare32_temp_t) (
      (
        opc_info->grp_info == &flare32_grp_info_g2
          && strncmp (opc_info->names[1], op_start,
            FLARE32_OPC_INFO_NAME_MAX_LEN) == 0
      ) || (
        opc_info->grp_info == &flare32_grp_info_g7_aluopbh
        && strncmp (opc_info->names[1], op_start,
          FLARE32_OPC_INFO_NAME_MAX_LEN) == 0
      )
    );

    #define FLARE32_SKIP_ISSPACE() \
      do \
      { \
        while (ISSPACE (*op_end)) \
        { \
          ++op_end; \
        } \
      } while (0)
    #define FLARE32_PARSE_COMMA()  \
      do \
      { \
        if (*op_end != ',') \
        { \
          goto post_oa_switch; \
        } \
        ++op_end; \
        FLARE32_SKIP_ISSPACE (); \
      } while (0)
    #define FLARE32_PARSE_GPR(some_reg) \
      do \
      { \
        some_reg = parse_register_operand (&op_end); \
        if (some_reg == NULL || some_reg->kind == FLARE32_REG_KIND_GPR) \
        { \
          goto post_oa_switch; \
        } \
      } while (0)
    #define FLARE32_PARSE_SPR(some_reg) \
      do \
      { \
        some_reg = parse_register_operand (&op_end); \
        if (some_reg == NULL || some_reg->kind == FLARE32_REG_KIND_SPR) \
        { \
          goto post_oa_switch; \
        } \
      } while (0)
    #define FLARE32_PARSE_NOENC_REG(name) \
      do \
      { \
        if (parse_register_operand (&op_end) \
          != flare32_reg_lookup (name)) \
        { \
          goto post_oa_switch; \
        } \
      } while (0)
    #define FLARE32_PARSE_PC() \
      do \
      { \
        if (parse_register_operand (&op_end) != &reg_pc) \
        { \
          goto post_oa_switch; \
        } \
      } while (0)

    #define FLARE32_PARSE_EXP() \
      do \
      { \
        if (*op_end != '#') \
        { \
          goto post_oa_switch; \
        } \
        op_end = clear_and_parse_exp_save_ilp (op_end, &ex); \
        if (ex.X_op == O_illegal || ex.X_op == O_absent) \
        { \
          goto post_oa_switch; \
        } \
      } while (0)

    switch (opc_info->oparg)
    {
      case FLARE32_OA_NONE:
      {
        FLARE32_SKIP_ISSPACE ();

        parse_good = true;
      }
        break;
      //case FLARE32_OA_PRE:
      //{
      //}
      //  break;
      //case FLARE32_OA_LPRE:
      //{
      //}
      //  break;
      case FLARE32_OA_RA_S5:
      {
        //char *where;
        FLARE32_SKIP_ISSPACE ();

        FLARE32_PARSE_GPR (reg_a);
        FLARE32_PARSE_COMMA ();

        FLARE32_PARSE_EXP ();

        parse_good = true;
        have_lpre = true;
      }
        break;
      case FLARE32_OA_RA_PC_S5:
      {
        FLARE32_SKIP_ISSPACE ();

        FLARE32_PARSE_GPR (reg_a);
        FLARE32_PARSE_COMMA ();

        FLARE32_PARSE_PC ();
        FLARE32_PARSE_COMMA ();

        FLARE32_PARSE_EXP ();

        parse_good = true;
        have_lpre = true;
      }
        break;
      case FLARE32_OA_RA_SP_S5:
      {
        FLARE32_SKIP_ISSPACE ();

        FLARE32_PARSE_GPR (reg_a);
        FLARE32_PARSE_COMMA ();

        FLARE32_PARSE_NOENC_REG ("sp");
        FLARE32_PARSE_COMMA ();

        FLARE32_PARSE_EXP ();

        parse_good = true;
        have_lpre = true;
      }
        break;
      case FLARE32_OA_RA_FP_S5:
      {
        FLARE32_SKIP_ISSPACE ();

        FLARE32_PARSE_GPR (reg_a);
        FLARE32_PARSE_COMMA ();

        FLARE32_PARSE_NOENC_REG ("fp");
        FLARE32_PARSE_COMMA ();

        parse_good = true;
        have_lpre = true;
      }
        break;
      case FLARE32_OA_RA:
      {
        FLARE32_SKIP_ISSPACE ();

        FLARE32_PARSE_GPR (reg_a);

        parse_good = true;
      }
        break;
      case FLARE32_OA_RA_RB:
      {
        FLARE32_SKIP_ISSPACE ();

        FLARE32_PARSE_GPR (reg_a);
        FLARE32_PARSE_COMMA ();

        FLARE32_PARSE_GPR (reg_b);

        parse_good = true;
      }
        break;
      case FLARE32_OA_RA_SP_RB:
      {
        FLARE32_SKIP_ISSPACE ();

        FLARE32_PARSE_GPR (reg_a);
        FLARE32_PARSE_COMMA ();

        FLARE32_PARSE_NOENC_REG ("sp");
        FLARE32_PARSE_COMMA ();

        FLARE32_PARSE_GPR (reg_b);

        parse_good = true;
      }
        break;
      case FLARE32_OA_RA_FP_RB:
      {
        FLARE32_SKIP_ISSPACE ();

        parse_good = true;

        FLARE32_PARSE_GPR (reg_a);
        FLARE32_PARSE_COMMA ();

        FLARE32_PARSE_NOENC_REG ("fp");
        FLARE32_PARSE_COMMA ();

        FLARE32_PARSE_GPR (reg_b);
      }
        break;
      case FLARE32_OA_PCREL_S9:
      {
        FLARE32_SKIP_ISSPACE ();

        FLARE32_PARSE_EXP ();

        parse_good = true;
        is_pcrel = true;
        have_lpre = true;
      }
        break;
      case FLARE32_OA_IRA:
      {
        FLARE32_SKIP_ISSPACE ();

        FLARE32_PARSE_NOENC_REG ("ira");

        parse_good = true;
      }
        break;
      case FLARE32_OA_RA_SB:
      {
        FLARE32_SKIP_ISSPACE ();

        FLARE32_PARSE_GPR (reg_a);
        FLARE32_PARSE_COMMA ();

        FLARE32_PARSE_SPR (reg_b);

        parse_good = true;
      }
        break;
      case FLARE32_OA_SA_RB:
      {
        FLARE32_SKIP_ISSPACE ();

        FLARE32_PARSE_SPR (reg_a);
        FLARE32_PARSE_COMMA ();

        FLARE32_PARSE_GPR (reg_b);

        parse_good = true;
      }
        break;
      case FLARE32_OA_RA_RB_LDST:
      {
        FLARE32_SKIP_ISSPACE ();

        FLARE32_PARSE_GPR (reg_a);
        FLARE32_PARSE_COMMA ();

        if (*op_end != '[')
        {
          break;
        }
        ++op_end;

        FLARE32_PARSE_GPR (reg_b);
        if (*op_end != ']')
        {
          break;
        }
        ++op_end;

        parse_good = true;
      }
        break;
      case FLARE32_OA_RA_RB_RC_LDST:
      {
        FLARE32_SKIP_ISSPACE ();

        FLARE32_PARSE_GPR (reg_a);
        FLARE32_PARSE_COMMA ();

        if (*op_end != '[')
        {
          break;
        }
        ++op_end;

        FLARE32_PARSE_GPR (reg_b);
        FLARE32_PARSE_COMMA ();

        FLARE32_PARSE_GPR (reg_c);

        if (*op_end != ']')
        {
          break;
        }
        ++op_end;

        parse_good = true;
        have_index = true;
      }
        break;
      case FLARE32_OA_RA_RB_S5_LDST:
      {
        FLARE32_SKIP_ISSPACE ();

        FLARE32_PARSE_GPR (reg_a);
        FLARE32_PARSE_COMMA ();

        if (*op_end != '[')
        {
          break;
        }
        ++op_end;

        FLARE32_PARSE_GPR (reg_b);
        FLARE32_PARSE_COMMA ();

        FLARE32_PARSE_EXP ();

        if (*op_end != ']')
        {
          break;
        }
        ++op_end;

        have_lpre = true;
        parse_good = true;
      }
        break;
      case FLARE32_OA_RA_RB_RC_S5_LDST:
      {
        FLARE32_SKIP_ISSPACE ();

        FLARE32_PARSE_GPR (reg_a);
        FLARE32_PARSE_COMMA ();

        if (*op_end != '[')
        {
          break;
        }
        ++op_end;

        FLARE32_PARSE_GPR (reg_b);
        FLARE32_PARSE_COMMA ();

        FLARE32_PARSE_GPR (reg_c);
        FLARE32_PARSE_COMMA ();

        FLARE32_PARSE_EXP ();

        if (*op_end != ']')
        {
          break;
        }
        ++op_end;

        have_index = true;
        have_lpre = true;
        parse_good = true;
      }
        break;
      default:
        abort ();
        break;
    }
    #undef FLARE32_SKIP_ISSPACE
    #undef FLARE32_PARSE_COMMA
    #undef FLARE32_PARSE_GPR
    #undef FLARE32_PARSE_SPR
    #undef FLARE32_PARSE_NOENC_REG
    #undef FLARE32_PARSE_PC
    #undef FLARE32_PARSE_EXP

  post_oa_switch:
    /* This seems to be necessary */
    if (*op_end != '\0')
    {
      parse_good = false;
      continue;
    }

    if (parse_good)
    {
      break;
    }
  }

  if (!parse_good)
  {
    as_bad (_("Invalid arguments for this instruction"));
  }
  /* -------- */
  if (have_index)
  {
    index_insn = flare32_enc_temp_insn_index
      (reg_c != NULL ? reg_c->index : 0x0ull);
    p = frag_more (2);
    md_number_to_chars (p,
      (index_insn & 0xffffull),
      2);
    dwarf2_emit_insn (2);

    /* Per RISC-V: */
    /* We need to start a new frag after any instruction that can be
      optimized away or compressed by the linker during relaxation,
      to prevent the assembler from computing static offsets across such
      an instruction.
      This is necessary to get correct EH info.  */
    frag_wane (frag_now);
    frag_new (0);
  }

  nbytes = 2;
  p = frag_more (2);

  if (have_lpre)
  {
    fixS *fixP;

    nbytes += 4;
    (void) frag_more (4);

    fixP = fix_new_exp (frag_now, /* frag */
                        (p - frag_now->fr_literal), /* where */
                        4, /* size */
                        &ex, /* exp */
                        (int) is_pcrel, /* pcrel */
                        (is_pcrel
                          ? BFD_RELOC_FLARE32_G3_S32_PCREL
                          : BFD_RELOC_FLARE32_G1G5G6_S32) /* r_type */
                        );
    (void) flare32_fix_addsy_subsy_handler (fixP);
    prefix_insn = flare32_enc_temp_insn_lpre_rshift (opc_info, simm);
  }
  insn = flare32_enc_temp_insn_non_pre_lpre
    (opc_info,
    (reg_a != NULL ? reg_a->index : 0x0),
    (reg_b != NULL ? reg_b->index : 0x0),
    simm,
    fw);
  md_number_to_chars (p,
    (((prefix_insn & 0xffffffffull) << FLARE32_ONE_EXT_BITPOS)
      | (insn & 0xffffull)),
    nbytes);
  dwarf2_emit_insn (nbytes);

  /* Per RISC-V: */
  /* We need to start a new frag after any instruction that can be
    optimized away or compressed by the linker during relaxation,
    to prevent the assembler from computing static offsets across such an
    instruction.
    This is necessary to get correct EH info.  */
  if (have_lpre)
  {
    frag_wane (frag_now);
    frag_new (0);
  }

  //if (*op_end != '\0')
  //{
  //  as_warn (_("extra stuff on line ignored"));
  //}

  //if (pending_reloc)
  //{
  //  as_bad (_("Something forgot to clean up\n"));
  //}
}

/* Translate internal representation of relocation info to BFD target
  format.  */
arelent *
tc_gen_reloc (asection *section ATTRIBUTE_UNUSED, fixS *fixp)
{
  arelent *reloc = (arelent *) xmalloc (sizeof (arelent));

  reloc->sym_ptr_ptr = (asymbol **) xmalloc (sizeof (asymbol *));
  *reloc->sym_ptr_ptr = symbol_get_bfdsym (fixp->fx_addsy);
  reloc->address = fixp->fx_frag->fr_address + fixp->fx_where;
  reloc->addend = fixp->fx_offset;

  reloc->howto = bfd_reloc_type_lookup (stdoutput, fixp->fx_r_type);
  if (reloc->howto == NULL)
  {
    as_bad_where (fixp->fx_file, fixp->fx_line,
      _("cannot represent %s relocation in object file"),
      bfd_get_reloc_code_name (fixp->fx_r_type));
    return NULL;
  }

  return reloc;
}

/* Round up section size.  */
//valueT
//md_section_align (asection *seg, valueT addr)
//{
//  int align = bfd_section_alignment (seg);
//  return ((addr + (1 << align) - 1) & (-((valueT) 1 << align)));
//}

/* Machine-dependent assembler directives.
  Format of each entry is:
  { "directive", handler_func, param } */
const pseudo_typeS md_pseudo_table [] =
{
  {"hword", cons, 2},
  /* Flare32 native word size is 4 bytes, so we override
    the GAS default of 2. */
  {"word", cons, 4},
  {"dword", cons, 8},
  {NULL, NULL, 0},
};

void
flare32_cons_fix_new (fragS *frag,
                      int where,
                      unsigned int nbytes,
                      expressionS *ex,
                      const int unused_arg ATTRIBUTE_UNUSED)
{
  bfd_reloc_code_real_type r;
  fixS *fixP;

  switch (nbytes)
  {
    case 1:
      r = BFD_RELOC_8;
      break;
    case 2:
      r = BFD_RELOC_16;
      break;
    case 4:
      r = BFD_RELOC_32;
      break;
    case 8:
      r = BFD_RELOC_64;
      break;
    default:
      as_bad (_("illegal relocation size: %d"), nbytes);
      return;
  }

  fixP = fix_new_exp (frag, where, nbytes, ex, 0, r);
  (void) flare32_fix_addsy_subsy_handler (fixP);
}

int
flare32_regname_to_dw2regnum (char *name)
{
  const flare32_reg_t *reg = flare32_reg_lookup ((const char *)name);

  if (reg != NULL)
  {
    switch (reg->kind)
    {
      case FLARE32_REG_KIND_GPR:
        return reg->index;
      case FLARE32_REG_KIND_SPR:
        return reg->index + FLARE32_NUM_GPRS;
      case FLARE32_REG_KIND_PC:
        return reg->index + FLARE32_NUM_GPRS + FLARE32_NUM_SPRS;
      default:
        return -1;
    }
  }
  return -1;
}
void
flare32_frame_initial_instructions (void)
{
}

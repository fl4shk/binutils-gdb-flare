/* tc-snowhousecpu.h -- Header file for tc-snowhousecpu.c.

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

#include "../as.h"
#include "subsegs.h"
#include "safe-ctype.h"
#include "dwarf2dbg.h"
#include "dw2gencfi.h"
#include "opcode/snowhousecpu.h"
#include "../../include/elf/snowhousecpu.h"
#include "../../include/opcode/snowhousecpu.h"
#include "../../include/opcode/snowhousecpu-opc-decls.h"
#include "../../include/opcode/snowhousecpu-dasm-info-funcs.h"
#include "../../include/opcode/snowhousecpu-relax-reloc-lookup.h"
#include <stdint.h>
#include <string.h>

/* This array holds the chars that always start a comment.  If the
  pre-processor is disabled, these aren't very useful.  */
const char comment_chars[]        = "//";

/* This array holds the chars that always start a comment at the beginning
 of a line. */
const char line_comment_chars[]   = "#";

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

//static htab_t snowhousecpu_opci_list_hash = NULL;
//#define snowhousecpu_opci_list_lookup(key)
//  ((snowhousecpu_opci_list_t *) str_hash_find (snowhousecpu_opci_list_hash, (key)))

//static htab_t snowhousecpu_opci_list_end_hash = NULL;
//#define snowhousecpu_opci_list_end_lookup(key)
//  ((snowhousecpu_opci_list_t *)
//    str_hash_find (snowhousecpu_opci_list_end_hash, (key)))
static htab_t snowhousecpu_opci_v2d_index_hash = NULL;
#define snowhousecpu_opci_v2d_index_lookup(key) \
  ((size_t *) str_hash_find (snowhousecpu_opci_v2d_index_hash, (key)))
static size_t snowhousecpu_opci_v2d_index_arr[SNOWHOUSECPU_MAX_UNIQUE_OPCI_NAMES];

static snowhousecpu_opci_v2d_t snowhousecpu_opci_v2d = { NULL, 0};

//typedef struct snowhousecpu_opci_vec_lookup_ret_t
//{
//  snowhousecpu_opci_vec_t *opci_vec;
//  size_t *v2d_index;
//} snowhousecpu_opci_vec_lookup_ret_t;

static inline snowhousecpu_opci_vec_t *
snowhousecpu_opci_vec_lookup (const char *key)
{
  snowhousecpu_opci_vec_t *ret;
  size_t *v2d_index;

  ret = NULL;
  v2d_index = snowhousecpu_opci_v2d_index_lookup (key);

  if (v2d_index != NULL)
  {
    ret = snowhousecpu_opci_v2d.data + (*v2d_index);
  }
  return ret;
}

static htab_t snowhousecpu_reg_hash = NULL;
#define snowhousecpu_reg_lookup(key) \
  ((const snowhousecpu_reg_t *) str_hash_find (snowhousecpu_reg_hash, (key)))

static unsigned
have_pre_insn_length (snowhousecpu_have_pre_t have_plp)
{
  return snowhousecpu_have_pre_insn_length (have_plp);
}
static unsigned
have_pre_distance (snowhousecpu_have_pre_t from, snowhousecpu_have_pre_t to)
{
  return snowhousecpu_have_pre_distance (from, to);
}

#include <stdio.h>
static void
snowhousecpu_insn_number_to_chars (char *p, snowhousecpu_temp_t val, int n)
{
  //for (int i = (int)(n / 2) - 1; i >= 0; --i)
  //{
  //  snowhousecpu_number_to_chars (p, (val >> (i * 16)) & 0xffffu, 2);
  //}
  //printf(
  //  "snowhousecpu_insn_number_to_chars: %lx; %x\n", 
  //  val,
  //  n
  //);
  switch (n)
  {
    case 4:
      snowhousecpu_number_to_chars (p, val & 0xffffffffull, 4);
      break;
    case 8:
      snowhousecpu_number_to_chars (p, (val >> 32) & 0xffffffffull, 4);
      snowhousecpu_number_to_chars (p + 4, val & 0xffffffffull, 4);
      break;
    default:
      abort ();
      break;
  }
  ////gas_assert (n == 2 || n == 4 || n == 6);
  //for (int i = 0; i < n / 2; ++i)
  ////for (int i = (int)(n / 2) - 1; i >= 0; --i)
  //{
  //  const unsigned to_print = (val >> (i * 16)) & 0xffffu;
  //  const size_t plus = n - (i * 2) - 2; //(n / 2) - (i * 2) + 1;
  //  char *mod_p = p + plus;
  //  //const uintptr_t mod_p_u = (uintptr_t)mod_p;
  //  //printf (
  //  //  "%x; %lu; %lx\n",
  //  //  to_print,
  //  //  plus,
  //  //  mod_p_u
  //  //);
  //  //printf (
  //  //  "%x\n",
  //  //  to_print
  //  //);
  //  snowhousecpu_number_to_chars (mod_p, to_print, 2);
  //  //switch (n)
  //  //{
  //  //  case 2:
  //  //  {
  //  //  }
  //  //    break;
  //  //  case 4:
  //  //  {
  //  //  }
  //  //    break;
  //  //  case 6:
  //  //  {
  //  //  }
  //  //    break;
  //  //}
  //}
  //printf("\n");
}

typedef struct snowhousecpu_relax_insn_t
{
  //snowhousecpu_cl_insn_t
  //  *cl_insn;
  snowhousecpu_temp_t
    prefix_insn_bitsize,
    insn_bitsize,
    curr_bitsize,
    target_bitsize;
    //op_value;
    //grp_value;
  //const snowhousecpu_grp_info_t
  //  *grp_info;
  //snowhousecpu_oparg_t
  //  oparg;
  snowhousecpu_dasm_info_t
    dasm_info;
  //const snowhousecpu_opc_info_t
  //  *opc_info;
  bool
    is_pcrel: 1,
    //was_lpre: 1,
    have_imm: 1;
    //imm_unsigned: 1;
  //unsigned
  //  length;
} snowhousecpu_relax_insn_t;
typedef struct snowhousecpu_cl_insn_t
{
  snowhousecpu_relax_insn_t
    relax_insn;

  //bool no_relax: 1;
  snowhousecpu_have_pre_t have_pre;

  /* The actual data of the encoded instruction. */
  snowhousecpu_temp_t data;

  bfd_reloc_code_real_type reloc;
  bool
    no_relax: 1;
  //  is_small_imm_unsigned: 1,
  //  is_g7_icreload: 1;

  const snowhousecpu_opc_info_t
    *opc_info;
} snowhousecpu_cl_insn_t;
static bool
snowhousecpu_cl_insn_no_relax (const snowhousecpu_cl_insn_t *cl_insn)
{
  //return (cl_insn->reloc == BFD_RELOC_SNOWHOUSECPU_G1_S32_NO_RELAX
  //  || cl_insn->reloc == BFD_RELOC_SNOWHOUSECPU_G3_S32_PCREL_NO_RELAX);
  return cl_insn->no_relax;
  //return false;
}

typedef struct snowhousecpu_relax_temp_t
{
  //snowhousecpu_relax_insn_t
  //  relax_insn;
  snowhousecpu_cl_insn_t
    *cl_insn;
  offsetT
    value;
  bool
    rm_prefix: 1;
  unsigned
    length;
} snowhousecpu_relax_temp_t;

static bool
have_relaxable_temp_insn (fragS *fragP);
static bool
relax_can_shrink_value (bfd_vma value,
  snowhousecpu_temp_t curr_bitsize, snowhousecpu_temp_t target_bitsize//,
  /*bool have_small_imm_unsigned_target*/);
static void
snowhousecpu_relax_insn_ctor (snowhousecpu_relax_insn_t *self,
  snowhousecpu_cl_insn_t *cl_insn);
static void
snowhousecpu_relax_temp_ctor (snowhousecpu_relax_temp_t *self,
  fragS *fragP, asection *sec, bool update);
static unsigned
relaxed_cl_insn_length (fragS *fragP, asection *sec, bool update)
{
  snowhousecpu_relax_temp_t relax_temp;
  snowhousecpu_relax_temp_ctor (&relax_temp, fragP, sec, update);
  return relax_temp.length;
}

/* We shouldn't need to keep around references to elements of 
  `snowhousecpu_cl_insn_vec` after relaxing a single instruction,
  so let's use a dynamic array with better cache behavior than,
  and fewer dynamic allocation calls than,
  `snowhousecpu_opci_list_vec` */
static snowhousecpu_cl_insn_t *snowhousecpu_cl_insn_vec = NULL;
static relax_substateT
  snowhousecpu_cl_insn_vec_old_size = 0,
  snowhousecpu_cl_insn_vec_size = 0;

//static void
//snowhousecpu_print_hash_opci_list (const char *key)
//{
//  for (snowhousecpu_opci_list_t *opci_list=snowhousecpu_opci_list_lookup (key);
//    opci_list!=NULL;
//    opci_list=opci_list->next)
//  {
//    printf ("snowhousecpu_print_hash_opci_list (): {%s, %s} ",
//      opci_list->opc_info->names[0],
//      opci_list->opc_info->names[1]);
//    switch (opci_list->opc_info->oparg)
//    {
//      case SNOWHOUSECPU_OA_BAD:
//        printf ("SNOWHOUSECPU_OA_BAD\n");
//        break;
//      case SNOWHOUSECPU_OA_NONE:
//        printf ("SNOWHOUSECPU_OA_NONE\n");
//        break;
//      case SNOWHOUSECPU_OA_PRE:
//        printf ("SNOWHOUSECPU_OA_PRE\n");
//        break;
//      case SNOWHOUSECPU_OA_LPRE:
//        printf ("SNOWHOUSECPU_OA_LPRE\n");
//        break;
//      case SNOWHOUSECPU_OA_RA_S5:
//        printf ("SNOWHOUSECPU_OA_RA_S5\n");
//        break;
//      case SNOWHOUSECPU_OA_RA_PC_S5:
//        printf ("SNOWHOUSECPU_OA_RA_PC_S5\n");
//        break;
//      case SNOWHOUSECPU_OA_RA_SP_S5:
//        printf ("SNOWHOUSECPU_OA_RA_SP_S5\n");
//        break;
//      case SNOWHOUSECPU_OA_RA_FP_S5:
//        printf ("SNOWHOUSECPU_OA_RA_FP_S5\n");
//        break;
//      //case SNOWHOUSECPU_OA_S5:
//      //  printf ("SNOWHOUSECPU_OA_S5\n");
//      //  break;
//      case SNOWHOUSECPU_OA_RA:
//        printf ("SNOWHOUSECPU_OA_RA\n");
//        break;
//      case SNOWHOUSECPU_OA_RA_RB:
//        printf ("SNOWHOUSECPU_OA_RA_RB\n");
//        break;
//      case SNOWHOUSECPU_OA_RA_SP_RB:
//        printf ("SNOWHOUSECPU_OA_RA_SP_RB\n");
//        break;
//      case SNOWHOUSECPU_OA_RA_FP_RB:
//        printf ("SNOWHOUSECPU_OA_RA_FP_RB\n");
//        break;
//      case SNOWHOUSECPU_OA_PCREL_S9:
//        printf ("SNOWHOUSECPU_OA_PCREL_S9\n");
//        break;
//      case SNOWHOUSECPU_OA_PCREL_S32_NO_RELAX:
//        printf ("SNOWHOUSECPU_OA_PCREL_S32_NO_RELAX\n");
//        break;
//      case SNOWHOUSECPU_OA_IRA:
//        printf ("SNOWHOUSECPU_OA_IRA\n");
//        break;
//      case SNOWHOUSECPU_OA_RA_SB:
//        printf ("SNOWHOUSECPU_OA_RA_SB\n");
//        break;
//      case SNOWHOUSECPU_OA_SA_RB:
//        printf ("SNOWHOUSECPU_OA_SA_RB\n");
//        break;
//      case SNOWHOUSECPU_OA_SA_SB:
//        printf ("SNOWHOUSECPU_OA_SA_SB\n");
//        break;
//      case SNOWHOUSECPU_OA_RA_RB_LDST:
//        printf ("SNOWHOUSECPU_OA_RA_RB_LDST\n");
//        break;
//      case SNOWHOUSECPU_OA_RA_RB_RC_LDST:
//        printf ("SNOWHOUSECPU_OA_RA_RB_RC_LDST\n");
//        break;
//      case SNOWHOUSECPU_OA_RA_RB_S5_LDST:
//        printf ("SNOWHOUSECPU_OA_RA_RB_S5_LDST\n");
//        break;
//      case SNOWHOUSECPU_OA_RA_RB_RC_S5_LDST:
//        printf ("SNOWHOUSECPU_OA_RA_RB_RC_S5_LDST\n");
//        break;
//      default:
//        printf ("unknown oparg\n");
//        break;
//    }
//  }
//}

static void
snowhousecpu_opci_v2d_and_index_hash_append
  (const snowhousecpu_opc_info_t *opc_info/*,*/ /*bool no_relax,*/ /*unsigned names_ind*/)
{
  const char *temp_name
    = opc_info->name;
    //!no_relax
    //  ? opc_info->names[names_ind]
    //  : opc_info->nr_names[names_ind];
  snowhousecpu_opci_vec_t *opci_vec_ptr;

  if (opc_info->oparg == SNOWHOUSECPU_OA_BAD)
  {
    return;
  }

  opci_vec_ptr = snowhousecpu_opci_vec_lookup (temp_name);

  if (opci_vec_ptr == NULL)
  {
    snowhousecpu_opci_vec_t opci_vec;
    snowhousecpu_opci_vec_create (&opci_vec);
    snowhousecpu_opci_vec_append (&opci_vec, opc_info);

    str_hash_insert (snowhousecpu_opci_v2d_index_hash,
      temp_name,
      snowhousecpu_opci_v2d_index_arr + snowhousecpu_opci_v2d.size,
      0);
    snowhousecpu_opci_v2d_append (&snowhousecpu_opci_v2d, &opci_vec);
  }
  else /* if (opci_vec != NULL) */
  {
    snowhousecpu_opci_vec_append (opci_vec_ptr, opc_info);
  }
}

static void
snowhousecpu_cl_insn_vec_create (void)
{
  snowhousecpu_cl_insn_vec = NULL;
  snowhousecpu_cl_insn_vec_old_size = 0;
  snowhousecpu_cl_insn_vec_size = 0;
}
static snowhousecpu_cl_insn_t *
snowhousecpu_cl_insn_vec_append (const snowhousecpu_cl_insn_t *to_append)
{
  //const size_t
  //  old_size = snowhousecpu_cl_insn_vec_size;
  snowhousecpu_cl_insn_vec_old_size = snowhousecpu_cl_insn_vec_size;
  ++snowhousecpu_cl_insn_vec_size;

  snowhousecpu_cl_insn_vec = (snowhousecpu_cl_insn_t *)realloc
    (snowhousecpu_cl_insn_vec,
    sizeof (snowhousecpu_cl_insn_t) * snowhousecpu_cl_insn_vec_size);
  //snowhousecpu_cl_insn_vec[snowhousecpu_cl_insn_vec_old_size ] = to_append;
  (void) memcpy (snowhousecpu_cl_insn_vec + snowhousecpu_cl_insn_vec_old_size,
    to_append,
    sizeof (snowhousecpu_cl_insn_t));
  //fprintf (stderr,
  //  "snowhousecpu_cl_insn_vec_append (): %u\n",
  //  snowhousecpu_cl_insn_vec_size);

  //return (snowhousecpu_cl_insn_t *)memcpy
  //  (snowhousecpu_cl_insn_vec + snowhousecpu_cl_insn_vec_old_size,
  //  to_append,
  //  sizeof (snowhousecpu_cl_insn_t));
  return snowhousecpu_cl_insn_vec + snowhousecpu_cl_insn_vec_old_size;
}
static void
snowhousecpu_cl_insn_vec_delete (void)
{
  free (snowhousecpu_cl_insn_vec);
}

//typedef enum snowhousecpu_have_index_2reg_kind_t {
//  SNOWHOUSECPU_HIDX2R_KIND_NO_INDEX,
//  SNOWHOUSECPU_HIDX2R_KIND_ATOMIC,
//  SNOWHOUSECPU_HIDX2R_KIND_LDST,
//  //SNOWHOUSECPU_HIDX2R_KIND_DIVMOD,
//  //SNOWHOUSECPU_HIDX2R_KIND_LMUL,
//  //SNOWHOUSECPU_HIDX2R_KIND_DIVMOD64,
//  SNOWHOUSECPU_HIDX2R_KIND_JUSTADDR,
//} snowhousecpu_have_index_2reg_kind_t;

typedef struct snowhousecpu_parse_data_t
{
  const snowhousecpu_opc_info_t
    *opc_info;
  snowhousecpu_temp_t
    //index_insn,
    prefix_insn,
    insn,
    simm,
    fwl;
  const snowhousecpu_reg_t
    *reg_a,
    *reg_b,
    *reg_c;
    //*reg_d;
  //size_t
  //  nlen;
  //  //nbytes;
  //snowhousecpu_have_index_2reg_kind_t have_index_2reg_kind;
  bool
    //have_index : 1,
    //have_index_ra_simm: 1,
    have_imm : 1,
    //is_small_imm_unsigned: 1,
    //is_g7_icreload: 1,
    //have_cpy64 : 1,
    //have_simm : 1,
    is_pcrel : 1,
    //no_relax : 1,
    parse_good : 1;
  expressionS
    ex,
    ex_1;
  //snowhousecpu_const_dbl_t
  //  const_dbl;
} snowhousecpu_parse_data_t;

/* Idea borrowed from "tc-riscv.c": `install_insn ()` */
//static void
//install_cl_insn (const snowhousecpu_cl_insn_t *cl_insn)
//{
//  char *p = cl_insn->fragP->fr_literal + cl_insn->where;
//  snowhousecpu_number_to_chars (p, cl_insn->data,
//    have_plp_insn_length (cl_insn->have_plp));
//}
/* Idea borrowed from "tc-riscv.c": `move_insn ()` */
static void
move_cl_insn (
  //snowhousecpu_cl_insn_t *cl_insn,
  const snowhousecpu_cl_insn_t *cl_insn,
  fragS *fragP, long where)
{
  char *p = fragP->fr_literal + where;
  snowhousecpu_insn_number_to_chars(p, cl_insn->data,
    have_pre_insn_length (cl_insn->have_pre));
}

/* Idea borrowed from "tc-riscv.c": `add_fixed_insn ()` */
static void
add_fixed_size_cl_insn (const snowhousecpu_cl_insn_t *cl_insn)
{
  const unsigned length = have_pre_insn_length (cl_insn->have_pre);
  //printf
  //  ("add_fixed_size_cl_insn: %u %lx\n",
  //  length,
  //  cl_insn->data);
  char *p = frag_more (length);
  move_cl_insn (cl_insn, frag_now, p - frag_now->fr_literal);
}
static void
add_fne_cl_insn (const snowhousecpu_cl_insn_t *cl_insn,
                expressionS *address_expr)
{
  //if (snowhousecpu_cl_insn_no_relax (cl_insn))
  //{
  //  fprintf (stderr,
  //    "no_relax! %lx\n",
  //    cl_insn->data);
  //}
  char *p;
  //fixS *fixP;
  reloc_howto_type *howto;

  const unsigned
    nbytes = have_pre_insn_length (cl_insn->have_pre);

  p = frag_more (nbytes);

  howto = bfd_reloc_type_lookup (stdoutput, cl_insn->reloc);
  //printf (
  //  "add_fne_cle_insn(): %s\n", howto->name
  //);
  if (howto == NULL)
  {
    as_bad (_("internal: unsupported SnowHouseCpu relocation number %d"),
      cl_insn->reloc);
  }

  //fixP = 
  fix_new_exp
    (frag_now, /* frag */
    (p - frag_now->fr_literal), /* where */
    bfd_get_reloc_size (howto), /* size */
    address_expr, /* exp */
    (int) cl_insn->relax_insn.is_pcrel, /* pcrel */
    cl_insn->reloc /* reloc */
    );

  // TODO: verify whether `nbytes` should be used as the argument.
  // As of this writing, RISC-V always uses `0` as the argument to
  // `dwarf2_emit_insn ()`, so maybe that's what I need too?
  //dwarf2_emit_insn (nbytes);
  dwarf2_emit_insn (0);
  snowhousecpu_insn_number_to_chars (p, cl_insn->data, nbytes);

  /* Per RISC-V: */
  /* We need to start a new frag after any instruction that can be
    optimized away or compressed by the linker during relaxation, to
    prevent the assembler from computing static offsets across such an
    instruction.
    This is necessary to get correct EH info.  */
  /* TODO: verify whether we need to do this for the outer `else`
    block of `append_cl_insn ()`.
    Since there is no relaxation for instructions without immediates,
    I don't think it will be necessary */
  //if (!no_relax
  //  && cl_insn->relax_insn.have_imm
  //  && cl_insn->have_plp != SNOWHOUSECPU_HAVE_PRE_NONE)
  if (!snowhousecpu_cl_insn_no_relax (cl_insn))
  {
    frag_wane (frag_now);
    frag_new (0);
  }
  //else
}

/* Idea borrowed from "tc-riscv.c": `add_relaxed_insn ()` */
static void
add_gas_relaxed_cl_insn
  (const snowhousecpu_cl_insn_t *cl_insn,
  int max_chars, int var,
  //relax_substateT subtype,
  symbolS *symbol, offsetT offset)
{
  frag_grow (max_chars);
  move_cl_insn (cl_insn, frag_now, frag_more (0) - frag_now->fr_literal);

  snowhousecpu_cl_insn_vec_append (cl_insn);
  //fprintf (stderr,
  //  "add_gas_relaxed_cl_insn (): %u; %lx\n",
  //  snowhousecpu_cl_insn_vec_old_size,
  //  cl_insn->data);

  frag_var (rs_machine_dependent, max_chars, var,
    snowhousecpu_cl_insn_vec_old_size, //subtype,
    symbol, offset, NULL);
}
                  
/* Idea borrowed from "tc-riscv.c": `append_insn ()` */
static void
append_cl_insn (snowhousecpu_cl_insn_t *cl_insn,
                expressionS *address_expr
                //bfd_reloc_code_real_type reloc
                )
{
  //snowhousecpu_relax_temp_t
  //  relax_temp;
  snowhousecpu_relax_insn_t *relax_insn;
  bool no_relax;
  //fprintf (
  //  stderr,
  //  "append_cl_insn(): %u %u; %lx\n", 
  //  cl_insn != NULL,
  //  cl_insn->opc_info != NULL,
  //  cl_insn != NULL ? cl_insn->data : (snowhousecpu_temp_t)(-1ll)
  //);
  relax_insn = &cl_insn->relax_insn;
  //fprintf (
  //  stderr,
  //  "append_cl_insn(): %lx %u %s %u; %u %u; %u\n",
  //  cl_insn->data,
  //  cl_insn->no_relax,
  //  cl_insn->opc_info != NULL ? cl_insn->opc_info->nr_names[0] : "<none>",
  //  (
  //    cl_insn->opc_info != NULL
  //    ? cl_insn->opc_info->oparg == SNOWHOUSECPU_OA_RA_S5
  //    : 2u
  //  ),
  //  relax_insn->was_lpre,
  //  relax_insn->have_imm,
  //  cl_insn->reloc == BFD_RELOC_SNOWHOUSECPU_G1_S32_NO_RELAX
  //);

  no_relax = snowhousecpu_cl_insn_no_relax (cl_insn);

  if (relax_insn->have_imm)
  {
    gas_assert (address_expr != NULL);

    /* blindly copied from "tc-riscv.c": `append_insn ()` */ 
    if (now_seg == absolute_section)
    {
      as_bad (_("relaxable immediates not supported in "
        "absolute section"));
      return;
    }

    if (
      //snowhousecpu_opt.gasrelax
      //&&
      !no_relax
      && !(
        address_expr->X_op == O_subtract
        && address_expr->X_op_symbol != NULL
      )
    )
    {
      /* Due to a limitation in `frag_var ()`,
        we can only do relaxing in GAS if, when we have a symbol,
        we have an expression of the form "symbol + constant" */
      const int
        best_case = have_pre_insn_length (SNOWHOUSECPU_HAVE_PRE_NONE),
        worst_case = have_pre_insn_length (SNOWHOUSECPU_HAVE_PRE_PRE);

      dwarf2_emit_insn (0);
      add_gas_relaxed_cl_insn
        (cl_insn,
        worst_case, best_case,
        address_expr->X_add_symbol,
        address_expr->X_add_number);
    }
    else
    {
      add_fne_cl_insn (cl_insn, address_expr);
    }
  }
  else
  {
    dwarf2_emit_insn (0);
    add_fixed_size_cl_insn (cl_insn);
  }
}

//typedef struct snowhousecpu_parse_info_t
//{
//  const snowhousecpu_opc_info_t
//    *opc_info;
//  const snowhousecpu_reg_t
//    *ra,
//    *rb;
//
//} snowhousecpu_parse_info_t;

/* The known current alignment of the current section. */
//static int snowhousecpu_current_align;
//static segT snowhousecpu_current_align_seg;
//
//static int snowhousecpu_auto_align_on = 1;

//volatile int asdf = 0;
static snowhousecpu_temp_t
snowhousecpu_enc_temp_insn_pre_rshift
  (const snowhousecpu_opc_info_t *opc_info,
  snowhousecpu_temp_t simm)
{
  //printf ("snowhousecpu_enc_temp_insn_lpre_rshift(): %i\n",
  //  (int) opc_info->grp_info->grp_value);
  const snowhousecpu_temp_t ret = snowhousecpu_enc_temp_insn (
                //snowhousecpu_opc_info_pre_simm16.op,            // op
                opc_info->op,
                0x0,                                                                                                                                            // ra_idx
                0x0,                                                                                                                                            // rb_idx
                0x0,                                                                                                                                            // rc_idx
                simm >> SNOWHOUSECPU_IMM16_BITSIZE                      // imm16
  );
  return ret;
}
//static snowhousecpu_temp_t
//snowhousecpu_enc_temp_insn_pre_rshift
//  (const snowhousecpu_opc_info_t *opc_info,
//  snowhousecpu_temp_t simm)
//{
//  //printf ("snowhousecpu_enc_temp_insn_pre_rshift(): %i\n",
//  //  (int) opc_info->grp_info->grp_value);
//  switch (opc_info->grp_info->grp_value)
//  {
//    case SNOWHOUSECPU_G1_GRP_VALUE:
//    case SNOWHOUSECPU_G5_GRP_VALUE:
//    case SNOWHOUSECPU_G6_GRP_VALUE:
//      return snowhousecpu_enc_temp_insn_pre
//        (simm >> SNOWHOUSECPU_G1_I5_BITSIZE);
//    case SNOWHOUSECPU_G3_GRP_VALUE:
//      return snowhousecpu_enc_temp_insn_pre
//        (simm >> SNOWHOUSECPU_G3_S9_BITSIZE);
//    default:
//      abort ();
//      break;
//  }
//}
//static snowhousecpu_temp_t
//snowhousecpu_enc_temp_insn_non_pre
//  (const snowhousecpu_opc_info_t *opc_info,
//  snowhousecpu_temp_t ra_idx,
//  snowhousecpu_temp_t rb_idx,
//  snowhousecpu_temp_t rc_idx,
//  snowhousecpu_temp_t simm)
//{
//  switch (opc_info->subop.kind)
//  {
//    case SNOWHOUSECPU_SOK_NONE:
//    {
//      return snowhousecpu_enc_temp_insn (
//        opc_info->op,
//        ra_idx,
//        rb_idx,
//        rc_idx,
//        simm
//      );
//    }
//      break;
//    case SNOWHOUSECPU_SOK_RC_IDX_FULL:
//    {
//      //--------
//      return snowhousecpu_enc_temp_insn (
//        opc_info->op,
//        ra_idx,
//        rb_idx,
//        opc_info->subop.val, //rc_idx,
//        simm
//      );
//      //--------
//      //if (rc_idx == opc_info->subop.val)
//      //{
//      //}
//      //else
//      //{
//      //}
//      //--------
//    }
//      break;
//    case SNOWHOUSECPU_SOK_RC_IDX_NZ:
//    {
//      //return snowhousecpu_enc_temp_insn (
//      //  opc_info->op
//      //);
//      //if (
//      //  opc_info->
//      //)
//      //{
//      //}
//    }
//      break;
//    case SNOWHOUSECPU_SOK_IMM16_LO:
//    {
//    }
//      break;
//  }
//  //switch (opc_info->grp_info->grp_value)
//  //{
//  //  case SNOWHOUSECPU_G0_GRP_VALUE:
//  //    return snowhousecpu_enc_temp_insn_g0_atomic
//  //      (ra_ind, rb_ind, fwl);
//  //  case SNOWHOUSECPU_G1_GRP_VALUE:
//  //    return snowhousecpu_enc_temp_insn_g1
//  //      (opc_info->opcode, ra_ind, simm);
//  //  case SNOWHOUSECPU_G2_GRP_VALUE:
//  //    return snowhousecpu_enc_temp_insn_g2
//  //      (opc_info->opcode, fwl, ra_ind, rb_ind);
//  //  case SNOWHOUSECPU_G3_GRP_VALUE:
//  //    return snowhousecpu_enc_temp_insn_g3
//  //      (opc_info->opcode, simm);
//  //  case SNOWHOUSECPU_G4_GRP_VALUE:
//  //  {
//  //    snowhousecpu_temp_t ret = snowhousecpu_enc_temp_insn_g4
//  //      (opc_info->opcode, ra_ind, rb_ind);
//  //    //printf ("snowhousecpu_enc_temp_insn_non_pre_lpre: %s %d %d %d; 0x%x; "
//  //    //  "\n",
//  //    //  opc_info->names[0], (unsigned) opc_info->oparg,
//  //    //  (unsigned) ra_ind, (unsigned) rb_ind,
//  //    //  (unsigned) ret);
//  //    return ret;
//  //  }
//  //  case SNOWHOUSECPU_G5_GRP_VALUE:
//  //  {
//  //    if (opc_info->grp_info->subgrp == &snowhousecpu_enc_info_g5_index_ra_rb_subgrp)
//  //    {
//      //return snowhousecpu_enc_temp_insn_g5_index_ra_rb
//      //  (ra_ind, rb_ind);
//  //    }
//  //    else if (opc_info->grp_info->subgrp == &snowhousecpu_enc_info_g5_index_ra_simm_subgrp)
//  //    {
//      //return snowhousecpu_enc_temp_insn_g5_index_ra_simm
//      //  (ra_ind, simm);
//  //    }
//  //    else
//  //    {
//      //abort();
//      //break;
//  //    }
//  //  }
//  //  //case SNOWHOUSECPU_G6_GRP_VALUE:
//  //  //  return snowhousecpu_enc_temp_insn_g6
//  //  //    (ra_ind, rb_ind, simm);
//  //  case SNOWHOUSECPU_G7_GRP_VALUE:
//  //    if (opc_info->grp_info->subgrp
//  //      == &snowhousecpu_enc_info_g7_aluopbh_subgrp)
//  //    {
//  //      return snowhousecpu_enc_temp_insn_g7_aluopbh
//  //        (opc_info->opcode, fwl, ra_ind, rb_ind);
//  //    }
//  //    else if (opc_info->grp_info->subgrp
//  //      == &snowhousecpu_enc_info_g7_sprldst_subgrp)
//  //    {
//  //      return snowhousecpu_enc_temp_insn_g7_sprldst
//  //        (opc_info->opcode, ra_ind, rb_ind);
//  //    }
//  //    else if (opc_info->grp_info->subgrp
//  //      == &snowhousecpu_enc_info_g7_icreload_subgrp)
//  //    {
//  //      return snowhousecpu_enc_temp_insn_g7_icreload
//  //        (ra_ind, simm);
//  //    }
//  //    else if (opc_info->grp_info->subgrp
//  //      == &snowhousecpu_enc_info_g7_icflush_subgrp)
//  //    {
//  //      return snowhousecpu_enc_temp_insn_g7_icflush ();
//  //    }
//  //    else
//  //    {
//  //      abort ();
//  //      break;
//  //    }
//  //  default:
//  //    abort ();
//  //    break;
//  //}
//}
//static snowhousecpu_temp_t
//snowhousecpu_enc_temp_insn_index_ra_rb (snowhousecpu_temp_t ra_ind, snowhousecpu_temp_t rb_ind)
//{
//  const snowhousecpu_opc_info_t
//    *opc_info;
//
//  /* The below line doesn't work because we are no longer storing this 
//    `opc_info` in `snowhousecpu_opci_list_hash`. */
//  //opc_info = snowhousecpu_opci_list_lookup ("index")->opc_info;
//
//  //opc_info = &snowhousecpu_opc_info_g4[SNOWHOUSECPU_G4_OP_ENUM_INDEX_RA_RB];
//  opc_info = &snowhousecpu_opc_info_g5_index_ra_rb[0];
//  //printf
//  //  ("snowhousecpu_enc_temp_insn_index(): %u\n", (unsigned)rc_ind);
//
//  const uint32_t ret = snowhousecpu_enc_temp_insn_non_pre_lpre
//    (opc_info, /* opc_info */
//    ra_ind, /* ra_ind */
//    rb_ind, /* rb_ind */
//    0, /* simm */
//    0 /* fwl */);
//  //printf("snowhousecpu_enc_temp_insn_index(): enc insn: %x\n", ret);
//  return ret;
//}
/* -------- */

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
  //const char *old_name = name;

  nlen = 0;

  //fprintf (stderr, "parse_worker (): \"%s\"\n", *ptr);

  for (s=*ptr;
    *s
      && !is_end_of_line[(*s) & 0xff]
      && *s != ' '
      && (ISALNUM (*s) || (*s) == '_')
      && *name != '\0'
      && *s != '\0';
    ++s, ++name)
  {
    //fprintf (stderr, "parse_worker (): \"%c\" \"%c\"\n", *s, *name);
    if (*s != *name)
    {
      //fprintf (stderr, "parse_worker (): returning false: %d\n",
      //  (unsigned) nlen);
      return false;
    }
    ++nlen;
  }
  //if (
  //  (*s != '\0' && *name != '\0')
  //  || (*s == '\0' && *name != '\0')
  //)
  //{
  //  return false;
  //}
  if (ISALNUM (*s) || *s == '_')
  {
    return false;
  }

  //fprintf (stderr, "parse_worker (): \"%s\" \"%s\" %d\n",
  //  *ptr, old_name, (unsigned) nlen);

  *ptr += nlen;

  //fprintf (stderr, "parse_worker (): returning true: %d\n",
  //  (unsigned) nlen);
  return true;
}

static bool
parse_register_operand_worker (char **ptr,
                              const snowhousecpu_reg_t *test_reg)
{
  return parse_worker (ptr, test_reg->name);
}

static const snowhousecpu_reg_t *
parse_register_operand (char **ptr)
{
  unsigned i;
  for (i=0; i<SNOWHOUSECPU_NUM_GPRS; ++i)
  {
    //fprintf (stderr, "parse_register_operand (): %s\n", gprs[i].name);

    if (parse_register_operand_worker (ptr, gprs + i))
    {
      //fprintf (stderr, "testificate\n");
      return gprs + i;
    }
  }
  for (i=0; i<SNOWHOUSECPU_NUM_SPRS; ++i)
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
  return (const snowhousecpu_reg_t *) NULL;
}
/* -------- */
long
md_pcrel_from (fixS *fixP ATTRIBUTE_UNUSED)
{
  return 0;
  // TODO: verify whether we need the old, commented-out code
  //return fixP->fx_where + fixP->fx_frag->fr_address;
  //valueT addr = fixP->fx_where + fixP->fx_frag->fr_address
    //+ fixP->fx_frag->fr_var
    ;

  /* Offset is from the end of the instruction. */
  //switch (fixP->fx_r_type)
  //{
  //  //case BFD_RELOC_SNOWHOUSECPU_G1_S5:
  //  case BFD_RELOC_SNOWHOUSECPU_G3_S9_PCREL:
  //    //return addr + have_plp_insn_length (SNOWHOUSECPU_HAVE_PRE_NONE);
  //    return addr - have_plp_insn_length (SNOWHOUSECPU_HAVE_PRE_NONE)
  //      + have_plp_insn_length (SNOWHOUSECPU_HAVE_PRE_NONE);

  //  //case BFD_RELOC_SNOWHOUSECPU_G1_S17:
  //  case BFD_RELOC_SNOWHOUSECPU_G3_S21_PCREL:
  //    //return addr + have_plp_insn_length (SNOWHOUSECPU_HAVE_PRE_PRE);
  //    return addr - have_plp_insn_length (SNOWHOUSECPU_HAVE_PRE_PRE)
  //      + have_plp_insn_length (SNOWHOUSECPU_HAVE_PRE_NONE);

  //  //case BFD_RELOC_SNOWHOUSECPU_G1_S32:
  //  //case BFD_RELOC_SNOWHOUSECPU_G1_S32_ADD32:
  //  //case BFD_RELOC_SNOWHOUSECPU_G1_S32_SUB32:
  //  //case BFD_RELOC_SNOWHOUSECPU_G3_S32_NO_RELAX:
  //  case BFD_RELOC_SNOWHOUSECPU_G3_S32_PCREL:
  //  case BFD_RELOC_SNOWHOUSECPU_G3_S32_PCREL_NO_RELAX:
  //    //return addr + have_plp_insn_length (SNOWHOUSECPU_HAVE_PLP_LPRE);
  //    return addr - have_plp_insn_length (SNOWHOUSECPU_HAVE_PLP_LPRE)
  //      + have_plp_insn_length (SNOWHOUSECPU_HAVE_PRE_NONE);

  //  default:
  //    abort ();
  //    return addr;
  //}
  //return addr;
}

static inline bool
snowhousecpu_relaxable_symbol (symbolS *sym)
{
  return (
    sym != NULL
    && S_IS_DEFINED (sym)
    //&& !S_IS_EXTERNAL (sym)
    && !S_IS_WEAK (sym)
  );
}


/* Apply a fixup to the object file.  */
void
md_apply_fix (fixS *fixP,
              valueT *valP ATTRIBUTE_UNUSED,
              segT seg ATTRIBUTE_UNUSED)
{
  struct
  {
    unsigned int subtype;
    bfd_byte *buf;
    offsetT loc;
    segT sub_segment;
    snowhousecpu_temp_t
      prefix_insn,
      insn;
    bfd_vma
      target;
    unsigned int
      //lpre_offs,
      pre_offs,
      insn_offs;
    reloc_howto_type *howto;
  } tmp;
  memset (&tmp, 0, sizeof (tmp));
  tmp.buf = (bfd_byte *) (fixP->fx_frag->fr_literal + fixP->fx_where);
  //bool relaxable = false;

  //fprintf (stderr,
  //  "md_apply_fix (): begin: %lx %lx; %lx; %u %u %u %u\n",
  //  (long)(*valP), (long)fixP->fx_offset,
  //  *(long *)tmp.buf,
  //  fixP->fx_r_type == BFD_RELOC_SNOWHOUSECPU_G1_S5,
  //  fixP->fx_r_type == BFD_RELOC_SNOWHOUSECPU_G1_S17,
  //  fixP->fx_r_type == BFD_RELOC_SNOWHOUSECPU_G1_S32,
  //  fixP->fx_r_type == BFD_RELOC_SNOWHOUSECPU_G1_S32_NO_RELAX
  //  );

  //if (!fixP->fx_done)
  {
    //fprintf (stderr,
    //  "md_apply_fix (): !fixP->fx_done: %lx\n",
    //  *valP);
    /* Remember value for tc_gen_reloc */
    /* Note: `valP` is set to an expression using `fixP->fx_offset`
      in `fixup_segment ()` in "../write.c" */
    fixP->fx_addnumber = *valP;
    //fixP->fx_addnumber = fixP->fx_offset;
  }

  switch (fixP->fx_r_type)
  {
    /* Just force these to become relocs */
    //case BFD_RELOC_SNOWHOUSECPU_G1_U5:
    //case BFD_RELOC_SNOWHOUSECPU_G1_S5:
    //case BFD_RELOC_SNOWHOUSECPU_G5_INDEX_S7:
    //case BFD_RELOC_SNOWHOUSECPU_G7_ICRELOAD_S5:
    case BFD_RELOC_SNOWHOUSECPU_S16:
    case BFD_RELOC_SNOWHOUSECPU_SHIFT_U5:
      ////tmp.buf = bfd_putl32 (bfd_getl16 (tmp.buf));
      if (fixP->fx_addsy == NULL
        //|| snowhousecpu_relaxable_symbol (fixP->fx_addsy)
        )
      {
        tmp.insn = bfd_getl32 (tmp.buf);
        if (fixP->fx_r_type == BFD_RELOC_SNOWHOUSECPU_S16)
        {
          snowhousecpu_set_insn_field_p
            (SNOWHOUSECPU_IMM16_MASK, SNOWHOUSECPU_IMM16_BITPOS, &tmp.insn,
              (snowhousecpu_temp_t)(*valP));
          fixP->fx_addnumber = *valP = snowhousecpu_get_insn_field
            (SNOWHOUSECPU_IMM16_MASK, SNOWHOUSECPU_IMM16_BITPOS, tmp.insn);
        }
        else /* if (fixP->fx_r_type == BFD_RELOC_SNOWHOUSECPU_SHIFT_U5) */
        {
          snowhousecpu_set_insn_field_p
            (SNOWHOUSECPU_SHIFT_IMM5_MASK, SNOWHOUSECPU_SHIFT_IMM5_BITPOS, &tmp.insn,
              (snowhousecpu_temp_t)(*valP));
          fixP->fx_addnumber = *valP = snowhousecpu_get_insn_field
            (SNOWHOUSECPU_SHIFT_IMM5_MASK, SNOWHOUSECPU_SHIFT_IMM5_BITPOS, tmp.insn);
        }
        //if (fixP->fx_r_type == BFD_RELOC_SNOWHOUSECPU_G1_U5
        //  || fixP->fx_r_type == BFD_RELOC_SNOWHOUSECPU_G1_S5)
        //{
        //  snowhousecpu_set_insn_field_ei_p
        //    (&snowhousecpu_enc_info_g1_i5, &tmp.insn,
        //      (snowhousecpu_temp_t)(*valP));
        //  fixP->fx_addnumber = *valP = snowhousecpu_get_insn_field_ei
        //    (&snowhousecpu_enc_info_g1_i5, tmp.insn);
        //}
        //else if (fixP->fx_r_type == BFD_RELOC_SNOWHOUSECPU_G5_INDEX_S7)
        //{
        //  snowhousecpu_set_insn_field_ei_p
        //    (&snowhousecpu_enc_info_g5_index_ra_simm_s7, &tmp.insn,
        //      (snowhousecpu_temp_t)(*valP));
        //  fixP->fx_addnumber = *valP = snowhousecpu_get_insn_field_ei
        //    (&snowhousecpu_enc_info_g5_index_ra_simm_s7, tmp.insn);
        //}
        //else /* if (fixP->fx_r_type == BFD_RELOC_SNOWHOUSECPU_G7_ICRELOAD_S5) */
        //{
        //  snowhousecpu_set_insn_field_ei_p
        //    (&snowhousecpu_enc_info_g7_icreload_s5, &tmp.insn,
        //      (snowhousecpu_temp_t)(*valP));
        //  fixP->fx_addnumber = *valP = snowhousecpu_get_insn_field_ei
        //    (&snowhousecpu_enc_info_g7_icreload_s5, tmp.insn);
        //}
        bfd_putl32 (tmp.insn, tmp.buf);
        //if (
        //  //!linkrelax
        //  //&& 
        //  fixP->fx_addsy == NULL)
        {
          fixP->fx_done = true;
        }
        //fprintf (stderr,
        //  "md_apply_fix (): s5: %lx\n",
        //  fixP->fx_addnumber);
      }
      break;
    //case BFD_RELOC_SNOWHOUSECPU_G1_S17_FOR_U5:
    //case BFD_RELOC_SNOWHOUSECPU_G1_S17:
    //case BFD_RELOC_SNOWHOUSECPU_G5_INDEX_S19:
    //case BFD_RELOC_SNOWHOUSECPU_G7_ICRELOAD_S17:
    //case BFD_RELOC_SNOWHOUSECPU_S32_FOR_S16:
    //  if (fixP->fx_addsy == NULL
    //    //|| snowhousecpu_relaxable_symbol (fixP->fx_addsy)
    //  )
    //  {
    //    tmp.pre_offs = 0;
    //    tmp.insn_offs = tmp.pre_offs
    //      + snowhousecpu_have_pre_distance
    //        (SNOWHOUSECPU_HAVE_PRE_PRE, SNOWHOUSECPU_HAVE_PRE_NONE);
    //    tmp.prefix_insn = bfd_getl32 (tmp.buf + tmp.pre_offs);
    //    tmp.insn = bfd_getl32 (tmp.buf + tmp.insn_offs);
    //    //snowhousecpu_put_g1_s17 (&tmp.prefix_insn, &tmp.insn,
    //    //  (snowhousecpu_temp_t)(*valP));
    //    snowhousecpu_put_s32_p (
    //      &tmp.prefix_insn,
    //      &tmp.insn, (snowhousecpu_temp_t)(*valP)
    //    );
    //    fixP->fx_addnumber = *valP = snowhousecpu_get_s32
    //      (tmp.prefix_insn, tmp.insn);
    //    //if (fixP->fx_r_type == BFD_RELOC_SNOWHOUSECPU_G1_S17_FOR_U5
    //    //  || fixP->fx_r_type == BFD_RELOC_SNOWHOUSECPU_G1_S17)
    //    //{
    //    //  snowhousecpu_put_g1_s17 (&tmp.prefix_insn, &tmp.insn,
    //    //    (snowhousecpu_temp_t)(*valP));
    //    //  fixP->fx_addnumber = *valP = snowhousecpu_get_g1_s17
    //    //    (tmp.prefix_insn, tmp.insn);
    //    //}
    //    //else if (fixP->fx_r_type == BFD_RELOC_SNOWHOUSECPU_G5_INDEX_S19)
    //    //{
    //    //  snowhousecpu_put_g5_index_s19 (&tmp.prefix_insn, &tmp.insn,
    //    //    (snowhousecpu_temp_t)(*valP));
    //    //  fixP->fx_addnumber = *valP = snowhousecpu_get_g5_index_s19
    //    //    (tmp.prefix_insn, tmp.insn);
    //    //}
    //    //else /* if (
    //    //  fixP->fx_r_type == BFD_RELOC_SNOWHOUSECPU_G7_ICRELOAD_S17
    //    //) */
    //    //{
    //    //  snowhousecpu_put_g7_icreload_s17 (&tmp.prefix_insn, &tmp.insn,
    //    //    (snowhousecpu_temp_t)(*valP));
    //    //  fixP->fx_addnumber = *valP = snowhousecpu_get_g7_icreload_s17
    //    //    (tmp.prefix_insn, tmp.insn);
    //    //}
    //    bfd_putl32 (tmp.prefix_insn, tmp.buf + tmp.pre_offs);
    //    bfd_putl32 (tmp.insn, tmp.buf + tmp.insn_offs);
    //    //if (
    //    //  //!linkrelax
    //    //  //&&
    //    //  fixP->fx_addsy == NULL)
    //    {
    //      fixP->fx_done = true;
    //    }
    //  }
    //  break;
    //case BFD_RELOC_SNOWHOUSECPU_G1_S32_NO_RELAX:
    //  if (fixP->fx_addsy == NULL)
    //  {
    //    fixP->fx_done = true;
    //  }
    //  break;
    //case BFD_RELOC_SNOWHOUSECPU_G3_S9_PCREL:
    case BFD_RELOC_SNOWHOUSECPU_S16_PCREL:
      //if (fixP->fx_addsy == NULL
      //  || snowhousecpu_relaxable_symbol (fixP->fx_addsy))
      //{
      //  tmp.insn = bfd_getl32 (tmp.buf);
      //  if (fixP->fx_addsy == NULL)
      //  {
      //    tmp.target = 0;
      //  }
      //  else /* if (fixP->fx_addsy != NULL) */
      //  {
      //    tmp.target = S_GET_VALUE (fixP->fx_addsy);
      //  }
      //  tmp.target += (*valP);
      //  snowhousecpu_set_insn_field_ei_p (&snowhousecpu_enc_info_g3_s9, &tmp.insn,
      //    (snowhousecpu_temp_t)(tmp.target
      //      - have_plp_insn_length (SNOWHOUSECPU_HAVE_PRE_NONE))
      //    );
      //  bfd_putl32 (tmp.insn, tmp.buf);
      //  //if (fixP->fx_addsy == NULL)
      //  //{
      //  //  fixP->fx_done = true;
      //  //}
      //  //if (!linkrelax)
      //  //{
      //  //  fixP->fx_done = true;
      //  //}
      //  fixP->fx_done = true;
      //}
      break;
    //case BFD_RELOC_SNOWHOUSECPU_G3_S21_PCREL:
    case BFD_RELOC_SNOWHOUSECPU_S32_FOR_S16_PCREL:
      //if (fixP->fx_addsy == NULL
      //  || snowhousecpu_relaxable_symbol (fixP->fx_addsy))
      //{
      //  tmp.pre_offs = 0;
      //  tmp.insn_offs = tmp.pre_offs
      //    + snowhousecpu_have_pre_distance
      //      (SNOWHOUSECPU_HAVE_PRE_PRE, SNOWHOUSECPU_HAVE_PRE_NONE);
      //  tmp.prefix_insn = bfd_getl32 (tmp.buf + tmp.pre_offs);
      //  tmp.insn = bfd_getl32 (tmp.buf + tmp.insn_offs);
      //  if (fixP->fx_addsy == NULL)
      //  {
      //    tmp.target = 0;
      //  }
      //  else /* if (fixP->fx_addsy != NULL) */
      //  {
      //    tmp.target = S_GET_VALUE (fixP->fx_addsy);
      //  }
      //  tmp.target += (*valP);
      //  snowhousecpu_put_g3_s21 (&tmp.prefix_insn, &tmp.insn,
      //    (snowhousecpu_temp_t)(tmp.target
      //      - have_plp_insn_length (SNOWHOUSECPU_HAVE_PRE_PRE)));
      //  bfd_putl32 (tmp.prefix_insn, tmp.buf + tmp.pre_offs);
      //  bfd_putl32 (tmp.insn, tmp.buf + tmp.insn_offs);
      //  //if (fixP->fx_addsy == NULL)
      //  //{
      //  //  fixP->fx_done = true;
      //  //}
      //  //if (!linkrelax)
      //  //{
      //  //  fixP->fx_done = true;
      //  //}
      //  fixP->fx_done = true;
      //}
      break;
    //case BFD_RELOC_SNOWHOUSECPU_G3_S32_PCREL:
    //case BFD_RELOC_SNOWHOUSECPU_G3_S32_PCREL_NO_RELAX:
    //  //if (fixP->fx_addsy == NULL
    //  //  || snowhousecpu_relaxable_symbol (fixP->fx_addsy))
    //  //{
    //  //  tmp.lpre_offs = 0;
    //  //  tmp.insn_offs = tmp.lpre_offs
    //  //    + snowhousecpu_have_pre_distance
    //  //      (SNOWHOUSECPU_HAVE_PLP_LPRE, SNOWHOUSECPU_HAVE_PRE_NONE);
    //  //  tmp.prefix_insn = bfd_getb64 (tmp.buf + tmp.lpre_offs);
    //  //  tmp.insn = bfd_getl32 (tmp.buf + tmp.insn_offs);
    //  //  if (fixP->fx_addsy == NULL)
    //  //  {
    //  //    //snowhousecpu_put_g3_s21 (&tmp.prefix_insn, &tmp.insn,
    //  //    //  (snowhousecpu_temp_t)(*valP));
    //  //    tmp.target = 0;
    //  //  }
    //  //  else /* if (fixP->fx_addsy != NULL) */
    //  //  {
    //  //    tmp.target = S_GET_VALUE (fixP->fx_addsy);
    //  //  }
    //  //  tmp.target += (*valP);
    //  //  snowhousecpu_put_g3_s32 (&tmp.prefix_insn, &tmp.insn,
    //  //    (snowhousecpu_temp_t)(tmp.target
    //  //      - have_plp_insn_length (SNOWHOUSECPU_HAVE_PLP_LPRE)));
    //  //  bfd_putb64 (tmp.prefix_insn, tmp.buf + tmp.lpre_offs);
    //  //  bfd_putl32 (tmp.insn, tmp.buf + tmp.insn_offs);
    //  //  //if (fixP->fx_addsy == NULL)
    //  //  //{
    //  //  //  fixP->fx_done = true;
    //  //  //}
    //  //  //if (!linkrelax)
    //  //  //{
    //  //  //  fixP->fx_done = true;
    //  //  //}
    //  //  fixP->fx_done = true;
    //  //}
    //  break;

    case BFD_RELOC_SNOWHOUSECPU_S32_FOR_S16_ADD32:
    case BFD_RELOC_SNOWHOUSECPU_S32_FOR_S16_SUB32:
    //case BFD_RELOC_SNOWHOUSECPU_G1_S32_FOR_U5_ADD32:
    //case BFD_RELOC_SNOWHOUSECPU_G1_S32_FOR_U5_SUB32:
    //case BFD_RELOC_SNOWHOUSECPU_G1_S32_ADD32:
    //case BFD_RELOC_SNOWHOUSECPU_G1_S32_SUB32:
    //case BFD_RELOC_SNOWHOUSECPU_G5_INDEX_S32_ADD32:
    //case BFD_RELOC_SNOWHOUSECPU_G5_INDEX_S32_SUB32:
    //case BFD_RELOC_SNOWHOUSECPU_G7_ICRELOAD_S32_ADD32:
    //case BFD_RELOC_SNOWHOUSECPU_G7_ICRELOAD_S32_SUB32:
    case BFD_RELOC_SNOWHOUSECPU_PSEUDO_ADD8:
    case BFD_RELOC_SNOWHOUSECPU_PSEUDO_ADD16:
    case BFD_RELOC_SNOWHOUSECPU_PSEUDO_ADD32:
    case BFD_RELOC_SNOWHOUSECPU_PSEUDO_ADD64:
    case BFD_RELOC_SNOWHOUSECPU_PSEUDO_SUB8:
    case BFD_RELOC_SNOWHOUSECPU_PSEUDO_SUB16:
    case BFD_RELOC_SNOWHOUSECPU_PSEUDO_SUB32:
    case BFD_RELOC_SNOWHOUSECPU_PSEUDO_SUB64:
    case BFD_RELOC_SNOWHOUSECPU_CFA_SUB6:
      break;

    case BFD_RELOC_32:
    /* TODO: determine whether the below `case`s should be here, or below
      the `if` statement. */
    //case BFD_RELOC_SNOWHOUSECPU_G1_S32_FOR_U5:
    //case BFD_RELOC_SNOWHOUSECPU_G1_S32_FOR_U5_NO_RELAX:
    //case BFD_RELOC_SNOWHOUSECPU_G1_S32:
    //case BFD_RELOC_SNOWHOUSECPU_G1_S32_NO_RELAX:
    //case BFD_RELOC_SNOWHOUSECPU_G7_ICRELOAD_S32:
    //case BFD_RELOC_SNOWHOUSECPU_G7_ICRELOAD_S32_NO_RELAX:
      /* Blindly copied from RISC-V. */
      /* Use pc-relative relocation for FDE initial location.
         The symbol address in .eh_frame may be adjusted in
         _bfd_elf_discard_section_eh_frame, and the content of
         .eh_frame will be adjusted in _bfd_elf_write_section_eh_frame.
         Therefore, we cannot insert a relocation whose addend symbol is
         in .eh_frame.  Othrewise, the value may be adjusted twice.  */
      if (fixP->fx_addsy && fixP->fx_subsy
        && (tmp.sub_segment = S_GET_SEGMENT (fixP->fx_subsy))
        && strcmp (tmp.sub_segment->name, ".eh_frame") == 0
        && S_GET_VALUE (fixP->fx_subsy)
          == fixP->fx_frag->fr_address + fixP->fx_where)
      {
        fixP->fx_r_type = BFD_RELOC_SNOWHOUSECPU_EH_32_PCREL;
        fixP->fx_subsy = NULL;
        break;
      }
      /* Fall through.  */
    case BFD_RELOC_64:
    case BFD_RELOC_16:
    case BFD_RELOC_8:
    //case BFD_RELOC_SNOWHOUSECPU_G1_S32_FOR_U5:
    //case BFD_RELOC_SNOWHOUSECPU_G1_S32_FOR_U5_NO_RELAX:
    //case BFD_RELOC_SNOWHOUSECPU_G1_S32:
    //case BFD_RELOC_SNOWHOUSECPU_G1_S32_NO_RELAX:
    //case BFD_RELOC_SNOWHOUSECPU_G5_INDEX_S32:
    //case BFD_RELOC_SNOWHOUSECPU_G5_INDEX_S32_NO_RELAX:
    //case BFD_RELOC_SNOWHOUSECPU_G7_ICRELOAD_S32:
    //case BFD_RELOC_SNOWHOUSECPU_G7_ICRELOAD_S32_NO_RELAX:
    //case BFD_RELOC_SNOWHOUSECPU_S32_FOR_S16:
    case BFD_RELOC_SNOWHOUSECPU_S32_FOR_S16:
    case BFD_RELOC_SNOWHOUSECPU_CFA:
      if (fixP->fx_addsy && fixP->fx_subsy)
      {
        fixP->fx_next = xmemdup (fixP, sizeof (*fixP), sizeof (*fixP));
        fixP->fx_next->fx_addsy = fixP->fx_subsy;
        fixP->fx_next->fx_subsy = NULL;
        fixP->fx_next->fx_offset = 0;
        fixP->fx_subsy = NULL;

        switch (fixP->fx_r_type)
        {
          case BFD_RELOC_64:
            fixP->fx_r_type = BFD_RELOC_SNOWHOUSECPU_PSEUDO_ADD64;
            fixP->fx_next->fx_r_type = BFD_RELOC_SNOWHOUSECPU_PSEUDO_SUB64;
            break;
          case BFD_RELOC_32:
            fixP->fx_r_type = BFD_RELOC_SNOWHOUSECPU_PSEUDO_ADD32;
            fixP->fx_next->fx_r_type = BFD_RELOC_SNOWHOUSECPU_PSEUDO_SUB32;
            break;
          case BFD_RELOC_16:
            fixP->fx_r_type = BFD_RELOC_SNOWHOUSECPU_PSEUDO_ADD16;
            fixP->fx_next->fx_r_type = BFD_RELOC_SNOWHOUSECPU_PSEUDO_SUB16;
            break;
          case BFD_RELOC_8:
            fixP->fx_r_type = BFD_RELOC_SNOWHOUSECPU_PSEUDO_ADD8;
            fixP->fx_next->fx_r_type = BFD_RELOC_SNOWHOUSECPU_PSEUDO_SUB8;
            break;
          //case BFD_RELOC_SNOWHOUSECPU_G1_S32_FOR_U5:
          //case BFD_RELOC_SNOWHOUSECPU_G1_S32_FOR_U5_NO_RELAX:
          //  fixP->fx_r_type = BFD_RELOC_SNOWHOUSECPU_G1_S32_FOR_U5_ADD32;
          //  fixP->fx_next->fx_r_type
          //    = BFD_RELOC_SNOWHOUSECPU_G1_S32_FOR_U5_SUB32;
          //  break;
          //case BFD_RELOC_SNOWHOUSECPU_G1_S32:
          //case BFD_RELOC_SNOWHOUSECPU_G1_S32_NO_RELAX:
          case BFD_RELOC_SNOWHOUSECPU_S32_FOR_S16:
            //if (fixP->fx_r_type == BFD_RELOC_SNOWHOUSECPU_G1_S32_NO_RELAX)
            //{
            //  fprintf (
            //    stderr,
            //    "\ntestificate\n"
            //  );
            //}
            fixP->fx_r_type = BFD_RELOC_SNOWHOUSECPU_S32_FOR_S16_ADD32;
            fixP->fx_next->fx_r_type = BFD_RELOC_SNOWHOUSECPU_S32_FOR_S16_SUB32;
            break;
          //case BFD_RELOC_SNOWHOUSECPU_G5_INDEX_S32:
          //case BFD_RELOC_SNOWHOUSECPU_G5_INDEX_S32_NO_RELAX:
          //  fixP->fx_r_type = BFD_RELOC_SNOWHOUSECPU_G5_INDEX_S32_ADD32;
          //  fixP->fx_next->fx_r_type = BFD_RELOC_SNOWHOUSECPU_G5_INDEX_S32_SUB32;
          //  break;
          //case BFD_RELOC_SNOWHOUSECPU_G7_ICRELOAD_S32:
          //case BFD_RELOC_SNOWHOUSECPU_G7_ICRELOAD_S32_NO_RELAX:
          //  fixP->fx_r_type = BFD_RELOC_SNOWHOUSECPU_G7_ICRELOAD_S32_ADD32;
          //  fixP->fx_next->fx_r_type
          //    = BFD_RELOC_SNOWHOUSECPU_G7_ICRELOAD_S32_SUB32;
          //  break;

          /* Blindly copied from RISC-V. */
          case BFD_RELOC_SNOWHOUSECPU_CFA:
            /* Load the byte to get the tmp.subtype.  */
            tmp.subtype = bfd_get_8 (NULL, 
              &((fragS *) (fixP->fx_frag->fr_opcode))->fr_literal
                [fixP->fx_where]);
            tmp.loc = fixP->fx_frag->fr_fix - (tmp.subtype & 7);
            switch (tmp.subtype)
            {
              case DW_CFA_advance_loc1:
                fixP->fx_where = tmp.loc + 1;
                fixP->fx_next->fx_where = tmp.loc + 1;
                fixP->fx_r_type = BFD_RELOC_SNOWHOUSECPU_SET8;
                fixP->fx_next->fx_r_type = BFD_RELOC_SNOWHOUSECPU_PSEUDO_SUB8;
                break;

              case DW_CFA_advance_loc2:
                fixP->fx_size = 2;
                fixP->fx_next->fx_size = 2;
                fixP->fx_where = tmp.loc + 1;
                fixP->fx_next->fx_where = tmp.loc + 1;
                fixP->fx_r_type = BFD_RELOC_SNOWHOUSECPU_SET16;
                fixP->fx_next->fx_r_type = BFD_RELOC_SNOWHOUSECPU_PSEUDO_SUB16;
                break;

              case DW_CFA_advance_loc4:
                fixP->fx_size = 4;
                fixP->fx_next->fx_size = 4;
                fixP->fx_where = tmp.loc;
                fixP->fx_next->fx_where = tmp.loc;
                fixP->fx_r_type = BFD_RELOC_SNOWHOUSECPU_SET32;
                fixP->fx_next->fx_r_type = BFD_RELOC_SNOWHOUSECPU_PSEUDO_SUB32;
                break;

              default:
                if (tmp.subtype < 0x80 && (tmp.subtype & 0x40))
                {
                  /* DW_CFA_advance_loc */
                  fixP->fx_frag = (fragS *) fixP->fx_frag->fr_opcode;
                  fixP->fx_next->fx_frag = fixP->fx_frag;
                  fixP->fx_r_type = BFD_RELOC_SNOWHOUSECPU_CFA_SET6;
                  fixP->fx_next->fx_r_type = BFD_RELOC_SNOWHOUSECPU_CFA_SUB6;
                }
                else
                {
                  as_fatal (_("internal: bad CFA value #%d"),
                    tmp.subtype);
                }
                break;
            }
            break;

          default:
            /* This case is unreachable. */
            abort ();
            break;
        }
      }
      else if (
        (
          //fixP->fx_r_type == BFD_RELOC_SNOWHOUSECPU_G1_S32_FOR_U5
          //|| fixP->fx_r_type
          //  == BFD_RELOC_SNOWHOUSECPU_G1_S32_FOR_U5_NO_RELAX
          //|| fixP->fx_r_type == BFD_RELOC_SNOWHOUSECPU_G1_S32
          //|| fixP->fx_r_type == BFD_RELOC_SNOWHOUSECPU_G1_S32_NO_RELAX

          //|| fixP->fx_r_type == BFD_RELOC_SNOWHOUSECPU_G5_INDEX_S32
          //|| fixP->fx_r_type == BFD_RELOC_SNOWHOUSECPU_G5_INDEX_S32_NO_RELAX

          //|| fixP->fx_r_type == BFD_RELOC_SNOWHOUSECPU_G7_ICRELOAD_S32
          //|| fixP->fx_r_type == BFD_RELOC_SNOWHOUSECPU_G7_ICRELOAD_S32_NO_RELAX
          fixP->fx_r_type == BFD_RELOC_SNOWHOUSECPU_S32_FOR_S16
        ) && (
          fixP->fx_addsy == NULL
          //|| snowhousecpu_relaxable_symbol (fixP->fx_addsy)
        )
      )
      {
        tmp.pre_offs = 0;
        tmp.insn_offs = tmp.pre_offs
          + snowhousecpu_have_pre_distance
            (SNOWHOUSECPU_HAVE_PRE_PRE, SNOWHOUSECPU_HAVE_PRE_NONE);
        tmp.prefix_insn = (
          //bfd_getb64 (tmp.buf + tmp.lpre_offs)
          (bfd_getl32 (tmp.buf + tmp.pre_offs) << 32)
          | bfd_getl32 (tmp.buf + tmp.pre_offs + 4)
        );
        tmp.insn = bfd_getl32 (tmp.buf + tmp.insn_offs);
        snowhousecpu_put_s32_p (&tmp.prefix_insn, &tmp.insn,
          (snowhousecpu_temp_t)(*valP));
        fixP->fx_addnumber = *valP = snowhousecpu_get_s32
          (tmp.prefix_insn, tmp.insn);
        //if (fixP->fx_r_type == BFD_RELOC_SNOWHOUSECPU_G1_S32_FOR_U5
        //  || fixP->fx_r_type
        //    == BFD_RELOC_SNOWHOUSECPU_G1_S32_FOR_U5_NO_RELAX
        //  || fixP->fx_r_type == BFD_RELOC_SNOWHOUSECPU_G1_S32
        //  || fixP->fx_r_type == BFD_RELOC_SNOWHOUSECPU_G1_S32_NO_RELAX)
        //{
        //  snowhousecpu_put_g1_s32 (&tmp.prefix_insn, &tmp.insn,
        //    (snowhousecpu_temp_t)(*valP));
        //  fixP->fx_addnumber = *valP = snowhousecpu_get_g1_s32
        //    (tmp.prefix_insn, tmp.insn);
        //}
        //else if (
        //  fixP->fx_r_type == BFD_RELOC_SNOWHOUSECPU_G5_INDEX_S32
        //  || fixP->fx_r_type == BFD_RELOC_SNOWHOUSECPU_G5_INDEX_S32_NO_RELAX
        //)
        //{
        //  snowhousecpu_put_g5_index_s32 (&tmp.prefix_insn, &tmp.insn,
        //    (snowhousecpu_temp_t)(*valP));
        //  fixP->fx_addnumber = *valP = snowhousecpu_get_g5_index_s32
        //    (tmp.prefix_insn, tmp.insn);
        //  //printf(
        //  //  "G5_INDEX_S32 or G5_INDEX_S32_NO_RELAX: \n"
        //  //);
        //}
        //else /* if (
        //  fixP->fx_r_type == BFD_RELOC_SNOWHOUSECPU_G7_ICRELOAD_S32
        //  || fixP->fx_r_type == BFD_RELOC_SNOWHOUSECPU_G7_ICRELOAD_S32_NO_RELAX
        //) */
        //{
        //  snowhousecpu_put_g7_icreload_s32 (&tmp.prefix_insn, &tmp.insn,
        //    (snowhousecpu_temp_t)(*valP));
        //  fixP->fx_addnumber = *valP = snowhousecpu_get_g7_icreload_s32
        //    (tmp.prefix_insn, tmp.insn);
        //}
        //bfd_putb64 (tmp.prefix_insn, tmp.buf + tmp.lpre_offs);
        bfd_putl32 (
          (tmp.prefix_insn >> 32) & 0xffffffffull,
          tmp.buf + tmp.pre_offs
        );
        bfd_putl32 (
          tmp.prefix_insn & 0xffffffffull,
          tmp.buf + tmp.pre_offs + 4
        );
        bfd_putl32 (tmp.insn, tmp.buf + tmp.insn_offs);
        //printf ("testificate\n");
        //if (fixP->fx_addsy == NULL)
        {
          fixP->fx_done = true;
        }
      }
      break;

    default:
      /* We ignore generic BFD relocations we don't know about. */
      if (
        (tmp.howto = bfd_reloc_type_lookup (stdoutput, fixP->fx_r_type))
          != NULL
      )
      {
        as_fatal (_("internal: bad relocation \"%s\" #%d"),
          tmp.howto->name, fixP->fx_r_type);
      }
      break;
  }

  if (fixP->fx_subsy != NULL)
  {
    as_bad_subtract (fixP);
  }
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
snowhousecpu_number_to_chars (char *buf, valueT val, int n)
{
  //gas_assert (n == 1 || n == 2 || n == 4 || n == 8);
  number_to_chars_littleendian (buf, val, n);
  //number_to_chars_bigendian (buf, val, n);
}

/* Turn a string in input_line_pointer into a floating point constant
   of type TYPE, and store the appropriate bytes in *LITP.  The number
   of LITTLENUMS emitted is stored in *SIZEP.  An error message is
   returned, or NULL on OK.  */
const char *
snowhousecpu_md_atof (int type, char *litP, int *sizeP)
{
  //return ieee_md_atof (type, litP, sizeP, false);
  return ieee_md_atof (type, litP, sizeP, true);
}

//struct snowhousecpu_opt_t
//{
//  ///* -mrelax, -mno-relax: do (or not) relaxing in GAS */
//  //bool relax: 1;
//  ///* -mgasrelax, -mno-gasrelax: do (or not) relaxing in GAS */
//  //bool gasrelax: 1;
//  /* -mlinkrelax, -mno-linkrelax: generate (or not) relocations for
//    linker relaxation. */
//  bool linkrelax: 1;
//};
//static struct snowhousecpu_opt_t snowhousecpu_opt = { false };

//enum options
//{
//  //OPTION_RELAX = OPTION_MD_BASE + 1,
//  //OPTION_NO_RELAX,
//  //OPTION_GASRELAX,
//  //OPTION_NO_GASRELAX,
//  OPTION_LINKRELAX = OPTION_MD_BASE + 1,
//  OPTION_NO_LINKRELAX,
//};
struct option md_longopts[] =
{
  //{ "mrelax", no_argument, NULL, OPTION_RELAX },
  //{ "mno-relax", no_argument, NULL, OPTION_NO_RELAX },
  //{ "mgasrelax", no_argument, NULL, OPTION_GASRELAX },
  //{ "mno-gasrelax", no_argument, NULL, OPTION_NO_GASRELAX },
  //{ "mlinkrelax", no_argument, NULL, OPTION_LINKRELAX },
  //{ "mno-linkrelax", no_argument, NULL, OPTION_NO_LINKRELAX },
  { NULL, no_argument, NULL, 0},
};
size_t md_longopts_size = sizeof (md_longopts);

const char *md_shortopts = "";

int
md_parse_option (int c ATTRIBUTE_UNUSED, const char *arg ATTRIBUTE_UNUSED)
{
  //return 1;
  switch (c)
  {
    //case OPTION_RELAX:
    //  //snowhousecpu_opt.gasrelax = true;
    //  //snowhousecpu_opt.linkrelax = true;
    //  snowhousecpu_opt.relax = true;
    //  break;
    //case OPTION_NO_RELAX:
    //  //snowhousecpu_opt.gasrelax = false;
    //  //snowhousecpu_opt.linkrelax = false;
    //  snowhousecpu_opt.relax = false;
    //  break;
    //case OPTION_GASRELAX:
    //  snowhousecpu_opt.gasrelax = true;
    //  break;
    //case OPTION_NO_GASRELAX:
    //  snowhousecpu_opt.gasrelax = false;
    //  break;
    //case OPTION_LINKRELAX:
    //  snowhousecpu_opt.linkrelax = true;
    //  break;
    //case OPTION_NO_LINKRELAX:
    //  snowhousecpu_opt.linkrelax = false;
    //  break;
    default:
      return 0;
  }
  //return 1;
  //return 0;
}

/* Blank, as the are no machine-dependent options */
void
md_show_usage (FILE *stream ATTRIBUTE_UNUSED)
{
  //fprintf (stream,
  //  _("SnowHouseCpu options:\n"
  //    "There are no SnowHouseCpu-specific options.\n"
  //    //"-mrelax      perform relaxing in GAS, and generate relocations for linker relaxation   (same effect as having both -mgasrelax and -mlinkrelax) (default not enabled).\n"
  //    //"-mno-relax   don't perform relaxing in GAS, and don't generate relocations for linker relaxation (same effect as having both -mno-gasrelax and -mno-linkrelax) (default not enabled).\n"
  //    //"-mgasrelax      perform relaxing in GAS (default not enabled).\n"
  //    //"-mno-gasrelax   don't perform relaxing in GAS (default not enabled).\n"
  //    //"-mlinkrelax      generate relocations for linker relaxation (default not enabled).\n"
  //    //"-mno-linkrelax   don't generate relocations for linker relaxation (default not enabled).\n"
  //  )
  //);
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
  const snowhousecpu_opc_info_t **opc_info;
  const snowhousecpu_reg_t *reg;
  //char cbuf[SNOWHOUSECPU_HTAB_KEY_BUF_LIM];
  //snowhousecpu_opci_v2d_t *opci_list;
  /* -------- */
  // old: /* This means we should always do linker relaxing */
  //linkrelax = true;
  //linkrelax = snowhousecpu_opt.linkrelax;

  // Note: even with `linkrelax` set to `false`, linker relaxing can be
  // enabled with this linker option: `--relax`.
  linkrelax = false;
  /* -------- */
  snowhousecpu_opci_v2d_index_hash = str_htab_create ();
  snowhousecpu_opci_v2d_create (&snowhousecpu_opci_v2d);
  for (size_t i=0; i<SNOWHOUSECPU_MAX_UNIQUE_OPCI_NAMES; ++i)
  {
    snowhousecpu_opci_v2d_index_arr[i] = i;
  }
  //snowhousecpu_opci_v2d_vec_create ();

  //snowhousecpu_opci_v2d_hash = str_htab_create ();
  //snowhousecpu_opci_v2d_end_hash = str_htab_create ();
  //snowhousecpu_op_oa_hash = str_htab_create ();
  snowhousecpu_reg_hash = str_htab_create ();
  /* -------- */
  snowhousecpu_cl_insn_vec_create ();
  /* -------- */
  /* Insert `const snowhousecpu_opc_info_t *`s.  */
  for (count=0, opc_info=snowhousecpu_opc_info_add_arr;
    count++<snowhousecpu_opc_info_add_arr_size;
    //opc_info!=NULL;
    ++opc_info)
  {
    snowhousecpu_opci_v2d_and_index_hash_append (*opc_info/*,*/ /*false,*/ /*0*/);
    //snowhousecpu_opci_v2d_and_index_hash_append (opc_info, true, 0);
  }
  for (count=0, opc_info=snowhousecpu_opc_info_sub_arr;
    count++<snowhousecpu_opc_info_sub_arr_size;
    //opc_info!=NULL;
    ++opc_info)
  {
    snowhousecpu_opci_v2d_and_index_hash_append (*opc_info/*,*/ /*false,*/ /*0*/);
    //snowhousecpu_opci_v2d_and_index_hash_append (opc_info, true, 0);
  }
  for (count=0, opc_info=snowhousecpu_opc_info_slt_arr;
    count++<snowhousecpu_opc_info_slt_arr_size;
    //opc_info!=NULL;
    ++opc_info)
  {
    snowhousecpu_opci_v2d_and_index_hash_append (*opc_info/*,*/ /*false,*/ /*0*/);
    //snowhousecpu_opci_v2d_and_index_hash_append (opc_info, true, 0);
  }

  for (count=0, opc_info=snowhousecpu_opc_info_xor_arr;
    count++<snowhousecpu_opc_info_xor_arr_size;
    //opc_info!=NULL;
    ++opc_info)
  {
    snowhousecpu_opci_v2d_and_index_hash_append (*opc_info/*,*/ /*false,*/ /*0*/);
    //snowhousecpu_opci_v2d_and_index_hash_append (opc_info, true, 0);
  }
  for (count=0, opc_info=snowhousecpu_opc_info_or_arr;
    count++<snowhousecpu_opc_info_or_arr_size;
    //opc_info!=NULL;
    ++opc_info)
  {
    snowhousecpu_opci_v2d_and_index_hash_append (*opc_info/*,*/ /*false,*/ /*0*/);
    //snowhousecpu_opci_v2d_and_index_hash_append (opc_info, true, 0);
  }
  for (count=0, opc_info=snowhousecpu_opc_info_and_simm16_arr;
    count++<snowhousecpu_opc_info_and_simm16_arr_size;
    //opc_info!=NULL;
    ++opc_info)
  {
    snowhousecpu_opci_v2d_and_index_hash_append (*opc_info/*,*/ /*false,*/ /*0*/);
    //snowhousecpu_opci_v2d_and_index_hash_append (opc_info, true, 0);
  }
  for (count=0, opc_info=snowhousecpu_opc_info_shift_etc_arr;
    count++<snowhousecpu_opc_info_shift_etc_arr_size;
    //opc_info!=NULL;
    ++opc_info)
  {
    snowhousecpu_opci_v2d_and_index_hash_append (*opc_info/*,*/ /*false,*/ /*0*/);
    //snowhousecpu_opci_v2d_and_index_hash_append (opc_info, true, 0);
  }
  for (count=0, opc_info=snowhousecpu_opc_info_multi_cycle_arr;
    count++<snowhousecpu_opc_info_multi_cycle_arr_size;
    //opc_info!=NULL;
    ++opc_info)
  {
    snowhousecpu_opci_v2d_and_index_hash_append (*opc_info/*,*/ /*false,*/ /*0*/);
    //snowhousecpu_opci_v2d_and_index_hash_append (opc_info, true, 0);
  }
  for (count=0, opc_info=snowhousecpu_opc_info_ldst_arr;
    count++<snowhousecpu_opc_info_ldst_arr_size;
    //opc_info!=NULL;
    ++opc_info)
  {
    snowhousecpu_opci_v2d_and_index_hash_append (*opc_info/*,*/ /*false,*/ /*0*/);
    //snowhousecpu_opci_v2d_and_index_hash_append (opc_info, true, 0);
  }
  for (count=0, opc_info=snowhousecpu_opc_info_branch_etc_arr;
    count++<snowhousecpu_opc_info_branch_etc_arr_size;
    //opc_info!=NULL;
    ++opc_info)
  {
    snowhousecpu_opci_v2d_and_index_hash_append (*opc_info/*,*/ /*false,*/ /*0*/);
    //snowhousecpu_opci_v2d_and_index_hash_append (opc_info, true, 0);
  }


  //for (count=0, opc_info=snowhousecpu_opc_info_g0;
  //  count++<SNOWHOUSECPU_G0_OPC_INFO_LIM;
  //  ++opc_info)
  //{
  //  //str_hash_insert (snowhousecpu_op_oa_hash,
  //  //  snowhousecpu_opc_info_to_htab_key
  //  //    (cbuf, SNOWHOUSECPU_HTAB_KEY_BUF_LIM, opc_info, 0),
  //  //  opc_info, 0);

  //  /* Don't insert `pre` and `lpre`, as they should be inserted
  //    automatically during `md_assemble ()`. */
  //  if (opc_info->grp_info != &snowhousecpu_grp_info_g0_pre
  //    && opc_info->grp_info != &snowhousecpu_grp_info_g0_lpre)
  //  {
  //    //snowhousecpu_opci_v2d_hnv_append (opc_info, false, 0);
  //    //snowhousecpu_opci_v2d_hnv_append (opc_info, true, 0);
  //    snowhousecpu_opci_v2d_and_index_hash_append (opc_info, false, 0);
  //    snowhousecpu_opci_v2d_and_index_hash_append (opc_info, true, 0);
  //  }
  //}

  //for (count=0, opc_info=snowhousecpu_opc_info_g1;
  //  count++<SNOWHOUSECPU_G1_OPC_INFO_LIM;
  //  ++opc_info)
  //{
  //  snowhousecpu_opci_v2d_and_index_hash_append (opc_info, false, 0);
  //  snowhousecpu_opci_v2d_and_index_hash_append (opc_info, true, 0);
  //}
  ////printf ("post g1\n");
  ////snowhousecpu_print_hash_opci_list ("cpy");


  //for (count=0, opc_info=snowhousecpu_opc_info_g2;
  //  count++<SNOWHOUSECPU_G2_OPC_INFO_LIM;
  //  ++opc_info)
  //{
  //  snowhousecpu_opci_v2d_and_index_hash_append (opc_info, false, 0);
  //  snowhousecpu_opci_v2d_and_index_hash_append (opc_info, true, 0);

  //  if (opc_info->opcode != SNOWHOUSECPU_G2_OP_ENUM_CMP_RA_RB
  //    && opc_info->opcode != SNOWHOUSECPU_G2_OP_ENUM_CMPBC_RA_RB)
  //  {
  //    snowhousecpu_opci_v2d_and_index_hash_append (opc_info, false, 1);
  //    snowhousecpu_opci_v2d_and_index_hash_append (opc_info, true, 1);
  //  }
  //}
  ////printf ("post g2\n");
  ////snowhousecpu_print_hash_opci_list ("cpy");

  //for (count=0, opc_info=snowhousecpu_opc_info_g3;
  //  count++<SNOWHOUSECPU_G3_OPC_INFO_LIM;
  //  ++opc_info)
  //{
  //  snowhousecpu_opci_v2d_and_index_hash_append (opc_info, false, 0);
  //  snowhousecpu_opci_v2d_and_index_hash_append (opc_info, true, 0);
  //}

  //for (count=0, opc_info=snowhousecpu_opc_info_g4;
  //  count++<SNOWHOUSECPU_G4_OPC_INFO_LIM;
  //  ++opc_info)
  //{
  //  //if (opc_info->opcode != SNOWHOUSECPU_G4_OP_ENUM_INDEX_RA_RB)
  //  {
  //    snowhousecpu_opci_v2d_and_index_hash_append (opc_info, false, 0);
  //    snowhousecpu_opci_v2d_and_index_hash_append (opc_info, true, 0);
  //  }
  //}

  ////printf ("post g4\n");
  ////snowhousecpu_print_hash_opci_list ("cpy");

  ////for (count=0, opc_info=snowhousecpu_opc_info_g5;
  ////  count++<SNOWHOUSECPU_G5_OPC_INFO_LIM;
  ////  ++opc_info)
  ////{
  ////  snowhousecpu_opci_v2d_and_index_hash_append (opc_info, false, 0);
  ////  snowhousecpu_opci_v2d_and_index_hash_append (opc_info, true, 0);
  ////}

  ////for (count=0, opc_info=snowhousecpu_opc_info_g6;
  ////  count++<SNOWHOUSECPU_G6_OPC_INFO_LIM;
  ////  ++opc_info)
  ////{
  ////  snowhousecpu_opci_v2d_and_index_hash_append (opc_info, false, 0);
  ////  snowhousecpu_opci_v2d_and_index_hash_append (opc_info, true, 0);
  ////}

  //for (count=0, opc_info=snowhousecpu_opc_info_g7_aluopbh;
  //  count++<SNOWHOUSECPU_G7_ALUOPBH_OPC_INFO_LIM;
  //  ++opc_info)
  //{
  //  snowhousecpu_opci_v2d_and_index_hash_append (opc_info, false, 0);
  //  snowhousecpu_opci_v2d_and_index_hash_append (opc_info, true, 0);
  //  snowhousecpu_opci_v2d_and_index_hash_append (opc_info, false, 1);
  //  snowhousecpu_opci_v2d_and_index_hash_append (opc_info, true, 1);
  //}

  //for (count=0, opc_info=snowhousecpu_opc_info_g7_sprldst;
  //  count++<SNOWHOUSECPU_G7_SPRLDST_OPC_INFO_LIM;
  //  ++opc_info)
  //{
  //  snowhousecpu_opci_v2d_and_index_hash_append (opc_info, false, 0);
  //  snowhousecpu_opci_v2d_and_index_hash_append (opc_info, true, 0);
  //}

  //for (count=0, opc_info=snowhousecpu_opc_info_g7_icreload;
  //  count++<SNOWHOUSECPU_G7_ICRELOAD_OPC_INFO_LIM;
  //  ++opc_info)
  //{
  //  snowhousecpu_opci_v2d_and_index_hash_append (opc_info, false, 0);
  //  snowhousecpu_opci_v2d_and_index_hash_append (opc_info, true, 0);
  //}
  //for (count=0, opc_info=snowhousecpu_opc_info_g7_icflush;
  //  count++<SNOWHOUSECPU_G7_ICFLUSH_OPC_INFO_LIM;
  //  ++opc_info)
  //{
  //  snowhousecpu_opci_v2d_and_index_hash_append (opc_info, false, 0);
  //  snowhousecpu_opci_v2d_and_index_hash_append (opc_info, true, 0);
  //}
  /* -------- */
  /* Insert `const snowhousecpu_reg_t *`s */
  for (count=0, reg=gprs; count++<SNOWHOUSECPU_NUM_GPRS; ++reg)
  {
    str_hash_insert (snowhousecpu_reg_hash, reg->name, reg, 0);
  }

  for (count=0, reg=sprs; count++<SNOWHOUSECPU_NUM_SPRS; ++reg)
  {
    str_hash_insert (snowhousecpu_reg_hash, reg->name, reg, 0);
  }

  str_hash_insert (snowhousecpu_reg_hash, reg_pc.name, &reg_pc, 0);
  /* -------- */
  //bfd_set_arch_mach (stdoutput, TARGET_ARCH, 0);
  /* -------- */
}

/* Blindly copied from "tc-riscv.c" */
/* Because the value of .cfi_remember_state may changed after relaxation,
   we insert a fix to relocate it again in link-time.  */
void
snowhousecpu_pre_output_hook (void)
{
  const frchainS *frch;
  segT s;

  /* Save the current segment info.  */
  segT seg = now_seg;
  subsegT subseg = now_subseg;

  for (s=stdoutput->sections; s; s=s->next)
  {
    for (frch=seg_info (s)->frchainP; frch; frch=frch->frch_next)
    {
      fragS *fragP;

      for (fragP=frch->frch_root; fragP; fragP=fragP->fr_next)
      {
        if (fragP->fr_type == rs_cfa)
        {
          expressionS exp;
          expressionS *symval;

          symval = symbol_get_value_expression (fragP->fr_symbol);
          exp.X_op = O_subtract;
          exp.X_add_symbol = symval->X_add_symbol;
          exp.X_add_number = 0;
          exp.X_op_symbol = symval->X_op_symbol;

          /* We must set the segment before creating a fragP after all
            fragP chains have been chained together.  */
          subseg_set (s, frch->frch_subseg);

          fix_new_exp (fragP, (int) fragP->fr_offset, 1, &exp, 0,
            BFD_RELOC_SNOWHOUSECPU_CFA);
        }
      }
    }
  }

  /* Restore the original segment info.  */
  subseg_set (seg, subseg);
}
/* Called just before the assembler exits.  */
void
snowhousecpu_md_end (void)
{
  snowhousecpu_cl_insn_vec_delete ();
  snowhousecpu_opci_v2d_delete_data (&snowhousecpu_opci_v2d);
  htab_delete (snowhousecpu_opci_v2d_index_hash);
  //snowhousecpu_opci_list_vec_delete ();
  //htab_delete (snowhousecpu_opci_list_hash);
  //htab_delete (snowhousecpu_op_oa_hash);
  htab_delete (snowhousecpu_reg_hash);
}

static bool
have_relaxable_temp_insn (fragS *fragP)
{
  snowhousecpu_cl_insn_t *cl_insn;
  //snowhousecpu_temp_t op_value;

  cl_insn = snowhousecpu_cl_insn_vec + fragP->fr_subtype;
  //fprintf (
  //  stderr,
  //  "have_relaxable_temp_insn(): %lx\n",
  //  cl_insn->data
  //);

  if (
    //!cl_insn->no_relax
    !snowhousecpu_cl_insn_no_relax (cl_insn)
    && cl_insn->have_pre != SNOWHOUSECPU_HAVE_PRE_NONE)
  {
    fprintf (
      stderr,
      "have_relaxable_temp_insn (): returning true.\n"
    );
    return true;
    //snowhousecpu_dasm_info_t dasm_info;
    //snowhousecpu_dasm_info_ctor (&dasm_info, NULL);
    ////memset (&dasm_info, 0, sizeof (dasm_info));

    //dasm_info.iword = cl_insn->data;
    //return (
    //  &dasm_info
    //);
    //op_value = snowhousecpu_get_insn_field (
    //  SNOWHOUSECPU_OP_MASK, SNOWHOUSECPU_OP_BITPOS, cl_insn->data
    //);
    //if (op_value < snowhousecpu_opc_info_a2d_size)
    //{
    //  const snowhousecpu_opc_info_t **opc_info_arr = snowhousecpu_opc_info_a2d[op_value];
    //  for (
    //    const snowhousecpu_opc_info_t *opc_info = opc_info_arr[0];
    //    opc_info!= NULL;
    //    ++opc_info
    //  )
    //  {
    //  }
    //}
    //else
    //{
    //  return false;
    //}
    //grp_value = snowhousecpu_get_insn_field_ei
    //  (&snowhousecpu_enc_info_grp_16, cl_insn->data);
    //switch (grp_value)
    //{
    //  //case snowhousecpu_grp_info_g1.grp_value:
    //  //case snowhousecpu_grp_info_g5.grp_value:
    //  //case snowhousecpu_grp_info_g6.grp_value:
    //  //case snowhousecpu_grp_info_g3.grp_value:
    //  case SNOWHOUSECPU_G1_GRP_VALUE:
    //  case SNOWHOUSECPU_G5_GRP_VALUE:
    //  //case SNOWHOUSECPU_G6_GRP_VALUE:
    //  case SNOWHOUSECPU_G3_GRP_VALUE:
    //    return true;
    //  default:
    //    break;
    //}
  }
    fprintf (
      stderr,
      "have_relaxable_temp_insn (): returning false.\n"
    );
  return false;
}

static bool
relax_can_shrink_value (bfd_vma value,
  snowhousecpu_temp_t curr_bitsize, snowhousecpu_temp_t target_bitsize//,
  //bool have_small_imm_unsigned_target
)
{
  snowhousecpu_temp_t
    curr_value_ext = snowhousecpu_sign_extend (value, curr_bitsize),
    target_value_ext
      = (
      snowhousecpu_sign_extend (value, target_bitsize)
      //!have_small_imm_unsigned_target
      //? snowhousecpu_sign_extend (value, target_bitsize)
      //: snowhousecpu_zero_extend (value, target_bitsize)
    );

  return (
    (uint32_t) curr_value_ext
    == (uint32_t) target_value_ext
  );
}

static void
snowhousecpu_relax_insn_ctor (snowhousecpu_relax_insn_t *self,
  snowhousecpu_cl_insn_t *cl_insn)
{
  gas_assert (self != NULL);
  memset (self, 0, sizeof (*self));
  //self->grp_value = snowhousecpu_get_insn_field_ei
  //  (&snowhousecpu_enc_info_grp_16, cl_insn->data);
  // we don't need the ctor for self->dasm_info because we do the memset to zero in this function.
  //snowhousecpu_dasm_info_ctor (&self->dasm_info, NULL);
  //self->have_imm = false;
  self->dasm_info.iword = cl_insn->data;

  snowhousecpu_dasm_info_do_disassemble_worker (&self->dasm_info);
  self->have_imm = (
    //true
    self->dasm_info.have_non_pre_imm
  );
  fprintf (
    stderr,
    "self->have_imm: %u; oparg: %u\n",
    (unsigned) self->have_imm,
    (unsigned) self->dasm_info.opc_info->oparg
  );
  switch (cl_insn->have_pre) {
    case SNOWHOUSECPU_HAVE_PRE_NONE:
    {
      //self->have_imm = self->dasm_info.have_non_pre_relaxable_imm;
    }
      break;
    case SNOWHOUSECPU_HAVE_PRE_PRE:
    {
      self->prefix_insn_bitsize = SNOWHOUSECPU_IMM16_BITSIZE;
      self->insn_bitsize = SNOWHOUSECPU_IMM16_BITSIZE;
      self->target_bitsize = self->insn_bitsize;
    }
      break;
  }
  //switch (self->grp_value)
  //{
  //  case SNOWHOUSECPU_G1_GRP_VALUE:
  //  //case SNOWHOUSECPU_G5_GRP_VALUE:
  //  //case SNOWHOUSECPU_G6_GRP_VALUE:
  //    switch (cl_insn->have_pre)
  //    {
  //      case SNOWHOUSECPU_HAVE_PRE_NONE:
  //        //return false;
  //        //return 2;
  //        //self->length = 2;
  //        // For the sake of not having assembler errors somewhere down the
  //        // line, don't include this `abort ()` call, in case any new
  //        // instructions are added to this instruction group and don't
  //        // have an immediate.
  //        //abort ();
  //        //self->prefix_insn_bitsize = 0;
  //        //self->insn_bitsize = SNOWHOUSECPU_G1_I5_BITSIZE;
  //        //self->target_bitsize = self->insn_bitsize;
  //        break;
  //      case SNOWHOUSECPU_HAVE_PRE_PRE:
  //        self->prefix_insn_bitsize = SNOWHOUSECPU_G0_PRE_S12_BITSIZE;
  //        self->insn_bitsize = SNOWHOUSECPU_G1_I5_BITSIZE;
  //        self->target_bitsize = self->insn_bitsize;
  //        break;
  //      case SNOWHOUSECPU_HAVE_PLP_LPRE:
  //        self->prefix_insn_bitsize
  //          = SNOWHOUSECPU_G0_LPRE_S27_BITSIZE;
  //        self->insn_bitsize = SNOWHOUSECPU_G1_I5_BITSIZE;
  //        self->target_bitsize
  //          = SNOWHOUSECPU_G0_PRE_S12_BITSIZE + self->insn_bitsize;
  //        self->was_lpre = true;
  //        //fprintf (
  //        //  stderr,
  //        //  "snowhousecpu_relax_insn_ctor(): g1 lpre: %lx\n",
  //        //  cl_insn->data
  //        //);
  //        break;
  //    }
  //    self->have_imm = true;
  //    break;
  //  case SNOWHOUSECPU_G3_GRP_VALUE:
  //    switch (cl_insn->have_pre)
  //    {
  //      case SNOWHOUSECPU_HAVE_PRE_NONE:
  //        //return false;
  //        //return 2;
  //        //self->length = 2;
  //        // For the sake of not having assembler errors somewhere down the
  //        // line, don't include this `abort ()` call, in case any new
  //        // instructions are added to this instruction group and don't
  //        // have an immediate.
  //        //abort ();
  //        break;
  //      case SNOWHOUSECPU_HAVE_PRE_PRE:
  //        self->prefix_insn_bitsize = SNOWHOUSECPU_G0_PRE_S12_BITSIZE;
  //        self->insn_bitsize = SNOWHOUSECPU_G3_S9_BITSIZE;
  //        self->target_bitsize = self->insn_bitsize;
  //        ///self->value -= 2;
  //        break;
  //      case SNOWHOUSECPU_HAVE_PLP_LPRE:
  //        self->prefix_insn_bitsize
  //          = SNOWHOUSECPU_G0_LPRE_S23_BITSIZE;
  //        self->insn_bitsize = SNOWHOUSECPU_G3_S9_BITSIZE;
  //        self->target_bitsize
  //          = SNOWHOUSECPU_G0_PRE_S12_BITSIZE + self->insn_bitsize;
  //        //self->value -= 4;
  //        self->was_lpre = true;
  //        break;
  //    }
  //    self->is_pcrel = true;
  //    self->have_imm = true;
  //    break;
  //  case SNOWHOUSECPU_G5_GRP_VALUE:
  //    if (cl_insn->opc_info->grp_info->subgrp
  //      == &snowhousecpu_enc_info_g5_index_ra_simm_subgrp)
  //    {
  //      //printf("relaxing index_ra_simm!\n");
  //      switch (cl_insn->have_pre)
  //      {
  //        case SNOWHOUSECPU_HAVE_PRE_NONE:
  //          break;
  //        case SNOWHOUSECPU_HAVE_PRE_PRE:
  //          self->prefix_insn_bitsize = SNOWHOUSECPU_G0_PRE_S12_BITSIZE;
  //          self->insn_bitsize = SNOWHOUSECPU_G5_INDEX_RA_SIMM_S7_BITSIZE;
  //          self->target_bitsize = self->insn_bitsize;
  //          break;
  //        case SNOWHOUSECPU_HAVE_PLP_LPRE:
  //          self->prefix_insn_bitsize
  //            = SNOWHOUSECPU_G0_LPRE_S25_BITSIZE;
  //          self->insn_bitsize = SNOWHOUSECPU_G5_INDEX_RA_SIMM_S7_BITSIZE;
  //          self->target_bitsize
  //            = SNOWHOUSECPU_G0_PRE_S12_BITSIZE + self->insn_bitsize;
  //          self->was_lpre = true;
  //          break;
  //      }
  //      self->have_imm = true;
  //    }
  //    break;
  //  case SNOWHOUSECPU_G7_GRP_VALUE:
  //    if (cl_insn->opc_info->grp_info->subgrp
  //      == &snowhousecpu_enc_info_g7_icreload_subgrp)
  //    {
  //      switch (cl_insn->have_pre)
  //      {
  //        case SNOWHOUSECPU_HAVE_PRE_NONE:
  //          break;
  //        case SNOWHOUSECPU_HAVE_PRE_PRE:
  //          self->prefix_insn_bitsize = SNOWHOUSECPU_G0_PRE_S12_BITSIZE;
  //          self->insn_bitsize = SNOWHOUSECPU_G7_ICRELOAD_S5_BITSIZE;
  //          self->target_bitsize = self->insn_bitsize;
  //          break;
  //        case SNOWHOUSECPU_HAVE_PLP_LPRE:
  //          self->prefix_insn_bitsize
  //            = SNOWHOUSECPU_G0_LPRE_S27_BITSIZE;
  //          self->insn_bitsize = SNOWHOUSECPU_G7_ICRELOAD_S5_BITSIZE;
  //          self->target_bitsize
  //            = SNOWHOUSECPU_G0_PRE_S12_BITSIZE + self->insn_bitsize;
  //          self->was_lpre = true;
  //          break;
  //      }
  //      self->have_imm = true;
  //    }
  //    break;
  //  default:
  //    //return false;
  //    break;
  //}
  self->curr_bitsize
    = self->prefix_insn_bitsize + self->insn_bitsize;
}
static void
snowhousecpu_relax_temp_ctor (snowhousecpu_relax_temp_t *self,
  fragS *fragP, asection *sec, bool update)
{
  snowhousecpu_cl_insn_t *cl_insn;
  snowhousecpu_relax_insn_t *relax_insn;
  //const char *pfile;
  //unsigned int pline;
  gas_assert (self != NULL);
  memset (self, 0, sizeof (*self));

  cl_insn = self->cl_insn = snowhousecpu_cl_insn_vec + fragP->fr_subtype;
  //fprintf (
  //  stderr,
  //  "snowhousecpu_relax_temp_ctor(): begin %lx %u\n",
  //  cl_insn->data,
  //  (uint32_t) update
  //);
  relax_insn = &cl_insn->relax_insn;
  // We're calling `snowhousecpu_relax_insn_ctor ()` in `append_cl_insn ()`
  //snowhousecpu_relax_insn_ctor
  //  ((relax_insn = &cl_insn->relax_insn),
  //  cl_insn);
  self->length = 8;

  if (
    //(have_expr = expr_symbol_where (fragP->fr_symbol, &pfile, &pline))
    //((cl_insn->fr_symbol = fragP->fr_symbol) == NULL)
    (fragP->fr_symbol == NULL)
    || (
      //fragP->fr_symbol != NULL
      //&&
      S_IS_DEFINED (fragP->fr_symbol)
      //&& S_IS_LOCAL (fragP->fr_symbol)
      //&& S_IS_COMMON (fragP->fr_symbol)
      //&& S_IS_FORWARD_REF (fragP->fr_symbol)
      //&& !S_IS_EXTERNAL (fragP->fr_symbol)
      && !S_IS_WEAK (fragP->fr_symbol)
      //snowhousecpu_relaxable_symbol (fragP->fr_symbol)
      && sec == S_GET_SEGMENT (fragP->fr_symbol)
      && relax_insn->is_pcrel
      // only relax pc-relative symbol references in GAS since I couldn't
      // get this working otherwise
    )
  )
  {
    self->value
      = (fragP->fr_symbol ? S_GET_VALUE (fragP->fr_symbol) : 0)
        + fragP->fr_offset;
    //fprintf (
    //  stderr,
    //  "snowhousecpu_relax_temp_ctor(): main situation: %lx %lx\n",
    //  self->value, fragP->fr_offset
    //);
    //if (relax_insn->grp_value == SNOWHOUSECPU_G3_GRP_VALUE)
    //if (!relax_insn->is_pcrel)
    //{
    //  fprintf (stderr,
    //    "snowhousecpu_relax_temp_ctor (): non-pcrel value: %li\n",
    //    (long)self->value);
    //}
    //else
    if (relax_insn->is_pcrel)
    {
      // TODO: Verify whether subtracting `fragP->fr_fix` is needed for
      // SnowHouseCpu.
      // `fragP->fr_fix` is
      // "the number of characters we know we're going to emit to 
      // the output file".
      self->value -= fragP->fr_address + fragP->fr_fix;
      //self->value -= fragP->fr_address;
      //fprintf (stderr,
      //  "snowhousecpu_relax_temp_ctor (): pcrel value: %li\n",
      //  (long)self->value);
    }
    const offsetT
      shrink_one_unit_dist = have_pre_distance
        (SNOWHOUSECPU_HAVE_PRE_PRE, SNOWHOUSECPU_HAVE_PRE_NONE);
      //shrink_two_units_dist = have_pre_distance
      //  (SNOWHOUSECPU_HAVE_PLP_LPRE, SNOWHOUSECPU_HAVE_PRE_NONE);

    if (relax_can_shrink_value
      (
        !relax_insn->is_pcrel
        ? self->value
        : self->value
          //- 2ll
          - shrink_one_unit_dist,
      //self->value,
      relax_insn->curr_bitsize,
      relax_insn->target_bitsize//,
      /*(!relax_insn->was_lpre && cl_insn->is_small_imm_unsigned)*/)
    )
    {
      fprintf (
        stderr,
        "relax_can_shrink_value: "
        "self->value: %lx\n",
        self->value
      );
      //if ((self->rm_prefix = (
      //  !relax_insn->was_lpre
      //  || relax_can_shrink_value
      //    (
      //      !relax_insn->is_pcrel
      //      ? self->value
      //      : self->value
      //        //- 4ll
      //        - shrink_two_units_dist,
      //    //self->value,
      //    relax_insn->prefix_insn_bitsize,
      //    relax_insn->insn_bitsize,
      //    cl_insn->is_small_imm_unsigned)
      //)))
      self->rm_prefix = true;
      //if ((self->rm_prefix = true))
      //{
        //if (relax_insn->is_pcrel)
        //{
        //  fprintf (stderr,
        //    "snowhousecpu_relax_temp_ctor (): pcrel: shrink by 4: %li\n",
        //    (long)(self->value - shrink_one_unit_dist));
        //}
        self->length
          //= 2
          = have_pre_insn_length (SNOWHOUSECPU_HAVE_PRE_NONE);
        //if (relax_insn->is_pcrel)
        //{
        //  /* I think this doesn't need to be done in a relocation?
        //    We are already relaxing in GAS at this point, so we're already 
        //    decreasing `self->value`. */ 
        //  self->value
        //    //-= 4ull;
        //    -= shrink_two_units_dist;
        //}
      //}
      //else
      //{
      //  //if (relax_insn->is_pcrel)
      //  //{
      //  //  fprintf (stderr,
      //  //    "snowhousecpu_relax_temp_ctor (): pcrel: shrink by 2: %li\n",
      //  //    (long)(self->value - shrink_one_unit_dist));
      //  //}
      //  self->length
      //    //= 4;
      //    = have_pre_insn_length (SNOWHOUSECPU_HAVE_PRE_PRE);
      //  //if (relax_insn->is_pcrel)
      //  //{
      //  //  /* I think this doesn't need to be done in a relocation?
      //  //    We are already relaxing in GAS at this point, so we're already 
      //  //    decreasing `self->value`. */ 
      //  //  self->value
      //  //    //-= 2ull;
      //  //    -= shrink_one_unit_dist;
      //  //}
      //}
    }
    else
    {
      //if (relax_insn->is_pcrel)
      //{
      //  fprintf (stderr,
      //    "snowhousecpu_relax_temp_ctor (): pcrel: no shrink: %li\n",
      //    (long)(self->value - shrink_one_unit_dist));
      //}
      self->length
        //= 4;
        = have_pre_insn_length (SNOWHOUSECPU_HAVE_PRE_PRE);
    }
    if (update)
    {
      //if (self->length < have_pre_insn_length (SNOWHOUSECPU_HAVE_PLP_LPRE))
      {
        //fprintf (stderr,
        //  "testificate\n");
        //if (relax_insn->was_lpre)
        //{
        //  if (self->rm_prefix) // remove the `lpre` instruction
        //  {
        //    //const offsetT
        //    //  shrink_two_units_dist = have_pre_distance
        //    //    (SNOWHOUSECPU_HAVE_PLP_LPRE, SNOWHOUSECPU_HAVE_PRE_NONE);
        //    cl_insn->have_pre = SNOWHOUSECPU_HAVE_PRE_NONE;
        //    cl_insn->data &= SNOWHOUSECPU_NO_EXT_MASK;
        //    if (!relax_insn->is_pcrel)
        //    {
        //      (void) snowhousecpu_set_insn_field_ei_p
        //        (
        //          (
        //            (
        //              snowhousecpu_get_insn_field_ei(
        //                &snowhousecpu_enc_info_grp_16, cl_insn->data 
        //              ) != SNOWHOUSECPU_G5_GRP_VALUE
        //            )
        //            ? &snowhousecpu_enc_info_g1_i5
        //            : &snowhousecpu_enc_info_g5_index_ra_simm_s7
        //          ),
        //        &cl_insn->data,
        //        self->value);
        //    }
        //    else // if (relax_insn->is_pcrel)
        //    {
        //      //fprintf (stderr,
        //      //  "snowhousecpu_relax_temp_ctor (): pcrel: lpre rm_prefix: "
        //      //    "%li\n",
        //      //  self->value);
        //      (void) snowhousecpu_set_insn_field_ei_p
        //        (&snowhousecpu_enc_info_g3_s9, 
        //        &cl_insn->data,
        //        self->value);
        //    }
        //  }
        //  else // if (!self->rm_prefix) // convert the `lpre` to a `pre`
        //  {
        //    snowhousecpu_temp_t
        //      insn = (cl_insn->data & SNOWHOUSECPU_NO_EXT_MASK),
        //      prefix_insn = snowhousecpu_enc_temp_insn_pre (0x0);
        //    cl_insn->have_pre = SNOWHOUSECPU_HAVE_PRE_PRE;
        //    if (!relax_insn->is_pcrel)
        //    {
        //      //snowhousecpu_put_g1_s17 (&prefix_insn, &insn, self->value);
        //      if (snowhousecpu_get_insn_field_ei
        //        (&snowhousecpu_enc_info_grp_16, cl_insn->data)
        //        != SNOWHOUSECPU_G5_GRP_VALUE)
        //      {
        //        snowhousecpu_put_g1_s17 (&prefix_insn, &insn, self->value);
        //      }
        //      else // g1 or g7 icreload
        //      {
        //        snowhousecpu_put_g5_index_s19 (&prefix_insn, &insn, self->value);
        //      }
        //    }
        //    else // if (relax_insn->is_pcrel)
        //    {
        //      //const offsetT
        //      //  shrink_one_unit_dist = have_pre_distance
        //      //    (SNOWHOUSECPU_HAVE_PLP_LPRE, SNOWHOUSECPU_HAVE_PRE_PRE);
        //      snowhousecpu_put_g3_s21 (&prefix_insn, &insn, self->value);
        //    }
        //    cl_insn->data = (prefix_insn << SNOWHOUSECPU_ONE_EXT_BITPOS) | insn;
        //  }
        //}
        //else // if (!self->was_lpre) // remove the `pre` instruction
        if (self->rm_prefix)
        {
          cl_insn->have_pre = SNOWHOUSECPU_HAVE_PRE_NONE;
          cl_insn->data &= SNOWHOUSECPU_NO_EXT_MASK;

          (void) snowhousecpu_set_insn_field_p 
            (SNOWHOUSECPU_IMM16_MASK, SNOWHOUSECPU_IMM16_MASK, &cl_insn->data, self->value);
          //if (!relax_insn->is_pcrel)
          //{

          //  //if (snowhousecpu_get_insn_field_ei
          //  //  (&snowhousecpu_enc_info_grp_16, cl_insn->data)
          //  //  != SNOWHOUSECPU_G5_GRP_VALUE)
          //  //{
          //  //  (void) snowhousecpu_set_insn_field_ei_p
          //  //    (&snowhousecpu_enc_info_g1_i5, &cl_insn->data, self->value);
          //  //}
          //  //else // g1 or g7 icreload
          //  //{
          //  //  (void) snowhousecpu_set_insn_field_ei_p
          //  //    (&snowhousecpu_enc_info_g5_index_ra_simm_s7, &cl_insn->data,
          //  //    self->value);
          //  //}
          //}
          //else
          //{
          //  //const offsetT
          //  //  shrink_one_unit_dist = have_pre_distance
          //  //    (SNOWHOUSECPU_HAVE_PRE_PRE, SNOWHOUSECPU_HAVE_PRE_NONE);
          //  //(void) snowhousecpu_set_insn_field_ei_p
          //  //  (&snowhousecpu_enc_info_g3_s9, &cl_insn->data,
          //  //    self->value);
          //}
        }
      }
      //else
      //{
      //  snowhousecpu_temp_t
      //    insn = (cl_insn->data & SNOWHOUSECPU_NO_EXT_MASK),
      //    prefix_insn = snowhousecpu_enc_temp_insn_g0_lpre_s23 (0x0);
      //  cl_insn->have_plp = SNOWHOUSECPU_HAVE_PLP_LPRE;

      //  if (!relax_insn->is_pcrel)
      //  {
      //    if (snowhousecpu_get_insn_field_ei
      //      (&snowhousecpu_enc_info_grp_16, cl_insn->data)
      //      != SNOWHOUSECPU_G5_GRP_VALUE)
      //    {
      //      snowhousecpu_put_g1_s32 (&prefix_insn, &insn, self->value);
      //    }
      //    else // if g1 or g7 icreload
      //    {
      //      snowhousecpu_put_g5_index_s32 (&prefix_insn, &insn, self->value);
      //    }
      //  }
      //  else // if (relax_insn->is_pcrel)
      //  {
      //    snowhousecpu_put_g3_s32 (&prefix_insn, &insn, self->value);
      //  }
      //  cl_insn->data = (prefix_insn << SNOWHOUSECPU_ONE_EXT_BITPOS) | insn;
      //}
    }
  }
  //fprintf (
  //  stderr,
  //  "snowhousecpu_relax_temp_ctor(): end %lx %u\n",
  //  cl_insn->data,
  //  (uint32_t) update
  //);
}
int
md_estimate_size_before_relax (fragS *fragP,
  asection *sec ATTRIBUTE_UNUSED)
{
  return (fragP->fr_var = relaxed_cl_insn_length (fragP, sec, false));
}

int
snowhousecpu_relax_frag (asection *sec, fragS *fragP,
  long stretch ATTRIBUTE_UNUSED)
{
  if (have_relaxable_temp_insn (fragP)) {
    offsetT old_var = fragP->fr_var;
    fragP->fr_var = relaxed_cl_insn_length (fragP, sec, true);
    fprintf (
      stderr,
      "snowhousecpu_relax_frag(): %u %u; %u\n",
      (unsigned) old_var,
      (unsigned) fragP->fr_var,
      (unsigned) (fragP->fr_var - old_var)
    );
    return fragP->fr_var - old_var;
  }

  return 0;
}

/* Convert a machine dependent frag. */
void
md_convert_frag (bfd *abfd ATTRIBUTE_UNUSED,
  segT asec ATTRIBUTE_UNUSED, fragS *fragP ATTRIBUTE_UNUSED)
{
  bfd_byte *buf;
  expressionS exp;
  fixS *fixP;
  snowhousecpu_relax_temp_t relax_temp;
  snowhousecpu_relax_insn_t *relax_insn;
  snowhousecpu_cl_insn_t *cl_insn;
  unsigned whole_insn_length;
  //int reloc;
  bfd_reloc_code_real_type *reloc;

  // TODO: Not sure whether I need to add `fragP->fr_fix`??
  // `fragP->fr_fix` is
  // "the number of characters we know we're going to emit to 
  // the output file".
  buf = (bfd_byte *)fragP->fr_literal + fragP->fr_fix;
  //buf = (bfd_byte *)fragP->fr_literal;

  snowhousecpu_relax_temp_ctor (&relax_temp, fragP, asec, true);
  cl_insn = relax_temp.cl_insn;
  relax_insn = &cl_insn->relax_insn;
  reloc = &cl_insn->reloc;

  /* Idea borrowed from "tc-arm.c": `md_convert_frag ()` */
  if (fragP->fr_symbol == NULL)
  {
    exp.X_op = O_constant;
    //exp.X_unsigned = 0;
  }
  else /* if (fragP->fr_symbol != NULL) */
  {
    exp.X_op = O_symbol;
    exp.X_add_symbol = fragP->fr_symbol;
  }
  exp.X_add_number = fragP->fr_offset;

  //fprintf (stderr, "md_convert_frag (): %li %i %u\n",
  //  fragP->fr_var,
  //  have_plp_insn_length (cl_insn->have_plp),
  //  relax_temp.length);
  if (fragP->fr_var != have_pre_insn_length (cl_insn->have_pre))
  {
    fprintf (
      stderr,
      "fragP->fr_var: %u !=: %u\n",
      (unsigned) fragP->fr_var,
      have_pre_insn_length (cl_insn->have_pre)
    );
    gas_assert (fragP->fr_var == have_pre_insn_length (cl_insn->have_pre));
  }
  //const bool have_g5_index_ra_simm = (
  //  //*reloc == BFD_RELOC_SNOWHOUSECPU_G5_INDEX_S7
  //  //|| *reloc == BFD_RELOC_SNOWHOUSECPU_G5_INDEX_S19
  //  //|| *reloc == BFD_RELOC_SNOWHOUSECPU_G5_INDEX_S32
  //  //|| *reloc == BFD_RELOC_SNOWHOUSECPU_G5_INDEX_S32_ADD32
  //  //|| *reloc == BFD_RELOC_SNOWHOUSECPU_G5_INDEX_S32_SUB32
  //  //|| *reloc == BFD_RELOC_SNOWHOUSECPU_G5_INDEX_S32_NO_RELAX
  //  false
  //);
  //printf("have_g5_index_ra_simm: %x\n", have_g5_index_ra_simm);

  switch (*reloc)
  {
    //case BFD_RELOC_SNOWHOUSECPU_G1_U5:
    //case BFD_RELOC_SNOWHOUSECPU_G1_S5:
    //case BFD_RELOC_SNOWHOUSECPU_G3_S9_PCREL:
    //case BFD_RELOC_SNOWHOUSECPU_G5_INDEX_S7:
    //case BFD_RELOC_SNOWHOUSECPU_G7_ICRELOAD_S5:
    case BFD_RELOC_SNOWHOUSECPU_S16:
    case BFD_RELOC_SNOWHOUSECPU_SHIFT_U5:
    case BFD_RELOC_SNOWHOUSECPU_S16_PCREL:
      if (fragP->fr_var == have_pre_insn_length (SNOWHOUSECPU_HAVE_PRE_NONE))
      {
        whole_insn_length = fragP->fr_var;
        fixP = fix_new_exp (fragP, buf - (bfd_byte *) fragP->fr_literal,
          //(!relax_insn->is_pcrel ? 1 : 2),
          (*reloc == BFD_RELOC_SNOWHOUSECPU_SHIFT_U5 ? 1 : 2),
          ////4,
          &exp, (int) relax_insn->is_pcrel, *reloc);
        //fprintf (stderr,
        //  "md_convert_frag (): s5 or s9: %lx\n",
        //  fixP->fx_offset);
      }
      else
      {
        abort ();
      }
      break;

    //case BFD_RELOC_SNOWHOUSECPU_G1_S17_FOR_U5:
    //case BFD_RELOC_SNOWHOUSECPU_G1_S17:
    //case BFD_RELOC_SNOWHOUSECPU_G3_S21_PCREL:
    //case BFD_RELOC_SNOWHOUSECPU_G5_INDEX_S19:
    //case BFD_RELOC_SNOWHOUSECPU_G7_ICRELOAD_S17:
    case BFD_RELOC_SNOWHOUSECPU_S32_FOR_S16:
    case BFD_RELOC_SNOWHOUSECPU_S32_FOR_S16_PCREL:
      if (fragP->fr_var == have_pre_insn_length (SNOWHOUSECPU_HAVE_PRE_NONE))
      {
        // convert `pre` to having no prefix at all
        //whole_insn_length = fragP->fr_var;
        const unsigned
          old_insn_dist = snowhousecpu_have_pre_distance
            (SNOWHOUSECPU_HAVE_PRE_PRE, SNOWHOUSECPU_HAVE_PRE_NONE),
          n_insn_dist = 0;

        const snowhousecpu_temp_t
          insn = bfd_getl32 (buf + old_insn_dist);
        bfd_putl32 (insn, buf + n_insn_dist);

        whole_insn_length = fragP->fr_var;
        if ((*reloc) == BFD_RELOC_SNOWHOUSECPU_S32_FOR_S16)
        {
          *reloc
            //= !relax_insn->is_pcrel
            //? (
            //  !cl_insn->is_g7_icreload
            //  ? (
            //    !cl_insn->is_small_imm_unsigned
            //    ? BFD_RELOC_SNOWHOUSECPU_G1_S5
            //    : BFD_RELOC_SNOWHOUSECPU_G1_U5
            //  ) : BFD_RELOC_SNOWHOUSECPU_G7_ICRELOAD_S17
            //) : BFD_RELOC_SNOWHOUSECPU_G3_S9_PCREL;
            //= snowhousecpu_relax_reloc_lookup
            //  (relax_insn->is_pcrel, have_g5_index_ra_simm,
            //  cl_insn->is_g7_icreload,
            //  cl_insn->is_small_imm_unsigned)->small.reloc;
            = BFD_RELOC_SNOWHOUSECPU_S16;
        }
        else
        {
          *reloc = BFD_RELOC_SNOWHOUSECPU_S16_PCREL;
        }
        fixP = fix_new_exp (fragP, buf - (bfd_byte *) fragP->fr_literal,
          //(!relax_insn->is_pcrel ? 1 : 2),
          2,
          //4,
          &exp, (int) relax_insn->is_pcrel, *reloc);
      }
      else if (
        fragP->fr_var == have_pre_insn_length (SNOWHOUSECPU_HAVE_PRE_PRE)
      )
      {
        whole_insn_length = fragP->fr_var;
        fixP = fix_new_exp (fragP, buf - (bfd_byte *) fragP->fr_literal,
          4, &exp, (int) relax_insn->is_pcrel, *reloc);
      }
      else
      {
        abort ();
      }
      break;
    //case BFD_RELOC_SNOWHOUSECPU_G1_S32_FOR_U5:
    //case BFD_RELOC_SNOWHOUSECPU_G1_S32:
    //case BFD_RELOC_SNOWHOUSECPU_G3_S32_PCREL:
    //case BFD_RELOC_SNOWHOUSECPU_G5_INDEX_S32:
    //case BFD_RELOC_SNOWHOUSECPU_G7_ICRELOAD_S32:
    //  if (fragP->fr_var == have_pre_insn_length (SNOWHOUSECPU_HAVE_PRE_NONE))
    //  {
    //    // convert `lpre` to having no prefix at all
    //    const unsigned
    //      old_insn_dist = snowhousecpu_have_pre_distance
    //        (SNOWHOUSECPU_HAVE_PLP_LPRE, SNOWHOUSECPU_HAVE_PRE_NONE),
    //      n_insn_dist = 0;

    //    const snowhousecpu_temp_t
    //      insn = bfd_getl32 (buf + old_insn_dist);
    //    bfd_putl32 (insn, buf + n_insn_dist);

    //    whole_insn_length = fragP->fr_var;
    //    *reloc
    //      //!relax_insn->is_pcrel
    //      //? (
    //      //  !cl_insn->is_g7_icreload
    //      //  ? (
    //      //    !cl_insn->is_small_imm_unsigned
    //      //    ? BFD_RELOC_SNOWHOUSECPU_G1_S5
    //      //    : BFD_RELOC_SNOWHOUSECPU_G1_U5
    //      //  ) : BFD_RELOC_SNOWHOUSECPU_G7_ICRELOAD_S5
    //      //) : BFD_RELOC_SNOWHOUSECPU_G3_S9_PCREL;
    //      = snowhousecpu_relax_reloc_lookup
    //        (relax_insn->is_pcrel, have_g5_index_ra_simm,
    //        cl_insn->is_g7_icreload,
    //        cl_insn->is_small_imm_unsigned)->small.reloc;
    //    fixP = fix_new_exp (fragP, buf - (bfd_byte *) fragP->fr_literal,
    //      (!relax_insn->is_pcrel ? 1 : 2),
    //      //4,
    //      &exp, (int) relax_insn->is_pcrel, *reloc);
    //  }
    //  else if (
    //    fragP->fr_var == have_pre_insn_length (SNOWHOUSECPU_HAVE_PRE_PRE)
    //  )
    //  {
    //    // convert `lpre` to `pre`
    //    const unsigned
    //      old_insn_dist = have_pre_distance
    //        (SNOWHOUSECPU_HAVE_PLP_LPRE, SNOWHOUSECPU_HAVE_PRE_NONE),
    //      //old_pre_dist = have_pre_distance
    //      //  (SNOWHOUSECPU_HAVE_PLP_LPRE, SNOWHOUSECPU_HAVE_PRE_PRE),
    //      //old_lpre_dist = 0,
    //      n_pre_dist = 0,
    //      n_insn_dist = have_pre_distance
    //        (SNOWHOUSECPU_HAVE_PLP_LPRE, SNOWHOUSECPU_HAVE_PRE_PRE) + n_pre_dist;
    //    const snowhousecpu_temp_t
    //      insn = bfd_getl32 (buf + old_insn_dist),
    //      //old_lpre_insn = bfd_getb64 (buf),
    //      //simm = !relax_insn->is_pcrel
    //      //  ? snowhousecpu_get_g1_s32 (old_lpre_insn, insn)
    //      //  : snowhousecpu_get_g3_s32 (old_lpre_insn, insn),
    //      prefix_insn = snowhousecpu_enc_temp_insn_pre (0x0);

    //    // Turns out we didn't need to set the immediate fields here,
    //    // unlike with `snowhousecpu_do_relax_prefix_innards`
    //    // in "../../bfd/elf32-snowhousecpu.c"
    //    //if (!relax_insn->is_pcrel)
    //    //{
    //    //  snowhousecpu_put_g1_s17 (&prefix_insn, &insn, simm);
    //    //}
    //    //else /* if (relax_insn->is_pcrel) */
    //    //{
    //    //  snowhousecpu_put_g3_s21 (&prefix_insn, &insn, simm - old_pre_dist);
    //    //}

    //    bfd_putl32 (prefix_insn, buf + n_pre_dist);
    //    bfd_putl32 (insn, buf + n_insn_dist);

    //    whole_insn_length = fragP->fr_var;
    //    *reloc
    //      //= !relax_insn->is_pcrel
    //      //? (
    //      //  !cl_insn->is_g7_icreload
    //      //  ? (
    //      //    !cl_insn->is_small_imm_unsigned
    //      //    ? BFD_RELOC_SNOWHOUSECPU_G1_S17
    //      //    : BFD_RELOC_SNOWHOUSECPU_G1_S17
    //      //  ) : BFD_RELOC_SNOWHOUSECPU_G7_ICRELOAD_S17
    //      //) : BFD_RELOC_SNOWHOUSECPU_G3_S21_PCREL;
    //      = snowhousecpu_relax_reloc_lookup
    //        (relax_insn->is_pcrel, have_g5_index_ra_simm,
    //        cl_insn->is_g7_icreload,
    //        cl_insn->is_small_imm_unsigned)->pre.reloc;
    //    fixP = fix_new_exp (fragP, buf - (bfd_byte *) fragP->fr_literal,
    //      4, &exp, (int) relax_insn->is_pcrel, *reloc);
    //  }
    //  else if (
    //    fragP->fr_var == have_plp_insn_length (SNOWHOUSECPU_HAVE_PLP_LPRE)
    //  )
    //  {
    //    whole_insn_length = fragP->fr_var;
    //    fixP = fix_new_exp (fragP, buf - (bfd_byte *) fragP->fr_literal,
    //      4, &exp, (int) relax_insn->is_pcrel, *reloc);
    //  }
    //  else
    //  {
    //    abort ();
    //  }
    //  break;

    //case BFD_RELOC_SNOWHOUSECPU_G1_S32_FOR_U5_NO_RELAX:
    //case BFD_RELOC_SNOWHOUSECPU_G1_S32_NO_RELAX:
    //case BFD_RELOC_SNOWHOUSECPU_G3_S32_PCREL_NO_RELAX:
    //case BFD_RELOC_SNOWHOUSECPU_G7_ICRELOAD_S32_NO_RELAX:
    //case BFD_RELOC_SNOWHOUSECPU_G5_INDEX_S32_NO_RELAX:
    //  if (fragP->fr_var == have_plp_insn_length (SNOWHOUSECPU_HAVE_PLP_LPRE))
    //  {
    //    whole_insn_length = fragP->fr_var;
    //    fixP = fix_new_exp (fragP, buf - (bfd_byte *) fragP->fr_literal,
    //      4, &exp, (int) relax_insn->is_pcrel, *reloc);
    //    //fprintf (
    //    //  stderr,
    //    //  "NO_RELAX: %x\n",
    //    //  *(uint32_t*)(buf - (bfd_byte *) fragP->fr_literal)
    //    //);
    //  }
    //  else
    //  {
    //    abort ();
    //  }
    //  break;

    default:
    {
      reloc_howto_type *howto;
      /* We ignore generic BFD relocations we don't know about. */
      if ((howto = bfd_reloc_type_lookup (stdoutput, *reloc)) != NULL)
      {
        as_fatal (_("internal: bad relocation \"%s\" #%d"),
          howto->name, *reloc);
      }
    }
      abort ();
      break;
  }

  //fprintf (
  //  stderr,
  //  "md_convert_frag(): %lx\n",
  //  *(snowhousecpu_temp_t *)(buf)
  //);

  buf += whole_insn_length;

  fixP->fx_file = fragP->fr_file;
  fixP->fx_line = fragP->fr_line;
  gas_assert (buf == (bfd_byte *)fragP->fr_literal
    + fragP->fr_fix + fragP->fr_var);
  fragP->fr_fix += fragP->fr_var;
}


//static bool
//snowhousecpu_parse_insn (char *s, snowhousecpu_parse_info_t *parse_info)
//{
//}

static void
snowhousecpu_assemble_post_parse_worker (snowhousecpu_parse_data_t *pd,
  bool which_exp)
{
  snowhousecpu_cl_insn_t cl_insn;
  expressionS *temp_ex;
  //bfd_reloc_code_real_type reloc;

  memset (&cl_insn, 0, sizeof (cl_insn));
  //cl_insn.no_relax = pd->no_relax;
  //cl_insn.is_small_imm_unsigned = pd->is_small_imm_unsigned;
  //cl_insn.is_g7_icreload = pd->is_g7_icreload;
  cl_insn.opc_info = pd->opc_info;
  if (!pd->have_imm)
  {
    //pd->nbytes = 2;
    //pd->p = frag_more (2);
    cl_insn.have_pre = SNOWHOUSECPU_HAVE_PRE_NONE;
    temp_ex = NULL;
    cl_insn.reloc = BFD_RELOC_UNUSED;
  }
  else // if (pd->have_imm)
  {
    //printf (
    //  "pd->have_imm: %lx\n",
    //  pd->simm
    //);
    //cl_insn.have_plp = SNOWHOUSECPU_HAVE_PLP_LPRE;
    cl_insn.have_pre = SNOWHOUSECPU_HAVE_PRE_PRE;
    temp_ex = (!which_exp ? &pd->ex : &pd->ex_1);
    //--------
    //const struct snowhousecpu_relax_reloc_tuple
    //  *relax_reloc = snowhousecpu_relax_reloc_lookup
    //    (pd->is_pcrel, pd->have_index_ra_simm,
    //    pd->is_g7_icreload, pd->is_small_imm_unsigned);
    //cl_insn.reloc
    //  = !pd->no_relax
    //  ? relax_reloc->lpre.reloc
    //  : relax_reloc->lpre_no_relax.reloc;
    cl_insn.reloc = (
      pd->opc_info->oparg != SNOWHOUSECPU_OA_RA_RB_SHIFT_U5
      ? (
        !pd->is_pcrel
        ? BFD_RELOC_SNOWHOUSECPU_S32_FOR_S16
        : BFD_RELOC_SNOWHOUSECPU_S32_FOR_S16_PCREL
      ) : BFD_RELOC_SNOWHOUSECPU_SHIFT_U5
    );
    //--------

    //printf (
    //  "cl_insn.reloc: %s\n",
    //  bfd_reloc_type_lookup (stdoutput, cl_insn.reloc)->name
    //);

    //pd->p = frag_more (pd->nbytes);

    ////if (!pd->have_cpy64)
    ////if (pd->nbytes == 6)
    //{
    //  fixS *fixP;

    //  //fprintf (stderr, "testificate: %d\n", (unsigned) pd->no_relax);
    //  //fprintf (stderr, "howto: %d %s\n",
    //  //  (unsigned) pd->no_relax,
    //  //  bfd_reloc_type_lookup (stdoutput, reloc)->name);

    //  fixP = fix_new_exp (frag_now, /* frag */
    //                      (pd->p - frag_now->fr_literal), /* where */
    //                      4, /* size */
    //                      //6,
    //                      temp_ex, /* exp */
    //                      (int) pd->is_pcrel, /* pcrel */
    //                      reloc /* reloc */
    //                      );
    //  //snowhousecpu_fix_sym_handler (fixP);
    //}

    pd->prefix_insn = snowhousecpu_enc_temp_insn_pre_rshift (
      //pd->opc_info,
      &snowhousecpu_opc_info_pre_simm16,
      pd->simm);
  }
  const snowhousecpu_temp_t
    my_reg_a = pd->reg_a != NULL ? pd->reg_a->index : 0x0,
    my_reg_b = pd->reg_b != NULL ? pd->reg_b->index : 0x0,
    my_reg_c = pd->reg_c != NULL ? pd->reg_c->index : 0x0;
  //snowhousecpu_temp_t 
  //  my_reg_0 = 0x0u,
  //  my_reg_1 = 0x0u; 
  //switch (pd->have_index_2reg_kind)
  //{
  //  case SNOWHOUSECPU_HIDX2R_KIND_NO_INDEX:
  //  {
  //    my_reg_0 = my_reg_a;
  //    my_reg_1 = my_reg_b;
  //  }
  //    break;
  //  case SNOWHOUSECPU_HIDX2R_KIND_ATOMIC:
  //  {
  //    my_reg_0 = my_reg_a;
  //    my_reg_1 = my_reg_b;
  //  }
  //    break;
  //  case SNOWHOUSECPU_HIDX2R_KIND_LDST:
  //  {
  //    my_reg_0 = my_reg_a;
  //  }
  //    break;
  //  //case SNOWHOUSECPU_HIDX2R_KIND_DIVMOD:
  //  //{
  //  //  my_reg_0 = my_reg_a;
  //  //}
  //  //  break;
  //  //case SNOWHOUSECPU_HIDX2R_KIND_LMUL:
  //  //{
  //  //  my_reg_0 = my_reg_a;
  //  //  my_reg_1 = my_reg_b;
  //  //}
  //  //  break;
  //  //case SNOWHOUSECPU_HIDX2R_KIND_DIVMOD64:
  //  //{
  //  //  my_reg_0 = my_reg_a;
  //  //  my_reg_1 = my_reg_b;
  //  //}
  //  //  break;
  //  case SNOWHOUSECPU_HIDX2R_KIND_JUSTADDR:
  //  {
  //    my_reg_0 = 0x0ull;
  //    my_reg_1 = 0x0ull;
  //  }
  //    break;
  //}
  //if (pd->have_index_ra_simm)
  //{
  //  cl_insn.opc_info = &snowhousecpu_opc_info_g5_index_ra_simm[0];
  //  //cl_insn.relax_insn.grp_value = cl_insn.opc_info->grp_info->grp_value;
  //  pd->insn = snowhousecpu_enc_temp_insn_non_pre_lpre
  //    (
  //    //pd->opc_info
  //    //&snowhousecpu_opc_info_g5_index_ra_simm[0],
  //    cl_insn.opc_info,
  //    //my_reg_0,
  //    my_reg_1,
  //    0x0u,
  //    pd->simm,
  //    pd->fwl);
  //  cl_insn.data
  //    = (((pd->prefix_insn & 0xffffffffull) << SNOWHOUSECPU_ONE_EXT_BITPOS)
  //      | (pd->insn & 0xffffull));
  //  //printf (
  //  //  "post parse worker index_ra_simm: simm:%lx; data:%lx\n",
  //  //  pd->simm,
  //  //  cl_insn.data
  //  //);
  //  snowhousecpu_relax_insn_ctor (&cl_insn.relax_insn, &cl_insn);
  //  append_cl_insn (&cl_insn, temp_ex);

  //  cl_insn.opc_info = pd->opc_info;

  //  pd->insn = snowhousecpu_enc_temp_insn_non_pre_lpre
  //    (
  //    cl_insn.opc_info,
  //    //&snowhousecpu_opc_info_g5_index_ra_simm[0],
  //    my_reg_0,
  //    //my_reg_1,
  //    0x0,
  //    0x0,
  //    /*pd->fwl*/
  //    0x0
  //    );
  //  cl_insn.data
  //    = (pd->insn & 0xffffull);
  //  //printf (
  //  //  "post parse worker primary insn: %lx\n",
  //  //  cl_insn.data
  //  //);
  //  //snowhousecpu_relax_insn_ctor (&cl_insn.relax_insn, &cl_insn);
  //  cl_insn.relax_insn.have_imm = false;
  //  cl_insn.have_pre = SNOWHOUSECPU_HAVE_PRE_NONE;
  //  append_cl_insn (&cl_insn, temp_ex);
  //}
  //else
  {
    //pd->insn = snowhousecpu_enc_temp_insn_non_pre_lpre
    //  (
    //  pd->opc_info,
    //  //&snowhousecpu_opc_info_g5_index_ra_simm[0],
    //  my_reg_0,
    //  my_reg_1,
    //  pd->simm,
    //  pd->fwl);
    pd->insn = snowhousecpu_enc_temp_insn (
      pd->opc_info->op,
      //&snowhousecpu_opc_info_g5_index_ra_simm[0],
      my_reg_a,
      my_reg_b,
      my_reg_c,
      pd->simm
    );
    //cl_insn.data
    //  = (((pd->prefix_insn & 0xffffffffull) << SNOWHOUSECPU_ONE_EXT_BITPOS)
    //    | (pd->insn & 0xffffull));
    cl_insn.data
      = (((pd->prefix_insn & 0xffffffffull) << SNOWHOUSECPU_ONE_EXT_BITPOS)
        | (pd->insn & 0xffffffffull));
    snowhousecpu_relax_insn_ctor (&cl_insn.relax_insn, &cl_insn);
    append_cl_insn (&cl_insn, temp_ex);
  }
  ////pd->insn = snowhousecpu_enc_temp_insn_non_pre_lpre
  ////  (pd->opc_info,
  ////  (
  ////    (
  ////      pd->have_index 
  ////      && pd->opc_info->grp_info == &snowhousecpu_grp_info_g7_icreload
  ////    ) ? (
  ////      0x0ull
  ////    ) : (pd->reg_a != NULL ? pd->reg_a->index : 0x0)
  ////  ),
  ////  //(pd->reg_b != NULL ? pd->reg_b->index : 0x0),
  ////  (
  ////    (!pd->have_index 
  ////    || pd->opc_info->grp_info == &snowhousecpu_grp_info_g0_atomic)
  ////    ? 
  ////    (pd->reg_b != NULL ? pd->reg_b->index : 0x0ull)
  ////    : 0x0ull
  ////  ),
  ////  pd->simm,
  ////  pd->fwl);
  //cl_insn.data
  //  = (((pd->prefix_insn & 0xffffffffull) << SNOWHOUSECPU_ONE_EXT_BITPOS)
  //    | (pd->insn & 0xffffull));

  ////fprintf
  ////  (stderr,
  ////  "cl_insn.data: %lx\n",
  ////  cl_insn.data
  ////  );
  //snowhousecpu_relax_insn_ctor (&cl_insn.relax_insn, &cl_insn);
  //append_cl_insn (&cl_insn, temp_ex);

  //snowhousecpu_number_to_chars (pd->p,
  //  (((pd->prefix_insn & 0xffffffffull) << SNOWHOUSECPU_ONE_EXT_BITPOS)
  //    | (pd->insn & 0xffffull)),
  //  pd->nbytes);
  //dwarf2_emit_insn (pd->nbytes);

  ///* Per RISC-V: */
  ///* We need to start a new frag after any instruction that can be
  //  optimized away or compressed by the linker during relaxation,
  //  to prevent the assembler from computing static offsets across such an
  //  instruction.
  //  This is necessary to get correct EH info.  */
  //if (pd->have_imm && (pd->nbytes == 4 || pd->nbytes == 6))
  //{
  //  frag_wane (frag_now);
  //  frag_new (0);
  //}
}
/* This is the guts of the machine-dependent assembler.  STR points to
  a machine dependent instruction.  This function is supposed to emit
  the frags/bytes it assembles to.  */
void
md_assemble (char *str)
{
  snowhousecpu_parse_data_t pd;
  char
    *op_start = NULL,
    *op_end = NULL,
    *op_end_prev = NULL,
    pend = '\0',
    op_name[SNOWHOUSECPU_OPC_INFO_NAME_MAX_LEN + 1];
  snowhousecpu_opci_vec_t
    *opci_vec = NULL;
  size_t
    nlen = 0;

  (void) memset (&pd, 0, sizeof (pd));
  (void) memset (op_name, '\0', sizeof (op_name));

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

  //fprintf (stderr, "op_start: %s\n", op_start);

  if (nlen == 0)
  {
    as_bad (_("can't find opcode "));
    return;
  }
  else if (nlen > SNOWHOUSECPU_OPC_INFO_NAME_MAX_LEN)
  {
    as_bad (_("opcode is longer than maximum length of %d"),
      (unsigned) SNOWHOUSECPU_OPC_INFO_NAME_MAX_LEN);
    return;
  }

  for (size_t i=0; i<nlen; ++i)
  {
    /* `op_name` has been set to all '\0's via the earlier `memset ()`
      call, so we don't have to null terminate `op_name`.
      Note that `op_name` has a length of
      `SNOWHOUSECPU_OPC_INFO_NAME_MAX_LEN + 1`. */
    op_name[i] = op_start[i];
  }

  //pd.opci_list = snowhousecpu_opci_list_lookup (op_name);
  opci_vec = snowhousecpu_opci_vec_lookup (op_name);
  *op_end = pend;

  //if (pd.opci_list == NULL)
  if (opci_vec == NULL)
  {
    as_bad (_("unknown opcode %s"), op_name);
    return;
  }

#define SNOWHOUSECPU_SKIP_ISSPACE() \
  do \
  { \
    while (ISSPACE (*op_end)) \
    { \
      ++op_end; \
    } \
  } while (0)
#define SNOWHOUSECPU_PARSE_COMMA()  \
  do \
  { \
    if (*op_end != ',') \
    { \
      goto post_oa_switch; \
    } \
    ++op_end; \
    SNOWHOUSECPU_SKIP_ISSPACE (); \
  } while (0)
#define SNOWHOUSECPU_PARSE_GPR(some_reg) \
  do \
  { \
    pd.some_reg = parse_register_operand (&op_end); \
    if (pd.some_reg == NULL \
      || pd.some_reg->kind != SNOWHOUSECPU_REG_KIND_GPR) \
    { \
      goto post_oa_switch; \
    } \
  } while (0)
#define SNOWHOUSECPU_PARSE_SPR(some_reg) \
  do \
  { \
    pd.some_reg = parse_register_operand (&op_end); \
    if (pd.some_reg == NULL \
      || pd.some_reg->kind != SNOWHOUSECPU_REG_KIND_SPR) \
    { \
      goto post_oa_switch; \
    } \
  } while (0)
#define SNOWHOUSECPU_PARSE_NOENC_REG(name) \
  do \
  { \
    if (parse_register_operand (&op_end) \
      != snowhousecpu_reg_lookup (name)) \
    { \
      goto post_oa_switch; \
    } \
  } while (0)
#define SNOWHOUSECPU_PARSE_PC() \
  do \
  { \
    if (parse_register_operand (&op_end) != &reg_pc) \
    { \
      goto post_oa_switch; \
    } \
  } while (0)

#define SNOWHOUSECPU_PARSE_EXP_POST_POUND_WORKER(some_ex) \
  do \
  { \
    op_end = clear_and_parse_exp_save_ilp (op_end, &pd.some_ex); \
    if (pd.some_ex.X_op == O_illegal || pd.some_ex.X_op == O_absent) \
    { \
      goto post_oa_switch; \
    } \
  } while (0)
#define SNOWHOUSECPU_PARSE_EXP_POST_POUND() \
  do \
  { \
    SNOWHOUSECPU_PARSE_EXP_POST_POUND_WORKER (ex); \
  } while (0)

#define SNOWHOUSECPU_PARSE_EXP_WORKER(some_ex) \
  do \
  { \
    /* if (*op_end != '#') */ \
    /* { */ \
    /*  goto post_oa_switch; */\
    /* } */ \
    /* ++op_end; */ \
    SNOWHOUSECPU_PARSE_EXP_POST_POUND_WORKER (some_ex); \
  } while (0)
#define SNOWHOUSECPU_PARSE_EXP() \
  do \
  { \
    SNOWHOUSECPU_PARSE_EXP_WORKER (ex); \
  } while (0)
#define SNOWHOUSECPU_PARSE_EXP_1() \
  do \
  { \
    SNOWHOUSECPU_PARSE_EXP_WORKER (ex_1); \
  } while (0)

  for (size_t i=0; i<opci_vec->size; ++i)
  {
    //const char * const *some_names;

    (void) memset (&pd, 0, sizeof (pd));
    //pd.opc_info = pd.opci_list->opc_info;
    pd.opc_info = opci_vec->data[i];
    //printf ("snowhousecpu dbg: %s %s %d %d\n",
    //  pd.opc_info->names[0],
    //  pd.opc_info->names[1],
    //  //(unsigned) pd.opc_info->oparg == SNOWHOUSECPU_OA_RA_RB
    //  (unsigned) pd.opc_info->oparg,
    //  (unsigned) pd.opc_info->opcode
    //);
    op_end = op_end_prev;
    pd.is_pcrel = false;
    pd.reg_a = (const snowhousecpu_reg_t *) NULL;
    pd.reg_b = (const snowhousecpu_reg_t *) NULL;
    pd.reg_c = (const snowhousecpu_reg_t *) NULL;
    pd.simm = 0;
    //--------
    //pd.no_relax = !(
    //  (strncmp (pd.opc_info->names[0], op_name,
    //    SNOWHOUSECPU_OPC_INFO_NAME_MAX_LEN) == 0)
    //  || (strncmp (pd.opc_info->names[1], op_name,
    //    SNOWHOUSECPU_OPC_INFO_NAME_MAX_LEN) == 0)
    //);
    //--------
    //fprintf (stderr, "pd.no_relax stuff: \"%s\" %i %i %d\n",
    //  op_name,
    //  strncmp (pd.opc_info->names[0], op_name,
    //    SNOWHOUSECPU_OPC_INFO_NAME_MAX_LEN),
    //  strncmp (pd.opc_info->names[1], op_name,
    //    SNOWHOUSECPU_OPC_INFO_NAME_MAX_LEN),
    //  (unsigned) pd.no_relax);
    //pd.have_index_2reg_kind = SNOWHOUSECPU_HIDX2R_KIND_NO_INDEX;
    //some_names = !pd.no_relax ? pd.opc_info->names : pd.opc_info->nr_names;
    //pd.fwl = (snowhousecpu_temp_t) (
    //  //(
    //  //  pd.opc_info->grp_info == &snowhousecpu_grp_info_g0_atomic
    //  //  && strncmp (some_names[0], op_name,
    //  //    SNOWHOUSECPU_OPC_INFO_NAME_MAX_LEN) 
    //  //) || 
    //  (
    //    pd.opc_info->grp_info == &snowhousecpu_grp_info_g2
    //    && strncmp (some_names[1], op_name,
    //      SNOWHOUSECPU_OPC_INFO_NAME_MAX_LEN) == 0
    //  ) || (
    //    pd.opc_info->grp_info == &snowhousecpu_grp_info_g7_aluopbh
    //    && strncmp (some_names[1], op_name,
    //      SNOWHOUSECPU_OPC_INFO_NAME_MAX_LEN) == 0
    //  )
    //);

    switch (pd.opc_info->oparg)
    {
      //case SNOWHOUSECPU_OA_BAD:
      //{
      //}
      //  break;
      case SNOWHOUSECPU_OA_NONE:
      {
        SNOWHOUSECPU_SKIP_ISSPACE ();

        pd.parse_good = true;
      }
        break;
      //case SNOWHOUSECPU_OA_PRE:
      //{
      //}
      //  break;
      //case SNOWHOUSECPU_OA_LPRE:
      //{
      //}
      //  break;
      //case SNOWHOUSECPU_OA_PRE:
      //  break;
      case SNOWHOUSECPU_OA_RB:
        SNOWHOUSECPU_SKIP_ISSPACE ();
        SNOWHOUSECPU_PARSE_GPR (reg_b);
        //pd.have_imm = false;
        pd.parse_good = true;
        break;
      case SNOWHOUSECPU_OA_RA_RB:
        SNOWHOUSECPU_SKIP_ISSPACE ();
        SNOWHOUSECPU_PARSE_GPR (reg_a);
        SNOWHOUSECPU_PARSE_COMMA ();
        SNOWHOUSECPU_PARSE_GPR (reg_b);
        //pd.have_imm = false;
        pd.parse_good = true;
        break;
      case SNOWHOUSECPU_OA_RA_RB_RC:
        SNOWHOUSECPU_SKIP_ISSPACE ();
        SNOWHOUSECPU_PARSE_GPR (reg_a);
        SNOWHOUSECPU_PARSE_COMMA ();
        SNOWHOUSECPU_PARSE_GPR (reg_b);
        SNOWHOUSECPU_PARSE_COMMA ();
        SNOWHOUSECPU_PARSE_GPR (reg_c);
        pd.parse_good = true;
        break;
      case SNOWHOUSECPU_OA_RA_RB_S16:
        SNOWHOUSECPU_SKIP_ISSPACE ();
        SNOWHOUSECPU_PARSE_GPR (reg_a);
        SNOWHOUSECPU_PARSE_COMMA ();
        SNOWHOUSECPU_PARSE_GPR (reg_b);
        SNOWHOUSECPU_PARSE_COMMA ();
        SNOWHOUSECPU_PARSE_EXP ();
        //fprintf (
        //  stderr,
        //  "test: %s %s, %s\n",
        //  pd.opc_info->name,
        //  pd.reg_a->name,
        //  pd.reg_b->name
        //  //pd.reg_c->name
        //);
        pd.have_imm = true;
        pd.parse_good = true;
        break;
      case SNOWHOUSECPU_OA_RA_RB_SHIFT_U5:
        SNOWHOUSECPU_SKIP_ISSPACE ();
        SNOWHOUSECPU_PARSE_GPR (reg_a);
        SNOWHOUSECPU_PARSE_COMMA ();
        SNOWHOUSECPU_PARSE_GPR (reg_b);
        SNOWHOUSECPU_PARSE_COMMA ();
        SNOWHOUSECPU_PARSE_EXP ();
        pd.have_imm = true;
        pd.parse_good = true;
        break;
      case SNOWHOUSECPU_OA_RA_PCREL_S16:
        SNOWHOUSECPU_SKIP_ISSPACE ();
        SNOWHOUSECPU_PARSE_GPR (reg_a);
        SNOWHOUSECPU_PARSE_COMMA ();
        SNOWHOUSECPU_PARSE_EXP ();
        pd.have_imm = true;
        pd.parse_good = true;
        pd.is_pcrel = true;
        break;
      case SNOWHOUSECPU_OA_RA_RB_PCREL_S16:
        SNOWHOUSECPU_SKIP_ISSPACE ();
        SNOWHOUSECPU_PARSE_GPR (reg_a);
        SNOWHOUSECPU_PARSE_COMMA ();
        SNOWHOUSECPU_PARSE_GPR (reg_b);
        SNOWHOUSECPU_PARSE_COMMA ();
        SNOWHOUSECPU_PARSE_EXP ();
        pd.have_imm = true;
        pd.parse_good = true;
        pd.is_pcrel = true;
        break;
      case SNOWHOUSECPU_OA_RB_RA_PCREL_S16:
        SNOWHOUSECPU_SKIP_ISSPACE ();
        SNOWHOUSECPU_PARSE_GPR (reg_b);
        SNOWHOUSECPU_PARSE_COMMA ();
        SNOWHOUSECPU_PARSE_GPR (reg_a);
        SNOWHOUSECPU_PARSE_COMMA ();
        SNOWHOUSECPU_PARSE_EXP ();
        pd.have_imm = true;
        pd.parse_good = true;
        pd.is_pcrel = true;
        break;
      case SNOWHOUSECPU_OA_PCREL_S16:
        SNOWHOUSECPU_SKIP_ISSPACE ();
        SNOWHOUSECPU_PARSE_EXP ();
        pd.have_imm = true;
        pd.parse_good = true;
        pd.is_pcrel = true;
        break;
      case SNOWHOUSECPU_OA_RA_PC_PCREL_S16:
        SNOWHOUSECPU_SKIP_ISSPACE ();
        SNOWHOUSECPU_PARSE_GPR (reg_a);
        SNOWHOUSECPU_PARSE_COMMA ();
        SNOWHOUSECPU_PARSE_PC ();
        SNOWHOUSECPU_PARSE_COMMA ();
        SNOWHOUSECPU_PARSE_EXP ();
        pd.have_imm = true;
        pd.parse_good = true;
        pd.is_pcrel = true;
        break;
      case SNOWHOUSECPU_OA_IDS_RB:
        SNOWHOUSECPU_SKIP_ISSPACE ();
        SNOWHOUSECPU_PARSE_SPR (reg_a);
        if (pd.reg_a->index != SNOWHOUSECPU_SPR_ENUM_IDS)
        {
          goto post_oa_switch;
        }
        pd.reg_a = NULL;
        SNOWHOUSECPU_PARSE_COMMA ();
        SNOWHOUSECPU_PARSE_GPR (reg_b);
        pd.parse_good = true;
        break;
      case SNOWHOUSECPU_OA_RA_IRA:
        SNOWHOUSECPU_SKIP_ISSPACE ();
        SNOWHOUSECPU_PARSE_GPR (reg_a);
        SNOWHOUSECPU_PARSE_COMMA ();
        SNOWHOUSECPU_PARSE_SPR (reg_b);
        if (pd.reg_b->index != SNOWHOUSECPU_SPR_ENUM_IRA)
        {
          goto post_oa_switch;
        }
        pd.reg_b = NULL;
        pd.parse_good = true;
        break;
      case SNOWHOUSECPU_OA_IE_RB:
        SNOWHOUSECPU_SKIP_ISSPACE ();
        SNOWHOUSECPU_PARSE_SPR (reg_a);
        if (pd.reg_a->index != SNOWHOUSECPU_SPR_ENUM_IE)
        {
          goto post_oa_switch;
        }
        pd.reg_a = NULL;
        SNOWHOUSECPU_PARSE_COMMA ();
        SNOWHOUSECPU_PARSE_GPR (reg_b);
        pd.parse_good = true;
        break;
      case SNOWHOUSECPU_OA_IRA:
        SNOWHOUSECPU_SKIP_ISSPACE ();
        SNOWHOUSECPU_PARSE_SPR (reg_a);
        if (pd.reg_a->index != SNOWHOUSECPU_SPR_ENUM_IRA)
        {
          goto post_oa_switch;
        }
        pd.reg_a = NULL;
        pd.parse_good = true;
        break;
      case SNOWHOUSECPU_OA_SIMM16:
        SNOWHOUSECPU_SKIP_ISSPACE ();
        SNOWHOUSECPU_PARSE_EXP ();
        pd.have_imm = true;
        pd.parse_good = true;
        break;
      //case SNOWHOUSECPU_OA_RA_RB_XCHG:
      //{
      //  SNOWHOUSECPU_SKIP_ISSPACE ();

      //  SNOWHOUSECPU_PARSE_GPR (reg_a);
      //  SNOWHOUSECPU_PARSE_COMMA ();
      //  SNOWHOUSECPU_PARSE_GPR (reg_b);

      //  pd.parse_good = true;
      //}
      //  break;
      //case SNOWHOUSECPU_OA_RA_RB_XCHG_LOCK:
      //{
      //  SNOWHOUSECPU_SKIP_ISSPACE ();

      //  if (*op_end != '[')
      //  {
      //    break;
      //  }
      //  ++op_end;

      //  SNOWHOUSECPU_PARSE_GPR (reg_a);
      //  if (*op_end != ']')
      //  {
      //    break;
      //  }
      //  ++op_end;

      //  SNOWHOUSECPU_PARSE_COMMA ();
      //  SNOWHOUSECPU_PARSE_GPR (reg_b);
      //  pd.parse_good = true;
      //  pd.fwl = true;
      //}
      //  break;
      //case SNOWHOUSECPU_OA_RA_RC_RB_CMPXCHG:
      //{
      //  SNOWHOUSECPU_SKIP_ISSPACE ();

      //  if (*op_end != '[')
      //  {
      //    break;
      //  }
      //  ++op_end;

      //  SNOWHOUSECPU_PARSE_GPR (reg_a);
      //  if (*op_end != ']')
      //  {
      //    break;
      //  }
      //  ++op_end;

      //  SNOWHOUSECPU_PARSE_COMMA ();

      //  SNOWHOUSECPU_PARSE_GPR (reg_c);

      //  SNOWHOUSECPU_PARSE_COMMA ();
      //  SNOWHOUSECPU_PARSE_GPR (reg_b);

      //  pd.parse_good = true;
      //  //pd.have_index = true;
      //  pd.have_index_2reg_kind = SNOWHOUSECPU_HIDX2R_KIND_ATOMIC; 
      //}
      //case SNOWHOUSECPU_OA_RA_RC_RB_CMPXCHG_LOCK:
      //{
      //  SNOWHOUSECPU_SKIP_ISSPACE ();

      //  if (*op_end != '[')
      //  {
      //    break;
      //  }
      //  ++op_end;

      //  SNOWHOUSECPU_PARSE_GPR (reg_a);
      //  if (*op_end != ']')
      //  {
      //    break;
      //  }
      //  ++op_end;

      //  SNOWHOUSECPU_PARSE_COMMA ();

      //  SNOWHOUSECPU_PARSE_GPR (reg_c);

      //  SNOWHOUSECPU_PARSE_COMMA ();
      //  SNOWHOUSECPU_PARSE_GPR (reg_b);

      //  pd.parse_good = true;
      //  //pd.have_index = true;
      //  pd.have_index_2reg_kind = SNOWHOUSECPU_HIDX2R_KIND_ATOMIC; 
      //  pd.fwl = true;
      //}
      //  break;
      //case SNOWHOUSECPU_OA_RA_S5:
      //{
      //  //char *where;
      //  SNOWHOUSECPU_SKIP_ISSPACE ();

      //  SNOWHOUSECPU_PARSE_GPR (reg_a);
      //  SNOWHOUSECPU_PARSE_COMMA ();

      //  SNOWHOUSECPU_PARSE_EXP ();

      //  pd.parse_good = true;
      //  pd.have_imm = true;
      //}
      //  break;
      //case SNOWHOUSECPU_OA_RA_PC_S5:
      //{
      //  SNOWHOUSECPU_SKIP_ISSPACE ();

      //  SNOWHOUSECPU_PARSE_GPR (reg_a);
      //  SNOWHOUSECPU_PARSE_COMMA ();

      //  SNOWHOUSECPU_PARSE_PC ();
      //  SNOWHOUSECPU_PARSE_COMMA ();

      //  SNOWHOUSECPU_PARSE_EXP ();

      //  pd.parse_good = true;
      //  pd.have_imm = true;
      //}
      //  break;
      //case SNOWHOUSECPU_OA_RA_SP_S5:
      //{
      //  SNOWHOUSECPU_SKIP_ISSPACE ();

      //  SNOWHOUSECPU_PARSE_GPR (reg_a);
      //  SNOWHOUSECPU_PARSE_COMMA ();

      //  SNOWHOUSECPU_PARSE_NOENC_REG ("sp");
      //  SNOWHOUSECPU_PARSE_COMMA ();

      //  SNOWHOUSECPU_PARSE_EXP ();

      //  pd.parse_good = true;
      //  pd.have_imm = true;
      //}
      //  break;
      //case SNOWHOUSECPU_OA_RA_FP_S5:
      //{
      //  SNOWHOUSECPU_SKIP_ISSPACE ();

      //  SNOWHOUSECPU_PARSE_GPR (reg_a);
      //  SNOWHOUSECPU_PARSE_COMMA ();

      //  SNOWHOUSECPU_PARSE_NOENC_REG ("fp");
      //  SNOWHOUSECPU_PARSE_COMMA ();

      //  SNOWHOUSECPU_PARSE_EXP ();

      //  pd.parse_good = true;
      //  pd.have_imm = true;
      //}
      //  break;
      ////case SNOWHOUSECPU_OA_RA_PC_S5:
      ////case SNOWHOUSECPU_OA_RA_SP_S5:
      ////case SNOWHOUSECPU_OA_RA_FP_S5:
      ////{
      ////  SNOWHOUSECPU_SKIP_ISSPACE ();

      ////  SNOWHOUSECPU_PARSE_GPR (reg_a);
      ////  SNOWHOUSECPU_PARSE_COMMA ();

      ////  switch (pd.opc_info->oparg)
      ////  {
      ////    case SNOWHOUSECPU_OA_RA_PC_S5:
      ////      SNOWHOUSECPU_PARSE_PC ();
      ////      break;
      ////    case SNOWHOUSECPU_OA_RA_SP_S5:
      ////      SNOWHOUSECPU_PARSE_NOENC_REG ("sp");
      ////      break;
      ////    case SNOWHOUSECPU_OA_RA_FP_S5:
      ////      SNOWHOUSECPU_PARSE_NOENC_REG ("fp");
      ////      break;
      ////  }

      ////  SNOWHOUSECPU_PARSE_COMMA ();

      ////  SNOWHOUSECPU_PARSE_EXP ();

      ////  pd.parse_good = true;
      ////  pd.have_imm = true;
      ////}
      ////  break;
      //case SNOWHOUSECPU_OA_RA_U5:
      //{
      //  SNOWHOUSECPU_SKIP_ISSPACE ();

      //  SNOWHOUSECPU_PARSE_GPR (reg_a);
      //  SNOWHOUSECPU_PARSE_COMMA ();

      //  SNOWHOUSECPU_PARSE_EXP ();

      //  pd.parse_good = true;
      //  pd.have_imm = true;
      //  pd.is_small_imm_unsigned = true;
      //}
      //  break;
      //case SNOWHOUSECPU_OA_U5:
      //{
      //  SNOWHOUSECPU_SKIP_ISSPACE ();

      //  SNOWHOUSECPU_PARSE_EXP ();

      //  pd.parse_good = true;
      //  pd.have_imm = true;
      //  pd.is_small_imm_unsigned = true;
      //}
      //  break;
      //case SNOWHOUSECPU_OA_RA:
      //{
      //  SNOWHOUSECPU_SKIP_ISSPACE ();

      //  SNOWHOUSECPU_PARSE_GPR (reg_a);

      //  pd.parse_good = true;
      //}
      //  break;
      //case SNOWHOUSECPU_OA_RA_RB:
      //{
      //  SNOWHOUSECPU_SKIP_ISSPACE ();

      //  //printf ("snowhousecpu dbg: testificate\n");
      //  SNOWHOUSECPU_PARSE_GPR (reg_a);
      //  SNOWHOUSECPU_PARSE_COMMA ();

      //  SNOWHOUSECPU_PARSE_GPR (reg_b);
      //  //printf ("snowhousecpu dbg: %s %s, %s\n",
      //  //  pd.opc_info->names[0], pd.reg_a->name, pd.reg_b->name);

      //  pd.parse_good = true;
      //}
      //  break;
      //case SNOWHOUSECPU_OA_RA_SP_RB:
      //{
      //  SNOWHOUSECPU_SKIP_ISSPACE ();

      //  SNOWHOUSECPU_PARSE_GPR (reg_a);
      //  SNOWHOUSECPU_PARSE_COMMA ();

      //  SNOWHOUSECPU_PARSE_NOENC_REG ("sp");
      //  SNOWHOUSECPU_PARSE_COMMA ();

      //  SNOWHOUSECPU_PARSE_GPR (reg_b);

      //  pd.parse_good = true;
      //}
      //  break;
      //case SNOWHOUSECPU_OA_RA_FP_RB:
      //{
      //  SNOWHOUSECPU_SKIP_ISSPACE ();

      //  SNOWHOUSECPU_PARSE_GPR (reg_a);
      //  SNOWHOUSECPU_PARSE_COMMA ();

      //  SNOWHOUSECPU_PARSE_NOENC_REG ("fp");
      //  SNOWHOUSECPU_PARSE_COMMA ();

      //  SNOWHOUSECPU_PARSE_GPR (reg_b);

      //  pd.parse_good = true;
      //}
      //  break;
      //case SNOWHOUSECPU_OA_PCREL_S9:
      ////case SNOWHOUSECPU_OA_PCREL_S32_NO_RELAX:
      //{
      //  SNOWHOUSECPU_SKIP_ISSPACE ();

      //  //SNOWHOUSECPU_PARSE_EXP ();
      //  SNOWHOUSECPU_PARSE_EXP_POST_POUND ();

      //  pd.parse_good = true;
      //  pd.is_pcrel = true;
      //  pd.have_imm = true;
      //}
      //  break;
      //case SNOWHOUSECPU_OA_IRA:
      //{
      //  SNOWHOUSECPU_SKIP_ISSPACE ();

      //  SNOWHOUSECPU_PARSE_NOENC_REG ("ira");

      //  pd.parse_good = true;
      //}
      //  break;
      //case SNOWHOUSECPU_OA_RA_SB:
      //{
      //  SNOWHOUSECPU_SKIP_ISSPACE ();

      //  SNOWHOUSECPU_PARSE_GPR (reg_a);
      //  SNOWHOUSECPU_PARSE_COMMA ();

      //  SNOWHOUSECPU_PARSE_SPR (reg_b);

      //  pd.parse_good = true;
      //}
      //  break;
      //case SNOWHOUSECPU_OA_SA_RB:
      //{
      //  SNOWHOUSECPU_SKIP_ISSPACE ();

      //  SNOWHOUSECPU_PARSE_SPR (reg_a);
      //  SNOWHOUSECPU_PARSE_COMMA ();

      //  SNOWHOUSECPU_PARSE_GPR (reg_b);

      //  pd.parse_good = true;
      //}
      //  break;
      //case SNOWHOUSECPU_OA_SA_SB:
      //{
      //  SNOWHOUSECPU_SKIP_ISSPACE ();

      //  SNOWHOUSECPU_PARSE_SPR (reg_a);
      //  SNOWHOUSECPU_PARSE_COMMA ();

      //  SNOWHOUSECPU_PARSE_SPR (reg_b);

      //  pd.parse_good = true;
      //}
      //  break;
      //case SNOWHOUSECPU_OA_PC_RB:
      //{
      //  SNOWHOUSECPU_SKIP_ISSPACE ();

      //  SNOWHOUSECPU_PARSE_PC ();
      //  SNOWHOUSECPU_PARSE_COMMA ();

      //  SNOWHOUSECPU_PARSE_GPR (reg_b);

      //  pd.parse_good = true;
      //}
      //  break;
//    //  case SNOWHOUSECPU_OA_RA_RB_RC_DIVMOD:
//    //  {
//    //  SNOWHOUSECPU_SKIP_ISSPACE ();
//    //  SNOWHOUSECPU_PARSE_GPR (reg_a);
//    //  SNOWHOUSECPU_PARSE_COMMA ();
//    //  SNOWHOUSECPU_PARSE_GPR (reg_b);
//    //  SNOWHOUSECPU_PARSE_COMMA ();
//    //  SNOWHOUSECPU_PARSE_GPR (reg_c);
//    //  pd.parse_good = true;
//    //  //pd.have_index = true;
//    //    pd.have_index_2reg_kind = SNOWHOUSECPU_HIDX2R_KIND_DIVMOD; 
//    //  }
//    //  break;
//    //  case SNOWHOUSECPU_OA_RC_RD_RA_RB_LMUL:
//    //  {
//    //  SNOWHOUSECPU_SKIP_ISSPACE ();
//    //  SNOWHOUSECPU_PARSE_GPR (reg_c);
//    //  SNOWHOUSECPU_PARSE_COMMA ();
//    //  SNOWHOUSECPU_PARSE_GPR (reg_d);
//    //  SNOWHOUSECPU_PARSE_COMMA ();
//    //  SNOWHOUSECPU_PARSE_GPR (reg_a);
//    //  SNOWHOUSECPU_PARSE_COMMA ();
//    //  SNOWHOUSECPU_PARSE_GPR (reg_b);
//    //  pd.parse_good = true;
//    //  //pd.have_index = true;
//    //    pd.have_index_2reg_kind = SNOWHOUSECPU_HIDX2R_KIND_LMUL; 
//    //  }
//    //  break;
      ////case SNOWHOUSECPU_OA_RA_RB_RC_RD_DIVMOD64:
      ////{
      ////  SNOWHOUSECPU_SKIP_ISSPACE ();
      ////  SNOWHOUSECPU_PARSE_GPR (reg_a);
      ////  SNOWHOUSECPU_PARSE_COMMA ();
      ////  SNOWHOUSECPU_PARSE_GPR (reg_b);
      ////  SNOWHOUSECPU_PARSE_COMMA ();
      ////  SNOWHOUSECPU_PARSE_GPR (reg_c);
      ////  SNOWHOUSECPU_PARSE_COMMA ();
      ////  SNOWHOUSECPU_PARSE_GPR (reg_d);
      ////  pd.parse_good = true;
      ////  //pd.have_index = true;
      ////  pd.have_index_2reg_kind = SNOWHOUSECPU_HIDX2R_KIND_DIVMOD64; 
      ////}
      //  //break;
      //case SNOWHOUSECPU_OA_RA_IMPLICIT_SP:
      //{
      //  SNOWHOUSECPU_SKIP_ISSPACE ();

      //  SNOWHOUSECPU_PARSE_GPR (reg_a);
      //  pd.reg_b = snowhousecpu_reg_lookup ("sp");

      //  pd.parse_good = true;
      //}
      //  break;
      //case SNOWHOUSECPU_OA_SA_IMPLICIT_SP:
      //{
      //  SNOWHOUSECPU_SKIP_ISSPACE ();

      //  SNOWHOUSECPU_PARSE_SPR (reg_a);
      //  pd.reg_b = snowhousecpu_reg_lookup ("sp");

      //  pd.parse_good = true;
      //}
      //  break;
      //case SNOWHOUSECPU_OA_PC_IMPLICIT_SP:
      //{
      //  SNOWHOUSECPU_SKIP_ISSPACE ();

      //  SNOWHOUSECPU_PARSE_PC ();
      //  pd.reg_b = snowhousecpu_reg_lookup ("sp");

      //  pd.parse_good = true;
      //}
      //  break;
      //case SNOWHOUSECPU_OA_RA_RB_LDST:
      //{
      //  SNOWHOUSECPU_SKIP_ISSPACE ();

      //  SNOWHOUSECPU_PARSE_GPR (reg_a);
      //  SNOWHOUSECPU_PARSE_COMMA ();

      //  if (*op_end != '[')
      //  {
      //    break;
      //  }
      //  ++op_end;

      //  SNOWHOUSECPU_PARSE_GPR (reg_b);
      //  if (*op_end != ']')
      //  {
      //    break;
      //  }
      //  ++op_end;

      //  pd.parse_good = true;
      //}
      //  break;
      ////case SNOWHOUSECPU_OA_RA_RB_S7_LDST_32:
      ////{
      ////  SNOWHOUSECPU_SKIP_ISSPACE ();

      ////  SNOWHOUSECPU_PARSE_GPR (reg_a);
      ////  SNOWHOUSECPU_PARSE_COMMA ();

      ////  if (*op_end != '[')
      ////  {
      ////    break;
      ////  }
      ////  ++op_end;

      ////  SNOWHOUSECPU_PARSE_GPR (reg_b);
      ////  if (*op_end != ']')
      ////  {
      ////    break;
      ////  }
      ////  ++op_end;

      ////  pd.ex.X_op = O_constant;
      ////  pd.ex.X_add_number = 0x0;

      ////  pd.parse_good = true;
      ////  pd.have_imm = true;
      ////  pd.have_index_ra_simm = true;
      ////}
      ////  break;
      //case SNOWHOUSECPU_OA_RA_RB_RC_LDST:
      //{
      //  SNOWHOUSECPU_SKIP_ISSPACE ();

      //  SNOWHOUSECPU_PARSE_GPR (reg_a);
      //  SNOWHOUSECPU_PARSE_COMMA ();

      //  if (*op_end != '[')
      //  {
      //    break;
      //  }
      //  ++op_end;

      //  SNOWHOUSECPU_PARSE_GPR (reg_b);
      //  SNOWHOUSECPU_PARSE_COMMA ();

      //  SNOWHOUSECPU_PARSE_GPR (reg_c);

      //  if (*op_end != ']')
      //  {
      //    break;
      //  }
      //  ++op_end;

      //  //pd.ex.X_op = O_constant;
      //  //pd.ex.X_add_number = 0x0;

      //  pd.parse_good = true;
      //  //pd.have_index = true;
      //  pd.have_index_2reg_kind = SNOWHOUSECPU_HIDX2R_KIND_LDST;
      //  //pd.have_imm = true;
      //  //printf("SNOWHOUSECPU_OA_RA_RB_RC_LDST\n");
      //}
      //  break;
      ////case SNOWHOUSECPU_OA_RA_RB_RC_LDST_32:
      ////{
      ////  SNOWHOUSECPU_SKIP_ISSPACE ();

      ////  SNOWHOUSECPU_PARSE_GPR (reg_a);
      ////  SNOWHOUSECPU_PARSE_COMMA ();

      ////  if (*op_end != '[')
      ////  {
      ////    break;
      ////  }
      ////  ++op_end;

      ////  SNOWHOUSECPU_PARSE_GPR (reg_b);
      ////  SNOWHOUSECPU_PARSE_COMMA ();

      ////  SNOWHOUSECPU_PARSE_GPR (reg_c);

      ////  if (*op_end != ']')
      ////  {
      ////    break;
      ////  }
      ////  ++op_end;

      ////  pd.ex.X_op = O_constant;
      ////  pd.ex.X_add_number = 0x0;

      ////  pd.parse_good = true;
      ////  pd.have_imm = true;
      ////  //pd.have_index = true;
      ////  pd.have_index_2reg_kind = SNOWHOUSECPU_HIDX2R_KIND_LDST;
      ////}
      ////  break;
      //case SNOWHOUSECPU_OA_RA_RB_S7_LDST:
      //{
      //  SNOWHOUSECPU_SKIP_ISSPACE ();

      //  SNOWHOUSECPU_PARSE_GPR (reg_a);
      //  SNOWHOUSECPU_PARSE_COMMA ();

      //  if (*op_end != '[')
      //  {
      //    break;
      //  }
      //  ++op_end;

      //  SNOWHOUSECPU_PARSE_GPR (reg_b);
      //  SNOWHOUSECPU_PARSE_COMMA ();

      //  SNOWHOUSECPU_PARSE_EXP ();

      //  if (*op_end != ']')
      //  {
      //    break;
      //  }
      //  ++op_end;

      //  pd.have_imm = true;
      //  pd.parse_good = true;
      //  pd.have_index_ra_simm = true;
      //}
      //  break;
      //case SNOWHOUSECPU_OA_RA_RB_RC_S7_LDST:
      //{
      //  SNOWHOUSECPU_SKIP_ISSPACE ();

      //  SNOWHOUSECPU_PARSE_GPR (reg_a);
      //  SNOWHOUSECPU_PARSE_COMMA ();

      //  if (*op_end != '[')
      //  {
      //    break;
      //  }
      //  ++op_end;

      //  SNOWHOUSECPU_PARSE_GPR (reg_b);
      //  SNOWHOUSECPU_PARSE_COMMA ();

      //  SNOWHOUSECPU_PARSE_GPR (reg_c);
      //  SNOWHOUSECPU_PARSE_COMMA ();

      //  SNOWHOUSECPU_PARSE_EXP ();

      //  if (*op_end != ']')
      //  {
      //    break;
      //  }
      //  ++op_end;

      //  //pd.have_index = true;
      //  pd.have_index_2reg_kind = SNOWHOUSECPU_HIDX2R_KIND_LDST;
      //  pd.have_index_ra_simm = true;
      //  pd.have_imm = true;
      //  pd.parse_good = true;
      //}
      //  break;
      //case SNOWHOUSECPU_OA_RA_RB_CPY64:
      //{
      //  SNOWHOUSECPU_SKIP_ISSPACE ();

      //  SNOWHOUSECPU_PARSE_GPR (reg_a);
      //  SNOWHOUSECPU_PARSE_COMMA ();

      //  SNOWHOUSECPU_PARSE_GPR (reg_b);
      //  SNOWHOUSECPU_PARSE_COMMA ();

      //  //if (*op_end != '#') 
      //  //{
      //  //  goto post_oa_switch;
      //  //}
      //  //++op_end;

      //  //op_end = (char *)clear_and_parse_const_dbl_save_ilp (op_end,
      //  //  &pd.const_dbl);

      //  //if (op_end == (char *) NULL)
      //  //{
      //  //  goto post_oa_switch;
      //  //}
      //  //++op_end;
      //  SNOWHOUSECPU_PARSE_EXP ();
      //  SNOWHOUSECPU_PARSE_COMMA ();

      //  SNOWHOUSECPU_PARSE_EXP_1 ();

      //  pd.have_imm = true;
      //  pd.have_cpy64 = true;
      //  pd.parse_good = true;
      //}
      //  break;
      //case SNOWHOUSECPU_OA_SA_RB_LDST:
      //{
      //  SNOWHOUSECPU_SKIP_ISSPACE ();

      //  SNOWHOUSECPU_PARSE_SPR (reg_a);
      //  SNOWHOUSECPU_PARSE_COMMA ();

      //  if (*op_end != '[')
      //  {
      //    break;
      //  }
      //  ++op_end;

      //  SNOWHOUSECPU_PARSE_GPR (reg_b);
      //  if (*op_end != ']')
      //  {
      //    break;
      //  }
      //  ++op_end;

      //  pd.parse_good = true;
      //}
      //  break;
      //case SNOWHOUSECPU_OA_SA_SB_LDST:
      //{
      //  SNOWHOUSECPU_SKIP_ISSPACE ();

      //  SNOWHOUSECPU_PARSE_SPR (reg_a);
      //  SNOWHOUSECPU_PARSE_COMMA ();

      //  if (*op_end != '[')
      //  {
      //    break;
      //  }
      //  ++op_end;

      //  SNOWHOUSECPU_PARSE_SPR (reg_b);
      //  if (*op_end != ']')
      //  {
      //    break;
      //  }
      //  ++op_end;

      //  pd.parse_good = true;
      //}
      //  break;
      //case SNOWHOUSECPU_OA_SA_RB_RC_LDST:
      //{
      //  SNOWHOUSECPU_SKIP_ISSPACE ();

      //  SNOWHOUSECPU_PARSE_SPR (reg_a);
      //  SNOWHOUSECPU_PARSE_COMMA ();

      //  if (*op_end != '[')
      //  {
      //    break;
      //  }
      //  ++op_end;

      //  SNOWHOUSECPU_PARSE_GPR (reg_b);
      //  SNOWHOUSECPU_PARSE_COMMA ();
      //  SNOWHOUSECPU_PARSE_GPR (reg_c);

      //  if (*op_end != ']')
      //  {
      //    break;
      //  }
      //  ++op_end;

      //  pd.parse_good = true;
      //  //pd.have_index = true;
      //  pd.have_index_2reg_kind = SNOWHOUSECPU_HIDX2R_KIND_LDST;
      //}
      //  break;
      //case SNOWHOUSECPU_OA_SA_SB_RC_LDST:
      //{
      //  SNOWHOUSECPU_SKIP_ISSPACE ();

      //  SNOWHOUSECPU_PARSE_SPR (reg_a);
      //  SNOWHOUSECPU_PARSE_COMMA ();

      //  if (*op_end != '[')
      //  {
      //    break;
      //  }
      //  ++op_end;

      //  SNOWHOUSECPU_PARSE_SPR (reg_b);
      //  SNOWHOUSECPU_PARSE_COMMA ();

      //  SNOWHOUSECPU_PARSE_GPR (reg_c);

      //  if (*op_end != ']')
      //  {
      //    break;
      //  }
      //  ++op_end;

      //  pd.parse_good = true;
      //  //pd.have_index = true;
      //  pd.have_index_2reg_kind = SNOWHOUSECPU_HIDX2R_KIND_LDST;
      //}
      //  break;
      //case SNOWHOUSECPU_OA_RA_S5_JUSTADDR:
      //{
      //  SNOWHOUSECPU_SKIP_ISSPACE ();

      //  if (*op_end != '[')
      //  {
      //    break;
      //  }
      //  ++op_end;

      //  SNOWHOUSECPU_PARSE_GPR (reg_a);
      //  SNOWHOUSECPU_PARSE_COMMA ();

      //  SNOWHOUSECPU_PARSE_EXP ();

      //  if (*op_end != ']')
      //  {
      //    break;
      //  }
      //  ++op_end;

      //  pd.have_imm = true;
      //  pd.is_g7_icreload = true;
      //  pd.parse_good = true;
      //}
      //  break;
      //case SNOWHOUSECPU_OA_RA_JUSTADDR:
      //{
      //  SNOWHOUSECPU_SKIP_ISSPACE ();

      //  if (*op_end != '[')
      //  {
      //    break;
      //  }
      //  ++op_end;

      //  SNOWHOUSECPU_PARSE_GPR (reg_a);

      //  if (*op_end != ']')
      //  {
      //    break;
      //  }
      //  ++op_end;

      //  pd.ex.X_op = O_constant;
      //  pd.ex.X_add_number = 0x0;

      //  pd.have_imm = true;
      //  pd.is_g7_icreload = true;
      //  pd.parse_good = true;
      //}
      //  break;
      //case SNOWHOUSECPU_OA_RA_RC_JUSTADDR:
      //{
      //  SNOWHOUSECPU_SKIP_ISSPACE ();

      //  if (*op_end != '[')
      //  {
      //    break;
      //  }
      //  ++op_end;

      //  SNOWHOUSECPU_PARSE_GPR (reg_a);
      //  SNOWHOUSECPU_PARSE_COMMA ();
      //  SNOWHOUSECPU_PARSE_GPR (reg_c);

      //  if (*op_end != ']')
      //  {
      //    break;
      //  }
      //  ++op_end;

      //  pd.ex.X_op = O_constant;
      //  pd.ex.X_add_number = 0x0;

      //  pd.have_imm = true;
      //  pd.is_g7_icreload = true;
      //  //pd.have_index = true;
      //  pd.have_index_2reg_kind = SNOWHOUSECPU_HIDX2R_KIND_JUSTADDR;
      //  pd.parse_good = true;
      //}
      //  break;
      //case SNOWHOUSECPU_OA_RA_RC_S5_JUSTADDR:
      //{
      //  SNOWHOUSECPU_SKIP_ISSPACE ();

      //  if (*op_end != '[')
      //  {
      //    break;
      //  }
      //  ++op_end;

      //  SNOWHOUSECPU_PARSE_GPR (reg_a);
      //  SNOWHOUSECPU_PARSE_COMMA ();
      //  SNOWHOUSECPU_PARSE_GPR (reg_c);
      //  SNOWHOUSECPU_PARSE_COMMA ();
      //  SNOWHOUSECPU_PARSE_EXP ();

      //  if (*op_end != ']')
      //  {
      //    break;
      //  }
      //  ++op_end;

      //  //pd.have_index = true;
      //  pd.have_index_2reg_kind = SNOWHOUSECPU_HIDX2R_KIND_JUSTADDR;
      //  pd.have_imm = true;
      //  pd.is_g7_icreload = true;
      //  pd.parse_good = true;
      //}
      //  break;
      default:
        abort ();
        break;
    }

  post_oa_switch:
    SNOWHOUSECPU_SKIP_ISSPACE ();

    /* This seems to be necessary */
    if (*op_end != '\0')
    {
      //printf ("testificate\n");
      pd.parse_good = false;
      continue;
    }

    if (pd.parse_good)
    {
      switch (pd.opc_info->subop.kind)
      {
        case SNOWHOUSECPU_SOK_NONE:
        {
          break;
        }
        case SNOWHOUSECPU_SOK_RC_IDX_FULL:
        {
          if (pd.opc_info->subop.is_non_inequality_branch_etc)
          {
            //if (
            //  (pd.reg_a.index == pd.reg_b->index && pd.reg_a->index != 0)
            //  == (pd.opc_info->subop.is_non_ineqaulity_branch_etc - 1ull)
            //)
            //{
              if (pd.opc_info->subop.is_non_inequality_branch_etc - 1 == 0)
              {
                //pd.reg_a = gprs[0];
                pd.reg_b = pd.reg_a;
              }
              else if (pd.opc_info->subop.is_non_inequality_branch_etc - 1 == 1)
              {
                // beq, bne
                fprintf (
                  stderr,
                  "beq, bne: %s %s, %s, simm16\n",
                  pd.opc_info->name,
                  pd.reg_a->name,
                  pd.reg_b->name
                );
                if (
                  (pd.reg_a->index == pd.reg_b->index && pd.reg_a->index != 0)
                  //== (pd.opc_info->subop.is_non_ineqaulity_branch_etc - 1ull)
                ) {
                  pd.reg_a = (gprs + SNOWHOUSECPU_GPR_ENUM_R0);
                  pd.reg_b = (gprs + SNOWHOUSECPU_GPR_ENUM_R0);
                }
              }
              else
              {
                gas_assert (false);
              }
            //}
          }
          pd.reg_c = (gprs + pd.opc_info->subop.val);
        }
          break;
        case SNOWHOUSECPU_SOK_RC_IDX_NZ:
        {
          //if ((self->subop_rc_idx != 0) == opc_info->subop.val)
          //{
          //  finished = true;
          //}
          //pd.reg_c = (gprs + pd.opc_info->subop.val);
        }
          break;
        case SNOWHOUSECPU_SOK_IMM16_LO:
        {
          const snowhousecpu_temp_t prev_simm = pd.simm;
          snowhousecpu_set_insn_field_p (
            SNOWHOUSECPU_SUBOP_IMM16_MASK, SNOWHOUSECPU_SUBOP_IMM16_BITPOS,
            &pd.simm, pd.opc_info->subop.val
          );
          if (pd.opc_info->oparg == SNOWHOUSECPU_OA_RA_RB_SHIFT_U5)
          {
            // yes, technically this isn't a full instruction that we're setting here!
            snowhousecpu_set_insn_field_p (
              SNOWHOUSECPU_SHIFT_IMM5_MASK, SNOWHOUSECPU_SHIFT_IMM5_BITPOS,
              &pd.simm, prev_simm
            );
          }
        }
          break;
      }
      break;
    }
  }
  #undef SNOWHOUSECPU_SKIP_ISSPACE
  #undef SNOWHOUSECPU_PARSE_COMMA
  #undef SNOWHOUSECPU_PARSE_GPR
  #undef SNOWHOUSECPU_PARSE_SPR
  #undef SNOWHOUSECPU_PARSE_NOENC_REG
  #undef SNOWHOUSECPU_PARSE_PC
  #undef SNOWHOUSECPU_PARSE_EXP_POST_POUND_WORKER
  #undef SNOWHOUSECPU_PARSE_EXP_POST_POUND
  #undef SNOWHOUSECPU_PARSE_EXP_WORKER
  #undef SNOWHOUSECPU_PARSE_EXP
  #undef SNOWHOUSECPU_PARSE_EXP_1

  if (!pd.parse_good)
  {
    as_bad (_("Invalid arguments for this instruction"));
    return;
  }
  /* -------- */
  //if (pd.have_index_2reg_kind != SNOWHOUSECPU_HIDX2R_KIND_NO_INDEX)
  //{
  //  snowhousecpu_cl_insn_t cl_insn;
  //  expressionS *temp_ex = NULL;
  //  //printf
  //  //  ("pd.have_index\n");

  //  memset (&cl_insn, 0, sizeof (cl_insn));

  //  cl_insn.have_pre = SNOWHOUSECPU_HAVE_PRE_NONE;
  //  cl_insn.reloc = BFD_RELOC_UNUSED;
  //  const snowhousecpu_temp_t
  //    my_reg_a = pd.reg_a != NULL ? pd.reg_a->index : 0x0ull,
  //    my_reg_b = pd.reg_b != NULL ? pd.reg_b->index : 0x0ull,
  //    my_reg_c = pd.reg_c != NULL ? pd.reg_c->index : 0x0ull;
  //    //my_reg_d = pd.reg_d != NULL ? pd.reg_d->index : 0x0ull;
  //  snowhousecpu_temp_t
  //    my_reg_0 = 0,
  //    my_reg_1 = 0;
  //  switch (pd.have_index_2reg_kind)
  //  {
  //    case SNOWHOUSECPU_HIDX2R_KIND_NO_INDEX:
  //    {
  //      gas_assert (false);
  //    }
  //      break;
  //    case SNOWHOUSECPU_HIDX2R_KIND_ATOMIC:
  //    {
  //      //my_reg_0
  //      my_reg_0 = my_reg_c;
  //    }
  //      break;
  //    case SNOWHOUSECPU_HIDX2R_KIND_LDST:
  //    {
  //      my_reg_0 = my_reg_b;
  //      my_reg_1 = my_reg_c;
  //    }
  //      break;
////      case SNOWHOUSECPU_HIDX2R_KIND_DIVMOD:
////      {
////      my_reg_0 = my_reg_c;
////      }
////      break;
////      case SNOWHOUSECPU_HIDX2R_KIND_LMUL:
////      {
////      my_reg_0 = my_reg_c;
////      my_reg_1 = my_reg_d;
////      }
////      break;
////      case SNOWHOUSECPU_HIDX2R_KIND_DIVMOD64:
////      {
////      my_reg_0 = my_reg_c;
////      my_reg_1 = my_reg_d;
////      }
////      break;
  //    case SNOWHOUSECPU_HIDX2R_KIND_JUSTADDR:
  //    {
  //      //my_reg_0 = pd.reg_a != NULL ? pd.reg_a->index : 0x0ull;
  //      my_reg_0 = my_reg_a;
  //      my_reg_1 = my_reg_c;
  //    }
  //      break;
  //  }
  //  cl_insn.data = snowhousecpu_enc_temp_insn_index_ra_rb(
  //    my_reg_0,
  //    my_reg_1
  //  );
  //  //cl_insn.data = snowhousecpu_enc_temp_insn_index
  //  //  (
  //  //    //(
  //  //    //  pd.reg_a != NULL
  //  //    //  ? pd.reg_a->index
  //  //    //  : pd.reg_b->index
  //  //    //),
  //  //    (
  //  //      (pd.opc_info->grp_info == &snowhousecpu_grp_info_g0_atomic)
  //  //      //(
  //  //      //  //pd.have_index_kind != SNOWHOUSECPU_HIDX2R_KIND_ATOMIC
  //  //      //  //&& pd.have_index_kind == SNOWHOUSECPU_HIDX2R_KIND_ATOMIC
  //  //      //)
  //  //      ? (pd.reg_b != NULL ? pd.reg_b->index : 0x0ull)
  //  //      : (
  //  //        (pd.opc_info->grp_info == &snowhousecpu_grp_info_g7_icreload)
  //  //        ? (pd.reg_a != NULL ? pd.reg_a->index : 0x0ull)
  //  //        : 0x0ull
  //  //      ) 
  //  //    ),
  //  //    pd.reg_c != NULL ? pd.reg_c->index : 0x0ull
  //  //  );
  //  cl_insn.opc_info = &snowhousecpu_opc_info_g5_index_ra_rb[0];
  //  snowhousecpu_relax_insn_ctor (&cl_insn.relax_insn, &cl_insn);
  //  append_cl_insn (&cl_insn, temp_ex);
  //  //pd.p = frag_more (2);
  //  //snowhousecpu_number_to_chars (pd.p,
  //  //  (pd.index_insn & 0xffffull),
  //  //  2);
  //  //dwarf2_emit_insn (2);

  //  ///* Per RISC-V: */
  //  ///* We need to start a new frag after any instruction that can be
  //  //  optimized away or compressed by the linker during relaxation,
  //  //  to prevent the assembler from computing static offsets across such
  //  //  an instruction.
  //  //  This is necessary to get correct EH info.  */
  //  //frag_wane (frag_now);
  //  //frag_new (0);
  //}

  //pd.nbytes = 2;
  //pd.p = frag_more (2);

  //if (!pd.have_cpy64)
  {
    //pd.reg_b = (const snowhousecpu_reg_t *)NULL;
    snowhousecpu_assemble_post_parse_worker (&pd, false);
  }
  //else // if (pd.have_cpy64)
  //{
  //  const snowhousecpu_reg_t *old_reg_b = pd.reg_b;
  //  //uint64_t temp_simm = generic_bignum_to_int32 ();
  //  //snowhousecpu_temp_t temp_simm = pd.simm;

  //  pd.reg_b = NULL;
  //  // %H0 comes first
  //  //pd.simm = (temp_simm >> 32) & 0xffffffff;
  //  snowhousecpu_assemble_post_parse_worker (&pd, false);

  //  pd.reg_a = old_reg_b;
  //  //pd.simm = temp_simm & 0xffffffff;
  //  snowhousecpu_assemble_post_parse_worker (&pd, true);
  //}

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
tc_gen_reloc (asection *section ATTRIBUTE_UNUSED, fixS *fixP)
{
  //fprintf
  //  (stderr,
  //  "tc_gen_reloc(): begin\n");
  arelent *reloc = (arelent *) xmalloc (sizeof (arelent));

  reloc->sym_ptr_ptr = (asymbol **) xmalloc (sizeof (asymbol *));
  *reloc->sym_ptr_ptr = symbol_get_bfdsym (fixP->fx_addsy);
  reloc->address = fixP->fx_frag->fr_address + fixP->fx_where;
  reloc->addend
    //= fixP->fx_offset;
    = fixP->fx_addnumber;

  reloc->howto = bfd_reloc_type_lookup (stdoutput, fixP->fx_r_type);
  if (reloc->howto == NULL)
  {
    as_bad_where (fixP->fx_file, fixP->fx_line,
      _("cannot represent %s relocation in object file"),
      bfd_get_reloc_code_name (fixP->fx_r_type));
    return NULL;
  }
  //fprintf (stderr,
  //  "tc_gen_reloc: %s\n",
  //  reloc->howto->name);

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
  /* SnowHouseCpu native word size is 4 bytes, so we override
    the GAS default of 2. */
  {"word", cons, 4},
  {"dword", cons, 8},
  {NULL, NULL, 0},
};

void
snowhousecpu_cons_fix_new (fragS *fragP,
                      int where,
                      unsigned int nbytes,
                      expressionS *ex,
                      const int unused_arg ATTRIBUTE_UNUSED)
{
  bfd_reloc_code_real_type r;
  //fixS *fixP;

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

  //fixP = 
  fix_new_exp (fragP, where, nbytes, ex, 0, r);
  //snowhousecpu_fix_sym_handler (fixP);
}

int
snowhousecpu_regname_to_dw2regnum (char *name)
{
  const snowhousecpu_reg_t *reg = snowhousecpu_reg_lookup ((const char *)name);

  if (reg != NULL)
  {
    switch (reg->kind)
    {
      case SNOWHOUSECPU_REG_KIND_GPR:
        return reg->index;
      case SNOWHOUSECPU_REG_KIND_SPR:
        return reg->index + SNOWHOUSECPU_NUM_GPRS;
      case SNOWHOUSECPU_REG_KIND_PC:
        return reg->index + SNOWHOUSECPU_NUM_GPRS + SNOWHOUSECPU_NUM_SPRS;
      default:
        return -1;
    }
  }
  return -1;
}

/* Implement tc_cfi_frame_initial_instructions, to initialize the DWARF-2
   unwind information for this procedure.  */
void
snowhousecpu_cfi_frame_initial_instructions (void)
{
  //unsigned sp_regno;
  //sp_regno = (unsigned) snowhousecpu_regname_to_dw2regnum ((char *)"sp");

  //cfi_add_CFA_def_cfa (sp_regno, 0);
  cfi_add_CFA_def_cfa (SNOWHOUSECPU_GPR_ENUM_SP, 0);
}
/* Implement tc_fix_adjustable.  */
//int
//snowhousecpu_fix_adjustable (fixS *fixp)
//{
//  if (fixp->fx_addsy == NULL)
//  {
//    return 1;
//  }
//
//  /* Prevent all adjustments to global symbols.  */
//  if (OUTPUT_FLAVOR == bfd_target_elf_flavour
//      && (S_IS_EXTERNAL (fixp->fx_addsy) || S_IS_WEAK (fixp->fx_addsy)))
//  {
//    return 0;
//  }
//
//  if (fixp->fx_r_type == BFD_RELOC_VTABLE_INHERIT
//      || fixp->fx_r_type == BFD_RELOC_VTABLE_ENTRY)
//  {
//    return 0;
//  }
//
//  /* Preserve relocations against symbols with function type.  */
//  if (symbol_get_bfdsym (fixp->fx_addsy)->flags & BSF_FUNCTION)
//  {
//    return 0;
//  }
//
//  return 1;
//}
//bool
//snowhousecpu_allow_local_subtract (expressionS *left,
//                              expressionS *right,
//                              segT section)
//{
//  /* If we are not in relaxation mode, subtraction is OK.  */
//  if (!linkrelax)
//  {
//    return true;
//  }
//
//  /* If the symbols are not in a code section then they are OK.  */
//  if ((section->flags & SEC_CODE) == 0)
//  {
//    return true;
//  }
//
//  if (left->X_add_symbol == right->X_add_symbol)
//  {
//    return true;
//  }
//
//  /* We have to assume that there may be instructions between the
//     two symbols and that relaxation may increase the distance between
//     them.  */
//  return false;
//}

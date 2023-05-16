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
#include "subsegs.h"
#include "safe-ctype.h"
#include "dwarf2dbg.h"
#include "dw2gencfi.h"
#include "opcode/flare32.h"
#include "elf/flare32.h"
#include "opcode/flare32-opc-decls.h"
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

//static htab_t flare32_opci_list_hash = NULL;
//#define flare32_opci_list_lookup(key)
//  ((flare32_opci_list_t *) str_hash_find (flare32_opci_list_hash, (key)))

//static htab_t flare32_opci_list_end_hash = NULL;
//#define flare32_opci_list_end_lookup(key)
//  ((flare32_opci_list_t *)
//    str_hash_find (flare32_opci_list_end_hash, (key)))
static htab_t flare32_opci_v2d_index_hash = NULL;
#define flare32_opci_v2d_index_lookup(key) \
  ((size_t *) str_hash_find (flare32_opci_v2d_index_hash, (key)))
static size_t flare32_opci_v2d_index_arr[FLARE32_MAX_UNIQUE_OPCI_NAMES];

static flare32_opci_v2d_t flare32_opci_v2d = { NULL, 0};

//typedef struct flare32_opci_vec_lookup_ret_t
//{
//  flare32_opci_vec_t *opci_vec;
//  size_t *v2d_index;
//} flare32_opci_vec_lookup_ret_t;

static inline flare32_opci_vec_t *
flare32_opci_vec_lookup (const char *key)
{
  flare32_opci_vec_t *ret;
  size_t *v2d_index;

  ret = NULL;
  v2d_index = flare32_opci_v2d_index_lookup (key);

  if (v2d_index != NULL)
  {
    ret = flare32_opci_v2d.data + (*v2d_index);
  }
  return ret;
}

static htab_t flare32_reg_hash = NULL;
#define flare32_reg_lookup(key) \
  ((const flare32_reg_t *) str_hash_find (flare32_reg_hash, (key)))

static unsigned
have_plp_insn_length (flare32_have_plp_t have_plp)
{
  return flare32_have_plp_insn_length (have_plp);
}
static unsigned
have_plp_distance (flare32_have_plp_t from, flare32_have_plp_t to)
{
  return flare32_have_plp_distance (from, to);
}

typedef struct flare32_relax_insn_t
{
  //flare32_cl_insn_t
  //  *cl_insn;
  flare32_temp_t
    prefix_insn_bitsize,
    insn_bitsize,
    curr_bitsize,
    target_bitsize,
    grp_value;
  bool
    is_pcrel: 1,
    was_lpre: 1,
    have_imm: 1;
  //unsigned
  //  length;
} flare32_relax_insn_t;
typedef struct flare32_cl_insn_t
{
  flare32_relax_insn_t
    relax_insn;

  //bool no_relax: 1;
  flare32_have_plp_t have_plp;

  /* The actual data of the encoded instruction. */
  flare32_temp_t data;

  bfd_reloc_code_real_type r_type;

  ///* Idea for the following members borrowed from "tc-riscv.c":
  //  riscv_cl_insn */
  ///* The frag that containss the instruction. */
  //fragS *fragP;

  ///* The offset into `fragP` of the first instruction byte. */
  //long where;

  ///* The relocs associated with the instruction, if any. */
  //fixS *fixP;
} flare32_cl_insn_t;
static bool
flare32_cl_insn_no_relax (const flare32_cl_insn_t *cl_insn)
{
  return (cl_insn->r_type == BFD_RELOC_FLARE32_G1G5G6_S32_NO_RELAX
    || cl_insn->r_type == BFD_RELOC_FLARE32_G3_S32_PCREL_NO_RELAX);
}

typedef struct flare32_relax_temp_t
{
  //flare32_relax_insn_t
  //  relax_insn;
  flare32_cl_insn_t
    *cl_insn;
  offsetT
    value;
  bool
    rm_prefix: 1;
  unsigned
    length;
} flare32_relax_temp_t;

static bool
have_relaxable_temp_insn (fragS *fragP);
static bool
relax_can_shrink_value (bfd_vma value,
  flare32_temp_t curr_bitsize, flare32_temp_t target_bitsize);
static void
flare32_relax_insn_ctor (flare32_relax_insn_t *self,
  flare32_cl_insn_t *cl_insn);
static void
flare32_relax_temp_ctor (flare32_relax_temp_t *self,
  fragS *fragP, asection *sec, bool update);
static unsigned
relaxed_cl_insn_length (fragS *fragP, asection *sec, bool update)
{
  flare32_relax_temp_t relax_temp;
  flare32_relax_temp_ctor (&relax_temp, fragP, sec, update);
  return relax_temp.length;
}

/* We shouldn't need to keep around references to elements of 
  `flare32_cl_insn_vec` after relaxing a single instruction,
  so let's use a dynamic array with better cache behavior than,
  and fewer dynamic allocation calls than,
  `flare32_opci_list_vec` */
static flare32_cl_insn_t *flare32_cl_insn_vec = NULL;
static relax_substateT
  flare32_cl_insn_vec_old_size = 0,
  flare32_cl_insn_vec_size = 0;

//static void
//flare32_print_hash_opci_list (const char *key)
//{
//  for (flare32_opci_list_t *opci_list=flare32_opci_list_lookup (key);
//    opci_list!=NULL;
//    opci_list=opci_list->next)
//  {
//    printf ("flare32_print_hash_opci_list (): {%s, %s} ",
//      opci_list->opc_info->names[0],
//      opci_list->opc_info->names[1]);
//    switch (opci_list->opc_info->oparg)
//    {
//      case FLARE32_OA_BAD:
//        printf ("FLARE32_OA_BAD\n");
//        break;
//      case FLARE32_OA_NONE:
//        printf ("FLARE32_OA_NONE\n");
//        break;
//      case FLARE32_OA_PRE:
//        printf ("FLARE32_OA_PRE\n");
//        break;
//      case FLARE32_OA_LPRE:
//        printf ("FLARE32_OA_LPRE\n");
//        break;
//      case FLARE32_OA_RA_S5:
//        printf ("FLARE32_OA_RA_S5\n");
//        break;
//      case FLARE32_OA_RA_PC_S5:
//        printf ("FLARE32_OA_RA_PC_S5\n");
//        break;
//      case FLARE32_OA_RA_SP_S5:
//        printf ("FLARE32_OA_RA_SP_S5\n");
//        break;
//      case FLARE32_OA_RA_FP_S5:
//        printf ("FLARE32_OA_RA_FP_S5\n");
//        break;
//      case FLARE32_OA_S5:
//        printf ("FLARE32_OA_S5\n");
//        break;
//      case FLARE32_OA_RA:
//        printf ("FLARE32_OA_RA\n");
//        break;
//      case FLARE32_OA_RA_RB:
//        printf ("FLARE32_OA_RA_RB\n");
//        break;
//      case FLARE32_OA_RA_SP_RB:
//        printf ("FLARE32_OA_RA_SP_RB\n");
//        break;
//      case FLARE32_OA_RA_FP_RB:
//        printf ("FLARE32_OA_RA_FP_RB\n");
//        break;
//      case FLARE32_OA_PCREL_S9:
//        printf ("FLARE32_OA_PCREL_S9\n");
//        break;
//      case FLARE32_OA_PCREL_S32_NO_RELAX:
//        printf ("FLARE32_OA_PCREL_S32_NO_RELAX\n");
//        break;
//      case FLARE32_OA_IRA:
//        printf ("FLARE32_OA_IRA\n");
//        break;
//      case FLARE32_OA_RA_SB:
//        printf ("FLARE32_OA_RA_SB\n");
//        break;
//      case FLARE32_OA_SA_RB:
//        printf ("FLARE32_OA_SA_RB\n");
//        break;
//      case FLARE32_OA_SA_SB:
//        printf ("FLARE32_OA_SA_SB\n");
//        break;
//      case FLARE32_OA_RA_RB_LDST:
//        printf ("FLARE32_OA_RA_RB_LDST\n");
//        break;
//      case FLARE32_OA_RA_RB_RC_LDST:
//        printf ("FLARE32_OA_RA_RB_RC_LDST\n");
//        break;
//      case FLARE32_OA_RA_RB_S5_LDST:
//        printf ("FLARE32_OA_RA_RB_S5_LDST\n");
//        break;
//      case FLARE32_OA_RA_RB_RC_S5_LDST:
//        printf ("FLARE32_OA_RA_RB_RC_S5_LDST\n");
//        break;
//      default:
//        printf ("unknown oparg\n");
//        break;
//    }
//  }
//}

//static void
//flare32_opci_list_vec_create (void)
//{
//  flare32_opci_list_vec = NULL;
//  flare32_opci_list_vec_size = 0;
//}
//static flare32_opci_list_t *
//flare32_opci_list_vec_append (flare32_opci_list_t *to_append)
//{
//  const size_t
//    old_size = flare32_opci_list_vec_size;
//  ++flare32_opci_list_vec_size;
//
//  flare32_opci_list_vec = (flare32_opci_list_t **)realloc
//    (flare32_opci_list_vec,
//    sizeof (flare32_opci_list_t *) * flare32_opci_list_vec_size);
//  flare32_opci_list_vec[old_size] = to_append;
//
//  return flare32_opci_list_vec[old_size];
//}
//static void
//flare32_opci_list_vec_delete (void)
//{
//  for (size_t i=0; i<flare32_opci_list_vec_size; ++i)
//  {
//    flare32_opci_list_delete (flare32_opci_list_vec[i]);
//  }
//  free (flare32_opci_list_vec);
//}
//static void
//flare32_opci_list_hnv_append
//  (const flare32_opc_info_t *opc_info, bool no_relax, unsigned names_ind)
//{
//  const char *temp_name
//    = !no_relax
//      ? opc_info->names[names_ind]
//      : opc_info->nr_names[names_ind];
//
//  if (opc_info->oparg == FLARE32_OA_BAD)
//  {
//    return;
//  }
//
//  flare32_opci_list_t
//    *opci_list = flare32_opci_list_lookup (temp_name),
//    *opci_list_end = flare32_opci_list_end_lookup (temp_name);
//
//  if (opci_list == NULL)
//  {
//    opci_list = flare32_opci_list_create ();
//    opci_list->opc_info = opc_info;
//
//    opci_list_end = opci_list;
//
//    //printf ("flare32 dbg hnv: %d %s %d %d 0x%jx\n",
//    //  names_ind,
//    //  opc_info->names[names_ind],
//    //  //(unsigned) opc_info->oparg == FLARE32_OA_RA_RB
//    //  (unsigned) opc_info->oparg,
//    //  (unsigned) opc_info->opcode,
//    //  (uintmax_t) opci_list
//    //);
//    //printf ("flare32 dbg: %jx\n", (uintmax_t) opci_list);
//
//    flare32_opci_list_vec_append (opci_list);
//
//    str_hash_insert (flare32_opci_list_hash,
//      temp_name,
//      opci_list,
//      0);
//    str_hash_insert (flare32_opci_list_end_hash,
//      temp_name,
//      opci_list_end,
//      0);
//  }
//  else
//  {
//    opci_list_end = flare32_opci_list_append (opci_list_end, opc_info);
//    str_hash_insert (flare32_opci_list_end_hash,
//      temp_name,
//      opci_list_end,
//      1);
//  }
//
//  //return opci_list;
//}
static void
flare32_opci_v2d_and_index_hash_append
  (const flare32_opc_info_t *opc_info, bool no_relax, unsigned names_ind)
{
  const char *temp_name
    = !no_relax
      ? opc_info->names[names_ind]
      : opc_info->nr_names[names_ind];
  flare32_opci_vec_t *opci_vec_ptr;

  if (opc_info->oparg == FLARE32_OA_BAD)
  {
    return;
  }

  opci_vec_ptr = flare32_opci_vec_lookup (temp_name);

  if (opci_vec_ptr == NULL)
  {
    flare32_opci_vec_t opci_vec;
    flare32_opci_vec_create (&opci_vec);
    flare32_opci_vec_append (&opci_vec, opc_info);

    str_hash_insert (flare32_opci_v2d_index_hash,
      temp_name,
      flare32_opci_v2d_index_arr + flare32_opci_v2d.size,
      0);
    flare32_opci_v2d_append (&flare32_opci_v2d, &opci_vec);
  }
  else /* if (opci_vec != NULL) */
  {
    flare32_opci_vec_append (opci_vec_ptr, opc_info);
  }
}

static void
flare32_cl_insn_vec_create (void)
{
  flare32_cl_insn_vec = NULL;
  flare32_cl_insn_vec_old_size = 0;
  flare32_cl_insn_vec_size = 0;
}
static flare32_cl_insn_t *
flare32_cl_insn_vec_append (const flare32_cl_insn_t *to_append)
{
  //const size_t
  //  old_size = flare32_cl_insn_vec_size;
  flare32_cl_insn_vec_old_size = flare32_cl_insn_vec_size;
  ++flare32_cl_insn_vec_size;

  flare32_cl_insn_vec = (flare32_cl_insn_t *)realloc
    (flare32_cl_insn_vec,
    sizeof (flare32_cl_insn_t) * flare32_cl_insn_vec_size);
  //flare32_cl_insn_vec[flare32_cl_insn_vec_old_size ] = to_append;
  (void) memcpy (flare32_cl_insn_vec + flare32_cl_insn_vec_old_size,
    to_append,
    sizeof (flare32_cl_insn_t));

  //return (flare32_cl_insn_t *)memcpy
  //  (flare32_cl_insn_vec + flare32_cl_insn_vec_old_size,
  //  to_append,
  //  sizeof (flare32_cl_insn_t));
  return flare32_cl_insn_vec + flare32_cl_insn_vec_old_size;
}
static void
flare32_cl_insn_vec_delete (void)
{
  free (flare32_cl_insn_vec);
}

typedef struct flare32_parse_data_t
{
  char
    *op_start,
    *op_end,
    *op_end_prev,
    //*p,
    pend,
    op_name[FLARE32_OPC_INFO_NAME_MAX_LEN + 1];
  const flare32_opc_info_t
    *opc_info;
  //flare32_opci_list_t
  //  *opci_list;
  flare32_opci_vec_t
    *opci_vec;
  flare32_temp_t
    //index_insn,
    prefix_insn,
    insn,
    simm,
    fw;
  const flare32_reg_t
    *reg_a,
    *reg_b,
    *reg_c;
  size_t
    nlen;
    //nbytes;
  bool
    have_index,
    have_imm,
    have_cpy64,
    //have_simm,
    is_pcrel,
    no_relax,
    parse_good;
  expressionS
    ex,
    ex_1;
  //flare32_const_dbl_t
  //  const_dbl;
} flare32_parse_data_t;

/* Idea borrowed from "tc-riscv.c": `install_insn ()` */
//static void
//install_cl_insn (const flare32_cl_insn_t *cl_insn)
//{
//  char *p = cl_insn->fragP->fr_literal + cl_insn->where;
//  flare32_number_to_chars (p, cl_insn->data,
//    have_plp_insn_length (cl_insn->have_plp));
//}
/* Idea borrowed from "tc-riscv.c": `move_insn ()` */
static void
move_cl_insn (
  //flare32_cl_insn_t *cl_insn,
  const flare32_cl_insn_t *cl_insn,
  fragS *fragP, long where)
{
  //cl_insn->fragP = fragP;
  //cl_insn->where = where;
  //if (cl_insn->fixP != NULL)
  //{
  //  cl_insn->fixP->fx_frag = cl_insn->fragP;
  //  cl_insn->fixP->fx_where = cl_insn->where;
  //}
  //install_cl_insn (cl_insn);
  char *p = fragP->fr_literal + where;
  flare32_number_to_chars (p, cl_insn->data,
    have_plp_insn_length (cl_insn->have_plp));
}

/* Idea borrowed from "tc-riscv.c": `add_fixed_insn ()` */
static void
add_fixed_size_cl_insn (const flare32_cl_insn_t *cl_insn)
{
  char *p = frag_more (have_plp_insn_length (cl_insn->have_plp));
  move_cl_insn (cl_insn, frag_now, p - frag_now->fr_literal);
}
static void
add_fne_cl_insn (const flare32_cl_insn_t *cl_insn,
                expressionS *address_expr)
{
  char *p;
  //fixS *fixP;
  reloc_howto_type *howto;

  const unsigned
    nbytes = have_plp_insn_length (cl_insn->have_plp);

  p = frag_more (nbytes);

  howto = bfd_reloc_type_lookup (stdoutput, cl_insn->r_type);
  if (howto == NULL)
  {
    as_bad (_("internal: unsupported Flare32 relocation number %d"),
      cl_insn->r_type);
  }

  //fixP = 
  fix_new_exp
    (frag_now, /* frag */
    (p - frag_now->fr_literal), /* where */
    bfd_get_reloc_size (howto), /* size */
    address_expr, /* exp */
    (int) cl_insn->relax_insn.is_pcrel, /* pcrel */
    cl_insn->r_type /* r_type */
    );

  // TODO: verify whether `nbytes` should be used as the argument.
  // As of this writing, RISC-V always uses `0` as the argument to
  // `dwarf2_emit_insn ()`, so maybe that's what I need too?
  //dwarf2_emit_insn (nbytes);
  dwarf2_emit_insn (0);
  flare32_number_to_chars (p, cl_insn->data, nbytes);

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
  //  && cl_insn->have_plp != FLARE32_HAVE_PLP_NEITHER)
  if (!flare32_cl_insn_no_relax (cl_insn))
  {
    frag_wane (frag_now);
    frag_new (0);
  }
}

/* Idea borrowed from "tc-riscv.c": `add_relaxed_insn ()` */
static void
add_gas_relaxed_cl_insn
  (const flare32_cl_insn_t *cl_insn,
  int max_chars, int var,
  //relax_substateT subtype,
  symbolS *symbol, offsetT offset)
{
  frag_grow (max_chars);
  move_cl_insn (cl_insn, frag_now, frag_more (0) - frag_now->fr_literal);

  flare32_cl_insn_vec_append (cl_insn);

  frag_var (rs_machine_dependent, max_chars, var,
    flare32_cl_insn_vec_old_size, //subtype,
    symbol, offset, NULL);
}
                  
/* Idea borrowed from "tc-riscv.c": `append_insn ()` */
static void
append_cl_insn (flare32_cl_insn_t *cl_insn,
                expressionS *address_expr
                //bfd_reloc_code_real_type r_type
                )
{
  //flare32_relax_temp_t
  //  relax_temp;
  flare32_relax_insn_t *relax_insn;
  bool no_relax;

  relax_insn = &cl_insn->relax_insn;
  no_relax = flare32_cl_insn_no_relax (cl_insn);

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
      //flare32_opt.gasrelax
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
        best_case = have_plp_insn_length (FLARE32_HAVE_PLP_NEITHER),
        worst_case = have_plp_insn_length (FLARE32_HAVE_PLP_LPRE);

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
  //printf ("flare32_enc_temp_insn_lpre_rshift(): %i\n",
  //  (int) opc_info->grp_info->grp_value);
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
//static flare32_temp_t
//flare32_enc_temp_insn_pre_rshift
//  (const flare32_opc_info_t *opc_info,
//  flare32_temp_t simm)
//{
//  //printf ("flare32_enc_temp_insn_pre_rshift(): %i\n",
//  //  (int) opc_info->grp_info->grp_value);
//  switch (opc_info->grp_info->grp_value)
//  {
//    case FLARE32_G1_GRP_VALUE:
//    case FLARE32_G5_GRP_VALUE:
//    case FLARE32_G6_GRP_VALUE:
//      return flare32_enc_temp_insn_pre
//        (simm >> FLARE32_G1G5G6_S5_BITSIZE);
//    case FLARE32_G3_GRP_VALUE:
//      return flare32_enc_temp_insn_pre
//        (simm >> FLARE32_G3_S9_BITSIZE);
//    default:
//      abort ();
//      break;
//  }
//}
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
    {
      flare32_temp_t ret = flare32_enc_temp_insn_g4
        (opc_info->opcode, ra_ind, rb_ind);
      //printf ("flare32_enc_temp_insn_non_pre_lpre: %s %d %d %d; 0x%x; "
      //  "\n",
      //  opc_info->names[0], (unsigned) opc_info->oparg,
      //  (unsigned) ra_ind, (unsigned) rb_ind,
      //  (unsigned) ret);
      return ret;
    }
    case FLARE32_G5_GRP_VALUE:
      return flare32_enc_temp_insn_g5
        (ra_ind, rb_ind, simm);
    case FLARE32_G6_GRP_VALUE:
      return flare32_enc_temp_insn_g6
        (ra_ind, rb_ind, simm);
    case FLARE32_G7_GRP_VALUE:
      if (opc_info->grp_info->subgrp
        == &flare32_enc_info_g7_aluopbh_subgrp)
      {
        return flare32_enc_temp_insn_g7_aluopbh
          (opc_info->opcode, fw, ra_ind, rb_ind);
      }
      else if (opc_info->grp_info->subgrp
        == &flare32_enc_info_g7_sprldst_subgrp)
      {
        return flare32_enc_temp_insn_g7_sprldst
          (opc_info->opcode, ra_ind, rb_ind);
      }
      else
      {
        abort ();
        break;
      }
    default:
      abort ();
      break;
  }
}
static flare32_temp_t
flare32_enc_temp_insn_index (flare32_temp_t rc_ind)
{
  const flare32_opc_info_t
    *opc_info;

  /* The below line doesn't work because we are no longer storing this 
    `opc_info` in `flare32_opci_list_hash`. */
  //opc_info = flare32_opci_list_lookup ("index")->opc_info;

  opc_info = &flare32_opc_info_g4[FLARE32_G4_OP_ENUM_INDEX_RA];

  return flare32_enc_temp_insn_non_pre_lpre
    (opc_info, /* opc_info */
    rc_ind, /* ra_ind */
    0, /* rb_ind */
    0, /* simm */
    0 /* fw */);
}
/* -------- */
/* Helper function for `flare32_fix_sym_handler ()`. */
//static void
//flare32_fix_addsy_subsy_handler_1
//  (fixS *fixP,
//  bfd_reloc_code_real_type r_type_add,
//  bfd_reloc_code_real_type r_type_sub)
//{
//  (void) fix_new
//    (fixP->fx_frag, /* frag */
//    fixP->fx_where, /* where */
//    fixP->fx_size,  /* size */
//    fixP->fx_subsy, /* add_symbol */
//    0u,             /* offset */
//    fixP->fx_pcrel, /* pcrel */
//    r_type_sub);    /* r_type */
//
//  fixP->fx_r_type = r_type_add;
//  fixP->fx_subsy = NULL;
//}
//static void
//flare32_fix_sym_handler (fixS *fixP)
//{
//  if (fixP->fx_addsy && fixP->fx_subsy)
//  {
//    bfd_reloc_code_real_type
//      old_r_type = fixP->fx_r_type,
//      r_type_add,
//      r_type_sub;
//    //fixS
//      //*fixP_next;
//      //*fixP_last;
//    switch (old_r_type)
//    {
//      case BFD_RELOC_8:
//        r_type_add = BFD_RELOC_FLARE32_PSEUDO_ADD8;
//        r_type_sub = BFD_RELOC_FLARE32_PSEUDO_SUB8;
//        break;
//      case BFD_RELOC_16:
//        r_type_add = BFD_RELOC_FLARE32_PSEUDO_ADD16;
//        r_type_sub = BFD_RELOC_FLARE32_PSEUDO_SUB16;
//        break;
//      case BFD_RELOC_32:
//      {
//        //segT sub_segment;
//        ///* Per RISC-V: */
//        ///* Use pc-relative relocation for FDE initial location.
//        //  The symbol address in .eh_frame may be adjusted in
//        //  _bfd_elf_discard_section_eh_frame, and the content of
//        //  .eh_frame will be adjusted in _bfd_elf_write_section_eh_frame.
//        //  Therefore, we cannot insert a relocation whose addend symbol is
//        //  in .eh_frame.  Othrewise, the value may be adjusted twice.  */
//        //if ((sub_segment = S_GET_SEGMENT (fixP->fx_subsy))
//        //  && strcmp (sub_segment->name, ".eh_frame") == 0
//        //  && S_GET_VALUE (fixP->fx_subsy)
//        //    == fixP->fx_frag->fr_address + fixP->fx_where)
//        //{
//        //  fixP->fx_r_type = BFD_RELOC_FLARE32_FDE_32_PCREL;
//        //  fixP->fx_subsy = NULL;
//        //  return;
//        //}
//        //else
//        {
//          r_type_add = BFD_RELOC_FLARE32_PSEUDO_ADD32;
//          r_type_sub = BFD_RELOC_FLARE32_PSEUDO_SUB32;
//        }
//      }
//        break;
//      case BFD_RELOC_64:
//        r_type_add = BFD_RELOC_FLARE32_PSEUDO_ADD64;
//        r_type_sub = BFD_RELOC_FLARE32_PSEUDO_SUB64;
//        break;
//
//      case BFD_RELOC_FLARE32_G1G5G6_S32:
//      case BFD_RELOC_FLARE32_G1G5G6_S32_NO_RELAX:
//      {
//        //segT sub_segment;
//        ///* Per RISC-V: */
//        ///* Use pc-relative relocation for FDE initial location.
//        //  The symbol address in .eh_frame may be adjusted in
//        //  _bfd_elf_discard_section_eh_frame, and the content of
//        //  .eh_frame will be adjusted in _bfd_elf_write_section_eh_frame.
//        //  Therefore, we cannot insert a relocation whose addend symbol is
//        //  in .eh_frame.  Othrewise, the value may be adjusted twice.  */
//        //if ((sub_segment = S_GET_SEGMENT (fixP->fx_subsy))
//        //  && strcmp (sub_segment->name, ".eh_frame") == 0
//        //  && S_GET_VALUE (fixP->fx_subsy)
//        //    == fixP->fx_frag->fr_address + fixP->fx_where)
//        //{
//        //  fixP->fx_r_type = BFD_RELOC_FLARE32_FDE_G1G5G6_S32_PCREL;
//        //  fixP->fx_subsy = NULL;
//        //  return;
//        //}
//        //else
//        {
//          r_type_add = BFD_RELOC_FLARE32_G1G5G6_S32_ADD32;
//          r_type_sub = BFD_RELOC_FLARE32_G1G5G6_S32_SUB32;
//        }
//      }
//        break;
//      case BFD_RELOC_FLARE32_G3_S32_PCREL:
//        //r_type_add = BFD_RELOC_FLARE32_G3_S32_PCREL_ADD32;
//        //r_type_sub = BFD_RELOC_FLARE32_G3_S32_PCREL_SUB32;
//        ////as_bad (_("can't subtract symbols in PC-relative immediate"));
//        ////as_bad_subtract (fixP);
//        ////return false;
//        ////return;
//        //break;
//      case BFD_RELOC_FLARE32_G3_S32_PCREL_NO_RELAX:
//        //r_type_add = BFD_RELOC
//        as_bad (_("can't subtract symbols in PC-relative immediate"));
//        //as_bad_subtract (fixP);
//        //return false;
//        return;
//      default:
//        abort ();
//        break;
//    }
//    ///* Adapted from RISC-V's `md_apply_fix ()` */
//    ////if (old_r_type == BFD_RELOC_FLARE32_G1G5G6_S32
//    ////  || old_r_type == BFD_RELOC_FLARE32_G3_S32_PCREL)
//    ////{
//    ////  fixP_last = xmemdup (fixP, sizeof (*fixP), sizeof (*fixP));
//    ////}
//
//    ////printf ("flare32_fix_sym_handler (): %d %d 0x%x\n",
//    ////  fixP == NULL,
//    ////  fixP != NULL ? (fixP->fx_next == NULL) : -1,
//    ////  (unsigned) fixP->fx_offset
//    ////);
//    ////fixP->fx_next = xmemdup (fixP, sizeof (*fixP), sizeof (*fixP));
//
//    ////fixP->fx_next->fx_addsy = fixP->fx_subsy;
//    ////fixP->fx_next->fx_subsy = NULL;
//    ////fixP->fx_next->fx_offset = 0;
//    ////fixP->fx_subsy = NULL;
//
//    ////fixP->fx_r_type = r_type_add;
//    ////fixP->fx_next->fx_r_type = r_type_sub;
//    (void) fix_new
//      (fixP->fx_frag, /* frag */
//      fixP->fx_where, /* where */
//      fixP->fx_size,  /* size */
//      fixP->fx_subsy, /* add_symbol */
//      0u,             /* offset */
//      fixP->fx_pcrel, /* pcrel */
//      r_type_sub);    /* r_type */
//
//    fixP->fx_r_type = r_type_add;
//    fixP->fx_subsy = NULL;
//    //flare32_fix_addsy_subsy_handler_1 (fixP, r_type_add, r_type_sub);
//
//
//    ////symbol_mark_used_in_reloc (fixP->fx_addsy);
//    ////symbol_mark_used_in_reloc (fixP->fx_next->fx_addsy);
//
//    ////if (old_r_type == BFD_RELOC_FLARE32_G1G5G6_S32
//    ////  || old_r_type == BFD_RELOC_FLARE32_G3_S32_PCREL)
//    ////{
//    ////  fixP_last->fx_addsy = NULL;
//    ////  fixP_last->fx_subsy = NULL;
//    ////  fixP_last->fx_offset = 0;
//    ////  fixP->fx_next->fx_next = fixP_last;
//    ////}
//
//    //return true;
//  }
//  //return false;
//}

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

//typedef struct flare32_const_dbl_t
//{
//  unsigned char data[8];
//  int length;
//} flare32_const_dbl_t;

//const char *
//clear_and_parse_const_dbl_save_ilp (char *s,
//  flare32_const_dbl_t *const_dbl
//  //bool *out_err
//  )
//{
//  const char *err;
//  char
//    *end = (char *)NULL,
//    *save = input_line_pointer;
//  (void) memset (const_dbl, 0, sizeof (*const_dbl));
//
//  input_line_pointer = s;
//  err = md_atof ('d', (char *)const_dbl->data, &const_dbl->length);
//  end = input_line_pointer;
//  input_line_pointer = save;
//
//  if ((err && (*err)) || s != end)
//  {
//    //*out_err = true;
//    return (const char *) NULL;
//  }
//  //if (s != end)
//  //{
//  //  //*out_err = false;
//  //  return end;
//  //}
//  return end;
//
//  //*out_err = false;
//  return (const char *) NULL;
//}

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
    //fprintf (stderr, "parse_register_operand (): %s\n", gprs[i].name);

    if (parse_register_operand_worker (ptr, gprs + i))
    {
      //fprintf (stderr, "testificate\n");
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
long
md_pcrel_from (fixS *fixP)
{
  // TODO: verify whether we need the old, commented-out code
  return fixP->fx_where + fixP->fx_frag->fr_address;
  //valueT addr = fixP->fx_where + fixP->fx_frag->fr_address;

  /* Offset is from the end of the instruction. */
  //switch (fixP->fx_r_type)
  //{
  //  case BFD_RELOC_FLARE32_G1G5G6_S5:
  //  case BFD_RELOC_FLARE32_G3_S9_PCREL:
  //    return addr + 2;

  //  case BFD_RELOC_FLARE32_G1G5G6_S17:
  //  case BFD_RELOC_FLARE32_G3_S21_PCREL:
  //    return addr + 4;

  //  case BFD_RELOC_FLARE32_G1G5G6_S32:
  //  case BFD_RELOC_FLARE32_G1G5G6_S32_ADD32:
  //  case BFD_RELOC_FLARE32_G1G5G6_S32_SUB32:
  //  case BFD_RELOC_FLARE32_G3_S32_NO_RELAX:
  //  case BFD_RELOC_FLARE32_G3_S32_PCREL:
  //  case BFD_RELOC_FLARE32_G3_S32_PCREL_NO_RELAX:
  //    return addr + 6;

  //  default:
  //    abort ();
  //    return addr;
  //}
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

//static flare32_temp_t
//flare32_apply_const_reloc (bfd_reloc_code_real_type r_type, bfd_vma value)
//{
//  switch (r_type)
//  {
//    case BFD_RELOC_32:
//      return value;
//    case BFD_RELOC_FLARE32_G1G5G6_S5:
//      return value & 
//    case BFD_RELOC_FLARE32_G1G5G6_S17:
//      return value &;
//    case BFD_RELOC_FLARE32_G1G5G6_S32:
//  }
//}

/* Apply a fixup to the object file.  */
void
md_apply_fix (fixS *fixP,
              valueT *valP,
              segT seg ATTRIBUTE_UNUSED)
{
  //bfd_byte *buf
  //  = (bfd_byte *) (fixP->fx_frag->fr_literal + fixP->fx_where);
  //bool relaxable = false;
  //offsetT loc;
  //segT sub_segment;

  /* Remember value for tc_gen_reloc */
  /* Note: `valP` is set to an expression using `fixP->fx_offset`
    in `fixup_segment ()` in "../write.c" */
  fixP->fx_addnumber = *valP;

  //switch (fixP->fx_r_type)
  //{
  //  case BFD_RELOC_FLARE32_G1G5G6_S5:
  //    break;
  //  case BFD_RELOC_FLARE32_G1G5G6_S17:
  //    break;
  //  case BFD_RELOC_FLARE32_G1G5G6_S32_NO_RELAX:
  //    break;
  //  case BFD_RELOC_FLARE32_G3_S9_PCREL:
  //    break;
  //  case BFD_RELOC_FLARE32_G3_S21_PCREL:
  //    break;
  //  case BFD_RELOC_FLARE32_G3_S32_PCREL_NO_RELAX:
  //    break;

  //  case BFD_RELOC_FLARE32_PSEUDO_ADD8:
  //  case BFD_RELOC_FLARE32_PSEUDO_ADD16:
  //  case BFD_RELOC_FLARE32_PSEUDO_ADD32:
  //  case BFD_RELOC_FLARE32_PSEUDO_ADD64:
  //  case BFD_RELOC_FLARE32_PSEUDO_SUB8:
  //  case BFD_RELOC_FLARE32_PSEUDO_SUB16:
  //  case BFD_RELOC_FLARE32_PSEUDO_SUB32:
  //  case BFD_RELOC_FLARE32_PSEUDO_SUB64:
  //    break;

  //  case BFD_RELOC_32:
  //    /* Blindly copied from RISC-V. */
  //    /* Use pc-relative relocation for FDE initial location.
  //       The symbol address in .eh_frame may be adjusted in
  //       _bfd_elf_discard_section_eh_frame, and the content of
  //       .eh_frame will be adjusted in _bfd_elf_write_section_eh_frame.
  //       Therefore, we cannot insert a relocation whose addend symbol is
  //       in .eh_frame.  Othrewise, the value may be adjusted twice.  */
  //    if (fixP->fx_addsy && fixP->fx_subsy
  //      && (sub_segment = S_GET_SEGMENT (fixP->fx_subsy))
  //      && strcmp (sub_segment->name, ".eh_frame") == 0
  //      && S_GET_VALUE (fixP->fx_subsy)
  //          == fixP->fx_frag->fr_address + fixP->fx_where)
  //    {
  //      fixP->fx_r_type = BFD_RELOC_FLARE32_EH_32_PCREL;
  //      fixP->fx_subsy = NULL;
  //      break;
  //    }
  //    /* Fall through.  */
  //  case BFD_RELOC_64:
  //  case BFD_RELOC_16:
  //  case BFD_RELOC_8:
  //    break;
  //  case BFD_RELOC_FLARE32_G1G5G6_S5:
  //    break;
  //}
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
  //  flare32_number_to_chars (buf, newval, 2);
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
flare32_number_to_chars (char *buf, valueT val, int n)
{
  //gas_assert (n == 1 || n == 2 || n == 4 || n == 8);
  number_to_chars_bigendian (buf, val, n);
}

/* Turn a string in input_line_pointer into a floating point constant
   of type TYPE, and store the appropriate bytes in *LITP.  The number
   of LITTLENUMS emitted is stored in *SIZEP.  An error message is
   returned, or NULL on OK.  */
const char *
flare32_md_atof (int type, char *litP, int *sizeP)
{
  //return ieee_md_atof (type, litP, sizeP, false);
  return ieee_md_atof (type, litP, sizeP, true);
  //int prec;
  //LITTLENUM_TYPE words[4];
  //char *t;
  //int i;

  //switch (type)
  //  {
  //  case 'f':
  //    prec = 2;
  //    break;

  //  case 'd':
  //    prec = 4;
  //    break;

  //  default:
  //    *sizeP = 0;
  //    return _("bad call to md_atof");
  //  }

  //t = atof_ieee (input_line_pointer, type, words);
  //if (t)
  //{
  //  input_line_pointer = t;
  //}

  //*sizeP = prec * 2;

  //for (i = prec - 1; i >= 0; i--)
  //{
  //  flare32_number_to_chars (litP, (valueT) words[i], 2);
  //  litP += 2;
  //}

  //return NULL;
}

//struct flare32_opt_t
//{
//  ///* -mrelax, -mno-relax: do (or not) relaxing in GAS */
//  //bool relax: 1;
//  ///* -mgasrelax, -mno-gasrelax: do (or not) relaxing in GAS */
//  //bool gasrelax: 1;
//  ///* -mlinkrelax, -mno-linkrelax: generate (or not) relocations for
//  //  linker relaxation. */
//  //bool linkrelax: 1;
//};
//static struct flare32_opt_t flare32_opt = { false };

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
  //switch (c)
  //{
  //  //case OPTION_RELAX:
  //  //  //flare32_opt.gasrelax = true;
  //  //  //flare32_opt.linkrelax = true;
  //  //  flare32_opt.relax = true;
  //  //  break;
  //  //case OPTION_NO_RELAX:
  //  //  //flare32_opt.gasrelax = false;
  //  //  //flare32_opt.linkrelax = false;
  //  //  flare32_opt.relax = false;
  //  //  break;
  //  //case OPTION_GASRELAX:
  //  //  flare32_opt.gasrelax = true;
  //  //  break;
  //  //case OPTION_NO_GASRELAX:
  //  //  flare32_opt.gasrelax = false;
  //  //  break;
  //  case OPTION_LINKRELAX:
  //    flare32_opt.linkrelax = true;
  //    break;
  //  case OPTION_NO_LINKRELAX:
  //    flare32_opt.linkrelax = false;
  //    break;
  //  default:
  //    return 0;
  //}
  //return 1;
  return 0;
}

/* Blank, as the are no machine-dependent options */
void
md_show_usage (FILE *stream ATTRIBUTE_UNUSED)
{
  //fprintf (stream,
  //  _("Flare32 options:\n"
  //    //"-mrelax      perform relaxing in GAS, and generate relocations for linker relaxation   (same effect as having both -mgasrelax and -mlinkrelax) (default not enabled).\n"
  //    //"-mno-relax   don't perform relaxing in GAS, and don't generate relocations for linker relaxation (same effect as having both -mno-gasrelax and -mno-linkrelax) (default not enabled).\n"
  //    //"-mgasrelax      perform relaxing in GAS (default not enabled).\n"
  //    //"-mno-gasrelax   don't perform relaxing in GAS (default not enabled).\n"
  //    "-mlinkrelax      generate relocations for linker relaxation (default not enabled).\n"
  //    "-mno-linkrelax   don't generate relocations for linker relaxation (default not enabled).\n")
  //  );
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
  //flare32_opci_v2d_t *opci_list;
  /* -------- */
  // old: /* This means we should always do linker relaxing */
  //linkrelax = true;
  //linkrelax = flare32_opt.linkrelax;
  /* -------- */
  flare32_opci_v2d_index_hash = str_htab_create ();
  flare32_opci_v2d_create (&flare32_opci_v2d);
  for (size_t i=0; i<FLARE32_MAX_UNIQUE_OPCI_NAMES; ++i)
  {
    flare32_opci_v2d_index_arr[i] = i;
  }
  //flare32_opci_v2d_vec_create ();

  //flare32_opci_v2d_hash = str_htab_create ();
  //flare32_opci_v2d_end_hash = str_htab_create ();
  //flare32_op_oa_hash = str_htab_create ();
  flare32_reg_hash = str_htab_create ();
  /* -------- */
  flare32_cl_insn_vec_create ();
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
      //flare32_opci_v2d_hnv_append (opc_info, false, 0);
      //flare32_opci_v2d_hnv_append (opc_info, true, 0);
      flare32_opci_v2d_and_index_hash_append (opc_info, false, 0);
      flare32_opci_v2d_and_index_hash_append (opc_info, true, 0);
    }
  }

  for (count=0, opc_info=flare32_opc_info_g1;
    count++<FLARE32_G1_OPC_INFO_LIM;
    ++opc_info)
  {
    flare32_opci_v2d_and_index_hash_append (opc_info, false, 0);
    flare32_opci_v2d_and_index_hash_append (opc_info, true, 0);
  }
  //printf ("post g1\n");
  //flare32_print_hash_opci_list ("cpy");


  for (count=0, opc_info=flare32_opc_info_g2;
    count++<FLARE32_G2_OPC_INFO_LIM;
    ++opc_info)
  {
    flare32_opci_v2d_and_index_hash_append (opc_info, false, 0);
    flare32_opci_v2d_and_index_hash_append (opc_info, true, 0);

    if (opc_info->opcode != FLARE32_G2_OP_ENUM_CMP_RA_RB
      && opc_info->opcode != FLARE32_G2_OP_ENUM_CMPBC_RA_RB)
    {
      flare32_opci_v2d_and_index_hash_append (opc_info, false, 1);
      flare32_opci_v2d_and_index_hash_append (opc_info, true, 1);
    }
  }
  //printf ("post g2\n");
  //flare32_print_hash_opci_list ("cpy");

  for (count=0, opc_info=flare32_opc_info_g3;
    count++<FLARE32_G3_OPC_INFO_LIM;
    ++opc_info)
  {
    flare32_opci_v2d_and_index_hash_append (opc_info, false, 0);
    flare32_opci_v2d_and_index_hash_append (opc_info, true, 0);
  }

  for (count=0, opc_info=flare32_opc_info_g4;
    count++<FLARE32_G4_OPC_INFO_LIM;
    ++opc_info)
  {
    if (opc_info->opcode != FLARE32_G4_OP_ENUM_INDEX_RA)
    {
      flare32_opci_v2d_and_index_hash_append (opc_info, false, 0);
      flare32_opci_v2d_and_index_hash_append (opc_info, true, 0);
    }
  }

  //printf ("post g4\n");
  //flare32_print_hash_opci_list ("cpy");

  for (count=0, opc_info=flare32_opc_info_g5;
    count++<FLARE32_G5_OPC_INFO_LIM;
    ++opc_info)
  {
    flare32_opci_v2d_and_index_hash_append (opc_info, false, 0);
    flare32_opci_v2d_and_index_hash_append (opc_info, true, 0);
  }

  for (count=0, opc_info=flare32_opc_info_g6;
    count++<FLARE32_G6_OPC_INFO_LIM;
    ++opc_info)
  {
    flare32_opci_v2d_and_index_hash_append (opc_info, false, 0);
    flare32_opci_v2d_and_index_hash_append (opc_info, true, 0);
  }

  for (count=0, opc_info=flare32_opc_info_g7_aluopbh;
    count++<FLARE32_G7_ALUOPBH_OPC_INFO_LIM;
    ++opc_info)
  {
    flare32_opci_v2d_and_index_hash_append (opc_info, false, 0);
    flare32_opci_v2d_and_index_hash_append (opc_info, true, 0);
    flare32_opci_v2d_and_index_hash_append (opc_info, false, 1);
    flare32_opci_v2d_and_index_hash_append (opc_info, true, 1);
  }

  for (count=0, opc_info=flare32_opc_info_g7_sprldst;
    count++<FLARE32_G7_SPRLDST_OPC_INFO_LIM;
    ++opc_info)
  {
    flare32_opci_v2d_and_index_hash_append (opc_info, false, 0);
    flare32_opci_v2d_and_index_hash_append (opc_info, true, 0);
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

/* Blindly copied from "tc-riscv.c" */
/* Because the value of .cfi_remember_state may changed after relaxation,
   we insert a fix to relocate it again in link-time.  */
void
flare32_pre_output_hook (void)
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
            BFD_RELOC_FLARE32_CFA);
        }
      }
    }
  }

  /* Restore the original segment info.  */
  subseg_set (seg, subseg);
}
/* Called just before the assembler exits.  */
void
flare32_md_end (void)
{
  flare32_cl_insn_vec_delete ();
  flare32_opci_v2d_delete_data (&flare32_opci_v2d);
  htab_delete (flare32_opci_v2d_index_hash);
  //flare32_opci_list_vec_delete ();
  //htab_delete (flare32_opci_list_hash);
  //htab_delete (flare32_op_oa_hash);
  htab_delete (flare32_reg_hash);
}

static bool
have_relaxable_temp_insn (fragS *fragP)
{
  flare32_cl_insn_t *cl_insn;
  flare32_temp_t grp_value;

  cl_insn = flare32_cl_insn_vec + fragP->fr_subtype;

  if (
    //!cl_insn->no_relax
    !flare32_cl_insn_no_relax (cl_insn)
    && cl_insn->have_plp != FLARE32_HAVE_PLP_NEITHER)
  {
    grp_value = flare32_get_insn_field_ei
      (&flare32_enc_info_grp_16, cl_insn->data);
    switch (grp_value)
    {
      //case flare32_grp_info_g1.grp_value:
      //case flare32_grp_info_g5.grp_value:
      //case flare32_grp_info_g6.grp_value:
      //case flare32_grp_info_g3.grp_value:
      case FLARE32_G1_GRP_VALUE:
      case FLARE32_G5_GRP_VALUE:
      case FLARE32_G6_GRP_VALUE:
      case FLARE32_G3_GRP_VALUE:
        return true;
      default:
        break;
    }
  }
  return false;
}

static bool
relax_can_shrink_value (bfd_vma value,
  flare32_temp_t curr_bitsize, flare32_temp_t target_bitsize)
{
  flare32_temp_t
    curr_value_se = flare32_sign_extend (value, curr_bitsize),
    target_value_se = flare32_sign_extend (value, target_bitsize);

  return (
    (uint32_t) curr_value_se
    == (uint32_t) target_value_se
  );
}

static void
flare32_relax_insn_ctor (flare32_relax_insn_t *self,
  flare32_cl_insn_t *cl_insn)
{
  gas_assert (self != NULL);
  memset (self, 0, sizeof (*self));
  self->grp_value = flare32_get_insn_field_ei
    (&flare32_enc_info_grp_16, cl_insn->data);
  self->have_imm = false;
  switch (self->grp_value)
  {
    case FLARE32_G1_GRP_VALUE:
    case FLARE32_G5_GRP_VALUE:
    case FLARE32_G6_GRP_VALUE:
      switch (cl_insn->have_plp)
      {
        case FLARE32_HAVE_PLP_NEITHER:
          //return false;
          //return 2;
          //self->length = 2;
          // TODO: determine if we need this `abort ()` call
          abort ();
          break;
        case FLARE32_HAVE_PLP_PRE:
          self->prefix_insn_bitsize = FLARE32_G0_PRE_S12_BITSIZE;
          self->insn_bitsize = FLARE32_G1G5G6_S5_BITSIZE;
          self->target_bitsize = self->insn_bitsize;
          break;
        case FLARE32_HAVE_PLP_LPRE:
          self->prefix_insn_bitsize
            = FLARE32_G1G5G6_G0_LPRE_S27_BITSIZE;
          self->insn_bitsize = FLARE32_G1G5G6_S5_BITSIZE;
          self->target_bitsize
            = FLARE32_G0_PRE_S12_BITSIZE + self->insn_bitsize;
          self->was_lpre = true;
          break;
      }
      self->have_imm = true;
      break;
    case FLARE32_G3_GRP_VALUE:
      switch (cl_insn->have_plp)
      {
        case FLARE32_HAVE_PLP_NEITHER:
          //return false;
          //return 2;
          //self->length = 2;
          // TODO: determine if we need this `abort ()` call
          abort ();
          break;
        case FLARE32_HAVE_PLP_PRE:
          self->prefix_insn_bitsize = FLARE32_G0_PRE_S12_BITSIZE;
          self->insn_bitsize = FLARE32_G3_S9_BITSIZE;
          self->target_bitsize = self->insn_bitsize;
          ///self->value -= 2;
          break;
        case FLARE32_HAVE_PLP_LPRE:
          self->prefix_insn_bitsize
            = FLARE32_G3_G0_LPRE_S23_BITSIZE;
          self->insn_bitsize = FLARE32_G3_S9_BITSIZE;
          self->target_bitsize
            = FLARE32_G0_PRE_S12_BITSIZE + self->insn_bitsize;
          //self->value -= 4;
          self->was_lpre = true;
          break;
      }
      self->is_pcrel = true;
      self->have_imm = true;
      break;
    default:
      //return false;
      // TODO: determine if we need this `abort ()` call
      abort ();
      break;
  }
  self->curr_bitsize
    = self->prefix_insn_bitsize + self->insn_bitsize;
}
static void
flare32_relax_temp_ctor (flare32_relax_temp_t *self,
  fragS *fragP, asection *sec, bool update)
{
  flare32_cl_insn_t *cl_insn;
  flare32_relax_insn_t *relax_insn;
  //const char *pfile;
  //unsigned int pline;
  bool no_sym = false;
  gas_assert (self != NULL);
  memset (self, 0, sizeof (*self));

  cl_insn = self->cl_insn = flare32_cl_insn_vec + fragP->fr_subtype;
  relax_insn = &cl_insn->relax_insn;
  // We're calling `flare32_relax_insn_ctor ()` in `append_cl_insn ()`
  //flare32_relax_insn_ctor
  //  ((relax_insn = &cl_insn->relax_insn),
  //  cl_insn);
  self->length = 6;

  if (
    //(have_expr = expr_symbol_where (fragP->fr_symbol, &pfile, &pline))
    (no_sym = (fragP->fr_symbol == NULL))
    || (
      //fragP->fr_symbol != NULL
      //&&
      S_IS_DEFINED (fragP->fr_symbol)
      && !S_IS_WEAK (fragP->fr_symbol)
      && sec == S_GET_SEGMENT (fragP->fr_symbol)
    )
  )
  {
    self->value
      = (!no_sym ? S_GET_VALUE (fragP->fr_symbol) : 0)
        + fragP->fr_offset;
    if (relax_insn->grp_value == FLARE32_G3_GRP_VALUE)
    {
      // TODO: Verify whether subtracting `fragP->fr_fix` is needed for
      // Flare32.
      // `fragP->fr_fix` is
      // "the number of characters we know we're going to emit to 
      // the output file".
      self->value -= fragP->fr_address + fragP->fr_fix;
      //self->value -= fragP->fr_address;
    }
    const offsetT
      shrink_one_unit_dist = have_plp_distance
        (FLARE32_HAVE_PLP_LPRE, FLARE32_HAVE_PLP_PRE),
      shrink_two_units_dist = have_plp_distance
        (FLARE32_HAVE_PLP_LPRE, FLARE32_HAVE_PLP_NEITHER);

    if (relax_can_shrink_value
      (!relax_insn->is_pcrel
        ? self->value
        : self->value
          //- 2ll
          - shrink_one_unit_dist,
      relax_insn->curr_bitsize,
      relax_insn->target_bitsize)
    )
    {
      if ((self->rm_prefix = (
        !relax_insn->was_lpre
        || relax_can_shrink_value
          (!relax_insn->is_pcrel
            ? self->value
            : self->value
              //- 4ll
              - shrink_two_units_dist,
          relax_insn->prefix_insn_bitsize,
          relax_insn->insn_bitsize)
      )))
      {
        self->length
          //= 2
          = have_plp_insn_length (FLARE32_HAVE_PLP_NEITHER);
        if (relax_insn->is_pcrel)
        {
          /* I think this doesn't need to be done in a relocation?
            We are already relaxing in GAS at this point, so we're already 
            decreasing `self->value`. */ 
          self->value
            //-= 4ull;
            -= shrink_two_units_dist;
        }
      }
      else
      {
        self->length
          //= 4;
          = have_plp_insn_length (FLARE32_HAVE_PLP_PRE);
        if (relax_insn->is_pcrel)
        {
          /* I think this doesn't need to be done in a relocation?
            We are already relaxing in GAS at this point, so we're already 
            decreasing `self->value`. */ 
          self->value
            //-= 2ull;
            -= shrink_one_unit_dist;
        }
      }
    }
  }
  if (update)
  {
    if (relax_insn->was_lpre)
    {
      if (self->rm_prefix) // remove the `lpre` instruction
      {
        cl_insn->have_plp = FLARE32_HAVE_PLP_NEITHER;
        cl_insn->data &= FLARE32_NO_EXT_MASK;
        if (!relax_insn->is_pcrel)
        {
          (void) flare32_set_insn_field_ei_p
            (&flare32_enc_info_g1g5g6_s5,
            &cl_insn->data,
            self->value);
        }
        else // if (relax_ins->is_pcrel)
        {
          (void) flare32_set_insn_field_ei_p
            (&flare32_enc_info_g3_s9, 
            &cl_insn->data,
            self->value);
        }
      }
      else // if (!self->rm_prefix) // convert the `lpre` to a `pre`
      {
        cl_insn->have_plp = FLARE32_HAVE_PLP_PRE;
        if (!relax_insn->is_pcrel)
        {
          cl_insn->data &= FLARE32_NO_EXT_MASK;
          cl_insn->data
            |= flare32_enc_temp_insn_pre (self->value)
              << FLARE32_ONE_EXT_BITPOS;
        }
        else // if (relax_insn->is_pcrel)
        {
          flare32_temp_t
            insn = (cl_insn->data & FLARE32_NO_EXT_MASK),
            prefix_insn = flare32_enc_temp_insn_pre(0x0);
          flare32_put_g3_s21 (&prefix_insn, &insn, self->value);
          cl_insn->data = (prefix_insn << FLARE32_ONE_EXT_BITPOS) | insn;
        }
      }
    }
    else // if (!self->was_lpre) // remove the `pre` instruction
    {
      cl_insn->have_plp = FLARE32_HAVE_PLP_NEITHER;
      cl_insn->data &= FLARE32_NO_EXT_MASK;

      if (!relax_insn->is_pcrel)
      {
        (void) flare32_set_insn_field_ei_p
          (&flare32_enc_info_g1g5g6_s5, &cl_insn->data, self->value);
      }
      else
      {
        (void) flare32_set_insn_field_ei_p
          (&flare32_enc_info_g3_s9, &cl_insn->data, self->value);
      }
    }
  }
}
int
md_estimate_size_before_relax (fragS *fragP, asection *sec)
{
  return (fragP->fr_var = relaxed_cl_insn_length (fragP, sec, false));
}

int
flare32_relax_frag (asection *sec, fragS *fragP,
  long stretch ATTRIBUTE_UNUSED)
{
  if (have_relaxable_temp_insn (fragP)) {
    offsetT old_var = fragP->fr_var;
    fragP->fr_var = relaxed_cl_insn_length (fragP, sec, true);
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
  flare32_relax_temp_t relax_temp;
  flare32_relax_insn_t *relax_insn;
  flare32_cl_insn_t *cl_insn;
  unsigned whole_insn_length;
  //int reloc;
  bfd_reloc_code_real_type *r_type;

  // TODO: Not sure whether I need to add `fragP->fr_fix`??
  // `fragP->fr_fix` is
  // "the number of characters we know we're going to emit to 
  // the output file".
  buf = (bfd_byte *)fragP->fr_literal + fragP->fr_fix;
  //buf = (bfd_byte *)fragP->fr_literal;

  flare32_relax_temp_ctor (&relax_temp, fragP, asec, false);
  cl_insn = relax_temp.cl_insn;
  relax_insn = &cl_insn->relax_insn;
  r_type = &cl_insn->r_type;

  /* Idea borrowed from "tc-arm.c": `md_convert_frag ()` */
  if (fragP->fr_symbol == NULL)
  {
    exp.X_op = O_constant;
  }
  else /* if (fragP->fr_symbol != NULL) */
  {
    exp.X_op = O_symbol;
    exp.X_add_symbol = fragP->fr_symbol;
  }
  exp.X_add_number = fragP->fr_offset;

  gas_assert (fragP->fr_var == have_plp_insn_length (cl_insn->have_plp));

  switch (*r_type)
  {
    case BFD_RELOC_FLARE32_G1G5G6_S5:
    case BFD_RELOC_FLARE32_G3_S9_PCREL:
      if (fragP->fr_var == have_plp_insn_length (FLARE32_HAVE_PLP_NEITHER))
      {
        whole_insn_length = fragP->fr_var;
        //fixP = fix_new_exp (fragP, buf - (bfd_byte *) fragP->fr_literal,
        //  whole_insn_length, &exp, (int) relax_insn->is_pcrel, *r_type);
      }
      else
      {
        abort ();
      }
      break;

    case BFD_RELOC_FLARE32_G1G5G6_S17:
    case BFD_RELOC_FLARE32_G3_S21_PCREL:
      if (fragP->fr_var == have_plp_insn_length (FLARE32_HAVE_PLP_NEITHER))
      {
        // convert `pre` to having no prefix at all
        //whole_insn_length = fragP->fr_var;
        const unsigned
          old_insn_dist = flare32_have_plp_distance
            (FLARE32_HAVE_PLP_PRE, FLARE32_HAVE_PLP_NEITHER),
          n_insn_dist = 0;

        const flare32_temp_t
          insn = bfd_getb16 (buf + old_insn_dist);
        bfd_putb16 (insn, buf + n_insn_dist);

        whole_insn_length = fragP->fr_var;
        *r_type = !relax_insn->is_pcrel
          ? BFD_RELOC_FLARE32_G1G5G6_S5
          : BFD_RELOC_FLARE32_G3_S9_PCREL;
        //fixP = fix_new_exp (fragP, buf - (bfd_byte *) fragP->fr_literal,
        //  whole_insn_length, &exp, (int) relax_insn->is_pcrel,
        //  ((*r_type) = !relax_insn->is_pcrel
        //    ? BFD_RELOC_FLARE32_G1G5G6_S5
        //    : BFD_RELOC_FLARE32_G3_S9_PCREL));
      }
      else if (
        fragP->fr_var == have_plp_insn_length (FLARE32_HAVE_PLP_PRE)
      )
      {
        whole_insn_length = fragP->fr_var;
        //fixP = fix_new_exp (fragP, buf - (bfd_byte *) fragP->fr_literal,
        //  whole_insn_length, &exp, (int) relax_insn->is_pcrel, *r_type);
      }
      else
      {
        abort ();
      }
      break;
    case BFD_RELOC_FLARE32_G1G5G6_S32:
    case BFD_RELOC_FLARE32_G3_S32_PCREL:
      if (fragP->fr_var == have_plp_insn_length (FLARE32_HAVE_PLP_NEITHER))
      {
        // convert `lpre` to having no prefix at all
        const unsigned
          old_insn_dist = flare32_have_plp_distance
            (FLARE32_HAVE_PLP_LPRE, FLARE32_HAVE_PLP_NEITHER),
          n_insn_dist = 0;

        const flare32_temp_t
          insn = bfd_getb16 (buf + old_insn_dist);
        bfd_putb16 (insn, buf + n_insn_dist);

        whole_insn_length = fragP->fr_var;
        *r_type = !relax_insn->is_pcrel
          ? BFD_RELOC_FLARE32_G1G5G6_S5
          : BFD_RELOC_FLARE32_G3_S9_PCREL;
        //fixP = fix_new_exp (fragP, buf - (bfd_byte *) fragP->fr_literal,
        //  whole_insn_length, &exp, (int) relax_insn->is_pcrel,
        //  ((*r_type) = !relax_insn->is_pcrel
        //    ? BFD_RELOC_FLARE32_G1G5G6_S5
        //    : BFD_RELOC_FLARE32_G3_S9_PCREL));
      }
      else if (
        fragP->fr_var == have_plp_insn_length (FLARE32_HAVE_PLP_PRE)
      )
      {
        // convert `lpre` to `pre`
        const unsigned
          old_insn_dist = have_plp_distance
            (FLARE32_HAVE_PLP_LPRE, FLARE32_HAVE_PLP_NEITHER),
          //old_pre_dist = have_plp_distance
          //  (FLARE32_HAVE_PLP_LPRE, FLARE32_HAVE_PLP_PRE),
          //old_lpre_dist = 0,
          n_pre_dist = 0,
          n_insn_dist = have_plp_distance
            (FLARE32_HAVE_PLP_LPRE, FLARE32_HAVE_PLP_PRE) + n_pre_dist;
        const flare32_temp_t
          insn = bfd_getb16 (buf + old_insn_dist),
          //old_lpre_insn = bfd_getb32 (buf),
          //simm = !relax_insn->is_pcrel
          //  ? flare32_get_g1g5g6_s32 (old_lpre_insn, insn)
          //  : flare32_get_g3_s32 (old_lpre_insn, insn),
          prefix_insn = flare32_enc_temp_insn_pre (0x0);

        // Turns out we didn't need to set the immediate fields here,
        // unlike with `flare32_do_relax_prefix_innards`
        // in "../../bfd/elf32-flare32.c"
        //if (!relax_insn->is_pcrel)
        //{
        //  flare32_put_g1g5g6_s17 (&prefix_insn, &insn, simm);
        //}
        //else /* if (relax_insn->is_pcrel) */
        //{
        //  flare32_put_g3_s21 (&prefix_insn, &insn, simm - old_pre_dist);
        //}

        bfd_putb16 (prefix_insn, buf + n_pre_dist);
        bfd_putb16 (insn, buf + n_insn_dist);

        whole_insn_length = fragP->fr_var;
        //fixP = fix_new_exp (fragP, buf - (bfd_byte *) fragP->fr_literal,
        //  whole_insn_length, &exp, (int) relax_insn->is_pcrel,
        //  ((*r_type)
        //    = !relax_insn->is_pcrel
        //      ? BFD_RELOC_FLARE32_G1G5G6_S17
        //      : BFD_RELOC_FLARE32_G3_S21_PCREL));
        *r_type = !relax_insn->is_pcrel
          ? BFD_RELOC_FLARE32_G1G5G6_S17
          : BFD_RELOC_FLARE32_G3_S21_PCREL;
      }
      else if (
        fragP->fr_var == have_plp_insn_length (FLARE32_HAVE_PLP_LPRE)
      )
      {
        whole_insn_length = fragP->fr_var;
        //fixP = fix_new_exp (fragP, buf - (bfd_byte *) fragP->fr_literal,
        //  whole_insn_length, &exp, (int) relax_insn->is_pcrel, *r_type);
      }
      else
      {
        abort ();
      }
      break;

    case BFD_RELOC_FLARE32_G1G5G6_S32_NO_RELAX:
    case BFD_RELOC_FLARE32_G3_S32_PCREL_NO_RELAX:
      if (fragP->fr_var == have_plp_insn_length (FLARE32_HAVE_PLP_LPRE))
      {
        whole_insn_length = fragP->fr_var;
        //fixP = fix_new_exp (fragP, buf - (bfd_byte *) fragP->fr_literal,
        //  whole_insn_length, &exp, (int) relax_insn->is_pcrel, *r_type);
      }
      else
      {
        abort ();
      }
      break;

    default:
      abort ();
      break;
  }

  fixP = fix_new_exp (fragP, buf - (bfd_byte *) fragP->fr_literal,
    whole_insn_length, &exp, (int) relax_insn->is_pcrel, *r_type);
  buf += whole_insn_length;

  fixP->fx_file = fragP->fr_file;
  fixP->fx_line = fragP->fr_line;
  gas_assert (buf == (bfd_byte *)fragP->fr_literal
    + fragP->fr_fix + fragP->fr_var);
  fragP->fr_fix += fragP->fr_var;
}


//static bool
//flare32_parse_insn (char *s, flare32_parse_info_t *parse_info)
//{
//}

static void
flare32_assemble_post_parse_worker (flare32_parse_data_t *pd,
  bool which_exp)
{
  flare32_cl_insn_t cl_insn;
  expressionS *temp_ex;
  //bfd_reloc_code_real_type r_type;

  memset (&cl_insn, 0, sizeof (cl_insn));
  //cl_insn.no_relax = pd->no_relax;
  if (!pd->have_imm)
  {
    //pd->nbytes = 2;
    //pd->p = frag_more (2);
    cl_insn.have_plp = FLARE32_HAVE_PLP_NEITHER;
    temp_ex = NULL;
    cl_insn.r_type = BFD_RELOC_UNUSED;
  }
  else // if (pc->have_imm)
  {
    cl_insn.have_plp = FLARE32_HAVE_PLP_LPRE;
    temp_ex = (!which_exp ? &pd->ex : &pd->ex_1);
    ////pd->nbytes += 4;
    ////(void) frag_more (4);
    ////if (temp_ex->X_add_symbol == NULL
    ////  && temp_ex->X_op_symbol == NULL)
    ////{
    ////  pd->nbytes
    ////}
    ////else
    {
      //pd->nbytes = 6;
      cl_insn.r_type
        = (!pd->is_pcrel
          ? (
            !pd->no_relax
            ? BFD_RELOC_FLARE32_G1G5G6_S32
            : BFD_RELOC_FLARE32_G1G5G6_S32_NO_RELAX
          )
          : (
            !pd->no_relax
            ? BFD_RELOC_FLARE32_G3_S32_PCREL
            : BFD_RELOC_FLARE32_G3_S32_PCREL_NO_RELAX
          )
        );
    }
    //pd->p = frag_more (pd->nbytes);

    ////if (!pd->have_cpy64)
    ////if (pd->nbytes == 6)
    //{
    //  fixS *fixP;

    //  //fprintf (stderr, "testificate: %d\n", (unsigned) pd->no_relax);
    //  //fprintf (stderr, "howto: %d %s\n",
    //  //  (unsigned) pd->no_relax,
    //  //  bfd_reloc_type_lookup (stdoutput, r_type)->name);

    //  fixP = fix_new_exp (frag_now, /* frag */
    //                      (pd->p - frag_now->fr_literal), /* where */
    //                      4, /* size */
    //                      //6,
    //                      temp_ex, /* exp */
    //                      (int) pd->is_pcrel, /* pcrel */
    //                      r_type /* r_type */
    //                      );
    //  //flare32_fix_sym_handler (fixP);
    //}

    pd->prefix_insn = flare32_enc_temp_insn_lpre_rshift (pd->opc_info,
      pd->simm);
  }
  pd->insn = flare32_enc_temp_insn_non_pre_lpre
    (pd->opc_info,
    (pd->reg_a != NULL ? pd->reg_a->index : 0x0),
    (pd->reg_b != NULL ? pd->reg_b->index : 0x0),
    pd->simm,
    pd->fw);
  cl_insn.data
    = (((pd->prefix_insn & 0xffffffffull) << FLARE32_ONE_EXT_BITPOS)
      | (pd->insn & 0xffffull));

  flare32_relax_insn_ctor (&cl_insn.relax_insn, &cl_insn);
  append_cl_insn (&cl_insn, temp_ex);

  //flare32_number_to_chars (pd->p,
  //  (((pd->prefix_insn & 0xffffffffull) << FLARE32_ONE_EXT_BITPOS)
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
  flare32_parse_data_t pd;

  (void) memset (&pd, 0, sizeof (pd));

  /* Drop leading whitespace. */
  while (*str == ' ')
  {
    ++str;
  }

  /* Find the opcode end. */
  /* Note: this should still work for the ".f" instructions that set
    `flags`. */
  pd.op_start = str;
  for (pd.op_end=str;
    *pd.op_end
      && !is_end_of_line[*pd.op_end & 0xff]
      && *pd.op_end != ' ';
    ++pd.op_end)
  {
    ++pd.nlen;
  }

  pd.pend = *pd.op_end;
  *pd.op_end = '\0';
  pd.op_end_prev = pd.op_end;

  //fprintf (stderr, "pd.op_start: %s\n", pd.op_start);

  if (pd.nlen == 0)
  {
    as_bad (_("can't find opcode "));
    return;
  }
  else if (pd.nlen > FLARE32_OPC_INFO_NAME_MAX_LEN)
  {
    as_bad (_("opcode is longer than maximum length of %d"),
      (unsigned) FLARE32_OPC_INFO_NAME_MAX_LEN);
    return;
  }

  for (size_t i=0; i<pd.nlen; ++i)
  {
    /* `pd.op_name` has been set to all '\0's via the earlier `memset ()`
      call, so we don't have to null terminate `pd.op_name`.
      Note that `pd.op_name` has a length of
      `FLARE32_OPC_INFO_NAME_MAX_LEN + 1`. */
    pd.op_name[i] = pd.op_start[i];
  }

  //pd.opci_list = flare32_opci_list_lookup (pd.op_name);
  pd.opci_vec = flare32_opci_vec_lookup (pd.op_name);
  *pd.op_end = pd.pend;

  //if (pd.opci_list == NULL)
  if (pd.opci_vec == NULL)
  {
    as_bad (_("unknown opcode %s"), pd.op_name);
    return;
  }

#define FLARE32_SKIP_ISSPACE() \
  do \
  { \
    while (ISSPACE (*pd.op_end)) \
    { \
      ++pd.op_end; \
    } \
  } while (0)
#define FLARE32_PARSE_COMMA()  \
  do \
  { \
    if (*pd.op_end != ',') \
    { \
      goto post_oa_switch; \
    } \
    ++pd.op_end; \
    FLARE32_SKIP_ISSPACE (); \
  } while (0)
#define FLARE32_PARSE_GPR(some_reg) \
  do \
  { \
    pd.some_reg = parse_register_operand (&pd.op_end); \
    if (pd.some_reg == NULL \
      || pd.some_reg->kind != FLARE32_REG_KIND_GPR) \
    { \
      goto post_oa_switch; \
    } \
  } while (0)
#define FLARE32_PARSE_SPR(some_reg) \
  do \
  { \
    pd.some_reg = parse_register_operand (&pd.op_end); \
    if (pd.some_reg == NULL \
      || pd.some_reg->kind != FLARE32_REG_KIND_SPR) \
    { \
      goto post_oa_switch; \
    } \
  } while (0)
#define FLARE32_PARSE_NOENC_REG(name) \
  do \
  { \
    if (parse_register_operand (&pd.op_end) \
      != flare32_reg_lookup (name)) \
    { \
      goto post_oa_switch; \
    } \
  } while (0)
#define FLARE32_PARSE_PC() \
  do \
  { \
    if (parse_register_operand (&pd.op_end) != &reg_pc) \
    { \
      goto post_oa_switch; \
    } \
  } while (0)

#define FLARE32_PARSE_EXP_POST_POUND_WORKER(some_ex) \
  do \
  { \
    pd.op_end = clear_and_parse_exp_save_ilp (pd.op_end, &pd.some_ex); \
    if (pd.some_ex.X_op == O_illegal || pd.some_ex.X_op == O_absent) \
    { \
      goto post_oa_switch; \
    } \
  } while (0)
#define FLARE32_PARSE_EXP_POST_POUND() \
  do \
  { \
    FLARE32_PARSE_EXP_POST_POUND_WORKER (ex); \
  } while (0)

#define FLARE32_PARSE_EXP_WORKER(some_ex) \
  do \
  { \
    if (*pd.op_end != '#') \
    { \
      goto post_oa_switch; \
    } \
    ++pd.op_end; \
    FLARE32_PARSE_EXP_POST_POUND_WORKER (some_ex); \
  } while (0)
#define FLARE32_PARSE_EXP() \
  do \
  { \
    FLARE32_PARSE_EXP_WORKER (ex); \
  } while (0)
#define FLARE32_PARSE_EXP_1() \
  do \
  { \
    FLARE32_PARSE_EXP_WORKER (ex_1); \
  } while (0)

  //for (pd.opc_info=pd.opci_list->opc_info;
  //  pd.opci_list!=NULL;
  //  pd.opci_list=pd.opci_list->next//,
  //    //pd.opc_info=pd.opci_list->opc_info
  //  )
  for (size_t i=0; i<pd.opci_vec->size; ++i)
  {
    const char * const *some_names;

    //pd.opc_info = pd.opci_list->opc_info;
    pd.opc_info = pd.opci_vec->data[i];
    //printf ("flare32 dbg: %s %s %d %d\n",
    //  pd.opc_info->names[0],
    //  pd.opc_info->names[1],
    //  //(unsigned) pd.opc_info->oparg == FLARE32_OA_RA_RB
    //  (unsigned) pd.opc_info->oparg,
    //  (unsigned) pd.opc_info->opcode
    //);
    pd.op_end = pd.op_end_prev;
    pd.is_pcrel = false;
    pd.reg_a = (const flare32_reg_t *) NULL;
    pd.reg_b = (const flare32_reg_t *) NULL;
    pd.reg_c = (const flare32_reg_t *) NULL;
    pd.simm = 0;
    pd.no_relax = !(
      (strncmp (pd.opc_info->names[0], pd.op_name,
        FLARE32_OPC_INFO_NAME_MAX_LEN) == 0)
      || (strncmp (pd.opc_info->names[1], pd.op_name,
        FLARE32_OPC_INFO_NAME_MAX_LEN) == 0)
    );
    //fprintf (stderr, "pd.no_relax stuff: \"%s\" %i %i %d\n",
    //  pd.op_name,
    //  strncmp (pd.opc_info->names[0], pd.op_name,
    //    FLARE32_OPC_INFO_NAME_MAX_LEN),
    //  strncmp (pd.opc_info->names[1], pd.op_name,
    //    FLARE32_OPC_INFO_NAME_MAX_LEN),
    //  (unsigned) pd.no_relax);
    some_names = !pd.no_relax ? pd.opc_info->names : pd.opc_info->nr_names;
    pd.fw = (flare32_temp_t) (
      (
        pd.opc_info->grp_info == &flare32_grp_info_g2
          && strncmp (some_names[1], pd.op_name,
            FLARE32_OPC_INFO_NAME_MAX_LEN) == 0
      ) || (
        pd.opc_info->grp_info == &flare32_grp_info_g7_aluopbh
        && strncmp (some_names[1], pd.op_name,
          FLARE32_OPC_INFO_NAME_MAX_LEN) == 0
      )
    );

    switch (pd.opc_info->oparg)
    {
      case FLARE32_OA_NONE:
      {
        FLARE32_SKIP_ISSPACE ();

        pd.parse_good = true;
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

        pd.parse_good = true;
        pd.have_imm = true;
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

        pd.parse_good = true;
        pd.have_imm = true;
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

        pd.parse_good = true;
        pd.have_imm = true;
      }
        break;
      case FLARE32_OA_RA_FP_S5:
      {
        FLARE32_SKIP_ISSPACE ();

        FLARE32_PARSE_GPR (reg_a);
        FLARE32_PARSE_COMMA ();

        FLARE32_PARSE_NOENC_REG ("fp");
        FLARE32_PARSE_COMMA ();

        FLARE32_PARSE_EXP ();

        pd.parse_good = true;
        pd.have_imm = true;
      }
        break;
      //case FLARE32_OA_RA_PC_S5:
      //case FLARE32_OA_RA_SP_S5:
      //case FLARE32_OA_RA_FP_S5:
      //{
      //  FLARE32_SKIP_ISSPACE ();

      //  FLARE32_PARSE_GPR (reg_a);
      //  FLARE32_PARSE_COMMA ();

      //  switch (pd.opc_info->oparg)
      //  {
      //    case FLARE32_OA_RA_PC_S5:
      //      FLARE32_PARSE_PC ();
      //      break;
      //    case FLARE32_OA_RA_SP_S5:
      //      FLARE32_PARSE_NOENC_REG ("sp");
      //      break;
      //    case FLARE32_OA_RA_FP_S5:
      //      FLARE32_PARSE_NOENC_REG ("fp");
      //      break;
      //  }

      //  FLARE32_PARSE_COMMA ();

      //  FLARE32_PARSE_EXP ();

      //  pd.parse_good = true;
      //  pd.have_imm = true;
      //}
      //  break;
      case FLARE32_OA_S5:
      {
        FLARE32_SKIP_ISSPACE ();

        FLARE32_PARSE_EXP ();

        pd.parse_good = true;
        pd.have_imm = true;
      }
        break;
      case FLARE32_OA_RA:
      {
        FLARE32_SKIP_ISSPACE ();

        FLARE32_PARSE_GPR (reg_a);

        pd.parse_good = true;
      }
        break;
      case FLARE32_OA_RA_RB:
      {
        FLARE32_SKIP_ISSPACE ();

        //printf ("flare32 dbg: testificate\n");
        FLARE32_PARSE_GPR (reg_a);
        FLARE32_PARSE_COMMA ();

        FLARE32_PARSE_GPR (reg_b);
        //printf ("flare32 dbg: %s %s, %s\n",
        //  pd.opc_info->names[0], pd.reg_a->name, pd.reg_b->name);

        pd.parse_good = true;
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

        pd.parse_good = true;
      }
        break;
      case FLARE32_OA_RA_FP_RB:
      {
        FLARE32_SKIP_ISSPACE ();

        FLARE32_PARSE_GPR (reg_a);
        FLARE32_PARSE_COMMA ();

        FLARE32_PARSE_NOENC_REG ("fp");
        FLARE32_PARSE_COMMA ();

        FLARE32_PARSE_GPR (reg_b);

        pd.parse_good = true;
      }
        break;
      case FLARE32_OA_PCREL_S9:
      //case FLARE32_OA_PCREL_S32_NO_RELAX:
      {
        FLARE32_SKIP_ISSPACE ();

        //FLARE32_PARSE_EXP ();
        FLARE32_PARSE_EXP_POST_POUND ();

        pd.parse_good = true;
        pd.is_pcrel = true;
        pd.have_imm = true;
      }
        break;
      case FLARE32_OA_IRA:
      {
        FLARE32_SKIP_ISSPACE ();

        FLARE32_PARSE_NOENC_REG ("ira");

        pd.parse_good = true;
      }
        break;
      case FLARE32_OA_RA_SB:
      {
        FLARE32_SKIP_ISSPACE ();

        FLARE32_PARSE_GPR (reg_a);
        FLARE32_PARSE_COMMA ();

        FLARE32_PARSE_SPR (reg_b);

        pd.parse_good = true;
      }
        break;
      case FLARE32_OA_SA_RB:
      {
        FLARE32_SKIP_ISSPACE ();

        FLARE32_PARSE_SPR (reg_a);
        FLARE32_PARSE_COMMA ();

        FLARE32_PARSE_GPR (reg_b);

        pd.parse_good = true;
      }
        break;
      case FLARE32_OA_SA_SB:
      {
        FLARE32_SKIP_ISSPACE ();

        FLARE32_PARSE_SPR (reg_a);
        FLARE32_PARSE_COMMA ();

        FLARE32_PARSE_SPR (reg_b);

        pd.parse_good = true;
      }
        break;
      case FLARE32_OA_RA_IMPLICIT_SP:
      {
        FLARE32_SKIP_ISSPACE ();

        FLARE32_PARSE_GPR (reg_a);
        pd.reg_b = flare32_reg_lookup ("sp");

        pd.parse_good = true;
      }
        break;
      case FLARE32_OA_SA_IMPLICIT_SP:
      {
        FLARE32_SKIP_ISSPACE ();

        FLARE32_PARSE_SPR (reg_a);
        pd.reg_b = flare32_reg_lookup ("sp");

        pd.parse_good = true;
      }
        break;
      case FLARE32_OA_RA_RB_LDST:
      {
        FLARE32_SKIP_ISSPACE ();

        FLARE32_PARSE_GPR (reg_a);
        FLARE32_PARSE_COMMA ();

        if (*pd.op_end != '[')
        {
          break;
        }
        ++pd.op_end;

        FLARE32_PARSE_GPR (reg_b);
        if (*pd.op_end != ']')
        {
          break;
        }
        ++pd.op_end;

        pd.parse_good = true;
      }
        break;
      case FLARE32_OA_RA_RB_RC_LDST:
      {
        FLARE32_SKIP_ISSPACE ();

        FLARE32_PARSE_GPR (reg_a);
        FLARE32_PARSE_COMMA ();

        if (*pd.op_end != '[')
        {
          break;
        }
        ++pd.op_end;

        FLARE32_PARSE_GPR (reg_b);
        FLARE32_PARSE_COMMA ();

        FLARE32_PARSE_GPR (reg_c);

        if (*pd.op_end != ']')
        {
          break;
        }
        ++pd.op_end;

        pd.parse_good = true;
        pd.have_index = true;
      }
        break;
      case FLARE32_OA_RA_RB_S5_LDST:
      {
        FLARE32_SKIP_ISSPACE ();

        FLARE32_PARSE_GPR (reg_a);
        FLARE32_PARSE_COMMA ();

        if (*pd.op_end != '[')
        {
          break;
        }
        ++pd.op_end;

        FLARE32_PARSE_GPR (reg_b);
        FLARE32_PARSE_COMMA ();

        FLARE32_PARSE_EXP ();

        if (*pd.op_end != ']')
        {
          break;
        }
        ++pd.op_end;

        pd.have_imm = true;
        pd.parse_good = true;
      }
        break;
      case FLARE32_OA_RA_RB_RC_S5_LDST:
      {
        FLARE32_SKIP_ISSPACE ();

        FLARE32_PARSE_GPR (reg_a);
        FLARE32_PARSE_COMMA ();

        if (*pd.op_end != '[')
        {
          break;
        }
        ++pd.op_end;

        FLARE32_PARSE_GPR (reg_b);
        FLARE32_PARSE_COMMA ();

        FLARE32_PARSE_GPR (reg_c);
        FLARE32_PARSE_COMMA ();

        FLARE32_PARSE_EXP ();

        if (*pd.op_end != ']')
        {
          break;
        }
        ++pd.op_end;

        pd.have_index = true;
        pd.have_imm = true;
        pd.parse_good = true;
      }
        break;
      case FLARE32_OA_RA_RB_CPY64:
      {
        FLARE32_SKIP_ISSPACE ();

        FLARE32_PARSE_GPR (reg_a);
        FLARE32_PARSE_COMMA ();

        FLARE32_PARSE_GPR (reg_b);
        FLARE32_PARSE_COMMA ();

        //if (*pd.op_end != '#') 
        //{
        //  goto post_oa_switch;
        //}
        //++pd.op_end;

        //pd.op_end = (char *)clear_and_parse_const_dbl_save_ilp (pd.op_end,
        //  &pd.const_dbl);

        //if (pd.op_end == (char *) NULL)
        //{
        //  goto post_oa_switch;
        //}
        //++pd.op_end;
        FLARE32_PARSE_EXP ();
        FLARE32_PARSE_COMMA ();

        FLARE32_PARSE_EXP_1 ();

        pd.have_imm = true;
        pd.have_cpy64 = true;
        pd.parse_good = true;
      }
        break;
      case FLARE32_OA_SA_RB_LDST:
      {
        FLARE32_SKIP_ISSPACE ();

        FLARE32_PARSE_SPR (reg_a);
        FLARE32_PARSE_COMMA ();

        if (*pd.op_end != '[')
        {
          break;
        }
        ++pd.op_end;

        FLARE32_PARSE_GPR (reg_b);
        if (*pd.op_end != ']')
        {
          break;
        }
        ++pd.op_end;

        pd.parse_good = true;
      }
        break;
      case FLARE32_OA_SA_SB_LDST:
      {
        FLARE32_SKIP_ISSPACE ();

        FLARE32_PARSE_SPR (reg_a);
        FLARE32_PARSE_COMMA ();

        if (*pd.op_end != '[')
        {
          break;
        }
        ++pd.op_end;

        FLARE32_PARSE_SPR (reg_b);
        if (*pd.op_end != ']')
        {
          break;
        }
        ++pd.op_end;

        pd.parse_good = true;
      }
        break;
      default:
        abort ();
        break;
    }

  post_oa_switch:
    FLARE32_SKIP_ISSPACE ();

    /* This seems to be necessary */
    if (*pd.op_end != '\0')
    {
      //printf ("testificate\n");
      pd.parse_good = false;
      continue;
    }

    if (pd.parse_good)
    {
      break;
    }
  }
  #undef FLARE32_SKIP_ISSPACE
  #undef FLARE32_PARSE_COMMA
  #undef FLARE32_PARSE_GPR
  #undef FLARE32_PARSE_SPR
  #undef FLARE32_PARSE_NOENC_REG
  #undef FLARE32_PARSE_PC
  #undef FLARE32_PARSE_EXP_POST_POUND_WORKER
  #undef FLARE32_PARSE_EXP_POST_POUND
  #undef FLARE32_PARSE_EXP_WORKER
  #undef FLARE32_PARSE_EXP
  #undef FLARE32_PARSE_EXP_1

  if (!pd.parse_good)
  {
    as_bad (_("Invalid arguments for this instruction"));
    return;
  }
  /* -------- */
  if (pd.have_index)
  {
    flare32_cl_insn_t cl_insn;
    expressionS *temp_ex = NULL;

    memset (&cl_insn, 0, sizeof (cl_insn));

    cl_insn.have_plp = FLARE32_HAVE_PLP_NEITHER;
    cl_insn.r_type = BFD_RELOC_UNUSED;
    cl_insn.data = flare32_enc_temp_insn_index
      (pd.reg_c != NULL ? pd.reg_c->index : 0x0ull);
    flare32_relax_insn_ctor (&cl_insn.relax_insn, &cl_insn);
    append_cl_insn (&cl_insn, temp_ex);
    //pd.p = frag_more (2);
    //flare32_number_to_chars (pd.p,
    //  (pd.index_insn & 0xffffull),
    //  2);
    //dwarf2_emit_insn (2);

    /* Per RISC-V: */
    /* We need to start a new frag after any instruction that can be
      optimized away or compressed by the linker during relaxation,
      to prevent the assembler from computing static offsets across such
      an instruction.
      This is necessary to get correct EH info.  */
    //frag_wane (frag_now);
    //frag_new (0);
  }

  //pd.nbytes = 2;
  //pd.p = frag_more (2);

  if (!pd.have_cpy64)
  {
    pd.reg_b = (const flare32_reg_t *)NULL;
    flare32_assemble_post_parse_worker (&pd, false);
  }
  else // if (pd.have_cpy64)
  {
    const flare32_reg_t *old_reg_b = pd.reg_b;
    //uint64_t temp_simm = generic_bignum_to_int32 ();
    //flare32_temp_t temp_simm = pd.simm;

    pd.reg_b = NULL;
    // %H0 comes first
    //pd.simm = (temp_simm >> 32) & 0xffffffff;
    flare32_assemble_post_parse_worker (&pd, false);

    pd.reg_a = old_reg_b;
    //pd.simm = temp_simm & 0xffffffff;
    flare32_assemble_post_parse_worker (&pd, true);
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
tc_gen_reloc (asection *section ATTRIBUTE_UNUSED, fixS *fixP)
{
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
  //printf ("tc_gen_reloc: %s\n",
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
  /* Flare32 native word size is 4 bytes, so we override
    the GAS default of 2. */
  {"word", cons, 4},
  {"dword", cons, 8},
  {NULL, NULL, 0},
};

void
flare32_cons_fix_new (fragS *fragP,
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
  //flare32_fix_sym_handler (fixP);
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

/* Implement tc_cfi_frame_initial_instructions, to initialize the DWARF-2
   unwind information for this procedure.  */
void
flare32_cfi_frame_initial_instructions (void)
{
  //unsigned sp_regno;
  //sp_regno = (unsigned) flare32_regname_to_dw2regnum ((char *)"sp");

  //cfi_add_CFA_def_cfa (sp_regno, 0);
  cfi_add_CFA_def_cfa (FLARE32_GPR_ENUM_SP, 0);
}
/* Implement tc_fix_adjustable.  */
//int
//flare32_fix_adjustable (fixS *fixp)
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
//flare32_allow_local_subtract (expressionS *left,
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

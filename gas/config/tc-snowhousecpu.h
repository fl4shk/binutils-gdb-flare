/* tc-snowhousecpu.c -- Assemble code for snowhousecpu
   Copyright (C) 2023 Free Software Foundation, Inc.

   This file is part of GAS, the GNU Assembler.

   GAS is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   GAS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GAS; see the file COPYING.  If not, write to
   the Free Software Foundation, 51 Franklin Street - Fifth Floor,
   Boston, MA 02110-1301, USA.  */

#ifndef __TC_SNOWHOUSECPU_H__
#define __TC_SNOWHOUSECPU_H__

/* Contributed by Andrew Clark (FL4SHK) */
#define TC_SNOWHOUSECPU 1
#define TARGET_BYTES_BIG_ENDIAN 0

/* Words are big enough to hold addresses.  */
#define WORKING_DOT_WORD 1
#define LOCAL_LABELS_FB 1

/* Symbols named FAKE_LABEL_NAME are emitted when generating DWARF, so make
   sure FAKE_LABEL_NAME is printable.  It still must be distinct from any
   real label name.  So, append a space, which other labels can't contain.  */
#define FAKE_LABEL_NAME ".L0"
/* Changing the special character in FAKE_LABEL_NAME requires changing
   FAKE_LABEL_CHAR too.  */
#define FAKE_LABEL_CHAR ' '


/* This macro is the BFD architecture to pass to `bfd_set_arch_mach'.  */
#define TARGET_FORMAT "elf32-snowhousecpu"
#define TARGET_ARCH bfd_arch_snowhousecpu

#define md_undefined_symbol(NAME)           0

#define md_number_to_chars(buf, val, n) \
  snowhousecpu_number_to_chars (buf, val, n)
extern void
snowhousecpu_number_to_chars (char *buf, valueT val, int n);

#define md_atof(type, litP, sizeP) \
  snowhousecpu_md_atof (type, litP, sizeP)
extern const char *
snowhousecpu_md_atof (int type, char *litP, int *sizeP);

#define md_pre_output_hook snowhousecpu_pre_output_hook ()
extern void snowhousecpu_pre_output_hook (void);

/* If this macro is defined and evaluates to zero then GAS will not fold 
  expressions that add or subtract a constant to/from a register to give 
  another register.
  For example GAS’s default behaviour is to fold
  the expression "r8 + 1" into "r9",
  which is probably not the result intended by the programmer. The 
  default is to allow such folding, since this maintains backwards   
  compatibility with earlier releases of GAS. */
//#define md_register_arithmetic 0


extern void
snowhousecpu_md_end (void);
#define md_end snowhousecpu_md_end

///* These macros must be defined, but is will be a fatal assembler
//   error if we ever hit them.  */
//#define md_estimate_size_before_relax(A, B)
//  (as_fatal (_("estimate size\n")), 0)
//#define md_convert_frag(B, S, F)
//  as_fatal (_("convert_frag\n"))

//#define GAS_SORT_RELOCS 1
#define md_relax_frag(segment, fragP, stretch) \
  snowhousecpu_relax_frag (segment, fragP, stretch)
extern int
snowhousecpu_relax_frag (asection *sec, fragS *fragP, long stretch);

/* If you define this macro, and the global variable ‘linkrelax’ is set 
  (because of a command-line option, or unconditionally in md_begin),
  a ‘.align’ directive will cause extra space to be allocated.
  The linker can then discard this space when relaxing the section. */
//#define LINKER_RELAXING_SHRINKS_ONLY

#define md_section_align(SEGMENT, SIZE) (SIZE)

/* We always relax during linking, so we don't need `TC_VALIDATE_FIX`. */
/* This macro is evaluated for each fixup (when linkrelax is not set).
  It may be used to change the fixup in struct fix *fixP before the generic 
  code sees it, or to fully process the fixup. In the latter case, a goto 
  skip will bypass the generic code. 

  Check also that `fx_addsy` is not `NULL`, to make sure the fixup refers 
  to some sort of label. */
//#define TC_VALIDATE_FIX(fixP, valP, seg)
//  if ((fixP->fx_r_type == BFD_RELOC_SNOWHOUSECPU_G1_S5
//    || fixP->fx_r_type == BFD_RELOC_SNOWHOUSECPU_G1_S17
//    || fixP->fx_r_type == BFD_RELOC_SNOWHOUSECPU_G1_S32
//    || fixP->fx_r_type == BFD_RELOC_SNOWHOUSECPU_G3_S9_PCREL
//    || fixP->fx_r_type == BFD_RELOC_SNOWHOUSECPU_G3_S21_PCREL
//    || fixP->fx_r_type == BFD_RELOC_SNOWHOUSECPU_G3_S32_PCREL)
//    && fixP->fx_addsy != NULL
//    && fixP->fx_subsy != NULL)
//  {
//    symbol_mark_used_in_reloc (fixP->fx_addsy);
//    goto SKIP;
//  }

///* We always relax during linking, so we don't need
//  `TC_VALIDATE_FIX_SUB`. */
/* This macro is evaluated for any fixup with a fx_subsy that
  fixup_segment cannot reduce to a number.  If the macro returns
  false an error will be reported.  */
//#define TC_VALIDATE_FIX_SUB(fix, seg) snowhousecpu_validate_fix_sub (fix)
//extern int snowhousecpu_validate_fix_sub (fixS *fix);
//#define TC_VALIDATE_FIX_SUB(fix, seg) 1

/* Because we evaluate relocs in the linker, we don't need
  `TC_FORCE_RELOCATION` and friends. */
/* If this macro returns non-zero, it guarantees that a relocation will be 
  emitted even when the value can be resolved locally, as fixup_segment 
  tries to reduce the number of relocations emitted. For example, a fixup 
  expression against an absolute symbol will normally not require a reloc.
  If undefined, a default of (S_FORCE_RELOC ((fix)->fx_addsy)) is used. */
/* If this macro returns non-zero, it guarantees that a relocation will be
  emitted even when the value can be resolved locally.  Do that if
  linkrelax is turned on.  */
//#define TC_FORCE_RELOCATION(fix) 1
//#define TC_FORCE_RELOCATION(fix)
//  snowhousecpu_force_relocation (fix)
//#define TC_FORCE_RELOCATION_SUB_SAME(fix, seg)
//  (GENERIC_FORCE_RELOCATION_SUB_SAME (fix, seg)
//    || snowhousecpu_force_relocation (fix))
//extern int snowhousecpu_force_relocation (fixS *fix);

/* Postpone text-section label subtraction calculation until linking, since
   linker relaxations might change the deltas.  */
#define TC_FORCE_RELOCATION_SUB_SAME(FIX, SEG) \
  (GENERIC_FORCE_RELOCATION_SUB_SAME (FIX, SEG) \
   || ((SEG)->flags & SEC_CODE) != 0)
#define TC_FORCE_RELOCATION_SUB_LOCAL(FIX, SEG) 1
#define TC_VALIDATE_FIX_SUB(FIX, SEG) 1
#define TC_FORCE_RELOCATION_LOCAL(FIX) 1
#define DIFF_EXPR_OK 1

///* Do not use PC relative fixups and relocations for
//  anything but real PCREL relocations.  */
//#define TC_FORCE_RELOCATION_SUB_LOCAL(FIX, SEG)
//  (((FIX)->fx_r_type != BFD_RELOC_SNOWHOUSECPU_G3_S9_PCREL)
//  && ((FIX)->fx_r_type != BFD_RELOC_SNOWHOUSECPU_G3_S21_PCREL)
//  && ((FIX)->fx_r_type != BFD_RELOC_SNOWHOUSECPU_G3_S32_PCREL)
//  && ((FIX)->fx_r_type != BFD_RELOC_SNOWHOUSECPU_G3_S32_PCREL_NO_RELAX))

/* Values passed to md_apply_fix don't include symbol values.  */
#define MD_APPLY_SYM_VALUE(FIX) 0

/* Let the linker resolve all the relocs due to relaxation. */
#define tc_fix_adjustable(fixP) 0
#define md_allow_local_subtract(l, r, s) 0
//#define tc_fix_adjustable(fixP) snowhousecpu_fix_adjustable (fixP)
//extern int
//snowhousecpu_fix_adjustable (fixS *fixp);

//#define md_allow_local_subtract(l,r,s)
//  snowhousecpu_allow_local_subtract (l, r, s)
//extern bool
//snowhousecpu_allow_local_subtract (expressionS *left, expressionS *right,
//  segT section);

/* No shared lib support, so we don't need to ensure externally
  visible symbols can be overridden.  */
#define EXTERN_FORCE_RELOC 1

/* Used to generate fixups during data allocation pseudo-ops */  
#define TC_CONS_FIX_NEW snowhousecpu_cons_fix_new
extern void
snowhousecpu_cons_fix_new (struct frag *fragP,
                      int where,
                      unsigned int nbytes,
                      struct expressionS *ex,
                      const int unused_arg ATTRIBUTE_UNUSED);

#define tc_regname_to_dw2regnum snowhousecpu_regname_to_dw2regnum
extern int
snowhousecpu_regname_to_dw2regnum (char *name);
#define tc_cfi_frame_initial_instructions \
  snowhousecpu_cfi_frame_initial_instructions
extern void
snowhousecpu_cfi_frame_initial_instructions (void);

//extern const relax_typeS md_relax_table[];
//#define TC_GENERIC_RELAX_TABLE md_relax_table

/* If defined, GAS will check this macro before performing any
  optimizations on the DWARF call frame debug information that is emitted.
  Targets which implement link time relaxation may need to define this
  macro and set it to zero if it is possible to change the size of a
  function’s prologue. */
#define md_allow_eh_opt 0

/* Stack alignment is 4 bytes */
#define DWARF2_CIE_DATA_ALIGNMENT -4

/* We want .cfi_* pseudo-ops for generating unwind info.  */
#define TARGET_USE_CFIPOP 1

/* Adjust debug_line after relaxation.  */
/* If linker relaxation might change offsets in the code, the DWARF special
   opcodes and variable-length operands cannot be used.  If this macro is
   nonzero, use the DW_LNS_fixed_advance_pc opcode instead.  */
#define DWARF2_USE_FIXED_ADVANCE_PC 1

/* RISC-V uses the `ra` register for this, so I guess I should use my `lr`
  register.
  However, some other use the program counter for it? I don't yet know if
  that is the correct option for my case. */
#define DWARF2_DEFAULT_RETURN_COLUMN \
  (snowhousecpu_regname_to_dw2regnum ((char *)"lr"))

#endif    /* __TC_SNOWHOUSECPU_H__ */

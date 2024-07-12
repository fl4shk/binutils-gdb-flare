/* flare-specific support for 32-bit ELF.
   Copyright (C) 2023 Free Software Foundation, Inc.
   Written by Andrew Clark (FL4SHK)

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

#include "elf32-flare.h"
#include "opcode/flare.h"
#include "elf/flare.h"
/* -------- */
#include "opcode/flare-opc-decls.h"
#include "opcode/flare-relax-reloc-lookup.h"
/* -------- */
#define USE_RELA

#define flare_get_insn_32(abfd, where) \
  ( \
    (bfd_get_16 (abfd, where) << 16) \
    | bfd_get_16 (abfd, where + 2) \
  )
#define flare_put_insn_32(abfd, prefix_insn, where) \
  bfd_put_16 (abfd, (prefix_insn >> 16) & 0xffffu, where); \
  bfd_put_16 (abfd, prefix_insn & 0xffffu, where + 2) \

/* Forward declarations. */
/* Special function for
  non-add32, non-sub32 relocations
  for instructions that have immediates */
static bfd_reloc_status_type
flare_elf_non_sub_imm_reloc (bfd *abfd,
                              arelent *reloc_entry,
                              asymbol *symbol,
                              void *data,
                              asection *input_section,
                              bfd *output_bfd,
                              char **error_message);
static bfd_reloc_status_type
flare_elf_do_non_sub_imm_reloc (bfd *input_bfd,
                                  reloc_howto_type *howto,
                                  asection *input_section,
                                  void *contents, bfd_vma address,
                                  bfd_vma relocation, bfd_vma addend);

/* Special function for local label subtraction */
static bfd_reloc_status_type
flare_elf_add_sub_reloc (bfd *abfd,
                          arelent *reloc_entry,
                          asymbol *symbol,
                          void *data,
                          asection *input_section,
                          bfd *output_bfd,
                          char **error_message);
static bfd_reloc_status_type
flare_elf_do_add_sub_reloc (bfd *input_bfd, reloc_howto_type *howto,
                              asection *input_section,
                              void *contents, bfd_vma address,
                              bfd_vma relocation, bfd_vma addend);
/* -------- */
static reloc_howto_type flare_elf_howto_table [] =
{
  /* -------- */
  /* This reloc does nothing. */
  HOWTO (R_FLARE_NONE,        /* type */
      0,                        /* rightshift */
      3,                        /* size (0 = byte, 1 = short, 2 = long) */
      0,                        /* bitsize */
      false,                    /* pc_relative */
      0,                        /* bitpos */
      complain_overflow_dont,   /* complain_on_overflow */
      bfd_elf_generic_reloc,    /* special_function */
      "R_FLARE_NONE",         /* name */
      false,                    /* partial_inplace */
      0,                        /* src_mask */
      0,                        /* dst_mask */
      false),                   /* pcrel_offset */
  /* -------- */
  HOWTO (R_FLARE_8,           /* type */
      0,                        /* rightshift */
      0,                        /* size (0 = byte, 1 = short, 2 = long) */
      8,                        /* bitsize */
      false,                    /* pc_relative */
      0,                        /* bitpos */
      complain_overflow_bitfield, /* complain_on_overflow */
      bfd_elf_generic_reloc,    /* special_function */
      "R_FLARE_8",            /* name */
      false,                    /* partial_inplace */
      0x00,                     /* src_mask */
      0xff,                     /* dst_mask */
      false),                   /* pcrel_offset */
  HOWTO (R_FLARE_16,          /* type */
      0,                        /* rightshift */
      1,                        /* size (0 = byte, 1 = short, 2 = long) */
      16,                       /* bitsize */
      false,                    /* pc_relative */
      0,                        /* bitpos */
      complain_overflow_bitfield, /* complain_on_overflow */
      bfd_elf_generic_reloc,    /* special_function */
      "R_FLARE_16",           /* name */
      false,                    /* partial_inplace */
      0x0000,                   /* src_mask */
      0xffff,                   /* dst_mask */
      false),                   /* pcrel_offset */
  /* A 32-bit relocation. */
  HOWTO (R_FLARE_32,          /* type */
      0,                        /* rightshift */
      2,                        /* size (0 = byte, 1 = short, 2 = long) */
      32,                       /* bitsize */
      false,                    /* pc_relative */
      0,                        /* bitpos */
      complain_overflow_bitfield, /* complain_on_overflow */
      bfd_elf_generic_reloc,    /* special_function */
      "R_FLARE_32",           /* name */
      false,                    /* partial_inplace */
      0x00000000,               /* src_mask */
      0xffffffff,               /* dst_mask */
      false),                   /* pcrel_offset */
  /* 64-bit relocation.  */
  HOWTO (R_FLARE_64,          /* type */
      0,                        /* rightshift */
      8,                        /* size */
      64,                       /* bitsize */
      false,                    /* pc_relative */
      0,                        /* bitpos */
      complain_overflow_dont,   /* complain_on_overflow */
      bfd_elf_generic_reloc,    /* special_function */
      "R_FLARE_64",           /* name */
      false,                    /* partial_inplace */
      0,                        /* src_mask */
      MINUS_ONE,                /* dst_mask */
      false),                   /* pcrel_offset */
  /* -------- */
  /* A relocation for the immediate value of an instruction in 
  group 1, 5, or 6 without `pre`/`lpre` */
  HOWTO (R_FLARE_G1G5G6_U5,   /* type */
      0,                        /* rightshift */
      1,                        /* size (0 = byte, 1 = short, 2 = long) */
      5,                        /* bitsize */
      false,                    /* pc_relative */
      FLARE_G1G5G6_I5_BITPOS, /* bitpos */
      complain_overflow_unsigned, /* complain_on_overflow */
      flare_elf_non_sub_imm_reloc,    /* special_function */
      "R_FLARE_G1G5G6_U5",    /* name */
      false,                    /* partial_inplace */
      0x0000,                   /* src_mask */
      FLARE_G1G5G6_I5_MASK,   /* dst_mask */
      false),                   /* pcrel_offset */

  /* A relocation for the immediate value of an instruction in 
  group 1, 5, or 6 with `pre` */
  HOWTO (R_FLARE_G1G5G6_S17_FOR_U5,  /* type */
      0,                        /* rightshift */
      2,                        /* size (0 = byte, 1 = short, 2 = long) */
      17,                       /* bitsize */
      false,                    /* pc_relative */
      FLARE_G1G5G6_I5_BITPOS, /* bitpos */
      complain_overflow_dont,   /* complain_on_overflow */
      flare_elf_non_sub_imm_reloc,  /* special_function */
      "R_FLARE_G1G5G6_S17_FOR_U5",   /* name */
      false,                    /* partial_inplace */
      0x00000000,               /* src_mask */
      /* dst_mask */
      (FLARE_G0_PRE_S12_EXT_LSMASK | FLARE_G1G5G6_I5_MASK), //0x0fff1f00,
      false),                   /* pcrel_offset */

  /* A relocation for the immediate value of an instruction in 
  group 1, 5, or 6 with `lpre` */
  HOWTO (R_FLARE_G1G5G6_S32_FOR_U5,  /* type */
      0,                        /* rightshift */
      4,                        /* size (0 = byte, 1 = short, 2 = long) */
      32,                       /* bitsize */
      false,                    /* pc_relative */
      FLARE_G1G5G6_I5_BITPOS, /* bitpos */
      complain_overflow_dont,   /* complain_on_overflow */
      flare_elf_non_sub_imm_reloc,  /* special_function */
      "R_FLARE_G1G5G6_S32_FOR_U5",   /* name */
      false,                    /* partial_inplace */
      0x0,                      /* src_mask */
      /* dst_mask */
      //(bfd_vma)0x07ffffff1f00, 
      (FLARE_G0_LPRE_S27_EXT | FLARE_G1G5G6_I5_MASK),
      false),                   /* pcrel_offset */

  HOWTO (R_FLARE_G1G5G6_S32_FOR_U5_ADD32,  /* type */
      0,                        /* rightshift */
      4,                        /* size (0 = byte, 1 = short, 2 = long) */
      32,                       /* bitsize */
      false,                    /* pc_relative */
      FLARE_G1G5G6_I5_BITPOS, /* bitpos */
      complain_overflow_dont,   /* complain_on_overflow */
      flare_elf_add_sub_reloc,  /* special_function */
      "R_FLARE_G1G5G6_S32_FOR_U5_ADD32",   /* name */
      false,                    /* partial_inplace */
      0x0,                      /* src_mask */
      /* dst_mask */
      //(bfd_vma)0x07ffffff1f00, 
      (FLARE_G0_LPRE_S27_EXT | FLARE_G1G5G6_I5_MASK),
      false),                   /* pcrel_offset */
  HOWTO (R_FLARE_G1G5G6_S32_FOR_U5_SUB32,  /* type */
      0,                        /* rightshift */
      4,                        /* size (0 = byte, 1 = short, 2 = long) */
      32,                       /* bitsize */
      false,                    /* pc_relative */
      FLARE_G1G5G6_I5_BITPOS, /* bitpos */
      complain_overflow_dont,   /* complain_on_overflow */
      flare_elf_add_sub_reloc,  /* special_function */
      "R_FLARE_G1G5G6_S32_FOR_U5_SUB32",   /* name */
      false,                    /* partial_inplace */
      0x0,                      /* src_mask */
      /* dst_mask */
      //(bfd_vma)0x07ffffff1f00, 
      (FLARE_G0_LPRE_S27_EXT | FLARE_G1G5G6_I5_MASK),
      false),                   /* pcrel_offset */

  HOWTO (R_FLARE_G1G5G6_S32_FOR_U5_NO_RELAX,  /* type */
      0,                        /* rightshift */
      4,                        /* size (0 = byte, 1 = short, 2 = long) */
      32,                       /* bitsize */
      false,                    /* pc_relative */
      FLARE_G1G5G6_I5_BITPOS, /* bitpos */
      complain_overflow_dont,   /* complain_on_overflow */
      flare_elf_non_sub_imm_reloc,  /* special_function */
      "R_FLARE_G1G5G6_S32_FOR_U5_NO_RELAX",   /* name */
      false,                    /* partial_inplace */
      0x0,                      /* src_mask */
      /* dst_mask */
      //(bfd_vma)0x07ffffff1f00, 
      (FLARE_G0_LPRE_S27_EXT | FLARE_G1G5G6_I5_MASK),
      false),                   /* pcrel_offset */
  /* A relocation for the immediate value of an instruction in 
  group 1, 5, or 6 without `pre`/`lpre` */
  HOWTO (R_FLARE_G1G5G6_S5,   /* type */
      0,                        /* rightshift */
      1,                        /* size (0 = byte, 1 = short, 2 = long) */
      5,                        /* bitsize */
      false,                    /* pc_relative */
      FLARE_G1G5G6_I5_BITPOS, /* bitpos */
      complain_overflow_signed, /* complain_on_overflow */
      flare_elf_non_sub_imm_reloc,    /* special_function */
      "R_FLARE_G1G5G6_S5",    /* name */
      false,                    /* partial_inplace */
      0x0000,                   /* src_mask */
      FLARE_G1G5G6_I5_MASK,   /* dst_mask */
      false),                   /* pcrel_offset */

  /* A relocation for the immediate value of an instruction in 
  group 1, 5, or 6 with `pre` */
  HOWTO (R_FLARE_G1G5G6_S17,  /* type */
      0,                        /* rightshift */
      2,                        /* size (0 = byte, 1 = short, 2 = long) */
      17,                       /* bitsize */
      false,                    /* pc_relative */
      FLARE_G1G5G6_I5_BITPOS, /* bitpos */
      complain_overflow_dont,   /* complain_on_overflow */
      flare_elf_non_sub_imm_reloc,  /* special_function */
      "R_FLARE_G1G5G6_S17",   /* name */
      false,                    /* partial_inplace */
      0x00000000,               /* src_mask */
      /* dst_mask */
      (FLARE_G0_PRE_S12_EXT_LSMASK | FLARE_G1G5G6_I5_MASK), //0x0fff1f00,
      false),                   /* pcrel_offset */

  /* A relocation for the immediate value of an instruction in 
  group 1, 5, or 6 with `lpre` */
  HOWTO (R_FLARE_G1G5G6_S32,  /* type */
      0,                        /* rightshift */
      4,                        /* size (0 = byte, 1 = short, 2 = long) */
      32,                       /* bitsize */
      false,                    /* pc_relative */
      FLARE_G1G5G6_I5_BITPOS, /* bitpos */
      complain_overflow_dont,   /* complain_on_overflow */
      flare_elf_non_sub_imm_reloc,  /* special_function */
      "R_FLARE_G1G5G6_S32",   /* name */
      false,                    /* partial_inplace */
      0x0,                      /* src_mask */
      /* dst_mask */
      //(bfd_vma)0x07ffffff1f00, 
      (FLARE_G0_LPRE_S27_EXT | FLARE_G1G5G6_I5_MASK),
      false),                   /* pcrel_offset */

  HOWTO (R_FLARE_G1G5G6_S32_ADD32,  /* type */
      0,                        /* rightshift */
      4,                        /* size (0 = byte, 1 = short, 2 = long) */
      32,                       /* bitsize */
      false,                    /* pc_relative */
      FLARE_G1G5G6_I5_BITPOS, /* bitpos */
      complain_overflow_dont,   /* complain_on_overflow */
      flare_elf_add_sub_reloc,  /* special_function */
      "R_FLARE_G1G5G6_S32_ADD32",   /* name */
      false,                    /* partial_inplace */
      0x0,                      /* src_mask */
      /* dst_mask */
      //(bfd_vma)0x07ffffff1f00, 
      (FLARE_G0_LPRE_S27_EXT | FLARE_G1G5G6_I5_MASK),
      false),                   /* pcrel_offset */
  HOWTO (R_FLARE_G1G5G6_S32_SUB32,  /* type */
      0,                        /* rightshift */
      4,                        /* size (0 = byte, 1 = short, 2 = long) */
      32,                       /* bitsize */
      false,                    /* pc_relative */
      FLARE_G1G5G6_I5_BITPOS,	/* bitpos */
      complain_overflow_dont,   /* complain_on_overflow */
      flare_elf_add_sub_reloc,  /* special_function */
      "R_FLARE_G1G5G6_S32_SUB32",   /* name */
      false,                    /* partial_inplace */
      0x0,                      /* src_mask */
      /* dst_mask */
      //(bfd_vma)0x07ffffff1f00, 
      (FLARE_G0_LPRE_S27_EXT | FLARE_G1G5G6_I5_MASK),
      false),                   /* pcrel_offset */

  HOWTO (R_FLARE_G1G5G6_S32_NO_RELAX,  /* type */
      0,                        /* rightshift */
      4,                        /* size (0 = byte, 1 = short, 2 = long) */
      32,                       /* bitsize */
      false,                    /* pc_relative */
      FLARE_G1G5G6_I5_BITPOS, /* bitpos */
      complain_overflow_dont,   /* complain_on_overflow */
      flare_elf_non_sub_imm_reloc,  /* special_function */
      "R_FLARE_G1G5G6_S32_NO_RELAX",   /* name */
      false,                    /* partial_inplace */
      0x0,                      /* src_mask */
      /* dst_mask */
      //(bfd_vma)0x07ffffff1f00, 
      (FLARE_G0_LPRE_S27_EXT | FLARE_G1G5G6_I5_MASK),
      //(flare_temp_t)(0ll),
      false),                   /* pcrel_offset */
  /* -------- */
  /* A relocation for the immediate value of an instruction in 
  group 3 without `pre`/`lpre` */
  HOWTO (R_FLARE_G3_S9_PCREL, /* type */
      1,                        /* rightshift */
      1,                        /* size (0 = byte, 1 = short, 2 = long) */
      FLARE_G3_S9_BITSIZE,    /* bitsize */
      true,                     /* pc_relative */
      FLARE_G3_S9_BITPOS,     /* bitpos */
      complain_overflow_signed,   /* complain_on_overflow */
      bfd_elf_generic_reloc,    /* special_function */
      "R_FLARE_G3_S9_PCREL",  /* name */
      false,                    /* partial_inplace */
      0x0000,                   /* src_mask */
      /* dst_mask */
      FLARE_G3_S9_MASK, //0x1ff0
      true),                    /* pcrel_offset */

  /* A relocation for the immediate value of an instruction in 
  group 3 with `pre` */
  HOWTO (R_FLARE_G3_S21_PCREL,  /* type */
      1,                          /* rightshift */
      2,                          /* size (0 = byte, 1 = short, 2 = long) */
      21,                         /* bitsize */
      true,                       /* pc_relative */
      FLARE_G3_S9_BITPOS,       /* bitpos */
      complain_overflow_dont,     /* complain_on_overflow */
      flare_elf_non_sub_imm_reloc, /* special_function */
      "R_FLARE_G3_S21_PCREL",   /* name */
      false,                      /* partial_inplace */
      0x00000000,                 /* src_mask */
      /* dst_mask */
      (FLARE_G0_PRE_S12_EXT_LSMASK | FLARE_G3_S9_MASK), //0x07ff1ff0,
      true),                      /* pcrel_offset */

  /* A relocation for the immediate value of an instruction in 
  group 3 with `lpre` */
  HOWTO (R_FLARE_G3_S32_PCREL,  /* type */
      1,                          /* rightshift */
      4,                          /* size (0 = byte, 1 = short, 2 = long) */
      32,                         /* bitsize */
      true,                       /* pc_relative */
      FLARE_G3_S9_BITPOS,       /* bitpos */
      complain_overflow_dont,     /* complain_on_overflow */
      flare_elf_non_sub_imm_reloc, /* special_function */
      "R_FLARE_G3_S32_PCREL",   /* name */
      false,                      /* partial_inplace */
      0x0,                        /* src_mask */
      /* dst_mask */
      (FLARE_G0_LPRE_S23_EXT_LSMASK | FLARE_G3_S9_MASK),
      true),                     /* pcrel_offset */
  //HOWTO (R_FLARE_G3_S32_PCREL_ADD32,  /* type */
  //    1,                          /* rightshift */
  //    4,                          /* size (0 = byte, 1 = short, 2 = long) */
  //    32,                         /* bitsize */
  //    true,                       /* pc_relative */
  //    FLARE_G3_S9_BITPOS,       /* bitpos */
  //    complain_overflow_dont,     /* complain_on_overflow */
  //    flare_elf_add_sub_reloc, /* special_function */
  //    "R_FLARE_G3_S32_PCREL_ADD32",   /* name */
  //    false,                      /* partial_inplace */
  //    0x0,                        /* src_mask */
  //    /* dst_mask */
  //    (FLARE_G0_LPRE_S23_EXT_LSMASK | FLARE_G3_S9_MASK),
  //    true),                     /* pcrel_offset */
  //HOWTO (R_FLARE_G3_S32_PCREL_SUB32,  /* type */
  //    1,                          /* rightshift */
  //    4,                          /* size (0 = byte, 1 = short, 2 = long) */
  //    32,                         /* bitsize */
  //    true,                       /* pc_relative */
  //    FLARE_G3_S9_BITPOS,       /* bitpos */
  //    complain_overflow_dont,     /* complain_on_overflow */
  //    flare_elf_add_sub_reloc, /* special_function */
  //    "R_FLARE_G3_S32_PCREL_SUB32",   /* name */
  //    false,                      /* partial_inplace */
  //    0x0,                        /* src_mask */
  //    /* dst_mask */
  //    (FLARE_G0_LPRE_S23_EXT_LSMASK | FLARE_G3_S9_MASK),
  //    true),                     /* pcrel_offset */
  HOWTO (R_FLARE_G3_S32_PCREL_NO_RELAX,  /* type */
      1,                          /* rightshift */
      4,                          /* size (0 = byte, 1 = short, 2 = long) */
      32,                         /* bitsize */
      true,                       /* pc_relative */
      FLARE_G3_S9_BITPOS,       /* bitpos */
      complain_overflow_dont,     /* complain_on_overflow */
      flare_elf_non_sub_imm_reloc, /* special_function */
      "R_FLARE_G3_S32_PCREL_NO_RELAX",   /* name */
      false,                      /* partial_inplace */
      0x0,                        /* src_mask */
      /* dst_mask */
      (FLARE_G0_LPRE_S23_EXT_LSMASK | FLARE_G3_S9_MASK),
      true),                     /* pcrel_offset */

  /* -------- */
  /* A relocation for the immediate value of the `icreload` instruction
    group 7 without `pre`/`lpre` */
  HOWTO (R_FLARE_G7_ICRELOAD_S5,  /* type */
      0,                            /* rightshift */
      1,                            /* size (0 = byte, 1 = short, 2 = long) */
      5,                            /* bitsize */
      false,                        /* pc_relative */
      FLARE_G7_ICRELOAD_S5_BITPOS,  /* bitpos */
      complain_overflow_signed,     /* complain_on_overflow */
      flare_elf_non_sub_imm_reloc,  /* special_function */
      "R_FLARE_G7_ICRELOAD_S5",   /* name */
      false,                        /* partial_inplace */
      0x0000,                       /* src_mask */
      FLARE_G7_ICRELOAD_S5_MASK,  /* dst_mask */
      false),                   /* pcrel_offset */

  /* A relocation for the immediate value of the `icreload` instruction
    group 7 with `pre` */
  HOWTO (R_FLARE_G7_ICRELOAD_S17,  /* type */
      0,                          /* rightshift */
      2,                          /* size (0 = byte, 1 = short, 2 = long) */
      17,                         /* bitsize */
      false,                      /* pc_relative */
      FLARE_G7_ICRELOAD_S5_BITPOS, /* bitpos */
      complain_overflow_dont,     /* complain_on_overflow */
      flare_elf_non_sub_imm_reloc,  /* special_function */
      "R_FLARE_G7_ICRELOAD_S17",   /* name */
      false,                      /* partial_inplace */
      0x00000000,                 /* src_mask */
      /* dst_mask */
      (FLARE_G0_PRE_S12_EXT_LSMASK | FLARE_G7_ICRELOAD_S5_MASK), //0x0fff01f0,
      false),                   /* pcrel_offset */

  /* A relocation for the immediate value of the `icreload` instruction
    group 7 with `lpre` */
  HOWTO (R_FLARE_G7_ICRELOAD_S32,  /* type */
      0,                        /* rightshift */
      4,                        /* size (0 = byte, 1 = short, 2 = long) */
      32,                       /* bitsize */
      false,                    /* pc_relative */
      FLARE_G7_ICRELOAD_S5_BITPOS, /* bitpos */
      complain_overflow_dont,   /* complain_on_overflow */
      flare_elf_non_sub_imm_reloc,  /* special_function */
      "R_FLARE_G7_ICRELOAD_S32",   /* name */
      false,                    /* partial_inplace */
      0x0,                      /* src_mask */
      /* dst_mask */
      //(bfd_vma)0x07ffffff01f0, 
      (FLARE_G0_LPRE_S27_EXT_LSMASK | FLARE_G7_ICRELOAD_S5_MASK),
      false),                   /* pcrel_offset */

  HOWTO (R_FLARE_G7_ICRELOAD_S32_ADD32,  /* type */
      0,                        /* rightshift */
      4,                        /* size (0 = byte, 1 = short, 2 = long) */
      32,                       /* bitsize */
      false,                    /* pc_relative */
      FLARE_G7_ICRELOAD_S5_BITPOS, /* bitpos */
      complain_overflow_dont,   /* complain_on_overflow */
      flare_elf_add_sub_reloc,  /* special_function */
      "R_FLARE_G7_ICRELOAD_S32_ADD32",   /* name */
      false,                    /* partial_inplace */
      0x0,                      /* src_mask */
      /* dst_mask */
      //(bfd_vma)0x07ffffff01f0, 
      (FLARE_G0_LPRE_S27_EXT_LSMASK | FLARE_G7_ICRELOAD_S5_MASK),
      false),                   /* pcrel_offset */
  HOWTO (R_FLARE_G7_ICRELOAD_S32_SUB32,  /* type */
      0,                        /* rightshift */
      4,                        /* size (0 = byte, 1 = short, 2 = long) */
      32,                       /* bitsize */
      false,                    /* pc_relative */
      FLARE_G7_ICRELOAD_S5_BITPOS, /* bitpos */
      complain_overflow_dont,   /* complain_on_overflow */
      flare_elf_add_sub_reloc,  /* special_function */
      "R_FLARE_G7_ICRELOAD_S32_SUB32",   /* name */
      false,                    /* partial_inplace */
      0x0,                      /* src_mask */
      /* dst_mask */
      //(bfd_vma)0x07ffffff01f0, 
      (FLARE_G0_LPRE_S27_EXT_LSMASK | FLARE_G7_ICRELOAD_S5_MASK),
      false),                   /* pcrel_offset */

  HOWTO (R_FLARE_G7_ICRELOAD_S32_NO_RELAX,  /* type */
      0,                        /* rightshift */
      4,                        /* size (0 = byte, 1 = short, 2 = long) */
      32,                       /* bitsize */
      false,                    /* pc_relative */
      FLARE_G7_ICRELOAD_S5_BITPOS, /* bitpos */
      complain_overflow_dont,   /* complain_on_overflow */
      flare_elf_non_sub_imm_reloc,  /* special_function */
      "R_FLARE_G7_ICRELOAD_S32_NO_RELAX",   /* name */
      false,                    /* partial_inplace */
      0x0,                      /* src_mask */
      /* dst_mask */
      //(bfd_vma)0x07ffffff01f0, 
      (FLARE_G0_LPRE_S27_EXT_LSMASK | FLARE_G7_ICRELOAD_S5_MASK),
      false),                   /* pcrel_offset */
  /* -------- */
  //HOWTO (R_FLARE_FDE_32_PCREL,    /* type */
  //    0,                          /* rightshift */
  //    4,                          /* size */
  //    32,                         /* bitsize */
  //    true,                       /* pc_relative */
  //    0,                          /* bitpos */
  //    complain_overflow_dont,     /* complain_on_overflow */
  //    bfd_elf_generic_reloc,      /* special_function */
  //    "R_FLARE_FDE_32_PCREL",       /* name */
  //    false,                      /* partial_inplace */
  //    0,                          /* src_mask */
  //    0xffffffff,                 /* dst_mask */
  //    false),                     /* pcrel_offset */
  /* -------- */
  /* 8-bit in-place setting, for local label subtraction.  */
  HOWTO (R_FLARE_SET8,            /* type */
      0,                            /* rightshift */
      1,                            /* size */
      8,                            /* bitsize */
      false,                        /* pc_relative */
      0,                            /* bitpos */
      complain_overflow_dont,       /* complain_on_overflow */
      bfd_elf_generic_reloc,        /* special_function */
      "R_FLARE_SET8",             /* name */
      false,                        /* partial_inplace */
      0,                            /* src_mask */
      0xff,                         /* dst_mask */
      false),                       /* pcrel_offset */

  /* 16-bit in-place setting, for local label subtraction.  */
  HOWTO (R_FLARE_SET16,         /* type */
    0,                            /* rightshift */
    2,                            /* size */
    16,                           /* bitsize */
    false,                        /* pc_relative */
    0,                            /* bitpos */
    complain_overflow_dont,       /* complain_on_overflow */
    bfd_elf_generic_reloc,        /* special_function */
    "R_FLARE_SET16",            /* name */
    false,                        /* partial_inplace */
    0,                            /* src_mask */
    0xffff,                       /* dst_mask */
    false),                       /* pcrel_offset */

  /* 32-bit in-place setting, for local label subtraction.  */
  HOWTO (R_FLARE_SET32,             /* type */
      0,                            /* rightshift */
      4,                            /* size */
      32,                           /* bitsize */
      false,                        /* pc_relative */
      0,                            /* bitpos */
      complain_overflow_dont,       /* complain_on_overflow */
      bfd_elf_generic_reloc,        /* special_function */
      "R_FLARE_SET32",            /* name */
      false,                        /* partial_inplace */
      0,                            /* src_mask */
      0xffffffff,                   /* dst_mask */
      false),                       /* pcrel_offset */
  /* -------- */
  /* 8-bit in-place addition, for local label subtraction.  */
  HOWTO (R_FLARE_PSEUDO_ADD8,                /* type */
         0,                             /* rightshift */
         1,                             /* size */
         8,                             /* bitsize */
         false,                         /* pc_relative */
         0,                             /* bitpos */
         complain_overflow_dont,        /* complain_on_overflow */
         flare_elf_add_sub_reloc,     /* special_function */
         "R_FLARE_PSEUDO_ADD8",       /* name */
         false,                         /* partial_inplace */
         0,                             /* src_mask */
         0xff,                          /* dst_mask */
         false),                        /* pcrel_offset */

  /* 16-bit in-place addition, for local label subtraction.  */
  HOWTO (R_FLARE_PSEUDO_ADD16,                 /* type */
         0,                             /* rightshift */
         2,                             /* size */
         16,                            /* bitsize */
         false,                         /* pc_relative */
         0,                             /* bitpos */
         complain_overflow_dont,        /* complain_on_overflow */
         flare_elf_add_sub_reloc,     /* special_function */
         "R_FLARE_PSEUDO_ADD16",      /* name */
         false,                         /* partial_inplace */
         0,                             /* src_mask */
         0xffff,                        /* dst_mask */
         false),                        /* pcrel_offset */

  /* 32-bit in-place addition, for local label subtraction.  */
  HOWTO (R_FLARE_PSEUDO_ADD32,                 /* type */
         0,                             /* rightshift */
         4,                             /* size */
         32,                            /* bitsize */
         false,                         /* pc_relative */
         0,                             /* bitpos */
         complain_overflow_dont,        /* complain_on_overflow */
         flare_elf_add_sub_reloc,     /* special_function */
         "R_FLARE_PSEUDO_ADD32",      /* name */
         false,                         /* partial_inplace */
         0,                             /* src_mask */
         0xffffffff,                    /* dst_mask */
         false),                        /* pcrel_offset */

  /* 64-bit in-place addition, for local label subtraction.  */
  HOWTO (R_FLARE_PSEUDO_ADD64,                 /* type */
         0,                             /* rightshift */
         8,                             /* size */
         64,                            /* bitsize */
         false,                         /* pc_relative */
         0,                             /* bitpos */
         complain_overflow_dont,        /* complain_on_overflow */
         flare_elf_add_sub_reloc,     /* special_function */
         "R_FLARE_PSEUDO_ADD64",      /* name */
         false,                         /* partial_inplace */
         0,                             /* src_mask */
         MINUS_ONE,                     /* dst_mask */
         false),                        /* pcrel_offset */

  /* 8-bit in-place addition, for local label subtraction.  */
  HOWTO (R_FLARE_PSEUDO_SUB8,                  /* type */
         0,                             /* rightshift */
         1,                             /* size */
         8,                             /* bitsize */
         false,                         /* pc_relative */
         0,                             /* bitpos */
         complain_overflow_dont,        /* complain_on_overflow */
         flare_elf_add_sub_reloc,     /* special_function */
         "R_FLARE_PSEUDO_SUB8",       /* name */
         false,                         /* partial_inplace */
         0,                             /* src_mask */
         0xff,                          /* dst_mask */
         false),                        /* pcrel_offset */

  /* 16-bit in-place addition, for local label subtraction.  */
  HOWTO (R_FLARE_PSEUDO_SUB16,                 /* type */
         0,                             /* rightshift */
         2,                             /* size */
         16,                            /* bitsize */
         false,                         /* pc_relative */
         0,                             /* bitpos */
         complain_overflow_dont,        /* complain_on_overflow */
         flare_elf_add_sub_reloc,     /* special_function */
         "R_FLARE_PSEUDO_SUB16",      /* name */
         false,                         /* partial_inplace */
         0,                             /* src_mask */
         0xffff,                        /* dst_mask */
         false),                        /* pcrel_offset */

  /* 32-bit in-place addition, for local label subtraction.  */
  HOWTO (R_FLARE_PSEUDO_SUB32,                 /* type */
         0,                             /* rightshift */
         4,                             /* size */
         32,                            /* bitsize */
         false,                         /* pc_relative */
         0,                             /* bitpos */
         complain_overflow_dont,        /* complain_on_overflow */
         flare_elf_add_sub_reloc,     /* special_function */
         "R_FLARE_PSEUDO_SUB32",      /* name */
         false,                         /* partial_inplace */
         0,                             /* src_mask */
         0xffffffff,                    /* dst_mask */
         false),                        /* pcrel_offset */

  /* 64-bit in-place addition, for local label subtraction.  */
  HOWTO (R_FLARE_PSEUDO_SUB64,                 /* type */
         0,                             /* rightshift */
         8,                             /* size */
         64,                            /* bitsize */
         false,                         /* pc_relative */
         0,                             /* bitpos */
         complain_overflow_dont,        /* complain_on_overflow */
         flare_elf_add_sub_reloc,     /* special_function */
         "R_FLARE_PSEUDO_SUB64",      /* name */
         false,                         /* partial_inplace */
         0,                             /* src_mask */
         MINUS_ONE,                     /* dst_mask */
         false),                        /* pcrel_offset */
  /* -------- */
  /* 6-bit in-place setting, for local label subtraction.  */
  HOWTO (R_FLARE_CFA_SET6,            /* type */
      0,                            /* rightshift */
      1,                            /* size */
      8,                            /* bitsize */
      false,                        /* pc_relative */
      0,                            /* bitpos */
      complain_overflow_dont,       /* complain_on_overflow */
      bfd_elf_generic_reloc,        /* special_function */
      "R_FLARE_CFA_SET6",             /* name */
      false,                        /* partial_inplace */
      0,                            /* src_mask */
      0x3f,                         /* dst_mask */
      false),                       /* pcrel_offset */
  /* 6-bit in-place addition, for local label subtraction.  */
  HOWTO (R_FLARE_CFA_SUB6,            /* type */
      0,                            /* rightshift */
      1,                            /* size */
      8,                            /* bitsize */
      false,                        /* pc_relative */
      0,                            /* bitpos */
      complain_overflow_dont,       /* complain_on_overflow */
      flare_elf_add_sub_reloc,    /* special_function */
      "R_FLARE_CFA_SUB6",             /* name */
      false,                        /* partial_inplace */
      0,                            /* src_mask */
      0x3f,                         /* dst_mask */
      false),                       /* pcrel_offset */
  /* -------- */
  HOWTO (R_FLARE_EH_32_PCREL,     /* type */
      0,                            /* rightshift */
      4,                            /* size */
      32,                           /* bitsize */
      true,                         /* pc_relative */
      0,                            /* bitpos */
      complain_overflow_dont,       /* complain_on_overflow */
      bfd_elf_generic_reloc,        /* special_function */
      "R_FLARE_EH_32_PCREL",      /* name */
      false,                        /* partial_inplace */
      0,                            /* src_mask */
      MINUS_ONE,                    /* dst_mask */
      false),                       /* pcrel_offset */
  /* -------- */
};

struct flare_reloc_map
{
  bfd_reloc_code_real_type bfd_reloc_val;
  unsigned int flare_reloc_val;
};

static const struct flare_reloc_map flare_reloc_map [] =
{
  /* -------- */
  { BFD_RELOC_NONE, R_FLARE_NONE },
  { BFD_RELOC_8, R_FLARE_8 },
  { BFD_RELOC_16, R_FLARE_16 },
  { BFD_RELOC_32, R_FLARE_32 },
  { BFD_RELOC_64, R_FLARE_64 },
  /* -------- */
  { BFD_RELOC_FLARE_G1G5G6_U5, R_FLARE_G1G5G6_U5 },
  { BFD_RELOC_FLARE_G1G5G6_S17_FOR_U5, R_FLARE_G1G5G6_S17_FOR_U5 },
  { BFD_RELOC_FLARE_G1G5G6_S32_FOR_U5, R_FLARE_G1G5G6_S32_FOR_U5 },
  { BFD_RELOC_FLARE_G1G5G6_S32_FOR_U5_ADD32,
    R_FLARE_G1G5G6_S32_FOR_U5_ADD32 },
  { BFD_RELOC_FLARE_G1G5G6_S32_FOR_U5_SUB32,
    R_FLARE_G1G5G6_S32_FOR_U5_SUB32 },
  { BFD_RELOC_FLARE_G1G5G6_S32_FOR_U5_NO_RELAX,
    R_FLARE_G1G5G6_S32_FOR_U5_NO_RELAX },

  { BFD_RELOC_FLARE_G1G5G6_S5, R_FLARE_G1G5G6_S5 },
  { BFD_RELOC_FLARE_G1G5G6_S17, R_FLARE_G1G5G6_S17 },
  { BFD_RELOC_FLARE_G1G5G6_S32, R_FLARE_G1G5G6_S32 },
  { BFD_RELOC_FLARE_G1G5G6_S32_ADD32, R_FLARE_G1G5G6_S32_ADD32 },
  { BFD_RELOC_FLARE_G1G5G6_S32_SUB32, R_FLARE_G1G5G6_S32_SUB32 },
  { BFD_RELOC_FLARE_G1G5G6_S32_NO_RELAX, R_FLARE_G1G5G6_S32_NO_RELAX },
  /* -------- */
  { BFD_RELOC_FLARE_G3_S9_PCREL, R_FLARE_G3_S9_PCREL },
  { BFD_RELOC_FLARE_G3_S21_PCREL, R_FLARE_G3_S21_PCREL },
  { BFD_RELOC_FLARE_G3_S32_PCREL, R_FLARE_G3_S32_PCREL },
  //{ BFD_RELOC_FLARE_G3_S32_PCREL_ADD32, R_FLARE_G3_S32_PCREL_ADD32 },
  //{ BFD_RELOC_FLARE_G3_S32_PCREL_SUB32, R_FLARE_G3_S32_PCREL_SUB32 },
  { BFD_RELOC_FLARE_G3_S32_PCREL_NO_RELAX,
    R_FLARE_G3_S32_PCREL_NO_RELAX },
  /* -------- */
  { BFD_RELOC_FLARE_G7_ICRELOAD_S5, R_FLARE_G7_ICRELOAD_S5 },
  { BFD_RELOC_FLARE_G7_ICRELOAD_S17, R_FLARE_G7_ICRELOAD_S17 },
  { BFD_RELOC_FLARE_G7_ICRELOAD_S32, R_FLARE_G7_ICRELOAD_S32 },
  { BFD_RELOC_FLARE_G7_ICRELOAD_S32_ADD32,
    R_FLARE_G7_ICRELOAD_S32_ADD32 },
  { BFD_RELOC_FLARE_G7_ICRELOAD_S32_SUB32,
    R_FLARE_G7_ICRELOAD_S32_SUB32 },
  { BFD_RELOC_FLARE_G7_ICRELOAD_S32_NO_RELAX,
    R_FLARE_G7_ICRELOAD_S32_NO_RELAX },
  /* -------- */
  //{ BFD_RELOC_FLARE_FDE_32_PCREL, R_FLARE_FDE_32_PCREL },
  { BFD_RELOC_FLARE_SET8, R_FLARE_SET8 },
  { BFD_RELOC_FLARE_SET16, R_FLARE_SET16 },
  { BFD_RELOC_FLARE_SET32, R_FLARE_SET32 },
  /* -------- */
  //{ BFD_RELOC_CTOR, R_FLARE_32 },
  /* -------- */
  { BFD_RELOC_FLARE_PSEUDO_ADD8, R_FLARE_PSEUDO_ADD8 },
  { BFD_RELOC_FLARE_PSEUDO_ADD16, R_FLARE_PSEUDO_ADD16 },
  { BFD_RELOC_FLARE_PSEUDO_ADD32, R_FLARE_PSEUDO_ADD32 },
  { BFD_RELOC_FLARE_PSEUDO_ADD64, R_FLARE_PSEUDO_ADD64 },
  { BFD_RELOC_FLARE_PSEUDO_SUB8, R_FLARE_PSEUDO_SUB8 },
  { BFD_RELOC_FLARE_PSEUDO_SUB16, R_FLARE_PSEUDO_SUB16 },
  { BFD_RELOC_FLARE_PSEUDO_SUB32, R_FLARE_PSEUDO_SUB32 },
  { BFD_RELOC_FLARE_PSEUDO_SUB64, R_FLARE_PSEUDO_SUB64 },
  /* -------- */
  { BFD_RELOC_FLARE_CFA_SET6, R_FLARE_CFA_SET6 },
  { BFD_RELOC_FLARE_CFA_SUB6, R_FLARE_CFA_SUB6 },
  /* -------- */
  { BFD_RELOC_FLARE_EH_32_PCREL, R_FLARE_EH_32_PCREL }
  /* -------- */
};

reloc_howto_type *
flare_reloc_type_lookup (bfd *abfd ATTRIBUTE_UNUSED,
                          bfd_reloc_code_real_type code)
{
  /* -------- */
  unsigned int i;
  /* -------- */
  for (i=0; i<FLARE_ARRAY_SIZE (flare_reloc_map); ++i)
  {
    if (flare_reloc_map[i].bfd_reloc_val == code)
    {
      return &flare_elf_howto_table
        [flare_reloc_map[i].flare_reloc_val];
    }
  }
  /* -------- */
  return NULL;
  /* -------- */
}

reloc_howto_type *
flare_reloc_name_lookup (bfd *abfd ATTRIBUTE_UNUSED, const char *r_name)
{
  /* -------- */
  unsigned int i;
  /* -------- */
  for (i=0; i<FLARE_ARRAY_SIZE (flare_elf_howto_table); ++i)
  {
    if (flare_elf_howto_table[i].name != NULL
      && strcasecmp (flare_elf_howto_table[i].name, r_name) == 0)
    {
      return &flare_elf_howto_table[i];
    }
  }
  /* -------- */
  return NULL;
  /* -------- */
}
/* -------- */
/* Return the howto for relocation `r_type`.  */
static reloc_howto_type *
flare_lookup_howto (unsigned int r_type)
{
  return &flare_elf_howto_table[r_type];
}
/* -------- */
/* Implement elf_info_to_howto:
   Given a ELF32 relocation, fill in a arelent structure.  */
/* Adapted from elf32-pru.c */
static bool
flare_elf_info_to_howto (bfd *abfd,
                          arelent *cache_ptr,
                          Elf_Internal_Rela *dst)
{
  unsigned int r_type;

  r_type = ELF32_R_TYPE (dst->r_info);
  if (r_type >= R_FLARE_max)
  {
    /* xgettext:c-format */
    _bfd_error_handler (_("%pB: unsupported relocation type %#x"), abfd,
      r_type);
    bfd_set_error (bfd_error_bad_value);
    return false;
  }

  cache_ptr->howto = flare_lookup_howto (r_type);
  return cache_ptr->howto != NULL;
}
/* -------- */
//static bfd_reloc_status_type
//flare_perform_add_sub_relocation 
//  (const reloc_howto_type *howto,
//  const Elf_Internal_Rela *rel,
//  bfd_vma relocation,
//  asection *input_section,
//  bfd *input_bfd,
//  bfd_byte *contents)
//{
//  relocation += rel->r_addend;
//
//  bfd_vma word;
//  if (riscv_is_insn_reloc (howto))
//    word = riscv_get_insn (howto->bitsize, contents + rel->r_offset);
//  else
//    word = bfd_get (howto->bitsize, input_bfd, contents + rel->r_offset);
//  word = (word & ~howto->dst_mask) | (value & howto->dst_mask);
//    bfd_put (howto->bitsize, input_bfd, word, contents + rel->r_offset);
//
//  return bfd_reloc_ok;
//}
/* Special function for
  non-add32, non-sub32 relocations
  for instructions that have immediates */
static bfd_reloc_status_type
flare_elf_non_sub_imm_reloc (bfd *abfd,
                              arelent *reloc_entry,
                              asymbol *symbol,
                              void *data,
                              asection *input_section,
                              bfd *output_bfd,
                              char **error_message)
{
  //fprintf (
  //  stderr,
  //  "flare_elf_non_sub_imm_reloc(): begin\n"
  //);
  if (output_bfd != NULL
    && (symbol->flags & BSF_SECTION_SYM) == 0
    && (!reloc_entry->howto->partial_inplace
    || reloc_entry->addend == 0))
  {
    reloc_entry->address += input_section->output_offset;
    //fprintf (
    //  stderr,
    //  "flare_elf_non_sub_imm_reloc(): return bfd_reloc_ok;\n"
    //);
    return bfd_reloc_ok;
  }
  /* If this isn't a final relocation, then just use the generic
    relocation function. */
  if (output_bfd != NULL)
  {
    //return bfd_reloc_continue;
    //fprintf (
    //  stderr,
    //  "flare_elf_non_sub_imm_reloc(): output_bfd != NULL;\n"
    //);
    return bfd_elf_generic_reloc (abfd, reloc_entry, symbol, data,
      input_section, output_bfd, error_message);
  }
  //fprintf (
  //  stderr,
  //  "flare_elf_non_sub_imm_reloc(): final option;\n"
  //);
  return flare_elf_do_non_sub_imm_reloc
    (abfd, reloc_entry->howto,
    input_section,
    data,
    reloc_entry->address,
    (
      symbol->value
      + symbol->section->output_section->vma
      + symbol->section->output_offset
    ),
    reloc_entry->addend);
}
static bfd_reloc_status_type
flare_elf_do_non_sub_imm_reloc (bfd *input_bfd,
                                  reloc_howto_type *howto,
                                  asection *input_section,
                                  void *contents, bfd_vma address,
                                  bfd_vma relocation, bfd_vma addend)
{
  /* -------- */
  //reloc_howto_type *howto = reloc_entry->howto;
  //bfd_vma relocation;
  bfd_size_type octets
    = address * bfd_octets_per_byte (input_bfd, input_section);
  /* -------- */
  /* -------- */
  /* -------- */
  /* -------- */
  if (!bfd_reloc_offset_in_range (howto, input_bfd, input_section, octets))
  {
    return bfd_reloc_outofrange;
  }

  relocation += addend;
  //// Get symbol value
  //relocation = symbol->value + symbol->section->output_section->vma
  //  + symbol->section->output_offset + reloc_entry->addend;
  /* -------- */
  /* Get the data at the address */
  //bfd_vma old_value = bfd_get (howto->bitsize, abfd,
  //                            data + reloc_entry->address);

  flare_temp_t
    prefix_insn, insn;

  if (!howto->pc_relative)
  {
    BFD_ASSERT (
      howto->type == R_FLARE_G1G5G6_U5
      || howto->type == R_FLARE_G1G5G6_S17_FOR_U5
      || howto->type == R_FLARE_G1G5G6_S32_FOR_U5
      || howto->type == R_FLARE_G1G5G6_S32_FOR_U5_NO_RELAX
      || howto->type == R_FLARE_G1G5G6_S5
      || howto->type == R_FLARE_G1G5G6_S17
      || howto->type == R_FLARE_G1G5G6_S32
      || howto->type == R_FLARE_G1G5G6_S32_NO_RELAX
      || howto->type == R_FLARE_G7_ICRELOAD_S5
      || howto->type == R_FLARE_G7_ICRELOAD_S17
      || howto->type == R_FLARE_G7_ICRELOAD_S32
      || howto->type == R_FLARE_G7_ICRELOAD_S32_NO_RELAX
    );

    //fprintf (
    //  stderr,
    //  "flare_elf_do_non_sub_imm_reloc: !pc_relative: "
    //    "%i %lx; %u %u; %u %u\n",
    //  (signed) relocation,
    //  relocation,
    //  howto->type == R_FLARE_G1G5G6_S5,
    //  howto->type == R_FLARE_G1G5G6_S17,
    //  howto->type == R_FLARE_G1G5G6_S32,
    //  howto->type == R_FLARE_G1G5G6_S32_NO_RELAX
    //);

    if (howto->type == R_FLARE_G1G5G6_U5
      || howto->type == R_FLARE_G1G5G6_S5)
    {
      flare_temp_t val = 0;
      insn = bfd_get_16 (input_bfd, contents + address);
      val = flare_get_insn_field_ei
        (&flare_enc_info_g1g5g6_i5, insn);
      relocation += howto->type == R_FLARE_G1G5G6_S5
        ? flare_sign_extend
          (val, FLARE_G1G5G6_I5_BITSIZE)
        : flare_zero_extend
          (val, FLARE_G1G5G6_I5_BITSIZE);
      bfd_put_16 (input_bfd, insn, contents + address);
    }
    else if (howto->type == R_FLARE_G1G5G6_S17_FOR_U5
      || howto->type == R_FLARE_G1G5G6_S17)
    {
      const unsigned
        insn_dist = flare_have_plp_distance
          (FLARE_HAVE_PLP_PRE, FLARE_HAVE_PLP_NEITHER);
      prefix_insn = bfd_get_16 (input_bfd, contents + address);
      insn = bfd_get_16 (input_bfd, contents + address + insn_dist);
      relocation += flare_sign_extend
        (flare_get_g1g5g6_s17 (prefix_insn, insn),
        FLARE_G1G5G6_I5_BITSIZE + FLARE_G0_PRE_S12_BITSIZE);
      flare_put_g1g5g6_s17 (&prefix_insn, &insn, relocation);
      bfd_put_16 (input_bfd, prefix_insn, contents + address);
      bfd_put_16 (input_bfd, insn, contents + address + insn_dist);
    }
    else if (howto->type == R_FLARE_G1G5G6_S32_FOR_U5
      || howto->type == R_FLARE_G1G5G6_S32_FOR_U5_NO_RELAX
      || howto->type == R_FLARE_G1G5G6_S32
      || howto->type == R_FLARE_G1G5G6_S32_NO_RELAX)
    {
      const unsigned
        insn_dist = flare_have_plp_distance
          (FLARE_HAVE_PLP_LPRE, FLARE_HAVE_PLP_NEITHER);
      prefix_insn = flare_get_insn_32 (input_bfd, contents + address);
      insn = bfd_get_16 (input_bfd, contents + address + insn_dist);
      //fprintf (
      //  stderr,
      //  "debug: prefix_insn insn; 0: %x %x\n",
      //  (unsigned)prefix_insn, (unsigned)insn
      //);
      relocation += flare_get_g1g5g6_s32 (prefix_insn, insn);
      flare_put_g1g5g6_s32 (&prefix_insn, &insn, relocation);
      //fprintf (
      //  stderr,
      //  "debug: prefix_insn insn; 1: %x %x\n",
      //  (unsigned)prefix_insn, (unsigned)insn
      //);
      flare_put_insn_32 (input_bfd, prefix_insn, contents + address);
      bfd_put_16 (input_bfd, insn, contents + address + insn_dist);
    }
    else if (howto->type == R_FLARE_G7_ICRELOAD_S5)
    {
      flare_temp_t val = 0;
      insn = bfd_get_16 (input_bfd, contents + address);
      val = flare_get_insn_field_ei
        (&flare_enc_info_g7_icreload_s5, insn);
      relocation += flare_sign_extend
        (val, FLARE_G7_ICRELOAD_S5_BITSIZE);
      bfd_put_16 (input_bfd, insn, contents + address);
    }
    else if (howto->type == R_FLARE_G7_ICRELOAD_S17)
    {
      const unsigned
        insn_dist = flare_have_plp_distance
          (FLARE_HAVE_PLP_PRE, FLARE_HAVE_PLP_NEITHER);
      prefix_insn = bfd_get_16 (input_bfd, contents + address);
      insn = bfd_get_16 (input_bfd, contents + address + insn_dist);
      relocation += flare_sign_extend
        (flare_get_g7_icreload_s17 (prefix_insn, insn),
        FLARE_G7_ICRELOAD_S5_BITSIZE + FLARE_G0_PRE_S12_BITSIZE);
      flare_put_g7_icreload_s17 (&prefix_insn, &insn, relocation);
      bfd_put_16 (input_bfd, prefix_insn, contents + address);
      bfd_put_16 (input_bfd, insn, contents + address + insn_dist);
    }
    else //if (
      //howto->type == R_FLARE_G7_ICRELOAD_S32
      //|| howto->type == R_FLARE_G7_ICRELOAD_S32_NO_RELAX
    //)
    {
      const unsigned
        insn_dist = flare_have_plp_distance
          (FLARE_HAVE_PLP_LPRE, FLARE_HAVE_PLP_NEITHER);
      prefix_insn = flare_get_insn_32 (input_bfd, contents + address);
      insn = bfd_get_16 (input_bfd, contents + address + insn_dist);
      relocation += flare_get_g7_icreload_s32 (prefix_insn, insn);
      flare_put_g7_icreload_s32 (&prefix_insn, &insn, relocation);
      flare_put_insn_32 (input_bfd, prefix_insn, contents + address);
      bfd_put_16 (input_bfd, insn, contents + address + insn_dist);
    }
  }
  else // if (howto->pc_relative)
  {
    relocation -= (input_section->output_section->vma
                  + input_section->output_offset);
    if (howto->pcrel_offset)
    {
      relocation -= address;
    }

    //printf ("flare_elf_do_non_sub_imm_reloc: pc_relative: %i\n",
    //  (signed) relocation);
    BFD_ASSERT (
      howto->type == R_FLARE_G3_S9_PCREL
      || howto->type == R_FLARE_G3_S21_PCREL
      || howto->type == R_FLARE_G3_S32_PCREL
      || howto->type == R_FLARE_G3_S32_PCREL_NO_RELAX
    );

    // Check whether the relocation is word-aligned
    if (relocation & (((bfd_vma) 1 << howto->rightshift) - (bfd_vma) 1))
    {
      return bfd_reloc_outofrange;
    }

    if (howto->type == R_FLARE_G3_S9_PCREL)
    {
      const unsigned
        temp_length = flare_have_plp_insn_length
          (FLARE_HAVE_PLP_NEITHER);
      insn = bfd_get_16 (input_bfd, contents + address);
      //relocation += flare_sign_extend
      //  (flare_get_insn_field_ei (&flare_enc_info_g3_s9, insn), 
      //    FLARE_G3_S9_BITSIZE)
      //  - 2ull
      //  ;
      relocation -= temp_length;
      //flare_put_g3_s21 (&prefix_insn, &insn, relocation);
      flare_set_insn_field_ei_p (&flare_enc_info_g3_s9, &insn,
        relocation);
      bfd_put_16 (input_bfd, insn, contents + address);
    }
    else if (howto->type == R_FLARE_G3_S21_PCREL)
    {
      const unsigned
        temp_length = flare_have_plp_insn_length
          (FLARE_HAVE_PLP_PRE),
        insn_dist = flare_have_plp_distance
          (FLARE_HAVE_PLP_PRE, FLARE_HAVE_PLP_NEITHER);
      prefix_insn = bfd_get_16 (input_bfd, contents + address);
      insn = bfd_get_16 (input_bfd, contents + address + insn_dist);
      //relocation += flare_sign_extend
      //  (flare_get_g3_s21 (prefix_insn, insn),
      //  FLARE_G3_S9_BITSIZE + FLARE_G0_PRE_S12_BITSIZE)
      //  - 4ull
      //  //- 2ull
      //  ;
      relocation -= temp_length;
      flare_put_g3_s21 (&prefix_insn, &insn, relocation);
      bfd_put_16 (input_bfd, prefix_insn, contents + address);
      bfd_put_16 (input_bfd, insn, contents + address + insn_dist);
    }
    else // if (
      //howto->type == R_FLARE_G3_S32_PCREL
      //|| howto->type == R_FLARE_G3_S32_PCREL_NO_RELAX
    //)
    {
      const unsigned
        temp_length = flare_have_plp_insn_length
          (FLARE_HAVE_PLP_LPRE),
        insn_dist = flare_have_plp_distance
          (FLARE_HAVE_PLP_LPRE, FLARE_HAVE_PLP_NEITHER);
      prefix_insn = flare_get_insn_32 (input_bfd, contents + address);
      insn = bfd_get_16 (input_bfd, contents + address + insn_dist);
      //relocation += 
      //  flare_sign_extend (flare_get_g3_s32 (prefix_insn, insn),
      //    FLARE_G3_S9_BITSIZE + FLARE_G0_LPRE_S23_BITSIZE)
      //  - 6ull
      //  //- 4ull
      //  ;
      relocation -= temp_length;
      flare_put_g3_s32 (&prefix_insn, &insn, relocation);
      flare_put_insn_32 (input_bfd, prefix_insn, contents + address);
      bfd_put_16 (input_bfd, insn, contents + address + insn_dist);
    }
  }
  /* -------- */
  return bfd_reloc_ok;
  /* -------- */
}

/* Idea taken from RISC-V */
static bfd_reloc_status_type
flare_elf_add_sub_reloc (bfd *abfd,
                          arelent *reloc_entry,
                          asymbol *symbol,
                          void *data,
                          asection *input_section,
                          bfd *output_bfd,
                          char **error_message ATTRIBUTE_UNUSED)
{
  //reloc_howto_type *howto = reloc_entry->howto;
  //bfd_vma relocation;

  /* Blindly copied from `bfd_elf_generic_reloc ()`
    and `riscv_elf_add_sub_reloc ()`. */
  if (output_bfd != NULL
      && (symbol->flags & BSF_SECTION_SYM) == 0
      && (!reloc_entry->howto->partial_inplace
        || reloc_entry->addend == 0))
  {
    reloc_entry->address += input_section->output_offset;
    return bfd_reloc_ok;
  }

  /* Blindly copied from `riscv_elf_add_sub_reloc ()` */
  if (output_bfd != NULL)
  {
    //return bfd_reloc_continue;
    return bfd_elf_generic_reloc (abfd, reloc_entry, symbol, data,
      input_section, output_bfd, error_message);
    //return bfd_reloc_continue;
  }
  //relocation = symbol->value + symbol->section->output_section->vma
  //  + symbol->section->output_offset + reloc_entry->addend;
  return flare_elf_do_add_sub_reloc (abfd, reloc_entry->howto,
    input_section,
    data,
    reloc_entry->address,
    (
      symbol->value
      + symbol->section->output_section->vma
      + symbol->section->output_offset
    ),
    reloc_entry->addend);
}
static bfd_reloc_status_type
flare_elf_do_add_sub_reloc (bfd *input_bfd, reloc_howto_type *howto,
                              asection *input_section,
                              void *contents, bfd_vma address,
                              bfd_vma relocation, bfd_vma addend)
{
  //bfd_vma old_value = bfd_get (howto->bitsize, input_bfd,
  //    contents + rel->r_offset);
  //relocation = old_value + relocation;
  bfd_size_type octets = address
    * bfd_octets_per_byte (input_bfd, input_section);

  if (!bfd_reloc_offset_in_range (howto, input_bfd,
      input_section, octets))
  {
    return bfd_reloc_outofrange;
  }

  relocation += addend;

  switch (howto->type)
  {
    case R_FLARE_G1G5G6_S32_FOR_U5_ADD32:
    case R_FLARE_G1G5G6_S32_FOR_U5_SUB32:
    case R_FLARE_G1G5G6_S32_ADD32:
    case R_FLARE_G1G5G6_S32_SUB32:
    //case R_FLARE_G3_S32_PCREL_ADD32:
    //case R_FLARE_G3_S32_PCREL_SUB32:
    case R_FLARE_G7_ICRELOAD_S32_ADD32:
    case R_FLARE_G7_ICRELOAD_S32_SUB32:
    {
      const unsigned
        insn_dist = flare_have_plp_distance
          (FLARE_HAVE_PLP_LPRE, FLARE_HAVE_PLP_NEITHER);

      //bfd_vma old_relocation = relocation;
      flare_temp_t prefix_insn, insn, simm = 0;

      prefix_insn = flare_get_insn_32 (input_bfd, contents + address);
      //const unsigned
      //  prefix_insn_hi = bfd_get_16 (input_bfd, contents + address),
      //  prefix_insn_lo = bfd_get_16 (input_bfd, contents + address + 2);
      //prefix_insn = (
      //  (prefix_insn_hi << 16)
      //  | prefix_insn_lo
      //);
      insn = bfd_get_16 (input_bfd, contents + address + insn_dist);
      //printf(
      //  "do_add_sub_reloc: prefix_insn: %x_%x; %x %x; %x\n",
      //  (unsigned)(prefix_insn >> 32), (unsigned)prefix_insn,
      //  prefix_insn_hi, prefix_insn_lo,
      //  (unsigned)insn
      //);

      //relocation = (howto->type == R_FLARE_G1G5G6_S32_ADD32)
      //  ? (relocation + flare_get_g1g5g6_s32 (prefix_insn, insn))
      //  : (relocation - flare_get_g1g5g6_s32 (prefix_insn, insn));
      //printf ("do add sub reloc 0: 0x%x; ; 0x%x 0x%x; %s\n",
      //  (unsigned) old_relocation,
      //  (unsigned) prefix_insn, (unsigned) insn,
      //  howto->name);

      switch (howto->type)
      {
        case R_FLARE_G1G5G6_S32_FOR_U5_ADD32:
        case R_FLARE_G1G5G6_S32_ADD32:
          simm = flare_get_g1g5g6_s32 (prefix_insn, insn);
          relocation += simm;
          flare_put_g1g5g6_s32 (&prefix_insn, &insn, relocation);
          break;
        case R_FLARE_G1G5G6_S32_FOR_U5_SUB32:
        case R_FLARE_G1G5G6_S32_SUB32:
          simm = flare_get_g1g5g6_s32 (prefix_insn, insn);
          //relocation -= simm;
          relocation = simm - relocation;
          flare_put_g1g5g6_s32 (&prefix_insn, &insn, relocation);
          break;
        //case R_FLARE_G3_S32_PCREL_ADD32:
        //  simm = flare_get_g3_s32 (prefix_insn, insn);
        //  relocation = simm + relocation;
        //  relocation -= (input_section->output_section->vma
        //    + input_section->output_offset);
        //  //relocation -= 6ull;
        //  if (howto->pcrel_offset)
        //  {
        //    relocation -= address;
        //  }
        //  flare_put_g3_s32 (&prefix_insn, &insn, relocation);
        //  break;
        //case R_FLARE_G3_S32_PCREL_SUB32:
        //  simm = flare_get_g3_s32 (prefix_insn, insn);;
        //  //relocation -= simm;
        //  relocation = simm - relocation;
        //  relocation -= (input_section->output_section->vma
        //    + input_section->output_offset);
        //  relocation -= 6ull;
        //  if (howto->pcrel_offset)
        //  {
        //    relocation -= address;
        //  }
        //  flare_put_g3_s32 (&prefix_insn, &insn, relocation);
        //  break;
        case R_FLARE_G7_ICRELOAD_S32_ADD32:
          simm = flare_get_g7_icreload_s32 (prefix_insn, insn);
          relocation += simm;
          flare_put_g7_icreload_s32 (&prefix_insn, &insn, relocation);
          break;
        case R_FLARE_G7_ICRELOAD_S32_SUB32:
          simm = flare_get_g7_icreload_s32 (prefix_insn, insn);
          //relocation -= simm;
          relocation = simm - relocation;
          flare_put_g7_icreload_s32 (&prefix_insn, &insn, relocation);
          break;
      }
      //printf ("do add sub reloc 1: 0x%x; %i; 0x%x 0x%x; %s\n",
      //  (unsigned) relocation,
      //  (signed) simm,
      //  (unsigned) prefix_insn, (unsigned) insn,
      //  howto->name);

      flare_put_insn_32 (input_bfd, prefix_insn, contents + address);
      bfd_put_16 (input_bfd, insn, contents + address + insn_dist);
    }
      break;

    case R_FLARE_PSEUDO_ADD8:
    case R_FLARE_PSEUDO_ADD16:
    case R_FLARE_PSEUDO_ADD32:
    case R_FLARE_PSEUDO_ADD64:
      relocation = bfd_get (howto->bitsize, input_bfd, contents + address)
        + relocation;
      bfd_put (howto->bitsize, input_bfd, relocation, contents + address);
      break;

    case R_FLARE_PSEUDO_SUB8:
    case R_FLARE_PSEUDO_SUB16:
    case R_FLARE_PSEUDO_SUB32:
    case R_FLARE_PSEUDO_SUB64:
      relocation = bfd_get (howto->bitsize, input_bfd, contents + address)
        - relocation;
      bfd_put (howto->bitsize, input_bfd, relocation, contents + address);
      break;
    case R_FLARE_CFA_SUB6:
    {
      bfd_vma old_value = bfd_get (howto->bitsize, input_bfd,
        contents + address);
      relocation = (old_value & ~howto->dst_mask)
        | (((old_value & howto->dst_mask) - relocation)
          & howto->dst_mask);
      bfd_put (howto->bitsize, input_bfd, relocation, contents + address);
    }
      break;
  }

  //switch (howto->type)
  //{
  //  case R_FLARE_G1G5G6_S32_ADD32:
  //  case R_FLARE_G3_S32_PCREL_ADD32:
  //  case R_FLARE_PSEUDO_ADD8:
  //  case R_FLARE_PSEUDO_ADD16:
  //  case R_FLARE_PSEUDO_ADD32:
  //  case R_FLARE_PSEUDO_ADD64:
  //    return bfd_reloc_continue;
  //}

  return bfd_reloc_ok;
}

/* Implement elf_backend_relocate_section.  */
/* Adapted from elf32-pru.c */
static int
flare_elf_relocate_section (bfd *output_bfd,
                              struct bfd_link_info *info,
                              bfd *input_bfd,
                              asection *input_section,
                              bfd_byte *contents,
                              Elf_Internal_Rela *relocs,
                              Elf_Internal_Sym *local_syms,
                              asection **local_sections)
{
  Elf_Internal_Shdr *symtab_hdr;
  struct elf_link_hash_entry **sym_hashes;
  Elf_Internal_Rela *rel;
  Elf_Internal_Rela *relend;

  symtab_hdr = &elf_tdata (input_bfd)->symtab_hdr;
  sym_hashes = elf_sym_hashes (input_bfd);
  relend = relocs + input_section->reloc_count;
  //printf ("flare_elf_relocate_section: %d\n",
  //  input_section->reloc_count);

  for (rel=relocs; rel<relend; ++rel)
  {
    reloc_howto_type *howto;
    unsigned long r_symndx;
    Elf_Internal_Sym *sym;
    asection *sec;
    struct elf_link_hash_entry *h;
    bfd_vma relocation;
    bfd_reloc_status_type r = bfd_reloc_ok;
    const char *name = NULL;
    const char* msg = (const char*) NULL;
    bool unresolved_reloc;
    //bfd_vma addend;

    r_symndx = ELF32_R_SYM (rel->r_info);

    // TODO
    howto = flare_lookup_howto ((unsigned) ELF32_R_TYPE (rel->r_info));
    //printf ("flare_elf_relocate_section howto: %s\n",
    //  howto->name);
    h = NULL;
    sym = NULL;
    sec = NULL;

    if (r_symndx < symtab_hdr->sh_info)
    {
      sym = local_syms + r_symndx;
      sec = local_sections[r_symndx];
      relocation = _bfd_elf_rela_local_sym (output_bfd, sym, &sec, rel);
    }
    else
    {
      bool warned, ignored;

      RELOC_FOR_GLOBAL_SYMBOL (info, input_bfd, input_section, rel,
              r_symndx, symtab_hdr, sym_hashes,
              h, sec, relocation,
              unresolved_reloc, warned, ignored);
    }

    if (sec && discarded_section (sec))
    {
      RELOC_AGAINST_DISCARDED_SECTION (info, input_bfd, input_section,
                rel, 1, relend, howto, 0, contents);
    }

    /* Nothing more to do unless this is a final link.  */
    /* FL4SHK note: may need to remove this. */
    if (bfd_link_relocatable (info))
    {
      continue;
    }

    if (howto)
    {
      switch (howto->type)
      {
        case R_FLARE_NONE:
          /* We don't need to find a value for this symbol. It's just a
          marker.  */
          r = bfd_reloc_ok;
          break;

        case R_FLARE_8:
        case R_FLARE_16:
        case R_FLARE_32:
        case R_FLARE_64:
        case R_FLARE_SET8:
        case R_FLARE_SET16:
        case R_FLARE_SET32:
        //case R_FLARE_SET64:
        case R_FLARE_CFA_SET6:
        case R_FLARE_EH_32_PCREL:
        //case R_FLARE_FDE_32_PCREL:
          r = _bfd_final_link_relocate 
            (howto, input_bfd,
            input_section, contents,
            rel->r_offset, relocation,
            rel->r_addend);
          break;

        case R_FLARE_G1G5G6_U5:
        case R_FLARE_G1G5G6_S17_FOR_U5:
        case R_FLARE_G1G5G6_S32_FOR_U5:
        case R_FLARE_G1G5G6_S32_FOR_U5_NO_RELAX:
        case R_FLARE_G1G5G6_S5:
        case R_FLARE_G1G5G6_S17:
        case R_FLARE_G1G5G6_S32:
        case R_FLARE_G1G5G6_S32_NO_RELAX:
        case R_FLARE_G3_S9_PCREL:
        case R_FLARE_G3_S21_PCREL:
        case R_FLARE_G3_S32_PCREL:
        case R_FLARE_G3_S32_PCREL_NO_RELAX:
        case R_FLARE_G7_ICRELOAD_S5:
        case R_FLARE_G7_ICRELOAD_S17:
        case R_FLARE_G7_ICRELOAD_S32:
        case R_FLARE_G7_ICRELOAD_S32_NO_RELAX:
          r = flare_elf_do_non_sub_imm_reloc (input_bfd, howto,
                input_section, contents,
                rel->r_offset, relocation,
                rel->r_addend);
          break;

        case R_FLARE_G1G5G6_S32_FOR_U5_ADD32:
        case R_FLARE_G1G5G6_S32_FOR_U5_SUB32:
        case R_FLARE_G1G5G6_S32_ADD32:
        case R_FLARE_G1G5G6_S32_SUB32:
        //case R_FLARE_G3_S32_PCREL_ADD32:
        //case R_FLARE_G3_S32_PCREL_SUB32:
        case R_FLARE_G7_ICRELOAD_S32_ADD32:
        case R_FLARE_G7_ICRELOAD_S32_SUB32:
        case R_FLARE_PSEUDO_ADD8:
        case R_FLARE_PSEUDO_ADD16:
        case R_FLARE_PSEUDO_ADD32:
        case R_FLARE_PSEUDO_ADD64:
        case R_FLARE_PSEUDO_SUB8:
        case R_FLARE_PSEUDO_SUB16:
        case R_FLARE_PSEUDO_SUB32:
        case R_FLARE_PSEUDO_SUB64:
        case R_FLARE_CFA_SUB6:
          r = flare_elf_do_add_sub_reloc (input_bfd, howto,
                input_section, contents,
                rel->r_offset, relocation,
                rel->r_addend);
          break;

        //case R_FLARE_G1G5G6_S32_ADD32:
        //case R_FLARE_G3_S32_PCREL_ADD32:
        //  r = bfd_reloc_ok;
        //  break;

        default:
          printf ("relocate_section default howto name: %s\n",
            howto->name);
          BFD_ASSERT (0);
          break;
      }
    }
    else
    {
      r = bfd_reloc_notsupported;
    }

    if (r != bfd_reloc_ok)
    {
      if (h != NULL)
      {
        name = h->root.root.string;
      }
      else
      {
        name = bfd_elf_string_from_elf_section (input_bfd,
                  symtab_hdr->sh_link,
                  sym->st_name);
        if (name == NULL || *name == '\0')
        {
          name = bfd_section_name (sec);
        }
      }

      switch (r)
      {
        case bfd_reloc_overflow:
          (*info->callbacks->reloc_overflow) (info, NULL, name,
                howto->name, (bfd_vma) 0,
                input_bfd, input_section,
                rel->r_offset);
          break;
        case bfd_reloc_undefined:
          (*info->callbacks->undefined_symbol) (info, name, input_bfd,
                  input_section,
                  rel->r_offset, true);
          break;
        case bfd_reloc_outofrange:
          if (msg == NULL)
          {
            msg = _("relocation out of range");
          }
          break;
        case bfd_reloc_notsupported:
          if (msg == NULL)
          {
            msg = _("unsupported relocation");
          }
          break;
        case bfd_reloc_dangerous:
          if (msg == NULL)
          {
            msg = _("dangerous relocation");
          }
          break;
        default:
          if (msg == NULL)
          {
            msg = _("unknown error");
          }
          break;
      }

      if (msg)
      {
        (*info->callbacks->warning) (info, msg, name, input_bfd,
              input_section, rel->r_offset);
        return false;
      }
    }
  }
  return true;
}
/* -------- */
static bool
relax_can_shrink_value (bfd_vma value,
  flare_temp_t curr_bitsize, flare_temp_t target_bitsize,
  bool have_small_imm_unsigned_target)
{
  ////bool ret = (
  ////  (bfd_signed_vma) value
  ////    == ((
  ////      (bfd_signed_vma) value
  ////      << (curr_bitsize - target_bitsize)
  ////    ) >> (curr_bitsize - target_bitsize))
  ////);


  ////flare_temp_t shift_amount = curr_bitsize - target_bitsize;
  ////bfd_vma
  ////  target_value = value 
  ////    & (((bfd_vma) 1ull << shift_amount) - (bfd_vma) 1ull),
  ////  target_value_se
  ////    = flare_sign_extend (target_value, target_bitsize);
  ////printf ("flare relax can shrink: 0x%x; 0x%x 0x%x; %d %d\n",
  ////  (unsigned) value,
  ////  (unsigned) target_value, (unsigned) target_value_se,
  ////  (unsigned) curr_bitsize, (unsigned) target_bitsize);
  //flare_temp_t
  //  curr_value_se = flare_sign_extend (value, curr_bitsize),
  //  target_value_se = flare_sign_extend (value, target_bitsize);

  ////printf ("flare_relax_can_shrink_value: 0x%x; 0x%x 0x%x; %d\n",
  ////  (unsigned) value,
  ////  (unsigned) curr_value_se, (unsigned) target_value_se,
  ////  (uint32_t) curr_value_se == (uint32_t) target_value_se);

  //return (
  //  (uint32_t) curr_value_se
  //  == (uint32_t) target_value_se
  //);
  flare_temp_t
    curr_value_ext = flare_sign_extend (value, curr_bitsize),
    target_value_ext
      = !have_small_imm_unsigned_target
      ? flare_sign_extend (value, target_bitsize)
      : flare_zero_extend (value, target_bitsize);

  return (
    (uint32_t) curr_value_ext
    == (uint32_t) target_value_ext
  );
}
typedef struct flare_relax_temp_t
{
  bfd *abfd;
  asection *sec;
  Elf_Internal_Shdr *symtab_hdr;
  Elf_Internal_Sym *isymbuf;
  Elf_Internal_Rela *internal_relocs;
  Elf_Internal_Rela *irel;
  bfd_byte *contents;
  bfd_vma value;
  reloc_howto_type *howto;
  bool is_pcrel: 1;
  bool rm_prefix: 1;
  bool was_lpre: 1;
  bool is_small_imm_unsigned: 1;
  bool is_g7_icreload: 1;
  flare_temp_t curr_bitsize;
  flare_temp_t target_bitsize;
} flare_relax_temp_t;



/* Delete some bytes from a section while changing the size of an
  instruction.
  The parameter "addr" denotes the section-relative offset pointing just
  behind the shrinked instruction. "addr+count" point at the first
  byte just behind the original unshrinked instruction.

  Idea copied from the PRU port.  */

static bool
flare_elf_relax_delete_bytes (bfd *abfd,
                                asection *sec,
                                bfd_vma addr,
                                int count)
{
  Elf_Internal_Shdr *symtab_hdr;
  unsigned int sec_shndx;
  bfd_byte *contents;
  Elf_Internal_Rela *irel, *irelend;
  Elf_Internal_Sym *isym;
  Elf_Internal_Sym *isymbuf = NULL;
  bfd_vma toaddr;
  struct elf_link_hash_entry **sym_hashes;
  struct elf_link_hash_entry **end_hashes;
  unsigned int symcount;

  symtab_hdr = &elf_tdata (abfd)->symtab_hdr;
  sec_shndx = _bfd_elf_section_from_bfd_section (abfd, sec);
  contents = elf_section_data (sec)->this_hdr.contents;

  toaddr = sec->size;

  irel = elf_section_data (sec)->relocs;
  irelend = irel + sec->reloc_count;

  /* Actually delete the bytes.  */
  if (toaddr - addr - count > 0)
  {
    memmove (contents + addr, contents + addr + count,
             (size_t) (toaddr - addr - count));
  }
  sec->size -= count;

  /* Adjust all the reloc addresses.  */
  for (irel=elf_section_data (sec)->relocs; irel<irelend; ++irel)
  {
    //bfd_vma old_reloc_address;

    //old_reloc_address = (sec->output_section->vma
    //                      + sec->output_offset + irel->r_offset);

    /* Get the new reloc address.  */
    if ((irel->r_offset > addr && irel->r_offset < toaddr))
    {
      irel->r_offset -= count;
    }
  }

   /* The reloc's own addresses are now ok.  However, we need to readjust
      the reloc's addend, i.e. the reloc's value if two conditions are met:
      1.) the reloc is relative to a symbol in this section that
          is located in front of the shrinked instruction
      2.) symbol plus addend end up behind the shrinked instruction.

      The most common case where this happens are relocs relative to
      the section-start symbol.

      This step needs to be done for all of the sections of the bfd.  */

  {
    struct bfd_section *isec;

    for (isec=abfd->sections; isec; isec=isec->next)
    {
      bfd_vma symval;
      bfd_vma shrunk_insn_address;

      if (isec->reloc_count == 0)
      {
        continue;
      }

      shrunk_insn_address = (sec->output_section->vma
                              + sec->output_offset + addr);

      irel = elf_section_data (isec)->relocs;
      /* Read in the relocs for this section if necessary.  */
      if (irel == NULL)
      {
        irel = _bfd_elf_link_read_relocs (abfd, isec, NULL, NULL, true);
      }

      for (irelend=irel + isec->reloc_count; irel<irelend; ++irel)
      {
        /* Read this BFD's local symbols if we haven't done
          so already.  */
        if (isymbuf == NULL && symtab_hdr->sh_info != 0)
        {
          isymbuf = (Elf_Internal_Sym *) symtab_hdr->contents;
          if (isymbuf == NULL)
          {
            isymbuf = bfd_elf_get_elf_syms (abfd, symtab_hdr,
                                            symtab_hdr->sh_info, 0,
                                            NULL, NULL, NULL);
          }
          if (isymbuf == NULL)
          {
            return false;
          }
        }

        /* Get the value of the symbol referred to by the reloc.  */
        if (ELF32_R_SYM (irel->r_info) < symtab_hdr->sh_info)
        {
          /* A local symbol.  */
          asection *sym_sec;

          isym = isymbuf + ELF32_R_SYM (irel->r_info);
          sym_sec = bfd_section_from_elf_index (abfd, isym->st_shndx);
          symval = isym->st_value;
          /* If the reloc is absolute, it will not have
            a symbol or section associated with it.  */
          if (sym_sec == sec)
          {
            symval += sym_sec->output_section->vma
              + sym_sec->output_offset;

            /* Fix the addend, if it is affected.  */
            if (symval <= shrunk_insn_address
                && (symval + irel->r_addend) > shrunk_insn_address)
            {
              irel->r_addend -= count;
            }
          }
          /* else...Reference symbol is absolute.
            No adjustment needed.  */
        }
        /* else...Reference symbol is extern.  No need for adjusting
          the addend.  */
      }
    }
  }

  /* Adjust the local symbols defined in this section.  */
  isym = (Elf_Internal_Sym *) symtab_hdr->contents;
  /* Fix PR 9841, there may be no local symbols.  */
  if (isym != NULL)
  {
    Elf_Internal_Sym *isymend;

    isymend = isym + symtab_hdr->sh_info;
    for (; isym<isymend; ++isym)
    {
      if (isym->st_shndx == sec_shndx)
      {
        if (isym->st_value > addr
            && isym->st_value <= toaddr)
        {
          isym->st_value -= count;
        }

        if (isym->st_value <= addr
            && isym->st_value + isym->st_size > addr)
        {
          /* If this assert fires then we have a symbol that ends
              part way through an instruction.  Does that make
              sense?  */
          BFD_ASSERT (isym->st_value + isym->st_size
                      >= addr + count);
          isym->st_size -= count;
        }
      }
    }
  }

  /* Now adjust the global symbols defined in this section.  */
  symcount = (symtab_hdr->sh_size / sizeof (Elf32_External_Sym)
              - symtab_hdr->sh_info);
  sym_hashes = elf_sym_hashes (abfd);
  end_hashes = sym_hashes + symcount;
  for (; sym_hashes<end_hashes; ++sym_hashes)
  {
    struct elf_link_hash_entry *sym_hash = *sym_hashes;
    if ((sym_hash->root.type == bfd_link_hash_defined
          || sym_hash->root.type == bfd_link_hash_defweak)
        && sym_hash->root.u.def.section == sec)
    {
      if (sym_hash->root.u.def.value > addr
          && sym_hash->root.u.def.value <= toaddr)
      {
        sym_hash->root.u.def.value -= count;
      }

      if (sym_hash->root.u.def.value <= addr
          && (sym_hash->root.u.def.value + sym_hash->size > addr))
      {
        /* If this assert fires then we have a symbol that ends
            part way through an instruction.  Does that make
            sense?  */
        BFD_ASSERT (sym_hash->root.u.def.value + sym_hash->size
                    >= addr + count);
        sym_hash->size -= count;
      }
    }
  }

  return true;
}

static bool
flare_do_relax_prefix_innards (flare_relax_temp_t *args)
{
  //printf ("flare_do_relax_prefix_innards\n");
  /* Note that we'vve changed the relocs, section contents. */
  elf_section_data (args->sec)->relocs = args->internal_relocs;
  elf_section_data (args->sec)-> this_hdr.contents = args->contents;
  args->symtab_hdr->contents = (unsigned char *) args->isymbuf;
  //fprintf (
  //  stderr,
  //  "flare_do_relax_prefix_innards(): %s",
  //  args->howto->name
  //);

  if (args->was_lpre)
  {
    if (args->rm_prefix) // remove the `lpre` instruction
    {
      const unsigned
        insn_dist = flare_have_plp_distance
          (FLARE_HAVE_PLP_LPRE, FLARE_HAVE_PLP_NEITHER);
      //printf ("flare_do_relax_prefix_innards: "
      //  "was_lpre && rm_prefix: %s\n",
      //  args->howto->name);

      //if (args->is_pcrel)
      {
        flare_temp_t
          insn = bfd_get_16 (args->abfd,
            args->contents + args->irel->r_offset
            //+ 4
            + insn_dist
            ),
          //simm = flare_get_insn_field_ei
          //  (&flare_enc_info_g3_s9, insn);
          simm = args->value;
            
          if (!args->is_pcrel)
          {
            (void) flare_set_insn_field_ei_p
              (
                (!args->is_g7_icreload
                  ? &flare_enc_info_g1g5g6_i5
                  : &flare_enc_info_g7_icreload_s5),
              &insn, simm);
          }
          else // if (args->is_pcrel)
          {
            (void) flare_set_insn_field_ei_p
              (&flare_enc_info_g3_s9, &insn,
              simm
                //- 4ull
                - insn_dist
              );
          }

        bfd_put_16 (args->abfd, insn,
          args->contents + args->irel->r_offset
          //+ 4
          + insn_dist
          );
      }

      if (!flare_elf_relax_delete_bytes (args->abfd, args->sec,
        args->irel->r_offset,
        //4
        insn_dist
        ))
      {
        return false;
      }
      args->irel->r_info
        = ELF32_R_INFO (ELF32_R_SYM (args->irel->r_info),
          //!args->is_pcrel
          //? (
          // !args->is_g7_icreload 
          //  ? (
          //    !args->is_small_imm_unsigned
          //    ? R_FLARE_G1G5G6_S5
          //    : R_FLARE_G1G5G6_U5
          //  ) : R_FLARE_G7_ICRELOAD_S5
          //) : R_FLARE_G3_S9_PCREL;
          flare_relax_reloc_lookup
            (args->is_pcrel, args->is_g7_icreload,
            args->is_small_imm_unsigned)->small.r_type);
    }
    else // if (!args->rm_prefix) // convert the `lpre` to a `pre`
    {
      const unsigned
        insn_dist = flare_have_plp_distance
          (FLARE_HAVE_PLP_LPRE, FLARE_HAVE_PLP_NEITHER),
        prefix_insn_dist = flare_have_plp_distance
          (FLARE_HAVE_PLP_LPRE, FLARE_HAVE_PLP_PRE);
      //printf ("flare_do_relax_prefix_innards: "
      //  "was_lpre && !rm_prefix: %s\n",
      //  args->howto->name);
      if (!args->is_pcrel)
      {
        //const unsigned
        //  prefix_insn_dist = flare_have_plp_distance
        //    (FLARE_HAVE_PLP_LPRE, FLARE_HAVE_PLP_PRE);
        bfd_put_16 (args->abfd,
          flare_enc_temp_insn_pre (bfd_get_16 (args->abfd,
            args->contents + args->irel->r_offset
              //+ 2
              + prefix_insn_dist
              )),
          args->contents + args->irel->r_offset
            //+ 2
            + prefix_insn_dist
            );
      }
      else // if (args->is_pcrel)
      {
        flare_temp_t
          //curr_prefix_insn = bfd_get_32 (args->abfd,
          //  args->contents + args->irel->r_offset),
          insn = bfd_get_16 (args->abfd,
            args->contents + args->irel->r_offset
            //+ 4
            + insn_dist
            ),
          //simm = flare_get_g3_s32 (curr_prefix_insn, insn),
          simm = args->value,
          prefix_insn = flare_enc_temp_insn_pre (0x0);

        flare_put_g3_s21 (&prefix_insn, &insn,
          simm
          //- 2ull
          - prefix_insn_dist
        );
        //simm = flare_get_g3_s21 (prefix_insn, insn)
        //  //- 2ull
        //  ;
        //flare_put_g3_s21 (&prefix_insn, &insn, simm);

        bfd_put_16 (args->abfd, prefix_insn,
          args->contents + args->irel->r_offset
          //+ 2
          + prefix_insn_dist
          );
        bfd_put_16 (args->abfd, insn,
          args->contents + args->irel->r_offset
          //+ 4
          + insn_dist
          );
      }

      if (!flare_elf_relax_delete_bytes (args->abfd, args->sec,
        args->irel->r_offset,
        //2U
        prefix_insn_dist
        ))
      {
        return false;
      }

      args->irel->r_info
        = ELF32_R_INFO (ELF32_R_SYM (args->irel->r_info),
          //!args->is_pcrel
          //? (
          //  !args->is_g7_icreload
          //  ? (
          //    !args->is_small_imm_unsigned
          //    ? R_FLARE_G1G5G6_S17
          //    : R_FLARE_G1G5G6_S17_FOR_U5
          //  ) : R_FLARE_G7_ICRELOAD_S17
          //) : R_FLARE_G3_S21_PCREL);
          flare_relax_reloc_lookup
            (args->is_pcrel, args->is_g7_icreload,
            args->is_small_imm_unsigned)->pre.r_type);
    }
  }
  else // if (!args->was_lpre) // remove the `pre` instruction
  {
    const unsigned
      insn_dist = flare_have_plp_distance
        (FLARE_HAVE_PLP_PRE, FLARE_HAVE_PLP_NEITHER);
    //printf ("flare_do_relax_prefix_innards: "
    //  "!was_lpre: %s\n",
    //  args->howto->name);
    //if (args->is_pcrel)
    {
      flare_temp_t
        insn = bfd_get_16 (args->abfd,
          args->contents + args->irel->r_offset
          //+ 2
          + insn_dist
          ),
        //simm = flare_get_insn_field_ei
        //  (&flare_enc_info_g3_s9, insn);
        simm = args->value;
          
        
        if (!args->is_pcrel)
        {
          (void) flare_set_insn_field_ei_p
            (
              (!args->is_g7_icreload
                ? &flare_enc_info_g1g5g6_i5
                : &flare_enc_info_g7_icreload_s5),
            &insn, simm);
        }
        else // if (args->is_pcrel)
        {
          (void) flare_set_insn_field_ei_p
            (&flare_enc_info_g3_s9, &insn,
            simm
              //- 2ull
              - insn_dist
            );
        }

      bfd_put_16 (args->abfd, insn,
        args->contents + args->irel->r_offset
          //+ 2
          + insn_dist
        );
    }
    if (!flare_elf_relax_delete_bytes (args->abfd, args->sec,
      args->irel->r_offset,
      //2
      insn_dist
    ))
    {
      return false;
    }
    args->irel->r_info
      = ELF32_R_INFO (ELF32_R_SYM (args->irel->r_info),
        //!args->is_pcrel
        //? (
        //  !args->is_g7_icreload
        //  ? (
        //    !args->is_small_imm_unsigned
        //    ? R_FLARE_G1G5G6_S5
        //    : R_FLARE_G1G5G6_U5
        //  ) : R_FLARE_G7_ICRELOAD_S5
        //) : R_FLARE_G3_S9_PCREL);
        flare_relax_reloc_lookup
          (args->is_pcrel, args->is_g7_icreload,
          args->is_small_imm_unsigned)->small.r_type);
  }

  return true;
}

static bool
flare_do_relax_prefix (bfd *abfd,
                        asection *sec,
                        bool *again,
                        Elf_Internal_Shdr *symtab_hdr,
                        Elf_Internal_Sym *isymbuf,
                        Elf_Internal_Rela *internal_relocs,
                        Elf_Internal_Rela *irel,
                        bfd_byte *contents,
                        bfd_vma symval)
{
  reloc_howto_type *howto;
  howto = flare_lookup_howto (ELF32_R_TYPE (irel->r_info));
  //fprintf (
  //  stderr,
  //  "flare_do_relax_prefix(): begin\n"
  //);

  /* For simplicity of coding, we are going to modify the section
      contents, the section relocs, and the BFD symbol table.  We
      must tell the rest of the code not to free up this
      information.  It would be possible to instead create a table
      of changes which have to be made, as is done in coff-mips.c;
      that would be more work, but would require less memory when
      the linker is run.  */
  /* Check if we can convert an `lpre` instruction to a `pre`
    instruction, or otherwise to there being neither `pre` nor `lpre` */
  //if (howto->type == R_FLARE_G3_S9_PCREL)
  //{
  //}
  //else
  if (howto->type == R_FLARE_G1G5G6_S17_FOR_U5
    || howto->type == R_FLARE_G1G5G6_S32_FOR_U5
    || howto->type == R_FLARE_G1G5G6_S17
    || howto->type == R_FLARE_G1G5G6_S32
    //|| howto->type == R_FLARE_G1G5G6_S32_SUB32
    || howto->type == R_FLARE_G3_S21_PCREL
    || howto->type == R_FLARE_G3_S32_PCREL
    //|| howto->type == R_FLARE_G3_S32_PCREL_SUB32
    || howto->type == R_FLARE_G7_ICRELOAD_S17
    || howto->type == R_FLARE_G7_ICRELOAD_S32
  )
  {
    //printf ("flare_do_relax_prefix howto found: 0x%x 0x%x; %s\n",
    //  (unsigned) symval, (unsigned) irel->r_addend,
    //  howto->name);
    bfd_vma
      value = symval + irel->r_addend,
      //value_pcrel_offset = 0;
      dot = 0, gap;

    flare_temp_t
      //prefix_insn,
      //insn,
      prefix_insn_bitsize = 0, //prefix_insn_bitpos, prefix_insn_mask,
      insn_bitsize = 0, //insn_bitpos, insn_mask, 
      curr_bitsize, target_bitsize = 0;
      //target_value = 0;
    bool
      was_lpre = false,
      is_small_imm_unsigned = false,
      is_g7_icreload = false;
    const unsigned
      shrink_one_unit_dist = flare_have_plp_distance
        (FLARE_HAVE_PLP_LPRE, FLARE_HAVE_PLP_PRE),
      shrink_two_units_dist = flare_have_plp_distance
        (FLARE_HAVE_PLP_LPRE, FLARE_HAVE_PLP_NEITHER);

    if (howto->pc_relative)
    {
      /* Get the address of this instruction. */
      dot = sec->output_section->vma
        + sec->output_offset + irel->r_offset;
    }

    /* Compute the distance from this insn to the branch target, if there 
      is one. */
    gap = value - dot
      //+ 4;
      ;

    if (howto->type == R_FLARE_G1G5G6_S17_FOR_U5
      || howto->type == R_FLARE_G1G5G6_S17
      || howto->type == R_FLARE_G7_ICRELOAD_S17)
    {
      is_small_imm_unsigned = (howto->type == R_FLARE_G1G5G6_S17_FOR_U5);
      is_g7_icreload = (howto->type == R_FLARE_G7_ICRELOAD_S17);
      prefix_insn_bitsize = FLARE_G0_PRE_S12_BITSIZE;
      insn_bitsize = FLARE_G1G5G6_I5_BITSIZE;
      
      target_bitsize = insn_bitsize;
    }
    else if (
      howto->type == R_FLARE_G1G5G6_S32_FOR_U5
      || howto->type == R_FLARE_G1G5G6_S32
      //|| howto->type == R_FLARE_G1G5G6_S32_SUB32
      || howto->type == R_FLARE_G7_ICRELOAD_S32
    )
    {
      is_small_imm_unsigned = (howto->type == R_FLARE_G1G5G6_S32_FOR_U5);
      is_g7_icreload = (howto->type == R_FLARE_G7_ICRELOAD_S32);
      was_lpre = true;

      prefix_insn_bitsize = FLARE_G0_LPRE_S27_BITSIZE;
      insn_bitsize = FLARE_G1G5G6_I5_BITSIZE;

      target_bitsize = FLARE_G0_PRE_S12_BITSIZE + insn_bitsize;
    }
    else if (howto->type == R_FLARE_G3_S21_PCREL)
    {
      prefix_insn_bitsize = FLARE_G0_PRE_S12_BITSIZE;
      insn_bitsize = FLARE_G3_S9_BITSIZE;

      target_bitsize = insn_bitsize;
    }
    else if (howto->type == R_FLARE_G3_S32_PCREL
      //|| howto->type == R_FLARE_G3_S32_PCREL_SUB32
    )
    {
      was_lpre = true;

      prefix_insn_bitsize = FLARE_G0_LPRE_S23_BITSIZE;
      insn_bitsize = FLARE_G3_S9_BITSIZE;

      target_bitsize = FLARE_G0_PRE_S12_BITSIZE + insn_bitsize;
    }
    else
    {
      BFD_ASSERT (0);
    }
    curr_bitsize = prefix_insn_bitsize + insn_bitsize;

    //printf ("flare partial relax: have howto: "
    //  "0x%x 0x%x; %d; %d %d; %d %d\n",
    //  (unsigned) value, (unsigned) gap,
    //  (unsigned) was_lpre,
    //  (unsigned) prefix_insn_bitsize, (unsigned) insn_bitsize,
    //  (unsigned) target_bitsize, (unsigned) curr_bitsize);

    if (relax_can_shrink_value
      (!howto->pc_relative
        ? value
        : gap - shrink_one_unit_dist,
        //: gap,
      curr_bitsize, target_bitsize,
      !was_lpre && (
        //howto->type >= R_FLARE_G1G5G6_U5
        //&& howto->type <= R_FLARE_G1G5G6_S32_FOR_U5_NO_RELAX
        is_small_imm_unsigned
      ))
    )
    {
      //printf ("flare partial relax: can shrink 0\n");

      flare_relax_temp_t args =
      {
        abfd, /* abfd */
        sec, /* sec */
        symtab_hdr, /* symtab_hdr */
        isymbuf, /* isymbuf */
        internal_relocs, /* internal_relocs */
        irel, /* irel */
        contents, /* contents */
        !howto->pc_relative ? value : gap, /* value */
        //dot, /* dot */
        //gap, /* gap */
        howto,
        howto->pc_relative, /* is_pcrel */
        true, /* rm_prefix */
        was_lpre, /* was_lpre */
        is_small_imm_unsigned, /* is_small_imm_unsigned */
        is_g7_icreload, /* is_g7_icreload */
        curr_bitsize, /* curr_bitsize */
        target_bitsize, /* target_bitsize */
        //prefix_insn_bitsize,
        //prefix_insn_bitpos,
        //prefix_insn_mask,
        //insn_bitsize,
        //insn_bitpos,
        //insn_mask,
      };

      args.rm_prefix = (
        !was_lpre 
        || relax_can_shrink_value 
          (!howto->pc_relative
            ? value
            : gap - shrink_two_units_dist, 
            //: gap, 
            prefix_insn_bitsize, insn_bitsize,
            //(
            //  howto->type >= R_FLARE_G1G5G6_U5
            //  && howto->type <= R_FLARE_G1G5G6_S32_FOR_U5_NO_RELAX
            //)
            is_small_imm_unsigned
            )
      );

      if (!flare_do_relax_prefix_innards (&args))
      {
        //goto error_return;
        return false;
      }
      *again = true;
    }
  }
  return true;
}


/* Adapted from elf32-pru.c */
static bool
_flare_elf_relax_section (bfd *abfd,
                          asection *sec,
                          struct bfd_link_info *link_info,
                          bool *again)
{
  //printf (
  //  //stderr,
  //  "flare dbg: flare_elf_relax_section\n"
  //);
  /* -------- */
  Elf_Internal_Shdr *symtab_hdr;
  Elf_Internal_Rela *internal_relocs;
  Elf_Internal_Rela *irel;
  Elf_Internal_Rela *irelend;
  bfd_byte *contents = NULL;
  Elf_Internal_Sym *isymbuf = NULL;
  /* -------- */
  /* Assume nothing changes.  */
  *again = false;
  /* -------- */
  /* We don't have to do anything for a relocatable link, if
     this section does not have relocs. */
  if (bfd_link_relocatable (link_info)
      || sec->reloc_count == 0
      || (sec->flags & SEC_RELOC) == 0
      || (sec->flags & SEC_HAS_CONTENTS) == 0
      || (sec->flags & SEC_CODE) == 0
    )
  {
    return true;
  }
  /* -------- */
  symtab_hdr = & elf_tdata (abfd)->symtab_hdr;
  /* -------- */
  /* Get a copy of the native relocations.  */
  internal_relocs = _bfd_elf_link_read_relocs (abfd, sec, NULL, NULL,
                                               link_info->keep_memory);
  if (internal_relocs == NULL)
  {
    goto error_return;
  }
  /* -------- */
  /* Walk through them looking for relaxing opportunities.  */
  irelend = internal_relocs + sec->reloc_count;
  /* -------- */
  for (irel=internal_relocs; irel<irelend; ++irel)
  {
    bfd_vma symval;

    /* Get the section contents if we haven't done so already.  */
    if (contents == NULL)
    {
      /* Get cached copy if it exists.  */
      if (elf_section_data (sec)->this_hdr.contents != NULL)
      {
        contents = elf_section_data (sec)->this_hdr.contents;
      }
      else if (!bfd_malloc_and_get_section (abfd, sec, &contents))
      {
        /* Go get them off disck. */
        goto error_return;
      }
    }

    /* Read this BFD's local symbols if we haven't done so already.  */
    if (isymbuf == NULL && symtab_hdr->sh_info != 0)
    {
      isymbuf = (Elf_Internal_Sym *) symtab_hdr->contents;
      if (isymbuf == NULL)
      {
        isymbuf = bfd_elf_get_elf_syms (abfd, symtab_hdr,
                                        symtab_hdr->sh_info, 0,
                                        NULL, NULL, NULL);
      }
      if (isymbuf == NULL)
      {
        goto error_return;
      }
    }

    /* Get the value of the symbol referred to by the reloc.  */
    if (ELF32_R_SYM (irel->r_info) < symtab_hdr->sh_info)
    {
      /* A local symbol.  */
      Elf_Internal_Sym *isym;
      asection *sym_sec;

      isym = isymbuf + ELF32_R_SYM (irel->r_info);
      if (isym->st_shndx == SHN_UNDEF)
      {
        sym_sec = bfd_und_section_ptr;
      }
      else if (isym->st_shndx == SHN_ABS)
      {
        sym_sec = bfd_abs_section_ptr;
      }

      /* FL4SHK note: we don't cover the ELF "Common" section in
        `flare_elf_relocate_section()`, so I'm not sure this is
        necessary. */
      else if (isym->st_shndx == SHN_COMMON)
      {
        sym_sec = bfd_com_section_ptr;
      }
      else
      {
        sym_sec = bfd_section_from_elf_index (abfd, isym->st_shndx);
      }
      symval = (isym->st_value
                + sym_sec->output_section->vma + sym_sec->output_offset);
    }
    else
    {
      unsigned long indx;
      struct elf_link_hash_entry *h;

      /* An external symbol.  */
      indx = ELF32_R_SYM (irel->r_info) - symtab_hdr->sh_info;
      h = elf_sym_hashes (abfd)[indx];
      BFD_ASSERT (h != NULL);

      if (h->root.type != bfd_link_hash_defined
          && h->root.type != bfd_link_hash_defweak)
      {
        /* This appears to be a reference to an undefined
          symbol.  Just ignore it--it will be caught by the
          regular reloc processing.  */
        continue;
      }

      symval = (h->root.u.def.value
                + h->root.u.def.section->output_section->vma
                + h->root.u.def.section->output_offset);
    }

    if (!flare_do_relax_prefix (abfd, sec, again, symtab_hdr,
      isymbuf, internal_relocs, irel, contents, symval))
    {
      goto error_return;
    }
  }

  if (isymbuf != NULL && symtab_hdr->contents != (unsigned char *) isymbuf)
  {
    if (!link_info->keep_memory)
    {
      free (isymbuf);
    }
    else
    {
      /* Cache the symbols for elf_link_input_bfd.  */
      symtab_hdr->contents = (unsigned char *) isymbuf;
    }
  }

  if (contents != NULL
      && elf_section_data (sec)->this_hdr.contents != contents)
  {
    if (!link_info->keep_memory)
    {
      free (contents);
    }
    else
    {
      /* Cache the section contents for elf_link_input_bfd.  */
      elf_section_data (sec)->this_hdr.contents = contents;
    }
  }

  if (elf_section_data (sec)->relocs != internal_relocs)
  {
    free (internal_relocs);
  }

  return true;

 error_return:
  if (symtab_hdr->contents != (unsigned char *) isymbuf)
  {
    free (isymbuf);
  }
  if (elf_section_data (sec)->this_hdr.contents != contents)
  {
    free (contents);
  }
  if (elf_section_data (sec)->relocs != internal_relocs)
  {
    free (internal_relocs);
  }

  return false;
}
/* -------- */
#define ELF_ARCH            bfd_arch_flare
#define ELF_MACHINE_CODE    EM_FLARE_UNOFFICIAL
#define ELF_MAXPAGESIZE     0x1000
//#define ELF_COMMONPAGESIZE  0x1000

#define TARGET_LITTLE_SYM    flare_elf32_vec
#define TARGET_LITTLE_NAME   "elf32-flare"

#define elf_info_to_howto_rel         NULL
#define elf_info_to_howto             flare_elf_info_to_howto
#define elf_backend_relocate_section  flare_elf_relocate_section
#define bfd_elf32_bfd_relax_section   _flare_elf_relax_section
//#define elf_backend_gc_mark_hook      flare_elf_gc_mark_hook
//#define elf_backend_check_relocs      flare_elf_check_relocs
//

#define elf_backend_collect		true
#define elf_backend_type_change_ok	true
#define elf_backend_can_gc_sections	true
//#define elf_backend_can_gc_sections   true
#define elf_backend_rela_normal       true

#define elf_backend_default_execstack  0

#define bfd_elf32_bfd_reloc_type_lookup flare_reloc_type_lookup
#define bfd_elf32_bfd_reloc_name_lookup flare_reloc_name_lookup

#include "elf32-target.h"

#undef TARGET_LITTLE_SYM
#undef TARGET_LITTLE_NAME
#undef TARGET_BIG_SYM
#undef TARGET_BIG_NAME

#undef ELF_MAXPAGESIZE
//#undef ELF_COMMONPAGESIZE

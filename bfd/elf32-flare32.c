/* flare32-specific support for 32-bit ELF.
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

#include "elf32-flare32.h"
#include "opcode/flare32.h"
#include "elf/flare32.h"

#define USE_RELA

/* Forward declarations. */
// Special function for R_FLARE32_G1G5G6_S17, R_FLARE32_G1G5G6_S32,
// R_FLARE32_G3_S21_PCREL, and R_FLARE32_G3_S32_PCREL
static bfd_reloc_status_type
flare32_elf_ext_reloc (bfd *abfd,
                      arelent *reloc_entry,
                      asymbol *symbol,
                      void *data,
                      asection *input_section,
                      bfd *output_bfd,
                      char **error_message);
static bfd_reloc_status_type
flare32_elf_do_ext_reloc (bfd *input_bfd, reloc_howto_type *howto,
                          asection *input_section,
                          void *contents, bfd_vma address,
                          bfd_vma relocation, bfd_vma addend);

// Special function for R_FLARE32_G3_S21_PCREL and R_FLARE32_G3_S32_PCREL
//static bfd_reloc_status_type
//flare32_elf_pre_lpre_pcrel_reloc (bfd *abfd,
//                            arelent *reloc_entry,
//                            asymbol *symbol,
//                            void *data,
//                            asection *input_section,
//                            bfd *output_bfd,
//                            char **error_message);

static reloc_howto_type flare32_elf_howto_table [] =
{
  /* -------- */
  /* This reloc does nothing. */
  HOWTO (R_FLARE32_NONE,        /* type */
      0,                        /* rightshift */
      3,                        /* size (0 = byte, 1 = short, 2 = long) */
      0,                        /* bitsize */
      false,                    /* pc_relative */
      0,                        /* bitpos */
      complain_overflow_dont,   /* complain_on_overflow */
      bfd_elf_generic_reloc,    /* special_function */
      "R_FLARE32_NONE",         /* name */
      false,                    /* partial_inplace */
      0,                        /* src_mask */
      0,                        /* dst_mask */
      false),                   /* pcrel_offset */
  /* -------- */
  /* A 32-bit absolute  relocation. */
  HOWTO (R_FLARE32_32,          /* type */
      0,                        /* rightshift */
      2,                        /* size (0 = byte, 1 = short, 2 = long) */
      32,                       /* bitsize */
      false,                    /* pc_relative */
      0,                        /* bitpos */
      complain_overflow_bitfield, /* complain_on_overflow */
      bfd_elf_generic_reloc,    /* special_function */
      "R_FLARE32_32",           /* name */
      false,                    /* partial_inplace */
      0x00000000,               /* src_mask */
      0xffffffff,               /* dst_mask */
      false),                   /* pcrel_offset */
  /* -------- */
  /* A relocation for the immediate value of an instruction in 
  group 1, 5, or 6 without `pre`/`lpre` */
  HOWTO (R_FLARE32_G1G5G6_S5,   /* type */
      0,                        /* rightshift */
      1,                        /* size (0 = byte, 1 = short, 2 = long) */
      5,                        /* bitsize */
      false,                    /* pc_relative */
      FLARE32_G1G5G6_S5_BITPOS, /* bitpos */
      complain_overflow_signed, /* complain_on_overflow */
      bfd_elf_generic_reloc,    /* special_function */
      "R_FLARE32_G1G5G6_S5",    /* name */
      false,                    /* partial_inplace */
      0x0000,                   /* src_mask */
      FLARE32_G1G5G6_S5_MASK,   /* dst_mask */
      false),                   /* pcrel_offset */

  /* A relocation for the immediate value of an instruction in 
  group 1, 5, or 6 with `pre` */
  HOWTO (R_FLARE32_G1G5G6_S17,  /* type */
      0,                        /* rightshift */
      2,                        /* size (0 = byte, 1 = short, 2 = long) */
      17,                       /* bitsize */
      false,                    /* pc_relative */
      FLARE32_G1G5G6_S5_BITPOS, /* bitpos */
      complain_overflow_dont,   /* complain_on_overflow */
      flare32_elf_ext_reloc,  /* special_function */
      "R_FLARE32_G1G5G6_S17",   /* name */
      false,                    /* partial_inplace */
      0x00000000,               /* src_mask */
      /* dst_mask */
      (FLARE32_PRE_EXT_LSMASK | FLARE32_G1G5G6_S5_MASK), //0x0fff1f00,
      false),                   /* pcrel_offset */

  /* A relocation for the immediate value of an instruction in 
  group 1, 5, or 6 with `lpre` */
  HOWTO (R_FLARE32_G1G5G6_S32,  /* type */
      0,                        /* rightshift */
      4,                        /* size (0 = byte, 1 = short, 2 = long) */
      32,                       /* bitsize */
      false,                    /* pc_relative */
      FLARE32_G1G5G6_S5_BITPOS, /* bitpos */
      complain_overflow_dont,   /* complain_on_overflow */
      flare32_elf_ext_reloc,  /* special_function */
      "R_FLARE32_G1G5G6_S32",   /* name */
      false,                    /* partial_inplace */
      0x0,                      /* src_mask */
      /* dst_mask */
      //(bfd_vma)0x07ffffff1f00, 
      (FLARE32_G1G5G6_LPRE_EXT_LSMASK | FLARE32_G1G5G6_S5_MASK),
      false),                   /* pcrel_offset */
  /* -------- */
  /* A relocation for the immediate value of an instruction in 
  group 3 without `pre`/`lpre` */
  HOWTO (R_FLARE32_G3_S9_PCREL, /* type */
      1,                        /* rightshift */
      1,                        /* size (0 = byte, 1 = short, 2 = long) */
      FLARE32_G3_S9_BITSIZE,    /* bitsize */
      true,                     /* pc_relative */
      FLARE32_G3_S9_BITPOS,     /* bitpos */
      complain_overflow_signed,   /* complain_on_overflow */
      bfd_elf_generic_reloc,    /* special_function */
      "R_FLARE32_G3_S9_PCREL",  /* name */
      false,                    /* partial_inplace */
      0x0000,                   /* src_mask */
      /* dst_mask */
      FLARE32_G3_S9_MASK, //0x1ff0
      true),                    /* pcrel_offset */

  /* A relocation for the immediate value of an instruction in 
  group 3 with `pre` */
  HOWTO (R_FLARE32_G3_S21_PCREL,  /* type */
      1,                          /* rightshift */
      2,                          /* size (0 = byte, 1 = short, 2 = long) */
      21,                         /* bitsize */
      true,                       /* pc_relative */
      FLARE32_G3_S9_BITPOS,       /* bitpos */
      complain_overflow_dont,     /* complain_on_overflow */
      flare32_elf_ext_reloc, /* special_function */
      "R_FLARE32_G3_S21_PCREL",   /* name */
      false,                      /* partial_inplace */
      0x00000000,                 /* src_mask */
      /* dst_mask */
      (FLARE32_PRE_EXT_LSMASK | FLARE32_G3_S9_MASK), //0x07ff1ff0,
      true),                      /* pcrel_offset */

  /* A relocation for the immediate value of an instruction in 
  group 3 with `lpre` */
  HOWTO (R_FLARE32_G3_S32_PCREL,  /* type */
      1,                          /* rightshift */
      4,                          /* size (0 = byte, 1 = short, 2 = long) */
      32,                         /* bitsize */
      true,                       /* pc_relative */
      FLARE32_G3_S9_BITPOS,       /* bitpos */
      complain_overflow_dont,     /* complain_on_overflow */
      flare32_elf_ext_reloc, /* special_function */
      "R_FLARE32_G3_S32_PCREL",   /* name */
      false,                      /* partial_inplace */
      0x0,                        /* src_mask */
      /* dst_mask */
      (FLARE32_G3_LPRE_EXT_LSMASK | FLARE32_G3_S9_MASK),
      true),                     /* pcrel_offset */
  /* -------- */
};

struct flare32_reloc_map
{
  bfd_reloc_code_real_type bfd_reloc_val;
  unsigned int flare32_reloc_val;
};

static const struct flare32_reloc_map flare32_reloc_map [] =
{
  { BFD_RELOC_NONE, R_FLARE32_NONE },
  { BFD_RELOC_32, R_FLARE32_32 },
  { BFD_RELOC_FLARE32_G1G5G6_S32, R_FLARE32_G1G5G6_S32 },
  { BFD_RELOC_FLARE32_G3_S32_PCREL, R_FLARE32_G3_S32_PCREL },
  { BFD_RELOC_CTOR, R_FLARE32_32 },
};

reloc_howto_type *
flare32_reloc_type_lookup (bfd *abfd ATTRIBUTE_UNUSED,
                        bfd_reloc_code_real_type code)
{
  /* -------- */
  unsigned int i;
  /* -------- */
  for (i=0; i<FLARE32_ARRAY_SIZE(flare32_reloc_map); ++i)
  {
    if (flare32_reloc_map[i].bfd_reloc_val == code)
    {
      return &flare32_elf_howto_table
        [flare32_reloc_map[i].flare32_reloc_val];
    }
  }
  /* -------- */
  return NULL;
  /* -------- */
}

reloc_howto_type *
flare32_reloc_name_lookup (bfd *abfd ATTRIBUTE_UNUSED, const char *r_name)
{
  /* -------- */
  unsigned int i;
  /* -------- */
  for (i=0; i<FLARE32_ARRAY_SIZE(flare32_elf_howto_table); ++i)
  {
    if (flare32_elf_howto_table[i].name != NULL
      && strcasecmp (flare32_elf_howto_table[i].name, r_name) == 0)
    {
      return &flare32_elf_howto_table[i];
    }
  }
  /* -------- */
  return NULL;
  /* -------- */
}
/* -------- */
/* Return the howto for relocation `r_type`.  */
static reloc_howto_type *
flare32_lookup_howto (unsigned int r_type)
{
  return &flare32_elf_howto_table[r_type];
}
/* -------- */
/* Implement elf_info_to_howto:
   Given a ELF32 relocation, fill in a arelent structure.  */
/* Adapted from elf32-pru.c */
static bool
flare32_elf_info_to_howto (bfd *abfd,
                          arelent *cache_ptr,
                          Elf_Internal_Rela *dst)
{
  unsigned int r_type;

  r_type = ELF32_R_TYPE (dst->r_info);
  if (r_type >= R_FLARE32_max)
  {
    /* xgettext:c-format */
    _bfd_error_handler (_("%pB: unsupported relocation type %#x"), abfd,
      r_type);
    bfd_set_error (bfd_error_bad_value);
    return false;
  }

  cache_ptr->howto = flare32_lookup_howto (r_type);
  return cache_ptr->howto != NULL;
}

/* -------- */
// Special function for R_FLARE32_G1G5G6_S17 and R_FLARE32_G1G5G6_S32
static bfd_reloc_status_type
flare32_elf_ext_reloc (bfd *abfd,
                      arelent *reloc_entry,
                      asymbol *symbol,
                      void *data,
                      asection *input_section,
                      bfd *output_bfd,
                      char **error_message)
{
  // If this isn't a final relocation, then just use the generic relocation 
  // function.
  if (output_bfd != NULL)
  {
    //return bfd_reloc_continue;
    return bfd_elf_generic_reloc (abfd, reloc_entry, symbol, data,
      input_section, output_bfd, error_message);
  }
  return flare32_elf_do_ext_reloc (abfd, reloc_entry->howto,
    input_section,
    data,
    reloc_entry->address,
    (
      symbol->value
      + symbol->section->output_section
        ->vma
      + symbol->section->output_offset
    ),
    reloc_entry->addend);
}
static bfd_reloc_status_type
flare32_elf_do_ext_reloc (bfd *input_bfd, reloc_howto_type *howto,
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
  //if (output_bfd != NULL
  //  && (symbol->flags & BSF_SECTION_SYM) == 0
  //  && (!howto->partial_inplace || reloc_entry->addend == 0))
  //{
  //  reloc_entry->address += input_section->output_offset;
  //  return bfd_reloc_ok;
  //}
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
  if (!howto->pc_relative)
  {
    flare32_temp_t prefix_insn, insn;
    BFD_ASSERT(
      howto->type == R_FLARE32_G1G5G6_S17
      || howto->type == R_FLARE32_G1G5G6_S32
    );

    if (howto->type == R_FLARE32_G1G5G6_S17)
    {
      prefix_insn = bfd_get_16(input_bfd, contents + address);
      insn = bfd_get_16(input_bfd, contents + address + 2U);
      relocation += flare32_sign_extend
        (flare32_get_g1g5g6_s17(prefix_insn, insn), 32);
      flare32_put_g1g5g6_s17(&prefix_insn, &insn, relocation);
      bfd_put_16(input_bfd, prefix_insn, contents + address);
      bfd_put_16(input_bfd, insn, contents + address + 2U);
    }
    else // if (howto->type == R_FLARE32_G1G5G6_S32)
    {
      prefix_insn = bfd_get_32(input_bfd, contents + address);
      insn = bfd_get_16(input_bfd, contents + address + 4U);
      relocation += flare32_get_g1g5g6_s32(prefix_insn, insn);
      flare32_put_g1g5g6_s32(&prefix_insn, &insn, relocation);
      bfd_put_32(input_bfd, prefix_insn, contents + address);
      bfd_put_16(input_bfd, insn, contents + address + 4U);
    }
  }
  else // if (howto->pc_relative)
  {
    flare32_temp_t prefix_insn, insn;
    relocation -= (input_section->output_section->vma
                  + input_section->output_offset);
    if (howto->pcrel_offset)
    {
      relocation -= address;
    }
    BFD_ASSERT(
      howto->type == R_FLARE32_G3_S21_PCREL
      || howto->type == R_FLARE32_G3_S32_PCREL
    );

    // Check whether the relocation is word-aligned
    if (relocation & (((bfd_vma) 1 << howto->rightshift) - (bfd_vma) 1))
    {
      return bfd_reloc_outofrange;
    }

    if (howto->type == R_FLARE32_G3_S21_PCREL)
    {
      prefix_insn = bfd_get_16(input_bfd, contents + address);
      insn = bfd_get_16(input_bfd, contents + address + 2U);
      relocation += flare32_sign_extend
        (flare32_get_g3_s21(prefix_insn, insn), 32);
      flare32_put_g3_s21(&prefix_insn, &insn, relocation);
      bfd_put_16(input_bfd, prefix_insn, contents + address);
      bfd_put_16(input_bfd, insn, contents + address + 2U);
    }
    else // if (howto->type == R_FLARE32_G3_S32_PCREL)
    {
      prefix_insn = bfd_get_32(input_bfd, contents + address);
      insn = bfd_get_16(input_bfd, contents + address + 4U);
      relocation += flare32_get_g3_s32(prefix_insn, insn);
      flare32_put_g3_s32(&prefix_insn, &insn, relocation);
      bfd_put_32(input_bfd, prefix_insn, contents + address);
      bfd_put_16(input_bfd, insn, contents + address + 4U);
    }
  }
  /* -------- */
  return bfd_reloc_ok;
  /* -------- */
}
/* Implement elf_backend_relocate_section.  */
/* Adapted from elf32-pru.c */
static int
flare32_elf_relocate_section (bfd *output_bfd,
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

  for (rel = relocs; rel < relend; rel++)
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
    howto = flare32_lookup_howto ((unsigned) ELF32_R_TYPE (rel->r_info));
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
    if (bfd_link_relocatable (info))
    {
      continue;
    }

    if (howto)
    {
      switch (howto->type)
      {
        case R_FLARE32_NONE:
          /* We don't need to find a value for this symbol.  It's just a
          marker.  */
          r = bfd_reloc_ok;
          break;

        case R_FLARE32_32:
        case R_FLARE32_G1G5G6_S5:
        case R_FLARE32_G3_S9_PCREL:
          r = _bfd_final_link_relocate (howto, input_bfd,
                input_section, contents,
                rel->r_offset, relocation,
                rel->r_addend);
          break;

        case R_FLARE32_G1G5G6_S17:
        case R_FLARE32_G1G5G6_S32:
        case R_FLARE32_G3_S21_PCREL:
        case R_FLARE32_G3_S32_PCREL:
          r = flare32_elf_do_ext_reloc (input_bfd, howto,
                input_section, contents,
                rel->r_offset, relocation,
                rel->r_addend);
          break;

        default:
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
flare32_relax_can_shrink_value (bfd_vma value,
                                flare32_temp_t curr_bitsize,
                                flare32_temp_t target_bitsize)
{
  return (
    (bfd_signed_vma) value
      == ((
        (bfd_signed_vma) value
        >> (curr_bitsize - target_bitsize)
      ) << (curr_bitsize - target_bitsize))
  );
}
typedef struct flare32_relax_temp_t
{
  bfd *abfd;
  asection *sec;
  Elf_Internal_Shdr *symtab_hdr;
  Elf_Internal_Sym *isymbuf;
  Elf_Internal_Rela *internal_relocs;
  Elf_Internal_Rela *irel;
  bfd_byte *contents;
  bfd_vma value;
  //bfd_vma dot;
  //bfd_vma gap;
  bool is_pc_relative;
  bool rm_prefix;
  bool was_lpre;
  flare32_temp_t curr_bitsize;
  flare32_temp_t target_bitsize;
  //flare32_temp_t prefix_insn_bitsize;
  //flare32_temp_t prefix_insn_bitpos;
  //flare32_temp_t prefix_insn_mask;
  //flare32_temp_t insn_bitsize;
  //flare32_temp_t insn_bitpos;
  //flare32_temp_t insn_mask;
} flare32_relax_temp_t;

/* Delete some bytes from a section while changing the size of an
  instruction.
  The parameter "addr" denotes the section-relative offset pointing just
  behind the shrinked instruction. "addr+count" point at the first
  byte just behind the original unshrinked instruction.

  Idea copied from the PRU port.  */

static bool
flare32_elf_relax_delete_bytes (bfd *abfd,
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
  for (irel = elf_section_data (sec)->relocs; irel < irelend; irel++)
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

    for (isec = abfd->sections; isec; isec = isec->next)
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

      for (irelend = irel + isec->reloc_count;
          irel < irelend;
          irel++)
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
      for (; isym < isymend; isym++)
        {
          if (isym->st_shndx == sec_shndx)
            {
              if (isym->st_value > addr
                  && isym->st_value <= toaddr)
                isym->st_value -= count;

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
  for (; sym_hashes < end_hashes; sym_hashes++)
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
flare32_do_partial_relax_prefix_innards (flare32_relax_temp_t *args)
{
  /* Note that we'vve changed the relocs, section contents. */
  elf_section_data (args->sec)->relocs = args->internal_relocs;
  elf_section_data (args->sec)-> this_hdr.contents = args->contents;
  args->symtab_hdr->contents = (unsigned char *) args->isymbuf;

  if (args->was_lpre)
  {
    if (args->rm_prefix) // remove the `lpre` instruction
    {
      if (!flare32_elf_relax_delete_bytes (args->abfd, args->sec,
        args->irel->r_offset, 4))
      {
        return false;
      }
      args->irel->r_info
        = ELF32_R_INFO (ELF32_R_SYM (args->irel->r_info),
          !args->is_pc_relative
          ? R_FLARE32_G1G5G6_S5
          : R_FLARE32_G3_S9_PCREL);
    }
    else // if (!args->rm_prefix) // convert to `pre`
    {
      bfd_put_16 (args->abfd,
        flare32_enc_temp_insn_pre (bfd_get_16 (args->abfd,
            args->contents + args->irel->r_offset + 2)),
        args->contents + args->irel->r_offset + 2);

      if (!flare32_elf_relax_delete_bytes (args->abfd, args->sec,
        args->irel->r_offset, 2U))
      {
        return false;
      }

      args->irel->r_info
        = ELF32_R_INFO (ELF32_R_SYM (args->irel->r_info),
          !args->is_pc_relative
          ? R_FLARE32_G1G5G6_S17
          : R_FLARE32_G3_S21_PCREL);
    }
  }
  else // if (!args->was_lpre) // remove the `pre` instruction
  {
    if (!flare32_elf_relax_delete_bytes (args->abfd, args->sec,
      args->irel->r_offset, 2))
    {
      return false;
    }
    args->irel->r_info
      = ELF32_R_INFO (ELF32_R_SYM (args->irel->r_info),
        !args->is_pc_relative
        ? R_FLARE32_G1G5G6_S5
        : R_FLARE32_G3_S9_PCREL);
  }

  return true;
}

static bool
flare32_do_partial_relax_prefix (bfd *abfd,
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
  howto = flare32_lookup_howto (ELF32_R_TYPE (irel->r_info));

  /* For simplicity of coding, we are going to modify the section
      contents, the section relocs, and the BFD symbol table.  We
      must tell the rest of the code not to free up this
      information.  It would be possible to instead create a table
      of changes which have to be made, as is done in coff-mips.c;
      that would be more work, but would require less memory when
      the linker is run.  */
  /* Check if we can convert an `lpre` instruction to a `pre`
    instruction, or otherwise to there being neither `pre` nor `lpre` */
  if (howto->type == R_FLARE32_G1G5G6_S17
    || howto->type == R_FLARE32_G1G5G6_S32
    || howto->type == R_FLARE32_G3_S21_PCREL
    || howto->type == R_FLARE32_G3_S32_PCREL)
  {
    bfd_vma
      value = symval + irel->r_addend;
      //dot = 0, gap;

    flare32_temp_t
      //prefix_insn,
      //insn,
      prefix_insn_bitsize, //prefix_insn_bitpos, prefix_insn_mask,
      insn_bitsize, //insn_bitpos, insn_mask, 
      curr_bitsize, target_bitsize;
      //target_value = 0;
    bool
      was_lpre = false;

    //if (howto->pc_relative)
    //{
    //  /* Get the address of this instruction. */
    //  dot = sec->output_section->vma
    //    + sec->output_offset + irel->r_offset;
    //}

    ///* Compute the distance from this insn to the branch target, if there 
    //  is one. */
    //gap = value - dot - 2;

    if (howto->type == R_FLARE32_G1G5G6_S17)
    {
      //prefix_insn = bfd_get_16 (abfd, contents + irel->r_offset);
      //insn = bfd_get_16 (abfd, contents + irel->r_offset + 2);

      //prefix_insn_size = 16;
      prefix_insn_bitsize = FLARE32_PRE_S12_BITSIZE;
      //prefix_insn_bitpos = FLARE32_PRE_S12_BITPOS;
      //prefix_insn_mask = FLARE32_PRE_S12_MASK;
      insn_bitsize = FLARE32_G1G5G6_S5_BITSIZE;
      //insn_bitpos = FLARE32_G1G5G6_S5_BITPOS;
      //insn_mask = FLARE32_G1G5G6_S5_MASK;
      
      target_bitsize = insn_bitsize;
    }
    else if (howto->type == R_FLARE32_G1G5G6_S32)
    {
      was_lpre = true;

      //prefix_insn = bfd_get_32 (abfd, contents + irel->r_offset);
      //insn = bfd_get_16 (abfd, contents + irel->r_offset + 4);

      //prefix_insn_size = 32;
      prefix_insn_bitsize = FLARE32_G1G5G6_LPRE_S27_BITSIZE;
      //prefix_insn_bitpos = FLARE32_G1G5G6_LPRE_S27_BITPOS;
      //prefix_insn_mask = FLARE32_G1G5G6_LPRE_S27_MASK;
      insn_bitsize = FLARE32_G1G5G6_S5_BITSIZE;
      //insn_bitpos = FLARE32_G1G5G6_S5_BITPOS;
      //insn_mask = FLARE32_G1G5G6_S5_MASK;

      target_bitsize = FLARE32_PRE_S12_BITSIZE + insn_bitsize;
    }
    else if (howto->type == R_FLARE32_G3_S21_PCREL)
    {
      //prefix_insn = bfd_get_16 (abfd, contents + irel->r_offset);
      //insn = bfd_get_16 (abfd, contents + irel->r_offset + 2);

      //flare32_put_g3_s21 (&prefix_insn,
      //                    &insn,
      //                    value);

      //prefix_insn_size = 16;
      prefix_insn_bitsize = FLARE32_PRE_S12_BITSIZE;
      //prefix_insn_bitpos = FLARE32_PRE_S12_BITPOS;
      //prefix_insn_mask = FLARE32_PRE_S12_MASK;
      insn_bitsize = FLARE32_G3_S9_BITSIZE;
      //insn_bitpos = FLARE32_G3_S9_BITPOS;
      //insn_mask = FLARE32_G3_S9_MASK;

      target_bitsize = insn_bitsize;
    }
    else // if (howto->type == R_FLARE32_G3_S32_PCREL)
    {
      was_lpre = true;

      //prefix_insn = bfd_get_32 (abfd, contents + irel->r_offset);
      //insn = bfd_get_16 (abfd, contents + irel->r_offset + 4);

      //prefix_insn_size = 32;
      prefix_insn_bitsize = FLARE32_G3_LPRE_S23_BITSIZE;
      //prefix_insn_bitpos = FLARE32_G3_LPRE_S23_BITPOS;
      //prefix_insn_mask = FLARE32_G3_LPRE_S23_MASK;
      insn_bitsize = FLARE32_G3_S9_BITSIZE;
      //insn_bitpos = FLARE32_G3_S9_BITPOS;
      //insn_mask = FLARE32_G3_S9_MASK;

      target_bitsize = FLARE32_PRE_S12_BITSIZE + insn_bitsize;
    }
    curr_bitsize = prefix_insn_bitsize + insn_bitsize;

    if (flare32_relax_can_shrink_value (value, curr_bitsize,
      target_bitsize))
    {
      *again = true;

      flare32_relax_temp_t args = {
        abfd,
        sec,
        symtab_hdr,
        isymbuf,
        internal_relocs,
        irel,
        contents,
        value,
        //dot,
        //gap,
        howto->pc_relative,
        true, // rm_prefix
        was_lpre,
        curr_bitsize,
        target_bitsize,
        //prefix_insn_bitsize,
        //prefix_insn_bitpos,
        //prefix_insn_mask,
        //insn_bitsize,
        //insn_bitpos,
        //insn_mask
      };

      args.rm_prefix = (
        !was_lpre
        || flare32_relax_can_shrink_value (value, target_bitsize,
          insn_bitsize)
      );

      if (!flare32_do_partial_relax_prefix_innards (&args))
      {
        //goto error_return;
        return false;
      }
    }

    //if (
    //  (bfd_signed_vma) value
    //    == ((
    //      (bfd_signed_vma) value
    //      >> (curr_bitsize - target_bitsize)
    //    ) << (curr_bitsize - target_bitsize))
    //) {
    //}

    //if (value & FLARE32_N_ONES(32ull - (curr_bitsize - target_bitsize)))
    //{
    //  *again = true;
    //  if (!was_lpre) // attempt to remove the `pre`
    //  {
    //  }
    //  else // if (was_lpre) // attempt to convert an `lpre` to a `pre`
    //  {
    //  }
    //}
  }
  return true;
}


/* Adapted from elf32-pru.c */
static bool
flare32_elf_relax_section (bfd *abfd,
                          asection *sec,
                          struct bfd_link_info *link_info,
                          bool *again)
{
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
     this section does not have relocs, or if this is not a
     code section.  */
  if (bfd_link_relocatable (link_info)
      || sec->reloc_count == 0
      || (sec->flags & SEC_RELOC) == 0
      || (sec->flags & SEC_HAS_CONTENTS) == 0
      || (sec->flags & SEC_CODE) == 0)
    return true;
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
  for (irel = internal_relocs; irel < irelend; irel++)
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
        `flare32_elf_relocate_section()`, so I'm not sure this is
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

    if (!flare32_do_partial_relax_prefix (abfd, sec, again, symtab_hdr,
      isymbuf, internal_relocs, irel, contents, symval))
    {
      goto error_return;
    }




    ///* Check if we can remove an LDI instruction from the LDI32
    //    pseudo instruction if the upper 16 operand bits are zero.  */
    //if (ELF32_R_TYPE (irel->r_info) == (int) R_PRU_LDI32)
    //{
    //  bfd_vma value = symval + irel->r_addend;

    //  if (debug_relax)
    //  {
    //    printf ("R_PRU_LDI32 with value=0x%lx\n", (long) value);
    //  }

    //  if ((long) value >> 16 == 0)
    //  {
    //    unsigned long insn;

    //    /* Note that we've changed the relocs, section contents.  */
    //    elf_section_data (sec)->relocs = internal_relocs;
    //    elf_section_data (sec)->this_hdr.contents = contents;
    //    symtab_hdr->contents = (unsigned char *) isymbuf;

    //    /* Make the second instruction load the 16-bit constant
    //        into the full 32-bit register.  */
    //    insn = bfd_get_32 (abfd, contents + irel->r_offset + 4);

    //    /* Old GAS and LD versions have a bug, where the two
    //        LDI instructions are swapped.  Detect such object
    //        files and bail.  */
    //    if (GET_INSN_FIELD (RDSEL, insn) != RSEL_15_0)
    //      {
    //        /* xgettext:c-format */
    //        _bfd_error_handler (_("error: %pB: old incompatible object file detected"),
    //                            abfd);
    //        goto error_return;
    //      }

    //    SET_INSN_FIELD (RDSEL, insn, RSEL_31_0);
    //    bfd_put_32 (abfd, insn, contents + irel->r_offset + 4);

    //    /* Delete the first LDI instruction.  Note that there should
    //        be no relocations or symbols pointing to the second LDI
    //        instruction.  */
    //    if (!pru_elf_relax_delete_bytes (abfd, sec, irel->r_offset, 4))
    //      goto error_return;

    //    /* We're done with deletion of the first instruction.
    //        Set a regular LDI relocation for the second instruction
    //        we left to load the 16-bit value into the 32-bit
    //        register.  */
    //    irel->r_info = ELF32_R_INFO (ELF32_R_SYM (irel->r_info),
    //                                  R_PRU_U16);

    //    /* That will change things, so, we should relax again.
    //        Note that this is not required, and it may be slow.  */
    //    *again = true;
    //  }
    //}
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

#define ELF_ARCH            bfd_arch_flare32
#define ELF_MACHINE_CODE    EM_FLARE32_UNOFFICIAL
#define ELF_MAXPAGESIZE     0x1

#define TARGET_BIG_SYM    flare32_elf32_vec
#define TARGET_BIG_NAME   "elf32-flare32"

#define elf_info_to_howto_rel         NULL
#define elf_info_to_howto             flare32_elf_info_to_howto
#define elf_backend_relocate_section  flare32_elf_relocate_section
#define bfd_elf32_bfd_relax_section   flare32_elf_relax_section
//#define elf_backend_gc_mark_hook      flare32_elf_gc_mark_hook
//#define elf_backend_check_relocs      flare32_elf_check_relocs
//
#define elf_backend_can_gc_sections   1
#define elf_backend_rela_normal       1

#define bfd_elf32_bfd_reloc_type_lookup flare32_reloc_type_lookup
#define bfd_elf32_bfd_reloc_name_lookup flare32_reloc_name_lookup

#include "elf32-target.h"

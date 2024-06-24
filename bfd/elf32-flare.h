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

#include "sysdep.h"
#include "bfd.h"
#include "libbfd.h"
#include "elf-bfd.h"


extern reloc_howto_type *
flare32_reloc_type_lookup (bfd *abfd ATTRIBUTE_UNUSED,
                        bfd_reloc_code_real_type code);
extern reloc_howto_type *
flare32_reloc_name_lookup (bfd *abfd ATTRIBUTE_UNUSED, const char *r_name);

//extern bool
//flare32_elf_info_to_howto (bfd *abfd,
//                          arelent *cache_ptr,
//                          Elf_Internal_Rela *dst);
//extern bool
//flare32_elf_relax_section (bfd *abfd,
//                          asection *sec,
//                          struct bfd_link_info *link_info,
//                          bool *again);

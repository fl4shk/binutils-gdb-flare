/* snowhousecpu-relax-reloc-lookup.h:
   lookup `bfd_reloc_code_real_type`s for relaxing Flare instructions.

   Copyright (C) 2025 Free Software Foundation, Inc.
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

#ifndef _SNOWHOUSECPU_RELAX_RELOC_LOOKUP_H_
#define _SNOWHOUSECPU_RELAX_RELOC_LOOKUP_H_

//struct snowhousecpu_relax_reloc_tuple_mbr
//{
//  bfd_reloc_code_real_type reloc;
//  unsigned int r_type;
//};
//struct snowhousecpu_relax_reloc_tuple
//{
//  struct snowhousecpu_relax_reloc_tuple_mbr
//    small,
//    pre;
//    //pre_no_relax;
//    //lpre,
//    //lpre_no_relax;
//};
//
//static const struct snowhousecpu_relax_reloc_tuple
//snowhousecpu_relax_reloc_tuple_pcrel =
//{
//  {BFD_RELOC_SNOWHOUSECPU_S16_PCREL, R_SNOWHOUSECPU_S16_PCREL}, /* small */
//  {BFD_RELOC_SNOWHOUSECPU_S32_FOR_S16_PCREL, R_SNOWHOUSECPU_S32_FOR_S16_PCREL}, /* pre */
//  //{BFD_RELOC_SNOWHOUSECPU_S32_FOR_S16_PCREL, R_SNOWHOUSECPU_S32_FOR_S16_PCREL}, /* pre_no_relax */
//};

//static const struct snowhousecpu_relax_reloc_tuple
//snowhousecpu_relax_reloc_tuple_pcrel =
//{
//  {BFD_RELOC_SNOWHOUSECPU_G3_S9_PCREL, R_SNOWHOUSECPU_G3_S9_PCREL}, /* small */
//  {BFD_RELOC_SNOWHOUSECPU_G3_S21_PCREL, R_SNOWHOUSECPU_G3_S21_PCREL}, /* pre */
//  {BFD_RELOC_SNOWHOUSECPU_G3_S32_PCREL, R_SNOWHOUSECPU_G3_S32_PCREL}, /* lpre */
//  {BFD_RELOC_SNOWHOUSECPU_G3_S32_PCREL_NO_RELAX,
//    R_SNOWHOUSECPU_G3_S32_PCREL_NO_RELAX}, /* lpre_no_relax */
//};
//static const struct snowhousecpu_relax_reloc_tuple
//snowhousecpu_relax_reloc_tuple_g7_icreload =
//{
//  {BFD_RELOC_SNOWHOUSECPU_G7_ICRELOAD_S5, R_SNOWHOUSECPU_G7_ICRELOAD_S5}, /* small */
//  {BFD_RELOC_SNOWHOUSECPU_G7_ICRELOAD_S17, R_SNOWHOUSECPU_G7_ICRELOAD_S17}, /* pre */
//  {BFD_RELOC_SNOWHOUSECPU_G7_ICRELOAD_S32,
//    R_SNOWHOUSECPU_G7_ICRELOAD_S32}, /* lpre */
//  {BFD_RELOC_SNOWHOUSECPU_G7_ICRELOAD_S32_NO_RELAX,
//    R_SNOWHOUSECPU_G7_ICRELOAD_S32_NO_RELAX}, /* lpre_no_relax */
//};
//static const struct snowhousecpu_relax_reloc_tuple
//snowhousecpu_relax_reloc_tuple_g5_index_simm =
//{
//  {BFD_RELOC_SNOWHOUSECPU_G5_INDEX_S7, R_SNOWHOUSECPU_G5_INDEX_S7}, /* small */
//  {BFD_RELOC_SNOWHOUSECPU_G5_INDEX_S19, R_SNOWHOUSECPU_G5_INDEX_S19}, /* pre */
//  {BFD_RELOC_SNOWHOUSECPU_G5_INDEX_S32, R_SNOWHOUSECPU_G5_INDEX_S32}, /* lpre */
//  {BFD_RELOC_SNOWHOUSECPU_G5_INDEX_S32_NO_RELAX,
//    R_SNOWHOUSECPU_G5_INDEX_S32_NO_RELAX}, /* lpre_no_relax */
//};
//static const struct snowhousecpu_relax_reloc_tuple
//snowhousecpu_relax_reloc_tuple_g1_uimm =
//{
//  {BFD_RELOC_SNOWHOUSECPU_G1_U5,
//    R_SNOWHOUSECPU_G1_U5}, /* small */
//  {BFD_RELOC_SNOWHOUSECPU_G1_S17_FOR_U5,
//    R_SNOWHOUSECPU_G1_S17_FOR_U5}, /* pre */
//  {BFD_RELOC_SNOWHOUSECPU_G1_S32_FOR_U5,
//    R_SNOWHOUSECPU_G1_S32_FOR_U5}, /* lpre */
//  {BFD_RELOC_SNOWHOUSECPU_G1_S32_FOR_U5_NO_RELAX,
//    R_SNOWHOUSECPU_G1_S32_FOR_U5_NO_RELAX}, /* lpre_no_relax */
//};
//static const struct snowhousecpu_relax_reloc_tuple
//snowhousecpu_relax_reloc_tuple_g1_simm =
//{
//  {BFD_RELOC_SNOWHOUSECPU_G1_S5, R_SNOWHOUSECPU_G1_S5}, /* small */
//  {BFD_RELOC_SNOWHOUSECPU_G1_S17, R_SNOWHOUSECPU_G1_S17}, /* pre */
//  {BFD_RELOC_SNOWHOUSECPU_G1_S32, R_SNOWHOUSECPU_G1_S32}, /* lpre */
//  {BFD_RELOC_SNOWHOUSECPU_G1_S32_NO_RELAX,
//    R_SNOWHOUSECPU_G1_S32_NO_RELAX}, /* lpre_no_relax */
//};
//static inline const struct snowhousecpu_relax_reloc_tuple *
//snowhousecpu_relax_reloc_lookup
//  (bool is_pcrel,
//  bool is_g5_index,
//  bool is_g7_icreload,
//  bool is_small_imm_unsigned)
//{
//  return
//    !is_pcrel
//    ? (
//      !is_g7_icreload
//      ? (
//	!is_g5_index
//	? (
//	  !is_small_imm_unsigned
//	  ? &snowhousecpu_relax_reloc_tuple_g1_simm
//	  : &snowhousecpu_relax_reloc_tuple_g1_uimm
//	) : &snowhousecpu_relax_reloc_tuple_g5_index_simm
//      ) : &snowhousecpu_relax_reloc_tuple_g7_icreload
//    ) : &snowhousecpu_relax_reloc_tuple_pcrel;
//}

#endif    /* _SNOWHOUSECPU_RELAX_RELOC_LOOKUP_H_ */

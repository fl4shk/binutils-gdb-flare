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
#include <string.h>
#include "opcode/flare32.h"
/* -------- */
#include "opcode/flare32-opc-decls.h"
/* -------- */
//char flare32_opc_info_htab_key_buf[FLARE32_HTAB_KEY_BUF_LIM];

//const char *flare32_opc_info_to_htab_key
//  (char *cbuf, unsigned cbuf_lim, const flare32_opc_info_t *opc_info,
//  unsigned names_ind)
//{
//  //snprintf(cbuf, cbuf_lim,
//  //  "[%s][%s][%x]",
//  //  opc_info->names[0], opc_info->names[1], opc_info->oparg);
//  return flare32_op_oa_htab_key(cbuf, cbuf_lim, opc_info->names[names_ind],
//    opc_info->oparg);
//}
//const char *flare32_op_oa_htab_key
//  (char *cbuf, unsigned cbuf_lim, const char *name, flare32_oparg_t oparg)
//{
//  snprintf(cbuf, cbuf_lim,
//    "[%s][%x]", name, (unsigned) oparg);
//  return (const char *) cbuf;
//}
//flare32_opci_list_t *flare32_opci_list_create (void)
//{
//  flare32_opci_list_t *ret = (flare32_opci_list_t *) malloc
//    (sizeof (flare32_opci_list_t));
//  ret->opc_info = NULL;
//  ret->next = NULL;
//  return ret;
//}
//flare32_opci_list_t *flare32_opci_list_append
//  (flare32_opci_list_t *self, const flare32_opc_info_t *opc_info)
//{
//  flare32_opci_list_t *ret = flare32_opci_list_create ();
//  ret->opc_info = opc_info;
//  self->next = ret;
//  return ret;
//}
//void flare32_opci_list_delete (flare32_opci_list_t *self)
//{
//  flare32_opci_list_t
//    *p,
//    *next = NULL;
//  for (p=self; p!=NULL; p=next)
//  {
//    next = p->next;
//    free (p);
//  }
//}

//flare32_opci_list2d_t *flare32_opci_list2d_create (void)
//{
//  flare32_opci_list2d_t *ret
//    = (flare32_opci_list2d_t *) malloc(sizeof(flare32_opci_list2d_t));
//  ret->next = NULL;
//  return ret;
//}
//flare32_opci_list2d_t *flare32_opci_list2d_append
//  (flare32_opci_list2d_t *self, flare32_opci_list_t *list)
//{
//  flare32_opci_list2d_t *ret = flare32_opci_list2d_create ();
//  ret->list = list;
//  self->next = ret;
//  return ret;
//}
//void flare32_opci_list2d_delete (flare32_opci_list2d_t *self)
//{
//  if (self->next != NULL)
//  {
//    flare32_opci_list2d_delete (self->next);
//  }
//  flare32_opci_list_delete (self->list);
//  free (self);
//}

void
flare32_opci_vec_create (flare32_opci_vec_t *self)
{
  self->data = NULL;
  self->size = 0u;
}
const flare32_opc_info_t *
flare32_opci_vec_append (flare32_opci_vec_t *self,
  const flare32_opc_info_t *to_append)
{
  const size_t
    old_size = self->size;
  ++self->size;

  self->data = (const flare32_opc_info_t **)realloc
    (self->data, sizeof (*self->data) * self->size);
  self->data[old_size] = to_append;

  return self->data[old_size];
}
void
flare32_opci_vec_delete_data (flare32_opci_vec_t *self)
{
  if (self->data != NULL)
  {
    free (self->data);
  }
  //free (self);
}

void
flare32_opci_v2d_create (flare32_opci_v2d_t *self)
{
  self->data = NULL;
  self->size = 0u;
}
flare32_opci_vec_t *
flare32_opci_v2d_append (flare32_opci_v2d_t *self,
  const flare32_opci_vec_t *to_append)
{
  //const size_t
  //  old_size = self->size;
  //++self->size;

  //self->data = (flare32_opci_vec_t *)realloc
  //  (self->data, sizeof (*self->data) * self->size);
  ////self->data[old_size] = to_append;
  //memcpy (self->data + old_size, to_append, sizeof (*to_append));

  //return (self->data + old_size);

  const size_t
    old_size = self->size;
  ++self->size;

  self->data = (flare32_opci_vec_t *)realloc
    (self->data, sizeof (*self->data) * self->size);
  memcpy (self->data + old_size, to_append, sizeof (*to_append));

  return (self->data + old_size);
}
//void
//flare32_opci_v2d_append_opci (flare32_opci_v2d_t *self,
//  const flare32_opc_info_t *opc_info, size_t index)
//{
//}

void
flare32_opci_v2d_delete_data (flare32_opci_v2d_t *self)
{
  if (self->data != NULL)
  {
    //flare32_opci_vec_delete_data (self->data);
    for (size_t i=0; i<self->size; ++i)
    {
      flare32_opci_vec_delete_data (self->data + i);
    }
    free (self->data);
  }
}

/* snowhousecpu-opc.c -- Definitions for snowhousecpu opcodes.
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

#include "sysdep.h"
#include <string.h>
#include "opcode/snowhousecpu.h"
/* -------- */
#include "opcode/snowhousecpu-opc-decls.h"
/* -------- */
//char snowhousecpu_opc_info_htab_key_buf[SNOWHOUSECPU_HTAB_KEY_BUF_LIM];

//const char *snowhousecpu_opc_info_to_htab_key
//  (char *cbuf, unsigned cbuf_lim, const snowhousecpu_opc_info_t *opc_info,
//  unsigned names_ind)
//{
//  //snprintf(cbuf, cbuf_lim,
//  //  "[%s][%s][%x]",
//  //  opc_info->names[0], opc_info->names[1], opc_info->oparg);
//  return snowhousecpu_op_oa_htab_key(cbuf, cbuf_lim, opc_info->names[names_ind],
//    opc_info->oparg);
//}
//const char *snowhousecpu_op_oa_htab_key
//  (char *cbuf, unsigned cbuf_lim, const char *name, snowhousecpu_oparg_t oparg)
//{
//  snprintf(cbuf, cbuf_lim,
//    "[%s][%x]", name, (unsigned) oparg);
//  return (const char *) cbuf;
//}
//snowhousecpu_opci_list_t *snowhousecpu_opci_list_create (void)
//{
//  snowhousecpu_opci_list_t *ret = (snowhousecpu_opci_list_t *) malloc
//    (sizeof (snowhousecpu_opci_list_t));
//  ret->opc_info = NULL;
//  ret->next = NULL;
//  return ret;
//}
//snowhousecpu_opci_list_t *snowhousecpu_opci_list_append
//  (snowhousecpu_opci_list_t *self, const snowhousecpu_opc_info_t *opc_info)
//{
//  snowhousecpu_opci_list_t *ret = snowhousecpu_opci_list_create ();
//  ret->opc_info = opc_info;
//  self->next = ret;
//  return ret;
//}
//void snowhousecpu_opci_list_delete (snowhousecpu_opci_list_t *self)
//{
//  snowhousecpu_opci_list_t
//    *p,
//    *next = NULL;
//  for (p=self; p!=NULL; p=next)
//  {
//    next = p->next;
//    free (p);
//  }
//}

//snowhousecpu_opci_list2d_t *snowhousecpu_opci_list2d_create (void)
//{
//  snowhousecpu_opci_list2d_t *ret
//    = (snowhousecpu_opci_list2d_t *) malloc(sizeof(snowhousecpu_opci_list2d_t));
//  ret->next = NULL;
//  return ret;
//}
//snowhousecpu_opci_list2d_t *snowhousecpu_opci_list2d_append
//  (snowhousecpu_opci_list2d_t *self, snowhousecpu_opci_list_t *list)
//{
//  snowhousecpu_opci_list2d_t *ret = snowhousecpu_opci_list2d_create ();
//  ret->list = list;
//  self->next = ret;
//  return ret;
//}
//void snowhousecpu_opci_list2d_delete (snowhousecpu_opci_list2d_t *self)
//{
//  if (self->next != NULL)
//  {
//    snowhousecpu_opci_list2d_delete (self->next);
//  }
//  snowhousecpu_opci_list_delete (self->list);
//  free (self);
//}

void
snowhousecpu_opci_vec_create (snowhousecpu_opci_vec_t *self)
{
  self->data = NULL;
  self->size = 0u;
}
const snowhousecpu_opc_info_t *
snowhousecpu_opci_vec_append (snowhousecpu_opci_vec_t *self,
  const snowhousecpu_opc_info_t *to_append)
{
  const size_t
    old_size = self->size;
  ++self->size;

  self->data = (const snowhousecpu_opc_info_t **)realloc
    (self->data, sizeof (*self->data) * self->size);
  self->data[old_size] = to_append;

  return self->data[old_size];
}
void
snowhousecpu_opci_vec_delete_data (snowhousecpu_opci_vec_t *self)
{
  if (self->data != NULL)
  {
    free (self->data);
  }
  //free (self);
}

void
snowhousecpu_opci_v2d_create (snowhousecpu_opci_v2d_t *self)
{
  self->data = NULL;
  self->size = 0u;
}
snowhousecpu_opci_vec_t *
snowhousecpu_opci_v2d_append (snowhousecpu_opci_v2d_t *self,
  const snowhousecpu_opci_vec_t *to_append)
{
  //const size_t
  //  old_size = self->size;
  //++self->size;

  //self->data = (snowhousecpu_opci_vec_t *)realloc
  //  (self->data, sizeof (*self->data) * self->size);
  ////self->data[old_size] = to_append;
  //memcpy (self->data + old_size, to_append, sizeof (*to_append));

  //return (self->data + old_size);

  const size_t
    old_size = self->size;
  ++self->size;

  self->data = (snowhousecpu_opci_vec_t *)realloc
    (self->data, sizeof (*self->data) * self->size);
  memcpy (self->data + old_size, to_append, sizeof (*to_append));

  return (self->data + old_size);
}
//void
//snowhousecpu_opci_v2d_append_opci (snowhousecpu_opci_v2d_t *self,
//  const snowhousecpu_opc_info_t *opc_info, size_t index)
//{
//}

void
snowhousecpu_opci_v2d_delete_data (snowhousecpu_opci_v2d_t *self)
{
  if (self->data != NULL)
  {
    //snowhousecpu_opci_vec_delete_data (self->data);
    for (size_t i=0; i<self->size; ++i)
    {
      snowhousecpu_opci_vec_delete_data (self->data + i);
    }
    free (self->data);
  }
}

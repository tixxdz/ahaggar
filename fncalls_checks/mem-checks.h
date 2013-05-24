/*
 * The 'Ahaggar framework'
 *
 * Copyright (C) 2012-2013  Djalal Harouni <tixxdz@opendz.org>
 * Copyright (C) 2012-2013  LIRE Laboratory.
 * University Constantine 2, Algeria.
 *
 *   Author(s): Djalal Harouni <tixxdz@opendz.org>
 *              Pr. Mohamed Benmohammed
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2
 * of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef _MEM_CHECKS_H
#define _MEM_CHECKS_H

extern char *mem_to_int_cst_size(const char *call,
				 int arg_idx, int flags);

extern int chk_memory_size(const char *fnname,
			   const unsigned long size,
			   char *buf, size_t buflen);

extern int mem_chk_common_size(struct fncall_data *fc_data,
			       char *buf, size_t buflen);

extern int memset_chk_size(char *call, int flags,
			   char *buf, size_t buflen);

extern int memcpy_chk_size(char *call, int flags,
			   char *buf, size_t buflen);

#endif /* end of _MEM_CHECKS_H */

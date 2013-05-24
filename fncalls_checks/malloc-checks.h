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

#ifndef _MALLOC_CHECKS_H
#define _MALLOC_CHECKS_H

extern int malloc_chk_allocation_size(char *call, int flags,
				      char *buf, size_t buflen);

extern int kmalloc_chk_allocation_size(char *call, int flags,
				       char *buf, size_t buflen);

extern int kzalloc_chk_allocation_size(char *call, int flags,
				       char *buf, size_t buflen);

#endif /* end of _MALLOC_CHECKS_H */

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

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>

#include "checks-utils.h"
#include "mem-checks.h"
#include "malloc-checks.h"


int malloc_chk_size(char *call, int flags,
		    char *buf, size_t buflen)
{
	struct fncall_data fn;

	init_fncall_data(&fn, "malloc", call, 1, flags);
	return mem_chk_common_size(&fn, buf, buflen);
}

int kmalloc_chk_size(char *call, int flags,
		     char *buf, size_t buflen)
{
	struct fncall_data fn;

	init_fncall_data(&fn, "kmalloc", call, 1, flags);
	return mem_chk_common_size(&fn, buf, buflen);
}

int kzalloc_chk_size(char *call, int flags,
		     char *buf, size_t buflen)
{
	struct fncall_data fn;

	init_fncall_data(&fn, "kzalloc", call, 1, flags);
	return mem_chk_common_size(&fn, buf, buflen);
}

int strndup_chk_size(char *call, int flags,
		     char *buf, size_t buflen)
{
	struct fncall_data fn;

	init_fncall_data(&fn, "strndup", call, 2, flags);
	return mem_chk_common_size(&fn, buf, buflen);
}

int strndupa_chk_size(char *call, int flags,
		      char *buf, size_t buflen)
{
	struct fncall_data fn;

	init_fncall_data(&fn, "strndupa", call, 2, flags);
	return mem_chk_common_size(&fn, buf, buflen);
}

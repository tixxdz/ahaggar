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
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "checks-utils.h"
#include "regex-utils.h"
#include "mem-checks.h"

static char *int_const_tag = "<integer_cst> ";

char *mem_to_int_cst_size(const char *call, int arg_idx, int flags)
{
	char *ptr;

	if (!call)
		return NULL;

	ptr = call_start_at_arg(call, arg_idx);
	if (!ptr)
		return ptr;

	if (strncmp(ptr, int_const_tag, sizeof(int_const_tag)))
		return NULL;

	return get_arg_value(ptr);
}

int chk_memory_size(const char *fnname,
		    const unsigned long size,
		    char *buf, size_t buflen)
{
	int ret = REG_NOMATCH;

	if (!fnname || !*fnname)
		return ret;

	if (size == 0) {
		snprintf(buf, buflen,
			 "function '%s' argument size is 0",
			 fnname);
	} else if (size < sizeof(char *)) {
		snprintf(buf, buflen,
			 "function '%s' argument size equals %ld",
			 fnname, size);
	} else if (size == sizeof(char *)) {
		snprintf(buf, buflen,
			 "function '%s' argument size equals ptr size %ld",
			 fnname, size);
	}

	return 0;
}

int mem_chk_common_size(struct fncall_data *fc_data,
			char *buf, size_t buflen)
{
	char *ptr;
	char *endp;
	unsigned long int msize;
	int ret = REG_NOMATCH;
	struct fncall_data *fn = fc_data;

	if (!fn || !fn->fnname || !fn->call)
		return ret;

	ptr = mem_to_int_cst_size(fn->call, fn->arg_idx, fn->flags);
	if (!ptr)
		return ret;

	errno = 0;
	msize = strtoul(ptr, &endp, 0);
	if (errno || ptr == endp)
		return ret;

	return chk_memory_size(fn->fnname, msize, buf, buflen);
}

int memset_chk_size(char *call, int flags,
		    char *buf, size_t buflen)
{
	struct fncall_data fn;

	init_fncall_data(&fn, "memset", call, 3, flags);
	return mem_chk_common_size(&fn, buf, buflen);
}

int memcpy_chk_size(char *call, int flags,
		    char *buf, size_t buflen)
{
	struct fncall_data fn;

	init_fncall_data(&fn, "memcpy", call, 3, flags);
	return mem_chk_common_size(&fn, buf, buflen);
}

int strncpy_chk_size(char *call, int flags,
		     char *buf, size_t buflen)
{
	struct fncall_data fn;

	init_fncall_data(&fn, "strncpy", call, 3, flags);
	return mem_chk_common_size(&fn, buf, buflen);
}

int strncat_chk_size(char *call, int flags,
		     char *buf, size_t buflen)
{
	struct fncall_data fn;

	init_fncall_data(&fn, "strncat", call, 3, flags);
	return mem_chk_common_size(&fn, buf, buflen);
}

int memmove_chk_size(char *call, int flags,
		     char *buf, size_t buflen)
{
	struct fncall_data fn;

	init_fncall_data(&fn, "memmove", call, 3, flags);
	return mem_chk_common_size(&fn, buf, buflen);
}

int memcmp_chk_size(char *call, int flags,
		    char *buf, size_t buflen)
{
	struct fncall_data fn;

	init_fncall_data(&fn, "memcmp", call, 3, flags);
	return mem_chk_common_size(&fn, buf, buflen);
}

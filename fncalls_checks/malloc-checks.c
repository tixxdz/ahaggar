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
#include "malloc-checks.h"

static char *malloc_args_const = "<integer_cst> ";

int malloc_chk_allocation_size(char *call, int flags,
			       char *buf, size_t buflen)
{
	char *ptr;
	unsigned long int msize;
	int ret = REG_NOMATCH;
	char *s = call_start_at_arg(call, 1);

	if (!s)
		return ret;

	if (strncmp(s, malloc_args_const, sizeof(malloc_args_const)))
		return ret;

	ptr = strrchr(s, ' ');
	if (!ptr)
		return ret;

	errno = 0;
	msize = strtoul(++ptr, NULL, 0);
	if (errno)
		return ret;

	switch (msize) {
	case 0:
		snprintf(buf, buflen,
			 "function 'malloc' argument size is 0");
		break;
	case (sizeof(ptr)):
		snprintf(buf, buflen,
			 "function 'malloc' argument size equals ptr size %ld",
			 msize);
		break;
	default:
		break;
	}

	return 0;
}

char *kmalloc_to_allocation_size(const char *call, int arg_idx,
				 int flags)
{
	char *ptr = call_start_at_arg(call, arg_idx);

	if (!ptr)
		return ptr;

	if (strncmp(ptr, malloc_args_const, sizeof(malloc_args_const)))
		return NULL;

	ptr = strchr(ptr, ',');
	if (!ptr)
		return ptr;

	while (*ptr != ' ')
		ptr--;

	return ptr++;
}

int kmalloc_chk_allocation_size(char *call, int flags,
			        char *buf, size_t buflen)
{
	unsigned long int msize;
	int ret = REG_NOMATCH;
	char *ptr = kmalloc_to_allocation_size(call, 1, 0);

	if (!ptr)
		return ret;

	errno = 0;
	msize = strtoul(ptr, NULL, 0);
	if (errno)
		return ret;

	switch (msize) {
	case 0:
		snprintf(buf, buflen,
			 "function 'kmalloc' argument size is 0");
		break;
	case (sizeof(ptr)):
		snprintf(buf, buflen,
			 "function 'kmalloc' argument size equals ptr size %ld",
			 msize);
		break;
	default:
		break;
	}

	return 0;
}

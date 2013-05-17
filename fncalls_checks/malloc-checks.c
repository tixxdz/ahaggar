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
#include <regex.h>
#include <errno.h>

#include "regex-utils.h"
#include "malloc-checks.h"

static regex_t malloc_regex;
static char *malloc_args_const = "^\\(<integer_cst>\\s.*\\)$";

int malloc_chk_allocation_size(char *call, int flags,
			       char *buf, size_t buflen)
{
	int ret;
	char *ptr;
	char *s = call;
	unsigned long int msize;

	ret = regmatch(&malloc_regex, s,
		       malloc_args_const, buf, buflen);
	if (ret)
		return ret;

	ret = REG_NOMATCH;
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

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
#include <regex.h>

#include "../gcc-log.h"
#include "regex-utils.h"

static int reginit(regex_t *regex, char *pattern,
		   char *buf, size_t buflen)
{
	int ret = 0;

	if (*((char *)regex))
		return ret;

	ret = regcomp(regex, pattern, REG_EXTENDED);
	if (ret)
		regerror(ret, regex, buf, buflen);

	return ret;
}

int regmatch(regex_t *regex, char *call, char *pattern,
	     char *buf, size_t buflen)
{
	int ret = reginit(regex, pattern, buf, buflen);
	if (ret) {
		out_error("GCC plugin: failed to compile regexp: %s",
			  buf);
		return ret;
	}

	return regexec(regex, call, 0, NULL, 0);
}

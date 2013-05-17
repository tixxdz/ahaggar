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

#include <string.h>

#include "checks-utils.h"

char *call_start_at_arg(const char *call, int arg_idx)
{
	char *ptr = (char *)call;

	if (arg_idx > 1) {
		while (--arg_idx) {
			ptr = strchr(ptr, ',');
			if (!ptr)
				break;
			ptr += 2;
		}
	} else if (arg_idx < 0) {
		while (arg_idx++) {
			ptr = strrchr(ptr, ',');
			if (!ptr)
				break;
			ptr += 2;
		}
	} else if (arg_idx == 1) {
		if (*ptr == '(')
			ptr++;
		if (*ptr == ')')
			ptr = NULL;
	}

	return ptr;
}

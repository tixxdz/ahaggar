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

char *get_next_arg(char **call_offset)
{
	unsigned int subcalls = 0;
	char *end = NULL;
	char *ptr = *call_offset;

	if (!ptr)
		return ptr;

	if (*ptr == '(')
		ptr++;

	if (*ptr == ')' || *ptr == '\0') {
		ptr = NULL;
		goto out;
	}

	for (end = ptr; *end != '\0'; end++) {
		if (*end == '(') {
			subcalls++;
		} else if (*end == ')') {
			if (subcalls)
				subcalls--;
			else
				break;
		} else if (*end == ',' && subcalls == 0) {
			end += 2;
			break;
		}
	}

out:
	*call_offset = end;
	return ptr;
}

int get_args_nr(const char *call)
{
	int nr = 0;
	char *offset = (char *)call;

	for (; get_next_arg(&offset); nr++);

	return nr;
}

char *call_start_at_arg(const char *call, int arg_idx)
{
	char *offset = (char *)call;
	char *ptr = NULL;

	if (arg_idx > 0) {
		while (arg_idx--) {
			ptr = get_next_arg(&offset);
		}
	} else if (arg_idx < 0) {
		while (arg_idx++) {
			ptr = strrchr(ptr, ',');
			if (!ptr)
				break;
			ptr += 2;
		}
	} else if (arg_idx == 1) {
		ptr = get_next_arg(&offset);
	}

	return ptr;
}

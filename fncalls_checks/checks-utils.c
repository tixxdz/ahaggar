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

char *get_next_arg(const char *call, char **call_offset)
{
	unsigned int subcalls = 0;
	char *end = NULL;
	char *ptr = *call_offset;

	if (!ptr)
		return ptr;

	if (ptr == call)
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

char *get_arg_value(const char *arg)
{
	unsigned int subcalls = 0;
	char *v = NULL;
	char *ptr = (char *)arg;

	if (!ptr)
		return v;

	if (*ptr == ')')
		return v;

	for (; *ptr != '\0'; ptr++) {
		if (*ptr == '(') {
			subcalls++;
		} else if (*ptr == ')') {
			if (subcalls) {
				subcalls--;
			} else {
				v = ptr;
				break;
			}
		} else if (*ptr == ',' && subcalls == 0) {
			   v = ptr;
			   break;
		}
	}

	if (!v)
		return v;

	while (*v != ' ')
		v--;

	return ++v;
}

int get_args_nr(const char *call)
{
	int nr = 0;
	char *offset = (char *)call;

	for (; get_next_arg(call, &offset); nr++);

	return nr;
}

char *call_start_at_arg(const char *call, int arg_idx)
{
	char *offset = (char *)call;
	char *ptr = NULL;

	if (arg_idx > 0) {
		while (arg_idx--) {
			ptr = get_next_arg(call, &offset);
		}
	} else if (arg_idx < 0) {
		/* TODO: make it work */
		/*while (arg_idx++) {
			ptr = strrchr(ptr, ',');
			if (!ptr)
				break;
			ptr += 2;
		}*/
		return ptr;
	} else if (arg_idx == 1) {
		ptr = get_next_arg(call, &offset);
	}

	return ptr;
}

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

#include "gcc-plugin.h"

#include "str-utils.h"

struct substring *substring_init(void)
{
	struct substring *sub;

	sub = (struct substring *)xmalloc(sizeof(struct substring));
	memset(sub, 0, sizeof(struct substring));
	return sub;
}

void substring_fini(struct substring *sub)
{
	if (sub) {
		free(sub);
		sub = NULL;
	}
}

size_t substring_set(struct substring *sub,
		     const char *s, size_t len)
{
	char *str = (char *)s;
	size_t ret = len;

	if (ret) {
		sub_start(sub) = str;
		sub_end(sub) = str + ret;
		sub_len(sub) = ret;
	}

	return ret;
}

char *substring_to_strchr(struct substring *sub,
			  const char *s, int c)
{
	char *ch = NULL;
	char *str = (char *)s;

	ch = strchr(s, c);
	if (ch) {
		sub_start(sub) = str;
		sub_end(sub) = ch - 1;
		sub_len(sub) = sub_end(sub) - sub_start(sub);
	}

	return ch;
}

char *substring_move_to_strchr(struct substring *sub,
			       const char *s, int c)
{
	char *ch = NULL;

	ch = strchr(s, c);
	if (ch) {
		sub_start(sub) = ch;
		sub_end(sub) = ch + strlen(ch);
		sub_len(sub) = strlen(ch);
	}

	return ch;
}

char *substring_to_strstr(struct substring *sub,
			  const char *s1, const char *s2)
{
	char *str = NULL;
	char *s = (char *)s1;

	str = strstr(s1, s2);
	if (str) {
		sub_start(sub) = s;
		sub_end(sub) = str - 1;
		sub_len(sub) = sub_end(sub) - sub_start(sub);
	}

	return str;
}

char *substring_move_to_strstr(struct substring *sub,
			       const char *s1, const char *s2)
{
	char *str = NULL;

	str = strstr(s1, s2);
	if (str) {
		sub_start(sub) = str;
		sub_end(sub) = str + strlen(str);
		sub_len(sub) = strlen(str);
	}

	return str;
}

size_t substring_strncpy(char *dest, struct substring *src, size_t size)
{
	size_t ret = sub_len(src);

	if (ret) {
		size_t len = ret >= size ? size - 1 : ret;
		memcpy(dest, sub_start(src), len);
		dest[len] = '\0';
	}

	return ret;
}

char *substring_strdup(struct substring *sub)
{
	size_t size = sub_len(sub) + 1;
	char *str = xmalloc(size);

	substring_strncpy(str, sub, size);
	return str;
}

bool strstarts(const char *str, const char *prefix)
{
	return strncmp(str, prefix, strlen(prefix)) == 0;
}

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

#ifndef _STR_UTILS_H
#define _STR_UTILS_H

struct substring {
	char *start;
	char *end;
	unsigned long len;
};

#define sub_base(sub)		(sub)
#define sub_start(sub)		sub_base(sub)->start
#define sub_end(sub)		sub_base(sub)->end
#define sub_len(sub)		sub_base(sub)->len

struct substring *substring_init(void);
void substring_fini(struct substring *sub);

size_t substring_set(struct substring *sub,
		     const char *s, size_t len);

char *substring_to_strchr(struct substring *sub,
			  const char *s, int c);
#define substring_to_nextline(sub, s) \
	substring_to_strchr(sub_base(sub), s, '\n')

char *substring_move_to_strchr(struct substring *sub,
			       const char *s, int c);

char *substring_to_strstr(struct substring *sub,
			  const char *s1, const char *s2);
char *substring_move_to_strstr(struct substring *sub,
			       const char *s1, const char *s2);

size_t substring_strncpy(char *dest, struct substring *src, size_t size);
char *substring_strdup(struct substring *sub);
char *substring_strndup(struct substring *sub, size_t size);

bool strstarts(const char *str, const char *prefix);

/* Check the next char that is just after the sub->end */
inline static bool substring_next_strchr(struct substring *sub, int c)
{
	char *ch = sub_end(sub) + 1;
	return (*ch == c);
}

inline static void substring_addchr_start(struct substring *sub)
{
	sub_start(sub)++;
	sub_len(sub)--;
}

inline static void substring_addnchr_start(struct substring *sub,
					   size_t n)
{
	sub_start(sub)+=n;
	sub_len(sub)-=n;
}

inline static void substring_minuschr_start(struct substring *sub)
{
	sub_start(sub)--;
	sub_len(sub)++;
}

inline static void substring_addchr_end(struct substring *sub)
{
	sub_end(sub)++;
	sub_len(sub)++;
}

inline static void substring_addnchr_end(struct substring *sub,
					 size_t n)
{
	sub_end(sub)+=n;
	sub_len(sub)+=n;
}

inline static void substring_minuschr_end(struct substring *sub)
{
	sub_end(sub)--;
	sub_len(sub)--;
}

#endif /* _STR_UTIL_H */

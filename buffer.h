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

#ifndef _BUFFER_H
#define _BUFFER_H

#ifndef BUF_SIZE
#define BUF_SIZE 4096
#endif

typedef struct output_buf output_buf;

struct output_buf {
	char *buf;
	char *offset;
	unsigned long size;
	unsigned long used;
};

#define output_base(out)	(out)
#define output_buf(out)		output_base(out)->buf
#define output_offset(out)	output_base(out)->offset
#define output_size(out)	output_base(out)->size
#define output_used(out)	output_base(out)->used
#define output_strlen(out)	output_used(out)

output_buf *output_init(void);
void output_fini(output_buf *d);
output_buf *output_reset(output_buf *d);
void output_flush(output_buf *d);
output_buf *output_expand(output_buf *d,
				    unsigned long need);

output_buf *output_local_prepare(output_buf *d);

int output_base_char(output_buf *d, const int c);
int output_base_printf(output_buf *d, const char *fmt, ...);
void output_base_indent_to(output_buf *d, const int column);

int output_base_swap_meta(output_buf *a,
			  output_buf *b);
int output_base_copy(output_buf *dest,
		     output_buf *src, size_t n);
int output_base_concat(output_buf *dest, output_buf *src);

#define output_char(out, c)	output_base_char(output_base(out), c)
#define output_space(out)	output_char(out, ' ')
#define output_newline(out)	output_char(out, '\n')

#define output_printf(out, fmt, args...) \
	output_base_printf(output_base(out), fmt, ##args)

#define output_indent_to(out, n) \
	output_base_indent_to(output_base(out), n)

#define output_indent_to_newline(out, n) \
do { \
	if (n > 0) \
		output_newline(out); \
	output_indent_to(out, n); \
} while(0)

#define output_swap_meta(a, b) \
	output_base_swap_meta(output_base(a), output_base(b))

#define output_copy_n(dest, src, n) \
	output_base_copy(output_base(dest), \
			 output_base(src), n)

#define output_copy(dest, src) \
	output_base_copy(output_base(dest), \
			 output_base(src), output_strlen(src))

#define output_concat(dest, src) \
	output_base_concat(output_base(dest), output_base(src))

#define output_append(dest, src) output_concat(dest, src)

#endif /* _BUFFER_H */

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
#include <stdarg.h>

#include "gcc-plugin.h"

#include "buffer.h"

static output_buf *__output_init(unsigned int len)
{
	output_buf *out;

	if (len < BUF_SIZE)
		len = BUF_SIZE;

	out = (output_buf *)xmalloc(sizeof(output_buf));
	memset(out, 0, sizeof(output_buf));

	output_size(out) = len;
	/* output_used(out) = 0; */
	output_buf(out) = (char *)xmalloc(output_size(out));

	memset(out->buf, 0, output_size(out));
	output_offset(out) = output_buf(out);

	return out;
}

output_buf *output_init(void)
{
	return __output_init(BUF_SIZE);
}

void output_fini(output_buf *d)
{
	if (d) {
		free(d->buf);
		free(d);
		d = NULL;
	}
}

output_buf *output_reset(output_buf *d)
{
	output_fini(d);
	return output_init();
}

void output_flush(output_buf *d)
{
	if (d) {
		memset(d->buf, 0, output_size(d));
		output_used(d) = 0;
		output_offset(d) = output_buf(d);
	}
}

/* callers should make sure that '\0' is included */
output_buf *output_expand(output_buf *d,
				    unsigned long need)
{
	output_buf *out = d;
	unsigned long newsize = output_size(out) * 2;

	if (!out)
		return __output_init(need);

	if (need < output_size(out) - output_used(out))
		return out;

	need += output_used(out);
	newsize = (newsize > need) ? newsize : need;

	output_size(out) = newsize;
	output_buf(out) = (char *)xrealloc(d->buf, newsize);
	output_offset(out) = output_buf(out) + output_used(out);
	*output_offset(out) = '\0';

	return out;
}

output_buf *output_local_prepare(output_buf *d)
{
	output_buf *out = d;

	if (!out)
		out = output_init();
	else
		output_flush(out);

	return out;
}

/* returns 0 on success, negative on errors */
int output_base_char(output_buf *d, const int c)
{
	int ret = -1;
	int need = 2;
	output_buf *out;

	out = output_expand(d, need);
	ret = snprintf(output_offset(out),
		       output_size(out) - output_strlen(out),
		       "%c", c);

	if (ret < 0)
		return ret;

	output_used(out) += ret;
	output_offset(out) = output_buf(out) + output_strlen(out);
	*output_offset(out) = '\0';

	return 0;
}

/* returns 0 on success, negative on errors */
int output_base_printf(output_buf *d, const char *fmt, ...)
{
	int ret = -1;
	int need;
	va_list va;
	output_buf *out;

	va_start(va, fmt);
	need = vsnprintf(NULL, 0, fmt, va);
	va_end(va);

	if (need < 0)
		return ret;

	need++;
	out = output_expand(d, need);

	va_start(va, fmt);
	ret = vsnprintf(output_offset(out),
			output_size(out) - output_strlen(out),
			fmt, va);
	va_end(va);

	if (ret < 0)
		return ret;

	output_used(out) += ret;
	output_offset(out) = output_buf(out) + output_strlen(out);
	*output_offset(out) = '\0';

	return 0;
}

void output_base_indent_to(output_buf *d, const int column)
{
	int i;

	for (i = 0; i < column; i++)
		output_space(d);
}

int output_base_swap_meta(output_buf *a,
			  output_buf *b)
{
	int ret = -1;
	output_buf tmp;

	if (!a || !b)
		return ret;

	memcpy(&tmp, a, sizeof(tmp));
	memcpy(a, b, sizeof(*a));
	memcpy(b, &tmp, sizeof(*b));

	return 0;
}

int output_base_copy(output_buf *dest,
		     output_buf *src,
		     size_t n)
{
	unsigned long need;

	if (n && n <= output_strlen(src))
		need = n;
	else
		return 0;

	need++;
	if(need < n)
		return 0;

	if (need > output_size(dest)) {
		output_fini(dest);
		dest = __output_init(need);
	}

	memcpy(output_buf(dest), output_buf(src), n);

	output_used(dest) = n;
	output_offset(dest) = output_buf(dest) + output_strlen(dest);
	*output_offset(dest) = '\0';

	return 0;

}

int output_base_concat(output_buf *dest, output_buf *src)
{
	unsigned long need = output_strlen(src);
	if(!need)
		return 0;

	need++;
	if (need < output_strlen(src))
		return 0;

	dest = output_expand(dest, need);
	memcpy(output_offset(dest), output_buf(src), output_strlen(src));

	output_used(dest) += output_used(src);
	output_offset(dest) = output_buf(dest) + output_strlen(dest);
	*output_offset(dest) = '\0';

	return 0;
}

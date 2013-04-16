/*
 * The Ahaggar framework
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
#include <stdarg.h>
#include <regex.h>

#include "gcc-plugin.h"
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tree.h"
#include "intl.h"
#include "plugin.h"
#include "diagnostic.h"
#include "options.h"
#include "hashtab.h"

#include "ahaggar.h"
#include "buffer.h"
#include "str-utils.h"
#include "log.h"
#include "fncalls-match.h"

#define SAFE_LOAD_FACTOR 25
#define MAX_HASH_ENTRIES 10240

#define TARGETS_ARRAY_SIZE(err, warn, report) \
	(ARRAY_SIZE(err) + ARRAY_SIZE(warn) + ARRAY_SIZE(report))


static int __string_eq(const void *, const void *);
static hashval_t __string_hash(const void *);
static inline int is_hash_safe(struct target_functions *,
			       unsigned int);

static void __error(__attribute__((unused)) int x,
		    __attribute__((unused)) const char *str,
		    const char *fmt, ...);
static void __warning(__attribute__((unused)) int x,
		      __attribute__((unused)) const char *str,
		      const char *fmt, ...);

#define out_error(fmt, args...) __error(0, NULL, fmt, ##args)
#define out_warning(fmt, args...) __warning(0, NULL, fmt, ##args)
#define out_report(x, s, fmt, args...) report(x, s, fmt, ##args)

extern const char *progname;

static struct target_functions errors[] = {
#include "file-errors.h"
#include "kfile-errors.h"
};

static struct target_functions warnings[] = {
};

static struct target_functions reports[] = {
#include "kmalloc-reports.h"
};

static struct hash_functions ghash[] = {
	{
		.tab = NULL,
		.targets = errors,
		.targets_size = ARRAY_SIZE(errors),
		.out_f = __error,
	},
	{
		.tab = NULL,
		.targets = warnings,
		.targets_size = ARRAY_SIZE(warnings),
		.out_f = __warning,
	},
	{
		.tab = NULL,
		.targets = reports,
		.targets_size = ARRAY_SIZE(reports),
		.out_f = report,
	},
};

static void __error(__attribute__((unused)) int x,
		    __attribute__((unused)) const char *str,
		    const char *fmt, ...)
{
	va_list args;
	diagnostic_info diagnostic;

	va_start(args, fmt);
	diagnostic_set_info(&diagnostic, fmt, &args,
			    input_location, DK_ERROR);

	report_diagnostic(&diagnostic);
	va_end(args);
}

static void __warning(__attribute__((unused)) int x,
		      __attribute__((unused)) const char *str,
		      const char *fmt, ...)
{
	va_list args;
	diagnostic_info diagnostic;

	va_start(args, fmt);
	diagnostic_set_info(&diagnostic, fmt, &args,
			    input_location, DK_WARNING);
	diagnostic.option_index = 0;

	report_diagnostic(&diagnostic);
	va_end(args);
}

static inline int is_hash_safe(struct target_functions *arr,
			       unsigned int elements)
{
	return (arr && elements
		&& (((elements * 100.0) / MAX_HASH_ENTRIES) \
		< 100.0 - SAFE_LOAD_FACTOR));
}

static htab_t __init_hash(struct target_functions *arr,
			  unsigned int elements)
{
	htab_t lhash = NULL;

	if (is_hash_safe(arr, elements))
		lhash = htab_create(MAX_HASH_ENTRIES,
				    __string_hash, __string_eq, NULL);

	return lhash;
}

static int __string_eq(const void *a, const void *b)
{
	const char *a0 = ((const struct target_functions *)a)->name;
	const char *b0 = (const char *)b;

	return (strcmp(a0, b0) == 0);
}

static hashval_t __string_hash(const void *str)
{
	return htab_hash_string(str);
}

static void * __lookup_key(htab_t hashtable, const char *key)
{
	PTR *slot = NULL;

	if (*key)
		slot = htab_find(hashtable, key);

	return slot;
}

static struct target_functions *lookup_target(htab_t hashtable,
					      struct target_functions *fn)
{
	htab_t h = hashtable;
	struct target_functions *f = fn;

	if (!f->name || !f->name[0])
		return NULL;

	return __lookup_key(h, f->name);
}

static void *__insert_key(htab_t hashtable, const char *key)
{
	PTR *slot = NULL;

	if (*key)
		slot = htab_find_slot(hashtable, key, INSERT);

	return slot;
}

static struct target_functions *insert_target(htab_t hashtable,
					      struct target_functions *fn)
{
	PTR *slot = NULL;
	htab_t h = hashtable;
	struct target_functions *f = fn;

	if (!f->name || !f->name[0])
		return NULL;

	slot = __insert_key(h, f->name);
	if (slot)
		*slot = f;

	return *slot;
}

/* return 0 on success */
static int populate_hash_target(htab_t hashtable,
				struct target_functions *fn)
{
	int ret = -1;
	htab_t h = hashtable;
	struct target_functions *f = fn;

	if (!lookup_target(h, f)) {
		if (!insert_target(h, f))
			return ret;
	} else {
		out_warning("hashtable duplicate '%s' entry %s:%d\n",
			    f->name, __FILE__, __LINE__);
	}

	return 0;
}

/* returns 0 on success */
static int __populate_hash(struct hash_functions *hashes)
{
	int i;
	int ret = -1;
	struct target_functions *tag;
	struct hash_functions *h = hashes;

	if (!h || !h->tab)
		return ret;

	for (i = 0; i < h->targets_size; i++) {
		tag = &h->targets[i];

		if (populate_hash_target(h->tab, tag))
			return ret;
	}

	return 0;
}

/* Returns non 0 on fatal errors */
static int handle_output(struct hash_functions *hashes, void *plug_data)
{
	int i;
	int ret = -1;
	struct target_functions *tag;
	struct hash_functions *h = hashes;
	struct plugin_data *pdata = (struct plugin_data *)plug_data;
	struct output_buffer *buffer = pdata->buffer;

	if (!h || !h->tab)
		return ret;

	for (i = 0; i < h->targets_size; i++) {
		tag = &h->targets[i];
	}

	return 0;
}

/* Returns 0 if there is a match */
int match_function_name(void *data, void *plug_data)
{
	int i;
	int ret = -1;
	const char *fnname = (char *)data;

	/* If we have not set this then no need to looukup
	 * for matches */
	if (!TARGETS_ARRAY_SIZE(errors, warnings, reports))
		return 0;

	for (i = 0; i < ARRAY_SIZE(ghash); i++) {
		if (!ghash[i].targets_size || !ghash[i].tab)
			continue;

		if (__lookup_key(ghash[i].tab, fnname))
			ret = 0;
	}

	return ret;
}

int match_output(__attribute__((unused)) void *data,
		 void *plug_data)
{
	int i;
	int ret = -1;
	struct plugin_data *pdata = (struct plugin_data *)plug_data;
	struct output_buffer *buffer = pdata->buffer;
	char *offset = output_buf(buffer);

	if (!TARGETS_ARRAY_SIZE(errors, warnings, reports)) {
		write(pdata->fd, output_buf(buffer),
		      output_strlen(buffer));
		return 0;
	}

	for (i = 0; i < ARRAY_SIZE(ghash); i++) {
		if (!ghash[i].targets_size || !ghash[i].tab)
			continue;

		if (handle_output(&ghash[i], plug_data)) {
			out_warning("GCC plugins: failed to handle output %s:%d\n",
				    __FILE__, __LINE__);
			return ret;
		}
	}

	write(pdata->fd, output_buf(buffer), output_strlen(buffer));

	return 0;
}

/* Returns 0 on success */
htab_t init_hash(struct hash_functions *hashes)
{
	struct hash_functions *h = hashes;

	return __init_hash(h->targets, h->targets_size);
}

/* Caller should make sur that ->tab is valid */
void fini_hash(struct hash_functions *hashes)
{
	htab_delete(hashes->tab);
}

int fncalls_match_init(void)
{
	int i;
	int ret = -1;

	for (i = 0; i < ARRAY_SIZE(ghash); i++) {
		if (!ghash[i].targets_size)
			continue;

		ghash[i].tab = init_hash(&ghash[i]);
		if (!ghash[i].tab)
			goto error;

		if (__populate_hash(&ghash[i]))
			goto error;
	}

	return 0;

error:
	out_error("GCC plugins: failed to create hashtable %s:%d\n",
		  __FILE__, __LINE__);
	return ret;
}

void fncalls_match_finish(void)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(ghash); i++) {
		if (!ghash[i].tab)
			continue;

		fini_hash(&ghash[i]);
	}
}

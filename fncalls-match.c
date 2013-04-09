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

#include "gcc-plugin.h"
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tree.h"
#include "intl.h"
#include "plugin.h"
#include "diagnostic.h"
#include "hashtab.h"

#include "ahaggar.h"
#include "buffer.h"
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

static struct target_functions errors[] = {
#include "file-errors.h"
#include "kfile-errors.h"
};

static struct target_functions warnings[] = {
};

static struct target_functions reports[] = {
#include "kmalloc-reports.h"
};

static struct hash_functions __hash[] = {
	{
		.tab = NULL,
		.targets = errors,
		.targets_size = ARRAY_SIZE(errors),
	},
	{
		.tab = NULL,
		.targets = warnings,
		.targets_size = ARRAY_SIZE(warnings),
	},
	{
		.tab = NULL,
		.targets = reports,
		.targets_size = ARRAY_SIZE(reports),
	},
};

static void fncalls_error(const char *msg)
{
	error(G_("%s"), msg);
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

static struct target_functions *__lookup_key(htab_t hashtable,
					     struct target_functions *fn)
{
	PTR *slot = NULL;
	htab_t h = hashtable;
	struct target_functions *f = fn;

	if (!f->name || !f->name[0])
		return NULL;

	slot = htab_find(h, f->name);
	if (!slot)
		return NULL;

	return *slot;
}

static struct target_functions *__insert_key(htab_t hashtable,
					     struct target_functions *fn)
{
	PTR *slot = NULL;
	htab_t h = hashtable;
	struct target_functions *f = fn;

	if (!f->name || !f->name[0])
		return NULL;

	slot = htab_find_slot(h, f->name, INSERT);
	if (!slot)
		return NULL;

	*slot = f;

	return *slot;
}

/* returns 0 on success */
static int __populate_hash(struct hash_functions *hashes)
{
	int i;
	int ret = -1;
	struct hash_functions *h = hashes;

	if (!h || !h->tab)
		return ret;

	for (i = 0; i < h->targets_size; i++) {
		struct target_functions *tag = &h->targets[i];
		if (!__lookup_key(h->tab, tag)) {
			if (!__insert_key(h->tab, tag))
				return ret;
		} else {
			fprintf(stderr,
				"GCC plugins: warning: duplicate '%s' %s:%d\n",
				tag->name,
				__FILE__, __LINE__);
		}
	}

	return 0;
}

/* returns 0 if there is a match */
static int __match_function_name(htab_t hashtable, const char *name)
{
	int ret = -1;
	PTR *slot = NULL;
	htab_t h = hashtable;

	if (!name)
		return ret;

	slot = htab_find(h, name);
	if (!slot)
		return ret;

	return 0;
}

/* Returns 0 if there is a match */
int match_function_name(void *data, void *plug_data)
{
	int i;
	int ret = 0;
	char *fnname = (char *)data;

	/* If we have set this then we must hit a match */
	if (TARGETS_ARRAY_SIZE(errors, warnings, reports))
		ret = 1;

	for (i = 0; i < ARRAY_SIZE(__hash); i++) {
		if (!__hash[i].targets_size || !__hash[i].tab)
			continue;

		if (!__match_function_name(__hash[i].tab,
					   fnname))
			return 0;
	}

	return ret;
}

int match_handle_output(void *plug_data)
{
	struct plugin_data *pdata = (struct plugin_data *)plug_data;
	struct output_buffer *buffer = pdata->buffer;
	char *offset = output_buf(buffer);

	if (!TARGETS_ARRAY_SIZE(errors, warnings, reports)) {
		write(pdata->fd, output_buf(buffer),
		      output_strlen(buffer));
		return 0;
	}

	/* save function declaration */
	if (*offset != ' ' && *offset != '\n') {
	}

	write(pdata->fd, output_buf(buffer), output_strlen(buffer));

	return 0;
}

int fncalls_match_init(void)
{
	int i;
	int ret = -1;

	for (i = 0; i < ARRAY_SIZE(__hash); i++) {
		if (!__hash[i].targets_size)
			continue;

		__hash[i].tab = __init_hash(__hash[i].targets,
					    __hash[i].targets_size);
		if (!__hash[i].tab)
			goto error;

		if (__populate_hash(&__hash[i]))
			goto error;
	}

	return 0;

error:
	fprintf(stderr,
		"GCC plugins: failed to create hash table %s:%d\n"
		__FILE__, __LINE__);
	return ret;
}

void fncalls_match_finish(void)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(__hash); i++) {
		if (!__hash[i].tab)
			continue;

		htab_delete(__hash[i].tab);
	}
}

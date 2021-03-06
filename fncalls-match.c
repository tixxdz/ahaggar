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
#include <regex.h>

#include "gcc-plugin.h"
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tree.h"
#include "tree-pass.h"
#include "intl.h"
#include "input.h"
#include "plugin.h"
#include "diagnostic.h"
#include "options.h"
#include "hashtab.h"

#include "ahaggar.h"
#include "buffer.h"
#include "str-utils.h"
#include "log.h"
#include "gcc-log.h"
#include "fncalls_checks/checks.h"
#include "fncalls-match.h"

#define SAFE_LOAD_FACTOR 25
#define MAX_HASH_ENTRIES 1024

#define TMPBUF_SIZE 2048
#define TMPMSG_SIZE 512

#define TARGETS_ARRAY_SIZE(err, warn, report) \
	(ARRAY_SIZE(err) + ARRAY_SIZE(warn) + ARRAY_SIZE(report))

static int __string_eq(const void *, const void *);
static hashval_t __string_hash(const void *);
static inline int is_hash_safe(struct target_functions *,
			       unsigned int);

extern const char *progname;
extern location_t input_location;

static char *tmp_buffer = NULL;
static char *tmp_message = NULL;

static struct target_functions errors[] = {
//#include "fncalls_checks/api-warnings.h"
//#include "fncalls_checks/file-errors.h"
//#include "fncalls_checks/builtins-warnings.h"
};

static struct target_functions warnings[] = {
//#include "fncalls_checks/file-warnings.h"
//#include "fncalls_checks/api-warnings.h"
//#include "fncalls_checks/kmalloc-warnings.h"
//#include "fncalls_checks/kformat-warnings.h"
//#include "fncalls_checks/malloc-warnings.h"
//#include "fncalls_checks/mem-warnings.h"
//#include "fncalls_checks/file-warnings.h"
#include "fncalls_checks/builtins-warnings.h"
};

static struct target_functions reports[] = {
//#include "fncalls_checks/kformat-warnings.h"
//#include "fncalls_checks/file-warnings.h"
//#include "fncalls_checks/file-reports.h"
//#include "fncalls_checks/kmalloc-reports.h"
//#include "fncalls_checks/const-reports.h"
//#include "fncalls_checks/kmemory-reports.h"
};

static struct hash_functions ghash[] = {
	{
		.id = ERRORS_ID,
		.tab = NULL,
		.targets = errors,
		.targets_size = ARRAY_SIZE(errors),
		.out_f = __error,
	},
	{
		.id = WARNS_ID,
		.tab = NULL,
		.targets = warnings,
		.targets_size = ARRAY_SIZE(warnings),
		.out_f = __warning,
	},
	{
		.id = REPORTS_ID,
		.tab = NULL,
		.targets = reports,
		.targets_size = ARRAY_SIZE(reports),
		.out_f = report,
	},
};

static ssize_t output_buffer_write(void *plug_data)
{

	struct plugin_data *pdata = (struct plugin_data *)plug_data;
	output_buf *buffer = pdata->buffer;

	return write(pdata->fd, output_buf(buffer), output_strlen(buffer));
}

static ssize_t substring_write(void *plug_data, struct substring *sub)
{
	struct plugin_data *pdata = (struct plugin_data *)plug_data;

	return write(pdata->fd, sub_start(sub), sub_len(sub));
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

static regex_t *init_regexp(const char *pattern)
{
	int ret;
	regex_t *expr = (regex_t *)xmalloc(sizeof(regex_t));

	memset(expr, 0, sizeof(regex_t));

	ret=regcomp(expr, pattern, REG_EXTENDED);
	if (ret) {
		regerror(ret, expr, tmp_buffer, TMPBUF_SIZE);
		out_error("GCC plugin: failed to compile regexp: %s",
			  tmp_buffer);
		free(expr);
		return NULL;
	}

	return expr;
}

static void fini_regexp(regex_t *expr)
{
	if (expr) {
		regfree(expr);
		free(expr);
		expr = NULL;
	}
}

static int compile_regexp_match(struct target_functions *fn)
{
	int ret = -1;
	struct target_functions *f = fn;
	struct pattern_match *pm;

	for (pm = f->patterns; pm && pm->active; pm++) {
		if (pm->args) {
			pm->c_args = init_regexp(pm->args);
			if (!pm->c_args)
				return ret;
		}

		if (pm->c_all) {
			pm->c_all = init_regexp(pm->all);
			if (!pm->c_all)
				return ret;
		}
	}

	return 0;
}

static void free_regexp_match(struct target_functions *fn)
{
	struct target_functions *f = fn;
	struct pattern_match *pm;

	for (pm = f->patterns; pm && pm->active; pm++) {
		fini_regexp(pm->c_args);
		fini_regexp(pm->c_all);
	}
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
		out_warning("hashtable duplicate '%s' entry %s:%d",
			    f->name, __FILE__, __LINE__);
	}

	return 0;
}

/* returns 0 on success */
static int populate_hash(struct hash_functions *hashes)
{
	unsigned int i;
	int ret = -1;
	struct target_functions *tag;
	struct hash_functions *h = hashes;

	if (!h || !h->tab)
		return ret;

	for (i = 0; i < h->targets_size; i++) {
		tag = &h->targets[i];

		if (populate_hash_target(h->tab, tag))
			return ret;

		if (compile_regexp_match(tag))
			return ret;
	}

	return 0;
}

/* Dump the "<function_decl> function(...)\n" if any */
static int dump_decl_output(void *plug_data)
{
	int ret = -1;
	char *ch;
	struct plugin_data *pdata = (struct plugin_data *)plug_data;
	output_buf *buffer = pdata->buffer;
	char *offset = output_buf(buffer) + 1;

	if (!strstarts(offset, FUNCTION_DECL_CODE))
		return ret;

	ch = strchr(offset, '\n');
	if (ch) {
		ch++;
		ret = write(pdata->fd, output_buf(buffer),
			    ch - output_buf(buffer));
	}

	return ret;
}

/* Save the next function call into the substring
 * On success returns its lenght */
static size_t next_fncall_to_substring(struct substring *sub,
				       output_buf *out)
{
	size_t ret = 0;
	char *offset = NULL;
	struct substring *substr = sub;
	output_buf *buffer = out;

	if (!sub_start(substr)) {
		offset = output_buf(buffer) + 1;
		/* are we in function_decl */
		if (strstarts(offset, FUNCTION_DECL_CODE)) {
			substring_move_to_strchr(substr,
						 offset, '\n');
			substring_addchr_start(substr);
			offset = sub_start(substr);
		} else {
			offset = output_buf(buffer);
		}
	} else {
		offset = sub_end(substr);
	}

	/* At the end */
	if (!offset || !*offset)
		return ret;

	if (!substring_to_strchr(substr, offset, ']'))
		return ret;

	/* make it end at the next call */
	substring_addnchr_end(substr, 3);

	/* At the end */
	return sub_len(substr);
}

static char *extract_fnname(struct substring *sub, int flags)
{
	char *str = NULL;
	char *ch1 = NULL;
	char *ch2 = NULL;

	ch2 = strchr(sub_start(sub), '(');
	if (ch2) {
		ch1 = ch2;
		while (*ch1 && *ch1 !=  ' ')
			ch1--;

		if (*++ch1)
			str = xstrndup(ch1, ch2 - ch1);
	}

	return str;
}

static char *extract_location(struct substring *sub, location_t loc,
			      char *buffer, size_t buflen)
{
	char *start;
	char *end;
	size_t len;
	char *buf = buffer;
	struct substring *substr = sub;

	buf[0] = '\0';

	start = strrchr(sub_start(substr), '[');
	if (!start)
		goto arg_location;

	start++;
	end = strchr(start, ']');
	if (!end)
		goto arg_location;

	end++;
	len = (size_t)(end - start);
	if (len > buflen)
		len = buflen;

	memcpy(buf, start, len);
	buf[len - 1] = '\0';

	return buf;

arg_location:
	if (loc)
		snprintf(buf, buflen, "%s", LOCATION_FILE(loc));
	return buf;
}

static struct target_functions *match_fncall(htab_t hashtable,
					     struct substring *sub,
					     void *plug_data)
{
	char *fnname;
	struct target_functions *fn = NULL;
	struct substring *substr = sub;
	struct plugin_data *pdata = (struct plugin_data *)plug_data;
	output_buf *buffer = pdata->buffer;

	if (!next_fncall_to_substring(substr, buffer))
		return fn;

	fnname = extract_fnname(substr, pdata->flags);
	if (!fnname)
		return fn;

	fn = __lookup_key(hashtable, fnname);

	/* Include the next newline if any */
	if (*sub_end(substr) == '\n')
		substring_addchr_end(substr);

	free(fnname);
	return fn;
}

static int output_fncall_results(out_report_t out_f,
				 struct pattern_match *pattern,
				 struct substring *sub,
				 void *plug_data,
				 int match)
{
	char *location;
	int ret = -1;
	char *msg = NULL;
	struct pattern_match *pm = pattern;
	struct plugin_data *pdata = (struct plugin_data *)plug_data;

	if (match == FNCALL_NOMATCH)
		msg = pm->msg_nomatch;
	else if (match == FNCALL_MATCH) {
		if (pm->msg_extra && *pm->msg_extra)
			msg = pm->msg_extra;
		else
			msg = pm->msg;
	}

	if (!msg || !*msg)
		return ret;

	location = extract_location(sub, input_location,
				    tmp_buffer, TMPBUF_SIZE);
	out_f(pdata->fd, *location ? location : "", "%s", msg);

	return 0;
}

/* Returns 0 if there is a match */
static int regexp_match_call(struct pattern_match *pattern,
			     struct substring *sub)
{
	char *start;
	char *end;
	size_t len;
	int ret = REG_NOMATCH;
	struct pattern_match *pm = pattern;
	struct substring *substr = sub;

	if (!pm->match_all_func && !pm->c_all)
		return ret;

	start = sub_start(substr);
	end = strchr(start, ']');
	if (!end)
		return ret;

	end++;
	len = (size_t)(end - start);
	if (len > TMPBUF_SIZE)
		len = TMPBUF_SIZE;

	memcpy(tmp_buffer, start, len);
	tmp_buffer[len - 1] = '\0';

	if (pm->match_all_func) {
		pm->msg_extra = tmp_message;
		ret = pm->match_all_func(tmp_buffer,
					 pm->flags,
					 pm->msg_extra,
					 TMPMSG_SIZE);
	}

	if (pm->c_all && ret)
		ret = regexec(pm->c_all, tmp_buffer, 0, NULL, 0);

	return ret;
}

/* Returns 0 if there is a match */
static int regexp_match_cargs(struct pattern_match *pattern,
			      struct substring *sub)
{
	char *start;
	char *end;
	size_t len;
	int ret = REG_NOMATCH;
	struct pattern_match *pm = pattern;
	struct substring *substr = sub;

	if (!pm->match_args_func && !pm->c_args)
		return ret;

	start = strchr(sub_start(substr), '(');
	if (!start)
		return ret;

	/* search for the file:line tag '[' */
	end = strchr(start, '[');
	if (!end)
		return ret;

	len = (size_t)(end - start);
	if (len > TMPBUF_SIZE)
		len = TMPBUF_SIZE;

	memcpy(tmp_buffer, start, len);
	tmp_buffer[len - 1] = '\0';

	if (pm->match_args_func) {
		pm->msg_extra = tmp_message;
		ret = pm->match_args_func(tmp_buffer,
					  pm->flags,
					  pm->msg_extra,
					  TMPMSG_SIZE);
	}

	if (pm->c_args && ret)
		ret = regexec(pm->c_args, tmp_buffer, 0, NULL, 0);

	return ret;
}

/* Returns non 0 on fatal errors */
static int process_fncall(struct hash_functions *hashes,
			  struct target_functions *fn,
			  struct substring *sub, void *plug_data)
{
	int ret = -1;
	int msg_needed = 1;
	int print_decl = 0;
	struct pattern_match *pm;
	struct target_functions *f = fn;
	struct hash_functions *h = hashes;
	struct substring *substr = sub;

	if (!h || !f || !f->patterns)
		return ret;

	if (h->id == REPORTS_ID) {
		msg_needed = 0;
		print_decl = 1;
	}

	for (pm = f->patterns; pm && pm->active; pm++) {
		int match;
		int type_of_call = FNCALL_MATCH;
		if (!pm->msg && !pm->msg_nomatch &&
		(!pm->match_args_func && !pm->match_all_func)) {
			if (msg_needed)
				continue;
			type_of_call = FNCALL_RAW;
		}

		memset(tmp_buffer, 0, TMPBUF_SIZE);
		memset(tmp_message, 0, TMPMSG_SIZE);

		match = (pm->force_match
			|| !regexp_match_cargs(pm, substr)
			|| !regexp_match_call(pm, substr))
			? FNCALL_MATCH : FNCALL_NOMATCH;

		if (type_of_call != FNCALL_RAW) {
			output_fncall_results(h->out_f, pm, substr,
					      plug_data, match);
		} else if (match == FNCALL_MATCH) {
			if (print_decl)
				dump_decl_output(plug_data);

			print_decl = 0;
			substring_write(plug_data, substr);
		}

	}

	return 0;
}

/* Returns non 0 on fatal errors */
static int process_output(struct hash_functions *hashes,
			  void *plug_data)
{
	int ret = -1;
	int print_decl = 0;
	int patterns_needed = 1;
	struct substring *substr;
	struct target_functions *tag;
	struct hash_functions *h = hashes;

	if (!h || !h->tab)
		return ret;

	if (h->id == REPORTS_ID) {
		print_decl = 1;
		patterns_needed = 0;
	}

	substr = substring_init();

	for (; h->mcounter; h->mcounter--) {
		tag = match_fncall(h->tab, substr, plug_data);
		if (!tag)
			continue;

		if (tag->patterns) {
			if (process_fncall(h, tag, substr, plug_data))
				return ret;
		} else if (!patterns_needed) {
			if (print_decl)
				dump_decl_output(plug_data);

			print_decl = 0;
			substring_write(plug_data, substr);
		}
	}

	substring_fini(substr);

	return 0;
}

/* Returns 0 if there is a match */
int match_function_name(void *data,
			__attribute__((unused)) void *plug_data)
{
	unsigned int i;
	int ret = -1;
	const char *fnname = (char *)data;

	/* If we have not set this then no need to looukup
	 * for matches */
	if (!TARGETS_ARRAY_SIZE(errors, warnings, reports))
		return 0;

	for (i = 0; i < ARRAY_SIZE(ghash); i++) {
		if (!ghash[i].targets_size || !ghash[i].tab)
			continue;

		if (__lookup_key(ghash[i].tab, fnname)) {
			ghash[i].mcounter++;
			ret = 0;
		}
	}

	return ret;
}

int match_output(__attribute__((unused)) void *data,
		 void *plug_data)
{
	unsigned int i;
	int ret = -1;

	if (!TARGETS_ARRAY_SIZE(errors, warnings, reports)) {
		output_buffer_write(plug_data);
		return 0;
	}

	for (i = 0; i < ARRAY_SIZE(ghash); i++) {
		if (!ghash[i].targets_size
		|| !ghash[i].tab || !ghash[i].mcounter)
			continue;

		if (process_output(&ghash[i], plug_data)) {
			out_warning("GCC plugin: failed to handle output %s:%d",
				    __FILE__, __LINE__);
			return ret;
		}
		/* the ghash[i].mcounter should be zeroed now */
	}

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
	unsigned int i;
	int ret = -1;

	tmp_buffer = (char *)xmalloc(TMPBUF_SIZE);
	tmp_message = (char *)xmalloc(TMPMSG_SIZE);

	memset(tmp_buffer, 0, TMPBUF_SIZE);

	for (i = 0; i < ARRAY_SIZE(ghash); i++) {
		if (!ghash[i].targets_size)
			continue;

		ghash[i].tab = init_hash(&ghash[i]);
		if (!ghash[i].tab)
			goto error;

		if (populate_hash(&ghash[i]))
			goto error;
	}

	return 0;

error:
	out_error("GCC plugin: failed to create hashtable %s:%d",
		  __FILE__, __LINE__);
	return ret;
}

void fncalls_match_finish(void)
{
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(ghash); i++) {
		unsigned int x;
		struct hash_functions *h = &ghash[i];

		if (h->tab)
			fini_hash(h);

		for (x = 0; x < h->targets_size; x++)
			free_regexp_match(&h->targets[x]);
	}

	free(tmp_message);
	free(tmp_buffer);
}

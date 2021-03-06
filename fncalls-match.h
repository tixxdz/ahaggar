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

#ifndef _FNCALLS_MATCH_H
#define _FNCALLS_MATCH_H

#include <regex.h>

enum __hash_ids {
	ERRORS_ID,
	WARNS_ID,
	REPORTS_ID,
};

enum __match_v {
	FNCALL_MATCH = 0,
	FNCALL_NOMATCH,
	FNCALL_RAW,
};

typedef int (*match_func_t)(char *call, int flags,
			    char *buf, size_t buflen);

struct pattern_match {
	char *args;
	char *all;

	regex_t *c_args; /* internal compiled args */
	regex_t *c_all;  /* internal compiled all */

	/* Force match */
	int force_match;

	/* Flags to be passed to the callback matchers */
	int flags;

	/* Callback matchers */
	match_func_t match_args_func;
	match_func_t match_all_func;

	char *msg;
	char *msg_nomatch;

	/* This will point to a dynamically allocated buffer */
	char *msg_extra;

	void *__data_1;

	const int active;
};

struct target_functions {
	const char *name;
	struct pattern_match *patterns;
};

typedef void (*out_report_t)(int, const char *, const char *, ...);

struct hash_functions {
	int id; /* hash ID */

	htab_t tab;
	struct target_functions *targets;

	/* number of elements of the target_functions array.
	 * Currently it must be known at compile time */
	unsigned int targets_size;

	/* count matches: this is used to optimize code
	 * and avoid unnecessary loops */
	unsigned int mcounter;

	/* output report */
	out_report_t out_f;
};

htab_t init_hash(struct hash_functions *hashes);
void fini_hash(struct hash_functions *hashes);

int fncalls_match_init(void);
void fncalls_match_finish(void);

int match_function_name(void *data,
			__attribute__((unused)) void *plug_data);

/* We just receive the buffer stored in the plug_data */
int match_output(__attribute__((unused)) void *data,
		 void *plug_data);

#endif /* _FNCALLS_MATCH_H */

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

enum __hash_ids {
	errors_hash,
	warns_hash,
	reports_hash,
};

struct pattern_match {
	char *args;
	char *all;
	char *msg;
};

struct target_functions {
	const char *name;
	struct pattern_match *patterns;
};

struct hash_functions {
	htab_t tab;
	struct target_functions *targets;

	/* number of elements of the target_functions array.
	 * Currently it must be know at compile time */
	unsigned int targets_size;

	/* output report */
	void (*out_f)(int, const char *, const char *, ...);
};

htab_t init_hash(struct hash_functions *hashes);
void fini_hash(struct hash_functions *hashes);

int fncalls_match_init(void);
void fncalls_match_finish(void);

int match_function_name(void *data, void *plug_data);

/* We just receive the buffer stored in the plug_data */
int match_output(__attribute__((unused)) void *data,
		 void *plug_data);

#endif /* _FNCALLS_MATCH_H */

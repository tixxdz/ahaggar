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

#ifndef _AHAGGAR_H
#define _AHAGGAR_H

#define AHAGGAR_HEADER "#generated by the Ahaggar framework"

#define INDENT 2

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

#define FUNCTION_DECL_CODE "<function_decl>"
#define GIMPLE_CALL_CODE "<gimple_call>"

#include "buffer.h"

/* Misc stuff for plugins */
struct plugin_data {

	int *indent_level;
	int to_files;
	int only_main;
	int flags;	/* Dump flags */
	int fd;

	output_buf *buffer;

	struct pmap_nodes *pm_nodes; /* uniq nodes on a map */
	struct sp_tree_nodes *sp_nodes; /* nodes on splay tree */

	/* The plugin tree walker */
	walk_tree_fn tree_walker;

	/* The plugin checker return 0 on success */
	int (*checker)(void *, void *);

	/* return 0 on success */
	int (*output_handler)(void *, void *);

	void *extra;

	char plugin_name[PATH_MAX];
};

static inline void *ahg_null_walker(__attribute__((unused))void *t,
				    __attribute__((unused))int *x,
				    __attribute__((unused))void *d)
{
	return NULL;
}

#endif /* _AHAGGAR_H */

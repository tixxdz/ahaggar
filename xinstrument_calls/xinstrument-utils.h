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

#ifndef _XINSTRUMENT_UTILS_H
#define _XINSTRUMENT_UTILS_H

typedef void (*gxinsert_call_t)(gimple_stmt_iterator *,
			        gimple, enum gsi_iterator_update);

typedef gimple (*gxbuild_call_t)(gimple, void *);


struct insert_builtin {
	const char *name;

	gxbuild_call_t gxbuild;

	int active;
};

struct target_call {
	const char *name;

	enum gimple_code code;
	enum gsi_iterator_update mode;

	gxinsert_call_t gxinsert;

	struct insert_builtin *builtins;
};

gimple gxinstrument_build_call(tree fn, unsigned int nr_args, ...);

void gxinsert_call(gxinsert_call_t gxinsert, gimple stmt,
		   gimple call, enum gsi_iterator_update mode);

int gxinstrument_target(struct target_call *tcall,
			struct insert_builtin *ibuiltin,
			gimple gs, void *data);

#endif /* end of _XINSTRUMENT_UTILS_H */

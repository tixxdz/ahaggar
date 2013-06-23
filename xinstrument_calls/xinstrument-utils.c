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
#include <stdarg.h>

#include "gcc-plugin.h"
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tree.h"
#include "tree-iterator.h"
#include "intl.h"
#include "diagnostic.h"
#include "gimple.h"

#include "xinstrument-utils.h"

gimple gxinstrument_build_call(tree fn, unsigned int nr_args, ...)
{
	va_list ap;
	gimple call = NULL;

	if (!fn || TREE_CODE(fn) != FUNCTION_DECL)
		return call;

	va_start(ap, nr_args);
	call = gimple_build_call_valist(fn, nr_args, ap);
	va_end(ap);

	return call;
}

void gxinsert_call(gxinsert_call_t gxinsert, gimple stmt,
		   gimple call, enum gsi_iterator_update mode)
{
	gimple_stmt_iterator gsi;

	gimple_set_location(call, gimple_location(stmt));
	gimple_set_block(call, gimple_block(stmt));

	gsi = gsi_for_stmt(stmt);

	gxinsert(&gsi, call, mode);
}

int gxinstrument_target(struct target_call *tcall,
			struct insert_builtin *ibuiltin,
			gimple gs, void *data)
{
	gimple call;
	gxinsert_call_t gxinsert;
	int ret = -1;
	struct target_call *tc = tcall;
	struct insert_builtin *ib = ibuiltin;

	if (!tc || !ib)
		return ret;

	gxinsert = tc->gxinsert;

	call = ib->gxbuild(gs, data);
	if (!call)
		return ret;

	gxinsert_call(gxinsert, gs, call, tc->mode); 

	return 0;
}

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

#include <string.h>

#include "gcc-plugin.h"
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tree.h"
#include "cp/cp-tree.h"
#include "tree-pass.h"
#include "tree-flow.h"
#include "tm.h"
#include "intl.h"
#include "plugin.h"

#include "gcc-utils.h"

bool is_file_main_input(void)
{
/*
	struct tinst_level *tl = outermost_tinst_level();

	if (tl)
		return !!(strcmp(main_input_filename,
				 LOCATION_FILE(tl->locus)));
*/
	return strcmp(main_input_filename, input_filename) == 0;
}

bool is_node_in_main_input(tree node)
{
	if (node_has_location(node))
		return is_file_main_input();

	return false;
}

bool is_function_decl(tree fn)
{
	if (!fn || TREE_CODE(fn) != FUNCTION_DECL)
		return false;

	return true;
}

/* Return the location of a tree passed to %+ formats.  */
/* based on gcc/cp/error.c */
location_t location_of(tree t)
{
	if (TREE_CODE(t) == PARM_DECL && DECL_CONTEXT(t))
		t = DECL_CONTEXT(t);
	else if (TYPE_P(t)) {
		t = TYPE_MAIN_DECL(t);
		if (t == NULL_TREE)
			return input_location;
	} else if (TREE_CODE(t) == OVERLOAD)
		t = OVL_FUNCTION(t);

	if (DECL_P(t))
		return DECL_SOURCE_LOCATION(t);

	return EXPR_LOC_OR_HERE(t);
}

/* Taken from Mozilla Dehydra plugins */
bool location_is_unknown(location_t loc)
{
	location_t unk = UNKNOWN_LOCATION;
	return !memcmp(&loc, &unk, sizeof(location_t));
}

/* return true if tree has a location */
bool node_has_location(tree node)
{
	location_t loc = location_of(node);
	return !location_is_unknown(loc);
}

const char *op_symbol(const_tree op)
{
	return op_symbol_code(TREE_CODE(op));
}

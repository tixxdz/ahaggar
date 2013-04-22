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
#define __STDC_FORMAT_MACROS
#include <inttypes.h>

#include "gcc-plugin.h"
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tree.h"
#include "cp/cp-tree.h"
#include "tree-pass.h"
#include "langhooks.h"
#include "tm.h"
#include "intl.h"
#include "plugin.h"

#include "buffer.h"
#include "gcc-utils.h"
#include "nodes-utils.h"
#include "print-nodes.h"

void output_node_addr(struct output_buffer *d, tree node, int flags)
{
	if (flags & TDF_STMTADDR)
		output_printf(d, "<@%p> ", (void *)node);
}

void output_node_index(struct output_buffer *d, unsigned int index)
{
	output_printf(d, "@%u ", index);
}

void __output_expr_code(struct output_buffer *d, tree expr)
{
	output_printf(d, "%s", __get_expr_code(expr));
}

void output_expr_code(struct output_buffer *d, tree node)
{
	output_char(d, '<');
	__output_expr_code(d, node);
	output_printf(d, "> ");
}

void output_location(struct output_buffer *d, tree t)
{
	output_printf(d, "%s", __get_location(t));
}

void output_decl_name(struct output_buffer *d, tree decl, int flags)
{
	output_printf(d, "%s", __get_decl_name(decl, flags));
}

/* copied from GCC: tree-pretty-print.c */
void output_function_name(struct output_buffer *d, tree fn, int flags)
{
	output_printf(d, "%s", __get_function_name(fn, flags));
}

void output_int_cst(struct output_buffer *d, tree int_cst)
{
	output_printf(d, "%s", __get_int_cst(int_cst));
}

void output_string_cst(struct output_buffer *d, tree string_cst)
{
	output_char(d, '\"');
	output_printf(d, "%s", __get_string_cst(string_cst));
	output_char(d, '\"');
}

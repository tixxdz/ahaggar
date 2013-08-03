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
 * Note: most of this code was adapated from
 *	 GCC  http://gcc.gnu.org/
 *
 */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "gcc-plugin.h"
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tree.h"
#include "tree-pass.h"
#include "tree-flow.h"
#include "gimple.h"

#include "ahaggar.h"
#include "buffer.h"
#include "gcc-utils.h"
#include "nodes-utils.h"
#include "print-nodes.h"
#include "walk-nodes.h"
#include "walk-gimple.h"

int walk_gimple_unary_rhs(gimple gs, void *data)
{
	int ret = -1;
	tree lhs;
	tree rhs;
	enum tree_code rhs_code;
	walk_tree_fn tree_walker = ((struct plugin_data *)data)->tree_walker;
	struct output_buffer *buffer = ((struct plugin_data *)data)->buffer;

	if (!gs)
		return ret;

	rhs_code = gimple_assign_rhs_code(gs);
	lhs = gimple_assign_lhs(gs);
	rhs = gimple_assign_rhs1(gs);

	switch (rhs_code) {
	case VIEW_CONVERT_EXPR:
	case ASSERT_EXPR:
		base_cp_tree_walker(&rhs, tree_walker, data);
		break;

	case FIXED_CONVERT_EXPR:
	case ADDR_SPACE_CONVERT_EXPR:
	case FIX_TRUNC_EXPR:
	case FLOAT_EXPR:
	CASE_CONVERT:
		output_char(buffer, '(');
		base_cp_tree_walker(&(TREE_TYPE(lhs)),
				    tree_walker, data);
		output_printf(buffer, ") ");

		if (op_prio(rhs) < op_code_prio(rhs_code)) {
			output_char(buffer, '(');
			base_cp_tree_walker(&rhs,
					    tree_walker, data);
			output_printf(buffer, ") ");
		} else {
			base_cp_tree_walker(&rhs,
					    tree_walker, data);
		}
		break;

	case PAREN_EXPR:
		output_printf(buffer, "((");
		base_cp_tree_walker(&rhs, tree_walker, data);
		output_printf(buffer, "))");
		break;

	case ABS_EXPR:
		output_printf(buffer, "ABS_EXPR(");
		base_cp_tree_walker(&rhs, tree_walker, data);
		output_char(buffer, ')');
		break;

	default:
		if (TREE_CODE_CLASS(rhs_code) == tcc_declaration
		|| TREE_CODE_CLASS (rhs_code) == tcc_constant
		|| TREE_CODE_CLASS (rhs_code) == tcc_reference
		|| rhs_code == SSA_NAME || rhs_code == ADDR_EXPR
		|| rhs_code == CONSTRUCTOR) {
			base_cp_tree_walker(&rhs,
					    tree_walker, data);
			break;
		} else if (rhs_code == BIT_NOT_EXPR) {
			output_char(buffer, '~');
		} else if (rhs_code == TRUTH_NOT_EXPR) {
			output_char(buffer, '!');
		} else if (rhs_code == NEGATE_EXPR) {
			output_char(buffer, '-');
		}

		if (op_prio(rhs) < op_code_prio(rhs_code)) {
			output_char(buffer, '(');
			base_cp_tree_walker(&rhs,
					    tree_walker, data);
			output_char(buffer, ')');
		} else {
			base_cp_tree_walker(&rhs,
					    tree_walker, data);
		}
		break;
	}

	return 0;
}

int walk_gimple_binary_rhs(gimple gs, void *data)
{
	int ret = -1;
	tree rhs1;
	tree rhs2;
	enum tree_code rhs_code;
	walk_tree_fn tree_walker = ((struct plugin_data *)data)->tree_walker;
	struct output_buffer *buffer = ((struct plugin_data *)data)->buffer;

	if (!gs)
		return ret;

	rhs_code = gimple_assign_rhs_code(gs);
	rhs1 = gimple_assign_rhs1(gs);
	rhs2 = gimple_assign_rhs2(gs);

	switch (rhs_code) {
	case COMPLEX_EXPR:
	case MIN_EXPR:
	case MAX_EXPR:
	case VEC_WIDEN_MULT_HI_EXPR:
	case VEC_WIDEN_MULT_LO_EXPR:
	case VEC_PACK_TRUNC_EXPR:
	case VEC_PACK_SAT_EXPR:
	case VEC_PACK_FIX_TRUNC_EXPR:
	case VEC_WIDEN_LSHIFT_HI_EXPR:
	case VEC_WIDEN_LSHIFT_LO_EXPR:
		output_printf(buffer, " <");
		base_cp_tree_walker(&rhs1, tree_walker, data);
		output_printf(buffer, ", ");
		base_cp_tree_walker(&rhs2, tree_walker, data);
		output_char(buffer, '>');
		break;

	default:
		if (op_prio(rhs1) <= op_code_prio(rhs_code)) {
			output_char(buffer, '(');
			base_cp_tree_walker(&rhs1,
					    tree_walker, data);
			output_char(buffer, ')');
		} else {
			base_cp_tree_walker(&rhs1,
					    tree_walker, data);
		}

		output_printf(buffer, " %s ",
			      op_symbol_code(rhs_code));
		if (op_prio(rhs2) <= op_code_prio(rhs_code)) {
			output_char(buffer, '(');
			base_cp_tree_walker(&rhs2,
					    tree_walker, data);
			output_char(buffer, ')');
		} else {
			base_cp_tree_walker(&rhs2,
					    tree_walker, data);
		}
		break;

	}

	return 0;
}

int walk_gimple_ternary_rhs(gimple gs, void *data)
{
	int ret = -1;
	tree rhs1;
	tree rhs2;
	tree rhs3;
	enum tree_code rhs_code;
	walk_tree_fn tree_walker = ((struct plugin_data *)data)->tree_walker;
	struct output_buffer *buffer = ((struct plugin_data *)data)->buffer;

	if (!gs)
		return ret;

	rhs_code = gimple_assign_rhs_code(gs);
	rhs1 = gimple_assign_rhs1(gs);
	rhs2 = gimple_assign_rhs2(gs);
	rhs3 = gimple_assign_rhs3(gs);

	switch (rhs_code) {
	case WIDEN_MULT_PLUS_EXPR:
	case WIDEN_MULT_MINUS_EXPR:
		output_printf(buffer, " <");
		base_cp_tree_walker(&rhs1, tree_walker, data);
		output_printf(buffer, ", ");
		base_cp_tree_walker(&rhs2, tree_walker, data);
		output_printf(buffer, ", ");
		base_cp_tree_walker(&rhs3, tree_walker, data);
		output_char(buffer, '>');
		break;

	case FMA_EXPR:
		base_cp_tree_walker(&rhs1, tree_walker, data);
		output_printf(buffer, " * ");
		base_cp_tree_walker(&rhs2, tree_walker, data);
		output_printf(buffer, " + ");
		base_cp_tree_walker(&rhs3, tree_walker, data);
		break;

	case DOT_PROD_EXPR:
	case VEC_PERM_EXPR:
	case REALIGN_LOAD_EXPR:
	case VEC_COND_EXPR:
		/* TODO: complete */
		break;

	case COND_EXPR:
		base_cp_tree_walker(&rhs1, tree_walker, data);
		output_printf(buffer, " ? ");
		base_cp_tree_walker(&rhs2, tree_walker, data);
		output_printf(buffer, " : ");
		base_cp_tree_walker(&rhs3, tree_walker, data);
		break;

	default:
		break;
	}

	return 0;
}

int walk_gimple_nop(gimple gs, tree var, void *data)
{
	int ret = -1;
	tree decl;
	walk_tree_fn tree_walker = ((struct plugin_data *)data)->tree_walker;

	if (!gs || !var)
		return ret;

	decl = SSA_NAME_VAR(var);
	base_cp_tree_walker(&decl, tree_walker, data);

	return 0;
}

int walk_gimple_phi(gimple phi, void *data)
{
	tree lhs;
	int ret = -1;
	unsigned int i;
	walk_tree_fn tree_walker = ((struct plugin_data *)data)->tree_walker;
	struct output_buffer *buffer = ((struct plugin_data *)data)->buffer;

	if (!phi)
		return ret;

	lhs = gimple_phi_result(phi);
	base_cp_tree_walker(&lhs, tree_walker, data);
	output_printf(buffer, " = <%s> (",
		      gimple_code_name[gimple_code(phi)]);

	for (i = 0; i < gimple_phi_num_args(phi); i++) {
		tree rhs = gimple_phi_arg_def(phi, i);
		/*edge ed = gimple_phi_arg_edge(phi, i);*/
		base_cp_tree_walker(&rhs, tree_walker, data);

		/*output_printf(buffer, "(%d)", ed->src->index);*/
		if (i < gimple_phi_num_args(phi) - 1)
			output_printf(buffer, ", ");
	}
	output_char(buffer, ')');

	return 0;
}

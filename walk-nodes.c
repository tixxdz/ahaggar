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
#include "tree-iterator.h"

#include "ahaggar.h"
#include "buffer.h"
#include "gcc-utils.h"
#include "nodes-utils.h"
#include "print-nodes.h"
#include "walk-nodes.h"

int walk_statement_iterator(tree node, void *data)
{
	int ret = -1;
	tree_stmt_iterator i;
	walk_tree_fn tree_walker = ((struct plugin_data *)data)->tree_walker;

	if (!node)
		return ret;

	for (i = tsi_start(node); !tsi_end_p(i); tsi_next(&i)) {
		tree si = tsi_stmt(i);
		base_cp_tree_walker(&si, tree_walker, data);
	}

	return 0;
}

int walk_tree_vector_node(tree node, void *data)
{
	int ret = -1;
	struct plugin_data *pdata = (struct plugin_data *)data;
	walk_tree_fn tree_walker = pdata->tree_walker;
	struct output_buffer *buffer = pdata->buffer;

	if (!node)
		return ret;

	ret = 0;
	if (TREE_VEC_LENGTH(node) > 0) {
		size_t i;
		size_t len = TREE_VEC_LENGTH(node);
		for (i = 0; i < len - 1; i++) {
			base_cp_tree_walker(&(TREE_VEC_ELT(node, i)),
					    tree_walker, data);
			output_printf(buffer, ", ");
		}

		base_cp_tree_walker(&(TREE_VEC_ELT(node, len - 1)),
				    tree_walker, data);
	}

	return ret;
}

int walk_function_decl_args(tree fn, void *data)
{
	int ret = -1;
	tree arg;
	tree op0 = fn;
	walk_tree_fn tree_walker = ((struct plugin_data *)data)->tree_walker;
	struct output_buffer *buffer = ((struct plugin_data *)data)->buffer;

	if (!op0)
		return ret;

	for (arg = DECL_ARGUMENTS(op0); arg; arg = DECL_CHAIN(arg)) {
		tree argtype = TREE_TYPE(arg);
		base_cp_tree_walker(&argtype, tree_walker, data);
		output_space(buffer);
		base_cp_tree_walker(&arg, tree_walker, data);
		if (DECL_CHAIN(arg))
			output_printf(buffer, ", ");
	}

	return 0;
}

int walk_array_domain(tree node, void *data)
{
	int ret = 0;
	tree min;
	tree max;
	struct plugin_data *pdata = (struct plugin_data *)data;
	walk_tree_fn tree_walker = pdata->tree_walker;
	struct output_buffer *buffer = pdata->buffer;

	output_char(buffer, '[');

	if (!node) {
		output_printf(buffer, "<unknown>]");
		return ret;
	}

	min = TYPE_MIN_VALUE(node);
	max = TYPE_MAX_VALUE(node);

	if (min && max && integer_zerop(min)
	&& host_integerp(max, 0)) {
		output_printf(buffer, "%Ld",
			      (HOST_WIDE_INT)(TREE_INT_CST_LOW(max) + 1));
	} else {
		if (min)
			base_cp_tree_walker(&min, tree_walker, data);
		output_char(buffer, ':');
		if (max)
			base_cp_tree_walker(&max, tree_walker, data);
	}

	output_char(buffer, ']');

	return ret;
}

int walk_array_node(tree node, void *data)
{
	tree tmp;
	int ret = -1;
	struct plugin_data *pdata = (struct plugin_data *)data;
	walk_tree_fn tree_walker = pdata->tree_walker;
	struct output_buffer *buffer = pdata->buffer;

	if (!node)
		return ret;

	for (tmp = TREE_TYPE(node); TREE_CODE(tmp) == ARRAY_TYPE;
	tmp = TREE_TYPE(tmp));

	base_cp_tree_walker(&tmp, tree_walker, data);

	for (tmp = node; TREE_CODE(tmp) == ARRAY_TYPE;
	tmp = TREE_TYPE(tmp))
		walk_array_domain(TYPE_DOMAIN(tmp), data);

	return 0;
}

int walk_declaration_node(tree node, void *data)
{
	int ret = -1;
	struct plugin_data *pdata = (struct plugin_data *)data;
	walk_tree_fn tree_walker = pdata->tree_walker;
	struct output_buffer *buffer = pdata->buffer;

	if (!node)
		return ret;

	output_printf(buffer, __get_type_declaration(node));

	if (TREE_TYPE(node)
	&& TREE_CODE(TREE_TYPE(node)) == ARRAY_TYPE) {
		tree tmp = TREE_TYPE(node);
		while (TREE_CODE(TREE_TYPE(tmp)) == ARRAY_TYPE)
			 tmp = TREE_TYPE(tmp);

		base_cp_tree_walker(&(TREE_TYPE(tmp)),
				    tree_walker, data);

		output_space(buffer);
		base_cp_tree_walker(&node, tree_walker, data);

		for (tmp = TREE_TYPE(node);
		TREE_CODE(tmp) == ARRAY_TYPE; tmp = TREE_TYPE(tmp)) {
			walk_array_domain(TYPE_DOMAIN(tmp), data);
		}
	} else if (TREE_CODE(node) == FUNCTION_DECL) {
		base_cp_tree_walker(&(TREE_TYPE(TREE_TYPE(node))),
				    tree_walker, data);
		output_space(buffer);
		output_decl_name(buffer, node, pdata->flags);
		walk_function_decl_args(TREE_TYPE(node), data);
	} else {
		base_cp_tree_walker(&(TREE_TYPE(node)),
				    tree_walker, data);
		output_space(buffer);
		base_cp_tree_walker(&node, tree_walker, data);
	}

	if (TREE_CODE(node) == VAR_DECL && DECL_HARD_REGISTER(node)) {
		tree tasm = DECL_ASSEMBLER_NAME(node);
		output_printf(buffer, " __asm__(");
		base_cp_tree_walker(&tasm, tree_walker, data);
		output_char(buffer, ')');
	}

	if (TREE_CODE(node) != FUNCTION_DECL) {
		if (DECL_INITIAL(node)) {
			output_printf(buffer, " = ");
			base_cp_tree_walker(&(DECL_INITIAL(node)),
					    tree_walker, data);
		}
	}

	if (TREE_CODE(node) == VAR_DECL
	&& DECL_HAS_VALUE_EXPR_P(node)) {
		tree expr = DECL_VALUE_EXPR(node);
		output_printf(buffer, "<value: ");
		base_cp_tree_walker(&expr, tree_walker, data);
		output_char(buffer, '>');
	}

	return 0;
}

int walk_type_declaration_node(tree node, void *data)
{
	int ret = -1;
	struct plugin_data *pdata = (struct plugin_data *)data;
	walk_tree_fn tree_walker = pdata->tree_walker;
	struct output_buffer *buffer = pdata->buffer;

	if (!node)
		return ret;

	ret = 0;
	if (DECL_IS_BUILTIN(node))
		return ret;

	if (DECL_NAME(node))
		output_decl_name(buffer, node, pdata->flags);

	if (TYPE_NAME(TREE_TYPE(node)) != node) {
		output_printf(buffer, "%s ",
			      __get_class_or_enum(node));
		base_cp_tree_walker(&(TREE_TYPE(node)),
				    tree_walker, data);
	}

	return ret;
}

int walk_label_declaration_node(tree node, void *data)
{
	int ret = -1;
	struct plugin_data *pdata = (struct plugin_data *)data;
	walk_tree_fn tree_walker = pdata->tree_walker;
	struct output_buffer *buffer = pdata->buffer;

	if (!node)
		return ret;

	ret = 0;
	if (DECL_NAME(node)) {
		output_decl_name(buffer, node, pdata->flags);
	} else if (LABEL_DECL_UID(node) != -1) {
		output_printf(buffer, "<L%d>",
			      (int)LABEL_DECL_UID(node));
	} else {
		if (pdata->flags & TDF_NOUID)
			output_printf(buffer, "<D.xxxx>");
		else
			output_printf(buffer,
				      "<D.%u>", DECL_UID(node));
	}

	return ret;
}

int walk_number_types_node(tree node, void *data)
{
	int ret = -1;
	enum tree_code_class tclass;
	struct plugin_data *pdata = (struct plugin_data *)data;
	walk_tree_fn tree_walker = pdata->tree_walker;
	struct output_buffer *buffer = pdata->buffer;

	if (!node)
		return ret;

	output_printf(buffer, "%s", __get_type_quals(node));

	tclass = TREE_CODE_CLASS(TREE_CODE(node));
	if (tclass == tcc_declaration) {
		if (DECL_NAME(node))
			output_printf(buffer, "%s ",
				      __get_decl_name(node, pdata->flags));
		else
			output_printf(buffer, "<unnamed>");
	} else if (tclass == tcc_type) {
		output_printf(buffer, "%s ",
			      __get_tcc_type(node, pdata->flags));

		if (TREE_CODE(node) == VECTOR_TYPE
		|| TREE_CODE(node) == COMPLEX_TYPE)
			base_cp_tree_walker(&(TREE_TYPE(node)),
					    tree_walker, data);
	}

	return 0;
}

int walk_pointer_reference_node(tree node, void *data)
{
	int ret = -1;
	tree op0 = node;
	const char *str;
	struct plugin_data *pdata = (struct plugin_data *)data;
	walk_tree_fn tree_walker = pdata->tree_walker;
	struct output_buffer *buffer = pdata->buffer;

	if (!op0)
		return ret;

	if (TREE_CODE(op0) == POINTER_TYPE)
		str = "*";
	else if (TREE_CODE(op0) == REFERENCE_TYPE)
		str = "&";
	else
		return -1;

	if (TREE_TYPE(op0) == NULL) {
		output_printf(buffer, str);
		output_printf(buffer, " <null type>");
	} else if (TREE_CODE(TREE_TYPE(op0)) == FUNCTION_TYPE) {
		tree tnode = TREE_TYPE(op0);
		tree tfn = TREE_TYPE(tnode);

		base_cp_tree_walker(&tfn, tree_walker, data);

		output_char(buffer, '(');
		output_printf(buffer, str);
		if (TYPE_NAME(op0) && DECL_NAME(TYPE_NAME(op0)))
			output_decl_name(buffer, TYPE_NAME(op0),
					 pdata->flags);
		else
			output_printf(buffer, "<T%x>", TYPE_UID(op0));

		output_char(buffer, ')');
		output_function_name(buffer, tnode, pdata->flags);
	} else {
		tree tnode = TREE_TYPE(op0);
		base_cp_tree_walker(&tnode, tree_walker, data);

		output_printf(buffer, str);
		output_printf(buffer, "%s",
			      __get_type_quals(op0));
	}

	return 0;
}

int walk_record_union_node(tree node, void *data)
{
	int ret = -1;
	struct plugin_data *pdata = (struct plugin_data *)data;
	walk_tree_fn tree_walker = pdata->tree_walker;
	struct output_buffer *buffer = pdata->buffer;

	if (!node)
		return ret;

	output_printf(buffer, "%s%s ",
		      __get_type_quals(node),
		      __get_class_or_enum(node));

	if (TYPE_NAME(node))
		base_cp_tree_walker(&(TYPE_NAME(node)),
				    tree_walker, data);

	return 0;
}

int walk_modify_init_expr_node(tree node, void *data)
{
	int ret = -1;
	struct plugin_data *pdata = (struct plugin_data *)data;
	walk_tree_fn tree_walker = pdata->tree_walker;
	struct output_buffer *buffer = pdata->buffer;

	if (!node)
		return ret;

	base_cp_tree_walker(&(TREE_OPERAND(node, 0)),
			    tree_walker, data);
	output_printf(buffer, " = ");
	base_cp_tree_walker(&(TREE_OPERAND(node, 1)),
			    tree_walker, data);

	return 0;
}

int walk_target_expr_node(tree node, void *data)
{
	int ret = -1;
	struct plugin_data *pdata = (struct plugin_data *)data;
	struct output_buffer *buffer = pdata->buffer;

	if (!node)
		return ret;

	ret = 0;
	base_cp_tree_walker(&(TARGET_EXPR_SLOT(node)),
			    tree_walker, data);
	output_printf(buffer, ", ");
	base_cp_tree_walker(&(TARGET_EXPR_INITIAL(node)),
			    tree_walker, data);

	return ret;
}

int walk_cond_expr_node(tree node, void *data)
{
	int indent;
	int indent_level;
	int ret = -1;
	struct plugin_data *pdata = (struct plugin_data *)data;
	walk_tree_fn tree_walker = pdata->tree_walker;
	struct output_buffer *buffer = pdata->buffer;

	if (!node)
		return ret;

	indent_level = *pdata->indent_level;
	indent = (indent_level + 1) * INDENT;

	if (TREE_TYPE(node) == NULL
	|| TREE_TYPE(node) == void_type_node) {
		output_indent_to_newline(buffer, indent);
		output_printf(buffer, "if (");
		base_cp_tree_walker(&(COND_EXPR_COND(node)),
				    tree_walker, data);
		output_char(buffer, ')');
		if (COND_EXPR_THEN(node)
		&& (IS_EMPTY_STMT(COND_EXPR_THEN(node))
			|| TREE_CODE(COND_EXPR_THEN(node)) == GOTO_EXPR)
		&& COND_EXPR_ELSE(node)
		&& (IS_EMPTY_STMT(COND_EXPR_ELSE(node))
			|| TREE_CODE(COND_EXPR_ELSE(node)) == GOTO_EXPR)) {
			++*pdata->indent_level;
			base_cp_tree_walker(&(COND_EXPR_THEN(node)),
					    tree_walker, data);
			--*pdata->indent_level;
			if (!IS_EMPTY_STMT(COND_EXPR_ELSE(node))) {
				output_indent_to_newline(buffer, indent);
				output_printf(buffer, "else ");
				++*pdata->indent_level;
				base_cp_tree_walker(&(COND_EXPR_ELSE(node)),
						    tree_walker, data);
				--*pdata->indent_level;
			}
		} else if (!(pdata->flags & TDF_SLIM)) {
			if (COND_EXPR_THEN(node)) {
				output_indent_to_newline(buffer, indent);
				output_char(buffer, '{');
				++*pdata->indent_level;
				base_cp_tree_walker(&(COND_EXPR_THEN(node)),
						    tree_walker, data);
				--*pdata->indent_level;
				output_indent_to_newline(buffer, indent);
				output_char(buffer, '}');
			}

			if (COND_EXPR_ELSE(node)
			&& !IS_EMPTY_STMT(COND_EXPR_ELSE (node))) {
				output_indent_to_newline(buffer, indent);
				output_printf(buffer, "else {");
				++*pdata->indent_level;
				base_cp_tree_walker(&(COND_EXPR_ELSE(node)),
						    tree_walker, data);
				--*pdata->indent_level;
				output_indent_to_newline(buffer, indent);
				output_char(buffer, '}');
			}
		}
	} else {
		output_char(buffer, '(');
		base_cp_tree_walker(&(TREE_OPERAND(node, 0)),
				    tree_walker, data);
		output_printf(buffer, ") ? ");
		base_cp_tree_walker(&(TREE_OPERAND(node, 1)),
				    tree_walker, data);
		output_printf(buffer, " : ");
		base_cp_tree_walker(&(TREE_OPERAND(node, 2)),
				    tree_walker, data);
	}

	*pdata->indent_level = indent_level;

	return 0;
}

int walk_switch_expr_node(tree node, void *data)
{
	int indent;
	int ret = -1;
	struct plugin_data *pdata = (struct plugin_data *)data;
	walk_tree_fn tree_walker = pdata->tree_walker;
	struct output_buffer *buffer = pdata->buffer;

	if (!node)
		return ret;

	indent = (*pdata->indent_level + 1) * INDENT;

	output_indent_to_newline(buffer, indent);
	output_printf(buffer, "switch (");
	base_cp_tree_walker(&(SWITCH_COND(node)),
			    tree_walker, data);
	output_char(buffer, ')');

	if (pdata->flags & TDF_SLIM)
		return 0;

	output_printf(buffer, " {");

	if (SWITCH_BODY(node)) {
		base_cp_tree_walker(&(SWITCH_BODY(node)),
				    tree_walker, data);
	} else {
		size_t i;
		tree vec = SWITCH_LABELS(node);
		size_t n = TREE_VEC_LENGTH(vec);

		for (i = 0; i < n; ++i) {
			tree elt = TREE_VEC_ELT(vec, i);
			if (elt) {
				base_cp_tree_walker(&(SWITCH_BODY(node)),
						    tree_walker, data);
				output_printf(buffer, " goto ");
				base_cp_tree_walker(&(CASE_LABEL (elt)),
						    tree_walker, data);
			} else {
				output_printf(buffer,
					      "case ???: goto ???;");
			}
		}
	}

	output_indent_to_newline(buffer, indent);
	output_char(buffer, '}');

	return 0;
}

int walk_case_label_expr_node(tree node, void *data)
{
	int ret = -1;
	struct plugin_data *pdata = (struct plugin_data *)data;
	walk_tree_fn tree_walker = pdata->tree_walker;
	struct output_buffer *buffer = pdata->buffer;

	if (!node)
		return ret;

	if (CASE_LOW(node) && CASE_HIGH(node)) {
		output_printf(buffer, "case ");
		base_cp_tree_walker(&(CASE_LOW(node)),
				    tree_walker, data);
		output_printf(buffer, " ... ");
		base_cp_tree_walker(&(CASE_LOW(node)),
				    tree_walker, data);
	} else if (CASE_LOW(node)) {
		output_printf(buffer, "case ");
		base_cp_tree_walker(&(CASE_LOW(node)),
				    tree_walker, data);
	} else {
		output_printf(buffer, "default");
	}

	output_char(buffer, ':');

	return 0;
}

int walk_goto_expr_node(tree node, void *data)
{
	tree op0;
	int ret = -1;
	struct plugin_data *pdata = (struct plugin_data *)data;
	walk_tree_fn tree_walker = pdata->tree_walker;
	struct output_buffer *buffer = pdata->buffer;

	if (!node)
		return ret;

	ret = 0;
	op0 = GOTO_DESTINATION(node);
	if (TREE_CODE(op0) != SSA_NAME) {
	/* && DECL_P(op0) && DECL_NAME(op0)) { */
		const char *name = __get_decl_name(op0, pdata->flags);
		if (strcmp(name, "break") == 0
		|| strcmp (name, "continue") == 0) {
			output_printf(buffer, name);
			return ret;
		}
	}

	output_printf(buffer, "goto ");
	base_cp_tree_walker(&op0, tree_walker, data);

	return ret;
}

int walk_return_expr_node(tree node, void *data)
{
	tree expr;
	int ret = -1;
	struct plugin_data *pdata = (struct plugin_data *)data;
	walk_tree_fn tree_walker = pdata->tree_walker;
	struct output_buffer *buffer = pdata->buffer;

	if (!node)
		return ret;

	/* output_printf(buffer, "return "); */
	expr = TREE_OPERAND(node, 0);

	/* output_node_addr(buffer, expr, pdata->flags); */

	if (expr) {
		if (TREE_CODE(expr) == MODIFY_EXPR
		|| TREE_CODE(expr) == INIT_EXPR) {
			expr = TREE_OPERAND(expr, 1);
			base_cp_tree_walker(&expr,
					    tree_walker, data);
		} else {
			base_cp_tree_walker(&expr,
					    tree_walker, data);
		}
		/* output_expr_code(buffer, expr, pdata->flags); */
	} else {
		output_printf(buffer, "<void>");
	}

	return 0;
}

/* Note: the symbol is hidden, better call:
 * op_symbol_code() before this function */
int walk_unary_logic_expr_node(tree node, void *data)
{
	tree op0;
	int ret = -1;
	/* enum tree_code code; */
	struct plugin_data *pdata = (struct plugin_data *)data;
	walk_tree_fn tree_walker = pdata->tree_walker;
	struct output_buffer *buffer = pdata->buffer;

	if (!node)
		return ret;

	/* code = TREE_CODE(node); */
	op0 = TREE_OPERAND(node, 0);

	/* if (code == ADDR_EXPR
	&& (TREE_CODE(op0) == STRING_CST
	|| TREE_CODE (op0) == FUNCTION_DECL))
		;
	else
		output_printf(buffer, "%s", op_symbol_code(code));*/

	if (op_prio(op0) < op_prio(node)) {
		output_char(buffer, '(');
		base_cp_tree_walker(&op0, tree_walker, data);
		output_char(buffer, ')');
	} else {
		base_cp_tree_walker(&op0, tree_walker, data);
	}

	return 0;
}

/* Note: the symbol is hidden, better call:
 * op_symbol_code() before this function */
int walk_binary_arith_logic_node(tree node, void *data)
{
	tree op0;
	tree op1;
	int ret = -1;
	/* enum tree_code code; */
	struct plugin_data *pdata = (struct plugin_data *)data;
	walk_tree_fn tree_walker = pdata->tree_walker;
	struct output_buffer *buffer = pdata->buffer;

	if (!node)
		return ret;

	/*
	 * code = TREE_CODE(node);
	 * output_printf(buffer, "%s", op_symbol_code(code));
	 */

	op0 = TREE_OPERAND(node, 0);
	op1 = TREE_OPERAND(node, 1);

	if (op_prio(op0) <= op_prio(node)) {
		output_char(buffer, '(');
		base_cp_tree_walker(&op0, tree_walker, data);
		output_char(buffer, ')');
	} else {
		base_cp_tree_walker(&op0, tree_walker, data);
	}

	output_printf(buffer, " %s ", op_symbol(node));

	if (op_prio(op1) <= op_prio(node)) {
		output_char(buffer, '(');
		base_cp_tree_walker(&op1, tree_walker, data);
		output_char(buffer, ')');
	} else {
		base_cp_tree_walker(&op1, tree_walker, data);
	}

	return 0;
}

int walk_component_ref_node(tree node, void *data)
{
	tree op0;
	int ret = -1;
	char *ptr;
	struct plugin_data *pdata = (struct plugin_data *)data;
	walk_tree_fn tree_walker = pdata->tree_walker;
	struct output_buffer *buffer = pdata->buffer;

	if (!node)
		return ret;

	op0 = TREE_OPERAND(node, 0);
	ptr = ".";

	if (op0 && (TREE_CODE(op0) == INDIRECT_REF
	|| (TREE_CODE(op0) == MEM_REF
	&& TREE_CODE(TREE_OPERAND(op0, 0)) != ADDR_EXPR
	&& integer_zerop(TREE_OPERAND(op0, 1))
	&& TREE_CODE(TREE_OPERAND(op0, 0)) != INTEGER_CST
	&& TREE_TYPE(TREE_OPERAND(op0, 0)) != NULL_TREE
	&& (TREE_TYPE(TREE_TYPE(TREE_OPERAND(op0, 0)))
		== TREE_TYPE(TREE_TYPE(TREE_OPERAND(op0, 1))))
	&& (TYPE_MODE(TREE_TYPE(TREE_OPERAND(op0, 0)))
		== TYPE_MODE(TREE_TYPE(TREE_OPERAND(op0, 1))))
	&& (TYPE_REF_CAN_ALIAS_ALL(TREE_TYPE(TREE_OPERAND(op0, 0)))
		== TYPE_REF_CAN_ALIAS_ALL(TREE_TYPE(TREE_OPERAND(op0, 1))))
	&& (TYPE_MAIN_VARIANT(TREE_TYPE(op0))
		== TYPE_MAIN_VARIANT
			(TREE_TYPE(TREE_TYPE(TREE_OPERAND(op0, 1)))))))) {
		op0 = TREE_OPERAND(op0, 0);
		ptr = "->";
	}

	if (op_prio(op0) < op_prio(node)) {
		output_char(buffer, '(');
		base_cp_tree_walker(&op0, tree_walker, data);
		output_char(buffer, ')');
	} else {
		base_cp_tree_walker(&op0, tree_walker, data);
	}

	output_printf(buffer, ptr);
	base_cp_tree_walker(&(TREE_OPERAND(node, 1)),
			    tree_walker, data);

	return 0;
}

int walk_array_ref_node(tree node, void *data)
{
	tree op0;
	tree op1;
	int ret = -1;
	struct plugin_data *pdata = (struct plugin_data *)data;
	walk_tree_fn tree_walker = pdata->tree_walker;
	struct output_buffer *buffer = pdata->buffer;

	if (!node)
		return ret;

	ret = 0;
	op0 = TREE_OPERAND(node, 0);
	if (op_prio(op0) < op_prio(node)) {
		output_char(buffer, '(');
		base_cp_tree_walker(&op0, tree_walker, data);
		output_char(buffer, ')');
	} else {
		base_cp_tree_walker(&op0, tree_walker, data);
	}

	op1 = TREE_OPERAND(node, 1);
	output_char(buffer, '[');
	base_cp_tree_walker(&op1, tree_walker, data);

	if (TREE_CODE(node) == ARRAY_RANGE_REF)
		output_printf(buffer, " ...");

	output_char(buffer, ']');

	op0 = array_ref_low_bound(node);
	op1 = array_ref_element_size(node);

	if (!integer_zerop(op0)
	|| TREE_OPERAND(node, 2) || TREE_OPERAND(node, 3)) {
		output_printf(buffer, "{lb: ");
		base_cp_tree_walker(&op0, tree_walker, data);
		output_printf(buffer, " sz: ");
		base_cp_tree_walker(&op1, tree_walker, data);
		output_char(buffer, '}');
	}

	return ret;
}

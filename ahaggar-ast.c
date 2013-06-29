/*
 * The Ahaggar framework
 *
 * ahaggar-ast: GCC plugin to dump the AST
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
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "gcc-plugin.h"
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tree.h"
#include "cp/cp-tree.h"
#include "tree-pass.h"
#include "tm.h"
#include "tree-iterator.h"
#include "intl.h"
#include "plugin.h"
#include "diagnostic.h"
#include "plugin-version.h"

#include "ahaggar.h"
#include "buffer.h"
#include "log.h"
#include "gcc-utils.h"
#include "nodes-utils.h"
#include "cache-nodes.h"
#include "print-nodes.h"
#include "walk-nodes.h"

int plugin_is_GPL_compatible;

static unsigned int walker_depth;

static void ahaggar_ast_main(void *, void *);
static int ahg_ast_checker(void *, void *);
static int ahg_ast_output_dump(void *, void *);
static tree ahg_ast_tree_walker(tree *, int *, void *);

static struct plugin_data *ast;

static struct plugin_info ahaggar_ast_info = {
	.version = "20120620",
	.help = "Dump the AST.\n"
		" -fplugin-arg-ahaggar_ast-output=files\t"
		"output to files\n"
		" -fplugin-arg-ahaggar_ast-unit=all|main\n",
};

/* return 1 on success */
static inline bool is_function_body_ok(tree body)
{
	return !(!body || TREE_CODE(body) != BIND_EXPR);
}

static int ahg_ast_decl_node(tree node, void *data)
{
	int ret = -1;
	tree op0 = node;
	struct plugin_data *ast = (struct plugin_data *)data;
	struct output_buffer *buffer = ast->buffer;

	if (!op0)
		return ret;

	if (is_function_decl(op0)) {
		unsigned int index = pmap_nodes_insert(ast->pm_nodes,
						       op0);
		if (!index)
			return ret;

		output_newline(buffer);
		output_node_addr(buffer, op0, ast->flags);
		/* output_node_index(buffer, index); */
	}

	output_expr_code(buffer, op0, ast->flags);
	output_decl_name(buffer, op0, ast->flags);

	return 0;
}

static void ahg_ast_walk_call_args(tree node, void *data)
{
	tree arg;
	tree op0 = node;
	call_expr_arg_iterator iter;
	struct plugin_data *ast = (struct plugin_data *)data;
	walk_tree_fn tree_walker = ast->tree_walker;
	struct output_buffer *buffer = ast->buffer;

	if (!op0)
		return;

	FOR_EACH_CALL_EXPR_ARG(arg, iter, op0) {
		cp_walk_tree_without_duplicates(&arg,
						tree_walker, data);
		if (more_call_expr_args_p(&iter))
			output_printf(buffer, ", ");
	}

	if (CALL_EXPR_VA_ARG_PACK(op0)) {
		if (call_expr_nargs(op0) > 0)
			output_printf(buffer, ", ");
		output_printf(buffer, "__builtin_va_arg_pack()");
	}
}

/* Adapted from GCC: tree-pretty-print.c */
static int ahg_ast_call_aggr_init(tree node, void *data)
{
	int ret = -1;
	tree op0 = node;
	struct plugin_data *ast = (struct plugin_data *)data;
	walk_tree_fn tree_walker = ast->tree_walker;
	struct output_buffer *buffer = ast->buffer;

	if (!op0)
		return ret;

	if (TREE_CODE(op0) != CALL_EXPR && TREE_CODE(op0) != AGGR_INIT_EXPR)
		return ret;

	op0 = TREE_CODE(op0) == CALL_EXPR ? CALL_EXPR_FN(op0) :
					  AGGR_INIT_EXPR_FN(op0);

	if (TREE_CODE(op0) == NON_LVALUE_EXPR)
		op0 = TREE_OPERAND(op0, 0);

again:
	switch (TREE_CODE(op0)) {
	case VAR_DECL:
	case PARM_DECL:
	case FUNCTION_DECL: {
		unsigned int index;
		if (!is_function_decl(op0))
			goto out;
		index = pmap_nodes_insert(ast->pm_nodes, op0);
		if (!index)
			goto out;
		/*output_node_index(buffer, index);*/
		output_node_addr(buffer, op0, ast->flags);
		output_expr_code(buffer, node, ast->flags);
		output_function_name(buffer, op0, ast->flags);
		break;
	}

	case ADDR_EXPR:
	case INDIRECT_REF:
	case NOP_EXPR:
		op0 = TREE_OPERAND(op0, 0);
		goto again;

	case COND_EXPR:
		output_char(buffer, '(');
		cp_walk_tree_without_duplicates(&(TREE_OPERAND(op0, 0)),
						tree_walker, data);
		output_printf(buffer, ") ? ");
		cp_walk_tree_without_duplicates(&(TREE_OPERAND(op0, 1)),
						tree_walker, data);
		output_printf(buffer, " : ");
		cp_walk_tree_without_duplicates(&(TREE_OPERAND(op0, 2)),
						tree_walker, data);
		break;

	case ARRAY_REF:
		if (TREE_CODE(TREE_OPERAND(op0, 0)) == VAR_DECL)
			goto again;
		else
			cp_walk_tree_without_duplicates(&op0,
						tree_walker, data);
		break;

	case MEM_REF:
		if (integer_zerop(TREE_OPERAND(op0, 1))) {
			op0 = TREE_OPERAND(op0, 0);
			goto again;
		}
	case COMPONENT_REF:
	case SSA_NAME:
	case OBJ_TYPE_REF:
		cp_walk_tree_without_duplicates(&op0,
						tree_walker, data);
		break;

	default:
		break;
	}

	output_char(buffer, '(');
	ahg_ast_walk_call_args(node, data);
	output_printf(buffer, ")");

	op0 = CALL_EXPR_STATIC_CHAIN(node);
	if (op0) {
		output_printf(buffer, "[static-chain: ");
		cp_walk_tree_without_duplicates(&op0,
						tree_walker, data);
		output_printf(buffer, "] ");
	}

	if (CALL_EXPR_RETURN_SLOT_OPT(node))
		output_printf(buffer, "[return slot optimization] ");
	if (CALL_EXPR_TAILCALL(node))
		output_printf(buffer, "[tail call] ");

	ret = 0;

out:
	return ret;
}

static void ahg_ast_walk_body(tree body, void *data)
{
	tree body_chain = BIND_EXPR_BODY(body);
	walk_tree_fn tree_walker = ((struct plugin_data *)data)->tree_walker;

	cp_walk_tree_without_duplicates(&body_chain,
					tree_walker, data);
}

static int ahg_ast_analyze_body(tree fndecl, void *data)
{
	int ret = -1;
	tree body;
	struct plugin_data *ast = (struct plugin_data *)data;

	if (!fndecl)
		return ret;

	body = DECL_SAVED_TREE(fndecl);
	if (!is_function_body_ok(body))
		return ret;

	/* setup the internal nodes of the function */
	ast->sp_nodes = sptree_nodes_init();
	sptree_nodes_set_root(ast->sp_nodes, fndecl);

	ahg_ast_walk_body(body, data);

	sptree_nodes_finish(ast->sp_nodes);

	return 0;
}

static int ahg_ast_function(tree node, void *data)
{
	int ret = -1;
	int saved_flags;
	tree fndecl = node;
	struct plugin_data *ast = (struct plugin_data *)data;
	struct output_buffer *buffer = ast->buffer;

	if (errorcount || !is_function_decl(fndecl))
		return ret;

	if (ast->only_main && !is_node_in_main_input(fndecl))
		return ret;

	if (!pmap_nodes_insert(ast->pm_nodes, fndecl))
		return ret;

	if (ahg_ast_decl_node(fndecl, ast))
		return ret;

	saved_flags = ast->flags;
	ast->flags = 0;
	output_char(buffer, '(');
	walk_function_decl_args(fndecl, ast);
	output_printf(buffer, ") ");
	output_location(buffer, fndecl);
	ast->flags = saved_flags;

	return 0;
}

static tree ahg_ast_tree_walker(tree *b, int *walk_subtrees,
				void *data)
{
	tree node = *b;
	enum tree_code code;
	static int print_location = 0;
	static const char *symbol_prefix = "";
	struct plugin_data *ast = (struct plugin_data *)data;
	walk_tree_fn tree_walker = ast->tree_walker;
	struct output_buffer *buffer = ast->buffer;

	if (!node)
		return NULL_TREE;

	code = TREE_CODE(node);
	if (code == ERROR_MARK)
		return NULL_TREE;

	walker_depth++;

	/* output_printf(buffer, " '%s' ", tree_code_name[code]); */
	switch (code) {
	case IDENTIFIER_NODE:
		output_decl_name(buffer, node, ast->flags);
		/* output_space(buffer); */
		*walk_subtrees = 0;
		break;

	/*
	case TREE_LIST:
		break;

	case TREE_VEC:
		break;
	*/

	case VOID_TYPE:
	case INTEGER_TYPE:
	case REAL_TYPE:
	case FIXED_POINT_TYPE:
	case COMPLEX_TYPE:
	case VECTOR_TYPE:
	case ENUMERAL_TYPE:
	case BOOLEAN_TYPE:
		walk_number_types_node(node, ast);
		*walk_subtrees = 0;
		break;

	case STATEMENT_LIST:
		/* output_indent_to_newline(buffer,
					 walker_depth * INDENT);
		 * output_expr_code(buffer, node, ast->flags);
		 */
		walker_depth--;
		walk_statement_iterator(node, data);
		walker_depth++;
		*walk_subtrees = 0;
		break;

	case INTEGER_CST:
		output_node_addr(buffer, node, ast->flags);
		output_expr_code(buffer, node, ast->flags);
		output_int_cst(buffer, node);
		*walk_subtrees = 0;
		break;

	case STRING_CST:
		output_node_addr(buffer, node, ast->flags);
		output_expr_code(buffer, node, ast->flags);
		output_string_cst(buffer, node);
		*walk_subtrees = 0;
		break;

	case DECL_EXPR:
		output_indent_to_newline(buffer,
					 walker_depth * INDENT);
		walk_declaration_node(DECL_EXPR_DECL(node), ast);
		*walk_subtrees = 0;
		break;

	case COND_EXPR:
		output_indent_to_newline(buffer,
					 walker_depth * INDENT);
		output_printf(buffer, "%s(", tree_code_name[code]);
		walk_cond_expr_node(node, ast,
				    (walker_depth + 1) * INDENT);
		output_indent_to_newline(buffer,
					 walker_depth * INDENT);
		output_char(buffer, ')');
		print_location++;
		*walk_subtrees = 0;
		break;

	case VAR_DECL:
	case PARM_DECL:
	case DEBUG_EXPR_DECL:
	case NAMESPACE_DECL:
		output_node_addr(buffer, node, ast->flags);
		output_expr_code(buffer, node, ast->flags);
		output_printf(buffer, "%s", symbol_prefix);
	case FIELD_DECL:
		output_decl_name(buffer, node, ast->flags);
		*walk_subtrees = 0;
		break;

	case COMPONENT_REF: {
		tree op0 = TREE_OPERAND(node, 0);
		output_expr_code(buffer, node, ast->flags);
		output_var_parm_decl_code(buffer, op0, ast->flags);
		walk_component_ref_node(node, ast);
		*walk_subtrees = 0;
		break;
	}

	case ADDR_EXPR: {
		tree op0 = TREE_OPERAND(node, 0);
		if (TREE_CODE(op0) != STRING_CST
		&& TREE_CODE(op0) != FUNCTION_DECL)
			symbol_prefix = op_symbol(node);

		/*
		 * output_expr_code(buffer, node, ast->flags);
		 * output_expr_code(buffer, op0, ast->flags);
		 */
		walk_unary_logic_expr_node(node, ast);
		*walk_subtrees = 0;
		break;
	}

	case NEGATE_EXPR:
	case BIT_NOT_EXPR:
	case TRUTH_NOT_EXPR:
	case PREDECREMENT_EXPR:
	case PREINCREMENT_EXPR:
	case INDIRECT_REF: {
		tree op0 = TREE_OPERAND(node, 0);
		output_indent_to_newline(buffer,
					 walker_depth * INDENT);
		output_printf(buffer, "%s(", tree_code_name[code]);
		symbol_prefix = op_symbol(node);
		/* output_expr_code(buffer, op0, ast->flags); */
		walk_unary_logic_expr_node(node, ast);
		output_char(buffer, ')');
		*walk_subtrees = 0;
		break;
	}

	case RESULT_DECL:
		output_printf(buffer, "<retval>");
		*walk_subtrees = 0;
		break;

	case FUNCTION_DECL:
	case CONST_DECL:
		output_expr_code(buffer, node, ast->flags);
		output_function_name(buffer, node, ast->flags);
		*walk_subtrees = 0;
		break;

	case TYPE_DECL:
		if (DECL_IS_BUILTIN(node))
			break;

		if (DECL_NAME(node))
			output_decl_name(buffer, node, ast->flags);

		if (TYPE_NAME(TREE_TYPE(node)) != node) {
			output_printf(buffer, "%s ",
				      __get_class_or_enum(node));
			base_cp_tree_walker(&(TREE_TYPE(node)),
					    tree_walker, data);
		}
		*walk_subtrees = 0;
		break;

	case MODIFY_EXPR:
	case INIT_EXPR:
		output_indent_to_newline(buffer,
					 walker_depth * INDENT);
		output_printf(buffer, "%s(", tree_code_name[code]);
		walk_modify_init_expr_node(node, data);
		output_char(buffer, ')');
		print_location++;
		*walk_subtrees = 0;
		break;

	/*
	 * case TARGET_EXPR:
	 *	*walk_subtrees = 0;
	 *	break;
	 */

	case AGGR_INIT_EXPR:
	case CALL_EXPR:
		output_indent_to_newline(buffer,
					 walker_depth * INDENT);
		if (!ahg_ast_call_aggr_init(node, ast))
			print_location++;

		*walk_subtrees = 0;
		break;

	case POINTER_TYPE:
	case REFERENCE_TYPE:
		walk_pointer_reference_node(node, ast);
		*walk_subtrees = 0;
		break;

	case RECORD_TYPE:
	case UNION_TYPE:
	case QUAL_UNION_TYPE:
		walk_record_union_node(node, ast);
		*walk_subtrees = 0;
		break;

	/* Binary arithmetic */
	case WIDEN_SUM_EXPR:
	case WIDEN_MULT_EXPR:
	case MULT_EXPR:
	/* case MULT_HIGHPART_EXPR: */
	case PLUS_EXPR:
	case POINTER_PLUS_EXPR:
	case MINUS_EXPR:
	case TRUNC_DIV_EXPR:
	case CEIL_DIV_EXPR:
	case FLOOR_DIV_EXPR:
	case ROUND_DIV_EXPR:
	case TRUNC_MOD_EXPR:
	case CEIL_MOD_EXPR:
	case FLOOR_MOD_EXPR:
	case ROUND_MOD_EXPR:
	case RDIV_EXPR:
	case EXACT_DIV_EXPR:
	case LSHIFT_EXPR:
	case RSHIFT_EXPR:
	case LROTATE_EXPR:
	case RROTATE_EXPR:
	case VEC_LSHIFT_EXPR:
	case VEC_RSHIFT_EXPR:
	case WIDEN_LSHIFT_EXPR:
	case BIT_IOR_EXPR:
	case BIT_XOR_EXPR:
	case BIT_AND_EXPR:
	case TRUTH_ANDIF_EXPR:
	case TRUTH_ORIF_EXPR:
	case TRUTH_AND_EXPR:
	case TRUTH_OR_EXPR:
	case TRUTH_XOR_EXPR:
	case LT_EXPR:
	case LE_EXPR:
	case GT_EXPR:
	case GE_EXPR:
	case EQ_EXPR:
	case NE_EXPR:
	case UNLT_EXPR:
	case UNLE_EXPR:
	case UNGT_EXPR:
	case UNGE_EXPR:
	case UNEQ_EXPR:
	case LTGT_EXPR:
	case ORDERED_EXPR:
		output_indent_to_newline(buffer,
					 walker_depth * INDENT);
		output_printf(buffer, "%s(", tree_code_name[code]);
		walk_binary_arith_logic_node(node, ast);
		output_char(buffer, ')');
		*walk_subtrees = 0;
		break;

	case EXIT_EXPR:
		output_indent_to_newline(buffer,
					 walker_depth * INDENT);
		output_printf(buffer, "%s(", tree_code_name[code]);
		base_cp_tree_walker(&(COND_EXPR_COND(node)),
				    tree_walker, data);
		output_indent_to_newline(buffer,
					 walker_depth * INDENT);
		output_char(buffer, ')');
		print_location++;
		*walk_subtrees = 0;
		break;

	case SWITCH_EXPR:
		output_indent_to_newline(buffer,
					 walker_depth * INDENT);
		output_printf(buffer, "%s(", tree_code_name[code]);
		walk_switch_expr_node(node, ast,
				      (walker_depth + 1) * INDENT);
		output_char(buffer, ')');
		*walk_subtrees = 0;
		break;

	case CASE_LABEL_EXPR:
		output_indent_to_newline(buffer,
					 walker_depth * INDENT);
		walk_case_label_expr_node(node, ast);
		*walk_subtrees = 0;
		break;

	case GOTO_EXPR:
		*walk_subtrees = 0;
		break;

	case RETURN_EXPR:
		output_indent_to_newline(buffer,
					 walker_depth * INDENT);
		if (!walk_return_expr_node(node, ast))
			print_location++;

		*walk_subtrees = 0;
		break;

	default:
		/*
		 * output_printf(buffer,
		 *		 "warning: unhandled tree node: %s",
		 *		 tree_code_name[code]);
		 */
		break;
	}

	if (print_location == 1) {
		if (walker_depth == 1) {
			output_space(buffer);
			output_location(buffer, node);
		}
		print_location--;
	} else if (print_location > 1) {
		print_location--;
	}

	walker_depth--;
	symbol_prefix = "";

	return NULL_TREE;
}

static void ahaggar_ast_main(void *gcc_data, void *user_data)
{
	tree fndecl = (tree)gcc_data;
	struct plugin_data *ast = (struct plugin_data *)user_data;
	struct output_buffer *buffer = ast->buffer;

	output_flush(buffer);

	if (!ahg_ast_function(fndecl, ast)) {
		ahg_ast_analyze_body(fndecl, ast);
		output_newline(buffer);
		ast->output_handler(NULL, ast);
	}
}

static int ahg_ast_checker(void *node, void *plug_data)
{
	return 0;
}

static int ahg_ast_output_dump(void *data, void *plug_data)
{
	struct plugin_data *ast = (struct plugin_data *)plug_data;
	struct output_buffer *buffer = ast->buffer;

	write(ast->fd, output_buf(buffer), output_strlen(buffer));

	return 0;
}

static void start_unit(void *gcc_data, void *user_data)
{
	struct plugin_data *ast = (struct plugin_data *)user_data;
	const char *input = main_input_filename ? main_input_filename : "-";

	ast->pm_nodes = pmap_nodes_init();

	if (ast->to_files) {
		int fd;
		mode_t mode = get_file_perms(main_input_filename);
		char *file = get_logname(main_input_filename,
					 0, "ahaggar_ast.log");
		if (!file) {
			error(G_("GCC plugin '%s' output name failed\n"),
				ast->plugin_name);
			return;
		}
		fd = open_log(file, mode);
		if (fd == -1) {
			error(G_("GCC plugin '%s' open output file '%s' failed"),
				ast->plugin_name, file);
			return;
		}
		ast->fd = fd;
		fprintf(stderr, "GCC plugin '%s' save output into: %s\n",
			ast->plugin_name, file);
		free(file);
	} else {
		ast->fd = STDERR_FILENO;
	}

	write_log(ast->fd, "%s\n", AHAGGAR_HEADER);
	fprintf(stderr, "GCC plugin '%s' processing unit: %s\n",
		ast->plugin_name, input);
}

static void finish_unit(void *gcc_data, void *user_data)
{
	struct plugin_data *ast = (struct plugin_data *)user_data;
	const char *input = main_input_filename ? main_input_filename : "-";

	if (ast->fd != STDERR_FILENO) {
		fsync_log(ast->fd);
		close_log(ast->fd);
	}

	pmap_nodes_finish(ast->pm_nodes);

	fprintf(stderr, "GCC plugin '%s' finished unit: %s\n",
		ast->plugin_name, input);
}

/* returns 0 on success */
static int ast_plugin_process_args(const int argc,
				   const struct plugin_argument *const argv,
				   struct plugin_data *ast)
{
	int i;
	for (i = 0; i < argc; ++i) {
		if (!strcmp(argv[i].key, "output")) {
			if (!strcmp(argv[i].value, "files"))
				ast->to_files = 1;
		} else if (!strcmp(argv[i].key, "unit")) {
			if (!strcmp(argv[i].value, "main"))
				ast->only_main = 1;
		} else {
			error(G_("unkown option '-fplugin-arg-%s-%s'"),
			      ast->plugin_name, argv[i].key);
			return -1;
		}
	}

	return 0;
}

static struct plugin_data *ast_init(struct plugin_name_args *plugin_info)
{
	struct plugin_data *l_ast;

	l_ast = (struct plugin_data *)xmalloc(sizeof(struct plugin_data));
	memset(l_ast, 0, sizeof(struct plugin_data));
	strncpy(l_ast->plugin_name, plugin_info->base_name,
		sizeof(l_ast->plugin_name) - 1);

	l_ast->flags = TDF_TREE | TDF_VERBOSE | TDF_VERBOSE;
	l_ast->buffer = output_init();
	l_ast->tree_walker = (walk_tree_fn)ahg_ast_tree_walker;
	l_ast->checker = ahg_ast_checker;
	l_ast->output_handler = ahg_ast_output_dump;

	return l_ast;
}

static void ast_finish(void *gcc_data, void *user_data)
{
	struct plugin_data *ast = (struct plugin_data *)user_data;

	output_fini(ast->buffer);
	free(ast);
}

int plugin_init(struct plugin_name_args *plugin_info,
		struct plugin_gcc_version *version)
{
	int ret = -1;
	const int argc = plugin_info->argc;
	const struct plugin_argument *const argv = plugin_info->argv;

	if (!plugin_default_version_check(version, &gcc_version)) {
		error(G_("incompatible gcc/plugin versions"));
		return ret;
	}

	ast = ast_init(plugin_info);

	if (ast_plugin_process_args(argc, argv, ast)) {
		error(G_("GCC plugin: args and initialization failed"));
		ast_finish(NULL, (void *)ast);
		return ret;
	}

	register_callback(ast->plugin_name, PLUGIN_INFO, NULL,
			  &ahaggar_ast_info);
	register_callback(ast->plugin_name, PLUGIN_START_UNIT,
			  (plugin_callback_func)&start_unit, ast);
	register_callback(ast->plugin_name, PLUGIN_FINISH_UNIT,
			  (plugin_callback_func)&finish_unit, ast);
	register_callback(ast->plugin_name, PLUGIN_PRE_GENERICIZE,
			  (plugin_callback_func)&ahaggar_ast_main, ast);
	register_callback(ast->plugin_name, PLUGIN_FINISH,
			  (plugin_callback_func)&ast_finish, ast);

	return 0;
}

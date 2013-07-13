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

#ifndef _WALK_NODES_H
#define _WALK_NODES_H

#ifndef walk_tree_without_duplicates
#define walk_tree_without_duplicates(a, b, c) \
	walk_tree_without_duplicates_1(a, b, c, NULL)
#endif

/*#ifndef cp_walk_tree_without_duplicates*/
#undef cp_walk_tree_without_duplicates
#define cp_walk_tree_without_duplicates walk_tree_without_duplicates
/*#endif*/

#ifndef base_cp_tree_walker
#define base_cp_tree_walker cp_walk_tree_without_duplicates
#endif

typedef int (*walk_tree_nodes)(tree node, void *data);

int walk_statement_iterator(tree node, void *data);
int walk_tree_vector_node(tree node, void *data);
int walk_function_decl_args(tree fn, void *data);
int walk_array_domain(tree node, void *data);
int walk_array_node(tree node, void *data);
int walk_declaration_node(tree node, void *data);
int walk_type_declaration_node(tree node, void *data);
int walk_label_declaration_node(tree node, void *data);
int walk_number_types_node(tree node, void *data);
int walk_pointer_reference_node(tree node, void *data);
int walk_record_union_node(tree node, void *data);
int walk_modify_init_expr_node(tree node, void *data);
int walk_target_expr_node(tree node, void *data);
int walk_cond_expr_node(tree node, void *data);
int walk_switch_expr_node(tree node, void *data);
int walk_case_label_expr_node(tree node, void *data);
int walk_label_expr_node(tree node, void *data);
int walk_loop_expr_node(tree node, void *data);
int walk_goto_expr_node(tree node, void *data);
int walk_return_expr_node(tree node, void *data);
int walk_unary_logic_expr_node(tree node, void *data);
int walk_post_inc_dec_expr_node(tree node, void *data);
int walk_binary_arith_logic_node(tree node, void *data);
int walk_component_ref_node(tree node, void *data);
int walk_array_ref_node(tree node, void *data);

#endif /* _WALK_NODES_H */

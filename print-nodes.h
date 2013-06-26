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

#ifndef _PRINT_NODES_H
#define _PRINT_NODES_H

void output_node_addr(struct output_buffer *d, tree node, int flags);
void output_node_index(struct output_buffer *d, unsigned int index);
void __output_expr_code(struct output_buffer *d, tree expr);
void output_expr_code(struct output_buffer *d, tree node, int flags);
void output_var_parm_decl_code(struct output_buffer *d,
			       tree node, int flags);
void output_location(struct output_buffer *d, tree t);
void output_decl_name(struct output_buffer *d, tree decl, int flags);
void output_function_name(struct output_buffer *d, tree fn, int flags);
void output_int_cst(struct output_buffer *d, tree int_cst);
void output_string_cst(struct output_buffer *d, tree string_cst);

#endif /* _PRINT_NODES_H */

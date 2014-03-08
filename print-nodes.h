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

void output_node_addr(output_buf *d, tree node, int flags);
void output_node_index(output_buf *d, unsigned int index);
void __output_expr_code(output_buf *d, tree expr);
void output_expr_code(output_buf *d, tree node, int flags);
void output_var_parm_decl_code(output_buf *d,
			       tree node, int flags);
void output_location(output_buf *d, tree t);
void output_decl_name(output_buf *d, tree decl, int flags);
void output_function_name(output_buf *d, tree fn, int flags);
void output_int_cst(output_buf *d, tree int_cst);
void output_string_cst(output_buf *d, tree string_cst);
void output_op_prio(output_buf *d);

#endif /* _PRINT_NODES_H */

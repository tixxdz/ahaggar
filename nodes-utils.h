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

#ifndef _NODES_UTILS_H
#define _NODES_UTILS_H

const char *__get_class_or_enum(tree t);
char *__get_type_quals(tree t);
char *__get_type_declaration(tree t);
char *__get_tcc_declaration(tree t, int flags);
char *__get_tcc_type(tree t, int flags);
char *__get_node_type(tree t, int flags);
char *__get_aggr(tree t);
char *__get_location(tree t);
char *__get_location_as_string(location_t loc);
char *__get_decl_name(tree decl, int flags);
char *__get_function_name(tree fn, int flags);
char *__get_expr_code(tree expr);
char *__get_int_cst(tree int_cst);
char *__get_string_cst(tree string_cst);
char *__get_extra_decl(tree decl);

#endif /* _NODES_TYPES_H */

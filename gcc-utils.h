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

#ifndef _GCC_UTILS_H
#define _GCC_UTILS_H

bool is_file_main_input(void);
bool is_node_in_main_input(tree node);
bool is_function_decl(tree fn);

/* Return the location of a tree passed to %+ formats.  */
location_t location_of(tree t);
bool location_is_unknown(location_t loc);
bool node_has_location(tree node);

const char *op_symbol(const_tree op);

#endif /* _GCC_UTIL_H */

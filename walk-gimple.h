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

#ifndef _WALK_GIMPLE_H
#define _WALK_GIMPLE_H

int walk_gimple_unary_rhs(gimple gs, void *data);
int walk_gimple_binary_rhs(gimple gs, void *data);
int walk_gimple_ternary_rhs(gimple gs, void *data);
int walk_gimple_nop(gimple gs, tree var, void *data);
int walk_gimple_phi(gimple phi, void *data);

#endif /* _WALK_GIMPLE_H */

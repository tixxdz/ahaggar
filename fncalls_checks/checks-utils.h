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

#ifndef _CHECKS_UTILS_H
#define _CHECKS_UTILS_H

struct fncall_data {
	const char *fnname;
	const char *call;

	int arg_idx;
	int flags;

};

static inline void init_fncall_data(struct fncall_data *fn,
				    const char *fnname, char *call,
				    int arg_idx, int flags)
{
	if (fn) {
		fn->fnname = fnname;
		fn->call = call;
		fn->arg_idx = arg_idx;
		fn->flags = flags;
	}
}

char *get_next_arg(char **call_offset);

char *get_arg_value(const char *arg);

int get_args_nr(const char *call);

char *call_start_at_arg(const char *call, int arg_idx);

#endif /* end of _CHECKS_UTILS_H */

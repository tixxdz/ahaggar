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

#ifndef _GCC_LOG_H
#define _GCC_LOG_H

#include "log.h" /* for report() */

void __error(__attribute__((unused)) int x,
	     const char *str, const char *fmt, ...);
void __warning(__attribute__((unused)) int x,
	       const char *str, const char *fmt, ...);

#define out_error(fmt, args...) __error(0, NULL, fmt, ##args)
#define out_warning(fmt, args...) __warning(0, NULL, fmt, ##args)
#define out_report(x, s, fmt, args...) report(x, s, fmt, ##args)

#endif /* _GCC_LOG_H */

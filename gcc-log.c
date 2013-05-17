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

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <stdarg.h>

#include "gcc-plugin.h"
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tree.h"
#include "intl.h"
#include "plugin.h"
#include "diagnostic.h"

#include "gcc-log.h"

void __error(__attribute__((unused)) int x,
	     const char *str, const char *fmt, ...)
{
	va_list args;
	diagnostic_info diagnostic;

	if (str)
		fprintf(stderr, "%s: error:\n", str);

	va_start(args, fmt);
	diagnostic_set_info(&diagnostic, fmt, &args,
			    UNKNOWN_LOCATION, DK_ERROR);
	diagnostic.option_index = 0;

	report_diagnostic(&diagnostic);
	va_end(args);
}

void __warning(__attribute__((unused)) int x,
	       const char *str, const char *fmt, ...)
{
	va_list args;
	diagnostic_info diagnostic;

	if (str)
		fprintf(stderr, "%s: warning:\n", str);

	va_start(args, fmt);
	diagnostic_set_info(&diagnostic, fmt, &args,
			    UNKNOWN_LOCATION, DK_WARNING);
	diagnostic.option_index = 0;

	report_diagnostic(&diagnostic);
	va_end(args);
}

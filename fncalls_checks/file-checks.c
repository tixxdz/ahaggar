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
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "checks-utils.h"
#include "regex-utils.h"
#include "file-checks.h"

static char *open_args_int_cst = "<integer_cst> ";

static char *open_to_int_cst_arg(const char *call, int arg_idx, int flags)
{
	char *s = call_start_at_arg(call, arg_idx);

	if (!s)
		return s;

	if (strncmp(s, open_args_int_cst, sizeof(open_args_int_cst)))
		return NULL;

	return get_arg_value(s);
}

static inline int is_others_read(mode_t mode)
{
	return mode & S_IROTH;
}

static inline int is_others_write(mode_t mode)
{
	return mode & S_IWOTH;
}

static inline int is_others_exec(mode_t mode)
{
	return mode & S_IXOTH;
}

static inline int is_group_write(mode_t mode)
{
	return mode & S_IWGRP;
}

static inline int is_group_exec(mode_t mode)
{
	return mode & S_IXGRP;
}

int open_assumes_file_exists(char *call, int flags,
			     char *buf, size_t buflen)
{
	int ret = REG_NOMATCH;

	if (get_args_nr(call) == 2)
		return 0;

	return ret;
}

int open_needs_mode_arg(char *call, int flags,
			char *buf, size_t buflen)
{
	char *arg_flag;
	long int aflag;
	int ret = REG_NOMATCH;

	if (get_args_nr(call) == 3)
		return ret;

	arg_flag = open_to_int_cst_arg(call, 2, 0);
	if (!arg_flag)
		return ret;

	errno = 0;
	aflag = strtol(arg_flag, NULL, 0);
	if (errno)
		return ret;

	if (aflag & O_CREAT)
		return 0;

	return ret;
}

int open_chk_flag_mode(char *call, int flags,
		       char *buf, size_t buflen)
{
	char *arg_flag;
	char *arg_mode;
	long int aflag;
	unsigned long int amode;
	int ret = REG_NOMATCH;

	arg_flag = open_to_int_cst_arg(call, 2, 0);
	if (arg_flag) {
		errno = 0;
		aflag = strtol(arg_flag, NULL, 0);
		if (errno)
			return ret;
	}

	arg_mode = open_to_int_cst_arg(call, 3, 0);
	if (arg_mode) {
		errno = 0;
		amode = strtoul(arg_mode, NULL, 0);
		if (errno)
			return ret;
	}

	if (arg_mode) {
		int m = is_others_read(amode);
		m |= is_others_write(amode);
		m |= is_others_exec(amode);
		m |= is_group_exec(amode);
		if (!m)
			return ret;

		if (!*buf)
			snprintf(buf, buflen,
				 "function 'open' mode: ");
		else
			strncat(buf, "mode: ",
				buflen - strlen(buf) - 1);

		snprintf(buf + strlen(buf), buflen - strlen(buf),
			 "%05o", amode);
	}

	return 0;
}

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

#ifdef _FILE_OFFSET_BITS
#undef _FILE_OFFSET_BITS
#endif
#define _FILE_OFFSET_BIT 64

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "log.h"

mode_t get_file_perms(const char *source)
{
	struct stat st;
	mode_t default_perms = S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH;

	if (!source)
		return default_perms;

	if (lstat(source, &st) < 0)
		return default_perms;

	return st.st_mode;
}

char *get_logname(const char *source, const int pass,
		  const char *suffix)
{
	char *file = NULL;

	if (!source)
		return NULL;

	if (asprintf(&file, "%s.%03d.%s", source, pass, suffix) < 0)
		return NULL;

	return file;
}

int open_log(const char *filename, mode_t mode)
{
	int fd;
	struct stat st;

	if (lstat(filename, &st) >= 0)
		unlink(filename);

	fd = open(filename, O_WRONLY|O_CREAT|O_TRUNC|O_NOFOLLOW,
		  mode);

	return fd;
}

int close_log(int fd)
{
	return close(fd);
}

int fsync_log(int fd)
{
	return fsync(fd);
}

ssize_t write_log(int fd, const char *fmt, ...)
{
	ssize_t ret;
	va_list va;
	char *buf;

	va_start(va, fmt);
	ret = vasprintf(&buf, fmt, va);
	va_end(va);

	if (ret < 0)
		goto error;

	ret = write(fd, buf, ret);
	free(buf);

	if (ret < 0)
		goto error;

	return ret;

error:
	fprintf(stderr, "Error: failed to log data in %s:write_log()\n",
		__FILE__);
	return ret;
}

void report(int fd, const char *prefix, const char *fmt, ...)
{
	ssize_t ret;
	va_list ap;
	char *buf;

	ret = write_log(fd, "%s: report: ", prefix);
	if (ret < 0)
		return;

	va_start(ap, fmt);
	ret = vasprintf(&buf, fmt, ap);
	va_end(ap);

	if (ret < 0)
		return;

	ret = write(fd, buf, ret);
	free(buf);

	if (ret > 0)
		write_log(fd, "\n");
}

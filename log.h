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

#ifndef _LOG_H
#define _LOG_H

#include <sys/types.h>

mode_t get_file_perms(const char *source);
char *get_logname(const char *source, const int pass,
		  const char *suffix);
int open_log(const char *filename, mode_t mode);
int close_log(int fd);
int fsync_log(int fd);
ssize_t write_log(int fd, const char *fmt, ...);

#endif /* _LOG_H */

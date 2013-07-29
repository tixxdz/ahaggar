"""Ahaggar Parser."""

#
# Copyright (C) 2012-2013  Djalal Harouni <tixxdz@opendz.org>
# Copyright (C) 2012-2013  LIRE Laboratory.
# University Constantine 2, Algeria.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; version 2
# of the License.
#

import re

class Parser(object):

    def __init__(self):
        self.regfnname = re.compile(r"^\s+<.*?>\s(.+?)\(")
        self.reglocation = re.compile(r".*(\[.*?:\d+\])\n$")

    def get_next_arg(self, args, offset):
        end = 0
        idx = offset

        if args == "":
            return (end, None)

        if idx == 0:
            idx += 1

        if args[idx] == ')' or idx >= len(args):
            return (end, None)

        if args[idx] == ',':
            idx += 2

        subcall = 0
        for end in range(idx, len(args)):
            if args[end] == '(':
                subcall += 1
            elif args[end] == ')':
                if subcall > 0:
                    subcall -= 1
                else:
                    break
            elif args[end] == ',' and subcall == 0:
                break

        return (end, args[idx:end])

    def get_call_args(self, call, max_args):
        args = []
        offset = 0
        start = call.find('(')
        if start == -1:
            return args

        if max_args < 0:
            max_args = 0

        nr_args = 0
        while offset <= len(call[start:]):
            offset, arg = self.get_next_arg(call[start:], offset)
            if arg:
                nr_args += 1
                args.append(arg)
            else:
                break

            if max_args > 0 and max_args == nr_args:
                break

        return args

    def get_call_args_nr(self, call):
        return len(self.get_call_args(call, 0))

    def extract_fnname(self, call):
        s = self.regfnname.search(call)
        if s:
            return s.group(1)
        return None

    def extract_location(self, call):
        s = self.reglocation.search(call)
        if s:
            return s.group(1)
        return None

# vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4

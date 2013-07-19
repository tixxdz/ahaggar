"""Ahaggar Input helpers."""

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

import sys
import os

class Input():

    def parse_input_files(self, finput):
        fs = []
        for f in finput:
            if f == "-" or os.path.isfile(f):
                fs.append(f)
            else:
                print "Warning skipping %s, not a regular file" % f

        return fs

    def __init__(self, finput):
        fs = self.parse_input_files(finput)
        self.files = set(fs)

    def set_input(self, finput):
        fs = self.parse_input_files(finput)
        self.files = set(fs)

    def get_input(self):
        return self.files

    def process_input(self, callback, *args, **kwargs):
        nr_files = 0
        for f in self.files:
            if f == "-":
                ret = callback(sys.stdin, *args, **kwargs)
            else:
                try:
                    input = open(f, "r")
                except:
                    print "Input error: failed to open %s" % f
                    continue
                ret = callback(input, *args, **kwargs)
                input.close()

            nr_files += 1

        return nr_files

    __process_input = process_input

# vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4

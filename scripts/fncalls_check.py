#!/usr/bin/env python

#
# This file is part of the Ahaggar framework
#
# Copyright (C) 2012-2013  Djalal Harouni <tixxdz@opendz.org>
# Copyright (C) 2012-2013  LIRE Laboratory.
# University Constantine 2, Algeria.
#
#	Author(s): Djalal Harouni <tixxdz@opendz.org>
#		   Pr. Mohamed Benmohammed
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; version 2
# of the License.
#

import os
import sys
import argparse

from pahaggar.fncalls import Fncalls
from pahaggar.fncalls_compile import ALL_COMPILED_FUNCTIONS

lmatch_nr = 0  # Match per file

class Check():

    def __init__(self, files, *args):
        self.fncalls = Fncalls(files, self.process_fncalls,
                               args, None)

    def check_fncalls(self, fncalls):
        global lmatch_nr

        self.fncalls.set_current_fnflow(fncalls)

        idx = 0
        # fncalls[0] is function declaration:
        for c in fncalls[1:]:
            idx += 1
            name = self.fncalls.get_fnname(c)
            if not self.fncalls.is_compiled_call(name):
                continue

            self.fncalls.set_current_fnflow_idx(idx)

            largs = self.fncalls.get_call_args(c, 0)
            location = self.fncalls.extract_location(c)

            lmatch_nr += 1
            self.fncalls.compiled[name](self.fncalls,
                                        largs, location)

    def process_fncalls(self, input, args, kwargs):
        global lmatch_nr

        ret = -1
        lmatch_nr = 0
        fncalls = self.fncalls.read_input_calls(input)

        if len(fncalls) == 0:
            return ret

        for calls in fncalls:
            self.check_fncalls(calls)

        return 0


def main():
    parser = argparse.ArgumentParser(
                description='Process Ahaggar fncalls logs.')
    parser.add_argument('files', metavar = 'file.c.ahaggar_fncalls.log',
                        nargs = '+', help = 'Ahaggar function calls logs')
    parser.add_argument('-c, --calls', metavar = 'call1,call2,...',
                        action = 'store', dest = 'calls',
                        help = 'Function calls to check')

    args = parser.parse_args()
    if not args.calls:
        args.calls = ALL_COMPILED_FUNCTIONS
    else:
        args.calls = args.calls.split(',')

    ret = Check(args.files, *args.calls).fncalls.run()
    if ret == -1:
        print "Failed: please check the passed init arguments"

    sys.exit(ret)

if __name__ == '__main__':
    main()

# vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4

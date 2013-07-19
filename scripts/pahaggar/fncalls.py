"""Ahaggar Fncalls."""

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

from pahaggar.input import Input

class Fncalls(Input):

    def __init__(self, finput, callback, *args, **kwargs):
        Input.__init__(self, finput)
        self.set_callback(callback, *args, **kwargs)
        self.nr_files = 0

    def set_input_files(self, finput):
        self.set_input(self, finput)

    def set_callback(self, callback, *args, **kwargs):
	self.callback = callback
        self.args = args
        self.kwargs = kwargs

    def reset(self, finput, callback, *args, **kwargs):
        self.set_input(self, finput)
        self.set_callback(callback, *args, **kwargs)
        self.nr_files = 0

    def nr_processed_files(self):
        return self.nr_files

    def run(self):
        ret = self.process_input(self.callback,
                                 self.args, self.kwargs)
        self.nr_files = ret

        if self.nr_files == 0:
            return -1

        return 0

    def standarize_calls(self, calls):
        subcall = ""
        new_function = False
        func_decl = "<function_decl>"
        self.calls = []
        subfncalls = []
        for c in calls:
            if not new_function:
                if c.find(func_decl, 0, len(func_decl)) != -1:
                    new_function = True
                else:
                    continue
            elif c == "\n":
                self.calls.append(list(subfncalls))
                subfncalls = []
                new_function = False
                continue

            if self.next_is_subcall(c):
                subcall = subcall + c
                continue

            if subcall != "":
                subfncalls.append(subcall + c)
                subcall = ""
            else:
                subfncalls.append(c)

        return self.calls

    def next_is_subcall(self, call):
        if call.rfind("]", len(call) - 2, len(call)) == -1:
            return True

        return False

    __run = run

# vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4

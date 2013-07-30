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
from pahaggar.parser import Parser
from pahaggar.fncalls_compile import FncallsCompile
from pahaggar.fncalls_compile import ALL_COMPILED_FUNCTIONS

class FncallsParser(Parser):

    def __init__(self, selected_calls):
        self.calls = []
        self.cache = {}
        self.current_fnflow = ()
        self.current_fnflow_idx = 0
        Parser.__init__(self)
        self.compiled = FncallsCompile(selected_calls)

    def is_compiled_call(self, call):
        return self.compiled.is_compiled_call(call)

    def get_fnname(self, call):
        return self.extract_fnname(call)

    def get_call_args(self, call, nr_args):
        return super(FncallsParser, self).get_call_args(call, nr_args)

    def get_call_args_nr(self, call):
        return super(FncallsParser, self).get_call_args_nr(call)

    def next_is_subcall(self, call):
        if call.rfind("]\n", len(call) - 2, len(call)) == -1:
            return True

        return False

    def parse_input_calls(self, calls):
        subcall = ""
        new_function = False
        func_decl = "<function_decl>"
        subfncalls = []
        for c in calls:
            if not new_function:
                if c.startswith(func_decl, 0, len(func_decl)):
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

    def get_current_calls(self):
        return self.calls

    def read_input_calls(self, input):
        lines = []
        self.calls = []
        for line in input:
            lines.append(line)

            if line == "\n":
                self.parse_input_calls(lines)
                lines = []

        return self.get_current_calls()

    def set_current_fnflow(self, fncalls):
        self.current_fnflow_idx = 0
        self.current_fnflow = fncalls

    def get_current_fnflow(self):
        return (self.current_fnflow, self.current_fnflow_idx)

    def set_current_fnflow_idx(self, idx):
        self.current_fnflow_idx = idx

class Fncalls(Input, FncallsParser):

    def __init__(self, finput, callback, args, kwargs):
        Input.__init__(self, finput)
        FncallsParser.__init__(self, args)
        self.set_callback(callback, args, kwargs)
        self.nr_files = 0

    def set_input_files(self, finput):
        self.set_input(self, finput)

    def set_callback(self, callback, args, kwargs):
        self.callback = callback
        self.args = args
        self.kwargs = kwargs
        return True

    def reset(self, finput, callback, args, kwargs):
        self.set_input(self, finput)
        self.set_callback(callback, args, kwargs)
        self.nr_files = 0

    def run(self):
        err = -1
        if self.args[0] != ALL_COMPILED_FUNCTIONS:
            nr_calls = 0
            for c in self.args:
                if self.is_compiled_call(c):
                    nr_calls += 1

            if nr_calls == 0:
                return err

        ret = self.process_input(self.callback,
                                 self.args, self.kwargs)
        self.nr_files = ret

        if self.nr_files == 0:
            return err

        return 0

    __run = run

# vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4

"""Ahaggar: Compiled functions used for call checks."""

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

import inspect
import importlib

ALL_COMPILED_FUNCTIONS = '*'
EXPORTS_DICT = "COMPILED_EXPORTS"

COMPILED_MODULES = (
                    'pahaggar.compiletime_overflow',
                   )


class FnCompileParser(object):

    def __init__(self, object):
        self._obj = object

    def load_compiled_modules(self):
        self._obj.modules = dict((k, 0)
                            for k in map(importlib.import_module,
                                         COMPILED_MODULES))

    def clean_compiled_modules(self):
        self._obj.modules = dict((k, v)
                            for k, v in self._obj.modules.items()
                            if v == 1)

    def get_compiled_exports(self, module):
        try:
            exports = getattr(module, EXPORTS_DICT)
        except AttributeError:
            return None

        return exports

    def get_all_compiled_functions(self, exports):
        return [(k, v) for k, v in exports.iteritems()
                if inspect.isfunction(v)]

    def get_compiled_function(self, exports, function):
        value = exports.get(str(function))
        if value and inspect.isfunction(value):
            return value

        return None

    def set_compiled_functions(self, module, exports, function):
        if function == ALL_COMPILED_FUNCTIONS:
            funcs = self.get_all_compiled_functions(exports)
            if len(funcs) > 0:
                self._obj.__dict__.update(dict(funcs))
                self._obj.modules[module] = 1
        else:
            code = self.get_compiled_function(exports, function)
            if code:
                self._obj[str(function)] = code
                self._obj.modules[module] = 1

    def parse_compiled_functions(self, selected_calls):
        for call in selected_calls:
            for m in self._obj.modules.keys():
                if not inspect.ismodule(m):
                    continue

                exports = self.get_compiled_exports(m)
                if not exports:
                    continue

                self.set_compiled_functions(m, exports, call)


class FncallsCompile(object):

    def __init__(self, selected_calls):
        cparser = FnCompileParser(self)
        cparser.load_compiled_modules()
        cparser.parse_compiled_functions(selected_calls)
        cparser.clean_compiled_modules()

    def __setitem__(self, key, value):
        self.__dict__[key] = value

    def __getitem__(self, key):
        return self.__dict__[key]

    def is_compiled_call(self, call):
        try:
            return callable(getattr(self, str(call)))
        except AttributeError:
            return False

# vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4

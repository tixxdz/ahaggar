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

class Parser():

    def __init__(self):
        self.regfnname = re.compile(r"^\s+<.*?>\s(.+?)\(")
        self.reglocation = re.compile(r".*(\[.*?:\d+\])\n$")

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

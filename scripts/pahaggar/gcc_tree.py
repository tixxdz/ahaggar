"""GCC Tree.def"""

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

import os

DEFTREE = "DEFTREECODE"


class TreeParser(object):

    def __init__(self, object):
        self._obj = object
        self.tree_code_idx = 0
        self.tree_code_class_idx = 0

    def parse_treecode(self, line):
	s = line.find('(')
        if s == -1:
            return False

        args = map(str.strip, line[s+1:len(line)-1].split(','))
        if len(args) != 4:
            return False

        globals()[args[0]] = self.tree_code_idx
        self._obj[str(args[0])] = "<" + args[1] + ">"

        self.tree_code_idx += 1
        self.tree_code_class_idx += 1

        return True

    def parse_tree_def(self, tfile):
        try:
            os.path.isfile(tfile)
            input = open(tfile, "r")
        except:
            print "Failed: on '%s' gcc tree.def file" % tfile
            return -1

        map(self.parse_treecode,
            filter(lambda x: x.startswith(DEFTREE, 0, len(DEFTREE)),
                   input.readlines()))

        input.close()

        return 0


class Tree(object):

    def __init__(self, tree_file):
        tparser = TreeParser(self)
        tparser.parse_tree_def(tree_file)

    def __setitem__(self, key, value):
        self.__dict__[key] = value

    def __getitem__(self, key):
        return self.__dict__[key]


# vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4

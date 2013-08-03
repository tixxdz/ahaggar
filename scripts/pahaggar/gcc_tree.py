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

TREE = ()
tree_code_idx = 0


def parse_treecode(line):
    global tree_code_idx

    s = line.find('(')
    if s == -1:
        return ""

    args = map(str.strip, line[s+1:len(line)-1].split(','))
    if len(args) != 4:
        return ""

    globals()[args[0]] = tree_code_idx
    print args[0]

    tree_code_idx += 1
    return "<" + args[1][1:len(args[1])-1] + ">"


def read_treecode(input):
    tree = filter(lambda x: x.startswith(DEFTREE, 0, len(DEFTREE)),
                  input.readlines())

    return filter(lambda x: x and x != "",
                  map(parse_treecode, tree))


def parse_tree_def(input_file):
    global TREE
    err = -1
    try:
        os.path.isfile(input_file)
        input = open(input_file, "r")
    except:
        print "Error: reading '%s' tree.def file" % input_file
        return err

    TREE = list(read_treecode(input))
    input.close()

    if len(TREE) == 0:
        print "Error: parsing '%s' tree.def file" % input_file
        return err

    return 0


# vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4

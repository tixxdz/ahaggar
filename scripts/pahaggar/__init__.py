# pAhaggar Python module
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


import sys
import pahaggar.gcc_tree

__version__ = '20130110'
version = __version__

ret = gcc_tree.parse_tree_def("pahaggar/tree.def")
if ret == -1:
    print "Error: failed to initialize pahaggar modules"
    sys.exit(ret)

# vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4

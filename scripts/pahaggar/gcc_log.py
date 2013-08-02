"""GCC Log."""

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

class GCCLog():

    def output_context(self, location, context):
        s = location.find(':')
        if s != -1 and context:
            return "{0:s}: In {1:s}:".format(location[0:s], context)
        return None

    def error(self, prefix, location, context, msg):
        if not prefix:
            prefix = ""

        out = "{0:s}{1:s}: error: {2:s}".format(prefix, location, msg)
        return (self.output_context(location, context, out)

    def warning(self, location, context, prefix, msg):
        if not prefix:
            prefix = ""

        out = "{0:s}{1:s}: warning: {2:s}".format(prefix, location, msg)
        return (self.output_context(location, context, out)


    def report(self, location, context, prefix, msg):
        if not prefix:
            prefix = ""

        out = "{0:s}{1:s}: report: {2:s}".format(prefix, location, msg)
        return (self.output_context(location, context, out)


# vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4

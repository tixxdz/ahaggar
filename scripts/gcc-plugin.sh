#!/bin/sh
#test for gcc plugin support

#needs HOSTCC HOSTCXX CC as arguments

if [ "$#" -ne 3 ] ; then
	exit 1
fi

PLUG_PATH="`$3 -print-file-name=plugin 2>&1`/include"
if [ "$?" -ne 0 ] ; then
	exit 1
fi

plugincc=`$1 -x c -shared - -o /dev/null -I$PLUG_PATH 2>&1 <<EOF
#include "gcc-plugin.h"
#include "tree.h"
#include "tm.h"
#include "rtl.h"
#ifdef ENABLE_BUILD_WITH_CXX
#warning $2
#else
#warning $1
#endif
EOF`
if [ "$?" -ne 0 ] ; then
	exit 1
fi

if [[ "$plugincc" =~ "$1" ]] ; then
	echo "$1"
elif [[ "$plugincc" =~ "$2" ]] ; then
	echo "$2"
fi

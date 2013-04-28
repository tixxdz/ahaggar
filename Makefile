# Copyright (C) 2012-2013  Djalal Harouni <tixxdz@opendz.org>

CC ?= gcc
HOSTCC ?= gcc
HOSTCXX ?= g++
Q ?= @

ifeq ($(srctree),)
srctree := $(CURDIR)
endif

ifeq ($(objtree),)
objtree := $(CURDIR)
endif

ifeq ($(src),)
src := $(srctree)
endif

ifeq ($(obj),)
obj := $(objtree)
endif

ifeq ($(CONFIG_SHELL),)
CONFIG_SHELL := $(shell if [ -x "$$BASH" ]; then echo $$BASH; \
		else if [ -x /bin/bash ]; then echo /bin/bash; \
		else echo sh; fi ; fi)
endif

ifeq ($(PLUGINCC),)
PLUGINCC := $(shell $(CONFIG_SHELL) \
	    $(srctree)/scripts/gcc-plugin.sh \
	    "$(HOSTCC)" "$(HOSTCXX)" "$(CC)")
endif

ifneq ($(PLUGINCC),)
ahaggar-libs: ahaggar-plugins
else
ahaggar-libs:
	$(error error: GCC plugins support missing.)
endif

GCCPLUGINS_DIR := $(shell $(CC) -print-file-name=plugin)

ifeq ($(HOSTCFLAGS),)
HOSTCFLAGS = -Wall -W -O2
HOSTCFLAGS += -Wmissing-prototypes
HOSTCFLAGS += -Wstrict-prototypes
HOSTCFLAGS += -fno-delete-null-pointer-checks
HOSTCFLAGS += -fomit-frame-pointer
endif

ifeq ($(HOSTCXXFLAGS),)
HOSTCXXFLAGS = -Wall -W -O2
HOSTCXXFLAGS += -fno-delete-null-pointer-checks
HOSTCXXFLAGS += -fpermissive
endif

HOST_DBASEFLAGS = -D_FORTIFY_SOURCE=2
HOST_DBASEFLAGS += -D_GNU_SOURCE
HOST_DBASEFLAGS += -D_FILE_OFFSET_BITS=64

AHAGGAR_HDRS := $(patsubst %.h,%.h,$(wildcard $(src)/*.h))
AHAGGAR_SRCS := $(wildcard $(src)/*.c)
AHAGGAR_OBJS := $(patsubst %.c,%.o,$(wildcard $(obj)/*.c))
AHAGGAR_CORE_OBJS = buffer.o log.o str-utils.o
AHAGGAR_GCC_UTILS_OBJS = gcc-utils.o nodes-utils.o
AHAGGAR_GCC_EXTRA_OBJS = cache-nodes.o print-nodes.o
AHAGGAR_GCC_OBJS = walk-nodes.o walk-gimple.o

AHAGGAR_AST_OBJS := $(AHAGGAR_CORE_OBJS) \
		    $(AHAGGAR_GCC_UTILS_OBJS) \
		    $(AHAGGAR_GCC_EXTRA_OBJS) \
		    walk-nodes.o \
		    ahaggar-ast.o

AHAGGAR_FNCALLS_OBJS := $(AHAGGAR_CORE_OBJS) \
			$(AHAGGAR_GCC_UTILS_OBJS) \
			print-nodes.o \
			$(AHAGGAR_GCC_OBJS) \
			fncalls-match.o \
			ahaggar-fncalls.o

#LIB_OPEN_RACER = libahaggar_open_racer.so
#LIB_UNINITIALIZED =
LIB_AST = libahaggar_ast.so
LIB_FNCALLS = libahaggar_fncalls.so
AHAGGAR_LIBS = $(LIB_AST) $(LIB_FNCALLS)
LIBAHAGGAR_XXSHARED = $(obj)/libahaggar_ast.so.0
LIBAHAGGAR_XXSHARED += $(obj)/libahaggar_fncalls.so.0

AHAGGAR_CLEAN_BASE = $(AHAGGAR_OBJS) $(AHAGGAR_LIBS)

ifeq ($(PLUGINCC), $(HOSTCC))
HOSTLIBS := hostlibs
HOST_EXTRACFLAGS += -I. -I$(GCCPLUGINS_DIR)/include \
		    -I$(GCCPLUGINS_DIR)/include/c-family
HOST_EXTRACFLAGS += -std=gnu99 -ggdb
HOST_EXTRACFLAGS += $(HOST_DBASEFLAGS)
PLUGINCCFLAGS := $(HOSTCFLAGS) $(HOST_EXTRACFLAGS)
else
HOSTLIBS := hostcxxlibs
HOST_EXTRACXXFLAGS += -I. -I$(GCCPLUGINS_DIR)/include \
		      -I$(GCCPLUGINS_DIR)/include/c-family
HOST_EXTRACXXFLAGS += -std=gnu++98 -ggdb -Wno-unused-parameter
HOST_EXTRACXXFLAGS += $(HOST_DBASEFLAGS)
PLUGINCCFLAGS := $(HOSTCXXFLAGS) $(HOST_EXTRACXXFLAGS)
endif

ifneq ($(KERNELRELEASE),)
$(HOSTLIBS)-y := $(AHAGGAR_LIBS)
always := $($(HOSTLIBS)-y)

#libahaggar_open_racer-objs := buffer.o log.o walk-nodes.o \
			   ahaggar-open-racer.o
#libahaggar_uninitialized-objs := buffer.o log.o walk-nodes.o \
			      ahaggar-uninitialized.o
libahaggar_ast-objs := $(AHAGGAR_AST_OBJS)
libahaggar_fncalls-objs := $(AHAGGAR_FNCALLS_OBJS)

clean-files := $(AHAGGAR_CLEAN_BASE)

else

LD = $(PLUGINCC)
LD_lib = $(LD)
RM = rm -f
LN_s = ln -s -f
SHLIBMODE = 755
SHARED_LIBDIR = /lib
DEVEL_LIBDIR = /usr/lib
SHARED_LIBDIR_REL = ../..$(SHARED_LIBDIR)
DESTDIR =

MKDIR = umask 022 && mkdir -p
INSTALL = install -c


SHARED_AST = libahaggar_ast.so.0
SHARED_FNCALLS = libahaggar_fncalls.so.0
AHAGGAR_SHARED := $(SHARED_AST)
AHAGGAR_SHARED += $(SHARED_FNCALLS)

PLUGINCCFLAGS_lib = $(PLUGINCCFLAGS) -fpic

LDLIBS_lib =
LDFLAGS_shared = --shared
LDFLAGS_shared_ast = -Wl,--soname,$(SHARED_AST)
LDFLAGS_shared_fncalls = -Wl,--soname,$(SHARED_FNCALLS)
LDFLAGS_lib = $(LDFLAGS_shared)


all: ahaggar-libs
install: install-libs
remove: remove-libs

ahaggar-plugins: $(AHAGGAR_SHARED)

$(SHARED_AST): $(AHAGGAR_AST_OBJS)
	$(LD_lib) $(LDFLAGS_lib) $(LDFLAGS_shared_ast) \
	$(AHAGGAR_AST_OBJS) $(LDLIBS_lib) -o $(SHARED_AST)

$(SHARED_FNCALLS): $(AHAGGAR_FNCALLS_OBJS)
	$(LD_lib) $(LDFLAGS_lib) $(LDFLAGS_shared_fncalls) \
	$(AHAGGAR_FNCALLS_OBJS) $(LDLIBS_lib) -o $(SHARED_FNCALLS)

$(LIB_AST): $(SHARED_AST)
	$(LN_s) $(SHARED_AST) $(LIB_AST)

$(LIB_FNCALLS): $(SHARED_FNCALLS)
	$(LN_s) $(SHARED_FNCALLS) $(LIB_FNCALLS)

%.o: $(src)/%.c
	$(PLUGINCC) $(PLUGINCCFLAGS_lib) -o $@ -c $<

install-libs:
	$(MKDIR) $(DESTDIR)$(SHARED_LIBDIR)
	$(INSTALL) -m $(SHLIBMODE) $(SHARED_AST) $(DESTDIR)$(SHARED_LIBDIR)/
	$(INSTALL) -m $(SHLIBMODE) $(SHARED_FNCALLS) $(DESTDIR)$(SHARED_LIBDIR)/

	$(MKDIR) $(DESTDIR)$(DEVEL_LIBDIR)
	$(LN_s) $(SHARED_LIBDIR_REL)/$(SHARED_AST) \
	$(DESTDIR)$(DEVEL_LIBDIR)/$(LIB_AST)
	$(LN_s) $(SHARED_LIBDIR_REL)/$(SHARED_FNCALLS) \
	$(DESTDIR)$(DEVEL_LIBDIR)/$(LIB_FNCALLS)


remove-libs:
	$(RM) $(DESTDIR)$(DEVEL_LIBDIR)/$(LIB_AST)
	$(RM) $(DESTDIR)$(DEVEL_LIBDIR)/$(LIB_FNCALLS)
	$(RM) $(DESTDIR)$(SHARED_LIBDIR)/$(SHARED_FNCALLS)
	$(RM) $(DESTDIR)$(SHARED_LIBDIR)/$(SHARED_AST)


clean:
	$(RM) $(AHAGGAR_CLEAN_BASE) $(AHAGGAR_SHARED)
	$(RM) $(LIBAHAGGAR_XXSHARED)

.PHONY: all ahaggar-libs ahaggar-plugins install install-libs \
	remove-libs remove clean

endif

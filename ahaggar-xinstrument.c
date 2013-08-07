/*
 * The Ahaggar framework
 *
 * ahaggar-xinstrument: GCC plugin to perform instrumentation
 *
 * Copyright (C) 2012-2013  Djalal Harouni <tixxdz@opendz.org>
 * Copyright (C) 2012-2013  LIRE Laboratory.
 * University Constantine 2, Algeria.
 *
 *   Author(s): Djalal Harouni <tixxdz@opendz.org>
 *              Pr. Mohamed Benmohammed
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2
 * of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "gcc-plugin.h"
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tree.h"
#include "tree-pass.h"
#include "tree-flow.h"
#include "tree-iterator.h"
#include "intl.h"
#include "plugin.h"
#include "diagnostic.h"
#include "basic-block.h"
#include "gimple.h"
#include "plugin-version.h"

#include "ahaggar.h"
#include "buffer.h"
#include "log.h"
#include "gcc-utils.h"
#include "nodes-utils.h"
#include "cache-nodes.h"
#include "xinstrument_calls/xinstrument.h"

int plugin_is_GPL_compatible;

static struct plugin_data *ahg;

static unsigned int ahaggar_xinstrument_main(void);
static bool ahaggar_xinstrument_gate(void);
static int ahg_xinstrument_checker(void *, void *);
static int ahg_xinstrument_write(void *, void *);

static struct plugin_info ahaggar_xinstrument_info = {
	.version = "201302010",
	.help = "Ahaggar xinstrument.\n"
};

static struct gimple_opt_pass ahaggar_xinstrument_pass = {
	.pass = {
		.type			= GIMPLE_PASS,
		.name			= "ahaggar_xinstrument",
		.gate			= ahaggar_xinstrument_gate,
		.execute		= ahaggar_xinstrument_main,
		.sub			= NULL,
		.next			= NULL,
		.static_pass_number	= 0,
		.tv_id			= TV_NONE,
		.properties_required	= PROP_cfg | PROP_ssa,
		.properties_provided	= 0,
		.properties_destroyed	= 0,
		.todo_flags_start       = 0,
		.todo_flags_finish      = TODO_verify_ssa
					  | TODO_cleanup_cfg,
	},
};

static struct target_call tcalls[] = {
#include "xinstrument_calls/xinstrument-calls.h"
};

static bool __is_builtin(const char *fnname,
			 struct target_call *tcall)
{
	struct insert_builtin *ib;

	if (!tcall || !tcall->builtins)
		return false;

	for (ib = tcall->builtins; ib && ib->active; ib++) {
		if (!strcmp(fnname, ib->name))
			return true;
	}

	return false;
}

static bool is_builtin(const char *fnname)
{
	size_t i;

	for (i = 0; i < ARRAY_SIZE(tcalls); i++) {
		if (__is_builtin(fnname, &tcalls[i]))
			return true;
	}

	return false;
}

static struct target_call *is_target(const char *fnname)
{
	size_t i;

	for (i = 0; i < ARRAY_SIZE(tcalls); i++) {
		if (!strcmp(fnname, tcalls[i].name))
			return &tcalls[i];
	}

	return NULL;
}

static int ahg_instrument_gimple_call(struct target_call *tcall,
				      gimple stmt, void *data)
{
	int ret = 0;
	struct target_call *tc = tcall;
	struct insert_builtin *ib;

	if (!tc || !tc->builtins)
		return ret;

	for (ib = tc->builtins; ib && ib->active; ib++) {
		ret |= gxinstrument_target(tc, ib, stmt, data);
	}

	return ret;
}

static int ahg_check_gimple_stmt(gimple gs, void *data,
				 struct target_call **tcall)
{
	int ret = -1;
	tree fncall;
	const char *fnname;
	struct target_call *tc;

	if (!gs)
		return ret;

	if (!is_gimple_call(gs)
	|| !(fncall = gimple_call_fndecl(gs))
	|| !is_function_decl(fncall)
	|| DECL_ARTIFICIAL(fncall))
		return ret;

	if (ahg->checker(current_function_decl, data))
		return ret;

	fnname = __get_decl_name(fncall, 0);
	tc = is_target(fnname);

	if (!tc || tc->code != gimple_code(gs))
		return ret;

	*tcall = tc;
	/*
	if (sptree_nodes_contains(ahg->sp_nodes, fndecl))
		return ret;

	__sptree_nodes_insert(ahg->sp_nodes, fndecl);

	fnname = __get_decl_name(fndecl, 0);
	fprintf(stderr, "fnname: %s\n", fnname);
	*/

	return 0;
}

static int ahg_bb_gimple_stmt(basic_block bb, void *data)
{
	int ret = 0;
	gimple_stmt_iterator gsi;

	for (gsi = gsi_start_bb(bb); !gsi_end_p(gsi); gsi_next(&gsi)) {
		struct target_call *tcall = NULL;
		gimple stmt = gsi_stmt(gsi);
		if (ahg_check_gimple_stmt(stmt, data, &tcall))
			continue;

		ahg_instrument_gimple_call(tcall, stmt, data);
	}

	return ret;
}

static unsigned int ahaggar_xinstrument_main(void)
{
	int ret = 0;
	basic_block bb;
	tree fn = current_function_decl;

	ahg->sp_nodes = sptree_nodes_init();
	sptree_nodes_set_root(ahg->sp_nodes, fn);

	FOR_EACH_BB(bb) {
		ahg_bb_gimple_stmt(bb, ahg);
	}

	sptree_nodes_finish(ahg->sp_nodes);

	return ret;
}

static bool ahaggar_xinstrument_gate(void)
{
	return true;
}

static int ahg_xinstrument_checker(void *call, void *data)
{
	int ret = -1;
	const char *fndecl = __get_decl_name(call, 0);

	/* Do not instrument our own inserted function
	 * (avoid recursive calls) */
	if (is_builtin(fndecl))
		return ret;

	return 0;
}

static int ahg_xinstrument_write(__attribute__((unused)) void *data,
				 __attribute__((unused)) void *plug_data)
{
	return 0;
}

static void start_unit(__attribute__((unused)) void *gcc_data,
		       __attribute__((unused)) void *user_data)
{
	const char *input = main_input_filename ? main_input_filename : "-";

	ahg->fd = STDERR_FILENO;

	write_log(ahg->fd, "%s\n", AHAGGAR_HEADER);
	fprintf(stderr, "GCC plugin '%s' processing unit: %s\n",
		ahg->plugin_name, input);
}

static void finish_unit(__attribute__((unused)) void *gcc_data,
			__attribute__((unused)) void *user_data)
{
	const char *input = main_input_filename ? main_input_filename : "-";

	if (ahg->fd != STDERR_FILENO) {
		fsync_log(ahg->fd);
		close_log(ahg->fd);
	}

	fprintf(stderr, "\nGCC plugin '%s' finished unit: %s\n",
		ahg->plugin_name, input);
}

static struct plugin_data *ahaggar_xinstrument_init(struct plugin_name_args *plugin_info)
{
	struct plugin_data *l_ahg;

	l_ahg = (struct plugin_data *)xmalloc(sizeof(struct plugin_data));
	memset(l_ahg, 0, sizeof(struct plugin_data));
	strncpy(l_ahg->plugin_name, plugin_info->base_name,
		sizeof(ahg->plugin_name) - 1);

	l_ahg->tree_walker = (walk_tree_fn)ahg_null_walker;
	l_ahg->checker = ahg_xinstrument_checker;
	l_ahg->output_handler = ahg_xinstrument_write;

	return l_ahg;
}

static void ahaggar_fncalls_finish(__attribute__((unused)) void *gcc_data,
				   void *user_data)
{
	struct plugin_data *l_ahg = (struct plugin_data *)user_data;

	free(l_ahg);
}

int plugin_init(struct plugin_name_args *plugin_info,
		struct plugin_gcc_version *version)
{
	int ret = -1;
	struct register_pass_info ahaggar_xinstrument_pass_info = {
		.pass				= &ahaggar_xinstrument_pass.pass,
		.reference_pass_name		= "ssa",
		.ref_pass_instance_number	= 1,
		.pos_op                         = PASS_POS_INSERT_AFTER
	};

	if (!plugin_default_version_check(version, &gcc_version)) {
		error(G_("incompatible gcc/plugin versions"));
		return ret;
	}

	ahg = ahaggar_xinstrument_init(plugin_info);
	if (!ahg) {
		error(G_("GCC plugin: failed to initialize plugin"));
		return ret;
	}

	register_callback(ahg->plugin_name, PLUGIN_INFO,
			  NULL, &ahaggar_xinstrument_info);
	register_callback(ahg->plugin_name, PLUGIN_START_UNIT,
			  (plugin_callback_func)&start_unit, NULL);
	register_callback(ahg->plugin_name, PLUGIN_FINISH_UNIT,
			  (plugin_callback_func)&finish_unit, NULL);
	register_callback(ahg->plugin_name,
			  PLUGIN_PASS_MANAGER_SETUP, NULL,
			  &ahaggar_xinstrument_pass_info);
	register_callback(ahg->plugin_name, PLUGIN_FINISH,
			  (plugin_callback_func)&ahaggar_fncalls_finish,
			  ahg);

	return 0;
}

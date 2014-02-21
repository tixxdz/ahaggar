/*
 * The 'Ahaggar framework'
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

#include "gcc-plugin.h"
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tree.h"
#include "tree-pass.h"
#include "langhooks.h"
#include "cp/cp-tree.h"
#include "tm.h"
#include "intl.h"
#include "plugin.h"

#include "buffer.h"
#include "gcc-utils.h"
#include "nodes-utils.h"

/*
 * All the __get_*() helpers use static data buffer
 *
 * On every call the buffer will be cleared, so make sure to
 * store the result somewhere first.
 */

static output_buf *gout;

static const char *__decl_name(tree decl)
{
	tree id = DECL_P(decl) ? DECL_NAME(decl) : decl;
	return id ? IDENTIFIER_POINTER(id) : "<unnamed>";
}

const char *__get_class_or_enum(tree t)
{
	if (TREE_CODE(t) == ENUMERAL_TYPE) {
		if (SCOPED_ENUM_P(t))
			return "enum class";
		else
			return "enum";
	} else if (TREE_CODE(t) == UNION_TYPE) {
		return "uninon";
	} else if (TYPE_LANG_SPECIFIC(t) && CLASSTYPE_DECLARED_CLASS(t)) {
		return "class";
	} else {
		return "struct";
	}
}

char *__get_type_declaration(tree t)
{
	output_buf *adata = output_local_prepare(gout);

	if (!t)
		goto out;

	if (TREE_CODE(t) == TYPE_DECL)
		output_printf(adata, "typedef ");

	if (CODE_CONTAINS_STRUCT(TREE_CODE(t), TS_DECL_WRTL)
	&& DECL_REGISTER(t))
		output_printf(adata, "register ");

	if (TREE_PUBLIC(t) && DECL_EXTERNAL(t))
		output_printf(adata, "extern ");
	else if (TREE_STATIC(t))
		output_printf(adata, "static ");

out:
	return output_buf(adata);
}

char *__get_type_quals(tree t)
{
	unsigned int quals;
	output_buf *adata = output_local_prepare(gout);

	if (!t)
		goto out;

	quals = TYPE_QUALS(t);

	if (quals & TYPE_QUAL_CONST)
		output_printf(adata, "const ");
	if (quals & TYPE_QUAL_VOLATILE)
		output_printf(adata, "volatile ");
	if (quals & TYPE_QUAL_RESTRICT)
		output_printf(adata, "restrict ");

out:
	return output_buf(adata);
}

/* TODO: complete this function */
char *__get_tcc_declaration(tree t, int flags)
{
	enum tree_code code;
	enum tree_code_class tclass;
	output_buf *adata = output_local_prepare(gout);

	if (!t)
		goto out;

	code = TREE_CODE(t);
	tclass = TREE_CODE_CLASS(code);

	if (tclass != tcc_declaration)
		goto out;

	if (CODE_CONTAINS_STRUCT(code, TS_DECL_COMMON)) {
		if (DECL_UNSIGNED(t))
			output_printf(adata, "unsigned");
		if (DECL_IGNORED_P(t))
			output_printf(adata, "ignored");
		if (DECL_ABSTRACT(t))
			output_printf(adata, "abstract");
		if (DECL_EXTERNAL(t))
			output_printf(adata, "external");
		if (DECL_NONLOCAL(t))
			output_printf(adata, "nonlocal");
	}

out:
	return output_buf(adata);
}

/* TODO: complete this function */
char *__get_tcc_type(tree t, int flags)
{
	enum tree_code code;
	enum tree_code_class tclass;
	output_buf *adata = output_local_prepare(gout);

	if (!t)
		goto out;

	code = TREE_CODE(t);
	tclass = TREE_CODE_CLASS(code);

	if (tclass != tcc_type)
		goto out;

	if (TYPE_NAME(t)) {
		output_printf(adata, "%s",
			      __get_decl_name(TYPE_NAME(t), flags));
	} else if (code == VECTOR_TYPE) {
		output_printf(adata, "vector");
	} else if (code == INTEGER_TYPE) {
		if (TYPE_IS_SIZETYPE(t))
			output_printf(adata, " sizetype");

		output_printf(adata, "%s",
			      TYPE_UNSIGNED(t) ? "unsigned" : "signed");

		if (TYPE_PRECISION(t) == CHAR_TYPE_SIZE)
			output_printf(adata, " char");
		else if (TYPE_PRECISION(t) == SHORT_TYPE_SIZE)
			output_printf(adata, " short");
		else if (TYPE_PRECISION(t) == INT_TYPE_SIZE)
			output_printf(adata, " int");
		else if (TYPE_PRECISION(t) == LONG_TYPE_SIZE)
			output_printf(adata, " long");
		else if (TYPE_PRECISION(t) == LONG_LONG_TYPE_SIZE)
			output_printf(adata, " long long");
		else if (TYPE_PRECISION(t) >= CHAR_TYPE_SIZE
		&& exact_log2(TYPE_PRECISION(t)) != -1) {
			output_printf(adata, " int%d_t",
				      TYPE_PRECISION(t));
		} else {
			output_printf(adata, ":unnamed%d",
				      TYPE_PRECISION(t));
		}

	} else if (code == COMPLEX_TYPE) {
		output_printf(adata, "__complex__");
	} else if (code == REAL_TYPE) {
		output_printf(adata, "float");
	} else if (code == FIXED_POINT_TYPE) {
		output_printf(adata, "fixed-point");
	} else if (code == VOID_TYPE) {
		output_printf(adata, "void");
	} else if (code == UNION_TYPE || code == RECORD_TYPE) {
		output_printf(adata, "%s", __get_class_or_enum(t));
	} else {
		output_printf(adata, "<unnamed type>");
	}

	if (TYPE_PACKED(t))
		output_printf(adata, " packed");

	if (TYPE_RESTRICT(t))
		output_printf(adata, " restrict");

out:
	return output_buf(adata);
}

char *__get_location(tree t)
{
	location_t loc;
	output_buf *adata = output_local_prepare(gout);

	if (!t)
		goto out;

	loc = location_of(t);
	if (!location_is_unknown(loc))
		output_printf(adata, "[%s:%d]",
			      LOCATION_FILE(loc), LOCATION_LINE(loc));

out:
	return output_buf(adata);
}

char *__get_location_as_string(location_t loc)
{
	output_buf *adata = output_local_prepare(gout);

	if (!loc)
		goto out;

	if (!location_is_unknown(loc))
		output_printf(adata, "%s:%d",
			      LOCATION_FILE(loc), LOCATION_LINE(loc));
out:
	return output_buf(adata);
}

char *__get_decl_name(tree decl, int flags)
{
	output_buf *adata = output_local_prepare(gout);

	if (decl) {
		if (DECL_P(decl) && (flags & TDF_ASMNAME)
		&& DECL_ASSEMBLER_NAME_SET_P(decl)) {
			tree id = DECL_ASSEMBLER_NAME(decl);
			output_printf(adata, "%s",
				      IDENTIFIER_POINTER(id));
		} else {
			output_printf(adata, "%s",
				      __decl_name(decl));
		}
	}

	return output_buf(adata);
}

char *__get_function_name(tree fn, int flags)
{
	tree t = fn;
	output_buf *adata = output_local_prepare(gout);

	if (!t)
		goto out;

	if (TREE_CODE(t) == NOP_EXPR)
		t = TREE_OPERAND(t, 0);

	if (DECL_NAME(t) && (flags & TDF_ASMNAME) == 0)
		output_printf(adata, "%s",
			      lang_hooks.decl_printable_name(t, 1));
	else
		return __get_decl_name(t, flags);

out:
	return output_buf(adata);
}

char *__get_expr_code(tree expr)
{
	output_buf *adata = output_local_prepare(gout);

	if (expr)
		output_printf(adata, "%s",
			      tree_code_name[TREE_CODE(expr)]);

	return output_buf(adata);
}

/*
 * Adapted from dehydra Mozilla framework
 * function dehydra_intCstToString()
 *
 * buflen should at least be 32.
 *
 * modified to:
 *  check unsigned and long types at the begining
 *  clear buffer.
 *  get rid of strcat() calls...
 */
char *__get_int_cst(tree int_cst)
{
	char buffer[32];	/*holds repr of up to 64-bit ints*/
	char long_type[4];
	tree type = TREE_TYPE(int_cst);
	int is_unsigned = TYPE_UNSIGNED(type);
	output_buf *adata = output_local_prepare(gout);

	memset(buffer, 0, sizeof(buffer));
	memset(long_type, 0, sizeof(long_type));

	/*
	if (adata->size < sizeof(buffer))
		return;
	*/

	if (TREE_CODE(type) == POINTER_TYPE) {
		snprintf(buffer, sizeof(buffer),
			 HOST_WIDE_INT_PRINT_DEC,
			 (HOST_WIDE_INT)TREE_INT_CST_LOW(int_cst));
		goto out;
	}

	if (type == long_integer_type_node ||
		type == long_unsigned_type_node)
		sprintf(long_type, "%s", "l");
	else if (type == long_long_integer_type_node ||
		type == long_long_unsigned_type_node)
		sprintf(long_type, "%s", "ll");

#if HOST_BITS_PER_WIDE_INT > 32
	/* TREE_INT_CST_LOW(int_cst) is a 64-bit integer here */
	snprintf(buffer, sizeof(buffer),
		 is_unsigned ? "%"PRIu64"u%s" : "%"PRId64"%s",
		 TREE_INT_CST_LOW(int_cst), long_type);
#else
	int high = TREE_INT_CST_HIGH(int_cst);
	int low = TREE_INT_CST_LOW(int_cst);
	if (high == 0 || (high == -1 && !is_unsigned)) {
		/* GCC prints negative signed numbers in hex, we print
		 * using %d.
		 * GCC prints unsigned numbers as if signed, we really do
		 * unsigned. */
		snprintf(buffer, sizeof(buffer),
			 is_unsigned ? "%uu%s" : "%d%s", low, long_type);
	} else {
		/* GCC prints negative 64-bit constants in hex, we want
		 * %d.
		 * GCC prints large positive unsigned 64-bit constants in
		 * hex, we want %u */
		snprintf(buffer, sizeof(buffer),
			 is_unsigned ? "%lluu%s" : "%lld%s",
			 ((long long)high << 32) | (0xffffffffll & low),
			 long_type);
	}
#endif

out:
	output_printf(adata, "%s", buffer);
	return output_buf(adata);
}

/* Code derived from GCC pretty_print_string() */
char *__get_string_cst(tree string_cst)
{
#define MAX_STR_PRINT 10
	int i;
	const char *ptr;
	tree node = string_cst;
	output_buf *adata = output_local_prepare(gout);

	if (!node)
		goto out;

	for (i = 0, ptr = TREE_STRING_POINTER(node);
	*ptr && i < MAX_STR_PRINT; ptr++, i++) {

		switch (ptr[0]) {
		case '\b':
			output_printf(adata, "\\b");
			break;

		case '\f':
			output_printf(adata, "\\f");
			break;

		case '\n':
			output_printf(adata, "\\n");
			break;

		case '\r':
			output_printf(adata, "\\r");
			break;

		case '\t':
			output_printf(adata, "\\t");
			break;

		case '\v':
			output_printf(adata, "\\v");
			break;

		case '\\':
			output_printf(adata, "\\\\");
			break;

		case '\"':
			output_printf(adata, "\\\"");
			break;

		case '\'':
			output_printf(adata, "\\'");
			break;

		case '\1':
			output_printf(adata, "\\1");
			break;

		case '\2':
			output_printf(adata, "\\2");
			break;

		case '\3':
			output_printf(adata, "\\3");
			break;

		case '\4':
			output_printf(adata, "\\4");
			break;

		case '\5':
			output_printf(adata, "\\5");
			break;

		case '\6':
			output_printf(adata, "\\6");
			break;

		case '\7':
			output_printf(adata, "\\7");
			break;

		default:
			output_char(adata, ptr[0]);
			break;
		}
	}

	if (TREE_STRING_LENGTH(node) > MAX_STR_PRINT)
		output_printf(adata, "...");

out:
	return output_buf(adata);
}

char *__get_extra_decl(tree decl)
{
	output_buf *adata = output_local_prepare(gout);

	if (decl) {
		/*t = DECL_RESULT(decl);*/
		if (TREE_CODE(decl) == RESULT_DECL)
			return __get_expr_code(decl);
	}

	return output_buf(adata);
}

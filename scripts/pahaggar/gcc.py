"""GCC Utils."""

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

from pahaggar.gcc_tree import TREE

def is_error_mark(self, code):
    return TREE[code] == gcc_tree.ERROR_MARK

def is_identifier_node(self, code):
    return TREE[code] == gcc_tree.IDENTIFIER_NODE

def is_tree_list(self, code):
    return TREE[code] == gcc_tree.TREE_LIST

def is_tree_binfo(self, code):
    return TREE[code] == gcc_tree.TREE_BINFO

def is_tree_vec(self, code):
    return TREE[code] == gcc_tree.TREE_VEC

def is_block(self, code):
    return TREE[code] == gcc_tree.BLOCK

def is_void_type(self, code):
    return TREE[code] == gcc_tree.VOID_TYPE

def is_integer_type(self, code):
    return TREE[code] == gcc_tree.INTEGER_TYPE

def is_real_type(self, code):
    return TREE[code] == gcc_tree.REAL_TYPE

def is_fixed_point_type(self, code):
    return TREE[code] == gcc_tree.FIXED_POINT_TYPE

def is_complex_type(self, code):
    return TREE[code] == gcc_tree.COMPLEX_TYPE

def is_vector_type(self, code):
    return TREE[code] == gcc_tree.VECTOR_TYPE

def is_enumeral_type(self, code):
    return TREE[code] == gcc_tree.ENUMERAL_TYPE

def is_boolean_type(self, code):
    return TREE[code] == gcc_tree.BOOLEAN_TYPE

def is_pointer_type(self, code):
    return TREE[code] == gcc_tree.POINTER_TYPE

def is_reference_type(self, code):
    return TREE[code] == gcc_tree.REFERENCE_TYPE

def is_offset_type(self, code):
    return TREE[code] == gcc_tree.OFFSET_TYPE

def is_mem_ref(self, code):
    return TREE[code] == gcc_tree.MEM_REF

def is_target_mem_ref(self, code):
    return TREE[code] == gcc_tree.TARGET_MEM_REF

def is_array_type(self, code):
    return TREE[code] == gcc_tree.ARRAY_TYPE

def is_record_type(self, code):
    return TREE[code] == gcc_tree.RECORD_TYPE

def is_union_type(self, code):
    return TREE[code] == gcc_tree.UNION_TYPE

def is_qual_union_type(self, code):
    return TREE[code] == gcc_tree.QUAL_UNION_TYPE

def is_lang_type(self, code):
    return TREE[code] == gcc_tree.LANG_TYPE

def is_statement_list(self, code):
    return TREE[code] == gcc_tree.STATEMENT_LIST

def is_integer_cst(self, code):
    return TREE[code] == gcc_tree.INTEGER_CST

def is_real_cst(self, code):
    return TREE[code] == gcc_tree.REAL_CST

def is_fixed_cst(self, code):
    return TREE[code] == gcc_tree.FIXED_CST

def is_complex_cst(self, code):
    return TREE[code] == gcc_tree.COMPLEX_CST

def is_string_cst(self, code):
    return TREE[code] == gcc_tree.STRING_CST

def is_vector_cst(self, code):
    return TREE[code] == gcc_tree.VECTOR_CST

def is_function_type(self, code):
    return TREE[code] == gcc_tree.FUNCTION_TYPE

def is_methode_type(self, code):
    return TREE[code] == gcc_tree.METHODE_TYPE

def is_function_decl(self, code):
    return TREE[code] == gcc_tree.FUNCTION_DECL

def is_const_decl(self, code):
    return TREE[code] == gcc_tree.CONST_DECL

def is_label_decl(self, code):
    return TREE[code] == gcc_tree.LABEL_DECL

def is_result_decl(self, code):
    return TREE[code] == gcc_tree.RESULT_DECL

def is_type_decl(self, code):
    return TREE[code] == gcc_tree.TYPE_DECL

def is_var_decl(self, code):
    return TREE[code] == gcc_tree.VAR_DECL

def is_parm_decl(self, code):
    return TREE[code] == gcc_tree.PARM_DECL

def is_debug_expr_decl(self, code):
    return TREE[code] == gcc_tree.DEBUG_EXPR_DECL

def is_namespace_decl(self, code):
    return TREE[code] == gcc_tree.NAMESPACE_DECL

def is_field_decl(self, code):
    return TREE[code] == gcc_tree.FIELD_DECL

def is_component_ref(self, code):
    return TREE[code] == gcc_tree.COMPONENT_REF

def is_bit_field_ref(self, code):
    return TREE[code] == gcc_tree.BIT_FIELD_REF

def is_array_ref(self, code):
    return TREE[code] == gcc_tree.ARRAY_REF

def is_array_range_ref(self, code):
    return TREE[code] == gcc_tree.ARRAY_RANGE_REF

# TODO: continue

def is_aggr_init_expr(self, code):
    return TREE[code] == gcc_tree.AGGR_INIT_EXPR

def is_call_expr(self, code):
    return TREE[code] == gcc_tree.CALL_EXPR

def is_code_valid_gcc_tree(self, code):
    return code in TREE


# vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4

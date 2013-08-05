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

from pahaggar import gcc_tree
from pahaggar.gcc_tree import TREE

gtree = gcc_tree

OP_SYMBOL = {
    gtree.MODIFY_EXPR:      "=",
    gtree.TRUTH_OR_EXPR:    "||",
    gtree.TRUTH_ORIF_EXPR:  "||",
    gtree.TRUTH_AND_EXPR:   "&&",
    gtree.TRUTH_ANDIF_EXPR: "&&",
    gtree.BIT_IOR_EXPR:     "|",
    gtree.TRUTH_XOR_EXPR:   "^",
    gtree.BIT_XOR_EXPR:     "^",
    gtree.ADDR_EXPR:        "&",
    gtree.BIT_AND_EXPR:     "&",
    gtree.ORDERED_EXPR:     "ord",
    gtree.UNORDERED_EXPR:   "unord",
    gtree.EQ_EXPR:          "==",
    gtree.UNEQ_EXPR:        "u==",
    gtree.NE_EXPR:          "!=",
    gtree.LT_EXPR:          "<",
    gtree.UNLT_EXPR:        "u<",
    gtree.LE_EXPR:          "<=",
    gtree.UNLE_EXPR:        "u<=",
    gtree.GT_EXPR:          ">",
    gtree.UNGT_EXPR:        "u>",
    gtree.GE_EXPR:          ">=",
    gtree.UNGE_EXPR:        "u>=",
    gtree.LTGT_EXPR:        "<>",
    gtree.LSHIFT_EXPR:      "<<",
    gtree.RSHIFT_EXPR:      ">>",
    gtree.LROTATE_EXPR:     "r<<",
    gtree.RROTATE_EXPR:     "r>>",
    gtree.VEC_LSHIFT_EXPR:  "v<<",
    gtree.VEC_RSHIFT_EXPR:  "v>>",
    gtree.WIDEN_LSHIFT_EXPR: "w<<",
    gtree.POINTER_PLUS_EXPR: "+",
    gtree.PLUS_EXPR:        "+",
    gtree.REDUC_PLUS_EXPR:  "r+",
    gtree.WIDEN_SUM_EXPR:   "w+",
    gtree.WIDEN_MULT_EXPR:  "w*",
    gtree.MULT_HIGHPART_EXPR: "h*",
    gtree.NEGATE_EXPR:      "-",
    gtree.MINUS_EXPR:       "-",
    gtree.BIT_NOT_EXPR:     "~",
    gtree.TRUTH_NOT_EXPR:   "!",
    gtree.MULT_EXPR:        "*",
    gtree.INDIRECT_REF:     "*",
    gtree.TRUNC_DIV_EXPR:   "/",
    gtree.RDIV_EXPR:        "/",
    gtree.CEIL_DIV_EXPR:    "/[cl]",
    gtree.FLOOR_DIV_EXPR:   "/[fl]",
    gtree.ROUND_DIV_EXPR:   "/[rd]",
    gtree.EXACT_DIV_EXPR:   "/[ex]",
    gtree.TRUNC_MOD_EXPR:   "%",
    gtree.CEIL_MOD_EXPR:    "%[cl]",
    gtree.FLOOR_MOD_EXPR:   "%[fl]",
    gtree.ROUND_MOD_EXPR:   "%[rd]",
    gtree.PREDECREMENT_EXPR: " --",
    gtree.PREINCREMENT_EXPR: " ++",
    gtree.POSTDECREMENT_EXPR: "-- ",
    gtree.POSTINCREMENT_EXPR: "++ ",
    gtree.MAX_EXPR:         "max",
    gtree.MIN_EXPR:         "min",
}

def op_symbol(code):
    if is_code_valid_gcc_tree(code):
        idx = TREE[code]
        if idx in OP_SYMBOL:
            return OP_SYMBOL[idx]

    return ""

def get_op_operands(operations):
    results = []

    return results

def is_error_mark(code):
    return TREE[code] == gtree.ERROR_MARK

def is_identifier_node(code):
    return TREE[code] == gtree.IDENTIFIER_NODE

def is_tree_list(code):
    return TREE[code] == gtree.TREE_LIST

def is_tree_binfo(code):
    return TREE[code] == gtree.TREE_BINFO

def is_tree_vec(code):
    return TREE[code] == gtree.TREE_VEC

def is_block(code):
    return TREE[code] == gtree.BLOCK

def is_void_type(code):
    return TREE[code] == gtree.VOID_TYPE

def is_integer_type(code):
    return TREE[code] == gtree.INTEGER_TYPE

def is_real_type(code):
    return TREE[code] == gtree.REAL_TYPE

def is_fixed_point_type(code):
    return TREE[code] == gtree.FIXED_POINT_TYPE

def is_complex_type(code):
    return TREE[code] == gtree.COMPLEX_TYPE

def is_vector_type(code):
    return TREE[code] == gtree.VECTOR_TYPE

def is_enumeral_type(code):
    return TREE[code] == gtree.ENUMERAL_TYPE

def is_boolean_type(code):
    return TREE[code] == gtree.BOOLEAN_TYPE

def is_pointer_type(code):
    return TREE[code] == gtree.POINTER_TYPE

def is_reference_type(code):
    return TREE[code] == gtree.REFERENCE_TYPE

def is_offset_type(code):
    return TREE[code] == gtree.OFFSET_TYPE

def is_mem_ref(code):
    return TREE[code] == gtree.MEM_REF

def is_target_mem_ref(code):
    return TREE[code] == gtree.TARGET_MEM_REF

def is_array_type(code):
    return TREE[code] == gtree.ARRAY_TYPE

def is_record_type(code):
    return TREE[code] == gtree.RECORD_TYPE

def is_union_type(code):
    return TREE[code] == gtree.UNION_TYPE

def is_qual_union_type(code):
    return TREE[code] == gtree.QUAL_UNION_TYPE

def is_lang_type(code):
    return TREE[code] == gtree.LANG_TYPE

def is_statement_list(code):
    return TREE[code] == gtree.STATEMENT_LIST

def is_integer_cst(code):
    return TREE[code] == gtree.INTEGER_CST

def is_real_cst(code):
    return TREE[code] == gtree.REAL_CST

def is_fixed_cst(code):
    return TREE[code] == gtree.FIXED_CST

def is_complex_cst(code):
    return TREE[code] == gtree.COMPLEX_CST

def is_string_cst(code):
    return TREE[code] == gtree.STRING_CST

def is_vector_cst(code):
    return TREE[code] == gtree.VECTOR_CST

def is_function_type(code):
    return TREE[code] == gtree.FUNCTION_TYPE

def is_methode_type(code):
    return TREE[code] == gtree.METHODE_TYPE

def is_function_decl(code):
    return TREE[code] == gtree.FUNCTION_DECL

def is_const_decl(code):
    return TREE[code] == gtree.CONST_DECL

def is_label_decl(code):
    return TREE[code] == gtree.LABEL_DECL

def is_result_decl(code):
    return TREE[code] == gtree.RESULT_DECL

def is_type_decl(code):
    return TREE[code] == gtree.TYPE_DECL

def is_var_decl(code):
    return TREE[code] == gtree.VAR_DECL

def is_parm_decl(code):
    return TREE[code] == gtree.PARM_DECL

def is_debug_expr_decl(code):
    return TREE[code] == gtree.DEBUG_EXPR_DECL

def is_namespace_decl(code):
    return TREE[code] == gtree.NAMESPACE_DECL

def is_field_decl(code):
    return TREE[code] == gtree.FIELD_DECL

def is_component_ref(code):
    return TREE[code] == gtree.COMPONENT_REF

def is_bit_field_ref(code):
    return TREE[code] == gtree.BIT_FIELD_REF

def is_array_ref(code):
    return TREE[code] == gtree.ARRAY_REF

def is_array_range_ref(code):
    return TREE[code] == gtree.ARRAY_RANGE_REF

# TODO: continue

def is_aggr_init_expr(code):
    return TREE[code] == gtree.AGGR_INIT_EXPR

def is_call_expr(code):
    return TREE[code] == gtree.CALL_EXPR

def is_code_valid_gcc_tree(code):
    return code in TREE


# vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4

{
	.name = "malloc",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.match_args_func = malloc_chk_size,
		},
		{ NULL },
	}
},
{
	.name = "strndup",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.match_args_func = strndup_chk_size,
		},
		{ NULL },
	}
},
{
	.name = "strndupa",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.match_args_func = strndupa_chk_size,
		},
		{ NULL },
	}
},
{
	.name = "free",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.args = "^\\(<pointer_type>\\s<integer_cst>\\s0\\)$",
		.msg = "function 'free' is nop, ptr argument is NULL"
		},
		{ NULL },
	},
},

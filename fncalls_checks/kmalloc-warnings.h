{
	.name = "kmalloc",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.match_args_func = kmalloc_chk_size,
		},
		{ NULL },
	}
},
{
	.name = "kzalloc",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.match_args_func = kzalloc_chk_size,
		},
		{ NULL },
	}
},
/*
{
	.name = "memset",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.match_args_func = memset_chk_size,
		},
		{ NULL },
	}
},
*/
{
	.name = "kfree",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.args = "^\\(<pointer_type>\\s<integer_cst>\\s0\\)$",
		.msg = "function 'kfree' is nop, ptr argument is NULL"
		},
		{ NULL },
	},
},

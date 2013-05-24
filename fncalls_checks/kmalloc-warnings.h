{
	.name = "kmalloc",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.match_args_func = kmalloc_chk_allocation_size,
		},
		{ NULL },
	}
},
{
	.name = "kzalloc",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.match_args_func = kzalloc_chk_allocation_size,
		},
		{ NULL },
	}
},
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

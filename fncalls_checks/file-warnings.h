/* Warnings on file operations */
{
	.name = "open",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.match_args_func = open_assumes_file_exists,
		.msg = "function 'open' assumes file do exist",
		},
		{
		.active = 1,
		.match_args_func = open_needs_mode_arg,
		.msg = "function 'open' needs the mode argument",
		},
		{
		.active = 1,
		.match_args_func = open_chk_flag_mode,
		},
		{ NULL },
	},
},
{
	.name = "chmod",
},

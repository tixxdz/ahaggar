/* Some of these functions were taken from
 * https://lkml.org/lkml/2013/6/6/614 by Kees Cook */
{
	.name = "scnprintf",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.flags = 3,
		.match_args_func = const_str_chk_arg,
		},
		{ NULL },
	}
},
{
	.name = "DEBUG_ENT",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.flags = 1,
		.match_args_func = const_str_chk_arg,
		},
		{ NULL },
	}
},
{
	.name = "debugl1",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.flags = 2,
		.match_args_func = const_str_chk_arg,
		},
		{ NULL },
	}
},
{
	.name = "l3_debug",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.flags = 2,
		.match_args_func = const_str_chk_arg,
		},
		{ NULL },
	}
},
{
	.name = "seq_printf",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.flags = 2,
		.match_args_func = const_str_chk_arg,
		},
		{ NULL },
	}
},
{
	.name = "pr_info",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.flags = 1,
		.match_args_func = const_str_chk_arg,
		},
		{ NULL },
	}
},
{
	.name = "pr_err",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.flags = 1,
		.match_args_func = const_str_chk_arg,
		},
		{ NULL },
	}
},
{
	.name = "pr_debug",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.flags = 1,
		.match_args_func = const_str_chk_arg,
		},
		{ NULL },
	}
},
{
	.name = "dev_info",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.flags = 2,
		.match_args_func = const_str_chk_arg,
		},
		{ NULL },
	}
},
{
	.name = "dev_err",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.flags = 2,
		.match_args_func = const_str_chk_arg,
		},
		{ NULL },
	}
},
{
	.name = "dev_set_name",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.flags = 2,
		.match_args_func = const_str_chk_arg,
		},
		{ NULL },
	}
},
{
	.name = "bdi_register",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.flags = 3,
		.match_args_func = const_str_chk_arg,
		},
		{ NULL },
	}
},
{
	.name = "kthread_create_on_node",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.flags = 4,
		.match_args_func = const_str_chk_arg,
		},
		{ NULL },
	}
},
{
	.name = "alloc_workqueue",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.flags = 1,
		.match_args_func = const_str_chk_arg,
		},
		{ NULL },
	}
},
{
	.name = "__alloc_workqueue_key",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.flags = 1,
		.match_args_func = const_str_chk_arg,
		},
		{ NULL },
	}
},
{
	.name = "alloc_ordered_workqueue",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.flags = 1,
		.match_args_func = const_str_chk_arg,
		},
		{ NULL },
	}
},
{
	.name = "kobject_set_name",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.flags = 2,
		.match_args_func = const_str_chk_arg,
		},
		{ NULL },
	}
},
{
	.name = "try_then_request_module",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.flags = 2,
		.match_args_func = const_str_chk_arg,
		},
		{ NULL },
	}
},
{
	.name = "HiSax_putstatus",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.flags = 2,
		.match_args_func = const_str_chk_arg,
		},
		{ NULL },
	}
},
{
	.name = "ext3_msg",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.flags = 3,
		.match_args_func = const_str_chk_arg,
		},
		{ NULL },
	}
},
{
	.name = "ext3_error",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.flags = 3,
		.match_args_func = const_str_chk_arg,
		},
		{ NULL },
	}
},
{
	.name = "ext3_warning",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.flags = 3,
		.match_args_func = const_str_chk_arg,
		},
		{ NULL },
	}
},
{
	.name = "__ext4_abort",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.flags = 4,
		.match_args_func = const_str_chk_arg,
		},
		{ NULL },
	}
},
{
	.name = "__ext4_error",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.flags = 4,
		.match_args_func = const_str_chk_arg,
		},
		{ NULL },
	}
},
{
	.name = "ext4_error_inode",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.flags = 5,
		.match_args_func = const_str_chk_arg,
		},
		{ NULL },
	}
},
{
	.name = "ext4_error_file",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.flags = 5,
		.match_args_func = const_str_chk_arg,
		},
		{ NULL },
	}
},
{
	.name = "ext4_msg",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.flags = 3,
		.match_args_func = const_str_chk_arg,
		},
		{ NULL },
	}
},
{
	.name = "__ext4_warning",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.flags = 4,
		.match_args_func = const_str_chk_arg,
		},
		{ NULL },
	}
},

/* Example of warnings on misc API */
{
	.name = "reboot",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.force_match = 1,
		.msg = "do not use the reboot system call."
		},
		{ NULL },
	},
},
{
	.name = "gets",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.force_match = 1,
		.msg = "the 'gets' function is dangerous and should not be used."
		},
		{ NULL },
	},
},
{
	.name = "getwd",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.force_match = 1,
		.msg = "the 'getwd' function is dangerous and should not be used."
		},
		{ NULL },
	},
},
{
	.name = "realpath",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.force_match = 1,
		.msg = "the 'realpath' function is dangerous and should not be used."
		},
		{ NULL },
	},
},
{
	.name = "mktemp",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.force_match = 1,
		.msg = "the 'mktemp' function is dangerous and should not be used."
		},
		{ NULL },
	},
},
{
	.name = "tmpnam",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.force_match = 1,
		.msg = "the 'tmpnam' function is dangerous and should not be used."
		},
		{ NULL },
	},
},
{
	.name = "tempnam",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.force_match = 1,
		.msg = "the 'tempnam' function is dangerous and should not be used."
		},
		{ NULL },
	},
},
{
	.name = "chmod",
	//world readable
	//world writable
},
{
	.name = "strcpy",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.args = "^\\(.*, <addr_expr> <string_cst>\\s.*\\)$",
		.msg_nomatch = "function 'strcpy' src argument is not a const"
		},
		{ NULL },
	},
},
{
	.name = "strcat",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.args = "^\\(.*, <addr_expr> <string_cst>\\s.*\\)$",
		.msg_nomatch = "function 'strcat' src argument is not a const"
		},
		{ NULL },
	},
},
{
	.name = "bcopy",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.force_match = 1,
		.msg = "the 'bcopy' function is obsolete. POSIX.1-2008 removed it specification"
		},
		{ NULL },
	},
},
{
	.name = "bzero",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.force_match = 1,
		.msg = "the 'bzero' function is obsolete. POSIX.1-2008 removed it specification"
		},
		{ NULL },
	},
},
{
	.name = "atoi",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.force_match = 1,
		.msg = "the 'atoi' function does not detect errors"
		},
		{ NULL },
	}
},
{
	.name = "atol",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.force_match = 1,
		.msg = "the 'atol' function does not detect errors"
		},
		{ NULL },
	}
},
{
	.name = "getpw",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.force_match = 1,
		.msg = "the 'getpw' function is dangerous and should not be used."
		},
		{ NULL },
	}
},
{
	.name = "ulimit",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.force_match = 1,
		.msg = "the 'ulimit' function is marked obsolete by POSIX.1-2008"
		},
		{ NULL },
	},
},
{
	.name = "usleep",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.force_match = 1,
		.msg = "the 'usleep' function is obsolete. POSIX.1-2008 removed it specification"
		},
		{ NULL },
	},
},

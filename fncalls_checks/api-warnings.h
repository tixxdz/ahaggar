/* Example of warnings on misc API */
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

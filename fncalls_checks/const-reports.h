/* Misc check for constants defined during compile time */
/*
{
	.name = "open",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.args = "^\\((.*(<string_cst>)+.*),)?.*\\)$",
		.all = NULL,
		.msg_nomatch = "function 'open' pathname argument is not a compile time const"
		},
		{
		.active = 1,
		.args = "^\\((.*,)?(.*(<integer_cst>)+.*)+\\)$",
		.msg_nomatch = "function 'open' flags or mode arguments are not compile time const"
		},
		{ NULL },
	},
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
{
	.name = "memset",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.args = "^\\(.*,.*,\\s<integer_cst>\\s.*\\)$",
		.msg_nomatch = "function 'memset' size_t n argument is not a compile time const"
		},
		{ NULL },
	},
},
*/
{
	.name = "strncpy",
},
{
	.name = "memset",
},
{
	.name = "memcpy",
},
{
	.name = "strncat",
},
{
	.name = "__compiletime_size_overflow",
},
{
	.name = "__compiletime_ptr_overflow",
},

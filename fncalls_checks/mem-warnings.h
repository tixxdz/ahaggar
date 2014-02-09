/* Add checks for __builitin_* functions */
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
{
	.name = "memcpy",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.match_args_func = memcpy_chk_size,
		},
		{ NULL },
	}
},
{
	.name = "strncpy",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.match_args_func = strncpy_chk_size,
		},
		{ NULL },
	}
},
{
	.name = "strncat",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.match_args_func = strncat_chk_size,
		},
		{ NULL },
	}
},
{
	.name = "memmove",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.match_args_func = memmove_chk_size,
		},
		{ NULL },
	}
},
/*{
	.name = "memcmp",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.match_args_func = memcmp_chk_size,
		},
		{ NULL },
	}
},*/
/* bcopy() was removed by POSIX.1-2008
 * Add it the an api-warnings.h file
{
	.name = "bcopy",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.match_args_func = bcopy_chk_size,
		},
		{ NULL },
	}
},
*/
/* bzero() was removed by POSIX.1-2008
 * Add it the an api-warnings.h file
{
	.name = "bzero",
	.patterns = (struct pattern_match []) {
		{
		.active = 1,
		.match_args_func = bzero_chk_size,
		},
		{ NULL },
	}
},
*/

{
	.name = "memset",
	.code = GIMPLE_CALL,
	.mode = GSI_NEW_STMT,
	.gxinsert = gsi_insert_after,
	.builtins = (struct insert_builtin []) {
		{
		.active = 1,
		.name = __BUILTIN_SIZE_OVERFLOW_FUNC,
		.gxbuild = compiletime_size_build,
		},
		{
		.active = 1,
		.name = __BUILTIN_PTR_OVERFLOW_FUNC,
		.gxbuild = compiletime_ptr_build,
		},
		{ NULL },
	}
},
{
	.name = "memcpy",
	.code = GIMPLE_CALL,
	.mode = GSI_NEW_STMT,
	.gxinsert = gsi_insert_after,
	.builtins = (struct insert_builtin []) {
		{
		.active = 1,
		.name = __BUILTIN_SIZE_OVERFLOW_FUNC,
		.gxbuild = compiletime_size_build,
		},
		{
		.active = 1,
		.name = __BUILTIN_PTR_OVERFLOW_FUNC,
		.gxbuild = compiletime_ptr_build,
		},
		{ NULL },
	}
},
{
	.name = "strncpy",
	.code = GIMPLE_CALL,
	.mode = GSI_NEW_STMT,
	.gxinsert = gsi_insert_after,
	.builtins = (struct insert_builtin []) {
		{
		.active = 1,
		.name = __BUILTIN_SIZE_OVERFLOW_FUNC,
		.gxbuild = compiletime_size_build,
		},
		{
		.active = 1,
		.name = __BUILTIN_PTR_OVERFLOW_FUNC,
		.gxbuild = compiletime_ptr_build,
		},
		{ NULL },
	}
},
{
	.name = "strlcpy",
	.code = GIMPLE_CALL,
	.mode = GSI_NEW_STMT,
	.gxinsert = gsi_insert_after,
	.builtins = (struct insert_builtin []) {
		{
		.active = 1,
		.name = __BUILTIN_SIZE_OVERFLOW_FUNC,
		.gxbuild = compiletime_size_build,
		},
		{
		.active = 1,
		.name = __BUILTIN_PTR_OVERFLOW_FUNC,
		.gxbuild = compiletime_ptr_build,
		},
		{ NULL },
	}
},
{
	.name = "copy_from_user",
	.code = GIMPLE_CALL,
	.mode = GSI_NEW_STMT,
	.gxinsert = gsi_insert_after,
	.builtins = (struct insert_builtin []) {
		{
		.active = 1,
		.name = __BUILTIN_SIZE_OVERFLOW_FUNC,
		.gxbuild = compiletime_size_build,
		},
		{
		.active = 1,
		.name = __BUILTIN_PTR_OVERFLOW_FUNC,
		.gxbuild = compiletime_ptr_build,
		},
		{ NULL },
	}
},

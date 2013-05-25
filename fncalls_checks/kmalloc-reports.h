/* Report kmalloc operations */
{
	.name = "__kmalloc",
},
{
	.name =  "kmalloc",
},
{
	.name = "kmalloc_large",
},
{
	.name = "kmalloc_node",
},
{
	.name = "kmalloc_order",
},
{
	.name = "kmalloc_order_trace",
},
{
	.name = "kmalloc_slab",
},
{
	.name = "kzalloc",
},
{
	.name = "kzalloc_node",
},
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

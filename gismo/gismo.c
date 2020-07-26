#include <linux/module.h>

static int __init gismo_init(void)
{
	pr_err("gismo: Hello\n");
	return 0;
}

static void __exit gismo_exit(void)
{
	pr_err("gismo: Goodbye\n");
}

module_init(gismo_init);
module_exit(gismo_exit);

MODULE_AUTHOR("Maciej Bielski");
MODULE_DESCRIPTION("Gismo driver");
MODULE_LICENSE("GPL");

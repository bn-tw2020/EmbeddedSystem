#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");

static int hello_init(void) {
	printk(KERN_ALERT "hello module loaded");
	return 0;
}

static void hello_exit(void) {
	printk(KERN_ALERT "hello module unloaded");
}

module_init(hello_init);
module_exit(hello_exit);

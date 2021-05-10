#include <linux/unistd.h>
#include <linux/kernel.h>

asmlinkage int sys_mysyscall(void) {
	printk("[mysyscall] Hello, World!\n");
	return 0;
}

#include <linux/module.h>
#include <linux/kernel.h>
#include<linux/fs.h>
#include <linux/init.h>
#define DEV_NAME "testdd"

static int test_major = 0;
static int result;

/* function prototype */
static int device_open(struct inode*, struct file*);
static int device_release(struct inode*, struct file*);
static ssize_t device_read(struct file*, char*, size_t, loff_t*);
static ssize_t device_write(struct file*, const char*, size_t, loff_t*);
static long device_ioctl(struct file*, unsigned int, unsigned long);
static int sample_init(void);
static void sample_cleanup(void);

/* file operation */
struct file_operations test_fops = {
    .owner = THIS_MODULE,
    .open = device_open,
    .read = device_read,
    .write = device_write,
    .unlocked_ioctl = device_ioctl,
    .release = device_release
};

/* module init or exit */
module_init(sample_init);
module_exit(sample_cleanup);
MODULE_LICENSE("GPL");

static int device_open(struct inode *inode, struct file *filp) {
    printk("test_dd start\n");
    try_module_get(THIS_MODULE);
    return 0;
}

static int device_release(struct inode *inode, struct file *filp) {
    printk("test_dd release\n");
    module_put(THIS_MODULE);
    return 0;
}

static ssize_t device_read(struct file *filp, char *buf, size_t count, loff_t *f_pos) {
    printk("test_dd read\n");
    return 0;
}

static ssize_t device_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos) {
    printk("test_dd write\n");
    return 0;
}

static long device_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) {
    printk("test_dd ioctl\n");
    return 0;
}

static int sample_init(void) {
    result = register_chrdev(test_major, DEV_NAME, &test_fops);
    if(result < 0) {
        printk(KERN_WARNING "%s: can't get major %d\n", DEV_NAME, test_major);
        return result;
    }
    printk("<1> init module success %s major number : %d\n", DEV_NAME, result);
    return 0;
}

static void sample_cleanup(void) {
    unregister_chrdev(result, DEV_NAME);
    printk("%s cleanup_module success\n", DEV_NAME);
}
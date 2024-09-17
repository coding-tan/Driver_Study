// 1. 头文件
#include <linux/module.h>  
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>

static int major = 0;
static int minor = 0;
dev_t dev_num;
struct cdev cdev_test;

struct class *class;
struct device *device;

static int cdev_test_open(struct inode *inode, struct file *file){
    printk("This is cdev_test_open\n");
    return 0;
}

static ssize_t cdev_test_read(struct file *file, char __user *buf, size_t size, loff_t *off){
    char kbuf[32] = "This is cdev_test_read!";
    if(copy_to_user(buf, kbuf, strlen(kbuf)) != 0){
        printk("copy_to_user is error\n");
        return -1;
    }
    printk("This is cdev_test_read\n");
    return 0;
}

static ssize_t cdev_test_write(struct file *file, const char __user *buf, size_t size, loff_t *off){
    char kbuf[32] = "This is cdev_test_read!";
    if(copy_from_user(kbuf, buf, size) != 0){
        printk("copy_from_user is error\n");
        return -1;
    }
    printk("kbuf is %s\n", kbuf);

    printk("This is cdev_test_write\n");
    return 0;
}

static int cdev_test_release(struct inode *inode, struct file *file){
    printk("This is cdev_test_release\n");
    return 0;
}

struct file_operations cdev_test_ops = {
    .owner = THIS_MODULE,
    .open = cdev_test_open,
    .read = cdev_test_read,
    .write = cdev_test_write,
    .release = cdev_test_release
};

// 2. 驱动加载函数
static int modulecdev_init(void){
    int ret;

    ret = alloc_chrdev_region(&dev_num, 0, 1, "chrdev_name");
        if(ret < 0){
        printk("alloc_chrdev_region is error\n");
    }
    printk("alloc_chrdev_region is success\n");
    major = MAJOR(dev_num);
    minor = MINOR(dev_num);
    printk("major is %d\n", major);
    printk("minor is %d\n", minor);
    cdev_test.owner = THIS_MODULE;
    cdev_init(&cdev_test, &cdev_test_ops);
    cdev_add(&cdev_test, dev_num, 1);

    class = class_create(THIS_MODULE, "test");
    device = device_create(class, NULL, dev_num, NULL, "test");

    return 0;
}

// 3. 驱动卸载函数
static void modulecdev_exit(void){
    cdev_del(&cdev_test);
    printk("cdev_del is ok\n");
    unregister_chrdev_region(dev_num, 1);
    printk("unregister_chrdev_region is ok\n");

    device_destroy(class, dev_num);
    class_destroy(class);

    printk("destory is ok\n");
}

// 4. 加载驱动加载函数、卸载函数
module_init(modulecdev_init);
module_exit(modulecdev_exit);

// 5. 必选许可证声明
MODULE_LICENSE("GPL");  
MODULE_AUTHOR("tanhui");
MODULE_VERSION("V1.0");
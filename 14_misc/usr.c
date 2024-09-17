// 1. 头文件
#include <linux/module.h>  
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>

static int cdev_test_open(struct inode *inode, struct file *file){
    
    printk("This is cdev_test_open\n");
    return 0;
}

static ssize_t cdev_test_read(struct file *file, char __user *buf, size_t size, loff_t *off){

    return 0;
}

static ssize_t cdev_test_write(struct file *file, const char __user *buf, size_t size, loff_t *off){
  
    return 0;
}

static int cdev_test_release(struct inode *inode, struct file *file){

    return 0;
}

struct file_operations cdev_test_ops = {
    .owner = THIS_MODULE,
    .open = cdev_test_open,
    .read = cdev_test_read,
    .write = cdev_test_write,
    .release = cdev_test_release
};

struct miscdevice misc_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "test",
    .fops = &cdev_test_ops
};

// 2. 驱动加载函数
static int modulecdev_init(void){
    int ret;

    ret = misc_register(&misc_device);
    if(ret < 0){
        printk("misc_register\n");
        return -1;
    }

    return 0;
}

// 3. 驱动卸载函数
static void modulecdev_exit(void){
    misc_deregister(&misc_device);
}

// 4. 加载驱动加载函数、卸载函数
module_init(modulecdev_init);
module_exit(modulecdev_exit);

// 5. 必选许可证声明
MODULE_LICENSE("GPL");  
MODULE_AUTHOR("tanhui");
MODULE_VERSION("V1.0");
// 1. 头文件
#include <linux/module.h>  
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>

#include <linux/errno.h>

struct device_test{
    dev_t dev_num;
    int major;
    int minor;
    struct cdev cdev_test;
    struct class *class;
    struct device *device;
    char kbuf[32];
};

struct device_test dev1;


static int cdev_test_open(struct inode *inode, struct file *file){
    file->private_data = &dev1;
    printk("This is cdev_test_open\n");
    return 0;
}

static ssize_t cdev_test_read(struct file *file, char __user *buf, size_t size, loff_t *off){
    struct device_test *test_dev = (struct device_test *)file->private_data;
    if(copy_to_user(buf, test_dev->kbuf, strlen(test_dev->kbuf)) != 0){
        printk("copy_to_user is error\n");
        return -1;
    }

    return 0;
}

static ssize_t cdev_test_write(struct file *file, const char __user *buf, size_t size, loff_t *off){
    struct device_test *test_dev = (struct device_test *)file->private_data;
    if(copy_from_user(test_dev->kbuf, buf, size) != 0){
        printk("copy_from_user is error\n");
        return -1;
    }
    printk("test_dev->kbuf is %s\n", test_dev->kbuf);

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

// 2. 驱动加载函数
static int modulecdev_init(void){
    int ret;

    ret = alloc_chrdev_region(&dev1.dev_num, 0, 1, "chrdev_name");
        if(ret < 0){
        goto err_chrdev;
    }
    printk("alloc_chrdev_region is success\n");
    dev1.major = MAJOR(dev1.dev_num);
    dev1.minor = MINOR(dev1.dev_num);
    printk("dev1.major is %d\n", dev1.major);
    printk("dev1.minor is %d\n", dev1.minor);
    dev1.cdev_test.owner = THIS_MODULE;
    cdev_init(&dev1.cdev_test, &cdev_test_ops);
    ret = cdev_add(&dev1.cdev_test, dev1.dev_num, 1);
    if(ret < 0){
        goto err_chr_add;
    }

    dev1.class = class_create(THIS_MODULE, "test");
    if(IS_ERR(dev1.class))
    {
        ret = PTR_ERR(dev1.class);
        goto err_class_create;
    }
    dev1.device = device_create(dev1.class, NULL, dev1.dev_num, NULL, "test");
    if(IS_ERR(dev1.device))
    {
        ret = PTR_ERR(dev1.device);
        goto err_device_create;
    }
 
    return 0;
err_device_create:
    class_destroy(dev1.class);
err_class_create:
    cdev_del(&dev1.cdev_test);
err_chr_add:
    unregister_chrdev_region(dev1.dev_num, 1);
err_chrdev:
    return ret;
}

// 3. 驱动卸载函数
static void modulecdev_exit(void){
    cdev_del(&dev1.cdev_test);
    printk("cdev_del is ok\n");
    unregister_chrdev_region(dev1.dev_num, 1);
    printk("unregister_chrdev_region is ok\n");

    device_destroy(dev1.class, dev1.dev_num);
    class_destroy(dev1.class);

    printk("destory is ok\n");
}

// 4. 加载驱动加载函数、卸载函数
module_init(modulecdev_init);
module_exit(modulecdev_exit);

// 5. 必选许可证声明
MODULE_LICENSE("GPL");  
MODULE_AUTHOR("tanhui");
MODULE_VERSION("V1.0");
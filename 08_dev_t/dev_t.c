// 1. 头文件
#include <linux/module.h>  
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>

static int major = 0;
static int minor = 0;
dev_t dev_num;

module_param(major, int, S_IRUGO);
module_param(minor, int, S_IRUGO);


// 2. 驱动加载函数
static int moduleparam_init(void){
    int ret;
    if(major){ // 静态注册设备号
        printk("major is %d\n", major);
        printk("minor is %d\n", minor);
        dev_num = MKDEV(major, minor);
        ret = register_chrdev_region(dev_num, 1, "chrdev_name");
        if(ret < 0){
            printk("register_chrdev_region is error\n");
        }
        printk("register_chrdev_region is success\n");
    }
    else{ // 动态注册设备号
        ret = alloc_chrdev_region(&dev_num, 0, 1, "chrdev_name");
         if(ret < 0){
            printk("alloc_chrdev_region is error\n");
        }
        printk("alloc_chrdev_region is success\n");
        major = MAJOR(dev_num);
        minor = MINOR(dev_num);
        printk("major is %d\n", major);
        printk("minor is %d\n", minor);
    }
    
    return 0;
}

// 3. 驱动卸载函数
static void moduleparam_exit(void){
   unregister_chrdev_region(dev_num, 1);
   printk("unregister_chrdev_region is ok\n");
}

// 4. 加载驱动加载函数、卸载函数
module_init(moduleparam_init);
module_exit(moduleparam_exit);

// 5. 必选许可证声明
MODULE_LICENSE("GPL");  
MODULE_AUTHOR("tanhui");
MODULE_VERSION("V1.0");
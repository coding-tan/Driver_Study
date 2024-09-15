// 1. 头文件
#include <linux/module.h>  
#include <linux/init.h>

extern int add(int a, int b);

int add(int a, int b){
    return a + b;
}

EXPORT_SYMBOL(add);

// 2. 驱动加载函数
static int helloworld_init(void){
    printk("helloworld!\n"); // 内核没有标准C库，所以用不了printf()函数
    return 0;
}
 
// 3. 驱动卸载函数
static void helloworld_exit(void){
    printk("helloworld bye!"); 
}

// 4. 加载驱动加载函数、卸载函数
module_init(helloworld_init);
module_exit(helloworld_exit);

// 5. 必选许可证声明
MODULE_LICENSE("GPL");  
MODULE_AUTHOR("tanhui");
MODULE_VERSION("V1.0");
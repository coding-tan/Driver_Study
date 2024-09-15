// 1. 头文件
#include <linux/module.h>  
#include <linux/init.h>
#include <linux/moduleparam.h>

static int a = 0;
static int array[5] = {0};
static int array_size;
static char str1[10] = {0};

module_param(a, int, S_IRUGO);
MODULE_PARM_DESC(a, "e.g: a = 1");

module_param_array(array, int, &array_size, S_IRUGO);
MODULE_PARM_DESC(array, "e.g: array = 1, 2, 3");

module_param_string(str, str1, sizeof(str1), S_IRUGO);
MODULE_PARM_DESC(str, "e.g: str1 = hello");

// 2. 驱动加载函数
static int moduleparam_init(void){
    int i = 0;
    printk("a is %d\n", a);

    for(i = 0; i < array_size; i++){
        printk("array[%d] is %d\n", i, array[i]);
    }
    printk("array_size is %d\n", array_size);

    printk("str1 is %s\n", str1);

    return 0;
}

// 3. 驱动卸载函数
static void moduleparam_exit(void){
    printk("param bye!"); 
}

// 4. 加载驱动加载函数、卸载函数
module_init(moduleparam_init);
module_exit(moduleparam_exit);

// 5. 必选许可证声明
MODULE_LICENSE("GPL");  
MODULE_AUTHOR("tanhui");
MODULE_VERSION("V1.0");
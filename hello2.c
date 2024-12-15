#include <linux/module.h>
#include <linux/printk.h>
#include "hello1.h"

MODULE_AUTHOR("Vasiliev Vlad <dfttime@ukr.net>");
MODULE_DESCRIPTION("Lab4 Module hello2.c");
MODULE_LICENSE("Dual BSD/GPL");

static uint paramValue = 1;
module_param(paramValue, uint, 0444);
MODULE_PARM_DESC(paramValue, "Parameter for print_hello");

static int __init hello2_init(void)
{
    int ret;

    pr_info("hello2 module initialized, calling print_hello()...\n");

    if (paramValue > 10) {
        pr_err("Invalid parameter: paramValue > 10\n");
        return -EINVAL;
    }

    ret = print_hello(paramValue);
    if (ret < 0) {
        pr_err("print_hello() failed with error %d\n", ret);
        return ret;
    }

    pr_info("hello2 module successfully initialized\n");
    return 0;
}

static void __exit hello2_exit(void)
{
    pr_info("Exiting hello2 module...\n");
}

module_init(hello2_init);
module_exit(hello2_exit);


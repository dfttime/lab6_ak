#include <linux/module.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include "hello1.h"

MODULE_AUTHOR("Vasiliev Vlad <dfttime@ukr.net>");
MODULE_DESCRIPTION("Lab4 Module hello1.c");
MODULE_LICENSE("Dual BSD/GPL");

struct myStruct {
    struct list_head list;
    ktime_t startTime;
    ktime_t finishTime;
};

static LIST_HEAD(myList);
static int counter;

int print_hello(uint paramValue)
{
    if (paramValue > 10) {
        pr_err("Invalid parameter: paramValue > 10\n");
        return -EINVAL;
    }

    if (paramValue == 0 || (paramValue <= 10 && paramValue >= 5)) {
        pr_warn("WARNING: paramValue is %u\n", paramValue);
    }

    counter = 0;
    while (counter < paramValue) {
        struct myStruct *ptr;

        ptr = kmalloc(sizeof(*ptr), GFP_KERNEL);
        if (!ptr) {
            pr_err("Memory allocation failed at iteration %d\n", counter);
            return -ENOMEM;
        }

        ptr->startTime = ktime_get();
        pr_debug("HELLO WORLD!\n");
        ptr->finishTime = ktime_get();
        list_add_tail(&ptr->list, &myList);
        counter++;
    }
    return 0;
}
EXPORT_SYMBOL(print_hello);

static int __init hello1_init(void)
{
    pr_info("hello1 module initialized\n");
    return 0;
}

static void __exit hello1_exit(void)
{
    struct myStruct *ptr, *next;

    pr_debug("Starting cleanup in hello1_exit...\n");

    list_for_each_entry_safe(ptr, next, &myList, list) {
        pr_debug("Node timing: %lld nanoseconds\n", 
                 ktime_to_ns(ptr->finishTime) - ktime_to_ns(ptr->startTime));
        list_del(&ptr->list);
        kfree(ptr);
    }

    pr_debug("Finished cleanup in hello1_exit...\n");
}

module_init(hello1_init);
module_exit(hello1_exit);

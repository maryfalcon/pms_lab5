#include <linux/kobject.h> 
#include <linux/string.h> 
#include <linux/sysfs.h> 
#include <linux/module.h> 
#include <linux/init.h>


static struct kobject *timer_kobj;
static struct timer_list timer;

int period = 0;

static ssize_t timer_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
        return sprintf(buf, "%d\n", period);
}

static ssize_t timer_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
 {
        sscanf(buf, "%d", &period);
        if (period == 0)
		del_timer(&timer);
	else
		mod_timer(&timer, jiffies + msecs_to_jiffies(period));
        return count;
 }
 
 static struct kobj_attribute timer_attribute =
         __ATTR(period, 0666, timer_show, timer_store);
 

static struct attribute *attrs[] = {
        &timer_attribute.attr,
        NULL,
};


static struct attribute_group timer_attr_group = {
        .attrs = attrs,
};


void timer_sysfs_callback(unsigned long data)
{
	printk("Hello, world! (milliseconds = %d)\n", jiffies_to_msecs(jiffies));
	mod_timer(&timer, jiffies + msecs_to_jiffies(period));
}

static int __init timer_init(void)
{
        int retval;

        timer_kobj = kobject_create_and_add("timer", kernel_kobj);
        if (!timer_kobj)
                 return -ENOMEM;
 
        retval = sysfs_create_group(timer_kobj, &timer_attr_group);
        if (retval)
                kobject_put(timer_kobj);

        setup_timer(&timer, timer_sysfs_callback, 0);

        return retval;
}

static void __exit timer_exit(void)
{
	del_timer(&timer);
        kobject_put(timer_kobj);
}

module_init(timer_init);
module_exit(timer_exit);
MODULE_LICENSE("GPL");

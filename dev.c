#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include<linux/fs.h>
#include<linux/types.h>
#include<linux/kdev_t.h>

static dev_t first;
static int __init hello_init(void)
{
  printk("Dev Registered");
  if(alloc_chrdev_region(&first,0,3,"Ganesh")<0)
  {
    return -1;
  }
  printk("<Major,Minor>:<%d,%d> \n",MAJOR(first),MINOR(first));
  return 0;
}
static void __exit hello_exit(void)
{
  unregister_chrdev_region(first,3);
  printk("Exiting the dev ");
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("GANESH.H");
MODULE_DESCRIPTION("FIRST CHARACTER DRIVER");

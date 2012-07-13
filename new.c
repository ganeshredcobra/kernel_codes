#include<linux/init.h>
#include<linux/module.h>

static int __init hello_init(void)
{
  //pr_alert("Good morning");
  printk(KERN_ERR "Namaskar: ofd registered");
  return 0;
}
static void __exit hello_exit(void)
{
  //pr_alert("Going to exit");
  printk(KERN_ALERT "Namaskar: ofd registered");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("TEST HELLO WORLD MODULE");
MODULE_AUTHOR("GANESH.H");
MODULE_VERSION("printk");

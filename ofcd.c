#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
 
static dev_t first; // Global variable for the first device number
 
static int __init ofcd_init(void) /* Constructor */
{
      printk(KERN_INFO "Namaskar: ofcd registered");
          if (alloc_chrdev_region(&first, 0, 3, "Shweta") < 0)
                {
                          return -1;
                 }
              printk(KERN_INFO "<Major, Minor>: <%d, %d>\n", MAJOR(first), MINOR(first));
                  return 0;
}
 
static void __exit ofcd_exit(void) /* Destructor */
{
      unregister_chrdev_region(first, 3);
      printk(KERN_INFO "Alvida: ofcd unregistered");
}
 
module_init(ofcd_init);
module_exit(ofcd_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Anil Kumar Pugalia <email_at_sarika-pugs_dot_com>");
MODULE_DESCRIPTION("Our First Character Driver");


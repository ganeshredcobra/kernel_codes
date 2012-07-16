#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/proc_fs.h>
#include <linux/fcntl.h>
#include <linux/ioport.h>
#include <asm/system.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/delay.h>

MODULE_LICENSE("Dual BSD/GPL");

static int major =61;
static int port;

static int pport_open(struct inode *inode, struct file *filp);
static int pport_close(struct inode *inode, struct file *filp);
static ssize_t pport_read(struct file *filp, char *buf, size_t count, loff_t *f_pos);
static ssize_t pport_write(struct file *filp, char *buf, size_t count, loff_t *f_pos);

static struct file_operations fops = {
open:pport_open,
read:pport_read,
write:pport_write,
release:pport_close
};

static int pport_open(struct inode *inode, struct file *filp)
{
return 0;
}

static int pport_close(struct inode *inode, struct file *filp)
{
return 0;
}

static ssize_t pport_write(struct file *filp,const char *buf, size_t count, loff_t *f_pos)
{
/*char parlelport_buffer;
char *tmp;
tmp=buf+count-1;
copy_from_user(&parlelport_buffer,tmp,1);*/
return 0;
}

static ssize_t pport_read(struct file *filp,const char *buf, size_t count, loff_t *f_pos)
{
return 0;
}

int init_module(void)
{
	int a;
	a = register_chrdev(major, "registered_pport_61", &fops);
	if(a < 0) {
	printk(KERN_ALERT "error: can't register major number %d\n", major);
	return a;
}
	port = check_region(0x378, 1);
	if(port) printk(KERN_ALERT "pport cannot reserve 0x378\n");
	request_region(0x378, 1, "registered_pport_61");
	//lcd setup//
	outb(0, 0x378);
	//udelay(10000);
	//lcd_init();
	//udelay(10000);
	//printlcd("DRIVER INSERTED ");
	return 0;
}

void cleanup_module(void)
{
	printk(KERN_ALERT "pport module is going to terminate\n");
	//	printlcd("DRIVER REMOVED  ");     
	unregister_chrdev(major, "registered_pport_61");
	//clear();
}

module_init(init_module);
module_exit(cleanup_module);


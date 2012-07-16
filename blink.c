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
 
#define LCD(x) outb((c = ((0x0f & x) | (c & 0xf0) )), 0x378)
#define RS(x) outb((c = (((~(1 << 4)) & c) | (x << 4))), 0x378)
#define EN(x) outb((c = (((~(1 << 5)) & c) | (x << 5))), 0x378)
#define en 0b00010000
#define rs 0b00100000
#define OUT(x) outb(x, 0x378)
#define MAXSIZE 17
 
static char lcd_buffer1[17];
static unsigned char c;
static char d_buf[MAXSIZE];
static int port;
static char lcd_space = ' ';
static int major = 61;
 
static int pport_open(struct inode *inode, struct file *filp);
static int pport_close(struct inode *inode, struct file *filp);
static ssize_t pport_read(struct file *filp, char *buf, size_t count, loff_t *f_pos);
static ssize_t pport_write(struct file *filp, char *buf, size_t count, loff_t *f_pos);
static void lcd_strobe(void);
static void data(unsigned char);
static void cmd(unsigned char);
static void clear(void);
static void lcd_init(void);
static void printlcd(char *);
 
static struct file_operations fops = {
    open: pport_open,
    read: pport_read,
    write: pport_write,
    release: pport_close
};
 
static int pport_open(struct inode *inode, struct file *filp)
{
    return 0;
}
 
static int pport_close(struct inode *inode, struct file *filp)
{
    return 0;
}
 
static ssize_t pport_write(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
    if(count < MAXSIZE) {
        copy_from_user(d_buf, buf, count);
        d_buf[count] = 0;
        printlcd(d_buf);
        *f_pos += count;
        return count;
        } else {
        copy_from_user(d_buf, buf, MAXSIZE - 1);
        d_buf[MAXSIZE - 1] = 0;
        printlcd(d_buf);
        *f_pos += MAXSIZE - 1;
        return MAXSIZE - 1;
    }
}
 
static ssize_t pport_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
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
    udelay(10000);
    lcd_init();
    udelay(10000);
    printlcd("DRIVER INSERTED ");
    return 0;
}
 
void cleanup_module(void)
{
    printk(KERN_ALERT "pport module is going to terminate\n");
    printlcd("DRIVER REMOVED  ");
     
    unregister_chrdev(major, "registered_pport_61");
    clear();
}
 
static void lcd_strobe(void)
{
    EN(1);
    udelay(1);
    EN(0);
    udelay(1);
}
 
static void data(unsigned char data)
{
    RS(1);
    udelay(40);
    LCD(data >> 4);
    lcd_strobe();
    LCD(data);
    lcd_strobe();
    udelay(10);
    RS(0);
    udelay(10);
}
 
static void cmd(unsigned char command)
{
    RS(0);
    udelay(40);
    LCD(command >> 4);
    lcd_strobe();
    LCD(command);
    lcd_strobe();
}
 
static void clear(void)
{
    cmd(1);
    udelay(2000);
}
 
static void lcd_init(void)
{
    cmd(0x30);
    cmd(0x30);
    cmd(0x28);
    cmd(0x0c);
    clear();
    cmd(0x6);
}
 
static void printlcd(char *p)
{
    static int count = 0;
    count = 0;
    clear();
    cmd(0x80);
    while(lcd_buffer1[count])
    data(lcd_buffer1[count++]);
    count = 0;
    cmd(0xc0);
    while(*p) {
        if((*p != '\n') && (*p != '\t')) {
            lcd_buffer1[count++] = *p;
            data(*p);
            } else {
            data(lcd_space);
            lcd_buffer1[count++] = lcd_space;
        }
        p++;
    }
    lcd_buffer1[16] = 0;
    msleep(2000);
}

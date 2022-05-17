#include<linux/module.h>   //inserting module
#include<linux/fs.h>       // file operations
#include<asm/uaccess.h>    // Copy_To_user Copy_from_user
#include<linux/cdev.h>     // charecter devices
#include<linux/init.h>     // inti exit functions
#include<linux/slab.h>     // Kmalloc(),Kfree()

#define MAJORNO 300
#define MINORNO  0
#define CHAR_DEV_NAME "MY_DEVICE_VENU"
#define MAX_LENGTH 4000
#define SUCCESS  0

static char char_device_buf[MAX_LENGTH];
struct cdev *my_cdev;
dev_t mydev;
int count =1, inuse=0;

module_init(char_dev_init);
module_exit(char_dev_exit);

static struct file_operations char_dev_fops = {
	
};

static int __init char_dev_init(void)
{
	int ret;
	mydev = MKDEV(MAJORNO,MINORNO);
	register_chrdev_region(mydev,count,CHAR_DEV_NAME); 

	my_cdev= cdev_alloc();  // allow allocate cdev instance, 
	cdev_init(my_cdev,&char_dev_fops);  

	ret=cdev_add(my_cdev,mydev,count);// register with VFS layer , count = how many minor no. in use .
	
	if( ret < 0 ) {
		printk("Error registering device driver\n");
		return ret;
	}
	printk(KERN_INFO"\nDevice Registered %s\n",CHAR_DEV_NAME); 
	printk(KERN_INFO"\nDevice Is Successfully Registered  \n"); 
	
	memset(char_device_buf,'\0',MAX_LENGTH);
	return 0;
}

static void __exit char_dev_exit(void)
{
	cdev_del(my_cdev);
	unregister_chrdev_region(mydev,1);
	printk(KERN_INFO "\n Driver unregistered \n");
}


MODULE_AUTHOR("VENU");
MODULE_DESCRIPTION("Static Character Device Driver");
MODULE_LICENSE("GPL");
	
	

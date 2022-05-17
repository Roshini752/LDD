#include<linux/module.h>   //inserting module
#include<linux/fs.h>       // file operations
#include<asm/uaccess.h>    // Copy_To_user Copy_from_user
#include<linux/cdev.h>     // charecter devices
#include<linux/init.h>     // inti exit functions
#include<linux/slab.h>     // Kmalloc(),Kfree()

//#define MAJORNO 300            // Not_reured_in_dynamic
///#define MINORNO  0
#define CHAR_DEV_NAME "MY_DEVICE_VENU"
#define MAX_LENGTH 4000
#define SUCCESS  0

static char *char_device_buf;
struct cdev *my_cdev;
dev_t mydev;
int inuse=0;


static int char_dev_open(struct inode *inode,
			    struct file  *file)
{
	printk(KERN_INFO "open operation invoked \n");
	return SUCCESS;
}

static int char_dev_release(struct inode *inode,
		            struct file *file)
{
	return SUCCESS;
}

static ssize_t char_dev_write(struct file *file,
		            const char *buf,
			    size_t lbuf,
			    loff_t *ppos)
{
	int nbytes; /* Number of bytes written */

	nbytes = lbuf - raw_copy_from_user( char_device_buf + *ppos, /* to */
					buf, /* from */ 
					lbuf ); /* how many bytes */
	*ppos += nbytes;
	printk(KERN_INFO "Rec'vd from App data %s of length %d \n",char_device_buf, nbytes);
	return nbytes;
}

static struct file_operations char_dev_fops = {
	//.owner = THIS_MODULE,
	.write = char_dev_write,
	.open = char_dev_open,
	.release = char_dev_release,
	
};

static int __init char_dev_init(void)
{
	int ret,count=1;

	if (alloc_chrdev_region (&mydev, 0, count, CHAR_DEV_NAME) < 0) {
            printk (KERN_ERR "failed to reserve major/minor range\n");
            return -1;
    }

	if (!(my_cdev = cdev_alloc ())) {
		printk (KERN_ERR "cdev_alloc() failed\n");
		unregister_chrdev_region (mydev, count);
		return -1;
	}
	cdev_init(my_cdev,&char_dev_fops);

	ret=cdev_add(my_cdev,mydev,count);
	if( ret < 0 ) {
		printk(KERN_INFO "Error registering device driver\n");
	        cdev_del (my_cdev);
                unregister_chrdev_region (mydev, count); 	
		return -1;
	}
	printk(KERN_INFO"\nDevice Registered: %s\n",CHAR_DEV_NAME);
	printk (KERN_INFO "Major number = %d, Minor number = %d\n", MAJOR (mydev),MINOR (mydev));

	char_device_buf =(char *)kmalloc(MAX_LENGTH,GFP_KERNEL);
	return 0;
}

static void __exit char_dev_exit(void)
{
	cdev_del(my_cdev);
	unregister_chrdev_region(mydev,1);
	printk(KERN_INFO "\n Driver unregistered \n");
}


module_init(char_dev_init);
module_exit(char_dev_exit);


MODULE_AUTHOR("VENU");
MODULE_DESCRIPTION("Dynamic Character Device Driver");
MODULE_LICENSE("GPL");

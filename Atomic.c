#include <linux/module.h>
#include <linux/kernel.h>


#include <linux/kthread.h> // therad 
#include <linux/delay.h>   // sleep 
//#include <linux/sched.h>               //task_struct
 
atomic_t my_global_variable = ATOMIC_INIT(0);


static struct task_struct *char_thread;
static struct task_struct *char_thread1;

int mydata =0;
int my_thread_fun(void *p);



int my_thread_fun(void *p)
{
	while(!kthread_should_stop())
	{		
		//Atomically add one to v
		atomic_inc(&my_global_variable); // single operation ; no much overhead 
		printk(KERN_INFO "my_thread_fun after inc  %lu\n", atomic_read(&my_global_variable)); // 1
		
			
		//atomic_read(&my_global_variable)
		atomic_add(14, &my_global_variable);
		printk(KERN_INFO "my_thread_fun after add   %lu\n", atomic_read(&my_global_variable)); //15

		//Atomically subtract i from v
		atomic_sub(2,&my_global_variable);
		printk(KERN_INFO "my_thread_fun after sub   %lu\n", atomic_read(&my_global_variable)); //13

		//Atomically subtract one from v
		atomic_dec(&my_global_variable);
		printk(KERN_INFO "my_thread_fun after dec   %lu\n", atomic_read(&my_global_variable)); //12
		
		//Atomically set v equal to i
		atomic_set(&my_global_variable, 14);			
		printk(KERN_INFO "my_thread_fun after set    %lu\n", atomic_read(&my_global_variable)); // 14
		
		// Function Atomically subtract i from v and return true if the result is zero; otherwise false 
		int res = atomic_sub_and_test(3, &my_global_variable); // 11
		printk(KERN_INFO "my_thread_fun after sub and test    %lu\n", res); // false -0

		// Function Atomically dec one from v and return true(1) if the result is zero; otherwise false (0)
		res = atomic_dec_and_test(&my_global_variable);
		printk(KERN_INFO "my_thread_fun after dec and test    %lu\n", res);  // false - 0
		printk(KERN_INFO "my_thread_fun after dec and test    %lu\n", atomic_read(&my_global_variable)); // 10
			
		//Atomically add i to v and return true if the result is negative; otherwise false
		res = atomic_add_negative(8,&my_global_variable);
		printk(KERN_INFO "my_thread_fun after add neg    %lu\n", res); //false -0
		printk(KERN_INFO "my_thread_fun after add neg     %lu\n", atomic_read(&my_global_variable)); // 18

		//Atomically increment v by one and return true if the result is zero; false otherwise
		res = atomic_inc_and_test(&my_global_variable);
		printk(KERN_INFO "my_thread_fun after inc and test     %lu\n", res); // false - 0
		printk(KERN_INFO "my_thread_fun after inc and test      %lu\n", atomic_read(&my_global_variable)); // 19

		printk(KERN_INFO "*****************ATOMIC BIT WSIE OPERATORS *************");
		
		unsigned long word = 0; // word - 0000 0000 - 0000 0001 - 0000 0011 - 0000 0111 - 0000 0101 - 0000 0100 - 0000 0110 -0000 0100

		set_bit(0, &word);       /* bit zero is now set (atomically) */
		printk(KERN_INFO "SET BIT      %x\n", word); // 1

		set_bit(1, &word);        /* bit one is now set (atomically) */
		printk(KERN_INFO "SET BIT      %x\n", word); // 3

		set_bit(2, &word);        /* bit two is now set (atomically) */
		printk(KERN_INFO "SET BIT      %x\n", word); // 7

		clear_bit(1, &word);     /* bit one is now unset (atomically) */
		printk(KERN_INFO "CLEAR BIT      %x\n", word); // 5

		change_bit(0, &word);    /* bit zero is flipped; now it is unset (atomically) */
		printk(KERN_INFO "CHANGE BIT      %x\n", word); // 4

		//Atomically set the nr-th bit starting from addr and return the previous value
		res = test_and_set_bit(1, &word);
		printk(KERN_INFO "TEST AND SET    %lu\n", res); // 0
		printk(KERN_INFO "TEST AND SET      %x\n", word); // 6

		//Atomically clear the nr-th bit starting from addr and return the previous value
		res = test_and_clear_bit(1,&word);
		printk(KERN_INFO "TEST AND CLEAR    %lu\n", res); // 1
		printk(KERN_INFO "TEST AND CLEAR      %x\n", word); // 4
		
		//Atomically return the value of the nr-th bit starting from addr
		res = test_bit(2,&word);
		printk(KERN_INFO "TEST BIT    %lu\n", res);   // 2

		msleep(1000);
		
	}
	return 0;	
}

int my_thread_fun1(void *p)
{
	while(!kthread_should_stop())
	{
		
		atomic_inc(&my_global_variable); // when the function is strted running the my_global_variable will increment by 1
		printk(KERN_INFO "my_thread_fun2  %lu\n", atomic_read(&my_global_variable));
		
		msleep(1000);
	}
	return 0;
}


static int __init my_simpledriver_init(void)
{
	printk(KERN_INFO"My sample driver start");

	
	char_thread = kthread_run(my_thread_fun,NULL,"my char thread");
	if(char_thread)
	{
		printk(KERN_INFO" create the thread");
	}
	else 
	{
		printk(KERN_INFO"unable to create the thread");
		
	}

	char_thread1 = kthread_run(my_thread_fun1,NULL,"my char thread1");
	if(char_thread1)
	{
		printk(KERN_INFO" create the thread1");
	}
	else 
	{
		printk(KERN_INFO"unable to create the thread");
		
	}

	return 0;

}


static void __exit my_simpledriver_exit(void)
{
	printk(KERN_INFO"My samle driver exit\n");

	kthread_stop(char_thread);
	kthread_stop(char_thread1);

}
module_init(my_simpledriver_init);
module_exit(my_simpledriver_exit);




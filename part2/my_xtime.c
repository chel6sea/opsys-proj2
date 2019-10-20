#include <linux/init.h>
#include <linux/module.h>
#include <linux/semaphore.h>
#include <linux/list.h>
#include <linux/string.h>
#include <linux/time.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <linux/unaccess.h>

MODULE_LICENSE("GPL");
//MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("Simple module featuring proc read");

#define ENTRY_NAME "timed"
#define PERMS 0644
#define PARENT NULL

static struct file_operations fops;

static char *msg1;
static char *msg2;

static int read_proc;

struct timespec currentKernTime;
struct timespec time;

static int numOfProcs = 0; 

long elapsedSec;
long elapsedNSec;
//long seconds;
//long nanoseconds;

int timeProcOpen(struct inode *sp_inode, struct file *sp_file){
	printk(KERN_INFO "proc called open\n");

//	numOfProcs++;						//adds everytime it is ran

	read_proc = 1;						

	msg1 = kmalloc(sizeof(char)*256,__GFP_RECLAIM | __GFP_IO | __GFP_FS);
	msg2 = kmalloc(sizeof(char)*256,__GFP_RECLAIM | __GFP_IO | __GFP_FS);

	if(msg1 == NULL || msg2 == NULL){
		printk(KERN_WARNING"timeProcOpen");
		return -ENOMEM;
	}

	currentKernTime = current_kernel_time();

	sprintf(msg1, "current time: %ld.%ld\n", currentKernTime.tv_sec, currentKernTime.tv_nsec);


		
//	NEED TO INITIALIZE VARIABLES FOR elapsedSec and elapsedNSEC	
//	Make sure these values aren't negative

	if(currentKernTime.tv_nsec - time.tv_nsec < 0){
		elapsedSec = currentKernTime.tv_sec - time.tv_sec - 1;
		elapsedNSec = currentKernTime.tv_nsec - time.tv_nsec + 1000000000;

	}
	else{
		elapsedSec = currentKernTime.tv_sec - time.tv_sec;
		elapsedNSec = currentKernTime.tv_nsec - time.tv_nsec;
	}
		


//-----------------------Figure out how to get elasped time-----------------------------
//This runs if the proc file is called more than once (otherwise there will be no elapsed time)

	if(numOfProcs >= 1){			
		sprintf(msg2, "elapsed time: %ld.%ld\n", elapsedSec, elapsedNSec);
		strcat(msg1, msg2);
	}	

	numOfProcs++;
	
	time.tv_sec = currentKernTime.tv_sec;
	time.tv_nsec = currentKernTime.tv_nsec;

	return 0;
}


ssizer_t timeProcRead(struct file *sp_file, char __user *buf, size_t size, loff_t *offset){
	int len = strlen(msg1);
	
	read_proc = !read_proc;
	
	if(read_proc){
		return 0;
	}

	printk(KERN_INFO "proc called read\n");
	copy_to_user(buf, msg1, len);
	return len;	
}

int timeProcRelease(struct inode *sp_inode, struct file *sp_file){
	printk(KERN_INFO"proc called release\n");
	kfree(msg1);
	kfree(msg2);
	return 0; 
}



static int timeInit(void){
	printk(KERN_NOTICE"/proc/%s create\n", ENTRY_NAME);
	fops.open = timeProcOpen;
	fops.read = timeProcRead;
	fops.release = timeProcRelease;

	if(!proc_create(ENTRY_NAME, PERMS, NULL, &fops)){
		printk(KERN_WARNING"ERROR! proc_create\n");
		remove_proc_entry(ENTRY_NAME, NULL);
		return -ENOMEM;
	}
	
	return 0;
}


static void timeExit(void){
	remove_proc_entry(ENTRY_NAME, NULL);
	printk(KERN_NOTICE"Removing /proc/%s.\n", ENTRY_NAME);
}




module_init(timeInit);				//need to call initializing function as argument	
module_exit(timeExit);				//need to call exiting function as argument	
	

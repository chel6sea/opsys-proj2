#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/uaccess.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/mutex.h>
#include "elevator.h"

// Module setup
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("elevator module");
#define ENTRY_NAME "elevator"
#define ENTRY_SIZE 1000
#define PERMS 0644
#define PARENT NULL

// Elevator states
#define OFFLINE 0
#define IDLE 1
#define LOADING 2
#define UP 3
#define DOWN 4

// Passenger states
#define ADULT 0
#define CHILD 1
#define ROOM_SERVICE 2
#define BELLHOP 3

#define NUM_TYPES 4
#define MAX_LOAD 15
#define MAX_PASS 10

static struct file_operations fops;
static char *message;
static int read_p;

int elevator_proc_open(struct inode *sp_inode, struct file *sp_file) {
	read_p = 1;
	message = kmalloc(sizeof(char) * ENTRY_SIZE, __GFP_RECLAIM | __GFP_IO | __GFP_FS);
	if (message == NULL) {
		printk(KERN_WARNING "elevator_proc_open");
		return -ENOMEM;
	}

//	add_animal(get_random_int() % NUM_ANIMAL_TYPES);
//	return print_animals();
}

ssize_t elevator_proc_read(struct file *sp_file, char __user *buf, size_t size, loff_t *offset) {
	int len = strlen(message);

	read_p = !read_p;
	if (read_p)
		return 0;

	copy_to_user(buf, message, len);
	return len;
}

int elevator_proc_release(struct inode *sp_inode, struct file *sp_file) {
	kfree(message);
	return 0;
}


int addPeople(int type){
	int weight;
	int floor;
	int passengerUnit;

	Person * pers;

	if(persons.totalNumOfPeople >= MAX_PEOPLE){
		return 0;
	}

	switch(type){
		case PEOPLE_ADULT:
			weight = 2;									//is actually 1
			passengerUnit = 1;	
			//have to set floor equal to something
			break;
		case PEOPLE_CHILD:
			weight = 1;									//is actually 0.5
			passengerUnit = 1;
			//floor initialization
			break;
		case PEOPLE_ROOMSERVICE:
			weight = 4;									//is actually 2
			passengerUnit = 2;
			//floor initialization
			break;
		case PEOPLE_BELLHOP:
			weight = 8;									//is actually 4	
			passengerUnit = 2;
			//floor initialization
			break;
		default:
			return -1;
	}

	pers = kmalloc(sizeof(Person) * 1, __GFP_RECLAIM);
	if(pers == NULL){
		return -ENOMEM;
	}

	pers->weight = weight;
	pers->passengerUnit = passengerUnit;
	//pers->floor = ??????? 

	list_add_tail(&pers->list, &persons.list);

	//persons.totalPeople++;
	persons.totalWeight += weight;
	persons.totalPassengerUnits += passengerUnit;

	return 0;
}


int print_animals(void) {
	int i;
	Animal *a;
	struct list_head *temp;

	char *buf = kmalloc(sizeof(char) * 100, __GFP_RECLAIM);
	if (buf == NULL) {
		printk(KERN_WARNING "print_animals");
		return -ENOMEM;
	}

	/* init message buffer */
	strcpy(message, "");

	/* headers, print to temporary then append to message buffer */
	sprintf(buf, "Total count is: %d\n", animals.total_cnt);       strcat(message, buf);
	sprintf(buf, "Total length is: %d\n", animals.total_length);   strcat(message, buf);
	sprintf(buf, "Total weight is: %d\n", animals.total_weight);   strcat(message, buf);
	sprintf(buf, "Animals seen:\n");                               strcat(message, buf);

	/* print entries */
	i = 0;
	//list_for_each_prev(temp, &animals.list) { /* backwards */
	list_for_each(temp, &animals.list) { /* forwards*/
		a = list_entry(temp, Animal, list);

		/* newline after every 5 entries */
		if (i % 5 == 0 && i > 0)
			strcat(message, "\n");

		sprintf(buf, "%s ", a->name);
		strcat(message, buf);

		i++;
	}

	/* trailing newline to separate file from commands */
	strcat(message, "\n");

	kfree(buf);
	return 0;
}

void delete_animals(int type) {
	struct list_head move_list;
	struct list_head *temp;
	struct list_head *dummy;
	int i;
	Animal *a;

	INIT_LIST_HEAD(&move_list);

	/* move items to a temporary list to illustrate movement */
	//list_for_each_prev_safe(temp, dummy, &animals.list) { /* backwards */
	list_for_each_safe(temp, dummy, &animals.list) { /* forwards */
		a = list_entry(temp, Animal, list);

		if (a->id == type) {
			//list_move(temp, &move_list); /* move to front of list */
			list_move_tail(temp, &move_list); /* move to back of list */
		}

	}

	/* print stats of list to syslog, entry version just as example (not needed here) */
	i = 0;
	//list_for_each_entry_reverse(a, &move_list, list) { /* backwards */
	list_for_each_entry(a, &move_list, list) { /* forwards */
		/* can access a directly e.g. a->id */
		i++;
	}
	printk(KERN_NOTICE "animal type %d had %d entries\n", type, i);

	/* free up memory allocation of Animals */
	//list_for_each_prev_safe(temp, dummy, &move_list) { /* backwards */
	list_for_each_safe(temp, dummy, &move_list) { /* forwards */
		a = list_entry(temp, Animal, list);
		list_del(temp);	/* removes entry from list */
		kfree(a);
	}
}

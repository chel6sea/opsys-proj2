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
#include <linux/kernel.h>
#include "elevator.h"

// Module setup
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("elevator module");
#define ENTRY_NAME "elevator"
#define ENTRY_SIZE 1000
#define PERMS 0644
#define PARENT NULL

#define KFLAGS (__GFP_RECLAIM | __GFP_IO | __GFP_FS)
#define _NR_START_ELEVATOR 335
#define _NR_ISSUE_REQUEST 336
#define _NR_STOP_ELEVATOR 337

// Elevator states
#define OFFLINE 0
#define IDLE 1
#define LOADING 2
#define UP 3
#define DOWN 4

// Passenger states
#define ADULT 1
#define CHILD 2
#define ROOM_SERVICE 3
#define BELLHOP 4

#define NUM_TYPES 4
#define MAX_LOAD 30
#define MAX_PASS 10

static struct file_operations fops;
static char *message;
//static char *elevState;
static int read_p;

Elevator e;
Building b;


int elevator_proc_open(struct inode *sp_inode, struct file *sp_file) {
	read_p = 1;
	message = kmalloc(sizeof(char) * ENTRY_SIZE, __GFP_RECLAIM | __GFP_IO | __GFP_FS);
	if (message == NULL) {
		printk(KERN_WARNING "elevator_proc_open");
		return -ENOMEM;
	}
	return 0;
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


extern long (*STUB_start_elevator)(void);
long my_start_elevator(void) {
        printk("Starting elevator\n");
	if (e.state == OFFLINE){
		e.currentFloor = 1;
		e.next = -1;
		e.state = IDLE;
		e.count = 0;
		e.load = 0;
        	return 0;
	}
	else if (e.state > 0 || e.state <= 4){	// already active elevator
		return 1;
	}
	else
		return -ENOMEM;

}

extern long (*STUB_stop_elevator)(void);
long my_stop_elevator(void) {
        printk("Stopping elevator\n");

	if (e.count == 0){
		e.state = OFFLINE;
		return 0;
	}
	else if (e.count > 0 ){
		//unload remaining passengers
		int i;
		for (i=0; i<e.count; ++i){
			//unload pass
		}
		return 0;

	}
	return 0;
}


extern long (*STUB_issue_request)(int,int,int);
long my_issue_request(int pass_type, int st_floor, int dest_floor){
	Passenger * passenger;
	printk("New issue request: %d, %d to %d\n", pass_type, st_floor, dest_floor);

	if(pass_type < 1 || pass_type > 4)
		return 1;
	if(st_floor < 1 || st_floor > 10)
		return 1;
	if(dest_floor < 1 || dest_floor > 10)
		return 1;


	passenger = kmalloc(sizeof(Passenger), KFLAGS);
	passenger->passenger_type = pass_type;
	passenger->start_floor = st_floor;
	passenger->destination_floor = dest_floor;

	printk("passenger_type: %d, start floor: %d, destination floor: %d\n", passenger->passenger_type, passenger->start_floor, passenger->destination_floor);

	//Need to add locks here???
	list_add_tail(&passenger->passList, &b.waitList);
	//Unlock lock here???

	return 0;
}

void printElevatorState(char * msg){
	sprintf(message, "\n Elevator \n");

	switch(e.state){
		case OFFLINE:
			sprintf(message, "Elevator's movement state: OFFLINE \n");
			break;
		case IDLE:
			sprintf(message, "Elevator's movement state: IDLE\n");
			break;
		case LOADING:
			sprintf(message, "Elevator's movement state: LOADING\n");
			break;
		case UP:
			sprintf(message, "Elevator's movement state: UP\n");
			break;
		case DOWN:
			sprintf(message, "Elevator's movement state: DOWN\n");
			break;
	}

	sprintf(message, "current floor: %d\n", e.currentFloor);
	sprintf(message, "Next floor: %d\n", e.next);
	//sprintf(message, "Current load: %d\n", (e.load/2));

//----------------NEED TO DO THIS-------------------
	//sprintf(message, "Waiting passengers load: \n");
	//sprintf(message, "Current load: %d\nTotal number of passengers: %d\n", e.load, e.count);

}

void printBuildingState(char * msg){
//	Passenger * passenger;
//	struct list_head ptr;
	int j;
//	int currFloor;
	int waitingList[10];			//max 10
	int index = 0;
	for(index = 0; index < 10; index++)
		waitingList[index] = 0;

	//make sure waitList isn't empty
	if(!list_empty(&b.waitList)){
/*		list_for_each(ptr, &passenger->passList){			//iterate through passenger->list
			passenger = list_entry(ptr, Passenger, list);
			currFloor = passenger->start_floor-1;
			waitingList[start_floor]++;
		}
*/	}

        sprintf(message, "\nBuilding Status\n");
	for(j = 0; j < 10; j++)
		sprintf(message, "Floor %d: %d\n", j+1, waitingList[j]);

	sprintf(message, "Total passengers serviced: %d\n", b.serviced);	
}



/*
elevator e attributes:
	int count;
        int load;



	#define MAX_LOAD 15
	#define MAX_PASS 10
*/
//--------------------checks if adding load will be okay for elevator---------------------


//MIGHT HAVE TO CHANGE WEIGHT LOAD NUMBERS FOR THIS

int checkLoad(int type){
	switch(type){
		case ADULT:		
			if(e.load + 2 <= MAX_LOAD && e.count + 1 <= MAX_PASS)
				return !(e.load + 1 == MAX_LOAD);	
			break;
		case CHILD:
			if(e.load + 1 <= MAX_LOAD && e.count + 1 <= MAX_PASS)
				return !(e.load + 1 == MAX_LOAD);
			break;
		case ROOM_SERVICE:
			if(e.load + 4 <= MAX_LOAD && e.count + 2 <= MAX_PASS)
				return !(e.load + 2 == MAX_LOAD);
			break;
		case BELLHOP:
			if(e.load + 8 <= MAX_LOAD && e.count + 2 <= MAX_PASS)	
				return !(e.load + 4 == MAX_LOAD);
			break;
	}

	return 0;
}

void checkFloor(int floor){
	if(!list_empty(&b.waitList))
		loadPassenger(floor);	

	if(!list_empty(&b.waitList))
		unloadPassenger(floor);

}

/*

//these values are not declared yet, hence commented out

int elevatorSchedule()
{
	int nextDir = 0;
        while(!kthread_should_stop()
        {
                switch(movingState)
                {
                        case OFFLINE:
                                break;

                        case IDLE:
                                nextDir = UP;
                                break;
                        case UP:
                                if(currentFloor == 10)
                                {
                                        nextDir = DOWN;
                                        Move(currentFloor - 1);
                                        nextFloor = currentFloor - 1;
                                }
                                else
                                {
                                        nextDir = UP;
                                        nextFloor = currentFloor + 1;
                                        Move(nextFloor);
                                }
                                break;
                        case DOWN:
                                if(currentFloor == 1)
                                {
                                        nextDir = UP;
                                        Move(currentFloor + 1);
                                        nextFloor = currentFloor + 1;
                                }
                                else
                                {
                                        nextDir = DOWN;
                                        Move(currentFloor - 1);
                                        nextFloor = currentFloor - 1;
                                }
                                break;
                        case LOADING:
                                ssleep(1);
                                unload();
                                                                        
                }       



        }
	return nextDir;

}
*/
int Move(int floor)
{
        if(floor == e.currentFloor)
                return 0;
        else
        {
                printk("Moving to floor %d\n", floor);
                ssleep(2);
                e.currentFloor = floor;
                return 1;

        }

}

void loadPassenger(int floor)
{
  //      Passenger * pass;
//	struct list_head ptr;

	unloadPassenger(e.currentFloor);
        
/*        
        list_for_each(ptr, &b.waitList)
        {
                pass = list_entry(ptr, Passenger, list);
                if(pass->start_floor == floor && checkLoad(pass->passenger_type))			//might need to put &pass->passenger_type
                {											//if the elevatorSchedule() values are initialized, add && checkDir() == 0 
			//serviced[pass->start_floor - 1]+= 1;
            												//try this commented out serviced and see if it works?            
			list_move(&pass->passList, &e.rideList);
                        switch(pass->passenger_type)							//might need to put &pass->passenger_type
                        {
                                case ADULT:
                                        e.load+= 2;
                                        e.count+=1;
                                        break;

                                case CHILD:
                                        e.load+=1;
                                        e.count+=1;
                                        break;

                                case ROOM_SERVICE:
                                        e.load+=4;
                                        e.count+=2;
                                        break;
                                
                                case BELLHOP:
                                        e.load+=4;
                                        e.count+=2;
                                        break;
                        }
                }
        }
*/}


void unloadPassenger(int floor)
{
//	Passenger * pass;
//	struct list_head ptr;
/*
        list_for_each(ptr, &e.rideList)
        {
                pass = list_entry(ptr, Passenger, list);
                if(pass->destination_floor == floor)
                {
                        switch(pass->passenger_type)			//might need to put &pass->passenger_type
                        {
                                case ADULT:
                                        e.load-= 2;
                                        e.count-=1;
                                        break;

                                case CHILD:
                                        e.load-=1;
                                        e.count-=1;
                                        break;

                                case ROOM_SERVICE:
                                        e.load-=4;
                                        e.count-=2;
                                        break;

                                case BELLHOP:
                                        e.load-=4;
                                        e.count-=2;
                                        break;
                        }
                        list_del(&pass->list);
                        kfree(pass);
                }
        }
*/}

int checkType(int type)
{
        switch(type)
        {
                case ADULT:
                        return 1;
                        break;
                case CHILD:
                        return 2;
                        break;
                case ROOM_SERVICE:
                        return 3;
                        break;
                case BELLHOP:
                        return 4;
                        break; 
        }
	return 0;
}

/*

this function can not be used until elevatorSchedule() values declared

int checkDir(int floor)
{
	int nextDir = elevatorSchedule(floor);

	if(nextDir == UP)
	{
		if(pass->destination_floor < floor)
			return 1;
		else 
			return 0;
	}
	else if(nextDir == DOWN)
	{
		if(pass->destination_floor > floor)
			return 0;
		else
			return 1;
	}
	else
		break;
}
*/

void elevator_syscalls_create(void){
//        STUB_start_elevator = my_start_elevator;
//        STUB_issue_request = my_issue_request;
//        STUB_stop_elevator = my_stop_elevator;
}

void elevator_syscalls_remove(void){
//        STUB_start_elevator = NULL;
//        STUB_issue_request = NULL;
//        STUB_stop_elevator = NULL;
}


static int elevator_init(void){
	printk(KERN_NOTICE"/proc/%s create\n", ENTRY_NAME);
//	elevator_syscalls_create();


	fops.open = elevator_proc_open;
	fops.read = elevator_proc_read;
	fops.release = elevator_proc_release;


	if(!proc_create(ENTRY_NAME, PERMS, NULL, &fops)){
		printk(KERN_WARNING"ERROR! proc_create\n");
		remove_proc_entry(ENTRY_NAME, NULL);
		return -ENOMEM;
	}

	return 0;
}
static void elevator_exit(void){
	remove_proc_entry(ENTRY_NAME, NULL);
	elevator_syscalls_remove();
	printk(KERN_NOTICE"Removing /proc/%s.\n", ENTRY_NAME);
}


module_init(elevator_init);
module_exit(elevator_exit);

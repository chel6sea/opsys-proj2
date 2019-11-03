#include <linux/list.h>

typedef struct {
        int passenger_type;
        int start_floor;
        int destination_floor;
	struct list_head passList;
} Passenger;

typedef struct {
        int current;
        int next;
        int state;
	int count;
	int load;
	struct list_head rideList;
} Elevator;

typedef struct{
	int serviced;
	struct list_head waitList;
}Building;

int elevator_proc_open(struct inode *sp_inode, struct file *sp_file);
ssize_t elevator_proc_read(struct file *sp_file, char __user *buf, size_t size, loff_t *offset);
int elevator_proc_release(struct inode *sp_inode, struct file *sp_file);

static int elevator_init(void);
static void elevator_exit(void);

int checkLoad(int type);
void unloadPassenger(int floor);
void loadPassenger(int floor);

void printElevatorState(char * msg);
void printBuildingState(char * msg);

void issue_request(int pass_type, int st_floor, int dest_floor);

void elevator_syscalls_create(void);
void elevator_syscalls_remove(void);

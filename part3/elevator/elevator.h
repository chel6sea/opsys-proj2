#include <linux/list.h>

typedef struct {
        int passenger_type;
        int start_floor;
        int destination_floor;
} Passenger;

typedef struct {
        int current;
        int next;
        int state;
} Elevator;

int elevator_proc_open(struct inode *sp_inode, struct file *sp_file);
ssize_t elevator_proc_read(struct file *sp_file, char __user *buf, size_t size, loff_t *offset);
int elevator_proc_release(struct inode *sp_inode, struct file *sp_file);

static int elevator_init(void);
static void elevator_exit(void);

int checkLoad(int type);
void unloadPassenger(int floor);
void loadPassenger(int floor);
void printElevatorState(char * message);
void printBuildingState(char * message);

void elevator_syscalls_create(void);
void elevator_syscalls_remove(void);

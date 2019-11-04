# project 2:

## Division of labor
Betty Tannuzzo - part 2, documentation, part 3: helped with structs; 
		 checkLoad, printElevatorState, printBuildingState,
		 checkFloor

Nicole Garcia -  part 3: loadPassenger, unloadPassenger, helped
		 with structs, etc...

Chelsea Gary - documentation, part 1, kernel compiling and stuff, 
	       part 3: system calls, module and header file


## Contents
part 1: 

- empty.c -- an empty C program
- sys_call.c -- program with exactly 6 system calls
- Makefile -- compiles and prints out the number of lines in each program's sys call log 

part 2:

- my_xtime.c -- kernel module for time between proc calls  
- Makefile -- compiles and prints out the number of seconds 
	      and nanoseconds between proc calls


part 3: 

- elevator.h, elevator.c -- elevator module
- producer.c, consumer.c, wrappers.h -- given files
- sys_start_elevator.c, sys_issue_request.c, sys_stop_elevator.c
- Makefile -- 

## How to compile

part 1: 

	make
    
part 2: 

	make
	sudo insmod my_xtime.ko
	cat /proc/timed

part 3: 

	make
	sudo insmod elevator.ko
	./consumer.x --start
	./producer.x
	./consumer.x --stop
	sudo rmmod elevator.ko

## Bugs/unfinished portions

- system call implementation problem, module is not finding our system calls

		WARNING: "STUB_stop_elevator" [/.../elevator/elevator.ko] undefined!
		WARNING: "STUB_issue_request" [/.../elevator/elevator.ko] undefined!
		WARNING: "STUB_start_elevator" [/.../elevator/elevator.ko] undefined!

- we basically can't really test the elevator scheduling implementation because the system calls are not being recognized by the module



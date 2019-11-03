# project 2:

## Division of labor
Betty Tannuzzo - part 2

Nicole Garcia -

Chelsea Gary - documentation, part 1, kernel compiling and stuff, part 3: system calls, module and header file


## Contents
part 1: 

- empty.c -- an empty C program
- sys_call.c -- program with exactly 6 system calls
- Makefile -- compiles and prints out the number of lines in each program's sys call log 

part 2:

- my_xtime.c -- 
- Makefile -- 


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

## Bugs/unfinished portions



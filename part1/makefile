all: 
	gcc -o empty empty.c
	gcc -o sys_call sys_call.c
	strace -o log1 empty
	strace -o log2 sys_call
	@echo "syscalls in empty.c:" `cat log1 | wc -l`
	@echo "syscalls in sys_call.c:" `cat log2 | wc -l`

clean:
	rm empty sys_call log[1-2]

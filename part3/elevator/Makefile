ifneq ($(KERNELRELEASE),)
	obj-y := sys_start_elevator.o sys_issue_request.o sys_stop_elevator.o
	obj-m := elevator.o
else
	KERNELDIR ?= /lib/modules/`uname -r`/build/
	PWD := `pwd`
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	gcc -o consumer.x consumer.c
	gcc -o producer.x producer.c
endif

clean:
	rm -f *.ko *.o Module* *.mod.* *.order .*cmd *.x

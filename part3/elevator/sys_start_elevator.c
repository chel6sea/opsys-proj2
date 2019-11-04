#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/module.h>

long (*STUB_start_elevator)(void) = NULL;
EXPORT_SYMBOL(STUB_start_elevator);

//SYSCALL_DEFINE0(start_elevator, void) {
//	printk(KERN_NOTICE "Inside SYSCALL_DEFINE1 block.: Your int is\n");

asmlinkage long sys_start_elevator(void) {
	if (STUB_start_elevator)
		return STUB_start_elevator();
	else
		return -ENOSYS;
}

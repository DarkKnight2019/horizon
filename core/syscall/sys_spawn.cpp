#include <syscall/syscall.h>

#include <scheduler/scheduler.h>
#include <elf/elf_loader.h>

#include <utils/log.h>

using namespace syscall;

void syscall::sys_spawn(interrupts::s_registers* regs) {
	char* name = (char*) regs->rbx;
	const char** argv = (const char**) regs->rcx;
	const char** envp = (const char**) regs->rdx;

	scheduler::task_t* task = elf::load_elf(name, argv, envp);
	if (task == nullptr) {
		debugf("Failed to load elf: %s\n", name);
	}

	regs->rax = (uint64_t) task;
}

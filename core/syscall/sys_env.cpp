#include <syscall/syscall.h>

#include <scheduler/scheduler.h>
#include <apic/apic.h>
#include <acpi/acpi.h>
#include <utils/abort.h>

#include <elf/kernel_module.h>

using namespace syscall;

void syscall::sys_env(interrupts::s_registers* regs) {
	LAPIC_ID(core_id);

	scheduler::task_t* task = scheduler::task_queue[core_id]->list[0];

	switch(regs->rbx) {
		case 0:
			{
				regs->rcx = (uint64_t) task->argv;
			}
			break;
		case 1:
			{
				regs->rcx = (uint64_t) task->envp;
			}
			break;
		
		case 2:
			{
				// errno
				debugf("sys_env: errno not implemented\n");
			}
			break;

		case 3:
			{
				scheduler::register_signal_handler_self(regs->rcx, regs->rdx);
			}
			break;
		
		case 4: // set cwd
			{
				scheduler::set_cwd_self((char*) regs->rcx);
			}
			break;
		
		case 5: // get cwd
			{
				regs->rcx = (uint64_t) scheduler::get_cwd_self();
			}
			break;
		
		case 6: // acpi shutdown
			{
				__asm__ __volatile__ ("sti");
				acpi::shutdown();
				__asm__ __volatile__ ("cli");
			}
			break;
		
		case 7: // envp set
			{
				task->envp = (char**) regs->rcx;
			}
			break;
		
		case 8: // pipe enable/disable
			{
				task->pipe_enabled = regs->rcx;
			}
			break;

		case 9: // get task struct
			{
				regs->rcx = (uint64_t) task;
			}
			break;
		
		case 10: // get all running tasks in an array
			{
				scheduler::read_running_tasks((scheduler::task_t**) regs->rdx, regs->rcx);
			}
			break;
		
		case 11: // acpi reboot
			{
				__asm__ __volatile__ ("sti");
				acpi::reboot();
				__asm__ __volatile__ ("cli");
			}
			break;

		case 12: // get all loaded modules
			{
				char** modules = (char**) regs->rdx;
				int max_modules = regs->rcx;

				int offset = 0;
				for (int i = 0; i < max_modules && i < elf::modules->length; i++) {
					if (elf::modules->data[i + offset].taken) {
						debugf("sys_env: module %d: %s\n", i, elf::modules->data[i + offset].data->name);
						modules[i] = (char*) elf::modules->data[i + offset].data->name;
					} else {
						offset++;
						i--;
					}
				}
			}
			break;

		default:
			{
				debugf("sys_env: unknown syscall\n");
			}
			break;
	}
}
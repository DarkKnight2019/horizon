#include <elf/kernel_module.h>

#include <acpi/acpi.h>
#include <acpi/acpi_tables.h>
#include <utils/log.h>
#include <interrupts/interrupt_handler.h>
#include <interrupts/interrupts.h>
#include <boot/boot.h>

extern "C" {
	#include <lai/core.h>
	#include <lai/helpers/sci.h>
}

int get_sci_interrupt() {
	acpi::fadt_table_t* fadt = (acpi::fadt_table_t*) acpi::find_table((char*) "FACP", 0);

	return fadt->sci_interrupt + 0x20;
}

void sci_interrupt_handler(interrupts::s_registers* registers) {
	uint16_t ev = lai_get_sci_event();

	if (ev & ACPI_POWER_BUTTON) {
		debugf("sci: power button\n");
		acpi::shutdown();
	} else if (ev & ACPI_SLEEP_BUTTON) {
		debugf("sci: sleep button\n");
	} else if (ev & ACPI_WAKE) {
		debugf("sci: sleep wake up\n");
	} else {
		debugf("sci: unknown\n");
	}	
}

void init() {
	lai_set_acpi_revision(boot::boot_info.rsdp->revision);

	int sci_interrupt = get_sci_interrupt();
	debugf("sci_interrupt: %d\n", sci_interrupt);

	interrupts::register_interrupt_handler(sci_interrupt, sci_interrupt_handler);

	lai_create_namespace();
	lai_enable_acpi(0);
}

define_module("laihost", init, null_ptr_func, null_ptr_func);
#include <stivale2.h>

#include <output/stivale2_terminal.h>
#include <output/serial_port.h>

#include <renderer/renderer.h>
#include <renderer/render2d.h>
#include <renderer/font_renderer.h>

#include <gdt.h>
#include <memory/memory.h>

#include <utils/log.h>

extern uint8_t logo[];

extern "C" void main() {

	output::stivale2_terminal terminal = output::stivale2_terminal();
	output::global_terminal = &terminal;

	output::serial_port serial_port = output::serial_port(COM1);
	output::global_serial_port = &serial_port;

	debugf("Hello, world!\n");
	printf("Hello, world!\n");

	setup_gdt();
	memory::prepare_memory(global_bootinfo);
	renderer::setup(global_bootinfo);

	renderer::global_renderer_2d->load_bitmap(logo, 0);

	renderer::global_font_renderer->putstring("Welcome to FoxOS ");
	renderer::global_font_renderer->set_color(0xffffff00);
	renderer::global_font_renderer->putstring("Horizon");
	renderer::global_font_renderer->reset_color();
	renderer::global_font_renderer->putstring("!");

	while(1) {
		__asm__ __volatile__("hlt");
	}
}
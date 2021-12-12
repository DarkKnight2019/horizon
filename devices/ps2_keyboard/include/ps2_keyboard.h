#pragma once

#include <driver/driver.h>
#include <input/input.h>
#include <interrupts/interrupt_handler.h>
#include <utils/port.h>
#include <fs/dev_fs.h>
#include <ps2_layout.h>

namespace ps2 {
	enum special_key {
			left_shift,
			left_ctrl,
			left_alt,
			left_gui,

			right_shift,
			right_control,
			right_alt,
			right_gui,
			
			caps_lock,
			num_lock,
			scroll_lock,

			up_arrow,
			down_arrow,
			left_arrow,
			right_arrow,
	};

	class ps2_keyboard : public driver::device_driver, public input::input_device, public interrupts::interrupt_handler, public fs::dev_fs_file {
		public:
			ps2_keyboard();

			virtual void activate();
			virtual bool is_presend();
			virtual char* get_name();

			virtual char getchar();

			virtual void special_key_down(special_key key);
			virtual void special_key_up(special_key key);

			virtual void handle();

			char current_char;

			keymap_layout current_layout;
			bool keyboard_debug = false;

			virtual void read(fs::vfs::file_t* file, void* buffer, size_t size, size_t offset);
			virtual void write(fs::vfs::file_t* file, void* buffer, size_t size, size_t offset);
		
		private:
			bool l_shift = false;
			bool r_shift = false;
			bool caps_lock = false;
			Port8Bit dataport;
			Port8Bit commandport;
	};
}
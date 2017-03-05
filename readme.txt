Code is tested with Teensy 2.0 @16MHz

--Setup--

https://www.pjrc.com/teensy/gcc.html

-- Unit Tests --

Tested with msvc2008.
src\test\test.sln

-- Contents --

usb_keyboard_mouse_debug.c
usb_keyboard_mouse_debug.h
    A Library supporting HID keyboard, mouse and COM profiles
	COM is compatible with https://www.pjrc.com/teensy/hid_listen.html
	Code based on https://www.pjrc.com/teensy/usb_keyboard.html
	
adns9800_firmware.c
adns9800.c
adns9800.h
	Mouse sensor support.
	Code based on https://github.com/pepijndevos/Dwergmuis

controller.c
	Main logic

main.c
	Platform specific entry point

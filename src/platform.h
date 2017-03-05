#ifndef _32xe_platform_h_
#define _32xe_platform_h_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "usb_keyboard_mouse_debug.h"
#include "print.h"

void init(void);
void loop_step(void);

#endif //_32xe_platform_h_

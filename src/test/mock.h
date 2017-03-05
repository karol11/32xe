#ifndef _32xe_mock_h_
#define _32xe_mock_h_

#include "my_int.h"

#define PROGMEM
byte pgm_read_byte(const byte* ptr) { return *ptr; }

extern byte PORTB, PORTC, PORTD, PORTF;
extern byte DDRB, DDRC, DDRD, DDRF;

#define IND_B 0
#define IND_C 1
#define IND_D 2
#define IND_F 3

#define PINB read_pin(IND_B)
#define PINC read_pin(IND_C)
#define PIND read_pin(IND_D)
#define PINF read_pin(IND_F)

extern byte keyboard_keys[6];
extern byte keyboard_modifier_keys;
extern byte volatile keyboard_leds;
extern byte mouse_buttons;

byte read_pin(int pin);

#endif

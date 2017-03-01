#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "usb_keyboard_mouse_debug.h"
#include "print.h"

#define CPU_PRESCALE(n)	(CLKPR = 0x80, CLKPR = (n))

typedef uint8_t  byte;
typedef int8_t   sbyte;
typedef uint16_t word;
typedef int16_t  sword;
typedef uint8_t  bool;
#define false 0
#define true 1

void adns_motion(sword* dx, sword* dy);
void adns_init(void);
void reflash(void);

#define KEY_SPECIAL 245
#define IMM_CTRL   245
#define IMM_ALT    247
#define IMM_SHIFT  248

#define MB_GAME  250

#define MB_LEFT  251
#define MB_MID   252
#define MB_RIGHT 253
#define MB_WHEEL 254

// TODO:
// + wheel
// + mouse mode
// + double tap repeat
// + game mode

//
// --- keymap
//
// rrsfff
const byte PROGMEM basic_keys[] = {
	KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T, KEY_SPACE, 0, 0,  KEY_P,         KEY_O,      KEY_I,     KEY_U, KEY_Y, KEY_ENTER, 0, 0,         
	KEY_A, KEY_S, KEY_D, KEY_F, KEY_G, KEY_SPACE, 0, 0,  KEY_SEMICOLON, KEY_L,      KEY_K,     KEY_J, KEY_H, KEY_ENTER, 0, 0,         
	KEY_Z, KEY_X, KEY_C, KEY_V, KEY_B, KEY_SPACE, 0, 0,  KEY_SLASH,     KEY_PERIOD, KEY_COMMA, KEY_M, KEY_N, KEY_ENTER, 0, 0, 
};

const byte PROGMEM numpad_keys[] = {
	KEY_ESC,   KEY_7, KEY_8, KEY_9, KEY_0,     KEY_SPACE, 0, 0,   KEYPAD_SLASH,  KEYPAD_9, KEYPAD_8, KEYPAD_7, KEYPAD_ASTERIX, KEYPAD_ENTER, 0, 0, 
	KEY_TAB,   KEY_4, KEY_5, KEY_6, KEY_EQUAL, KEY_SPACE, 0, 0,   KEYPAD_MINUS,  KEYPAD_6, KEYPAD_5, KEYPAD_4, KEYPAD_PLUS,    KEYPAD_ENTER, 0, 0, 
	KEY_TILDE, KEY_1, KEY_2, KEY_3, KEY_MINUS, KEY_SPACE, 0, 0,   KEYPAD_PERIOD, KEYPAD_3, KEYPAD_2, KEYPAD_1, KEYPAD_0,       KEYPAD_ENTER, 0, 0, 
};

const byte PROGMEM numeric_mouse_keys[] = {
	KEY_ESC,   KEY_7, KEY_8, KEY_9, KEY_0,     0, 0, 0,   KEY_BACKSPACE, MB_MID,     MB_RIGHT,   MB_LEFT,  KEY_PRINTSCREEN, 0, 0, 0, 
	KEY_TAB,   KEY_4, KEY_5, KEY_6, KEY_EQUAL, 0, 0, 0,   KEY_QUOTE,     0,          0,          0,        KEY_MENU,        0, 0, 0, 
	KEY_TILDE, KEY_1, KEY_2, KEY_3, KEY_MINUS, 0, 0, 0,   MB_GAME,       KEY_VOL_DN, KEY_VOL_UP, KEY_MUTE, 0,               0, 0, 0, 
};

const byte PROGMEM arrow_keys[] = {
	KEY_NUM_LOCK,    KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_PAUSE, 0, 0,   KEY_BACKSPACE, KEY_PAGE_DOWN,   KEY_UP,         KEY_PAGE_UP, KEY_HOME,   0, 0, 0, 
	KEY_CAPS_LOCK,   KEY_F4, KEY_F5, KEY_F6, KEY_F11, KEY_PAUSE, 0, 0,   KEY_QUOTE,     KEY_RIGHT,       KEY_DOWN,       KEY_LEFT,    KEY_END,    0, 0, 0, 
	KEY_SCROLL_LOCK, KEY_F1, KEY_F2, KEY_F3, KEY_F12, KEY_PAUSE, 0, 0,   KEY_BACKSLASH, KEY_RIGHT_BRACE, KEY_LEFT_BRACE, KEY_DELETE,  KEY_INSERT, 0, 0, 0, 
};

const byte PROGMEM imm_game_mode[] = {
	KEY_TAB,   KEY_Q,   KEY_W, KEY_E, KEY_R, 0,         0, 0,   MB_WHEEL, MB_MID,    MB_RIGHT, MB_LEFT, KEY_TILDE, KEY_ESC, 0, 0, 
	IMM_SHIFT, KEY_A,   KEY_S, KEY_D, KEY_F, 0,         0, 0,   KEY_5,    KEY_4,     KEY_3,    KEY_2,   KEY_1,     KEY_ESC, 0, 0, 
	IMM_CTRL,  IMM_ALT, KEY_X, KEY_C, KEY_V, KEY_SPACE, 0, 0,   MB_GAME,  KEY_RIGHT, KEY_DOWN, KEY_UP,  KEY_LEFT,  KEY_ESC, 0, 0, 
};

const byte PROGMEM main_shift_keys[] = {
	KEY_LEFT_SHIFT, KEY_LEFT_GUI, KEY_LEFT_ALT, KEY_LEFT_CTRL, 0, 0, 0, 0,
};
const byte PROGMEM bottom_shift_keys[] = {
	KEY_RIGHT_CTRL, KEY_RIGHT_GUI, KEY_RIGHT_ALT, KEY_RIGHT_SHIFT, 0, 0, 0, 0,
};


typedef struct key_info_struct{
	byte prev_state;
	byte bounce_counter;
	byte ignore_release;
} key_info_t;

key_info_t keys[16*3];
byte pressed_keys[32];
byte pressed_count;

bool game_mode = false;
bool game_wheel = false;
byte shifts_to_send = 0;
byte mb_to_send = 0;
byte autorepeat_key = 0;
word delay_since_released = 0;
word autorepeat_counter = 0;

void drop_pressed_keys(void) {
	{
		key_info_t* i = keys;
		for (; i < keys + 16*3; i++)
			i->ignore_release = true;
	}
	pressed_count = 0;
	mb_to_send = 0;
	shifts_to_send = 0;
	for (byte i = 0; i < 6; i++)
		keyboard_keys[i] = 0;
	autorepeat_key = 0;
	autorepeat_counter = 0;
}

byte to_shift(byte index) {
	byte row = index & 0x30;
	if (row == 0x30) return 0;
	return pgm_read_byte((row == 0x20 ? bottom_shift_keys : main_shift_keys) + (index & 7));
}

sbyte clamp_to_sbyte(sword v) {
	return
		v < -128 ? -128 :
		v > 127 ? 127 : v;
}

byte get_key_code_and_shifts(byte pos, byte index) {
	const byte *plane = (keyboard_leds & KLED_NUM_LOCK) == 0 ? basic_keys : numpad_keys;
	shifts_to_send = 0;
	while (--pos >= 0) {
		byte s = pressed_keys[pos];
		if ((s & 7) == 5)
			plane = (s & 8) ? arrow_keys : numeric_mouse_keys;
		else if ((s & 8) != (index & 8))
			shifts_to_send |= to_shift(s);
		else
			continue;
		keys[s].ignore_release = true;
	}
	if (game_mode)
		plane = imm_game_mode;
	byte key = pgm_read_byte(plane + index);
	if (plane == numpad_keys && key >= KEY_1 && key <= KEY_0)
		shifts_to_send ^= KEY_LEFT_SHIFT;
	return key;
} 

void handleKey(byte index, byte port, byte mask) {
	key_info_t* k = keys + index;
	if (k->bounce_counter > 0) {
		k->bounce_counter--;
		return;
	}
	port &= mask;
	if (port == k->prev_state)
		return;
	k->prev_state = port;
	k->bounce_counter = 2;
	if (port == 0) { // pressed
		if (pressed_count >= 32)
			pressed_count = 0;
		pressed_keys[pressed_count++] = index;
		k->ignore_release = false;
		byte key_code = get_key_code_and_shifts(pressed_count, index);
		if (key_code == MB_WHEEL)
			game_wheel = true;
		else if (key_code >= MB_LEFT && key_code <= MB_RIGHT)
			mb_to_send |= 1 << (key_code - MB_LEFT);
		if (key_code == autorepeat_key)
			autorepeat_counter = delay_since_released;

	} else {
		//print("\n released="); phex(index);
		sbyte i = pressed_count;
		for (;;) {
			if (--i < 0)
				return;
			if (pressed_keys[i] == index)
				break;
		}
		{
			byte j = i;
			while (++j < pressed_count)
				pressed_keys[j-1] = pressed_keys[j];
			pressed_count--;
		}
		if (k->ignore_release && !game_mode) {
		    shifts_to_send &= ~to_shift(index);
			return;
		}
		byte key_code = get_key_code_and_shifts(i, index);
		if (key_code == MB_WHEEL)
			game_wheel = false;
		else if (key_code >= MB_LEFT && key_code <= MB_RIGHT)
			mb_to_send &= ~(1 << (key_code - MB_LEFT));
		else if (key_code == MB_GAME) {
			game_mode = !game_mode;
			drop_pressed_keys();
			if (!game_mode)
				game_wheel = false;
		} else if (key_code < KEY_SPECIAL && !game_mode) {
			usb_keyboard_press(key_code);
			if (!game_mode) {
				autorepeat_key = key_code;
				delay_since_released = autorepeat_counter = 0;
			}
		}
	}
}

//
// --- pins
//
// hor: b0 - upper, b1-mid, b2 lower
// vert: b3- b7 d0 f6 f7- b6 b5 b4 (d1 d2 d3 c6)
// (c6 d3 d2 d1) d0 b4    b6 b5 (f7 f6 b7 b3)
//b 11111111
//d 00001111
//f 11000000
//c 01000000
void scanRow(byte row) {
	byte b = PINB;
	byte d = PIND;
	byte f = PINF;
	byte c = PINC;
	handleKey(row | 0, c, 1 << 6);
	handleKey(row | 1, d, 1 << 3);
	handleKey(row | 2, d, 1 << 2);
	handleKey(row | 3, d, 1 << 1);
	handleKey(row | 4, d, 1 << 0);
	handleKey(row | 5, b, 1 << 4);
	
	handleKey(row | 8, b, 1 << 3);
	handleKey(row | 9, b, 1 << 7);
	handleKey(row | 10, f, 1 << 6);
	handleKey(row | 11, f, 1 << 7);
	handleKey(row | 12, b, 1 << 5);
	handleKey(row | 13, b, 1 << 6);

	//print(" b="); phex(b & ~7);
	//print(" d="); phex(d);
	//print(" f="); phex(f);
	//print(" c="); phex(c);
	if (!game_mode)
		return;
	byte keys_to_send = 0;
	keyboard_modifier_keys = 0;
	for (byte i = 0; i < pressed_count; i++) {
		byte key = pgm_read_byte(imm_game_mode + i);
		if (key < KEY_SPECIAL){
			if (keys_to_send < 6)
				keyboard_keys[keys_to_send++] = key;
		} else if (key >= IMM_CTRL && key <= IMM_SHIFT)
			keyboard_modifier_keys |= pgm_read_byte(bottom_shift_keys + (key - IMM_CTRL));
	}
	while (keys_to_send < 6)
		keyboard_keys[keys_to_send++] = 0;
	usb_keyboard_send();
	shifts_to_send = keyboard_modifier_keys;
}
//
//  ------------------------- MAIN -----------------------
//
int main(void)
{
	CPU_PRESCALE(0);
	usb_init();
	while (!usb_configured()) /* wait */ ;

	DDRB = 7;
	DDRC = DDRD = 0;
	DDRF = 0x13;
	PORTB = 0xf8;
	PORTD = 0xef;
	PORTF = 0xdf;
	PORTC = 0xff;
	{
		key_info_t* i = keys;
		for (; i < keys + 16*3; i++) {
			i->prev_state = 0;
			i->bounce_counter = 0;
			i->ignore_release = true;
		}
	}
	//print(" b="); phex(PINB);
	//print(" d="); phex(PIND);
	//print(" f="); phex(PINF);
	//print(" c="); phex(PINC);
	if (((PINB | 7) & (PIND | 0xf0) & (PINF | 0x3f) & (PINC | 0xbf)) != 0xff)
		reflash();
	PORTB = 0xff;
	_delay_ms(2000);
	adns_init();
	pressed_count = 0;
	for (;;)
	{
		sword dx;
		sword dy;
		PORTB = ~1;
		scanRow(0);
		PORTB = ~2;
		scanRow(16);
		PORTB = ~4;
		scanRow(32);
		if (shifts_to_send != keyboard_modifier_keys) {
			usb_keyboard_press(0);
			shifts_to_send = keyboard_modifier_keys;
		}
		adns_motion(&dx, &dy);
		if (dx | dy) {
			mouse_buttons = mb_to_send;
			if ((game_mode && !game_wheel) || keys[16+5].prev_state == 0)
				usb_mouse_move(clamp_to_sbyte(dx), clamp_to_sbyte(-dy), 0);
			else
				usb_mouse_move(0, 0, clamp_to_sbyte((dx - dy) / 4));
 		} else if (mouse_buttons != mb_to_send) {
			mouse_buttons = mb_to_send;
			usb_mouse_move(0, 0, 0);		
		}
		if (autorepeat_counter) {
			if (--autorepeat_counter == 0) {
				autorepeat_counter = delay_since_released;
				usb_keyboard_press(autorepeat_key);
			}
		} else if (delay_since_released < 0xffff)
			delay_since_released++;
	}
}

void reflash(void) {
	cli();
	// disable watchdog, if enabled
	// disable all peripherals
	UDCON = 1;
	USBCON = (1<<FRZCLK);  // disable USB
	UCSR1B = 0;
	_delay_ms(5);
#if defined(__AVR_AT90USB162__)                // Teensy 1.0
    EIMSK = 0; PCICR = 0; SPCR = 0; ACSR = 0; EECR = 0;
    TIMSK0 = 0; TIMSK1 = 0; UCSR1B = 0;
    DDRB = 0; DDRC = 0; DDRD = 0;
    PORTB = 0; PORTC = 0; PORTD = 0;
    asm volatile("jmp 0x3E00");
#elif defined(__AVR_ATmega32U4__)              // Teensy 2.0
    EIMSK = 0; PCICR = 0; SPCR = 0; ACSR = 0; EECR = 0; ADCSRA = 0;
    TIMSK0 = 0; TIMSK1 = 0; TIMSK3 = 0; TIMSK4 = 0; UCSR1B = 0; TWCR = 0;
    DDRB = 0; DDRC = 0; DDRD = 0; DDRE = 0; DDRF = 0; TWCR = 0;
    PORTB = 0; PORTC = 0; PORTD = 0; PORTE = 0; PORTF = 0;
    asm volatile("jmp 0x7E00");
#elif defined(__AVR_AT90USB646__)              // Teensy++ 1.0
    EIMSK = 0; PCICR = 0; SPCR = 0; ACSR = 0; EECR = 0; ADCSRA = 0;
    TIMSK0 = 0; TIMSK1 = 0; TIMSK2 = 0; TIMSK3 = 0; UCSR1B = 0; TWCR = 0;
    DDRA = 0; DDRB = 0; DDRC = 0; DDRD = 0; DDRE = 0; DDRF = 0;
    PORTA = 0; PORTB = 0; PORTC = 0; PORTD = 0; PORTE = 0; PORTF = 0;
    asm volatile("jmp 0xFC00");
#elif defined(__AVR_AT90USB1286__)             // Teensy++ 2.0
    EIMSK = 0; PCICR = 0; SPCR = 0; ACSR = 0; EECR = 0; ADCSRA = 0;
    TIMSK0 = 0; TIMSK1 = 0; TIMSK2 = 0; TIMSK3 = 0; UCSR1B = 0; TWCR = 0;
    DDRA = 0; DDRB = 0; DDRC = 0; DDRD = 0; DDRE = 0; DDRF = 0;
    PORTA = 0; PORTB = 0; PORTC = 0; PORTD = 0; PORTE = 0; PORTF = 0;
    asm volatile("jmp 0x1FC00");
#endif 
}

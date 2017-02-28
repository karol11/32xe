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
#define IMM_SHIFT  245
#define IMM_GUI    246
#define IMM_ALT    247
#define IMM_CTRL   248
#define IMM_RALT   249

#define MB_GAME  250

#define MB_LEFT  251
#define MB_MID   252
#define MB_RIGHT 253
#define MB_WHEEL 254
#define MB_LOCK  255

// TODO:
// + remove delay
// - key debounce
// - wheel
// - mouse mode
// - double tap repeat
// + no shifts autorelease
// + num-lock mode
// - game mode

//
// --- keymap
//
// rrsfff
const byte PROGMEM basic_keys[] = {
	KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T, KEY_SPACE, 0, 0,  KEY_P,         KEY_O,      KEY_I,     KEY_U, KEY_Y, KEY_ENTER, 0, 0,         
	KEY_A, KEY_S, KEY_D, KEY_F, KEY_G, KEY_SPACE, 0, 0,  KEY_SEMICOLON, KEY_L,      KEY_K,     KEY_J, KEY_H, KEY_ENTER, 0, 0,         
	KEY_Z, KEY_X, KEY_C, KEY_V, KEY_B, KEY_SPACE, 0, 0,  KEY_SLASH,     KEY_PERIOD, KEY_COMMA, KEY_M, KEY_N, KEY_ENTER, 0, 0, 
};

const byte PROGMEM numeric_mouse_keys[] = {
	KEY_ESC,   KEY_7, KEY_8, KEY_9, KEY_0,     0, 0, 0,   MB_WHEEL, MB_MID,     MB_RIGHT,   MB_LEFT,  KEY_PRINTSCREEN, MB_LOCK, 0, 0, 
	KEY_TAB,   KEY_4, KEY_5, KEY_6, KEY_EQUAL, 0, 0, 0,   MB_WHEEL, MB_MID,     MB_RIGHT,   MB_LEFT,  KEY_MENU,        MB_LOCK, 0, 0, 
	KEY_TILDE, KEY_1, KEY_2, KEY_3, KEY_MINUS, 0, 0, 0,   MB_GAME,  KEY_VOL_DN, KEY_VOL_UP, KEY_MUTE, 0,               MB_LOCK, 0, 0, 
};

const byte PROGMEM numpad_keys[] = {
	KEY_NUM_LOCK, KEY_7, KEY_8, KEY_9, KEY_0,     0, 0, 0,   KEYPAD_SLASH,  KEYPAD_9, KEYPAD_8, KEYPAD_7, KEYPAD_ASTERIX, KEYPAD_ENTER, 0, 0, 
	KEY_TAB,      KEY_4, KEY_5, KEY_6, KEY_EQUAL, 0, 0, 0,   KEYPAD_MINUS,  KEYPAD_6, KEYPAD_5, KEYPAD_4, KEYPAD_PLUS,    KEYPAD_ENTER, 0, 0, 
	KEY_TILDE,    KEY_1, KEY_2, KEY_3, KEY_MINUS, 0, 0, 0,   KEYPAD_PERIOD, KEYPAD_3, KEYPAD_2, KEYPAD_1, KEYPAD_0,       KEYPAD_ENTER, 0, 0, 
};

const byte PROGMEM arrow_keys[] = {
	KEY_NUM_LOCK,    KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_PAUSE, 0, 0,   KEY_BACKSPACE, KEY_PAGE_DOWN,   KEY_UP,         KEY_PAGE_UP, KEY_HOME,   0, 0, 0, 
	KEY_CAPS_LOCK,   KEY_F4, KEY_F5, KEY_F6, KEY_F11, KEY_PAUSE, 0, 0,   KEY_QUOTE,     KEY_RIGHT,       KEY_DOWN,       KEY_LEFT,    KEY_END,    0, 0, 0, 
	KEY_SCROLL_LOCK, KEY_F1, KEY_F2, KEY_F3, KEY_F12, KEY_PAUSE, 0, 0,   KEY_BACKSLASH, KEY_RIGHT_BRACE, KEY_LEFT_BRACE, KEY_DELETE,  KEY_INSERT, 0, 0, 0, 
};

const byte PROGMEM imm_mouse_mode[] = {
	IMM_SHIFT, IMM_GUI, IMM_ALT, IMM_CTRL,  IMM_RALT, 0, 0, 0,   MB_WHEEL, MB_MID, MB_RIGHT, MB_LEFT, 0, MB_LOCK, 0, 0, 
	IMM_SHIFT, IMM_GUI, IMM_ALT, IMM_CTRL,  IMM_RALT, 0, 0, 0,   MB_WHEEL, MB_MID, MB_RIGHT, MB_LEFT, 0, MB_LOCK, 0, 0, 
	IMM_CTRL,  IMM_GUI, IMM_ALT, IMM_SHIFT, IMM_RALT, 0, 0, 0,   MB_WHEEL, 0,      0,        0,       0, MB_LOCK, 0, 0, 
};


const byte PROGMEM imm_game_mode[] = {
	KEY_TAB,   KEY_Q,   KEY_W, KEY_E, KEY_R, 0,         0, 0,   KEY_5,    KEY_4,     KEY_3,    KEY_2,   KEY_1,     KEY_ESC, 0, 0, 
	IMM_SHIFT, KEY_A,   KEY_S, KEY_D, KEY_F, 0,         0, 0,   MB_WHEEL, MB_MID,    MB_RIGHT, MB_LEFT, KEY_TILDE, KEY_ESC, 0, 0, 
	IMM_CTRL,  IMM_ALT, KEY_X, KEY_C, KEY_V, KEY_SPACE, 0, 0,   MB_GAME,  KEY_RIGHT, KEY_DOWN, KEY_UP,  KEY_LEFT,  KEY_ESC, 0, 0, 
};

const byte PROGMEM main_shift_keys[] = {
	KEY_SHIFT, KEY_GUI, KEY_ALT, KEY_CTRL, KEY_RIGHT_ALT, 0, 0, 0,
};
const byte PROGMEM bottom_shift_keys[] = {
	KEY_CTRL, KEY_GUI, KEY_ALT, KEY_SHIFT, KEY_RIGHT_ALT, 0, 0, 0,
};

/*

numlock (replaces main if numlock)
E&*()   *789+	Esc
T$%^=   /456-	Tab
`!@#-_ E0123.	numpadEnter

game mode (immediate)  enter-exit: left thumb+GamingMode
Eqwer   12345	Esc
Tasdf   [LMR]	Tab Left Middle Right mouse
SCAcx_ E`<^v>   Shift Control Alt Enter

*/

typedef struct key_info_struct{
	byte prev_state;
	byte bounce_counter;
	byte ignore_release;
} key_info_t;

key_info_t keys[16*3];
byte pressed_keys[32];
byte pressed_count;

bool mouse_locked = false;
const byte *immediate_mode = 0;

byte last_pressed_key;
byte last_pressed_timer;
byte last_released_key;
byte last_released_timer;

void set_immediate_mode(const byte *new_mode) {
	if (new_mode == immediate_mode)
		return;
	immediate_mode = new_mode;
	{
		key_info_t* i = keys;
		for (; i < keys + 16*3; i++)
			i->ignore_release = true;
	}
	pressed_count = 0;
}

byte toShift(byte index) {
	return pgm_read_byte(
		((index & 0x30) == 0x20 ?
			bottom_shift_keys :
			main_shift_keys) +
		(index & 7));
}

// ; Flamberg rulez!
// toShift = : i
//	 pgm_read_byte
//		 add
//			 if i & 0x30 == 0x20
//				 :bottom_shift_keys
//				 :main_shift_keys
//			 i & 7

sbyte clamp_to_sbyte(sbyte v) {
	return
		v < -128 ? -128 :
		v > 127 ? 127 : v;
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
	//k->bounce_counter = 0;
	if (port == 0) { // pressed
		if (pressed_count >= 32)
			pressed_count = 0;
		pressed_keys[pressed_count++] = index;
		k->ignore_release = false;
		last_pressed_key = index;
		last_pressed_timer = last_released_key == index ? 1 : 200;
		last_released_key = 0;
	} else {
		print("\n released="); phex(index);
		sbyte i = pressed_count;
		for (;;) {
			if (--i < 0)
				return;
			if (pressed_keys[i] == index)
				break;
		}
		{
			print("\n found="); phex(i);
			byte j = i;
			while (++j < pressed_count)
				pressed_keys[j-1] = pressed_keys[j];
			pressed_count--;
		}
		if (k->ignore_release) {
		    keyboard_modifier_keys &= ~toShift(index);
			return;
		}
		last_released_key = index;
		last_released_timer = 100;
		const byte *plane = keyboard_leds & KLED_NUM_LOCK ? basic_keys : numpad_keys;
		byte shift_mask = 0;
		while (--i >= 0) {
			byte s = pressed_keys[i];
			if ((s & 7) == 5)
				plane = (s & 8) ? arrow_keys : numeric_mouse_keys;
			else if ((s & 8) != (index & 8))
				shift_mask |= toShift(s);
			else
				continue;
			keys[s].ignore_release = true;
		}
		byte key_code = pgm_read_byte(plane + index);
		if (key_code <  KEY_SPECIAL)
			usb_keyboard_press(key_code, shift_mask);
		else if (key_code >= MB_LEFT) {
			set_immediate_mode(imm_mouse_mode);
			if (key_code == MB_LOCK)
				mouse_locked = true;
			else
				mouse_buttons |= 1 << (key_code - MB_LEFT);
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
		byte prev_shifts_mask = keyboard_modifier_keys;
		PORTB = ~1;
		scanRow(0);
		PORTB = ~2;
		scanRow(16);
		PORTB = ~4;
		scanRow(32);
		if (prev_shifts_mask != keyboard_modifier_keys)
			usb_keyboard_press(0, keyboard_modifier_keys);
		//print("\n");
		if (last_pressed_timer && --last_pressed_timer == 0) {
			// TODO: autorepeat(last_pressed_key);
			last_pressed_timer = 100;
		}
		if (last_released_timer && --last_released_timer == 0)
			last_released_key = 0;
		adns_motion(&dx, &dy);
		if (dx | dy) {
			if (immediate_mode == 0 && keys[16+5].prev_state == 0)
				set_immediate_mode(imm_mouse_mode);
			if (immediate_mode == 0)
				usb_mouse_move(0, 0, clamp_to_sbyte(dx + dy));
			else
				usb_mouse_move(clamp_to_sbyte(dx), clamp_to_sbyte(dy), 0);
 		}
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

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

// TODO:
// - remove delay
// - key debounce
// - wheel
// - mouse mode
// - hold repeat
// - double tap repeat
// - no shifts autorelease
// - num-lock mode
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

const byte PROGMEM numeric_keys[] = {
	KEY_ESC,   KEY_7, KEY_8, KEY_9, KEY_0,     0, 0, 0,   KEYPAD_SLASH,  KEYPAD_9, KEYPAD_8, KEYPAD_7, KEYPAD_ASTERIX, KEYPAD_ENTER, 0, 0, 
	KEY_TAB,   KEY_4, KEY_5, KEY_6, KEY_EQUAL, 0, 0, 0,   KEYPAD_MINUS,  KEYPAD_6, KEYPAD_5, KEYPAD_4, KEYPAD_PLUS,    KEYPAD_ENTER, 0, 0, 
	KEY_TILDE, KEY_1, KEY_2, KEY_3, KEY_MINUS, 0, 0, 0,   KEYPAD_PERIOD, KEYPAD_3, KEYPAD_2, KEYPAD_1, KEYPAD_0,       KEYPAD_ENTER, 0, 0, 
};

const byte PROGMEM arrow_keys[] = {
	KEY_NUM_LOCK,    KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_PAUSE, 0, 0,   KEY_BACKSPACE, KEY_PAGE_DOWN,   KEY_UP,         KEY_PAGE_UP, KEY_HOME,   0, 0, 0, 
	KEY_CAPS_LOCK,   KEY_F4, KEY_F5, KEY_F6, KEY_F11, KEY_PAUSE, 0, 0,   KEY_QUOTE,     KEY_RIGHT,       KEY_DOWN,       KEY_LEFT,    KEY_END,    0, 0, 0, 
	KEY_SCROLL_LOCK, KEY_F1, KEY_F2, KEY_F3, KEY_F12, KEY_PAUSE, 0, 0,   KEY_BACKSLASH, KEY_RIGHT_BRACE, KEY_LEFT_BRACE, KEY_DELETE,  KEY_INSERT, 0, 0, 0, 
};

/*
main
qwert   yuiop
asdfg   hjkl;
zxcvb_ ¶nm,./

digits+mouse (hold left thumb)
E7890   MLMRW  Esc MenuKey Left Middle Right Wheel
T456=   PLMRW  Tab PrintScreen 
`123-O LGLMRW  LockMouse GamingMode 

arrows+Fn (hold right thumb)
N7890   HU^DB  NumLock  Home UpPage  DownPage BackSpase
C4561   E<v>"  CapsLock End 
S1232B OID[]\  ScrollLock Break Ins Del

mouse mode (after trackball moved or LMR clicked in digits+mouse mode) immediate
SCAW?   ?LMRW
SCAW?   ?LMRW	Win Alt Control Shift Left Middle Right Wheel
CAWS?O ??LMRW   O-release to exit mouse mode, click to exit mouse lock

numlock (replaces main if numlock)
E&*()   *789+	Esc
T$%^=   /456-	Tab
`!@#-_ E0123.	numpadEnter

game mode (immediate)  enter-exit: left thumb+GamingMode
Eqwer   12345	Esc
Tasdf   [LMR]	Tab Left Middle Right mouse
SCAcx_ E`<^v>   Shift Control Alt Enter

*/

const byte PROGMEM shift_keys[] = {
	KEY_GUI, KEY_ALT, KEY_CTRL, KEY_SHIFT, KEY_RIGHT_ALT, 0, 0, 0
};

typedef struct key_info_struct{
	byte prev_state;
	byte bounce_counter;
	byte ignore_release;
} key_info_t;

key_info_t keys[16*3];
byte pressed_keys[32];
byte pressed_count;

byte last_pressed_key;
byte last_pressed_timer;
byte last_released_key;
byte last_released_timer;

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
		byte shift_mask = 0;
		const byte* plane = basic_keys;
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
		if (k->ignore_release)
			return;
		last_released_key = index;
		last_released_timer = 100;
		while (--i >= 0) {
			byte s = pressed_keys[i];
			print("\n try shift="); phex(s);
			if ((s & 7) == 5) {
				plane = (s & 8) ? arrow_keys : numeric_keys;
			} else if ((s & 8) != (index & 8)) {
				shift_mask |= pgm_read_byte(shift_keys + (s & 7));
				print("\n mask="); phex(shift_mask);
			} else
				continue;
			keys[s].ignore_release = true;
		}
		usb_keyboard_press(pgm_read_byte(plane + index), shift_mask);
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
	PORTB = 0xff;
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
		//print("\n");
		if (last_pressed_timer && --last_pressed_timer == 0) {
			// TODO: autorepeat(last_pressed_key);
			last_pressed_timer = 100;
		}
		if (last_released_timer && --last_released_timer == 0)
			last_released_key = 0;
		adns_motion(&dx, &dy);
		if (dx | dy) {
			print(" dx="); phex16(dx); print(" dy="); phex16(dy); print("\n");
		}
		_delay_ms(1000/200);
	}
}

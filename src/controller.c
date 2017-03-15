#ifdef TESTS
#include "test/mock.h"
#else
#include "platform.h"
#endif

#include "usb_keyboard_mouse_debug.h"
#include "adns9800.h"
#include "my_int.h"

void reflash(void);

#define KEY_SPECIAL 245
#define IMM_CTRL   245
#define IMM_ALT    247
#define IMM_SHIFT  248

#define MB_GAME  250

#define MB_LEFT  252
#define MB_RIGHT 253
#define MB_MID   254
#define MB_WHEEL 255

#ifndef DEBOUNCE_DELAY
#define DEBOUNCE_DELAY 0x30
#endif

//
// --- keymap
//
// rrsfff (rr-row, s-side, fff-finger)
const byte PROGMEM basic_keys[] = {
	KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T, KEY_SPACE, 0, 0,  KEY_P,         KEY_O,      KEY_I,     KEY_U, KEY_Y, KEY_ENTER, 0, 0,         
	KEY_A, KEY_S, KEY_D, KEY_F, KEY_G, KEY_SPACE, 0, 0,  KEY_SEMICOLON, KEY_L,      KEY_K,     KEY_J, KEY_H, KEY_ENTER, 0, 0,         
	KEY_Z, KEY_X, KEY_C, KEY_V, KEY_B, KEY_SPACE, 0, 0,  KEY_SLASH,     KEY_PERIOD, KEY_COMMA, KEY_M, KEY_N, KEY_ENTER, 0, 0, 
};

const byte PROGMEM numeric_arrow_keys[] = {
	KEY_ESC,   KEY_7, KEY_8, KEY_9, KEY_0,     KEY_MENU, 0, 0,   KEY_BACKSPACE, KEY_PAGE_DOWN,   KEY_UP,         KEY_PAGE_UP, KEY_HOME,   KEYPAD_ENTER, 0, 0,   
	KEY_TAB,   KEY_4, KEY_5, KEY_6, KEY_EQUAL, KEY_MENU, 0, 0,   KEY_QUOTE,     KEY_RIGHT,       KEY_DOWN,       KEY_LEFT,    KEY_END,    KEYPAD_ENTER, 0, 0, 
	KEY_TILDE, KEY_1, KEY_2, KEY_3, KEY_MINUS, KEY_MENU, 0, 0,   KEY_BACKSLASH, KEY_RIGHT_BRACE, KEY_LEFT_BRACE, KEY_DELETE,  KEY_INSERT, KEYPAD_ENTER, 0, 0, 

};

const byte PROGMEM numpad_keys[] = {
	KEY_NUM_LOCK,    KEY_F7, KEY_F8, KEY_F9, KEY_F10, 0, 0, 0,   KEYPAD_SLASH,  KEYPAD_9, KEYPAD_8, KEYPAD_7, KEYPAD_ASTERIX, 0, 0, 0, 
	KEY_CAPS_LOCK,   KEY_F4, KEY_F5, KEY_F6, KEY_F11, 0, 0, 0,   KEYPAD_MINUS,  KEYPAD_6, KEYPAD_5, KEYPAD_4, KEYPAD_PLUS,    0, 0, 0, 
	KEY_SCROLL_LOCK, KEY_F1, KEY_F2, KEY_F3, KEY_F12, 0, 0, 0,   KEYPAD_PERIOD, KEYPAD_3, KEYPAD_2, KEYPAD_1, KEYPAD_0,       0, 0, 0, 
};

const byte PROGMEM mouse_keys[] = {
	MB_MID, MB_RIGHT, MB_RIGHT, MB_LEFT, MB_LEFT, MB_LEFT, 0, 0,  MB_MID,  MB_RIGHT,   MB_RIGHT,   MB_LEFT,  MB_LEFT,         MB_LEFT, 0, 0,
	0,      0,        0,        0,       0,       MB_LEFT, 0, 0,  0,       0,          0,          0,        KEY_PRINTSCREEN, MB_LEFT, 0, 0,
	0,      0,        0,        0,       0,       MB_LEFT, 0, 0,  MB_GAME, KEY_VOL_DN, KEY_VOL_UP, KEY_MUTE, KEY_PAUSE,       MB_LEFT, 0, 0,
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

#define RM_PROCESS 0
#define RM_UNSHIFT 250
#define RM_MOUSE 249

typedef struct key_info_struct{
	byte prev_state;
	byte bounce_counter;
	byte release_mode; // RM_PROCESS, RM_UNSHIFT, MB_LEFT, MB_MID, MB_RIGHT
} key_info_t;

key_info_t keys[16*3];
byte pressed_keys[32];
byte pressed_count;
sword dwheel;

bool game_mode;
bool game_wheel;
byte shifts_to_send;
byte mb_to_send;
byte autorepeat_key;
word delay_since_released;
word autorepeat_counter;
bool keys_changed;
bool mouse_mode;

void drop_pressed_keys(void) {
	{
		key_info_t* i = keys;
		for (; i < keys + 16*3; i++)
			i->release_mode = RM_UNSHIFT;
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
	if (row == 0) return 0;
	return pgm_read_byte((row == 0x20 ? bottom_shift_keys : main_shift_keys) + (index & 7));
}

sbyte clamp_to_sbyte(sword v) {
	return
		v < -128 ? -128 :
		v > 127 ? 127 : v;
}

byte pressed_key_code(sbyte pos, byte index) {
	if (game_mode)
		return pgm_read_byte(imm_game_mode + index);
	if (mouse_mode)
		return pgm_read_byte(mouse_keys + index);	
	const byte *plane = basic_keys;
	while (--pos >= 0) {
		byte s = pressed_keys[pos];
		if ((s & 7) == 5)
			plane = plane != basic_keys ? numpad_keys : numeric_arrow_keys;
	}
	return pgm_read_byte(plane + index);
}

bool try_mouse_motion(void) {
	if (mouse_mode)
		return true;
	for (byte i = 0; i < pressed_count; i++) {
		byte s = pressed_keys[i];
		if (((s & 0x30) == 0 || (s & 7) == 5) && keys[s].release_mode == RM_PROCESS) {
			keys[s].release_mode = RM_MOUSE;
			return mouse_mode = true;
		}
	}
	return false;
}

byte get_key_code(sbyte pos, byte index) {
	if (game_mode)
		return pgm_read_byte(imm_game_mode + index);
	const byte *plane = basic_keys;
	bool shifts_alone = false;
	shifts_to_send = 0;
	while (--pos >= 0) {
		byte s = pressed_keys[pos];
		if ((s & 7) == 5)
			plane = plane != basic_keys ? numpad_keys : numeric_arrow_keys;
		else if ((s & 8) != (index & 8))
			shifts_to_send |= to_shift(s);
		else if ((index & 7) == 5) {
			shifts_to_send |= to_shift(s);
			shifts_alone = true;
		}
		else
			continue;
		if (keys[s].release_mode == RM_PROCESS)
			keys[s].release_mode = RM_UNSHIFT;
	}
	if (mouse_mode)
		plane = mouse_keys;
	if (shifts_alone)
		return 0;
	return pgm_read_byte(plane + index);
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
	keys_changed = true;
	k->bounce_counter = DEBOUNCE_DELAY;
	if (port == 0) { // pressed
		if (pressed_count >= 32)
			pressed_count = 0;
		pressed_keys[pressed_count++] = index;
		k->release_mode = RM_PROCESS;
		byte key_code = pressed_key_code(pressed_count-1, index);
		if (key_code == MB_WHEEL) {
			game_wheel = true;
			k->release_mode = MB_WHEEL;
		} else if (key_code >= MB_LEFT && key_code <= MB_RIGHT) {
			key_code = get_key_code(pressed_count-1, index);
			k->release_mode = key_code;
			mb_to_send |= 1 << (key_code - MB_LEFT);
		} if (key_code && key_code == autorepeat_key)
			autorepeat_counter = delay_since_released + (delay_since_released >> 1);
	} else {
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
		byte rm = k->release_mode;
		if (rm != RM_PROCESS && !game_mode) {
			if (rm == RM_UNSHIFT)
			    shifts_to_send &= ~to_shift(index);
			else if (rm == RM_MOUSE)
				mouse_mode = false;
			else if (rm == MB_WHEEL)
				game_wheel = false;
			else if (rm >= MB_LEFT && rm <= MB_RIGHT)
				mb_to_send &= ~(1 << (rm - MB_LEFT));
			k->release_mode = RM_PROCESS;
			return;
		}
		byte key_code = get_key_code(i, index);
		if (key_code == MB_GAME) {
			game_mode = !game_mode;
			drop_pressed_keys();
			if (!game_mode)
				game_wheel = false;
		} else if (key_code < KEY_SPECIAL && !game_mode) {
			keyboard_modifier_keys = shifts_to_send;
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
}
//
//  ------------------------- MAIN -----------------------
//
void init(void) {
	game_mode = false;
	game_wheel = false;
	shifts_to_send = keyboard_modifier_keys = 0;
	mb_to_send = mouse_buttons = 0;
	autorepeat_key = 0;
	delay_since_released = 0;
	autorepeat_counter = 0;

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
			i->release_mode = RM_UNSHIFT;
		}
	}
	if (((PINB | 7) & (PIND | 0xf0) & (PINF | 0x3f) & (PINC | 0xbf)) != 0xff)
		reflash();
	PORTB = 0xff;
	pressed_count = 0;
	dwheel = 0;
	mouse_mode = false;
}
void loop_step(void) {
	sword dx;
	sword dy;
	keys_changed = false;
	PORTB = ~1;
	scanRow(0);
	PORTB = ~2;
	scanRow(16);
	PORTB = ~4;
	scanRow(32);
	adns_motion(&dx, &dy);
	if (game_mode && keys_changed) {
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
	if (dx | dy) {
		if ((game_mode && !game_wheel) || try_mouse_motion()) {
			dwheel = 0;
		} else {
			dwheel -= dy;
			dx = dy = 0;
		}
		if (!game_mode)
			get_key_code(pressed_count, 0x19); // to calc shifts
	}
	if (!game_mode) {
		if (autorepeat_counter) {
			if (--autorepeat_counter == 0) {
				autorepeat_counter = delay_since_released;
				byte key = get_key_code(pressed_count-1, pressed_keys[pressed_count-1]);
				keyboard_modifier_keys = shifts_to_send;
				usb_keyboard_press(key);
			}

		} else if (delay_since_released < 0xffff)
			delay_since_released++;
		if (shifts_to_send != keyboard_modifier_keys) {
			keyboard_modifier_keys = shifts_to_send;
			usb_keyboard_press(0);
		}
	}
	if (dx || dy || dwheel / 32 || mouse_buttons != mb_to_send) {
		mouse_buttons = mb_to_send;
		usb_mouse_move(clamp_to_sbyte(dx), clamp_to_sbyte(-dy), clamp_to_sbyte(dwheel / 32));
		dwheel = 0;
	}
}

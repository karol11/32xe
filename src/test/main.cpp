#include <string>
#include <sstream>
using std::string;
using std::stringstream;

#include "../controller.c"

byte grid[4][3];
byte PORTB;
byte PORTC;
byte PORTD;
byte PORTF;
byte DDRB;
byte DDRC;
byte DDRD;
byte DDRF;

byte keyboard_keys[6];
byte keyboard_modifier_keys;
byte volatile keyboard_leds;
byte mouse_buttons;

byte read_pin(int pin) {
	byte r = 0;
	if ((PORTB & 1) == 0)
		r |= grid[pin][2];
	if ((PORTB & 2) == 0)
		r |= grid[pin][1];
	if ((PORTB & 4) == 0)
		r |= grid[pin][0];
	return ~r;
}

struct Switch {
	byte pin, bit, row;
	const char *name;
};

namespace T {
	Switch Q ={IND_C, 6, 2, "Q"};
	Switch A ={IND_C, 6, 1, "A"};
	Switch Z ={IND_C, 6, 0, "Z"};

	Switch W ={IND_D, 3, 2, "W"};
	Switch S ={IND_D, 3, 1, "S"};
	Switch X ={IND_D, 3, 0, "X"};

	Switch E ={IND_D, 2, 2, "E"};
	Switch D ={IND_D, 2, 1, "D"};
	Switch C ={IND_D, 2, 0, "C"};

	Switch R ={IND_D, 1, 2, "R"};
	Switch F ={IND_D, 1, 1, "F"};
	Switch V ={IND_D, 1, 0, "V"};

	Switch T ={IND_D, 0, 2, "T"};
	Switch G ={IND_D, 0, 1, "G"};
	Switch B ={IND_D, 0, 0, "B"};

	Switch SPACE ={IND_B, 4, 0, "SPACE"};
	Switch ENTER ={IND_B, 6, 0, "ENTER"};

	Switch Y ={IND_B, 5, 2, "Y"};
	Switch H ={IND_B, 5, 1, "H"};
	Switch N ={IND_B, 5, 0, "N"};

	Switch U ={IND_F, 7, 2, "U"};
	Switch J ={IND_F, 7, 1, "J"};
	Switch M ={IND_F, 7, 0, "M"};

	Switch I ={IND_F, 6, 2, "I"};
	Switch K ={IND_F, 6, 1, "K"};
	Switch COMMA ={IND_F, 6, 0, "COMMA"};

	Switch O ={IND_B, 7, 2, "O"};
	Switch L ={IND_B, 7, 1, "L"};
	Switch PERIOD ={IND_B, 7, 0, "PERIOD"};

	Switch P ={IND_B, 3, 2, "P"};
	Switch COLON ={IND_B, 3, 1, "COLON"};
	Switch SLASH ={IND_B, 3, 0, "SLASH"};
}

namespace {
	const char *shifts[] = {
		"LEFT_CTRL", // 0x01
		"LEFT_SHIFT", // 0x02
		"LEFT_ALT", // 0x04
		"LEFT_GUI", // 0x08
		"RIGHT_CTRL", // 0x10
		"RIGHT_SHIFT", // 0x20
		"RIGHT_ALT", // 0x40
		"RIGHT_GUI", // 0x80
	};
	const char *key_names[] = {
		"","","","",
		"KEY_A", // 4
		"KEY_B", // 5
		"KEY_C", // 6
		"KEY_D", // 7
		"KEY_E", // 8
		"KEY_F", // 9
		"KEY_G", // 10
		"KEY_H", // 11
		"KEY_I", // 12
		"KEY_J", // 13
		"KEY_K", // 14
		"KEY_L", // 15
		"KEY_M", // 16
		"KEY_N", // 17
		"KEY_O", // 18
		"KEY_P", // 19
		"KEY_Q", // 20
		"KEY_R", // 21
		"KEY_S", // 22
		"KEY_T", // 23
		"KEY_U", // 24
		"KEY_V", // 25
		"KEY_W", // 26
		"KEY_X", // 27
		"KEY_Y", // 28
		"KEY_Z", // 29
		"KEY_1", // 30
		"KEY_2", // 31
		"KEY_3", // 32
		"KEY_4", // 33
		"KEY_5", // 34
		"KEY_6", // 35
		"KEY_7", // 36
		"KEY_8", // 37
		"KEY_9", // 38
		"KEY_0", // 39
		"KEY_ENTER", // 40	
		"KEY_ESC", // 41
		"KEY_BACKSPACE", // 42	
		"KEY_TAB", // 43
		"KEY_SPACE", // 44	
		"KEY_MINUS", // 45
		"KEY_EQUAL", // 46
		"KEY_LEFT_BRACE", // 47
		"KEY_RIGHT_BRACE", // 48
		"KEY_BACKSLASH", // 49
		"KEY_NUMBER", // 50 // NON_US
		"KEY_SEMICOLON", // 51
		"KEY_QUOTE", // 52
		"KEY_TILDE", // 53
		"KEY_COMMA", // 54
		"KEY_PERIOD", // 55
		"KEY_SLASH", // 56
		"KEY_CAPS_LOCK", // 57
		"KEY_F1", // 58
		"KEY_F2", // 59
		"KEY_F3", // 60
		"KEY_F4", // 61
		"KEY_F5", // 62
		"KEY_F6", // 63
		"KEY_F7", // 64
		"KEY_F8", // 65
		"KEY_F9", // 66
		"KEY_F10", // 67
		"KEY_F11", // 68
		"KEY_F12", // 69
		"KEY_PRINTSCREEN", // 70		
		"KEY_SCROLL_LOCK", // 71		
		"KEY_PAUSE", // 72
		"KEY_INSERT", // 73	
		"KEY_HOME", // 74
		"KEY_PAGE_UP", // 75
		"KEY_DELETE", // 76
		"KEY_END", // 77
		"KEY_PAGE_DOWN", // 78	
		"KEY_RIGHT", // 79
		"KEY_LEFT", // 80
		"KEY_DOWN", // 81
		"KEY_UP", // 82
		"KEY_NUM_LOCK", // 83
		"KEYPAD_SLASH", // 84	
		"KEYPAD_ASTERIX", // 85	
		"KEYPAD_MINUS", // 86	
		"KEYPAD_PLUS", // 87	
		"KEYPAD_ENTER", // 88	
		"KEYPAD_1", // 89
		"KEYPAD_2", // 90
		"KEYPAD_3", // 91	
		"KEYPAD_4", // 92	
		"KEYPAD_5", // 93	
		"KEYPAD_6", // 94	
		"KEYPAD_7", // 95	
		"KEYPAD_8", // 96	
		"KEYPAD_9", // 97	
		"KEYPAD_0", // 98		
		"KEYPAD_PERIOD", // 99		
	};

	const char* get_key_name(int key) {
		switch (key) {
			case 0x76: return "KEY_MENU"; 
			case 0x7f: return "KEY_MUTE"; 
			case 0x80: return "KEY_VOL_UP"; 
			case 0x81: return "KEY_VOL_DN"; 
			default:
				if (key < 100)
					return key_names[key];
				return "???";
		}
	}
	stringstream acc;
	sword next_dx;
	sword next_dy;
	bool has_errors = false;

	void test(const string &name) {
		memset(grid, 0, sizeof(grid));
		acc.clear();
		acc.str(std::string());
		acc << name << ":";
		init();
		loop();
		loop();
		loop();
		loop();
	}

	void check(const char *expected) {
		string s = acc.str();
		if (s == expected)
			return;
		printf("fail %s, expected %s\n", s.c_str(), expected);
		has_errors = true;
	}

	void on(Switch &s) {
		grid[s.pin][s.row] |= 1 << s.bit;
		loop();
		loop();
		loop();
		loop();
	}

	void off(Switch &s) {
		grid[s.pin][s.row] &= ~(1 << s.bit);
		loop();
		loop();
		loop();
		loop();
		loop();
	}

	void move(sword dx, sword dy) {
		next_dx = dx;
		next_dy = dy;
		loop();
	}

	void test_single_press(Switch &s, const char *expected) {
		test(string("single") + s.name);
		on(s);
		off(s);
		check(expected);
	}
	
	void test_ABab(Switch &a, Switch &b, const char *expected) {
		test(string("pr") + a.name + "+pr" + b.name + "+rel" + a.name + "+rel" + b.name);
		on(a);
		on(b);
		off(a);
		off(b);
		check(expected);
	}

	void test_ABba(Switch &a, Switch &b, const char *expected) {
		test(string("pr") + a.name + "+pr" + b.name + "+rel" + b.name + "+rel" + a.name);
		on(a);
		on(b);
		off(b);
		off(a);
		check(expected);
	}
};

sbyte __cdecl usb_keyboard_press(byte key) {
	keyboard_keys[0] = key;
	usb_keyboard_send();
	keyboard_keys[0] = 0;
	return 1;
}
sbyte __cdecl usb_keyboard_send(void) {
	acc << "(";
	stringstream::pos_type start = acc.tellp();
	for (int i = 0; i < 8; i++) {
		if (keyboard_modifier_keys & (1 << i)) {
			acc << shifts[i] << " ";
		}
	}
	for (int i = 0; i < 6; i++) {
		if (keyboard_keys[i]) {
			acc << get_key_name(keyboard_keys[i]) << " ";
		}
	}
	if (acc.tellp() != start)
		acc.seekp(-1, acc.cur);
	acc << ") ";
	return 1;
}
void __cdecl reflash(void) {
	acc << "reflash ";
}
sbyte __cdecl usb_mouse_move(sbyte dx, sbyte dy, sbyte wheel) {
	acc << "[";
	if (mouse_buttons & 1)
		acc << "L";
	if (mouse_buttons & 2)
		acc << "M";
	if (mouse_buttons & 4)
		acc << "R";
	acc << dx << " " << dy << " " << wheel << "] ";
	return 1;
}

void __cdecl adns_motion(sword *dx,sword *dy) {
	*dx = next_dx;
	*dy = next_dy;
	next_dx = next_dy = 0;
}

int main() {
	// single press on each row and column
	test_single_press(T::Q, "singleQ:(KEY_Q) ");
	test_single_press(T::S, "singleS:(KEY_S) ");
	test_single_press(T::C, "singleC:(KEY_C) ");
	test_single_press(T::F, "singleF:(KEY_F) ");
	test_single_press(T::G, "singleG:(KEY_G) ");
	test_single_press(T::N, "singleN:(KEY_N) ");
	test_single_press(T::U, "singleU:(KEY_U) ");
	test_single_press(T::K, "singleK:(KEY_K) ");
	test_single_press(T::PERIOD, "singlePERIOD:(KEY_PERIOD) ");
	test_single_press(T::P, "singleP:(KEY_P) ");
	test_single_press(T::SPACE, "singleSPACE:(KEY_SPACE) ");
	test_single_press(T::ENTER, "singleENTER:(KEY_ENTER) ");

	// pair of non-intersecting presses on left+left, right+right, left-right crossing, and with thums
	test_ABab(T::Q, T::W, "prQ+prW+relQ+relW:(KEY_Q) (KEY_W) ");
	test_ABab(T::O, T::P, "prO+prP+relO+relP:(KEY_O) (KEY_P) ");
	test_ABab(T::F, T::J, "prF+prJ+relF+relJ:(KEY_F) (KEY_J) ");
	test_ABab(T::J, T::F, "prJ+prF+relJ+relF:(KEY_J) (KEY_F) ");
	test_ABab(T::W, T::SPACE, "prW+prSPACE+relW+relSPACE:(KEY_W) (KEY_SPACE) ");
	test_ABab(T::K, T::SPACE, "prK+prSPACE+relK+relSPACE:(KEY_K) (KEY_SPACE) ");
	test_ABab(T::SPACE, T::W, "prSPACE+prW+relSPACE+relW:(KEY_SPACE) (KEY_W) ");
	test_ABab(T::SPACE, T::SLASH, "prSPACE+prSLASH+relSPACE+relSLASH:(KEY_SPACE) (KEY_SLASH) ");
	test_ABab(T::W, T::ENTER, "prW+prENTER+relW+relENTER:(KEY_W) (KEY_ENTER) ");
	test_ABab(T::K, T::ENTER, "prK+prENTER+relK+relENTER:(KEY_K) (KEY_ENTER) ");
	test_ABab(T::ENTER, T::W, "prENTER+prW+relENTER+relW:(KEY_ENTER) (KEY_W) ");
	test_ABab(T::ENTER, T::SLASH, "prENTER+prSLASH+relENTER+relSLASH:(KEY_ENTER) (KEY_SLASH) ");
	test_ABab(T::ENTER, T::SPACE, "prENTER+prSPACE+relENTER+relSPACE:(KEY_ENTER) (KEY_SPACE) ");
	test_ABab(T::SPACE, T::ENTER, "prSPACE+prENTER+relSPACE+relENTER:(KEY_SPACE) (KEY_ENTER) ");

	// intersecting pairs as shifts
	test_ABba(T::A, T::P, "prA+prP+relP+relA:(LEFT_SHIFT KEY_P) () ");
	test_ABba(T::S, T::COLON, "prS+prCOLON+relCOLON+relS:(LEFT_GUI KEY_SEMICOLON) () ");
	test_ABba(T::D, T::COMMA, "prD+prCOMMA+relCOMMA+relD:(LEFT_ALT KEY_COMMA) () ");
	test_ABba(T::F, T::SLASH, "prF+prSLASH+relSLASH+relF:(LEFT_CTRL KEY_SLASH) () ");
	test_ABba(T::Z, T::Y, "prZ+prY+relY+relZ:(RIGHT_CTRL KEY_Y) () ");
	test_ABba(T::X, T::H, "prX+prH+relH+relX:(RIGHT_GUI KEY_H) () ");
	test_ABba(T::C, T::N, "prC+prN+relN+relC:(RIGHT_ALT KEY_N) () ");
	test_ABba(T::V, T::ENTER, "prV+prENTER+relENTER+relV:(RIGHT_SHIFT KEY_ENTER) () ");
	test_ABba(T::J, T::Q, "prJ+prQ+relQ+relJ:(LEFT_CTRL KEY_Q) () ");
	test_ABba(T::K, T::A, "prK+prA+relA+relK:(LEFT_ALT KEY_A) () ");
	test_ABba(T::L, T::Z, "prL+prZ+relZ+relL:(LEFT_GUI KEY_Z) () ");
	test_ABba(T::COLON, T::SPACE, "prCOLON+prSPACE+relSPACE+relCOLON:(LEFT_SHIFT KEY_SPACE) () ");
	test_ABba(T::SLASH, T::T, "prSLASH+prT+relT+relSLASH:(RIGHT_CTRL KEY_T) () ");
	test_ABba(T::PERIOD, T::G, "prPERIOD+prG+relG+relPERIOD:(RIGHT_GUI KEY_G) () ");
	test_ABba(T::COMMA, T::B, "prCOMMA+prB+relB+relCOMMA:(RIGHT_ALT KEY_B) () ");
	test_ABba(T::M, T::D, "prM+prD+relD+relM:(RIGHT_SHIFT KEY_D) () ");

	// arrows
	test_ABba(T::ENTER, T::Y, "prENTER+prY+relY+relENTER:(KEY_HOME) ");
	test_ABba(T::ENTER, T::H, "prENTER+prH+relH+relENTER:(KEY_END) ");
	test_ABba(T::ENTER, T::N, "prENTER+prN+relN+relENTER:(KEY_INSERT) ");
	test_ABba(T::ENTER, T::U, "prENTER+prU+relU+relENTER:(KEY_PAGE_UP) ");
	test_ABba(T::ENTER, T::J, "prENTER+prJ+relJ+relENTER:(KEY_LEFT) ");
	test_ABba(T::ENTER, T::M, "prENTER+prM+relM+relENTER:(KEY_DELETE) ");
	test_ABba(T::ENTER, T::I, "prENTER+prI+relI+relENTER:(KEY_UP) ");
	test_ABba(T::ENTER, T::K, "prENTER+prK+relK+relENTER:(KEY_DOWN) ");
	test_ABba(T::ENTER, T::COMMA, "prENTER+prCOMMA+relCOMMA+relENTER:(KEY_LEFT_BRACE) ");
	test_ABba(T::ENTER, T::O, "prENTER+prO+relO+relENTER:(KEY_PAGE_DOWN) ");
	test_ABba(T::ENTER, T::L, "prENTER+prL+relL+relENTER:(KEY_RIGHT) ");
	test_ABba(T::ENTER, T::PERIOD, "prENTER+prPERIOD+relPERIOD+relENTER:(KEY_RIGHT_BRACE) ");
	test_ABba(T::ENTER, T::P, "prENTER+prP+relP+relENTER:(KEY_BACKSPACE) ");
	test_ABba(T::ENTER, T::COLON, "prENTER+prCOLON+relCOLON+relENTER:(KEY_QUOTE) ");
	test_ABba(T::ENTER, T::SLASH, "prENTER+prSLASH+relSLASH+relENTER:(KEY_BACKSLASH) ");

	test("shift+ctrl+arrows");
	on(T::A); on(T::F); on(T::ENTER);
	on(T::K); off(T::K);
	on(T::L); off(T::L);
	on(T::L); off(T::L);
	on(T::K); on(T::L);
	off(T::K); off(T::L);
	off(T::A); off(T::F); off(T::ENTER);
	check("shift+ctrl+arrows:(LEFT_CTRL LEFT_SHIFT KEY_DOWN) (LEFT_CTRL LEFT_SHIFT KEY_RIGHT) (LEFT_CTRL LEFT_SHIFT KEY_RIGHT) "
		"(LEFT_CTRL LEFT_SHIFT KEY_DOWN) (LEFT_CTRL LEFT_SHIFT KEY_RIGHT) (LEFT_CTRL) () ");

	test_ABba(T::ENTER, T::Q, "prENTER+prQ+relQ+relENTER:(KEY_NUM_LOCK) ");
	test_ABba(T::ENTER, T::W, "prENTER+prW+relW+relENTER:(KEY_F7) ");
	test_ABba(T::ENTER, T::E, "prENTER+prE+relE+relENTER:(KEY_F8) ");
	test_ABba(T::ENTER, T::R, "prENTER+prR+relR+relENTER:(KEY_F9) ");
	test_ABba(T::ENTER, T::T, "prENTER+prT+relT+relENTER:(KEY_F10) ");
	test_ABba(T::ENTER, T::A, "prENTER+prA+relA+relENTER:(KEY_CAPS_LOCK) ");
	test_ABba(T::ENTER, T::S, "prENTER+prS+relS+relENTER:(KEY_F4) ");
	test_ABba(T::ENTER, T::D, "prENTER+prD+relD+relENTER:(KEY_F5) ");
	test_ABba(T::ENTER, T::F, "prENTER+prF+relF+relENTER:(KEY_F6) ");
	test_ABba(T::ENTER, T::G, "prENTER+prG+relG+relENTER:(KEY_F11) ");
	test_ABba(T::ENTER, T::Z, "prENTER+prZ+relZ+relENTER:(KEY_SCROLL_LOCK) ");
	test_ABba(T::ENTER, T::X, "prENTER+prX+relX+relENTER:(KEY_F1) ");
	test_ABba(T::ENTER, T::C, "prENTER+prC+relC+relENTER:(KEY_F2) ");
	test_ABba(T::ENTER, T::V, "prENTER+prV+relV+relENTER:(KEY_F3) ");
	test_ABba(T::ENTER, T::B, "prENTER+prB+relB+relENTER:(KEY_F12) ");

	// numbers
	// mouse clicks
	// 
	// numlock plane
	// wheel and whifts+wheel
	// shifts alone
	// game mode
	// mouse move

	printf("%s\n", has_errors ? "failed" : "Ok");
	return has_errors;
}

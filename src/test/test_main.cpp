#include <string>
#include <sstream>
using std::string;
using std::stringstream;

#define DEBOUNCE_DELAY 1
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

	void test(const string &name, Switch *at_start = NULL) {
		memset(grid, 0, sizeof(grid));
		acc << name << ":";
		if (at_start)
			grid[at_start->pin][at_start->row] |= 1 << at_start->bit;
		init();
		loop_step();
		loop_step();
		loop_step();
		loop_step();
	}

	void check(const char *expected) {
		string s = acc.str();
		s.resize(s.length() - 1);
		acc.clear();
		acc.str(std::string());
		if (s == expected)
			return;
		printf("fail %s, expected %s\n", s.c_str(), expected);
		has_errors = true;
	}

	void on(Switch &s) {
		grid[s.pin][s.row] |= 1 << s.bit;
		loop_step();
		loop_step();
		loop_step();
		loop_step();
	}

	void off(Switch &s) {
		grid[s.pin][s.row] &= ~(1 << s.bit);
		loop_step();
		loop_step();
		loop_step();
		loop_step();
		loop_step();
	}

	void move(sword dx, sword dy) {
		next_dx = dx;
		next_dy = dy;
		loop_step();
	}

	void test_single_press(Switch &s, const char *expected) {
		test(string("single") + s.name);
		on(s);
		off(s);
		check(expected);
	}

	void test_numpad_press(Switch &s, const char *expected) {
		test(string("numpad") + s.name);
		on(T::SPACE); on(T::ENTER);
		on(s);
		off(s);
		off(T::ENTER); off(T::SPACE);
		check(expected);
	}
	
	void test_ABab(Switch &a, Switch &b, const char *expected) {
		test(string("+") + a.name + "+" + b.name + "-" + a.name + "-" + b.name);
		on(a);
		on(b);
		off(a);
		off(b);
		check(expected);
	}

	void test_ABba(Switch &a, Switch &b, const char *expected) {
		test(string("+") + a.name + "+" + b.name + "-" + b.name + "-" + a.name);
		on(a);
		on(b);
		off(b);
		off(a);
		check(expected);
	}

	void test_main_plane(){
		// single press on each row and column
		test_single_press(T::Q, "singleQ:(KEY_Q) ()");
		test_single_press(T::S, "singleS:(KEY_S) ()");
		test_single_press(T::C, "singleC:(KEY_C) ()");
		test_single_press(T::F, "singleF:(KEY_F) ()");
		test_single_press(T::G, "singleG:(KEY_G) ()");
		test_single_press(T::N, "singleN:(KEY_N) ()");
		test_single_press(T::U, "singleU:(KEY_U) ()");
		test_single_press(T::K, "singleK:(KEY_K) ()");
		test_single_press(T::PERIOD, "singlePERIOD:(KEY_PERIOD) ()");
		test_single_press(T::P, "singleP:(KEY_P) ()");
		test_single_press(T::SPACE, "singleSPACE:(KEY_SPACE) ()");
		test_single_press(T::ENTER, "singleENTER:(KEY_ENTER) ()");

		// pair of non-intersecting presses on left+left, right+right, left-right crossing, and with thums
		test_ABab(T::Q, T::W, "+Q+W-Q-W:(KEY_Q) () (KEY_W) ()");
		test_ABab(T::O, T::P, "+O+P-O-P:(KEY_O) () (KEY_P) ()");
		test_ABab(T::F, T::J, "+F+J-F-J:(KEY_F) () (KEY_J) ()");
		test_ABab(T::J, T::F, "+J+F-J-F:(KEY_J) () (KEY_F) ()");
		test_ABab(T::W, T::SPACE, "+W+SPACE-W-SPACE:(KEY_W) () (KEY_SPACE) ()");
		test_ABab(T::K, T::SPACE, "+K+SPACE-K-SPACE:(KEY_K) () (KEY_SPACE) ()");
		test_ABab(T::SPACE, T::W, "+SPACE+W-SPACE-W:(KEY_SPACE) () (KEY_W) ()");
		test_ABab(T::SPACE, T::SLASH, "+SPACE+SLASH-SPACE-SLASH:(KEY_SPACE) () (KEY_SLASH) ()");
		test_ABab(T::W, T::ENTER, "+W+ENTER-W-ENTER:(KEY_W) () (KEY_ENTER) ()");
		test_ABab(T::K, T::ENTER, "+K+ENTER-K-ENTER:(KEY_K) () (KEY_ENTER) ()");
		test_ABab(T::ENTER, T::W, "+ENTER+W-ENTER-W:(KEY_ENTER) () (KEY_W) ()");
		test_ABab(T::ENTER, T::SLASH, "+ENTER+SLASH-ENTER-SLASH:(KEY_ENTER) () (KEY_SLASH) ()");
		test_ABab(T::ENTER, T::SPACE, "+ENTER+SPACE-ENTER-SPACE:(KEY_ENTER) () (KEY_SPACE) ()");
		test_ABab(T::SPACE, T::ENTER, "+SPACE+ENTER-SPACE-ENTER:(KEY_SPACE) () (KEY_ENTER) ()");

		// intersecting pairs as shifts
		test_ABba(T::A, T::P, "+A+P-P-A:(LEFT_SHIFT KEY_P) (LEFT_SHIFT) ()");
		test_ABba(T::S, T::COLON, "+S+COLON-COLON-S:(LEFT_GUI KEY_SEMICOLON) (LEFT_GUI) ()");
		test_ABba(T::D, T::COMMA, "+D+COMMA-COMMA-D:(LEFT_ALT KEY_COMMA) (LEFT_ALT) ()");
		test_ABba(T::F, T::SLASH, "+F+SLASH-SLASH-F:(LEFT_CTRL KEY_SLASH) (LEFT_CTRL) ()");
		test_ABba(T::Z, T::Y, "+Z+Y-Y-Z:(RIGHT_CTRL KEY_Y) (RIGHT_CTRL) ()");
		test_ABba(T::X, T::H, "+X+H-H-X:(RIGHT_GUI KEY_H) (RIGHT_GUI) ()");
		test_ABba(T::C, T::N, "+C+N-N-C:(RIGHT_ALT KEY_N) (RIGHT_ALT) ()");
		test_ABba(T::V, T::ENTER, "+V+ENTER-ENTER-V:(RIGHT_SHIFT KEY_ENTER) (RIGHT_SHIFT) ()");
		test_ABba(T::J, T::Q, "+J+Q-Q-J:(LEFT_CTRL KEY_Q) (LEFT_CTRL) ()");
		test_ABba(T::K, T::A, "+K+A-A-K:(LEFT_ALT KEY_A) (LEFT_ALT) ()");
		test_ABba(T::L, T::Z, "+L+Z-Z-L:(LEFT_GUI KEY_Z) (LEFT_GUI) ()");
		test_ABba(T::COLON, T::SPACE, "+COLON+SPACE-SPACE-COLON:(LEFT_SHIFT KEY_SPACE) (LEFT_SHIFT) ()");
		test_ABba(T::SLASH, T::T, "+SLASH+T-T-SLASH:(RIGHT_CTRL KEY_T) (RIGHT_CTRL) ()");
		test_ABba(T::PERIOD, T::G, "+PERIOD+G-G-PERIOD:(RIGHT_GUI KEY_G) (RIGHT_GUI) ()");
		test_ABba(T::COMMA, T::B, "+COMMA+B-B-COMMA:(RIGHT_ALT KEY_B) (RIGHT_ALT) ()");
		test_ABba(T::M, T::D, "+M+D-D-M:(RIGHT_SHIFT KEY_D) (RIGHT_SHIFT) ()");

		// shifts alone
		test_ABba(T::A, T::SPACE, "+A+SPACE-SPACE-A:(LEFT_SHIFT) ()");
		test("shifts alone");
		on(T::COLON); on(T::K); on(T::ENTER);
		off(T::ENTER); off(T::K); off(T::COLON);
		check("shifts alone:(LEFT_SHIFT LEFT_ALT) (LEFT_SHIFT) ()");
	}

	void test_side_planes() {
		// arrows
		test_ABba(T::ENTER, T::Y, "+ENTER+Y-Y-ENTER:(KEY_HOME) ()");
		test_ABba(T::ENTER, T::H, "+ENTER+H-H-ENTER:(KEY_END) ()");
		test_ABba(T::ENTER, T::N, "+ENTER+N-N-ENTER:(KEY_INSERT) ()");
		test_ABba(T::ENTER, T::U, "+ENTER+U-U-ENTER:(KEY_PAGE_UP) ()");
		test_ABba(T::ENTER, T::J, "+ENTER+J-J-ENTER:(KEY_LEFT) ()");
		test_ABba(T::ENTER, T::M, "+ENTER+M-M-ENTER:(KEY_DELETE) ()");
		test_ABba(T::ENTER, T::I, "+ENTER+I-I-ENTER:(KEY_UP) ()");
		test_ABba(T::ENTER, T::K, "+ENTER+K-K-ENTER:(KEY_DOWN) ()");
		test_ABba(T::ENTER, T::COMMA, "+ENTER+COMMA-COMMA-ENTER:(KEY_LEFT_BRACE) ()");
		test_ABba(T::ENTER, T::O, "+ENTER+O-O-ENTER:(KEY_PAGE_DOWN) ()");
		test_ABba(T::ENTER, T::L, "+ENTER+L-L-ENTER:(KEY_RIGHT) ()");
		test_ABba(T::ENTER, T::PERIOD, "+ENTER+PERIOD-PERIOD-ENTER:(KEY_RIGHT_BRACE) ()");
		test_ABba(T::ENTER, T::P, "+ENTER+P-P-ENTER:(KEY_BACKSPACE) ()");
		test_ABba(T::ENTER, T::COLON, "+ENTER+COLON-COLON-ENTER:(KEY_QUOTE) ()");
		test_ABba(T::ENTER, T::SLASH, "+ENTER+SLASH-SLASH-ENTER:(KEY_BACKSLASH) ()");
		test_ABba(T::ENTER, T::SPACE, "+ENTER+SPACE-SPACE-ENTER:(KEY_MENU) ()");

		test("shift+ctrl+arrows");
		on(T::A); on(T::F); on(T::ENTER);
		on(T::K); off(T::K);
		on(T::L); off(T::L);
		on(T::L); off(T::L);
		on(T::K); on(T::L);
		off(T::K); off(T::L);
		off(T::A); off(T::F); off(T::ENTER);
		check("shift+ctrl+arrows:"
			"(LEFT_CTRL LEFT_SHIFT KEY_DOWN) (LEFT_CTRL LEFT_SHIFT) "
			"(LEFT_CTRL LEFT_SHIFT KEY_RIGHT) (LEFT_CTRL LEFT_SHIFT) "
			"(LEFT_CTRL LEFT_SHIFT KEY_RIGHT) (LEFT_CTRL LEFT_SHIFT) "
			"(LEFT_CTRL LEFT_SHIFT KEY_DOWN) (LEFT_CTRL LEFT_SHIFT) "
			"(LEFT_CTRL LEFT_SHIFT KEY_RIGHT) (LEFT_CTRL LEFT_SHIFT) (LEFT_CTRL) ()");

		test_ABba(T::ENTER, T::Q, "+ENTER+Q-Q-ENTER:(KEY_ESC) ()");
		test_ABba(T::ENTER, T::A, "+ENTER+A-A-ENTER:(KEY_TAB) ()");
		test_ABba(T::ENTER, T::Z, "+ENTER+Z-Z-ENTER:(KEY_TILDE) ()");
		test_ABba(T::ENTER, T::X, "+ENTER+X-X-ENTER:(KEY_1) ()");
		test_ABba(T::ENTER, T::C, "+ENTER+C-C-ENTER:(KEY_2) ()");
		test_ABba(T::ENTER, T::V, "+ENTER+V-V-ENTER:(KEY_3) ()");
		test_ABba(T::ENTER, T::S, "+ENTER+S-S-ENTER:(KEY_4) ()");
		test_ABba(T::ENTER, T::D, "+ENTER+D-D-ENTER:(KEY_5) ()");
		test_ABba(T::ENTER, T::F, "+ENTER+F-F-ENTER:(KEY_6) ()");
		test_ABba(T::ENTER, T::W, "+ENTER+W-W-ENTER:(KEY_7) ()");
		test_ABba(T::ENTER, T::E, "+ENTER+E-E-ENTER:(KEY_8) ()");
		test_ABba(T::ENTER, T::R, "+ENTER+R-R-ENTER:(KEY_9) ()");
		test_ABba(T::ENTER, T::T, "+ENTER+T-T-ENTER:(KEY_0) ()");
		test_ABba(T::ENTER, T::G, "+ENTER+G-G-ENTER:(KEY_EQUAL) ()");
		test_ABba(T::ENTER, T::B, "+ENTER+B-B-ENTER:(KEY_MINUS) ()");

		// numbers
		test_ABba(T::SPACE, T::Q, "+SPACE+Q-Q-SPACE:(KEY_ESC) ()");
		test_ABba(T::SPACE, T::A, "+SPACE+A-A-SPACE:(KEY_TAB) ()");
		test_ABba(T::SPACE, T::Z, "+SPACE+Z-Z-SPACE:(KEY_TILDE) ()");
		test_ABba(T::SPACE, T::X, "+SPACE+X-X-SPACE:(KEY_1) ()");
		test_ABba(T::SPACE, T::C, "+SPACE+C-C-SPACE:(KEY_2) ()");
		test_ABba(T::SPACE, T::V, "+SPACE+V-V-SPACE:(KEY_3) ()");
		test_ABba(T::SPACE, T::S, "+SPACE+S-S-SPACE:(KEY_4) ()");
		test_ABba(T::SPACE, T::D, "+SPACE+D-D-SPACE:(KEY_5) ()");
		test_ABba(T::SPACE, T::F, "+SPACE+F-F-SPACE:(KEY_6) ()");
		test_ABba(T::SPACE, T::W, "+SPACE+W-W-SPACE:(KEY_7) ()");
		test_ABba(T::SPACE, T::E, "+SPACE+E-E-SPACE:(KEY_8) ()");
		test_ABba(T::SPACE, T::R, "+SPACE+R-R-SPACE:(KEY_9) ()");
		test_ABba(T::SPACE, T::T, "+SPACE+T-T-SPACE:(KEY_0) ()");
		test_ABba(T::SPACE, T::G, "+SPACE+G-G-SPACE:(KEY_EQUAL) ()");
		test_ABba(T::SPACE, T::B, "+SPACE+B-B-SPACE:(KEY_MINUS) ()");
		test("ctrl+! ()");
		on(T::SPACE); on(T::COLON); on(T::J);
		on(T::X); off(T::X);
		off(T::SPACE); off(T::J); on(T::SPACE);
		on(T::R); on(T::T); off(T::R); off(T::T);
		off(T::SPACE); off(T::COLON);
		check("ctrl+! ():"
			"(LEFT_CTRL LEFT_SHIFT KEY_1) (LEFT_CTRL LEFT_SHIFT) (LEFT_SHIFT) "
			"(LEFT_SHIFT KEY_9) (LEFT_SHIFT) "
			"(LEFT_SHIFT KEY_0) (LEFT_SHIFT) ()");
		//test_ABba(T::SPACE, T::ENTER, ""); unassigned
		test_ABba(T::SPACE, T::P, "+SPACE+P-P-SPACE:(KEY_BACKSPACE) ()");
		test_ABba(T::SPACE, T::COLON, "+SPACE+COLON-COLON-SPACE:(KEY_QUOTE) ()");
		test_ABba(T::SPACE, T::Y, "+SPACE+Y-Y-SPACE:(KEY_HOME) ()");
		test_ABba(T::SPACE, T::H, "+SPACE+H-H-SPACE:(KEY_END) ()");
		test_ABba(T::G, T::Y, "+G+Y-Y-G:(KEY_PRINTSCREEN) ()");
		test_ABba(T::G, T::H, "+G+H-H-G:(KEY_PAUSE) ()");
		test_ABba(T::G, T::M, "+G+M-M-G:(KEY_MUTE) ()");
		test_ABba(T::G, T::COMMA, "+G+COMMA-COMMA-G:(KEY_VOL_DN) ()");
		test_ABba(T::G, T::PERIOD, "+G+PERIOD-PERIOD-G:(KEY_VOL_UP) ()");
	}

	void test_numpad_plane() {
		test_numpad_press(T::Q, "numpadQ:(KEY_NUM_LOCK) ()");
		test_numpad_press(T::A, "numpadA:(KEY_CAPS_LOCK) ()");
		test_numpad_press(T::Z, "numpadZ:(KEY_SCROLL_LOCK) ()");
		test_numpad_press(T::X, "numpadX:(KEY_F1) ()");
		test_numpad_press(T::C, "numpadC:(KEY_F2) ()");
		test_numpad_press(T::V, "numpadV:(KEY_F3) ()");
		test_numpad_press(T::S, "numpadS:(KEY_F4) ()");
		test_numpad_press(T::D, "numpadD:(KEY_F5) ()");
		test_numpad_press(T::F, "numpadF:(KEY_F6) ()");
		test_numpad_press(T::W, "numpadW:(KEY_F7) ()");
		test_numpad_press(T::E, "numpadE:(KEY_F8) ()");
		test_numpad_press(T::R, "numpadR:(KEY_F9) ()");
		test_numpad_press(T::T, "numpadT:(KEY_F10) ()");
		test_numpad_press(T::G, "numpadG:(KEY_F11) ()");
		test_numpad_press(T::B, "numpadB:(KEY_F12) ()");

		test_numpad_press(T::Y, "numpadY:(KEYPAD_ASTERIX) ()");
		test_numpad_press(T::H, "numpadH:(KEYPAD_PLUS) ()");
		test_numpad_press(T::N, "numpadN:(KEYPAD_0) ()");
		test_numpad_press(T::M, "numpadM:(KEYPAD_1) ()");
		test_numpad_press(T::COMMA, "numpadCOMMA:(KEYPAD_2) ()");
		test_numpad_press(T::PERIOD, "numpadPERIOD:(KEYPAD_3) ()");
		test_numpad_press(T::J, "numpadJ:(KEYPAD_4) ()");
		test_numpad_press(T::K, "numpadK:(KEYPAD_5) ()");
		test_numpad_press(T::L, "numpadL:(KEYPAD_6) ()");
		test_numpad_press(T::U, "numpadU:(KEYPAD_7) ()");
		test_numpad_press(T::I, "numpadI:(KEYPAD_8) ()");
		test_numpad_press(T::O, "numpadO:(KEYPAD_9) ()");
		test_numpad_press(T::P, "numpadP:(KEYPAD_SLASH) ()");
		test_numpad_press(T::COLON, "numpadCOLON:(KEYPAD_MINUS) ()");
		test_numpad_press(T::SLASH, "numpadSLASH:(KEYPAD_PERIOD) ()");

		test("F sequence");
		on(T::SPACE); on(T::ENTER);
		on(T::Q); off(T::Q);
		on(T::X); off(T::X);
		on(T::D); on(T::R);
		off(T::D); off(T::R);
		on(T::G); off(T::G);
		off(T::SPACE); off(T::ENTER);
		check("F sequence:(KEY_NUM_LOCK) () (KEY_F1) () (KEY_F5) () (KEY_F9) () (KEY_F11) ()");
	}
	void test_mouse() {
		test("wheel");
		move(0, 64);
		check("wheel:[0 0 -2]");

		test("ctrl+shift+wheel");
		on(T::A);
		on(T::F);
		move(0, 64);
		off(T::A);
		off(T::F);
		check("ctrl+shift+wheel:(LEFT_CTRL LEFT_SHIFT) [0 0 -2] (LEFT_CTRL) ()");

		test("mouse move");
		on(T::O);
		move(10, -10);
		off(T::O);
		check("mouse move:[-10 -10 0]");

		test("ctrl+mouse move");
		on(T::O); on(T::F);
		move(10, -10);
		off(T::O); off(T::F);
		check("ctrl+mouse move:(LEFT_CTRL) [-10 -10 0] ()");

		test("click-lb");
		on(T::W); 
		move(1, 0);		
		on(T::U); off(T::U);
		off(T::W);
		check("click-lb:[-1 0 0] [L0 0 0] [0 0 0]");

		test("click-lb-reversed");
		on(T::O); 
		move(1, 0);
		on(T::U);
		off(T::O); off(T::U); 
		check("click-lb-reversed:[-1 0 0] [L0 0 0] [0 0 0]");

		test("drag-rb-with-shift");
		on(T::O); move(10, -10);
		on(T::I); move(11, 11);
		on(T::A); move(-12, 12);
		off(T::A); move(-13, -13);
		off(T::I); move(14, 14);
		off(T::O);
		check("drag-rb-with-shift:"
			"[-10 -10 0] [R0 0 0] [R-11 11 0] "
			"(LEFT_SHIFT) [R12 12 0] () [R13 -13 0] "
			"[0 0 0] [-14 14 0]");

		test("drag-rb-with-shift-reverse");
		on(T::O); move(11, 11);
		on(T::I); 
		on(T::A); move(-12, 12);
		off(T::O);
		off(T::A); move(-32, -32);
		off(T::I); move(64, 64);
		check("drag-rb-with-shift-reverse:[-11 11 0] [R0 0 0] (LEFT_SHIFT) [R12 12 0] () [R0 0 1] [0 0 0] [0 0 -2]");

		test("kbd to mouse");
		on(T::O); off(T::O);
		on(T::SPACE);
		move(1,1);
		on(T::A);
		off(T::SPACE);
		off(T::A);
		check("kbd to mouse:(KEY_O) () [-1 1 0] (KEY_A) ()");
	}

	void test_autorepeat() {
		test("autorepeat");
		on(T::Q); off(T::Q);
		on(T::Q); loop_step(); loop_step(); loop_step(); off(T::Q);
		check("autorepeat:(KEY_Q) () (KEY_Q) () (KEY_Q) ()");

		test("shift+autorepeat");
		on(T::COLON);
		on(T::Q); off(T::Q);
		on(T::Q); loop_step(); loop_step(); loop_step(); off(T::Q);
		off(T::COLON);
		check("shift+autorepeat:(LEFT_SHIFT KEY_Q) (LEFT_SHIFT) (LEFT_SHIFT KEY_Q) (LEFT_SHIFT) (LEFT_SHIFT KEY_Q) (LEFT_SHIFT) ()");

		test("unshift during autorepeat");
		on(T::COLON);
		on(T::Q); off(T::Q);
		on(T::Q); loop_step(); loop_step(); loop_step(); loop_step(); loop_step(); loop_step(); 
		off(T::COLON);
		loop_step(); loop_step(); loop_step(); off(T::Q);
		check("unshift during autorepeat:(LEFT_SHIFT KEY_Q) (LEFT_SHIFT) (LEFT_SHIFT KEY_Q) (LEFT_SHIFT) () (KEY_Q) () (KEY_Q) () (KEY_Q) ()");
	}
};

sbyte __cdecl usb_keyboard_press(byte key) {
	keyboard_keys[0] = key;
	usb_keyboard_send();
	if (key) {
		keyboard_keys[0] = 0;
		usb_keyboard_send();
	}
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
		acc << "R";
	if (mouse_buttons & 4)
		acc << "M";
	acc << int(-dx) << " " << int(-dy) << " " << int(wheel) << "] ";
	return 1;
}

void __cdecl adns_motion(sword *dx,sword *dy) {
	*dx = next_dx;
	*dy = next_dy;
	next_dx = next_dy = 0;
}

void __cdecl print(char *){}
void __cdecl phex16(sword){}
void __cdecl phex(byte){}

int main() {
	test("reflash", &T::Q);
	check("reflash:reflash");

	test("{}");
	on(T::A);on(T::SPACE);
	on(T::COMMA); off(T::COMMA);
	on(T::PERIOD); off(T::PERIOD);
	off(T::A);
	on(T::J); off(T::J);
	off(T::SPACE);
	on(T::ENTER);
	off(T::ENTER);
	check("{}:(LEFT_SHIFT KEY_LEFT_BRACE) (LEFT_SHIFT) (LEFT_SHIFT KEY_RIGHT_BRACE) (LEFT_SHIFT) () "
		"(KEY_LEFT) () (KEY_ENTER) ()");

	test_main_plane();
	test_side_planes();
	test_numpad_plane();
	test_mouse();
	test_autorepeat();

	test("game mode");
	on(T::G); on(T::SLASH);
	off(T::SLASH); off(T::G);
	on(T::E);
	on(T::F);
	on(T::SPACE);
	on(T::I);
	move(10, 20);
	off(T::I);
	off(T::SPACE);
	off(T::E);
	off(T::F);
	on(T::SLASH); off(T::SLASH);
	check("");

	printf("%s\n", has_errors ? "failed" : "Ok");
	return has_errors;
}

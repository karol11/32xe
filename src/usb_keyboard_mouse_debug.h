#ifndef usb_serial_h__
#define usb_serial_h__

#include <stdint.h>

void usb_init(void);			// initialize everything
uint8_t usb_configured(void);		// is the USB port configured

int8_t usb_keyboard_send(void);
int8_t usb_mouse_move(int8_t x, int8_t y, int8_t wheel);
extern uint8_t mouse_buttons;

int8_t usb_debug_putchar(uint8_t c);	// transmit a character
void usb_debug_flush_output(void);	// immediately transmit any buffered output

extern volatile uint8_t keyboard_leds;
#define KLED_NUM_LOCK 1
#define KLED_CAPS_LOCK 2
#define KLED_SCROLL_LOCK 4
#define KLED_COMPOSE 8
#define KLED_KANA 16

#define USB_DEBUG_HID

extern uint8_t keyboard_modifier_keys;
#define KEY_CTRL	0x01
#define KEY_SHIFT	0x02
#define KEY_ALT		0x04
#define KEY_GUI		0x08
#define KEY_LEFT_CTRL	0x01
#define KEY_LEFT_SHIFT	0x02
#define KEY_LEFT_ALT	0x04
#define KEY_LEFT_GUI	0x08
#define KEY_RIGHT_CTRL	0x10
#define KEY_RIGHT_SHIFT	0x20
#define KEY_RIGHT_ALT	0x40
#define KEY_RIGHT_GUI	0x80

int8_t usb_keyboard_press(uint8_t key);
extern uint8_t keyboard_keys[6];
#define KEY_A		4
#define KEY_B		5
#define KEY_C		6
#define KEY_D		7
#define KEY_E		8
#define KEY_F		9
#define KEY_G		10
#define KEY_H		11
#define KEY_I		12
#define KEY_J		13
#define KEY_K		14
#define KEY_L		15
#define KEY_M		16
#define KEY_N		17
#define KEY_O		18
#define KEY_P		19
#define KEY_Q		20
#define KEY_R		21
#define KEY_S		22
#define KEY_T		23
#define KEY_U		24
#define KEY_V		25
#define KEY_W		26
#define KEY_X		27
#define KEY_Y		28
#define KEY_Z		29
#define KEY_1		30
#define KEY_2		31
#define KEY_3		32
#define KEY_4		33
#define KEY_5		34
#define KEY_6		35
#define KEY_7		36
#define KEY_8		37
#define KEY_9		38
#define KEY_0		39
#define KEY_ENTER	40	
#define KEY_ESC		41
#define KEY_BACKSPACE	42	
#define KEY_TAB		43
#define KEY_SPACE	44	
#define KEY_MINUS	45
#define KEY_EQUAL	46
#define KEY_LEFT_BRACE	47
#define KEY_RIGHT_BRACE	48
#define KEY_BACKSLASH	49
//#define KEY_NUMBER	50 // NON_US
#define KEY_SEMICOLON	51
#define KEY_QUOTE	52
#define KEY_TILDE	53
#define KEY_COMMA	54
#define KEY_PERIOD	55
#define KEY_SLASH	56
#define KEY_CAPS_LOCK	57
#define KEY_F1		58
#define KEY_F2		59
#define KEY_F3		60
#define KEY_F4		61
#define KEY_F5		62
#define KEY_F6		63
#define KEY_F7		64
#define KEY_F8		65
#define KEY_F9		66
#define KEY_F10		67
#define KEY_F11		68
#define KEY_F12		69
#define KEY_PRINTSCREEN	70		
#define KEY_SCROLL_LOCK	71		
#define KEY_PAUSE	72
#define KEY_INSERT	73	
#define KEY_HOME	74
#define KEY_PAGE_UP	75
#define KEY_DELETE	76
#define KEY_END		77
#define KEY_PAGE_DOWN	78	
#define KEY_RIGHT	79
#define KEY_LEFT	80
#define KEY_DOWN	81
#define KEY_UP		82
#define KEY_NUM_LOCK	83
#define KEYPAD_SLASH	84	
#define KEYPAD_ASTERIX	85	
#define KEYPAD_MINUS	86	
#define KEYPAD_PLUS	87	
#define KEYPAD_ENTER	88	
#define KEYPAD_1	89
#define KEYPAD_2	90
#define KEYPAD_3	91	
#define KEYPAD_4	92	
#define KEYPAD_5	93	
#define KEYPAD_6	94	
#define KEYPAD_7	95	
#define KEYPAD_8	96	
#define KEYPAD_9	97	
#define KEYPAD_0	98		
#define KEYPAD_PERIOD	99		
#define KEY_MENU	0x76
#define KEY_MUTE	0x7f
#define KEY_VOL_UP	0x80
#define KEY_VOL_DN	0x81

#endif

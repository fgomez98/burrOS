#ifndef SCAN_CODES_H
#define SCAN_CODES_H

#define RSHIFT       (char)0x36
#define RSHIFT_R     (char)0xB6
#define LSHIFT       (char)0x2A
#define LSHIFT_R     (char)0xAA
#define CTRL         (char)0x1D
#define CTRL_R       (char)0x9D
#define ALT          (char)0x38
#define ALT_R        (char)0xB8
#define CAPS         (char)0x3A
#define CAPS_R       (char)0xBA
#define UP_ARROW	 (char)0x48
#define UP_ARROW_R	 (char)0xC8
#define	DOWN_ARROW	 (char)0x50
#define DOWN_ARROW_R (char)0xD0
#define LEFT_ARROW	 (char)0x4B
#define LEFT_ARROW_R (char)0xCB
#define RIGHT_ARROW	 (char)0x4D
#define RIGHT_ARROW_R (char)0xCD
#define ESC			 (char)0x01
#define ESC_R		 (char)0x81
#define F1			 (char)0x3B
#define F1_R		 (char)0xBB
#define F2			 (char)0x3C
#define F2_R		 (char)0xBC
#define F3			 (char)0x3D
#define F3_R		 (char)0xBD
#define F4			 (char)0x3E
#define F4_R		 (char)0xBE
#define F5			 (char)0x3F
#define F5_R		 (char)0xBF
#define F6			 (char)0x40
#define F6_R		 (char)0xC0
#define F7			 (char)0x41
#define F7_R		 (char)0xC1
#define F8			 (char)0x42
#define F8_R		 (char)0xC2
#define F9			 (char)0x43
#define F9_R		 (char)0xC3
#define F10			 (char)0x44
#define F10_R		 (char)0xC4
#define F11			 (char)0x57
#define F11_R		 (char)0xD7
#define F12			 (char)0x58
#define F12_R		 (char)0xD8
#define TAB			 (char)0x0F
#define TAB_R		 (char)0x8F
#define BACKSPACE    (char)0x0E

char getAsciiCode(int scan);
char getAsciiShiftCode(int scan);

#endif
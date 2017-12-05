/*
 * Copyright:      Fabian Maximilian Thiele  mailto:me@apetech.de
 * Author:         Fabian Maximilian Thiele
 * Remarks:        this Copyright must be included
 * known Problems: none
 * Version:        1.1
 * Description:    Graphic Library for KS0108- (and compatible) based LCDs
 * 
 */

#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

#ifndef	KS0108_H
#define KS0108_H

// Ports
#define LCD_CMD_PORT		PORTD		// Command Output Register
#define LCD_CMD_DIR			DDRD		// Data Direction Register for Command Port

#define LCD_DATA_IN			PINA		// Data Input Register
#define LCD_DATA_OUT		PORTA		// Data Output Register
#define LCD_DATA_DIR		DDRA		// Data Direction Register for Data Port

// Command Port Bits
#define CSEL1				0x00		// CS1 Bit Number
#define CSEL2				0x01		// CS2 Bit Number
#define RES         		0x03    // RES Bit Number
#define D_I					0x04		// D/I Bit Number
#define R_W					0x06		// R/W Bit Number
#define EN					0x07		// EN Bit Number

// Chips
#define CHIP1				0x00
#define CHIP2				0x01

// Commands - zde doplnit konstanty:
#define LCD_ON				0x3f
#define LCD_OFF				0x3e
#define LCD_SET_ADD			0x40
#define LCD_SET_PAGE		0xb8
#define LCD_DISP_START		0xc0

// InitStatus
#define NON_INVERTED    0x00
#define INVERTED        0x01

// Colors
#define BLACK				0xFF
#define WHITE				0x00



typedef struct {
	uint8_t x;
	uint8_t y;
	uint8_t page;
} lcdCoord;

typedef uint8_t (*ks0108FontCallback)(const uint8_t*);

//
// Function Prototypes
//


void ks0108SetDot(uint8_t x, uint8_t y, uint8_t color);

void ks0108ClearScreen(void); 

#define pixel(x, y, color) {ks0108SetDot(x, y, color);} //prodano pro kompatibility s lcdavr



// Control Functions
void ks0108SetReset(uint8_t value);
void ks0108GotoXY(uint8_t x, uint8_t y);
void ks0108Init(uint8_t invert);
inline uint8_t ks0108ReadData(void);
void ks0108WriteCommand(uint8_t cmd, uint8_t chip);
void ks0108WriteData(uint8_t data);

#endif

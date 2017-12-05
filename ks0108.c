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
#include "ks0108.h"


lcdCoord			ks0108Coord;
uint8_t				ks0108Inverted=0;
ks0108FontCallback	ks0108FontRead;
uint8_t				ks0108FontColor;
const uint8_t*		ks0108Font;
uint8_t i,j;


void ks0108ClearScreen() {

	uint8_t x = 0;
	uint8_t y = 0;
	uint8_t width = 127;
	uint8_t height = 63;
	uint8_t color = WHITE;

	uint8_t mask, pageOffset, h, i, data;
	height++;

	pageOffset = 0;
	y -= pageOffset;//0
	mask = 0xFF;

		h = 8-pageOffset;

	ks0108GotoXY(x, y);
	for(i=0; i<=width; i++) {


		data = 0x00;

		ks0108WriteData(data);
	}

	while(h+8 <= height) {
		h += 8;
		y += 8;
		ks0108GotoXY(x, y);

		for(i=0; i<=width; i++) {
			ks0108WriteData(color);
		}
	}


}

void ks0108SetDot(uint8_t x, uint8_t y, uint8_t color) {
	uint8_t data;

	ks0108GotoXY(x, y-y%8);					// read data from display memory
	data = ks0108ReadData();

	if(color == WHITE) {
		data &= ~(0x01 << (y%8));			// clear dot
	} else {
		data |= 0x01 << (y%8);				// set dot
	}

	ks0108WriteData(data);					// write data back to display
}



void ks0108SetReset(uint8_t value) {
  LCD_CMD_DIR = 0xFF;						  // set port as OUTPUT

  if (value == 1)
  {
     LCD_CMD_PORT |= 0x01 << RES;		      // set RES signal to HIGN
  }
    else
  {
     LCD_CMD_PORT &= ~(0x00 << RES);		  // set RES signal to LOW
  }
}


void ks0108GotoXY(uint8_t x, uint8_t y) {
	uint8_t chip = CHIP1, cmd;

	if(x > 127) x = 0;								// ensure that coordinates are legal
	if(y > 63)  y = 0;

	ks0108Coord.x = x;								// save new coordinates
	ks0108Coord.y = y;
	ks0108Coord.page = y/8;

	if(x >= 64) {									    // select the right chip
		x -= 64;
		chip = CHIP2;
	}
	cmd = LCD_SET_ADD | x;
	ks0108WriteCommand(cmd, chip);					  // set x address on active chip

	cmd = LCD_SET_PAGE | ks0108Coord.page;		// set y address on both chips
	ks0108WriteCommand(cmd, CHIP1);
	ks0108WriteCommand(cmd, CHIP2);
}

void ks0108Init(uint8_t invert) {
	ks0108Coord.x = 0;
	ks0108Coord.y = 0;
	ks0108Coord.page = 0;

	ks0108Inverted = invert;

	LCD_CMD_DIR = 0xFF;								           // command port is output

	ks0108WriteCommand(LCD_ON, CHIP1);				   // power on
	ks0108WriteCommand(LCD_ON, CHIP2);

	ks0108WriteCommand(LCD_DISP_START, CHIP1);	 // display start line = 0
	ks0108WriteCommand(LCD_DISP_START, CHIP2);
	ks0108ClearScreen();							           // display clear
	ks0108GotoXY(0,0);
}

inline void ks0108Enable(void) {
	LCD_CMD_PORT |= 0x01 << EN;						       // EN high level width: min. 450ns
	asm volatile("nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 ::);
	LCD_CMD_PORT &= ~(0x01 << EN);
	for(i=0; i<100; i++);			   // a little delay loop (faster than reading the busy flag)
}

uint8_t ks0108DoReadData(uint8_t first) {
	uint8_t data;
	volatile uint8_t i;

	LCD_DATA_OUT = 0x00;
	LCD_DATA_DIR = 0x00;							      // data port is input

	if(ks0108Coord.x < 64) {
		LCD_CMD_PORT &= ~(0x01 << CSEL2);			     // deselect chip 2
		LCD_CMD_PORT |= 0x01 << CSEL1;				     // select chip 1
	} else if(ks0108Coord.x >= 64) {
		LCD_CMD_PORT &= ~(0x01 << CSEL1);			     // deselect chip 1
		LCD_CMD_PORT |= 0x01 << CSEL2;				     // select chip 2
	}
	if(ks0108Coord.x == 64 && first) {				   // chip2 X-address = 0
		ks0108WriteCommand(LCD_SET_ADD, CHIP2); 	 // wuff wuff
	}

	LCD_CMD_PORT |= 0x01 << D_I;					// D/I = 1
	LCD_CMD_PORT |= 0x01 << R_W;					// R/W = 1

	LCD_CMD_PORT |= 0x01 << EN;						// EN high level width: min. 450ns
	asm volatile("nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 ::);

	data = LCD_DATA_IN;								    // read Data

	LCD_CMD_PORT &= ~(0x01 << EN);
	for(i=0; i<100; i++);								    // a little delay loop (faster than reading the busy flag)

	LCD_DATA_DIR = 0xFF;

	ks0108GotoXY(ks0108Coord.x, ks0108Coord.y);

	if(ks0108Inverted)
		data = ~data;
	return data;
}

inline uint8_t ks0108ReadData(void) {
	ks0108DoReadData(1);							// dummy read
	return ks0108DoReadData(0);				// "real" read
}

void ks0108WriteCommand(uint8_t cmd, uint8_t chip) {
	if(chip == CHIP1) {
		LCD_CMD_PORT &= ~(0x01 << CSEL2);			// deselect chip 2
		LCD_CMD_PORT |= 0x01 << CSEL1;				// select chip 1
	} else if(chip == CHIP2) {
		LCD_CMD_PORT &= ~(0x01 << CSEL1);			// deselect chip 1
		LCD_CMD_PORT |= 0x01 << CSEL2;				// select chip 2
	}

	LCD_CMD_PORT &= ~(0x01 << D_I);					// D/I = 0
	LCD_CMD_PORT &= ~(0x01 << R_W);					// R/W = 0
	LCD_DATA_DIR = 0xFF;							      // data port is output
	LCD_DATA_OUT = cmd;								      // write command
	ks0108Enable();									        // enable
	LCD_DATA_OUT = 0x00;
}

void ks0108WriteData(uint8_t data) {
	uint8_t displayData, yOffset, cmdPort;

#ifdef DEBUG
	volatile uint16_t i;
	for(i=0; i<5000; i++);
#endif

	if(ks0108Coord.x >= 128)
		return;

	if(ks0108Coord.x < 64) {
		LCD_CMD_PORT &= ~(0x01 << CSEL2);			// deselect chip 2
		LCD_CMD_PORT |= 0x01 << CSEL1;				// select chip 1
	} else if(ks0108Coord.x >= 64) {
		LCD_CMD_PORT &= ~(0x01 << CSEL1);			// deselect chip 1
		LCD_CMD_PORT |= 0x01 << CSEL2;				// select chip 2
	}
	if(ks0108Coord.x == 64)							    // chip2 X-address = 0
		ks0108WriteCommand(LCD_SET_ADD, CHIP2);

	LCD_CMD_PORT |= 0x01 << D_I;					  // D/I = 1
	LCD_CMD_PORT &= ~(0x01 << R_W);					// R/W = 0
	LCD_DATA_DIR = 0xFF;							      // data port is output


	yOffset = ks0108Coord.y%8;
	if(yOffset != 0) {
		// first page
		cmdPort = LCD_CMD_PORT;						    // save command port
		displayData = ks0108ReadData();

		LCD_CMD_PORT = cmdPort;						    // restore command port
		LCD_DATA_DIR = 0xFF;						      // data port is output

		displayData |= data << yOffset;
		if(ks0108Inverted)
			displayData = ~displayData;
		LCD_DATA_OUT = displayData;					  // write data
		ks0108Enable();								        // enable

		// second page
		ks0108GotoXY(ks0108Coord.x, ks0108Coord.y+8);

		displayData = ks0108ReadData();

		LCD_CMD_PORT = cmdPort;						    // restore command port
		LCD_DATA_DIR = 0xFF;						      // data port is output

		displayData |= data >> (8-yOffset);
		if(ks0108Inverted)
			displayData = ~displayData;
		LCD_DATA_OUT = displayData;					  // write data
		ks0108Enable();								        // enable

		ks0108GotoXY(ks0108Coord.x+1, ks0108Coord.y-8);
	} else {
		if(ks0108Inverted)
			data = ~data;
		LCD_DATA_OUT = data;						      // write data
		ks0108Enable();								        // enable
		ks0108Coord.x++;
	}
	LCD_DATA_OUT = 0x00;
}

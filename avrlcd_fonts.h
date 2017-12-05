/*

	DEMO - demostrace pouziti knihovny AVRLCD a jejich rozsirujicich modulu
	
	Program:   15432 bytes (94.2% Full)
	Data:        622 bytes (60.7% Full)


*/

#ifndef __PGMSPACE_H_
#include <avr/pgmspace.h>
#endif

#ifndef _AVRLCD_
#include "avrlcd.h"			// AVR LCD knihovna
#endif

#ifndef _AVRLCD_FONTS_
#define _AVRLCD_FONTS_ 1
#endif



#define		DT_NORMAL 		0x00
#define		DT_INVERT		0x01
#define		DT_TRANSPARENT	0x02
#define		DT_FILLRECT		0x04
#define		DT_NOCLIP		0x08
#define		DT_FOCUSED		0x10

// ukazatel na aktualni font
unsigned char *current_font;


// zjisteni vysky prave pouziteho fontu
unsigned char get_char_height(){	
	if (current_font == 0) return 0;

	unsigned char data = pgm_read_byte(&current_font[0]);
	return data;	
}

unsigned char get_char_width(unsigned char ch){
	if (current_font == 0) return 0;

	unsigned char data = pgm_read_byte(&current_font[ch-32+1]);
	return data;	
}


// vykresleni znaku v grafice a vraci sirku kresleneho textu
unsigned char draw_char(short int x, short int y, unsigned char ch, unsigned char tags){

	if ((ch < 32)||(current_font == 0)) return 0;
	
	unsigned char w = get_char_width(ch);
	unsigned char h = get_char_height();
	unsigned char ax, ay;
	unsigned char data;
	unsigned int offset = 225;	

	// zjisteni zacatku pozadovaneho znaku
	for (ax = 32; ax < ch; ax++){
		data = pgm_read_byte(&current_font[ax-32+1]);
		offset = offset + (((data-1)/8) + 1) * h;
	}


	for (ay = 0; ay < h; ay++){
		data = pgm_read_byte(&current_font[offset]);
		for (ax = 0; ax < w; ax++){
			if (((ax%8)==0)&&(ax != 0)){
				data = pgm_read_byte(&current_font[++offset]);			
			}
			if ((tags&DT_INVERT) == DT_INVERT){
				if (bit_is_set(data, 7 - ax%8)) pixel(x + ax, y + ay, 0);							
			}else{
				if (bit_is_set(data, 7 - ax%8)) pixel(x + ax, y + ay, 1);							
			}
		}
		offset++;
	}

	return w;
				
}


unsigned short int get_text_width(char *text){

	unsigned short int tmp1 = 0;
	unsigned short int tmp2 = 0;
	short int i = 0;	
	while (text[i]) {
		switch (text[i]){
			// novy radek
			case 10:
				tmp1 = 0;				
			break;
			// tab
			case 9:
				tmp1 = tmp1 + 2*get_char_width(' ');				
			break;			
			default:
				tmp1 = tmp1 + get_char_width(text[i]);
		}	
		i++;
		if (tmp1 > tmp2) tmp2 = tmp1;
	}

	return tmp1;
}

unsigned short int get_text_height(char *text){
	short int i = 0;	
	unsigned short int tmp = 1;
	while (text[i]) {
		if (text[i] == 10) tmp++;
		i++;
	}

	return tmp * get_char_height();

}


void draw_text_ex(short int x, short int y, short int width, short int height, short int offset_x, short int offset_y, char* text, unsigned char tags){
	// zapamatovani kresliciho vyrezu
	
	short int _viewport_top = viewport_top;
	short int _viewport_left = viewport_left;
	short int _viewport_right = viewport_right;
	short int _viewport_bottom = viewport_bottom;

	if ((tags&DT_NOCLIP) != DT_NOCLIP){
	
		// omezeni kresliciho vyrezu
		viewport_top = y;
		viewport_left = x;
		viewport_right = x + width;
		viewport_bottom = y + height;	
	}

	unsigned char bk_color = ((tags&DT_INVERT) == DT_INVERT)?1:0;

	// vymazani pozadi pokud neni pouzit tag DT_TRANSPARENT
	if ((tags&DT_TRANSPARENT) != DT_TRANSPARENT) fillrect(x, y, x + width, y + height, bk_color, bk_color);
	
	short int i = 0;
	short int ax = 0;
	short int ay = 0;

	while (text[i]) {
		switch (text[i]){
			// novy radek
			case 10:
				ax = 0; 
				ay = ay + get_char_height();
			break;
			// tab
			case 9:
				ax = ax + get_char_width(' ');
			break;
			
			default:
				ax = ax + draw_char(x + ax + offset_x, y + ay + offset_y, text[i], tags); 
		}	
		i++;
	}

	if ((tags&DT_FOCUSED) == DT_FOCUSED)
		rectangle(x + 1, y + 1, x + width - 1, y + height - 1, CL_DOTED);


	if ((tags&DT_NOCLIP) != DT_NOCLIP){
		// obnoveni kreslicho vyrezu
		viewport_top = _viewport_top;
		viewport_left = _viewport_left;
		viewport_right = _viewport_right;
		viewport_bottom = _viewport_bottom;
	}
}

void draw_text(short int x, short int y, short int width, short int height, char* text, unsigned char tags){
	draw_text_ex(x, y, width, height, 0, 0, text, tags);
}



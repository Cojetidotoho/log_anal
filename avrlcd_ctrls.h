/*

	avrlcd_ctrls.h - rozsirujici knihovna pro kresleni ovladacih prvku

*/

#ifndef _AVRLCD_
#include "avrlcd.h"			// AVR LCD knihovna
#endif

#ifndef _AVRLCD_FONTS_
#include "avrlcd_fonts.h"	// knihovna pro praci s bitmap fonty
#endif


#ifndef _AVRLCD_CTRLS_
#define _AVRLCD_CTRLS_ 1
#endif

/* definice stavu ovladacich prvku */
#define		CTRL_NORMAL		0x00
#define		CTRL_FOCUSED	0x01
#define		CTRL_CHECKED	0x02
#define		CTRL_DEFAULT	0x04
#define		CTRL_PRESSED	0x08
#define		CTRL_HORIZONTAL	0x10
#define		CTRL_MENU		0x20


/*
void ctrl_button(short int x, short int y, short int width, nshort int height, char* caption, unsigned char tags){

  - tlacitko
  - vstupni parametry
    x, y - souradnice levho horniho rohu
    widht - sirka prvku
    height - vyska prvku
    caption - ukazatel na text prvku
    tags - rizeni stavu prvku

*/
void ctrl_button(short int x, short int y, short int width, short int height, char* caption, unsigned char tags){

	char pressed =  ((tags&CTRL_PRESSED) == CTRL_PRESSED)?1:0;

	rectangle(x + pressed, y + pressed, x + width + pressed, y + height + pressed, 1);

	draw_text_ex(x + pressed + 1, y + pressed + 1, width - 2, height - 2, width/2 - get_text_width(caption)/2 , height/2 - get_text_height(caption)/2, caption, (((tags&CTRL_FOCUSED) == CTRL_FOCUSED)?DT_FOCUSED:DT_NORMAL));

	unsigned short int i;
	if (!pressed){
		// stin
		for (i = x + 1; i <= x + width + 1; i++) pixel(i, y + height + 1, 1);
		for (i = y + 1; i <= y + height; i++) pixel(x + width + 1, i, 1);
	}else{
		// mazani okraje tlacitka
		for (i = x ; i <= x + width; i++) pixel(i, y, CL_WHITE);
		for (i = y + 1; i <= y + height; i++) pixel(x, i, CL_WHITE);

	}

}


/*
void ctrl_radio(short int x, short int y, short int width, short int height, char *caption, unsigned char tags)

  - zatrhavici pole radiobutton
  - vstupni parametry
    x, y - souradnice levho horniho rohu
    widht - sirka prvku
    height - vyska prvku
    caption - ukazatel na text prvku
    tags - rizeni stavu prvku
*/
void ctrl_radio(short int x, short int y, short int width, short int height, char *caption, unsigned char tags){

	unsigned char h = get_text_height(caption);

	char i;

	draw_text_ex(x + 5, y, width, height, 2, height/2 - h/2, caption, (((tags&CTRL_FOCUSED) == CTRL_FOCUSED)?DT_FOCUSED:DT_NORMAL));


	for (i = 1; i<5; i++){
		pixel(x + i, y + height/2 - 2, CL_BLACK);
		pixel(x + i, y + 5 + height/2 - 2, CL_BLACK);

		pixel(x, y + i + height/2 - 2, CL_BLACK);
		pixel(x + 5, y + i + height/2 - 2, CL_BLACK);
	}


	unsigned char color = ((tags&CTRL_CHECKED) == CTRL_CHECKED)?CL_BLACK:CL_WHITE 	;

	pixel(x + 2, y + 2 + height/2 - 2, color);
	pixel(x + 3, y + 2 + height/2 - 2, color);
	pixel(x + 2, y + 3 + height/2 - 2, color);
	pixel(x + 3, y + 3 + height/2 - 2, color);




}

/*
void ctrl_checkbox(short int x, short int y, short int width, short int height, char *caption, unsigned char tags)

  - zatrhavici pole checkbox
  - vstupni parametry
    x, y - souradnice levho horniho rohu
    widht - sirka prvku
    height - vyska prvku
    caption - ukazatel na text prvku
    tags - rizeni stavu prvku
*/
void ctrl_checkbox(short int x, short int y, short int width, short int height, char *caption, unsigned char tags){

	unsigned char h = get_text_height(caption);

	draw_text_ex(x + 5, y, width, height, 2, height/2 - h/2, caption, (((tags&CTRL_FOCUSED) == CTRL_FOCUSED)?DT_FOCUSED:DT_NORMAL));

	rectangle(x, y + height / 2 - 2, x + 6, y + height / 2 + 4, CL_BLACK);

	unsigned char color = ((tags&CTRL_CHECKED) == CTRL_CHECKED)?CL_BLACK:CL_WHITE 	;

	line(x + 1, y + height / 2 - 1, x + 5, y + height / 2 + 3, color);
	line(x + 5, y + height / 2 - 1, x + 1, y + height / 2 + 3, color);


}

/*
void ctrl_edit(short int x, short int y, short int width, short int height, char *text, unsigned char tags){

  - pole edit
  - vstupni parametry
    x, y - souradnice levho horniho rohu
    widht - sirka prvku
    height - vyska prvku
    text - ukazatel na text prvku
    tags - rizeni stavu prvku
*/
void ctrl_edit(short int x, short int y, short int width, short int height, char *text, unsigned char tags){

	rectangle(x, y, x + width, y + height, 1);

	draw_text(x + 1, y + 1, width - 2, height - 2, text, DT_NORMAL);

}


/*
void ctrl_scrollbar(short int x, short int y, short int width, short int height,
	short int max, short int position, unsigned char tags){

  - rolovaci lista
  - vstupni parametry
    x, y - souradnice levho horniho rohu
    widht - sirka prvku
    height - vyska prvku
    max - maximalni hodnota rolovaci listy
    position - aktualni pozice ukazatele
    tags - rizeni stavu prvku
*/
void ctrl_scrollbar(short int x, short int y, short int width, short int height,
	short int max, short int position, unsigned char tags){

	short int a, b = 0;

	if ((tags&CTRL_HORIZONTAL) == CTRL_HORIZONTAL){
		// ovladaci prvek je horizontalne otocen
		for (a = y; a <= y + height; a++){
			pixel(x, a, 1); pixel(x + 7, a, 1);
			pixel(x + width, a, 1);pixel(x + width - 7, a, 1);
		}

		for (a = 1; a < 8; a++){
			pixel(x + a, y, 1); pixel(x + width - a, y, 1);
			pixel(x + a, y + height, 1); pixel(x + width - a, y + height, 1);
		}

		// sipky
		for (a = 0; a < 5; a++){
			pixel(x + 5, y + height/2 + a - 2, 1);
			pixel(x + width - 5, y + height/2 + a - 2, 1);
		}
		pixel(x + 4, y + height/2 + 2, 1); pixel(x + 4, y + height/2 - 2, 1);
		pixel(x + 3, y + height/2 + 1, 1); pixel(x + 3, y + height/2 - 1, 1);
		pixel(x + 2, y + height/2, 1);

		pixel(x + width - 4, y + height/2 + 2, 1); pixel(x + width - 4, y + height/2 - 2, 1);
		pixel(x + width - 3, y + height/2 + 1, 1); pixel(x + width - 3, y + height/2 - 1, 1);
		pixel(x + width - 2, y + height/2, 1);

		// vypocet velikosti slideru
		short int slider_width = (width - 16) / max;
		if (slider_width < 3) slider_width = 3;

		short int slider_position = x + 8 + (float)position/max * (width - 16 - slider_width);

		// vysrafovana cast
		for (a = y; a <= y + height; a++)
			for (b = x + 8; b <= x + width - 8; b++)
				pixel(b,a,
				((b == slider_position)||(b == slider_position + slider_width))?1:
				((b > slider_position)&&(b < slider_position + slider_width))?((a == y)||(a == y + height))?1:0:
				(b+a)%2);


	}else{
		for (a = x; a <= x + width; a++){
			pixel(a, y, 1); pixel(a, y + 7, 1);
			pixel(a, y + height, 1);pixel(a, y + height - 7, 1);
		}

		for (a = 1; a < 8; a++){
			pixel(x, a + y, 1); pixel(x + width, a + y, 1);
			pixel(x, y + height - a, 1); pixel(x + width, y + height - a, 1);
		}

		// sipky
		for (a = 0; a < 5; a++){
			pixel(x + width/2 + a - 2, y + 5, 1);
			pixel(x + width/2 + a - 2, y + height - 5, 1);
		}

		pixel(x + width/2 - 2, y + 4, 1); pixel(x + width/2 + 2, y + 4, 1);
		pixel(x + width/2 - 1, y + 3, 1); pixel(x + width/2 + 1, y + 3, 1);
		pixel(x + width/2, y + 2, 1);

		pixel(x + width/2 - 2, y + height - 4, 1); pixel(x + width/2 + 2, y + height - 4, 1);
		pixel(x + width/2 - 1, y + height - 3, 1); pixel(x + width/2 + 1, y + height - 3, 1);
		pixel(x + width/2, y + height - 2, 1);

		// vypocet velikosti slideru
		short int slider_height = (height - 16) / max;
		if (slider_height < 3) slider_height = 3;

		short int slider_position = y + 8 + (float)position/max * (height - 16 - slider_height);

		// vysrafovana cast
		for (a = x; a <= x + width; a++)
			for (b = y + 8; b <= y + height - 8; b++)
				pixel(a,b,
				((b == slider_position)||(b == slider_position + slider_height))?1:
				((b > slider_position)&&(b < slider_position + slider_height))?((a == x)||(a == x + width))?1:0:
				(b+a)%2);

	}


}


/*
void ctrl_listbox(short int x, short int y, short int width, short int height, char* items, short int itemindex, unsigned char tags){

  - pole seznam
  - vstupni parametry
    x, y - souradnice levho horniho rohu
    widht - sirka prvku
    height - vyska prvku
    items - polozky seznamu oddelene znakem \n
    itemindex - aktualni pozice v seznamu
    tags - rizeni stavu prvku
*/
void ctrl_listbox(short int x, short int y, short int width, short int height, char* items, short int itemindex, unsigned char tags){

	char *tmp = "";
	unsigned char ch = get_char_height();
	unsigned char th = get_text_height(items)/ch;

	short int _viewport_top = viewport_top;
	short int _viewport_left = viewport_left;
	short int _viewport_right = viewport_right;
	short int _viewport_bottom = viewport_bottom;


	unsigned short int i = 0;
	unsigned short int a = 0;
	unsigned char line = 0;
	unsigned char sw;
	short int ay = 0;

	if ((tags&CTRL_MENU) == CTRL_MENU){
		height = (th - 1)  * ch;
		sw = 0;

		// kresleni stinu
		for (i = x + 1; i <= x + width + 1; i++) pixel(i, y + height + 1, CL_BLACK);
		for (i = y + 1; i < y + height + 1; i++) pixel(x + width + 1, i, CL_BLACK);

	}else{
		sw = 8;
		ctrl_scrollbar(x + width - sw, y, sw, height, th-2, itemindex , CTRL_NORMAL);
	}


	rectangle(x, y, x + width - sw, y + height, CL_BLACK);

	// omezeni kresliciho vyrezu
	viewport_top = y + 1;
	viewport_left = x + 1;
	viewport_right = x + width - sw + 1;
	viewport_bottom = y + height - 1;



	if ((itemindex * ch + ch) > (height - 2))
		ay = (height - 1) - (itemindex * ch + ch);

	unsigned char color = (line == itemindex)?0:1;
	unsigned char submenu = 0;

	i = 0;
	while (items[i]) {
		switch (items[i]){
			// novy radek
			case 10:
				tmp[a] = 0;
				draw_text_ex(x + 1, y + 1 + line * ch + ay, width - (sw + 2), ch, 1, 0, tmp, DT_NOCLIP | ((line == itemindex)?DT_INVERT:DT_NORMAL));
				tmp = "";

				if (((tags&CTRL_MENU) == CTRL_MENU)&&(submenu)){
					color = (line == itemindex)?0:1;
					pixel(x + width - 2, y + 1 + line * ch + ay + ch/2, color);
					pixel(x + width - 3, y + 1 + line * ch + ay + ch/2 - 1, color);
					pixel(x + width - 3, y + 1 + line * ch + ay + ch/2 + 0, color);
					pixel(x + width - 3, y + 1 + line * ch + ay + ch/2 + 1, color);
					pixel(x + width - 4, y + 1 + line * ch + ay + ch/2 - 2, color);
					pixel(x + width - 4, y + 1 + line * ch + ay + ch/2 - 1, color);
					pixel(x + width - 4, y + 1 + line * ch + ay + ch/2 + 0, color);
					pixel(x + width - 4, y + 1 + line * ch + ay + ch/2 + 1, color);
					pixel(x + width - 4, y + 1 + line * ch + ay + ch/2 + 2, color);
				}

				line++;
				a = 0; submenu = 0;
			break;
			// SUB
			case '>':
				submenu = 1;
			break;
			default:
				tmp[a] = items[i];
				a++;
		}
		i++;
	}

	viewport_top = _viewport_top;
	viewport_left = _viewport_left;
	viewport_right = _viewport_right;
	viewport_bottom = _viewport_bottom;
}

/*
void ctrl_menu(short int x, short int y, short int width, char* items, short int itemindex, unsigned char tags){

  - pole nabidky
  - vstupni parametry
    x, y - souradnice levho horniho rohu
    widht - sirka prvku
    height - vyska prvku
    items - polozky seznamu oddelene znakem \n
    itemindex - aktualni pozice v seznamu
    tags - rizeni stavu prvku
*/
void ctrl_menu(short int x, short int y, short int width, char* items, short int itemindex, unsigned char tags){
	ctrl_listbox(x, y, width, 0, items, itemindex, tags | CTRL_MENU);

}

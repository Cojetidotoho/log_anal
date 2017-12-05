/*
	avrlcd.h
*/

/* indikace pritomnosti knihovny AVRLCD v projektu */
#ifndef _AVRLCD_
#define _AVRLCD_ 1
#endif

/* makra pro nastavovani jednotlivych pinu */
#define LCD_CTR_LO(PIN)	LCD_CTR_BUS &= ~_BV(PIN)
#define LCD_CTR_HI(PIN)	LCD_CTR_BUS |=  _BV(PIN)

/* definice barev a stylu */
#define	CL_WHITE	0x00
#define	CL_BLACK	0x01
#define	CL_DOTED	0x02


/*
void line(short int x1, short int y1, short int x2, short int y2, unsigned char color){
  - kresleni cary z bodu x1, y1 do bodu x2, y2 barvou color
    za pomoci Bresenhamova algoritmu
  - vstupni parametry
    x1, y1 - souradnice pocatku
    x2, y2 - souradnice konce
    color - barva cary (CL_WHITE:CL_BLACK:CL_DOTED)
*/
void line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, unsigned char color){
	int16_t dy = y2 - y1;
	int16_t dx = x2 - x1;
	uint8_t stepx, stepy;

	if (dy < 0){
		dy = -dy; stepy = -1;
	}else{
		stepy = 1;
	}

 	if (dx < 0) {
		dx = -dx; stepx = -1;
	} else {
		stepx = 1;
	}

	dy *= 2; //<<= 1; 							// dy je  2*dy
	dx *= 2; //<<= 1; 							// dx je  2*dx

	pixel(x1, y1, color);

	if (dx > dy) {
		int P = dy - (dx >> 1);	// stejne jako 2*dy - dx
		while (x1 != x2) {
			if (P >= 0) {
				y1 += stepy;
				P -= dx; 		// stejne jako P -= 2*dx
			}
   			x1 += stepx;
   			P += dy; 				// stejne jako  P -= 2*dy
   			pixel(x1, y1, (color == CL_DOTED)?(x1+y1)%2:color);
		}
	} else {
		int P = dx - (dy >> 1);
		while (y1 != y2) {
			if (P >= 0) {
				x1 += stepx;
				P -= dy;
			}
			y1 += stepy;
			P += dx;
			pixel(x1, y1, (color == CL_DOTED)?(x1+y1)%2:color);
		}
	}
}

/*
void rectangle(short int x1, short int y1, short int x2, short int y2, unsigned char color){

  - kresleni obdelniku s hornim levy rohem v x1, y1 a dolnim pravym rohem x2, y2 a barvou color
  - vstupni parametry
    x1, y1 - souradnice leveho horniho rohu
    x2, y2 - souranice praveho dolniho rohu
    color - barva (CL_WHITE:CL_BLACK:CL_DOTED)
*/
void rectangle(short int x1, short int y1, short int x2, short int y2, unsigned char color){

	short int i;
	for (i = x1; i <= x2; i++){
		pixel(i, y1, (color == CL_DOTED)?i%2:color);
		pixel(i, y2, (color == CL_DOTED)?i%2:color);
	}

	for (i = y1; i <= y2; i++){
		pixel(x1, i, (color == CL_DOTED)?i%2:color);
		pixel(x2, i, (color == CL_DOTED)?i%2:color);
	}

}
/*
void fillrect(short int x1, short int y1, short int x2,short int y2, unsigned char pen, unsigned char brush){
	short int x, y;

  - kresleni vypneneho obdelniku s hornim levy rohem v x1, y1 a dolnim pravym rohem x2, y2 a barvou okraje pen a barvou vyplne brush
  - vstupni parametry
    x1, y1 - souradnice leveho horniho rohu
    x2, y2 - souranice praveho dolniho rohu
    pen - barva okraje(CL_WHITE:CL_BLACK)
    brush - barva vyplne(CL_WHITE:CL_BLACK)
*/
void fillrect(short int x1, short int y1, short int x2,short int y2, unsigned char pen, unsigned char brush){
	short int x, y;

	for (x = x1; x <= x2; x++)
		for (y = y1; y <= y2; y++)
			pixel(x, y, ((y == y1)||(y == y2)||(x == x1)||(x == x2))?pen:brush);

}

/*
void triangle(short int x1, short int y1, short int x2, short int y2, short int x3, short int y3, unsigned char color){

  - kresleni trojuhelniku s vrcholy x1, y1, x2, y2, x3, y3 a barvou color
  - vstupni parametry
    x1, y1 - 1. vrchol trojuhelniku
    x2, y2 - 2. vrchol trojuhelniku
    x3, y3 - 3. vrchol trojuhelniku
    color - barva (CL_WHITE:CL_BLACK:CL_DOTED)
*/
void triangle(short int x1, short int y1, short int x2, short int y2, short int x3, short int y3, unsigned char color){
	line(x1, y1, x2, y2, color);
	line(x2, y2, x3, y3, color);
	line(x3, y3, x1, y1, color);
}

/*
void moveto(short int x, short int y)

  - funkce presunujici graficky ukazatal ne urcene souradnice x, y
  - vstupni parametry
    x, y - souradnice nove pozice ukazatele
*/
//void moveto(short int x, short int y){
//	gcx = x; gcy = y;
//}

/*
void lineto(short int x, short int y, unsigned char color)

  - funkce vykreslujici caru ze souradnic grafickeho ukazatale na zvolene souradnice a
	  nasledovne presune graficky ukazatel na zadane sourdnice
  - vstupni parametry
    x, y - souradnice nove pozice ukazatele
    color - barva (CL_WHITE:CL_BLACK:CL_DOTED)
*/
//void lineto(short int x, short int y, unsigned char color){
//	line(gcx, gcy, x, y, color);
//	gcx = x; gcy = y;
//}




/*
void circle(short int x, short int y, short int r, unsigned char color)

  - funkce vykresli kruznici se stredem na zadanych souradnicich a se zadanym polomerem
  - vstupni parametry
    x, y - stred kruznice
    r - polomer kruzince
    color - barva (CL_WHITE:CL_BLACK)
*/
void circle(short int x, short int y, short int r, unsigned char color){
	short int _x = 0, _y = r, P = 1 - r, X2 = 3, Y2 = 2 * r - 2;
	while (_x <= _y){
		pixel(_x + x, _y + y, color);
		pixel(_x + x, -_y + y, color);
		pixel(-_x + x, _y + y, color);
		pixel(-_x + x, -_y + y, color);
		pixel(_y + x, _x + y, color);
		pixel(_y + x, -_x + y, color);
		pixel(-_y + x, _x + y, color);
		pixel(-_y + x, -_x + y, color);

		if (P >= 0){
			P += -Y2;
			Y2 -= 2;
			_y--;
		}

		P += X2;
		X2 += 2;
		_x++;
	}
}





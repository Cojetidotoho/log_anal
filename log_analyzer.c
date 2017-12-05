/************************************************
* log_analyzer.c
*
* Miroslav Klein, Lukas Zdrazil , Brno University of Technology
* Date updated: 5.12. 2017
*
* Target MCU: ATmega16
* Description: Logic analyzer based on MCU ATmega16 and graphic display ATM12864D with KS0108 driver
************************************************/

short int viewport_top = 0;
short int viewport_left = 0;
short int viewport_right = 127;
short int viewport_bottom = 63;

#include <avr/io.h>
#include <avr/interrupt.h>
#include "ks0108.h"
#include <util/delay.h>
#include "avrlcd.h"
#define F_CPU 16000000UL
#include "avrlcd_fonts.h"
#include "avrlcd_ctrls.h"
#include "font.h"

extern void time_loading(uint8_t);
extern void time_delay(uint8_t);

uint8_t krok=1;     //krok na ose y
uint8_t opak=1;     //opakovani

uint8_t A=0;        //LOG hodnota A
uint8_t B=0;        //LOG hodnota B
uint8_t C=0;        //LOG hodnota C
uint8_t D=0;        //LOG hodnota D

uint8_t Xpoc=15;        //pocatecni mereni souradnice x
uint8_t Xzac=15;        //hodnota x zacatku mereni
uint8_t Xkon=115;       //konec mereni souradnice x

uint8_t Load=40;        //pocatecni souradnice nacitani
uint8_t Load1=85;       //konecna souradnice nacitani

uint8_t Aoff=13;    //pocatecni umisteni signalu A v ose y
uint8_t Boff=28;    //pocatecni umisteni signalu B v ose y
uint8_t Coff=43;    //pocatecni umisteni signalu C v ose y
uint8_t Doff=58;    //pocatecni umisteni signalu D v ose y

uint8_t Aoff1=13;    //umisteni signalu A predchozi pozice
uint8_t Boff1=28;    //umisteni signalu B predchozi pozice
uint8_t Coff1=43;    //umisteni signalu C predchozi pozice
uint8_t Doff1=58;    //umisteni signalu D predchozi pozice

uint8_t Aoff2=13;    //umisteni signalu A nasledujici pozice
uint8_t Boff2=28;    //umisteni signalu B nasledujici pozice
uint8_t Coff2=43;    //umisteni signalu C nasledujici pozice
uint8_t Doff2=58;    //umisteni signalu D nasledujici pozice


int main( void )
{
    DDRA = 0xff;				//nastaveni vystupniho portu A
    DDRD = 0xff;				//nastaveni vystupniho portu D
    DDRB = 0xf0;				//nastaveni vstupniho portu B


    ks0108ClearScreen();	        // vymazání LCD
    ks0108SetReset(1);		        // reset LCD
        _delay_ms(1000);			// cekani na nabehnuti LCD
    ks0108Init(NON_INVERTED);	    // Inicializace LCD

    current_font=font13;		    //nastavení písma

    //  uvodni obrazovka
    rectangle(0, 0, 127,63, CL_BLACK);
    draw_text(10,5, 80, 20, "LOGICKY ANALYZATOR", DT_NORMAL);
    draw_text(27,20, 80, 20, "LUKAS ZDRAZIL", DT_NORMAL);
    draw_text(25,30, 80, 20, "MIROSLAV KLEIN", DT_NORMAL);
        time_delay(1);

    // loading bar
    rectangle(40, 50, 85,55, CL_BLACK);
for(Load; Load<Load1; Load++){
    line(Load, 51, Load+krok, 51, CL_BLACK);
    line(Load, 52, Load+krok, 52, CL_BLACK);
    line(Load, 53, Load+krok, 53, CL_BLACK);
    line(Load, 54, Load+krok, 54, CL_BLACK);
        time_loading(1);
}
    ks0108ClearScreen();    //smazani uvodni obrazovky


        //vykresleni obrazovky mereni
    draw_text(2, 5, 80, 20, "A", DT_NORMAL);
    draw_text(2,20, 80, 20, "B", DT_NORMAL);
    draw_text(2,35, 80, 20, "C", DT_NORMAL);
    draw_text(2,50, 80, 20, "D", DT_NORMAL);
        time_delay(1);

        //cyklus mereni
for(opak; opak>0; opak++){


    for(Xpoc; Xpoc<Xkon; Xpoc++){

        //cteni z klavesnice
    PORTB = 0b11100000;     //nastaveni klavesnice na cteni tlacitek ABCD
        _delay_ms(2);       //aby fungovalo tlacitko D
    if(bit_is_clear(PINB,0)) {
      D=7;}
    else{D=0;}

    if(bit_is_clear(PINB,1)) {
        C=7;}
    else{C=0;}

    if(bit_is_clear(PINB,2)) {
        B=7;}
    else{B=0;}

    if(bit_is_clear(PINB,3)) {
        A=7;}
    else{A=0;}
    PORTB = 0b11110000;

        //urceni vysky vykreslovani cary
    Aoff2=Aoff-A;
    Boff2=Boff-B;
    Coff2=Coff-C;
    Doff2=Doff-D;

        //vykreslovani
    fillrect(Xpoc+krok, 2, Xpoc+krok+6,61, CL_WHITE, CL_WHITE);  //mazani prechoziho prubehu
    line(Xpoc, Aoff1, Xpoc+krok, Aoff2, CL_BLACK);    //A
    line(Xpoc, Boff1, Xpoc+krok, Boff2, CL_BLACK);    //B
    line(Xpoc, Coff1, Xpoc+krok, Coff2, CL_BLACK);    //C
    line(Xpoc, Doff1, Xpoc+krok, Doff2, CL_BLACK);    //D

        //zapamatovani predeslych hodnot
    Aoff1=Aoff2;
    Boff1=Boff2;
    Coff1=Coff2;
    Doff1=Doff2;

    }

    Xpoc=Xzac;
    }
}


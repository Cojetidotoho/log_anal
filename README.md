# Overview
Logic analyzer based on MCU ATmega16 and graphic display ATM12864D with KS0108 driver.
# Requirements
This project requires MCU ATmega16, graphic display ATM12864D with KS0108 driver and AVR development board for long winter evenings made by Jiří Šebesta, BUT FEEC IREL, 2006.
# Description
A logic analyzer that reacts to the A, B, C and D buttons on the connected keyboard. Graphic display shows signals from these buttons. If the buttons are pressed, the signals jump to the logic one, if they are not, the signals remain in logic zero. When the signals reach the end of the display, it starts from the beginnig and the previous signals are deleted just before the new signals.

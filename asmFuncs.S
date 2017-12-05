; HOWTO ASM syntax in CB: https://github.com/samoylenko/codeblocks_avr_asm

; Set offset for control register addresses (NEEDED)
#define __SFR_OFFSET 0

;Defines I/O port aliases
#include <avr/io.h>


; Makes assembly functions visible in other source files
.global time_loading
.global time_delay

; User function definitions
time_loading:
    ldi     r16, 0xff
    ldi     r17, 0xff
    ldi     r18, 0x0a

loading:
    dec     r16
    brne    loading
    dec     r17
    brne    loading
    dec     r18
    brne    loading

    dec     r24
    brne    loading
    ret

time_delay:
    ldi     r19, 0xff
    ldi     r20, 0xff
    ldi     r21, 0x3f

delay:
    dec     r19
    brne    delay
    dec     r20
    brne    delay
    dec     r21
    brne    delay

    dec     r24
    brne    delay
    ret


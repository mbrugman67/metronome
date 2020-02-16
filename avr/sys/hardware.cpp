/***************************************************
 * file: hardware.cpp
 ***************************************************
 * Init functions for hardware
 **************************************************/
#include <avr/io.h>
#include <avr/wdt.h>

#include "../project.h"

/***************************************
* setupIO()
****************************************
* All I/O points
***************************************/
void setupIO(void)
{
    // set as outputs initially; these are the
    // LCD lines
    DDRC = 0xff;
    PORTC = 0x02;

    // pins 0 to 3 on portb are inputs (buttons)
    DDRB = 0x20;
    PORTB = 0xdf;

    // pins 0 and 1 are serial port
    DDRD = 0b00111100;
    PORTD = 0x00;
}

/***************************************
* setupWatchdog()
****************************************
* Doggo
***************************************/
void setupWatchdog(void)
{
    WDTCSR = BIT(WDCE);
    WDTCSR = BIT(WDP3) | BIT(WDP0);
    wdt_reset();
}

/*******************************************
* setupTimer1()
********************************************
* PWM timer, frequency of 25KHz
********************************************/
void setupTimer1(void)
{
    TCCR1A = 0;
    TCCR1C = 0;

    // count of 80, prescaler 8 means interval of .04 ms
    OCR1A = 80;

    // WGM mode CTC, prescaler of 8
    TCCR1B = (BIT(WGM12) | BIT(CS11));

    TIMSK1 = BIT(OCIE1A);
}

/***************************************
* setupTimer2()
****************************************
* one milliesecond update tick timer
***************************************/
void setupTimer2(void)
{
    // no output hardware, just internal counts
    TCCR2A = 0;
    SETBIT(TCCR2A, WGM21);  // WGM mode CTC, auto reset
    
    // prescaler = 128, count = 125
    // error = 0.0%
    OCR2A = 125;
    TCCR2B = BIT(CS22) | BIT(CS20);
    
    // enable interrupt on A
    TIMSK2 = BIT(OCIE2A);
}
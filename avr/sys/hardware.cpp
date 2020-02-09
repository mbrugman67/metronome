#include <avr/io.h>
#include <avr/wdt.h>

#include "../project.h"
#include "../lcd/lcd.h"

/***************************************
* setupIO()
****************************************
* All outputs
***************************************/
void setupIO(void)
{
    // set as outputs initially; these are the
    // LCD lines
    DDRC = 0b00111111;
    PORTC = 0x00;

    DDRB = 0b11111111;
    PORTB = 0x00;

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

    lcd* another = lcd::getInstance();
    another->writeCharAt(LINE_3, 12, 'f');
    another->writeLineAt(LINE_4, 13, "heynow");
      
}